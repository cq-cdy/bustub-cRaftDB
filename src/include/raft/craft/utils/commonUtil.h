#pragma once
#include <string>
#include <regex>
#include "unordered_map"
#include "fstream"
bool isValidIpPort(const std::string& str) {
    std::regex pattern("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\:([0-9]{1,5})$");
    if (std::regex_match(str, pattern)) {
        return true;
    }
    return false;
}
class ConfigReader {
public:
    ConfigReader(const std::string &filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                parseLine(line);
            }
            file.close();
        }
    }

    std::multimap<std::string, std::string> getMap() const {
        return configMap;
    }

private:
    std::multimap<std::string, std::string> configMap;

    void parseLine(const std::string &line) {
        std::string trimmedLine = trim(line);

        if (trimmedLine.empty() || trimmedLine[0] == '#') {
            return;
        }

        std::size_t pos = trimmedLine.find('=');
        if (pos != std::string::npos) {
            std::string key = trim(trimmedLine.substr(0, pos));
            std::string value = trim(trimmedLine.substr(pos + 1));
            configMap.emplace(key, value);
        }
    }

    static std::string trim(const std::string &str) {
        std::size_t first = str.find_first_not_of(' ');
        if (std::string::npos == first) {
            return "";
        }
        std::size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }
};

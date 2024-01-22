#pragma once
#include "regex"
#include <utility>
#include <string>
#include <limits.h>
#include <unistd.h>
#include <iostream>
std::string getExecutablePath() {
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    return std::string(result, (count > 0) ? count : 0);
}

int getDeBugId(){
    std::string exec_path = getExecutablePath();
    std::cout << "执行文件路径: " << exec_path << std::endl;
        // 正则表达式提取序号
  std::regex pattern("/dbs/([0-9]+)/");
    std::smatch match;
    int me = 0;;
    if (std::regex_search(exec_path, match, pattern) && match.size() > 1) {
        std::cout << "提取的序号是: " << match.str(1) << std::endl;
        me = std::atoi(match.str(1).c_str());
        char s[1024];
    } else {
        std::cerr << "在路径中未找到序号。" << std::endl;
        return -1;
    }
    return me;
}
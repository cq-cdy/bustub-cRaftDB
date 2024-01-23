
#pragma once

#include <string>
#include <vector>
#include "libgo/coroutine.h"
#include <libgo/context/context.h>
#include <libgo/coroutine.h>
#include <libgo/defer/defer.h>
#include <libgo/netio/unix/epoll_reactor.h>
#include "random"
#include "fstream"
#include "filesystem"
#include "spdlog/spdlog.h"
#include "raft/json.hpp"

using uint = unsigned int;
using json = nlohmann::json;
enum class STATE {
    FOLLOWER,
    CANDIDATE,
    LEADER,
};
enum class RETURN_TYPE {
    TIME_OUT,
    APPLYED,
    INSTALL_SNAPSHOT_META,
    NOT_INSTALL_SNAPSHOT_META,
    INSTALL_SNAPSHOT_META_TIMEOUT,
    INSTALL_SNAPSHOT_FILE_FAILED,
    STATE_CHANGED,
};

struct Command_ {
    std::string  content;
};

struct ApplyMsg {
    bool commandValid{};
    Command_ command{};
    int commandIndex{};
};

struct ClientResult {
    int term{};
    int index{};
    bool is_timeout{};
};
struct ServerCallResult{
    int index{};
    int term{};
    bool isLeader{};
};
static uint getElectionTimeOut(uint timeout) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(timeout, timeout*2);
    return dis(gen);

}

static void check(const std::filesystem::path &dir) {
    if (!std::filesystem::exists(dir.parent_path())) {
        std::filesystem::create_directories(dir.parent_path());
        spdlog::info("success create path:[{}]", dir.parent_path().string());
    }
    if (!std::filesystem::exists(dir)) {
        std::ofstream ofs(dir);
        if (ofs) {
            spdlog::info("success create file:[{}]", dir.string());
        } else {
            spdlog::error("error create file:[{}]", dir.string());
            exit(2);
        }
    }
}

json jsonFromString(std::string str) {
  json js;
  try {
    js = json::parse(str);

  } catch (std::exception) {
    spdlog::error("json parse error!");
    exit(1);
  }
  return js;
}

void checkJson(json &js) {
  if (js.find("clientId") == js.end()) {
    spdlog::error("not fount json [clientId]!");
    exit(1);
  }
  if (js.find("commandId") == js.end()) {
    spdlog::error("not fount json [commandId]!");
    exit(1);
  }
  if (js.find("sql") == js.end()) {
    spdlog::error("not fount json [sql]!");
    exit(1);
  }
}

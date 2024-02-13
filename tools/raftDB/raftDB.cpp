#include <iostream>
#include <string>
#include "binder/binder.h"
#include "common/bustub_instance.h"
#include "common/exception.h"
#include "common/util/string_util.h"
#include "libfort/lib/fort.hpp"
#include "linenoise/linenoise.h"
#include "utf8proc/utf8proc.h"

#include "cRaftDBserver.h"
auto GetWidthOfUtf8(const void *beg, const void *end, size_t *width) -> int {
  size_t computed_width = 0;
  utf8proc_ssize_t n;
  utf8proc_ssize_t size = static_cast<const char *>(end) - static_cast<const char *>(beg);
  auto pstring = static_cast<utf8proc_uint8_t const *>(beg);
  utf8proc_int32_t data;
  while ((n = utf8proc_iterate(pstring, size, &data)) > 0) {
    computed_width += utf8proc_charwidth(data);
    pstring += n;
    size -= n;
  }
  *width = computed_width;
  return 0;
}

#define SHELL 0


#if SHELL == 0
auto main(int arc, char **argv) -> int {
  std::thread([] { co_sched.Start(3, 6); }).detach();
  std::string dbName = "bustub.db";

  // set log level
  spdlog::set_level(spdlog::level::info);

  // set snapshot and persist path
  std::string abs_path = "/home/cdy/code/projects/bustub/tools/raftDB";
  cRaftDB db(abs_path,dbName);
  sleep(INT32_MAX);
//   c
  auto bustub = std::make_unique<bustub::BustubInstance>(dbName);
  bustub->GenerateMockTable();

  if (bustub->buffer_pool_manager_ != nullptr) {
    bustub->GenerateTestTable();
  }

  linenoiseHistorySetMaxLen(1024);
  linenoiseSetMultiLine(1);
  cRaftDB db_server(abs_path, dbName);
  co_chan<ApplyMsg> msgCh(100000);
  craft::Raft raft(&db_server, &msgCh);
  raft.launch();

  while (true) {
    auto writer = bustub::FortTableWriter();
    ApplyMsg msg;
    msgCh >> msg;
    auto sqlJson = jsonFromString(msg.command.content);
    spdlog::debug("get commit json : {}" ,sqlJson.dump());
    auto  sql = sqlJson["sql"];
    bustub->ExecuteSql(sql, writer);
    //writer 是结果集
  }
  sleep(INTMAX_MAX);
}
#else
// NOLINTNEXTLINE
auto main(int argc, char **argv) -> int {
  //     std::thread([] { co_sched.Start(0,0); }).detach();
  // go[]{
  //     std::cout << "in libgo";
  // };
  ft_set_u8strwid_func(&GetWidthOfUtf8);
  // start libgo coroutine
  auto bustub = std::make_unique<bustub::BustubInstance>("test.db");

  auto default_prompt = "TaLonSQL> ";
  auto emoji_prompt = "\U0001f6c1> ";  // the bathtub emoji
  bool use_emoji_prompt = false;
  bool disable_tty = false;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--emoji-prompt") == 0) {
      use_emoji_prompt = true;
      break;
    }
    if (strcmp(argv[i], "--disable-tty") == 0) {
      disable_tty = true;
      break;
    }
  }

  bustub->GenerateMockTable();

  if (bustub->buffer_pool_manager_ != nullptr) {
    bustub->GenerateTestTable();
  }

  linenoiseHistorySetMaxLen(1024);
  linenoiseSetMultiLine(1);

  auto prompt = use_emoji_prompt ? emoji_prompt : default_prompt;

  while (true) {
    std::string query;
    bool first_line = true;
    while (true) {
      auto line_prompt = first_line ? prompt : "... ";
      if (!disable_tty) {
        char *query_c_str = linenoise(line_prompt);
        if (query_c_str == nullptr) {
          return 0;
        }
        query += query_c_str;
        if (bustub::StringUtil::EndsWith(query, ";") || bustub::StringUtil::StartsWith(query, "\\")) {
          break;
        }
        query += " ";
        linenoiseFree(query_c_str);
      } else {
        std::string query_line;
        std::cout << line_prompt;
        std::getline(std::cin, query_line);
        if (!std::cin) {
          return 0;
        }
        query += query_line;
        if (bustub::StringUtil::EndsWith(query, ";") || bustub::StringUtil::StartsWith(query, "\\")) {
          break;
        }
        query += "\n";
      }
      first_line = false;
    }

    if (!disable_tty) {
      linenoiseHistoryAdd(query.c_str());
    }

    try {
      auto writer = bustub::FortTableWriter();
      bustub->ExecuteSql(query, writer);
      for (const auto &table : writer.tables_) {
        std::cout << table;
      }
    } catch (bustub::Exception &ex) {
      std::cerr << ex.what() << std::endl;
    }
  }

  return 0;
}
#endif

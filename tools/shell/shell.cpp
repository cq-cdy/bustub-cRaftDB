#include <iostream>
#include <string>
#include "binder/binder.h"
#include "common/bustub_instance.h"
#include "common/exception.h"
#include "common/util/string_util.h"
#include "libfort/lib/fort.hpp"
#include "linenoise/linenoise.h"
#include "utf8proc/utf8proc.h"

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

// NOLINTNEXTLINE
auto main(int argc, char **argv) -> int {
    //     std::thread([] { co_sched.Start(0,0); }).detach();
    // go[]{
    //     std::cout << "in libgo";
    // };
  ft_set_u8strwid_func(&GetWidthOfUtf8);
 // start libgo coroutine
  auto bustub = std::make_unique<bustub::BustubInstance>("/home/cdy/code/projects/bustub/tools/shell","test.db");

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

  std::cout << "Welcome to the BusTub shell! Type \\help to learn more." << std::endl << std::endl;

  linenoiseHistorySetMaxLen(1024);
  linenoiseSetMultiLine(1);

  auto prompt = use_emoji_prompt ? emoji_prompt : default_prompt;

  while (true) {
    std::string query;
    json js;
    bool first_line = true;
    // 测试用终端shell，实际要改成rpc ，从节点不能用shell，不然从节点无法excute sql
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
      
      js["sql"] = query;
      js["clientId"] = "127.0.0.1";
      js["commandId"] = 12345;
      // 上述js 对象可以由rpc对象替换
      checkJson(js);
    
      bustub->co_mtx_.lock();
      if(bustub->lastApplies_[js["clientId"]] == js["commandId"]){
        // 重复了
        continue;
      }
      bustub->co_mtx_.unlock();
      bustub->ExecuteSql(js, writer);
      for (const auto &table : writer.tables_) {
        std::cout << table;
      }
    } catch (bustub::Exception &ex) {
      std::cerr << ex.what() << std::endl;
    }
  }

  return 0;
}

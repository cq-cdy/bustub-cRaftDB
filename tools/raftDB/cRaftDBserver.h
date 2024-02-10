#pragma once
#include "raft/craft/raft.h"
#include "raft/json.hpp"
#include "unordered_map"
class cRaftDB : public craft::AbstractPersist {
 public:
  cRaftDB(std::string path, std::string snapFileName) : AbstractPersist(std::move(path), std::move(snapFileName)) {
    msgCh_ptr = new co_chan<ApplyMsg>(100);
    raft_ptr = new craft::Raft(this, msgCh_ptr);
    raft_ptr->launch();

    /*
        handle_func -> 
        while(true){
            sql <- msgCh_ptr
            bustub->excute(sql);
            // notify rpcCall return
        }
    */
  }

  // todo
  void rpcCall(std::string js /*,RPCRequest update_sql RPCReponse bool*/) {
    json js_ = jsonFromString(js);
    checkJson(js_);
    //  用 js_["commandId"] 和js_["clientId"] 从unordered_map 判断命令是否重复
    // 实现线性一致性。 
    // 如果是非查询语句
    auto res =  raft_ptr->submitCommand(js); // 可以往集群提交
    //wait handle_func apply
    return ;
  }
  craft::Raft *getRaftInstance() const { return raft_ptr; }
  co_chan<ApplyMsg> *getApplyChanInstance() const { return msgCh_ptr; }

  ~cRaftDB() {
    delete raft_ptr;
    delete msgCh_ptr;
  }

  void deserialization(const char *filename) override {}

  void serialization() override {}

 private:
  std::unordered_map<int, int> lastApplies_;
  craft::Raft *raft_ptr = nullptr;
  co_chan<ApplyMsg> *msgCh_ptr = nullptr;
  std::mutex mtx_;
  co_mutex co_mtx_;
};
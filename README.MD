# BusTubDB-cRaft：基于cRaft的轻量级分布式关系型数据库内核                    

*  基于C++17和CMU-BustubDB,cRaft分布式化非查询语句(宕机或毁机依靠SQL日志回滚)
* 在cmu-BusTubDB的基础上实现了缓存池、B+树索引构建、SQL执行器和优化器、并发控制等
* 分布式环境下客户端并发请求采用保证线性一致性为基准。

> 主要是结合mit 6.824与cmu15445两个课程下做的一个小玩具

环境要求请参考我的另一个玩具 [cRaft: Raft distributed consensus algorithm service framework based on C++ stacked coroutines](https://github.com/cq-cdy/cRaft)

参考代码[bustub](https://github.com/cmu-db/bustub) 和[bustub](https://github.com/ejunjsh/bustub)

> 当然很蠢的是目前只是做了一个简单的去共识SQL语句(性能很低)， 没有去共识操作日志和底层KV Pair。 

目前整体的cmu-busub代码量约1.4W行，目前已经完成了cmu的数据库内核的实现，以及将cRaft代码库整合上(tools/shell/shell.cpp)，接下来准备完成的是重写sql的上层服务将其分布式。


## 接下来的工作是将这个简单的关系型数据库内核提供将sql语句分布式化后的自动excute

已经初步构建分布式环境 `bustub/src/include/raft`

首选让bustub_instance 继承我们的craft 中的AbstractPersist类完成raft日志与bustub的交互
并且自定义快照逻辑
``` c++
class BustubInstance : public craft::AbstractPersist {
    // 重写反序列化与序列化方法
  void deserialization(const char *filename) override;

  void serialization() override;
}

```
> 因为在这个项目中的快照文件就默认为db_file文件，当快照跨界点转移时，就会默认复制db_file文件，正常sql操作就已经会落盘到db_file，，因此可以选择空实现这两个方法，但如果追求完全一致性，可以选在deserialization方法中重新加载热点page到bufferpool中。

> 但例如实现KV服务器时，就需要对KV时重写反序列化来保证哪些数据需要被加载到内存中，和哪些数据在快照转移时需要序列化到快照文件中，

在实例化时指定raft日志文件目录和db_file文件名，实现快照接口解耦
``` c++
BustubInstance::BustubInstance(const std::string &path, const std::string &db_snap_file_name)
    : AbstractPersist(std::move(path), std::move(db_snap_file_name)) {
        ...
```



bustub_instance.h 中引入 raft模块
``` C++
...
  craft::Raft *raft_ptr = nullptr;
  co_chan<ApplyMsg> *msgCh_ptr = nullptr;
  std::mutex mtx_;
  co_mutex co_mtx_;
  ...

```
bustub_instance.cpp中配置raft实例
``` C++
  msgCh_ptr = new co_chan<ApplyMsg>(100);
  raft_ptr = new craft::Raft(this, msgCh_ptr);
  raft_ptr->setClusterAddress({"127.0.0.1:12345", "127.0.0.1:12346", "127.0.0.1:12347"});
  raft_ptr->setLogLevel(spdlog::level::err);
  raft_ptr->launch();  // launch raft RPC service
```
当前 shell.cpp环境中测试，真实的部署场景 需要 将请求rpc化，否则follower无法excute sql
``` c++

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
```
在auto BustubInstance::ExecuteSqlTxn(const std::string &sql, ResultWriter &writer, Transaction *txn, Binder &binder) 中 等待 raft集群提交(*msgCh_ptr >> msg;)， 再选择事务提交
``` c++
 std::string sql= js["sql"];
  binder.ParseAndSave(sql);
  for (auto *stmt : binder.statement_nodes_) {
    auto statement = binder.BindStatement(stmt);
    if (statement->type_ == StatementType::SELECT_STATEMENT) {
      isSELECTsql = true;
      break;
    }
  }
  std::string commited_sql;
  if (!isSELECTsql) {
    while(raft_ptr->submitCommand(sql).isLeader);
    ApplyMsg msg;
    *msgCh_ptr >> msg;
    commited_sql = msg.command.content;
    lastApplies_[js["clientId"]] = js["commandId"];
  }
  isSELECTsql = false;
  auto txn = txn_manager_->Begin();
  auto result = ExecuteSqlTxn(commited_sql, writer, txn, binder);

  txn_manager_->Commit(txn);
```

## 后续

为了方便刚做完cmu15445 的小伙伴能够直观的看出两个模块的组合，因此直接在shell.cpp上进行了封装，可以直接进行调试，但在正式部署成服务时，务必不要采用shell，要采用rpc服务监听的方式，不然follower节点无法执行sql，要将  *msgCh_ptr >> msg; 这句代码死循环
在正式部署时，一定要去除shell模式，直接用rpc的方式接收封装的json，然后保持如下直接raft提交后 再数据库事务提交
```c++
while(1){

    while(raft_ptr->submitCommand(sql).isLeader);
    ApplyMsg msg;
    *msgCh_ptr >> msg;
    commited_sql = msg.command.content;
    lastApplies_[js["clientId"]] = js["commandId"];
}

```
> 后续再重新封装一个非shell式的服务发布的版本。



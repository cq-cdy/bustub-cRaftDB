# BusTubDB-cRaft：基于cRaft的轻量级分布式关系型数据库内核                    

*  基于C++17和CMU-BustubDB,cRaft分布式化非查询语句(宕机或毁机依靠SQL日志回滚)
* 在cmu-BusTubDB的基础上实现了存储引擎、B+树索引构建、SQL执行器和优化器、并发控制等
* 分布式环境下客户端并发请求采用保证线性一致性为基准。

> 主要是结合mit 6.824与cmu15445两个课程下做的一个小玩具

环境要求请参考我的另一个玩具 [cRaft: Raft distributed consensus algorithm service framework based on C++ stacked coroutines](https://github.com/cq-cdy/cRaft)

参考代码[bustub](https://github.com/cmu-db/bustub) 和[bustub](https://github.com/ejunjsh/bustub)

目前整体的cmu-busub代码量约1.4W行，目前已经完成了cmu的数据库内核的实现，以及将cRaft代码库整合上(tools/shell/shell.cpp)，接下来准备完成的是重写sql的上层服务将其分布式。
```c++
class bustubDB : public craft::AbstractPersist{
    ...
}
```

## 接下来的工作是将这个简单的关系型数据库内核分布式化

已经初步构建分布式环境`bustub/src/tools/raftDB` 和 `bustub/src/include/raft`




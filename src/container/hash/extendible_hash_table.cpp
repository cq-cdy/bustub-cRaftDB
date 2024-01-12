//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// extendible_hash_table.cpp
//
// Identification: src/container/hash/extendible_hash_table.cpp
//
// Copyright (c) 2022, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include <cassert>
#include <cstdlib>
#include <functional>
#include <list>
#include <utility>

#include "container/hash/extendible_hash_table.h"
#include "storage/page/page.h"

namespace bustub {

// k 是 pageid,V是pageid在缓存池中的下标
template <typename K, typename V>
ExtendibleHashTable<K, V>::ExtendibleHashTable(size_t bucket_size)
    : global_depth_(0), bucket_size_(bucket_size), num_buckets_(1) {
  dir_.push_back(std::make_shared<Bucket>(bucket_size, 0));
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::IndexOf(const K &key) -> size_t {
  int mask = (1 << global_depth_) - 1;
  return std::hash<K>()(key) & mask;
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetGlobalDepth() const -> int {
  std::scoped_lock<std::mutex> lock(latch_);
  return GetGlobalDepthInternal();
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetGlobalDepthInternal() const -> int {
  return global_depth_;
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetLocalDepth(int dir_index) const -> int {
  std::scoped_lock<std::mutex> lock(latch_);
  return GetLocalDepthInternal(dir_index);
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetLocalDepthInternal(int dir_index) const -> int {
  return dir_[dir_index]->GetDepth();
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetNumBuckets() const -> int {
  std::scoped_lock<std::mutex> lock(latch_);
  return GetNumBucketsInternal();
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::GetNumBucketsInternal() const -> int {
  return num_buckets_;
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::Find(const K &key, V &value) -> bool {
  std::scoped_lock<std::mutex> lock(latch_);

  auto index = IndexOf(key);
  auto target_bucket = dir_[index];

  return target_bucket->Find(key, value);
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::Remove(const K &key) -> bool {
  std::scoped_lock<std::mutex> lock(latch_);

  auto index = IndexOf(key);
  auto target_bucket = dir_[index];

  return target_bucket->Remove(key);
}

template <typename K, typename V>
void ExtendibleHashTable<K, V>::Insert(const K &key, const V &value) {
  std::scoped_lock<std::mutex> lock(latch_);

  // 这个IsFull()满就是单纯指某桶的元素个数满了，这个存储size多少个是创建桶的时候指定的
  // 不是深度
  while (dir_[IndexOf(key)]->IsFull()) {  // 注意这是while，一定会保证 不满 才会往下进行
    // full then  here
    auto index = IndexOf(key);
    auto target_bucket = dir_[index];

    // 并且用来区分的dir_的bit位也满了,那就要增加bit位的深度了
    if (target_bucket->GetDepth() == GetGlobalDepthInternal()) {
      global_depth_++;
      int capacity = dir_.size();
      dir_.resize(capacity << 1);
      for (int i = 0; i < capacity; i++) {
        dir_[i + capacity] = dir_[i];
      }
    }

    int mask = 1 << target_bucket->GetDepth();
    // 因为 dir_[IndexOf(key)] 这个桶满了，要分成两个桶，所以这里创建两个，增加桶的bit位置
    auto bucket_0 = std::make_shared<Bucket>(bucket_size_, target_bucket->GetDepth() + 1);
    auto bucket_1 = std::make_shared<Bucket>(bucket_size_, target_bucket->GetDepth() + 1);

    // 把原先满的桶中的元素，重新hash到这2个新桶里面
    for (const auto &item : target_bucket->GetItems()) {
      size_t hash_key = std::hash<K>()(item.first);
      // 用来区分哪些元素进哪个桶的策略罢了
      if ((hash_key & mask) != 0U) {
        bucket_1->Insert(item.first, item.second);
      } else {
        bucket_0->Insert(item.first, item.second);
      }
    }

    num_buckets_++;

    /*
        这里并不是会丢弃掉bucket_1 或者 bucket_0
        因为，在从0开始假如元素时，当桶深度 == 全局深度时，就会 <<1 扩容
        因此 移位扩容时，其中dir_.resize(capacity << 1);扩容了两倍，
        会导致另一个dir_ 也会 指向 该target_bucket，
        又因为是移位扩容，所以原先的 dir[i] 和 dir[i + capacity] 与mask,
        做&时候是肯定不一样的，mask就是由 1 << target_bucket->GetDepth()得到的
        所以下面的for循环，如果dir[i] 走的 bucket_1，那么dir[i+capacity] 就必然是 bucket_0
        因为扩容两倍，就相当于 <<1 ,而mask也是 <<1 ,所以i和i+capacity，在二进制上，一定是i+capacity
        会多一个二进制为，也就是i+capacity 正好和 mask (1 << depth) 能为true，i 和 mask & 为false

        关键就是 dir_中不值一个目录指向 target_bucket，而且，这些目录，i & mask 一定不是全相同的
        于是就可以对这两个新桶重定向
    */
    for (size_t i = 0; i < dir_.size(); i++) {
      if (dir_[i] == target_bucket) {
        if ((i & mask) != 0U) {
          dir_[i] = bucket_1;
        } else {
          dir_[i] = bucket_0;
        }
      }
    }
  }

  auto index = IndexOf(key);
  auto target_bucket = dir_[index];

  for (auto &item : target_bucket->GetItems()) {
    if (item.first == key) {
      item.second = value;
      return;
    }
  }

  target_bucket->Insert(key, value);
}

//===--------------------------------------------------------------------===//
// Bucket
//===--------------------------------------------------------------------===//
template <typename K, typename V>
ExtendibleHashTable<K, V>::Bucket::Bucket(size_t array_size, int depth) : size_(array_size), depth_(depth) {}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::Bucket::Find(const K &key, V &value) -> bool {
  return std::any_of(list_.begin(), list_.end(), [&key, &value](const auto &item) {
    if (item.first == key) {
      value = item.second;  // value 为传出参数
      return true;
    }
    return false;
  });
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::Bucket::Remove(const K &key) -> bool {
  return std::any_of(list_.begin(), list_.end(), [&key, this](const auto &item) {
    if (item.first == key) {
      this->list_.remove(item);
      return true;
    }
    return false;
  });
}

template <typename K, typename V>
auto ExtendibleHashTable<K, V>::Bucket::Insert(const K &key, const V &value) -> bool {
  if (IsFull()) {
    return false;
  }
  list_.emplace_back(key, value);
  return true;
}

template class ExtendibleHashTable<page_id_t, Page *>;
template class ExtendibleHashTable<Page *, std::list<Page *>::iterator>;
template class ExtendibleHashTable<int, int>;
// test purpose
template class ExtendibleHashTable<int, std::string>;
template class ExtendibleHashTable<int, std::list<int>::iterator>;

}  // namespace bustub

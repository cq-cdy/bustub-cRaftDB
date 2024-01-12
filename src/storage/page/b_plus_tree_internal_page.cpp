//===----------------------------------------------------------------------===//
//
//                         CMU-DB Project (15-445/645)
//                         ***DO NO SHARE PUBLICLY***
//
// Identification: src/page/b_plus_tree_internal_page.cpp
//
// Copyright (c) 2018, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include <iostream>
#include <sstream>

#include "common/exception.h"
#include "storage/page/b_plus_tree_internal_page.h"

namespace bustub {
/*****************************************************************************
 * HELPER METHODS AND UTILITIES
 *****************************************************************************/
/*
 * Init method after creating a new internal page
 * Including set page type, set current size, set page id, set parent id and set
 * max page size
 */
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::Init(page_id_t page_id, page_id_t parent_id, int max_size) {
  SetPageType(IndexPageType::INTERNAL_PAGE);
  SetSize(0);
  SetPageId(page_id);
  SetParentPageId(parent_id);
  SetMaxSize(max_size);
}

/*
 * Helper method to get/set the key associated with input "index"(a.k.a
 * array offset)
 */
INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_INTERNAL_PAGE_TYPE::KeyAt(int index) const -> KeyType {
  // replace with your own code
  return array_[index].first;
}

INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::SetKeyAt(int index, const KeyType &key) { array_[index].first = key; }

/*
 * Helper method to get the value associated with input "index"(a.k.a array
 * offset)
 */
INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_INTERNAL_PAGE_TYPE::ValueAt(int index) const -> ValueType { return array_[index].second; }

INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::SetValueAt(int index, const ValueType &value) { array_[index].second = value; }

INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_INTERNAL_PAGE_TYPE::ValueIndex(const ValueType &value) const -> int {
  auto it = std::find_if(array_, array_ + GetSize(), [&value](const auto &pair) { return pair.second == value; });
  return std::distance(array_, it);
}

INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_INTERNAL_PAGE_TYPE::Lookup(const KeyType &key, const KeyComparator &comparator) const -> ValueType {
  // 找到第一个不小于的key的
  auto target = std::lower_bound(array_ + 1, array_ + GetSize(), key,
                                 [&comparator](const auto &pair, auto k) { return comparator(pair.first, k) < 0; });
  if (target == array_ + GetSize()) {  // 末尾，要去最右指针的page
    return ValueAt(GetSize() - 1);
  }
  if (comparator(target->first, key) == 0) {
    return target->second;
  }
  return std::prev(target)->second;
}

INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::PopulateNewRoot(const ValueType &old_value, const KeyType &new_key,
                                                     const ValueType &new_value) {
  SetKeyAt(1, new_key);
  SetValueAt(0, old_value);
  SetValueAt(1, new_value);
  SetSize(2);
}

INDEX_TEMPLATE_ARGUMENTS
/// @brief
/// 在 B+树内部页中插入一个新的节点，在old_value之后插入new KV
/// @param old_value
/// @param new_key
/// @param new_value
/// @return
auto B_PLUS_TREE_INTERNAL_PAGE_TYPE::InsertNodeAfter(const ValueType &old_value, const KeyType &new_key,
                                                     const ValueType &new_value) -> int {
  auto new_value_idx = ValueIndex(old_value) + 1;
  //从 new_value_idx 开始到数组末尾的所有元素向后移动一位，为新值腾出空间。
  std::move_backward(array_ + new_value_idx, array_ + GetSize(), array_ + GetSize() + 1);

  array_[new_value_idx].first = new_key;
  array_[new_value_idx].second = new_value;

  IncreaseSize(1);

  return GetSize();
}

INDEX_TEMPLATE_ARGUMENTS
/// @brief
/// @param recipient 目标地址
/// @param buffer_pool_manager
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::MoveHalfTo(BPlusTreeInternalPage *recipient,
                                                BufferPoolManager *buffer_pool_manager) {
  int start_split_indx = GetMinSize();
  int original_size = GetSize();
  SetSize(start_split_indx);
  recipient->CopyNFrom(array_ + start_split_indx, original_size - start_split_indx, buffer_pool_manager);
}

INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::CopyNFrom(MappingType *items, int size, BufferPoolManager *buffer_pool_manager) {
  std::copy(items, items + size, array_ + GetSize());

  for (int i = 0; i < size; i++) {
    auto page = buffer_pool_manager->FetchPage(ValueAt(i + GetSize()));
    auto *node = reinterpret_cast<BPlusTreePage *>(page->GetData());
    node->SetParentPageId(GetPageId());
    buffer_pool_manager->UnpinPage(page->GetPageId(), true);  // dirty page
  }

  IncreaseSize(size);
}

INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::Remove(int index) {
  //  array_ + index + 1 要删除元素的后一个位置
  //  array_ + GetSize() ：到最后一个位置
  //  array_ + index：移动到 被删除节点的位置
  std::move(array_ + index + 1, array_ + GetSize(), array_ + index);
  IncreaseSize(-1);
}

INDEX_TEMPLATE_ARGUMENTS
auto B_PLUS_TREE_INTERNAL_PAGE_TYPE::RemoveAndReturnOnlyChild() -> ValueType {
  ValueType only_value = ValueAt(0);  // 注意0号value是有效的，0号key才是不用的
  SetSize(0);
  return only_value;
}

INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::MoveAllTo(BPlusTreeInternalPage *recipient, const KeyType &middle_key,
                                               BufferPoolManager *buffer_pool_manager) {
  /*
 内部节点中的0号位置
在B+树的内部节点中，键用于指示子节点中键值对的分界。在某些实现中，内部节点的0号位置键可能不用于存储实际的键值，
而是作为一个哨兵值或特殊标记。这时，SetKeyAt(0,
middle_key) 可能是在节点分裂后，将新的分界键设置到内部节点的0号位置。这通常发生在内部节点需要调整其指向子节点的指针时。
例如，在一个节点分裂时，中间的键被提升到父节点，且可能被设置在0号位置，表示所有小于这个键的子节点都应该位于这个内部节点的左侧。*/
  SetKeyAt(0, middle_key);
  recipient->CopyNFrom(array_, GetSize(), buffer_pool_manager);
  SetSize(0);
}

//将当前节点的第一个元素移动到另一个节点的末尾，通常再兄弟节点之间再平衡时使用
INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::MoveFirstToEndOf(BPlusTreeInternalPage *recipient, const KeyType &middle_key,
                                                      BufferPoolManager *buffer_pool_manager) {
  SetKeyAt(0, middle_key);
  auto first_item = array_[0];
  recipient->CopyLastFrom(first_item, buffer_pool_manager);

  std::move(array_ + 1, array_ + GetSize(), array_);
  IncreaseSize(-1);
}

INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::CopyLastFrom(const MappingType &pair, BufferPoolManager *buffer_pool_manager) {
  *(array_ + GetSize()) = pair;  // 在末尾添加一个KV
  IncreaseSize(1);
  // pair.second 就是V，就是子节点，子节点的父节点就算当前page
  auto page = buffer_pool_manager->FetchPage(pair.second);
  auto *node = reinterpret_cast<BPlusTreePage *>(page->GetData());
  node->SetParentPageId(GetPageId());
  buffer_pool_manager->UnpinPage(page->GetPageId(), true);
}

INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::MoveLastToFrontOf(BPlusTreeInternalPage *recipient, const KeyType &middle_key,
                                                       BufferPoolManager *buffer_pool_manager) {
  auto last_item = array_[GetSize() - 1];
  recipient->SetKeyAt(0, middle_key);
  recipient->CopyFirstFrom(last_item, buffer_pool_manager);

  IncreaseSize(-1);
}

INDEX_TEMPLATE_ARGUMENTS
void B_PLUS_TREE_INTERNAL_PAGE_TYPE::CopyFirstFrom(const MappingType &pair, BufferPoolManager *buffer_pool_manager) {
  std::move_backward(array_, array_ + GetSize(), array_ + GetSize() + 1);
  *array_ = pair;
  IncreaseSize(1);

  auto page = buffer_pool_manager->FetchPage(pair.second);
  auto *node = reinterpret_cast<BPlusTreePage *>(page->GetData());
  node->SetParentPageId(GetPageId());
  buffer_pool_manager->UnpinPage(page->GetPageId(), true);
}

// valuetype for internalNode should be page id_t
template class BPlusTreeInternalPage<GenericKey<4>, page_id_t, GenericComparator<4>>;
template class BPlusTreeInternalPage<GenericKey<8>, page_id_t, GenericComparator<8>>;
template class BPlusTreeInternalPage<GenericKey<16>, page_id_t, GenericComparator<16>>;
template class BPlusTreeInternalPage<GenericKey<32>, page_id_t, GenericComparator<32>>;
template class BPlusTreeInternalPage<GenericKey<64>, page_id_t, GenericComparator<64>>;
}  // namespace bustub

#ifndef PAIRING_HEAP_H
#define PAIRING_HEAP_H

#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

class PairingNode {
public:
  PairingNode *prev;
  PairingNode *next;
  PairingNode *first_child;

  PairingNode() {
    prev = next = first_child = nullptr;
  }

  void reset() {
    prev = next = first_child = nullptr;
  }
};

template <typename T, typename Compare, size_t Offset>
class PairingHeap {
public:
  PairingHeap(const Compare& comp=Compare());
  size_t size();
  void insert(T& n);
  void decrease_key(T& n);
  T& get_min();
  void delete_min();

private:
  size_t hsize;
  PairingNode *root;
  Compare less;

  PairingNode* link(PairingNode *x, PairingNode *y);
};

template <typename T, typename Compare, size_t Offset>
PairingHeap<T, Compare, Offset>::PairingHeap(const Compare& comp)
: hsize(0), root(nullptr), less(comp)
{ }

template <typename T, typename Compare, size_t Offset>
size_t PairingHeap<T, Compare, Offset>::size() {
  return hsize;
}

template <typename T, typename Compare, size_t Offset>
void PairingHeap<T, Compare, Offset>::insert(T& t) {
  PairingNode *n = reinterpret_cast<PairingNode *>(
      reinterpret_cast<char *>(&t) + Offset
  );
  if (n == nullptr) return;
  assert(n->prev == nullptr);
  assert(n->next == nullptr);
  assert(n->first_child == nullptr);
  root = link(root, n);
  hsize++;
}

template <typename T, typename Compare, size_t Offset>
void PairingHeap<T, Compare, Offset>::decrease_key(T& t) {
  PairingNode *n = reinterpret_cast<PairingNode *>(
      reinterpret_cast<char *>(&t) + Offset
  );
  if (n == nullptr || n == root) return;
  assert(n->prev != nullptr);

  if (n->next != nullptr) {
    n->next->prev = n->prev;
  }

  if (n->prev->first_child == n) {
    n->prev->first_child = n->next;
  } else {
    n->prev->next = n->next;
  }

  n->prev = n->next = nullptr;
  root = link(root, n);
}

template <typename T, typename Compare, size_t Offset>
T& PairingHeap<T, Compare, Offset>::get_min() {
  return *(reinterpret_cast<T *>(reinterpret_cast<char *>(root) - Offset));
}

template <typename T, typename Compare, size_t Offset>
void PairingHeap<T, Compare, Offset>::delete_min() {
  if (hsize == 0) return;

  PairingNode *n = root->first_child;
  std::vector<PairingNode *> tmp_list;

  while (n != nullptr && n->next != nullptr) {
    PairingNode *n1 = n;
    PairingNode *n2 = n->next;
    n = n2->next;
    n1->prev = n1->next = nullptr;
    n2->prev = n2->next = nullptr;
    tmp_list.push_back(link(n1, n2));
  }

  if (n != nullptr) {
    n->prev = nullptr;
  }

  for (auto it = tmp_list.rbegin(); it != tmp_list.rend(); ++it) {
    assert((*it)->prev == nullptr);
    n = link(n, *it);
  }

  root->prev = root->next = root->first_child = nullptr;
  root = n;
  hsize--;
}

template <typename T, typename Compare, size_t Offset>
PairingNode *PairingHeap<T, Compare, Offset>::link(PairingNode *x,
                                                   PairingNode *y) {
  if (x == nullptr) return y;
  if (y == nullptr) return x;

  assert(x->prev == nullptr);
  assert(x->next == nullptr);
  assert(y->prev == nullptr);
  assert(y->next == nullptr);

  T *tx = reinterpret_cast<T *>(reinterpret_cast<char *>(x) - Offset);
  T *ty = reinterpret_cast<T *>(reinterpret_cast<char *>(y) - Offset);

  if (less(*tx, *ty)) {
    y->prev = x;
    y->next = x->first_child;
    if (x->first_child != nullptr) {
      x->first_child->prev = y;
    }
    x->first_child = y;
    assert(x->prev == nullptr);
    assert(x->next == nullptr);
    return x;
  } else {
    x->prev = y;
    x->next = y->first_child;
    if (y->first_child != nullptr) {
      y->first_child->prev = x;
    }
    y->first_child = x;
    assert(y->prev == nullptr);
    assert(y->next == nullptr);
    return y;
  }
}

#endif

#pragma once
#include <iostream>
#include <memory>

// forward linked list
template <class T, class Allocator = std::allocator<T>> class List {
private:
  struct Node {
    Node() : next_node{nullptr} {}
    Node(const T &value) : next_node{nullptr}, data{value} {}
    Node *next_node;
    T data;
  };
  Node *first_node_ptr;
  Node *last_node_ptr;
  size_t total_number;
  using AllocatorTraits = std::allocator_traits<Allocator>;
  using node_alloc_t =
      typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
  node_alloc_t node_alloc;

public:
  struct Iterator {
    Iterator() : node_ptr{nullptr} {}
    Iterator(Node *own_ptr) : node_ptr{own_ptr} {}

    Node *operator++() {
      node_ptr = node_ptr->next_node;
      return node_ptr;
    }
    T get() { 
      if(node_ptr!=nullptr)
        return node_ptr->data;
      }

    bool operator!=(const Iterator &other) {
      return !(node_ptr == other.node_ptr);
    }

  private:
    Node *node_ptr;
  };

  List() : first_node_ptr{nullptr}, last_node_ptr{nullptr}, total_number{0} {}
  List(const List &other) : first_node_ptr{nullptr}, last_node_ptr{nullptr}, total_number{0}{
    
    if (other.first_node_ptr != nullptr)
      {
          Node *other_ptr_toCopy = other.first_node_ptr;
          Node *otherPtrTemp = nullptr;
          
          while (other_ptr_toCopy != nullptr)
          {
              otherPtrTemp = new Node(other_ptr_toCopy->data);
              if (first_node_ptr == nullptr)
              {
                  first_node_ptr = otherPtrTemp;
                  last_node_ptr = otherPtrTemp;
              }
              else
              {
                  last_node_ptr->next_node = otherPtrTemp;
                  last_node_ptr = otherPtrTemp;
              }
              other_ptr_toCopy = other_ptr_toCopy->next_node;
          }
          total_number = other.total_number;
      }
  }
  ~List() { clear(); }

  void clear() {
    if (first_node_ptr == nullptr) {
      return;
    }

    Node *current_node_ptr = first_node_ptr;
    Node *next_node_ptr = nullptr;

    while (current_node_ptr != nullptr) {
      next_node_ptr = current_node_ptr->next_node;
      node_alloc.deallocate(current_node_ptr, 1);
      current_node_ptr = next_node_ptr;
    }

    first_node_ptr = nullptr;
    last_node_ptr = nullptr;
    total_number = 0;
  }

  size_t size() const { return total_number; }

  void push_back(const T &value) {
    Node *own_ptr = node_alloc.allocate(1);
    node_alloc.construct(own_ptr, value);

    if (first_node_ptr == nullptr) {
      first_node_ptr = own_ptr;
      last_node_ptr = own_ptr;
    } else {
      last_node_ptr->next_node = own_ptr;
      last_node_ptr = own_ptr;
    }

    ++total_number;
  }

  bool insert(const size_t &place, const T &value) {
    if (place > total_number)
      return false;

    if (place == total_number) {
      push_back(value);
      return true;
    }

    Node *own_ptr = node_alloc.allocate(1);
    node_alloc.construct(own_ptr, value);

    Node *current_node_ptr = first_node_ptr;
    Node *prev_node_ptr = nullptr;

    size_t i = 0;
    while (i != place) {
      prev_node_ptr = current_node_ptr;
      current_node_ptr = current_node_ptr->next_node;
      ++i;
    }

    if (current_node_ptr != nullptr) {
      own_ptr->next_node = current_node_ptr;
    } else {
      return false;
    }

    if (prev_node_ptr == nullptr) {
      first_node_ptr = own_ptr;
    } else {
      prev_node_ptr->next_node = own_ptr;
    }

    ++total_number;

    return true;
  }
  T &operator[](const size_t &place) {
    Node *current_node_ptr = get_node_by_place(place);

    return current_node_ptr->data;
  }
  List &operator=(const List &rhs) {
    List temp(rhs);
    Node *last = last_node_ptr;
    last_node_ptr = temp.last_node_ptr;
    temp.last_node_ptr = last;

    Node *first = first_node_ptr;
    first_node_ptr = temp.first_node_ptr;
    temp.first_node_ptr = first;

    size_t size = total_number;
    total_number = temp.total_number;
    temp.total_number = size;

    return *this;
  }
  Iterator begin() const { return Iterator(first_node_ptr); }

  Iterator last_valid() const {
    if (last_node_ptr == nullptr)
      return Iterator(nullptr);
    return Iterator(last_node_ptr);
  }

  Iterator end() const {
    if (last_node_ptr == nullptr)
      return Iterator(nullptr);
    return Iterator(last_node_ptr->next_node);
  }

private:
  Node *get_node_by_place(const size_t &place) const {
    if (first_node_ptr == nullptr) {
      return nullptr;
    }

    Node *current_node_ptr = first_node_ptr;

    size_t i = 0;
    while (i != place) {
      current_node_ptr = current_node_ptr->next_node;

      if (current_node_ptr == nullptr) {
        return nullptr;
      }

      ++i;
    }

    return current_node_ptr;
  }

private:
  Iterator owl_iterator;
};

template <class T, class Allocator>
std::ostream &operator<<(std::ostream &os, const List<T, Allocator> &lcont) {
  for (auto iter = lcont.begin(); iter != lcont.end(); ++iter) {
    os << iter.get();
    if (iter != lcont.last_valid()) {
      os << ", ";
    }
  }

  return os;
}

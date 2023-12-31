#pragma once

#include "Chunk.h"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <new>
#include <utility>

const int limit = 10;

template <typename T, size_t N = limit> class CustomAllocator {
public:
  using value_type = T;
  using pointer = T *;

  using size_type = std::size_t;

  template <typename U> struct rebind {
    using other = CustomAllocator<U, N>;
  };

private:
  size_type items_per_chunk;
  Chunk<value_type> *first_allocator_chunk = nullptr;
  Chunk<value_type> *last_allocator_chunk = nullptr;
  size_type alloc_requests_number = 0;
  Chunk<value_type> *allocate_new_chunk(const size_type &items_number);
public:

  CustomAllocator() {items_per_chunk=limit;}

  template <typename U, size_type S>
  CustomAllocator(const CustomAllocator<U, S>) {}

  ~CustomAllocator() {
    std::cout << "Allocator destructor" << std::endl;
    Chunk<value_type> *tmp_chunk = nullptr;
    last_allocator_chunk = first_allocator_chunk;

    while (last_allocator_chunk != nullptr) {
      tmp_chunk = last_allocator_chunk->next;
      last_allocator_chunk->free_memory();
      delete last_allocator_chunk;
      last_allocator_chunk = tmp_chunk;
    }
  }
  pointer allocate(const size_type n) {
    if (first_allocator_chunk == nullptr) {
      size_type items_number = (n > items_per_chunk) ? n : items_per_chunk;

      first_allocator_chunk = allocate_new_chunk(items_number);

      last_allocator_chunk = first_allocator_chunk;
      alloc_requests_number++;

      return reinterpret_cast<pointer>(last_allocator_chunk->occupy_memory(n));
    }

    if (last_allocator_chunk->enough_space(n)) {
      alloc_requests_number++;
      return reinterpret_cast<pointer>(last_allocator_chunk->occupy_memory(n));
    }

    Chunk<value_type> *tmp_chunk = first_allocator_chunk;
    while (tmp_chunk != last_allocator_chunk) {
      if (tmp_chunk->enough_space(n)) {
        alloc_requests_number++;
        return reinterpret_cast<pointer>(tmp_chunk->occupy_memory(n));
      }
      tmp_chunk = tmp_chunk->next;
    }

    size_type items_number = (n > items_per_chunk) ? n : items_per_chunk;
    last_allocator_chunk->next = allocate_new_chunk(items_number);

    if (last_allocator_chunk->next == nullptr) {
      throw std::bad_alloc();
    }

    last_allocator_chunk = last_allocator_chunk->next;
    alloc_requests_number++;
    return reinterpret_cast<pointer>(last_allocator_chunk->occupy_memory(n));
  }
  void deallocate(pointer p, const size_type &n) {
    if (first_allocator_chunk == nullptr) {
      std::cout << "deallocate error!" << std::endl;
      return;
    }

    Chunk<value_type> *curr_chunk = first_allocator_chunk;
    Chunk<value_type> *next_chunk = nullptr;
    Chunk<value_type> *prev_chunk = nullptr;

    while (curr_chunk != nullptr) {
      next_chunk = curr_chunk->next;
      if (curr_chunk->deoccupy_memory(p, n)) {
        break;
      }
      prev_chunk = curr_chunk;
      curr_chunk = curr_chunk->next;
    }

    if (curr_chunk->is_free()) {
      if (prev_chunk == nullptr) {
        first_allocator_chunk = next_chunk;
      } else {
        prev_chunk->next = next_chunk;
      }

      if (last_allocator_chunk == curr_chunk) {
        last_allocator_chunk = prev_chunk;
      }

      if (first_allocator_chunk == curr_chunk) {
        first_allocator_chunk = next_chunk;
      }

      delete curr_chunk;
    }

    --alloc_requests_number;
  }
  template <typename U, typename... Args> void construct(U *p, Args &&...args) {
    new (p) U(std::forward<Args>(args)...);
  }
};

template <class T, size_t N, class U, size_t S>
constexpr bool operator==(const CustomAllocator<T, N> &,
                          const CustomAllocator<U, S> &) noexcept {
  return true;
}

template <class T, size_t N, class U, size_t S>
constexpr bool operator!=(const CustomAllocator<T, N> &x,
                          const CustomAllocator<U, S> &y) noexcept {
  return !(x == y);
}

template <typename T, size_t N>
Chunk<T> *
CustomAllocator<T, N>::allocate_new_chunk(const size_t &items_number) {
  Chunk<T> *chunk_ptr = reinterpret_cast<Chunk<T> *>(new Chunk<T>);
  if (!chunk_ptr) {
    return nullptr;
  }

  if (nullptr == chunk_ptr->allocate_space(items_number)) {
    delete chunk_ptr;
    return nullptr;
  }

  return chunk_ptr;
}

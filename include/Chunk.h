#include <cstdlib>
#include <iostream>
template <typename T> struct Chunk {
  void *memory_ptr = nullptr;
  void *end_ptr = nullptr;

  std::size_t size = 0;
  std::size_t occupied = 0;
  std::size_t released = 0;

  Chunk<T> *next = nullptr;

  Chunk()
      : memory_ptr{nullptr}, end_ptr{nullptr}, size{0}, occupied{0},
        released{0} {}

  void *allocate_space(const std::size_t &items_number) {
    memory_ptr = std::malloc(items_number * sizeof(T));

    if (memory_ptr == nullptr) {
      return nullptr;
    }

    end_ptr = memory_ptr;
    size = items_number;

    return memory_ptr;
  }

  void free_memory() {
    std::free(memory_ptr);
    memory_ptr = nullptr;
    std::cout << "FREE all memory" << std::endl;
    
  }
  std::size_t memory_space() { return sizeof(T) * occupied; }
  void *occupy_memory(const std::size_t &n) {
    void *p = end_ptr;
    end_ptr = ((char *)end_ptr) + n * sizeof(T);
    occupied += n;

    return p;
  }

  bool deoccupy_memory(void *p, const std::size_t &n) {
    if (!contains_pointer(p)) {
      return false;
    }

    released += n;

    if (released == occupied) {
      free_memory();
    }

    return true;
  }

  bool is_free() { return (memory_ptr == nullptr); }

  bool enough_space(const std::size_t &n) { return ((size - occupied) >= n); }

  bool contains_pointer(void *p) {
    void *border_ptr = ((char *)memory_ptr) + size * sizeof(T);
    return ((p >= memory_ptr) && (p < border_ptr));
  }
};

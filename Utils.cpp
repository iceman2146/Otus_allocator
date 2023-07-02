#include "Utils.h"

#include "custom_allocator.h"
#include "custom_container.h"
#include <iostream>
#include <map>
#include <string>


using AllocatorInt = CustomAllocator<int, 10>;
using AllocatorPair = CustomAllocator<std::pair<int, int>, 10>;

void Separator(std::string str, int n) {
  for (int i = 0; i < n; i++) {
    std::cout << "-";
  }
  std::cout << std::endl;
  std::cout << str << std::endl;
}

long long calculate_factorial(int n) {
  long long factorial = 1;

  for (int i = 1; i <= n; ++i) {
    factorial *= i;
  }

  return factorial;
}
void test_std_map_with_std_allocator() {
  using TestMap = std::map<int, int>;

  auto test_map = TestMap{};

  for (int i = 0; i < 10; ++i) {
    test_map[i] = static_cast<int>(calculate_factorial(i));
  }

  std::cout << std::endl;
  for (auto test_pair : test_map) {
    std::cout << test_pair.first << " " << test_pair.second << std::endl;
  }
  std::cout << std::endl;
}

void test_std_map_with_custom_allocator() {
  using TestMap = std::map<int, int, std::less<int>, AllocatorPair>;

  auto test_map = TestMap{};

  for (int i = 0; i < 10; ++i) {
    test_map[i] = static_cast<int>(calculate_factorial(i));
  }

  std::cout << std::endl;
  for (auto test_pair : test_map) {
    std::cout << test_pair.first << " " << test_pair.second << std::endl;
  }
  std::cout << std::endl;
}

void test_custom_container_with_custom_allocator() {
  using TestList = List<int, AllocatorInt>;

  TestList owl_cont_custom_alloc;

  for (int i = 0; i < 10; ++i) {
    owl_cont_custom_alloc.push_back(calculate_factorial(i));
  }

  std::cout << std::endl;
  std::cout << owl_cont_custom_alloc << std::endl;
  std::cout << std::endl;
}

void test_custom_container_with_std_allocator() {
  List<int> owl_cont_std_alloc;

  for (int i = 0; i < 10; ++i) {
    owl_cont_std_alloc.push_back(calculate_factorial(i));
  }

  std::cout << std::endl;
  std::cout << owl_cont_std_alloc << std::endl;
  std::cout << std::endl;
}

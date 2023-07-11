#include "Utils.h"

#include "custom_allocator.h"
#include "custom_container.h"
#include "version.h"
#include <forward_list>
#include <iostream>
#include <map>
#include <string>


int version() { return PROJECT_VERSION_PATCH; }

using AllocatorInt = CustomAllocator<int, 10>;
using AllocatorPair = CustomAllocator<std::pair<int, int>, 10>;

void Separator(std::string str, int n) {
  for (int i = 0; i < n; i++) {
    std::cout << "-";
  }
  std::cout << std::endl;
  std::cout << str << std::endl;
}

long long factorial(int n) {
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
    test_map[i] = static_cast<int>(factorial(i));
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
    test_map[i] = static_cast<int>(factorial(i));
  }

  std::cout << std::endl;

  for (auto test_pair : test_map) {
    std::cout << test_pair.first << " " << test_pair.second << std::endl;
  }
  std::cout << std::endl;
}


void test_custom_container_with_custom_allocator() {
  using TestList = List<int, AllocatorInt>;

  TestList list;

  for (int i = 0; i < 10; ++i) {
    list.push_back(factorial(i));
  }

  std::cout << std::endl;
  std::cout << list << std::endl;
  std::cout << std::endl;
}

void test_custom_container_with_std_allocator() {
  List<int> list;

  for (int i = 0; i < 10; ++i) {
    list.push_back(factorial(i));
  }

  std::cout << std::endl;
  std::cout << list << std::endl;
  std::cout << std::endl;
}

void ryzhikov_test()
{
  List<int> a;
  a.push_back(42);
  auto b=a;
  std::cout<<"A:\n";
  std::cout << a << std::endl;
  std::cout<<"B:\n";
  std::cout << b << std::endl;
  


  

}
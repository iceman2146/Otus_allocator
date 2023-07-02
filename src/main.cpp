#include "Utils.h"
#include <iostream>

int main () {
	Separator("test_std_map_with_custom_allocator",80);
	test_std_map_with_custom_allocator();

	Separator("test_std_map_with_std_allocator",80);
	test_std_map_with_std_allocator();

	Separator("test_custom_container_with_custom_allocator",80);
	test_custom_container_with_custom_allocator();

	Separator("test_custom_container_with_std_allocator",80);
	test_custom_container_with_std_allocator();
	

    return 0;
}

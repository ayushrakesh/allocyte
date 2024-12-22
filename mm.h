
#include <cstdint>
#include <cstring>

constexpr uint32_t MM_MAX_STRUCT_NAME = 32;
constexpr uint32_t SYSTEM_PAGE_SIZE = 4096; // Example system page size

struct vm_page_family
{
  char struct_name[MM_MAX_STRUCT_NAME];
  uint32_t struct_size;
};

struct vm_page_for_families
{
  vm_page_for_families *next;
  vm_page_family families[]; // Flexible array member (C++ alternative to zero-length array)
};

constexpr uint32_t MAX_FAMILIES_PER_PAGE = (SYSTEM_PAGE_SIZE - sizeof(vm_page_for_families *)) / sizeof(vm_page_family);

#define ITERATE_PAGE_FAMILIES_BEGIN(vm_page_for_families_ptr, curr)                                                                            \
  {                                                                                                                                            \
    uint32_t count = 0;                                                                                                                        \
    for (curr = (vm_page_family *)&vm_page_for_families_ptr->families[0]; curr->struct_size && count < MAX_FAMILIES_PER_PAGE; curr++, count++) \
    {
#define ITERATE_PAGE_FAMILIES_END(vm_page_for_families_ptr, curr) \
  }                                                               \
  }

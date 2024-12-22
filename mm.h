#ifndef VM_PAGE_H
#define VM_PAGE_H

#include <cstdint>
#include <cstring>

#define MM_MAX_STRUCT_NAME 32

struct vm_page_family
{
  char struct_name[MM_MAX_STRUCT_NAME];
  uint32_t struct_size;
};

struct vm_page_for_families
{
  vm_page_for_families *next;
  vm_page_family families[0];
};

#define MAX_FAMILIES_PER_VM_PAGE (SYSTEM_PAGE_SIZE - sizeof(vm_page_for_families *)) / sizeof(vm_page_family)

#define ITERATE_PAGE_FAMILIES_BEGIN(vm_page_for_families_ptr, curr)       \
  {                                                                       \
    uint32_t count = 0;                                                   \
    for (curr = (vm_page_family *)&vm_page_for_families_ptr->families[0]; \
         curr->struct_size && count < MAX_FAMILIES_PER_VM_PAGE;           \
         curr++, count++)

#define ITERATE_PAGE_FAMILIES_END(vm_page_for_families_ptr, curr) }

#endif // VM_PAGE_H

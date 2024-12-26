#include <stdio.h>
#include <memory>
#include <cstring>
#include <unistd.h>
#include <sys/mman.h>

#include "mm.h"

#include <bits/stdc++.h>
using namespace std;

static vm_page_for_families *first_vm_page_for_families = NULL;
static size_t SYSTEM_PAGE_SIZE = 0;

void mm_init()
{
  SYSTEM_PAGE_SIZE = getpagesize();
}

static void *mm_get_new_vm_page_from_kernel(int units)
{
  void *vm_page = mmap(0, units * SYSTEM_PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, 0, 0);

  if (vm_page == MAP_FAILED)
  {
    std::cerr << "Error: VM page allocation failed" << std::endl;
    return nullptr;
  }

  std::memset(vm_page, 0, units * SYSTEM_PAGE_SIZE);
  return vm_page;
}

void mm_instantiate_page_family(const char *struct_name, uint32_t struct_size)
{
  vm_page_family *page_family_curr = NULL;
  vm_page_for_families *page_families_curr = NULL;

  if (struct_size > SYSTEM_PAGE_SIZE)
  {
    cout << "Error : Structure size exceeds the system page size" << endl;
    return;
  }

  if (first_vm_page_for_families == NULL)
  {
    first_vm_page_for_families = (vm_page_for_families *)mm_get_new_vm_page_from_kernel(1);
    first_vm_page_for_families->next = NULL;
    strncpy(first_vm_page_for_families->families[0].struct_name, struct_name, MM_MAX_STRUCT_NAME);
    first_vm_page_for_families->families[0].struct_size = struct_size;
    return;
  }

  uint32_t count = 0;

  ITERATE_PAGE_FAMILIES_BEGIN(first_vm_page_for_families, page_family_curr)
  {
    if (strncmp(page_family_curr->struct_name, struct_name, MM_MAX_STRUCT_NAME) != 0)
    {
      count++;
      continue;
    }
    assert(0); // Structure already exists
  }
  ITERATE_PAGE_FAMILIES_END(first_vm_page_for_families, page_family_curr);

  if (count == MAX_FAMILIES_PER_VM_PAGE)
  {
    page_families_curr = (vm_page_for_families *)mm_get_new_vm_page_from_kernel(1);
    page_families_curr->next = first_vm_page_for_families;
    first_vm_page_for_families = page_families_curr;
    page_family_curr = &first_vm_page_for_families->families[0];
  }

  strncpy(page_family_curr->struct_name, struct_name, MM_MAX_STRUCT_NAME);
  page_family_curr->struct_size = struct_size;
}

static void mm_union_free_blocks(block_meta_data *first, block_meta_data *second)
{
  assert(first->is_free == MM_TRUE && second->is_free == MM_TRUE);

  first->block_size += second->block_size + sizeof(block_meta_data);

  first->next = second->next;

  if (second->next)
  {
    second->next->prev = first;
  }
}

// int main()
// {
//   mm_init();
//   cout << "VM page size = " << SYSTEM_PAGE_SIZE << std::endl;

//   void *addr1 = mm_get_new_vm_page_from_kernel(1);
//   void *addr2 = mm_get_new_vm_page_from_kernel(1);

//   cout << "page 1 = " << addr1 << ", page 2 = " << addr2 << std::endl;

//   return 0;
// }

#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <bits/stdc++.h>
using namespace std;

static size_t SYSTEM_PAGE_SIZE = 0;

void mm_init()
{
  SYSTEM_PAGE_SIZE = sysconf(_SC_PAGESIZE); // Use sysconf in C++
}

// Function to request VM page from kernel
static void *mm_get_new_vm_page_from_kernel(int units)
{
  char *vm_page = static_cast<char *>(mmap(
      nullptr, // Use nullptr instead of 0
      units * SYSTEM_PAGE_SIZE,
      PROT_READ | PROT_WRITE | PROT_EXEC,
      MAP_ANON | MAP_PRIVATE,
      -1, 0)); // -1 is used instead of 0 for file descriptor

  if (vm_page == MAP_FAILED)
  {
    std::cerr << "Error: VM Page allocation failed\n";
    return nullptr;
  }
  std::memset(vm_page, 0, units * SYSTEM_PAGE_SIZE);
  return static_cast<void *>(vm_page);
}

// Function to return a page to kernel
static void mm_return_vm_page_to_kernel(void *vm_page, int units)
{
  if (munmap(vm_page, units * SYSTEM_PAGE_SIZE) != 0)
  {
    std::cerr << "Error: Could not unmap VM page to kernel\n";
  }
}

int main()
{
  mm_init();
  cout << "VM page size is - " << SYSTEM_PAGE_SIZE << endl;
  void *addr1 = mm_get_new_vm_page_from_kernel(1);
  void *addr2 = mm_get_new_vm_page_from_kernel(1);
  cout << "page 1 = " << addr1 << " and page 2 = " << addr2 << endl;
  cout << (unsigned long)addr1 - (unsigned long)addr2 << endl;
}
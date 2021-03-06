#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <mach/mach_init.h>
#include <mach/mach_error.h>
#include <mach/mach_traps.h>
#include <mach/mach_types.h>
#include <mach/vm_map.h>
#include <mach/host_priv.h>

#define OFFSET 0x2000

vm_address_t kern_base() {
	kern_return_t ret;
	task_t kern_task;
	vm_region_submap_info_data_64_t info;
	vm_size_t size;
	mach_msg_type_number_t info_count = VM_REGION_SUBMAP_INFO_COUNT_64;
	unsigned int depth = 0;
	vm_address_t addr = 0x81200000;
	
	ret = task_for_pid(mach_task_self(), 0, &kern_task);
	if(ret != KERN_SUCCESS) {
		return 0;
	}
	
	while(1) {
		ret = vm_region_recurse_64(kern_task, &addr, &size, &depth, (vm_region_info_t)&info, &info_count);
		
		if(ret != KERN_SUCCESS) {
			break;
		}
		
		if(size > 1024*1024*1024) {
			return addr + OFFSET;
		}
		
		addr += size;
	}
	return 0;
}

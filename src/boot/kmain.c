#include <stdio.h>
#include <beryllium/video.h>
#include <terminal.h>
#include <log.h>
#include <beryllium/timer.h>
#include <system.h>
#include <stdlib.h>
#include <beryllium/vfs.h>
#include <beryllium/fs/device_man.h>
#include <beryllium/vterm.h>
#include <beryllium/thread.h>
#include <beryllium/driver.h>
#include <beryllium/watchdog.h>
#include <beryllium/kmonitor.h>
#include <beryllium/timing.h>
#include <error.h>
#include <elf.h>
elf_t kernel_elf;
extern tree_t   * device_tree;
#ifdef X86
void x86_switch_to_usermode();
#endif
/**
Kernel main function
**/
void kmain()
{
	#ifdef X86
	//Print status messages
	terminal_set_statusbar("Beryllium Unstable Isotope v. 0.0.0.4 (git)");
	#endif
	klog(LOG_INFO,"KRN","CoreLibs initialising...\n");
	#ifdef DEBUG
	klog(LOG_WARN,"KRN","Running Debug Kernel! Some things might not work properly!\n");
	#endif
	//Start Services
	timing_init();
	kmonitor_init();
    device_manager_start();
	video_graphics_init();
	vfs_init();
	init_vfs_devices();
	wd_init();
	klog(LOG_INFO,"KRN","Kernel took %dms to become fully operational!\n",timer_getHi());

	//Launch a shell

	klog(LOG_FAIL,"KRN","Kernel init rescue shell launching -- no init found!\n");
	#ifdef X86
	kshell_init();
	int i = 0;
	while(true)
	{
		kshell_parse_char(kb_read());
		wd_notify(WD_NOTIFY_KMAIN); //TODO: Make watchdog wrappers
	}
	#endif
	klog(LOG_FAIL,"KRN","No init process to start (kernel init stub not compiled)!\n");
	klog(LOG_INFO,"KRN","Logging kernel structures to serial before halting...\n");
	printf("VFS\n");
	vfs_print_tree_node(vfs_tree->root, 0);
	printf("Device Tree\n");
	device_tree_enumerate(device_tree->root, 0);
	printf("Timers\n");
	list_timers();
	printf("\n");
	while(true)
	{
		wd_notify(WD_NOTIFY_KMAIN); //TODO: Make watchdog wrappers
	}
}

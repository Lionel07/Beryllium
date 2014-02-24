#include <stdio.h>
#include <video.h>
#include <terminal.h>
#include <log.h>
#include <drivers/timer.h>
#include <version.h>
void kmain()
{
	klog(LOG_INFO,"KERN","Beryllium initialising...\n");
	#ifdef DEBUG
	klog(LOG_WARN,"KERN","This kernel is a debug kernel! Some things might not work properly!\n");
	#endif
    klog(LOG_INFO,"KERN","Finished initialising...\n");

    klog(LOG_DEBUG,"IO","Waiting for 2000 clock ticks...\n");
    while(timer_getHi()<2000)
    {

    }
	terminal_set_statusbar("Beryllium Unstable Isotope v. 0.0.0.2 (git)");
	video_graphics_init();

    while(1)
    {

    }
    //halt();
}

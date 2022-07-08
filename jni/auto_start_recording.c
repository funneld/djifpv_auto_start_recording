#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "auto_start_recording.h"


int32_t _ZN19GlassRacingChnlMenu7timeOutEv (void* this) {

	int32_t ret = 0;

	if (!timeOut && !getInstance){

		timeOut = dlsym (RTLD_NEXT, "_ZN19GlassRacingChnlMenu7timeOutEv");
		if (timeOut == 0){
			guiLib = dlopen("/system/lib/libtp1801_gui.so", 1);
			timeOut = dlsym (guiLib, "_ZN19GlassRacingChnlMenu7timeOutEv");
			if (timeOut == 0)
			{
				printf("dlsym: %s\n", dlerror());
				return 0;
			}
		}

		getInstance = dlsym (RTLD_NEXT, "_ZN17GlassUserSettings11getInstanceEv");
		if (getInstance == 0){
			if(!guiLib){guiLib = dlopen("/system/lib/libtp1801_gui.so", 1);}
			getInstance = dlsym (guiLib, "_ZN17GlassUserSettings11getInstanceEv");
			if (getInstance == 0)
			{
				printf("dlsym: %s\n", dlerror());
				return 0;
			}
		}
	}

	uint32_t inst = getInstance();

	gs_link_stat_t link = GS_LINK_STAT_UKNOWN;
	gs_link_stat_t *link_state = &link;

	uint32_t *gs_gui_config = (uint32_t *)*(uint32_t *)((int)inst + 0xe4);
	uint32_t *gs_info = (uint32_t *)*(uint32_t *)((int)gs_gui_config + 0x4c);
	gs_modem_get_link_state_wrap = (void *)*(int32_t *)((int)gs_gui_config + 0x228);
	gs_lv_transcode_record = (void *)*(int32_t *)((int)gs_gui_config + 0x3D8);

	gs_modem_get_link_state_wrap(gs_info, link_state);

	if(link != GS_LINK_STAT_NORMAL){
		clock_gettime(CLOCK_MONOTONIC, &start);
	}

	clock_gettime(CLOCK_MONOTONIC, &now);

	if((link == GS_LINK_STAT_NORMAL) && restart_record && ((now.tv_sec - start.tv_sec) > 9)){
		restart_record = false;
		gs_lv_transcode_record(gs_info, true, RECORD_BUTTON);
		printf("START RECORDING...\n");
	}
	else if(((link == GS_LINK_STAT_LOST) || (link == GS_LINK_STAT_UKNOWN)) && !restart_record){
		restart_record = true;
		printf("LINK LOST!\n");
	}


	ret = timeOut(this);

	return ret;
}

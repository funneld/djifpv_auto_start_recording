#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "auto_start_recording.h"


void _ZN19GlassRacingChnlMenu7timeOutEv(void* this) {

	if (!timeOut && !getInstance){

		timeOut = dlsym (RTLD_NEXT, "_ZN19GlassRacingChnlMenu7timeOutEv");
		if (timeOut == 0){
			if(!guiLib){guiLib = dlopen("/system/lib/libtp1801_gui.so", 1);}
			timeOut = dlsym (guiLib, "_ZN19GlassRacingChnlMenu7timeOutEv");
			if (timeOut == 0)
			{
				printf("dlsym: %s\n", dlerror());
				return;
			}
		}

		getInstance = dlsym (RTLD_NEXT, "_ZN17GlassUserSettings11getInstanceEv");
		if (getInstance == 0){
			if(!guiLib){guiLib = dlopen("/system/lib/libtp1801_gui.so", 1);}
			getInstance = dlsym (guiLib, "_ZN17GlassUserSettings11getInstanceEv");
			if (getInstance == 0)
			{
				printf("dlsym: %s\n", dlerror());
				return;
			}
		}
	}

	instance = getInstance();

	gs_gui_config = (__gs_gui_config *)*(uint32_t *)((int)instance + 0xe4);

	gs_link_stat_t link_state = GS_LINK_STAT_UKNOWN;
	gs_link_stat_t *ptr_link_state = &link_state;
	record_state_t record_state;

	gs_gui_config->gs_get_uav_power_status(gs_gui_config->gs_info, ptr_power_status, ptr_arm_flag);
	gs_gui_config->gs_modem_get_link_state_wrap(gs_gui_config->gs_info, ptr_link_state);


	if((link_state == GS_LINK_STAT_NORMAL) && restart_record) {
		restart_record = false;
		gs_gui_config->gs_lv_transcode_record(gs_gui_config->gs_info, true, RECORD_BUTTON);
		printf("START RECORDING...\n");
	}
	else if((link_state == GS_LINK_STAT_LOST) && !restart_record){
		restart_record = true;
		printf("LINK LOST!\n");
	}
	
	
/* 
	if((prev_arm_flag - arm_flag) == 1){
		int cnt = 0;
		while(cnt < 20){ //check rec state for max 2 seconds
			cnt += 1;
			record_state = gs_gui_config->gs_lv_transcode_get_state(gs_gui_config->gs_info);
			if(record_state == RECORD_STATE_IDLE){
				usleep(750000); //corrupts file without some delay
				gs_gui_config->gs_lv_transcode_record(gs_gui_config->gs_info, true, RECORD_BUTTON);
				printf("RESTART RECORDING AFTER A DISARM...\n");
				break;
			}
			usleep(100000);
		}
	}

	prev_arm_flag = arm_flag;

 */ //restart record after disarm doesn't work reliably. Leave it here for potential future use.
 
	timeOut(this);

	return;
}

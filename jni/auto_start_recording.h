#include <time.h>

typedef enum gs_link_stat_t {
    GS_LINK_STAT_NORMAL=0,
    GS_LINK_STAT_WEAK=1,
    GS_LINK_STAT_LOST=2,
    GS_LINK_STAT_UKNOWN=3
} gs_link_stat_t;

typedef enum Record_sender {
    RECORD_BUTTON=0,
    RECORD_DISARM=1
} Record_sender;

static void *guiLib = 0;
static bool restart_record = true;
static int32_t (* gs_modem_get_link_state_wrap)(void *_gs_info, enum gs_link_stat_t *) = 0;
static int32_t (* gs_lv_transcode_record)(void *_gs_info, bool b_record, Record_sender sender) = 0;
static uint32_t (* timeOut)(void *this) = 0;
static uint32_t (* getInstance)() = 0;
struct timespec start, now;

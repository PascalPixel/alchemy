#include "types.h"

struct Fields_08099810 {
    u8 filler[0x24c];
    s16 value;
};

extern struct Fields_08099810 Data_02000240;

void ScheduleCallbackAfterFrames(s32, s32);
extern u8 Data_08099679;

void BattleEffect_ScheduleCallbackWhenValue24cSet(void) {
    if (Data_02000240.value != 0) {
        ScheduleCallbackAfterFrames((s32)&Data_08099679, 0xc80);
    }
}

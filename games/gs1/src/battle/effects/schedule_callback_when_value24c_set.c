#include "types.h"
#include "scene.h"

struct Fields_08099810 {
    u8 filler[0x24c];
    s16 value;
};

extern struct Fields_08099810 gCell;

void ScheduleCallbackAfterFrames(s32, s32);
extern u8 gRom;

void BattleFx_ScheduleCallbackWhenValue24cSet(void)
{
    if (gCell.value != 0) {
        ScheduleCallbackAfterFrames((s32)&gRom, 0xc80);
    }
}

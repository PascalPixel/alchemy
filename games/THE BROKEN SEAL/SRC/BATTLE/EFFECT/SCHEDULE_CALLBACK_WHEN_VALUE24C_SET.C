#include "TYPES.H"
#include "SCENE.H"

struct Fields_08099810 {
    u8 filler[0x24c];
    s16 value;
};

extern struct Fields_08099810 gGameState;

void ScheduleCallbackAfterFrames(s32, s32);
extern u8 gRom;

void BattleFx_ScheduleCallbackWhenValue24cSet(void)
{
    if (gGameState.value != 0) {
        ScheduleCallbackAfterFrames((s32)&gRom, 0xc80);
    }
}

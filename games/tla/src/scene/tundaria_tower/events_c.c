#include "tundaria_tower.h"

void Func_02000fb8(s32 mode, s32 cue)
{
    if (cue == 0) {
        Func_02002e00();
        Scheduler_RemoveCallbackFar(Func_0200137c);
    }
    if (mode == 1) {
        Func_02002fc0(13, 26, 13, 16, 3, 3);
        Call6((void (*)())Func_02002fe8, 13, 26, 3, 3, 13, 16);
        GameFlag_SetBit(0x948);
    }
    if (mode == 2)
        Func_02002d44(0xe80000, 0x100000, 0x1180000, 2);
    if (mode == 3)
        Func_02002d44(0xe80000, 0x100000, 0x1180000, 30);
    if (cue == 0x3e7)
        Func_02002ebc();
}

const struct SceneEvent *Func_02001044(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov68fRoom1)
        return (const struct SceneEvent *)gData_020037f8;
    if (gPartyState.scene == (s32)&SceneId_Ov68fRoom2)
        return (const struct SceneEvent *)gData_0200381c;
    if (gPartyState.scene == (s32)&SceneId_Ov68fRoom3)
        return (const struct SceneEvent *)gData_0200393c;
    if (gPartyState.scene == (s32)&SceneId_Ov68fRoom4)
        return (const struct SceneEvent *)gData_020039e4;
    if (gPartyState.scene == (s32)&SceneId_Ov68fRoom5)
        return (const struct SceneEvent *)gData_02003a5c;
    if (gPartyState.scene == (s32)&SceneId_Ov68fRoom6)
        return (const struct SceneEvent *)gData_02003b28;
    if (gPartyState.scene == (s32)&SceneId_Ov68fRoom7)
        return (const struct SceneEvent *)gData_02003c90;
    return (const struct SceneEvent *)gData_0200381c;
}

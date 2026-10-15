#include "trial_road.h"

extern const u8 Data_02007044[];
extern const u8 Data_02007cf8[];
extern const u8 Data_02007d40[];
extern const u8 Data_02007d94[];
extern const u8 *const Data_02007e00[];

const u8 *Func_02001890(void)
{
    s32 pattern = gTrialRoadState->pattern;

    if (gPartyState.scene == (s32)&SceneId_Ov697Room1)
        return Data_02007e00[pattern & 7];
    if (gPartyState.scene == (s32)&SceneId_Ov697Room3)
        return Data_02007cf8;
    if (gPartyState.scene == (s32)&SceneId_Ov697Room4)
        return Data_02007d40;
    if (gPartyState.scene == (s32)&SceneId_Ov697Room2)
        return Data_02007d94;
    return Data_02007044;
}

void Func_02001904(void)
{
    if (!GameFlag_Test(0x307))
        OBJECT_BYTE(Object_GetById(17), 98) = 1;
    else
        Func_020058e0(20, 0, 0);
    Func_020006b4(15);
    Func_020006b4(16);
    Func_020006b4(17);
    Func_020006b4(18);
    Func_020006b4(19);
    Func_0200062c(16);
    Func_0200062c(17);
    Func_0200062c(18);
    Func_0200062c(19);
    {
        /* FAKEMATCH: a temporary keeps the priority constant ahead of the callback load (differ 2 without it). */
        s32 priority = 0x480;

        Scheduler_AddOrUpdateCallback(Func_020007b4, priority);
    }
}

void Func_02001974(void)
{
    Func_020006b4(15);
    Func_020006b4(16);
    Func_020006b4(17);
    Func_020006b4(18);
    Func_020006b4(19);
    Func_020006b4(20);
    Func_0200062c(15);
    Func_0200062c(16);
    Func_0200062c(17);
    Func_0200062c(18);
    Func_0200062c(19);
    Func_0200062c(20);
    OBJECT_BYTE(Object_GetById(17), 98) = 1;
    OBJECT_BYTE(Object_GetById(18), 98) = 1;
    OBJECT_BYTE(Object_GetById(19), 98) = 1;
    OBJECT_BYTE(Object_GetById(20), 98) = 1;
    if (!GameFlag_Test(0x214))
        Object_SetModeById(17, 5);
    else
        Func_0200070c(17, -16);
    if (!GameFlag_Test(0x215))
        Object_SetModeById(18, 5);
    else
        Func_0200070c(18, -16);
    if (!GameFlag_Test(0x216))
        Object_SetModeById(19, 5);
    else
        Func_0200070c(19, -16);
    if (!GameFlag_Test(0x217))
        Object_SetModeById(20, 5);
    else
        Func_0200070c(20, -16);
}

void Func_02001a70(void)
{
    if (!GameFlag_Test(0x327))
        OBJECT_BYTE(Object_GetById(15), 98) = 1;
    else
        Func_020058e0(20, 0, 0);
    if (!GameFlag_Test(0x328))
        OBJECT_BYTE(Object_GetById(16), 98) = 1;
    else
        Func_020058e0(21, 0, 0);
    Func_020006b4(15);
    Func_020006b4(16);
    Func_020006b4(17);
    Func_020006b4(18);
    Func_020006b4(19);
    Func_0200062c(15);
    Func_0200062c(16);
    Func_0200062c(17);
    Func_0200062c(18);
    Func_0200062c(19);
}

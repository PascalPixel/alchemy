#include "types.h"

extern u8 *Data_03001ebc;

void Func_02002344(s32, s32, s32);
s32 Func_0200259e(s32);
void Func_020025a8(void);
s32 Func_020025b4(s32);
void Func_02002602(void);
void Func_0200261e(s32, s32);
void Func_02002638(s32, s32);
void Func_02002646(s32);
void Func_02002670(s32, s32);

#define ActorPresentation_RunActorNineteenDialogueAndSetSceneState Func_02000bec

void ActorPresentation_RunActorNineteenDialogueAndSetSceneState(void)
{
    Func_020025a8();
    Func_02002646(0x1342);
    Func_0200261e(19, 0);
    Func_02002344(19, 0, 2);
    Func_02002670(19, 0);
    Func_02002638(19, 1);
    if (Func_0200259e(231) != -1 && Func_020025b4(0x858) == 0) {
        u16 *scene_state = (u16 *)(Data_03001ebc + 370);
        u16 value = 1;

        *scene_state = value;
    }
    Func_02002602();
}

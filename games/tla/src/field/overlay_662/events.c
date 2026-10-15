#include "ov662.h"

u32 *Func_02000d44(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov662Room2)
        return (u32 *)Data_02001bd0;
    if (gPartyState.scene == (s32)&SceneId_Ov662Room3)
        return (u32 *)Data_02001c18;
    return Data_02001b58;
}

void Func_02000d84(void)
{
    struct Ov662Rect rect;

    if (Func_020001d4(&rect))
        Func_02000458(rect);
}

void Func_02000dac(void)
{
    Func_02000898();
    Func_02001958();
    {
        u8 *party = (u8 *)&gPartyState;

        /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
        do {
        } while (0);
        party[0x2c2] = 2;
    }
    Func_020018d8((s32)&SceneId_Ov662Room1, 10);
    Battle_WaitMode0(40);
    Func_020008fc();
}

void Func_02000de0(void)
{
    Func_02000898();
    Func_02001958();
    {
        u8 *party = (u8 *)&gPartyState;

        /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
        do {
        } while (0);
        party[0x2c2] = 2;
    }
    Func_020018d8((s32)&SceneId_Ov662Room1, 9);
    Battle_WaitMode0(40);
    Func_020008fc();
}

#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void Func_020018a8();
void GameFlag_SetBit();
void GameFlag_Test();

void Func_02000e14(s32 arg0)
{
    s32 result0;

    result0 = Value1((s32 (*)())GameFlag_Test, 0x300);
    if (result0 != 0) goto L_02008e3a;
    if (arg0 != 3) goto L_02008e3a;
    Value3((s32 (*)())Func_020018a8, 11, 0, 0);
    Call1((void (*)())GameFlag_SetBit, 0x300);
L_02008e3a:
    return;
}

void Func_02000e3c(s32 arg0)
{
    s32 result0;

    result0 = Value1((s32 (*)())GameFlag_Test, 0x301);
    if (result0 != 0) goto L_02008e66;
    if (arg0 != 3) goto L_02008e66;
    Call3((void (*)())Func_020018a8, 15, 0, 0);
    Value1((s32 (*)())GameFlag_SetBit, 0x301);
L_02008e66:
    return;
}

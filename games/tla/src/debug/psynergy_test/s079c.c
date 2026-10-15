#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "call.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
s32 Item_AdjustCounter(s32 item, s32 delta);
void Func_02000dac(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02000dc4(struct ObjectRuntime *object, s32 value);
void Func_02000e74(s32 actor, s32 x, s32 z);
void Func_02000ed4(s32 actor, s32 angle);
void Func_02000edc(s32 actor, s32 angle, s32 frames);
void Func_02000f04(s32 actor);
void Func_02000f0c(s32 actor);

s32 Func_0200079c(void)
{
    u8 *work = Ram_HeapSlots->event_work;

    /* FAKEMATCH: the turns and placements go through call.h's Call2 and Call3, which build each call's constants again (direct: differ 2, 8 and 18), and a one-pass block between the two entrance tests keeps the second a load of its own, as in overlay 657's s0b84.c (without it gcse shares one halfword load, differ 18). */
    *(s32 *)(work + 428) = 0x100;
    work[3383] = 0;
    work[3382] = 0;
    Item_AdjustCounter(5, 1);
    Item_AdjustCounter(5, 1);
    Item_AdjustCounter(5, 1);
    Item_AdjustCounter(6, 1);
    Item_AdjustCounter(6, 1);
    Item_AdjustCounter(7, 1);
    Item_AdjustCounter(106, 1);
    Item_AdjustCounter(108, 1);
    Item_AdjustCounter(109, 1);
    Item_AdjustCounter(113, 1);
    Item_AdjustCounter(123, 1);
    Item_AdjustCounter(130, 1);
    Item_AdjustCounter(140, 1);
    Item_AdjustCounter(151, 1);
    gPartyState.event_mode = 2;
    Object_SetModeById(15, 2);
    Object_SetModeById(16, 2);
    Call3((void (*)())Func_02000edc, 9, 0x8000, 0);
    Call2((void (*)())Func_02000ed4, 10, 0x8000);
    Call2((void (*)())Func_02000ed4, 12, 0);
    if (GameFlag_Test(0x300))
        Func_02000dac(36, 28, 1, 1, 10, 10);
    if (GameFlag_Test(0x301))
        Call3((void (*)())Func_02000e74, 18, 184 << 16, 184 << 16);
    if (GameFlag_Test(0x302) && !GameFlag_Test(0x310))
        Call3((void (*)())Func_02000e74, 19, 200 << 16, 200 << 16);
    Func_02000dc4(Object_GetById(19), 50);
    {
        struct PartyState *party = &gPartyState;

        if (party->entrance == 6)
            Func_02000f04(party->current_owner);
        do {
        } while (0);
        if (party->entrance == 8)
            Func_02000f0c(party->current_owner);
    }
    return 0;
}

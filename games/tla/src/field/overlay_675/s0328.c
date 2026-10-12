#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct Ov675EventWork {
    u8 unknown_000[0x1ac];
    s32 value_1ac;
};

struct ObjectRuntime *Object_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000158(void);
void Func_02000178(void);
void Func_020012ec(s32 a, s32 b);
void Func_020012fc(s32 a);
void Func_0200132c(void);

static __inline__ void Func_020012ec_(s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020012ec(a, b);
}

void Func_02000328(void)
{
    struct Ov675EventWork *work = Ram_HeapSlots->event_work;
    s32 saved;

    /* FAKEMATCH: a one-pass block loads the event work before its offset is built (one reordered row). */
    do {
    } while (0);
    saved = work->value_1ac;
    Func_020012ec_(0x202108, 1);
    Func_020012ec_(0x10000, 2);
    ((struct Ov675EventWork *)Ram_HeapSlots->event_work)->value_1ac = 0x100;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02000178();
    Func_0200132c();
    WaitFrames(40);
    Func_02000158();
    Func_020012ec_(0x10000, 1);
    Func_020012fc(16);
    WaitFrames(16);
    {
        struct PartyState *party = &gPartyState;

        /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
        do {
        } while (0);
        party->unknown_2c2 = 0;
    }
    work->value_1ac = saved;
}


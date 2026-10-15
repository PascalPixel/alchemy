#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct MarsEntranceEventWork {
    u8 unk000[0x1ac];
    s32 value1ac;
};

void WaitFrames(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02004718(void);
void Func_02004738(void);
void Func_02004d58(s32 a, s32 b);
void Func_02004d60(s32 a);
void Func_02004dc8(void);

static __inline__ void Func_02004d58_(s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02004d58(a, b);
}

void Func_020048e8(void)
{
    struct MarsEntranceEventWork *work = Ram_HeapSlots->eventWork;
    s32 saved;

    /* FAKEMATCH: a one-pass block loads the event work before its offset is built (one reordered row). */
    do {
    } while (0);
    saved = work->value1ac;
    Func_02004d58_(0x202108, 1);
    Func_02004d58_(0x10000, 2);
    ((struct MarsEntranceEventWork *)Ram_HeapSlots->eventWork)->value1ac = 0x100;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02004738();
    Func_02004dc8();
    WaitFrames(40);
    Func_02004718();
    Func_02004d58_(0x10000, 1);
    Func_02004d60(16);
    WaitFrames(16);
    {
        struct PartyState *party = &gPartyState;

        /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
        do {
        } while (0);
        party->unk2c2 = 0;
    }
    work->value1ac = saved;
}

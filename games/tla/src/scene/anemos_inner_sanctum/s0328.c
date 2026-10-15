#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct AnemosInnerSanctumEventWork
{
    u8  unk000[0x1ac];
    s32 value1ac;
};

struct ObjectRuntime *Object_GetById(s32 id);
void                  GameFlag_SetBit(s32 flag);
void                  WaitFrames(s32 frames);
void                  Event_StartScreenTransition(void);
void                  Event_WaitTransitionFrames(void);
void                  Func_02000158(void);
void                  Func_02000178(void);
void                  Func_020017b4(void);
void                  Func_02003458(s32 a, s32 b);
void                  Func_02003468(s32 a);
void                  Func_020034a8(void);

static __inline__ void Func_02003458_(s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02003458(a, b);
}

void Func_02000328(void)
{
    struct AnemosInnerSanctumEventWork *work = Ram_HeapSlots->eventWork;
    s32                                 saved;

    /* FAKEMATCH: a one-pass block loads the event work before its offset is built (one reordered row). */
    do
    {
    } while (0);
    saved = work->value1ac;
    Func_02003458_(0x202108, 1);
    Func_02003458_(0x10000, 2);
    ((struct AnemosInnerSanctumEventWork *)Ram_HeapSlots->eventWork)->value1ac = 0x100;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02000178();
    Func_020034a8();
    WaitFrames(40);
    Func_02000158();
    Func_02003458_(0x10000, 1);
    Func_02003468(16);
    WaitFrames(16);
    {
        struct PartyState *party = &gPartyState;

        /* FAKEMATCH: a one-pass block loads the party state before its offset is built (one reordered row). */
        do
        {
        } while (0);
        party->unk2c2 = 0;
    }
    work->value1ac = saved;
}

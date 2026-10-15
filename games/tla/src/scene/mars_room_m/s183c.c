#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct MarsRoomMEventWork1ac {
    u8 unknown_000[0x1ac];
    s32 value_1ac;
};

struct ObjectRuntime *Object_GetById(s32 id);
void GameFlag_SetBit(s32 flag);
void WaitFrames(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200166c(void);
void Func_0200168c(void);
void Func_020017b4(void);
void Func_02003248(s32 a, s32 b);
void Func_02003258(s32 a);
void Func_020032a0(void);

static __inline__ void Func_02003248_(s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02003248(a, b);
}

void Func_0200183c(void)
{
    struct MarsRoomMEventWork1ac *work = Ram_HeapSlots->event_work;
    s32 saved;

    /* FAKEMATCH: a one-pass block loads the event work before its offset is built (one reordered row). */
    do {
    } while (0);
    saved = work->value_1ac;
    Func_02003248_(0x202108, 1);
    Func_02003248_(0x10000, 2);
    ((struct MarsRoomMEventWork1ac *)Ram_HeapSlots->event_work)->value_1ac = 0x100;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_0200168c();
    Func_020032a0();
    WaitFrames(40);
    Func_0200166c();
    Func_02003248_(0x10000, 1);
    Func_02003258(16);
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


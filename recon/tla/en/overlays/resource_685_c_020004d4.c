/* Draft, differ 49 of 112: the native builds 513 from the 0x1ac offset (adds r2, #85) and rebuilds 0x214 for the owner, where this draft builds 513 apart and reaches 0x214 from 0x1ac; the entrance == 99 test compiles as a halfword shifted compare after a hoisted ldrh, where the native reloads with ldrsh and compares 99. Tried gPartyState directly and a chained store. */
#include "../../../../games/tla/src/scene/champa_cliff_caves/scene.h"
#include "scene_ids.h"

struct EventTransition
{
    u8 unknown_000[0x1ac];
    s32 type;
};

void Func_02001774(void);
void Func_02001868(void);
void Func_020034d4(s32 actor, s32 x, s32 z);
void Func_020035ac(s32 a);
void Func_0200361c(s32 a, s32 b);

static __inline__ void Func_020034d4_(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020034d4(actor, x, z);
}

static __inline__ void Func_0200361c_(s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_0200361c(a, b);
}

s32 Func_020004d4(void)
{
    struct PartyState *party = &gPartyState;

#if defined(TLA_EDITION_JA)
    ((struct EventTransition *)Ram_HeapSlots->event_work)->type = 516;
#else
    ((struct EventTransition *)Ram_HeapSlots->event_work)->type = 513;
#endif
    if (party->scene != (s32)&SceneId_Ov685Room1)
        Object_GetById(party->current_owner)->priority_flags |= 32;
    Func_020035ac(0);
    if (party->scene == (s32)&SceneId_Ov685Room4)
    {
        if (party->entrance == 98)
            Func_02001774();
        if (party->entrance == 99)
        {
            party->secondary = 23;
            party->entrance = 23;
            Func_02001868();
        }
        if (GameFlag_Test(0x977) && !GameFlag_Test(0x978))
        {
            Func_0200361c_(10, 326);
            Func_020034d4_(10, 0x2980000, 0xe80000);
            Object_GetById(10)->motion_flags = 4;
        }
        if (GameFlag_Test(0x977))
            Func_020034d4_(8, 0x2980000, 0xc80000);
    }
    else if (party->scene == (s32)&SceneId_Ov685Room3)
    {
        if (GameFlag_Test(0x97e))
        {
            Object_SetModeById(8, 11);
            ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 0);
        }
    }
    return 0;
}

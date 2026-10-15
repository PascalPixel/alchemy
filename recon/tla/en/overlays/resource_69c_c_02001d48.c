/* Drafts of overlay 69C Func_02001d48 and Func_02001db8 (first written 2026-10-12); overlay 6A3 has the same two
 * functions at 0200c13c and 0200c1ac.
 * Func_02001d48: differ 2 of 49 (one reordered row): the native loads object->x (ldr r6, [r0, #8]) before it copies
 * the mask 64 for the first and; the draft copies first. Measured: one statement or two for each cell, one-pass
 * blocks on the x load, on both loads and on the masked event x (differ 4, 33, 57).
 * Func_02001db8: differ 18 of 131, order only, at four places: the base and offset registers of the first
 * currentOwner address are exchanged; the pool zero for motionFlags loads between the two halves of 0x4000 in the
 * native and after the angle store here; the branch over the literal pool sits one instruction earlier in the
 * native; and the last block builds its zero before the animationKind address. What the bytes needed so far: x
 * reused as the loop counter (differ 83 to 34), a word temporary for 0xf1f, the mask in a variable set before a
 * one-pass boundary after the x shift, and a one-pass boundary after the WINOUT store. Those three devices need
 * FAKEMATCH tags with these reasons when the function is adopted. */
#include "../../../../games/tla/src/scene/jupiter_room_3/local.h"
#include "io_reg.h"

struct PushEvent {
    u8 unk00[6];
    s16 x;
    s16 z;
    u8 unk0a[4];
    u16 busy;
};

void Func_02001cc0(void);
void Func_02002328(s32 x, s32 z, s32 a, s32 b, s32 cellX, s32 cellZ);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void Func_02001d48(struct PushEvent *event)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
    s32 x;
    s32 z;

    x = object->x >> 20;
    x |= (s16)(event->x & 64);
    z = object->z >> 20;
    z |= (s16)(event->z & 64);

    Func_02001cc0();
    Func_02002470(161);
    Func_02002328(event->x, event->z + 1, 1, 1, x, z);
    Battle_WaitMode0(12);
}

void Func_02001db8(struct PushEvent *event)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
    s32 x;
    s32 z;
    s32 mask;

    mask = 64;
    x = object->x >> 20;
    do { } while (0);
    x |= (s16)(event->x & mask);
    z = object->z >> 20;
    z |= (s16)(event->z & mask);
    Func_02001cc0();
    Func_02002470(229);
    Func_02002328(event->x, event->z + 2, 1, 1, x, z);
    Battle_WaitMode0(12);
    {
        u32 zero = 0;

        object->angle = 0x4000;
        object->motionFlags = zero;
    }
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner), 0);
    {
        /* FAKEMATCH: a word temporary builds 0xf1f with movs, lsls, adds as the native does; a halfword constant store loads it from the pool. */
        u32 winout = 0xf1f;

        REG_WINOUT = winout;
    }
    do { } while (0);
    REG_DISPCNT |= 0x8000;
    event->busy = 1;
    Object_SetModeById(gPartyState.currentOwner, 28);
    Battle_WaitMode0(16);
    for (x = 0; x < 40; x++) {
        if (x == 5)
            Func_02002470(204);
        object->scaleX += -0x400;
        object->scaleY += -0x300;
        object->y += -0x9999;
        WaitFrames(1);
    }
    Object_GetById(gPartyState.currentOwner)->animationKind = 0;
    event->busy = 0;
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
}

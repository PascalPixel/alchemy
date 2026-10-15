#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct EventWork154 {
    u8 unknown_000[0x154];
    s16 value;
};

struct LohoPicture {
    const void *data;
    u16 a;
    u16 b;
};

extern const struct LohoPicture Data_02001f14;

void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020012a0(const void *data, s32 a, s32 b);
void Func_020012a8(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y);
void Func_020012e0(void);
void Func_020012e8(void);
void Func_02001348(s32 id, s32 x, s32 z);
void Func_020013d0(s32 value);
void Func_020013f8(s32 value);
void Func_02001448(s32 cue);

static __inline__ void Func_020012a0_(const void *data, s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020012a0(data, a, b);
}

static __inline__ void Func_020012a8_(s32 src_x, s32 src_y, s32 width, s32 height, s32 dest_x, s32 dest_y)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020012a8(src_x, src_y, width, height, dest_x, dest_y);
}

static __inline__ void Func_02001348_(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001348(id, x, z);
}

static __inline__ void Offset_(s32 id, s32 dx, s32 dz)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_OffsetPositionAndResetMotion(id, dx, dz);
}

void Func_02000eec(void)
{
    struct PartyState *party = &gPartyState;
    struct EventWork154 *work = Ram_HeapSlots->event_work;

    /* FAKEMATCH: a one-pass block loads the party state before the event work (one reordered row). */
    do {
    } while (0);
    Object_SetModeById(party->current_owner, 2);
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x8000, 0x4000);
    Func_02001448(123);
    Offset_(gPartyState.current_owner, 0, -4);
    Func_020013d0(work->value);
}

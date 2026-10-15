#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "object_runtime.h"
#include "party_state.h"

struct EventWork154 {
    u8 unk000[0x154];
    s16 exit;
};

struct ObjectRuntime *Object_GetById(s32 id);
void Func_020056b4(struct ObjectRuntime *object);
void Func_0200580c(s32 value);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndReset(s32 id, s32 x, s32 z);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 frames);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Func_0200565c(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_02005664(struct ObjectRuntime *object);
void Func_02005854(s32 sound);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020057c4(s32 exit);

static __inline__ void Actor_SetSpeed(s32 id, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Actor_Walk(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetPositionAndReset(id, x, z);
}

static __inline__ void Actor_Turn(s32 id, s32 angle, s32 frames)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_ArmCallback(id, angle, frames);
}

static __inline__ void Actor_Place(struct ObjectRuntime *object, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_0200565c(object, x, y, z);
}

// 穴へ飛びおりる
void Func_02001630(void)
{
    struct ObjectRuntime *object = Object_GetById(gPartyState.currentOwner);
    s32 x = object->x >> 20;
    s32 z = object->z >> 20;
    struct ObjectRuntime *leader;
    struct EventWork154 *work;

    if (x == 28 && z == 9 && !(gInput.held & 0x10))
        return;
    if (x == 29 && z == 10 && !(gInput.held & 0x40))
        return;
    work = Ram_HeapSlots->eventWork;
    leader = Object_GetById(gPartyState.currentOwner);
    Func_020056b4(leader);
    Func_0200580c(0);
    leader->motionFlags = 0;
    Actor_SetSpeed(gPartyState.currentOwner, 0x10000, 0x8000);
    Actor_Walk(gPartyState.currentOwner, 472, 144);
    Actor_Turn(gPartyState.currentOwner, 0xc000, 0);
    Object_RefreshSelectorById(gPartyState.currentOwner);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner), 0);
    Object_SetModeById(gPartyState.currentOwner, 13);
    Actor_Place(leader, 472 << 16, 0, 156 << 16);
    Func_02005664(leader);
    Object_SetModeById(gPartyState.currentOwner, 10);
    Func_02005854(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_020057c4(work->exit);
}

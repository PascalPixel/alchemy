/* differ 2: after Func_02004d78 the native loads the event work before it starts the offset of the frame count (which it keeps in r6); here the offset starts first. A barrier after the load gives that order but cuts the later calls from the arguments of Func_02004d78, which then load r1 last (differ 2 again). */
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"

struct EventTransition {
    u8 unknown_000[0x1ac];
    s32 type;
    u8 unknown_1b0[4];
    s32 frames;
};

extern const u8 Data_02004eb8[];

struct ObjectRuntime *Object_GetById(s32 id);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Object_AttachWorkTargetToObject(s32 actor, s32 target);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const u8 *action);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_020028cc(void);
void Func_02004c20(void);
void Func_02004d40(s32 value);
void Func_02004d78(s32 a, s32 b);
void Func_02004db8(s32 value);
void Func_02004df8(s32 value, s32 mode);

static __inline__ void ObjectMotion_SetSpeedParameters_(s32 actor, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetSpeedParameters(actor, speed, acceleration);
}

static __inline__ void Func_02004d78_(s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02004d78(a, b);
}

static __inline__ void Func_02004df8_(s32 value, s32 mode)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02004df8(value, mode);
}

void Func_020029a4(void)
{
    struct ObjectRuntime *object = Object_GetById(8);

    Battle_WaitMode0(60);
    Func_02004c20();
    Func_02004db8(0);
    Func_02004df8_(0x9999, 1);
    object->scale_x = 0x13333;
    object->scale_y = 0x13333;
    Object_AttachWorkTargetToObject(8, 1);
    WaitFrames(1);
    Object_SetPartAttribute(Object_GetById(gPartyState.current_owner), 15);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.current_owner), 0);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(8), 0);
    ObjectMotion_SetSpeedParameters_(8, 0x6666, 0x3333);
    object->action = 0;
    ObjectMotion_EnableActionAndSetCallback(8, Data_02004eb8);
    Scheduler_AddOrUpdateCallback(Func_020028cc, 0x480);
    ((struct EventTransition *)Ram_HeapSlots->event_work)->type = 256;
    Func_02004d78_(0x10003, 1);
    ((struct EventTransition *)Ram_HeapSlots->event_work)->frames = 32;
    Event_StartScreenTransition();
    Battle_WaitMode0(120);
    Func_02004df8_(0x16666, 300);
    Battle_WaitMode0(270);
    ((struct EventTransition *)Ram_HeapSlots->event_work)->frames = 16;
    *(u16 *)0x05000000 = 0x7fff;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02004d40(77);
}

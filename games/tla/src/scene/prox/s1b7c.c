#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct EventWork154 {
    u8 unknown_000[0x154];
    s16 return_entrance;
};

struct ObjectRuntime *Object_GetById(s32 id);
void WaitFrames(s32 frames);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200463c(s32 cue);
void Func_020043f4(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_020043fc(struct ObjectRuntime *object);
void Func_0200458c(s32 entrance);

void Func_02001b7c(void)
{
    struct EventWork154 *work = Ram_HeapSlots->event_work;
    s32 entrance = work->return_entrance;
    struct ObjectRuntime *object = Object_GetById(gPartyState.current_owner);
    s32 cell;
    s32 id;

    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x8000, 0x4000);
    cell = object->x >> 16;
    Func_020043f4(object, ((8 - cell % 16) << 16) + object->x, object->y, object->z - 0x40000);
    Func_020043fc(object);
    object->angle = 0xc000;
    Func_0200463c(128);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.current_owner), 0);
    id = gPartyState.current_owner;
    if (entrance == 6)
        Object_SetModeById(id, 25);
    else
        Object_SetModeById(id, 26);
    WaitFrames(4);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_0200458c(entrance);
}

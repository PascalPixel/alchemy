#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct EventWork154 {
    u8 unk000[0x154];
    s16 returnEntrance;
};

extern const u8 gData_020046f8[];

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_EnableActionAndSetCallback(s32 id, const void *script);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02004464(void);
void Func_020045ec(s32 skip);
void Func_0200463c(s32 cue);
void Func_020043f4(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Func_020043fc(struct ObjectRuntime *object);
void Func_02004404(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_0200458c(s32 entrance);

static __inline__ s32 Flag_Test(s32 flag)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    return GameFlag_Test(flag);
}

static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02004404(a, b, c, d, e, f);
}

static __inline__ void Wait(s32 frames)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    WaitFrames(frames);
}

static __inline__ void Cue(s32 cue)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_0200463c(cue);
}

void Func_020019a4(s32 a, s32 b, s32 c, s32 d, s32 mode, s32 f)
{
    struct EventWork154 *work = Ram_HeapSlots->eventWork;
    struct ObjectRuntime *object;
    s32 cell;

    Func_02004464();
    Func_020045ec(0);
    if (!Flag_Test(0x200)) {
        if (mode == 2)
            Cue(188);
        else
            Cue(158);
        Map_CopyCells(a, b, c, d, mode, f);
        Battle_WaitMode0(20);
    }
    ObjectMotion_SetSpeedParameters(gPartyState.currentOwner, 0x8000, 0x4000);
    if (mode == 1) {
        object = Object_GetById(gPartyState.currentOwner);
        cell = object->x >> 16;
        Func_020043f4(object, ((8 - cell % 16) << 16) + object->x, object->y, object->z);
        Func_020043fc(object);
        object->angle = 0xc000;
    }
    ObjectMotion_EnableActionAndSetCallback(gPartyState.currentOwner, gData_020046f8);
    Wait(12);
    Cue(123);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_0200458c(work->returnEntrance);
}

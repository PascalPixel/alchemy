#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Func_020056b4(struct ObjectRuntime *object);
void Func_020056bc(void);
void Func_0200580c(s32 value);
void Func_02005854(s32 sound);
void ObjectMotion_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);
void Battle_WaitMode0(s32 frames);

static __inline__ void Actor_SetSpeed(s32 id, s32 speed, s32 acceleration)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetSpeedParameters(id, speed, acceleration);
}

static __inline__ void Actor_SetPosition(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    ObjectMotion_SetPositionAndCommit(id, x, z);
}

void Func_02001728(struct ObjectRuntime *actor)
{
    Func_020056b4(actor);
    Func_0200580c(0);
    Func_02005854(581);
    Actor_SetSpeed(gPartyState.current_owner, 0x19999, 0x19999);
    Actor_SetPosition(gPartyState.current_owner, 152, 424);
    Func_020056bc();
}

void Func_0200176c(void)
{
    struct ObjectRuntime *actor = Object_GetById(gPartyState.current_owner);
    s32 speed;

    Func_020056b4(actor);
    Func_0200580c(0);
    Func_02005854(581);
    actor->motion_flags = 0;
    speed = 0x19999;
    /* FAKEMATCH: the native loads the speed before the owner, whose base is already in a register here; the plain call loads the owner first (one reordered pair, differ 2) */
    asm volatile("" : "+l"(speed));
    Actor_SetSpeed(gPartyState.current_owner, speed, 0x19999);
    Actor_SetPosition(gPartyState.current_owner, 152, 488);
    actor->motion_flags = 3;
    Battle_WaitMode0(8);
    Func_02005854(161);
    Object_SetModeById(gPartyState.current_owner, 22);
    Battle_WaitMode0(20);
    Func_020056bc();
}

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

void Func_02001728(struct ObjectRuntime *actor)
{
    s32 acceleration;
    s32 z;
    s32 x;
    s32 owner;

    Func_020056b4(actor);
    Func_0200580c(0);
    Func_02005854(581);
    acceleration = 0x19999;
    /* FAKEMATCH: ⚓️ loads the speed and the acceleration from the pool separately, as the slides in slide.c do */
    asm("" : "+l"(acceleration));
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, 0x19999, acceleration);
    z = 212;
    /* FAKEMATCH: the native starts the z, loads the owner and the x, and doubles the z last; the plain constant 424 is doubled before the x is built (one reordered pair, differ 2), and so it is with either barrier alone */
    asm volatile("" : "+l"(z));
    x = 152;
    owner = gPartyState.current_owner;
    /* FAKEMATCH: the second barrier of the pair above: the owner and the x are loaded before the z is doubled */
    asm volatile("" : "+l"(x), "+l"(owner));
    ObjectMotion_SetPositionAndCommit(owner, x, z << 1);
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
    /* FAKEMATCH: the native loads the speed, the owner and the acceleration in that order, the two constants from the pool separately; the plain call loads the owner first and shares the constant */
    asm volatile("" : "+l"(speed));
    ObjectMotion_SetSpeedParameters(gPartyState.current_owner, speed, 0x19999);
    ObjectMotion_SetPositionAndCommit(gPartyState.current_owner, 152, 488);
    actor->motion_flags = 3;
    Battle_WaitMode0(8);
    Func_02005854(161);
    Object_SetModeById(gPartyState.current_owner, 22);
    Battle_WaitMode0(20);
    Func_020056bc();
}

/* Draft, differ 22 of 177: written from the listing (☀️'s FieldScene_RunSupplementalSequenceOne is
 * its twin). Left: the options pointer and i take r10 and r8 where the native has r8 and r10;
 * the velocity array's address is built before the cosine call, not after it; and both random
 * terms load the velocity word one row later than the native. The inlines keep the -1 and
 * 0x50000 constants per call, as ☀️'s scene layer does (differ 59 called directly). Tried:
 * declaration orders, a do-while loop, velocity at function scope, the random terms as separate
 * subtractions, permuter 4 minutes (24). */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_effect.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Func_020056b4(void);
void Func_0200580c(s32 value);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_0200564c(void);
void WaitFrames(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02005854(s32 sound);
void Battle_WaitMode0(s32 frames);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
u32 Random16Far(void);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_020000b8(s32 x, s32 y, s32 z, s32 velocity_x, s32 velocity_y, s32 velocity_z,
    u32 flags, struct EffectOptions *options);
void Func_02005794(s32 id, s32 value);
void Func_02005694(s32 a, s32 b, s32 c);
void Func_0200569c(void);
void Func_020056bc(void);

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode)
{
    Motion_CamBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c)
{
    Func_02005694(a, b, c);
}

void Func_02002394(void)
{
    struct ObjectRuntime *actor = Object_GetById(gPartyState.current_owner);
    u32 i;
    struct EffectOptions options;

    Func_020056b4();
    Func_0200580c(0);
    CamBounds(-1, -1, -1, 0);
    Func_0200564c();
    WaitFrames(1);
    actor->y = 0x820000;
    actor->gravity = 0x8000;
    actor->bounce = 0;
    actor->motion_flags = 0;
    *(s32 *)((u8 *)Ram_HeapSlots->event_work + 0x1ac) = 0x100;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02005854(204);
    actor->motion_flags = 3;
    Battle_WaitMode0(24);
    options.palette = 7;
    options.update = Effect_AdvanceMotion;
    options.start_scale_x = 0xcccc;
    options.start_scale_y = 0xcccc;
    for (i = 0; i <= 16; i++) {
        s32 angle;
        s32 velocity[3];

        angle = i << 12;
        velocity[0] = Math_Cosine(angle);
        velocity[1] = 0;
        velocity[2] = Math_Sine(angle);
        velocity[0] += velocity[0] >> 1;
        velocity[0] += (Random16Far() * 0xc000 >> 16) - 0x6000;
        velocity[2] += (Random16Far() * 0x6000 >> 16) - 0x3000;
        Func_020000b8(actor->x, actor->y, actor->z, velocity[0], velocity[1], velocity[2], 0x1090001, &options);
    }
    Func_02005854(188);
    Func_02005794(gPartyState.current_owner, 0x101);
    Object_SetModeById(gPartyState.current_owner, 49);
    SetWorkTriple(0x50000, 0x50000, 0x10000);
    SetWorkTriple(-1, -1, 0xe666);
    Func_0200569c();
    Func_02005794(gPartyState.current_owner, 0x100);
    actor->gravity = 0x10000;
    actor->bounce = 0x4000;
    WaitFrames(10);
    Object_SetModeById(gPartyState.current_owner, 1);
    *(s32 *)((u8 *)Ram_HeapSlots->event_work + 0x1ac) = 0x209;
    Func_020056bc();
}

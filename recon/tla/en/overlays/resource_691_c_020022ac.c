/* Draft, differ 28 of 174 (left uncommitted by an earlier worker, kept under
 * S4): the leader lands and 17 dust effects spread around it. Left: the
 * options pointer and the loop counter take r10 and r8 the other way round,
 * and the two random terms' pool constants load one row late. */
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"
#include "call.h"

struct MagmaRockEffectOptions {
    s32 kind;
    s32 palette;
    s32 scale_x;
    s32 scale_y;
    u8 unknown_10[8];
    u16 id;
    u8 unknown_1a[2];
    const void *data;
    s32 unknown_20;
    void (*update)();
};

struct MagmaRockEventWork {
    u8 unknown_000[0x1ac];
    s32 field_1ac;
};

struct ObjectRuntime *Object_GetById(s32 id);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
void Motion_CamBounds(s32 a, s32 b, s32 c, s32 d);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
u32 Random16Far(void);
void Effect_AdvanceMotion();
void Func_02003aec(void);
void Func_02003bcc(s32 value);
void Func_02003a7c(void);
void Func_02003c54(s32 cue);
void Func_02003b44(s32 actor, s32 value);
void Func_02003ac4(s32 a, s32 b, s32 c);
void Func_02003acc(void);
void Func_02003af4(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ, u32 flags, struct MagmaRockEffectOptions *options);

void Func_020022ac(void)
{
    struct ObjectRuntime *leader = Object_GetById(gPartyState.current_owner);
    struct MagmaRockEffectOptions options;
    u32 i;
    s32 angle;
    s32 velocity[3];

    Func_02003aec();
    Func_02003bcc(0);
    Call4((void (*)())Motion_CamBounds, -1, -1, -1, 0);
    Func_02003a7c();
    WaitFrames(1);
    leader->y += 0x820000;
    leader->gravity = 0x8000;
    leader->bounce = 0;
    leader->motion_flags = 0;
    ((struct MagmaRockEventWork *)Ram_HeapSlots->event_work)->field_1ac = 0x100;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Func_02003c54(204);
    leader->motion_flags = 3;
    Battle_WaitMode0(24);
    options.palette = 7;
    options.update = Effect_AdvanceMotion;
    options.scale_x = 0xcccc;
    options.scale_y = 0xcccc;
    i = 0;
loop:
    angle = i << 12;
    velocity[0] = Math_Cosine(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sine(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16Far() * 3) << 14 >> 16) - 0x6000;
    velocity[2] += ((Random16Far() * 3) << 13 >> 16) - 0x3000;
    Func_0200015c(leader->x, leader->y, leader->z, velocity[0], velocity[1], velocity[2], 0x1090001, &options);
    i++;
    if (i <= 16)
        goto loop;
    Func_02003c54(188);
    Func_02003b44(gPartyState.current_owner, 0x101);
    Object_SetModeById(gPartyState.current_owner, 22);
    Call3((void (*)())Func_02003ac4, 0x50000, 0x50000, 0x10000);
    Call3((void (*)())Func_02003ac4, -1, -1, 0xe666);
    Func_02003acc();
    Func_02003b44(gPartyState.current_owner, 0x100);
    leader->gravity = 0x10000;
    leader->bounce = 0x4000;
    ((struct MagmaRockEventWork *)Ram_HeapSlots->event_work)->field_1ac = 0x204;
    Func_02003af4();
}

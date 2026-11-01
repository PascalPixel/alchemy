// Draft, differ 18 of 174, skeleton gap 1 (tla-en, 2026-11-01): the leader
// lands and 17 dust effects spread around it. Twin of
// VenusLighthouse_LandWithRadialDust in The Broken Seal: the same calls in the
// same order, the same 17 particles (i << 12) and the same effect options; this
// game adds the event work's 0x1ac writes and the two random velocity terms.
// Left: gcse's PRE and loop.c hoist the velocity array's address to before the
// loop where the native builds it after the cosine call in every pass
// (resource_69c_c_02001f50.c records the same gap), and i and the options
// address take r8 and r10 the wrong way round (global.c ranks i 9 refs over 110
// insns just above the address's 7 over 62).
// Tried 1 November, none better: the twin's for loop with inner-scope angle and
// velocity, do/while, ++i in the test, the leader assigned after its
// declaration (all 18, gap 1); an options pointer local (18 at best); a named
// scale constant (10, gap 4); a dead early return before the loop (18).
// Moving i = 0 up to just after options.palette lengthens i's life enough to
// fix the swap and gives differ 6, but sched2 then places mov r10 before the
// scale stores, gap 2, so it is not kept. The sibling reaches differ 4 only
// with a pin, which this draft does not take.
#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "call.h"

struct MagmaRockEffectOptions {
  s32 kind;
  s32 palette;
  s32 scaleX;
  s32 scaleY;
  u8 unk10[8];
  u16 id;
  u8 unk1a[2];
  const void *data;
  s32 unk20;
  void (*update)();
};

struct MagmaRockEventWork {
  u8 unk000[0x1ac];
  s32 field1ac;
};

struct Actor *Actor_GetById(s32 id);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
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
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   struct MagmaRockEffectOptions *options);

void Func_020022ac(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct MagmaRockEffectOptions options;
  u32 i;
  s32 angle;
  s32 velocity[3];

  Func_02003aec();
  Func_02003bcc(0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Func_02003a7c();
  wait(1);
  leader->y += 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct MagmaRockEventWork *)Ram_HeapSlots->eventWork)->field1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02003c54(204);
  leader->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.scaleX = 0xcccc;
  options.scaleY = 0xcccc;
  i = 0;
loop:
  angle = i << 12;
  velocity[0] = Math_Cosine(angle);
  velocity[1] = 0;
  velocity[2] = Math_Sine(angle);
  velocity[0] += velocity[0] >> 1;
  velocity[0] += ((Random16Far() * 3) << 14 >> 16) - 0x6000;
  velocity[2] += ((Random16Far() * 3) << 13 >> 16) - 0x3000;
  Func_0200015c(leader->x, leader->y, leader->z, velocity[0], velocity[1],
                velocity[2], 0x1090001, &options);
  i++;
  if (i <= 16)
    goto loop;
  Func_02003c54(188);
  Func_02003b44(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 22);
  Call3((void (*)())Func_02003ac4, 0x50000, 0x50000, 0x10000);
  Call3((void (*)())Func_02003ac4, -1, -1, 0xe666);
  Func_02003acc();
  Func_02003b44(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  ((struct MagmaRockEventWork *)Ram_HeapSlots->eventWork)->field1ac = 0x204;
  Func_02003af4();
}

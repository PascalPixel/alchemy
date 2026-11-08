#include "version.h"

// The Japanese form is the older revision and is still on its listing.
#if TLA_PATCH_1

#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "call.h"

struct LandingEffectOptions {
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

struct LandingEventWork {
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
void Func_02002e44(void);
void Func_02002edc(s32 value);
void Func_02002dec(void);
void Func_02002eec(s32 cue);
void Func_02002e9c(s32 actor, s32 value);
void Func_02002e24(s32 a, s32 b, s32 c);
void Func_02002e2c(void);
void Func_02002e4c(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   struct LandingEffectOptions *options);

void Func_02000418(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct LandingEffectOptions options;
  u32 i;
  s32 angle;
  s32 velocity[3];

  Func_02002e44();
  Func_02002edc(0);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Func_02002dec();
  wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct LandingEventWork *)Ram_HeapSlots->eventWork)->field1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02002eec(204);
  leader->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.scaleX = SPLIT_CONSTANT(0xa3d7);
  options.scaleY = SPLIT_CONSTANT(0xa3d7);
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
  Func_02002eec(188);
  Func_02002e9c(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 49);
  Call3((void (*)())Func_02002e24, 0x50000, 0x50000, 0x10000);
  Call3((void (*)())Func_02002e24, -1, -1, SPLIT_CONSTANT(0xe666));
  Func_02002e2c();
  Func_02002e9c(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  ((struct LandingEventWork *)Ram_HeapSlots->eventWork)->field1ac = 0x204;
  Func_02002e4c();
}

#endif

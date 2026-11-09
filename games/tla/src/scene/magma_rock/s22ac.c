#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"

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

void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 Math_Cosine(s32 angle);
u32 Random16Far(void);
void Effect_AdvanceMotion();
void Func_02003aec(void);
void Func_02003a7c(void);
void Func_02003af4(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   struct MagmaRockEffectOptions *options);

void Func_020022ac(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct MagmaRockEffectOptions options;
  u32 i;
  s32 angle;
  s32 velocity[3];

  Func_02003aec();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02003a7c();
  System_Wait(1);
  leader->y += 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct MagmaRockEventWork *)Ram_HeapSlots->eventWork)->field1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(204);
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
  velocity[2] = Math_Sin(angle);
  velocity[0] += velocity[0] >> 1;
  velocity[0] += ((Random16Far() * 3) << 14 >> 16) - 0x6000;
  velocity[2] += ((Random16Far() * 3) << 13 >> 16) - 0x3000;
  Func_0200015c(leader->x, leader->y, leader->z, velocity[0], velocity[1],
                velocity[2], 0x1090001, &options);
  i++;
  if (i <= 16)
    goto loop;
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 22);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  ((struct MagmaRockEventWork *)Ram_HeapSlots->eventWork)->field1ac = 0x204;
  Func_02003af4();
}

#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "frame.h"
#include "field_effect.h"
#include "event_inline.h"

void Func_02003048(void);
void Func_02003050(void);
void Func_02002fa8(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 Math_Cosine(s32 angle);
void Effect_AdvanceMotion(struct FieldEffect *effect);
void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

static inline void TundariaTower_SetWork(s32 a, s32 b, s32 c) {
  MapGrid_SetShake(a, b, c);
}

static inline void TundariaTower_CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

void Func_020011e0(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  s32 angle;
  u32 i;

  Func_02003048();
  Event_ResolvePendingActions(0);
  TundariaTower_CamBounds(-1, -1, -1, 0);
  Func_02002fa8();
  wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(204);
  leader->motionFlags = 3;
  Event_Wait(24);
  options.palette = 7;
  options.update = (void *)Effect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  i = 0;
  do {
    s32 velocity[3];

    angle = i << 12;
    velocity[0] = Math_Cosine(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sine(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16Far() * 3) << 14 >> 16) - 0x6000;
    velocity[2] += ((Random16Far() * 3) << 13 >> 16) - 0x3000;
    Func_020000b8(leader->x, leader->y, leader->z, velocity[0], velocity[1],
                  velocity[2], 0x01090001, &options);
    i++;
  } while (i <= 16);
  Sound_Play(188);
  Func_080d489c(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 22);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  Map_WaitForShakeToSettle();
  Func_080d489c(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Func_02003050();
}

// Draft, differ 16 of 172: the leader drops onto the floor below with sixteen
// sparks thrown around it in a ring (Func_020000b8), then the camera shakes
// and the leader lands. Left: the native keeps the spark loop's count in r10
// and the options in r8 (here the other way round) and builds the velocity's
// address inside the loop; a goto loop, the velocity declared in the loop and
// a six-minute permute do not reproduce it.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "event_work.h"
#include "frame.h"
#include "lib/call.h"
#include "field_effect.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02003000(s32 a, s32 b, s32 c);
void Func_02003008(void);
void Func_02003048(void);
void Func_02003050(void);
void Func_020030c8(s32 actor, s32 a);
void Func_02003148(s32 a);
void Func_020031a8(s32 sound);
void Func_02002fa8(void);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
u32 Random16Far(void);
void Effect_AdvanceMotion(struct FieldEffect *effect);
void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

static inline void TundariaTower_SetWork(s32 a, s32 b, s32 c) {
  Call3((void (*)())Func_02003000, a, b, c);
}

static inline void TundariaTower_CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

void Func_020011e0(void) {
  struct ObjectRuntime *leader = Object_GetById(gAdventure.currentOwner);
  struct EffectOptions options;
  s32 angle;
  u32 i;

  Func_02003048();
  Func_02003148(0);
  TundariaTower_CamBounds(-1, -1, -1, 0);
  Func_02002fa8();
  WaitFrames(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_020031a8(204);
  leader->motionFlags = 3;
  FieldEvent_WaitFrames(24);
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
  Func_020031a8(188);
  Func_020030c8(gAdventure.currentOwner, 0x101);
  Object_SetModeById(gAdventure.currentOwner, 22);
  Call3((void (*)())Func_02003000, 0x50000, 0x50000, 0x10000);
  Call3((void (*)())Func_02003000, -1, -1, 0xe666);
  Func_02003008();
  Func_020030c8(gAdventure.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  Func_02003050();
}

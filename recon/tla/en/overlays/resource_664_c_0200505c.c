// Draft, differ 81 of 172: inside an event, the leader drops from height 130
// after a screen transition (cue 204), lands in a ring of seventeen dust
// effects, plays cue 188 and mode 22 while the screen shakes twice, then
// gets its normal gravity back. The same listing as overlay 68f's
// Func_020011e0. Left: the ring loop's registers and order (the native keeps
// the count in r10 and the velocity on the stack).
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "system/field_effect.h"
#include "system/vector3.h"

struct ObjectRuntime *Object_GetById(u32 id);
void Func_02005e14(void);
void Func_02005e1c(void);
void Func_02005f2c(s32 skip);
void Func_02005f94(s32 cue);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02005d84(void);
void WaitFrames(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void FieldEvent_WaitFrames(s32 frames);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
u16 Random16Far(void);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_020002f4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_02005ec4(u32 id, s32 mode);
void Object_SetModeById(u32 id, s32 mode);
void Func_02005dcc(s32 a, s32 b, s32 c);
void Func_02005dd4(void);

void Func_0200505c(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct EffectOptions options;
  struct Vector3 velocity;
  u32 i;
  s32 angle;

  Func_02005e14();
  Func_02005f2c(0);
  Camera_MoveWithinMapBounds(-1, -1, -1, 0);
  Func_02005d84();
  WaitFrames(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02005f94(204);
  leader->motionFlags = 3;
  FieldEvent_WaitFrames(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = options.startScaleY = 0xcccc;
  for (i = 0; i <= 16; i++) {
    angle = i << 12;
    velocity.x = Math_Cosine(angle);
    velocity.y = 0;
    velocity.z = Math_Sine(angle);
    velocity.x += velocity.x >> 1;
    velocity.x += ((u32)(Random16Far() * 3) << 14 >> 16) - 0x6000;
    velocity.z += ((u32)(Random16Far() * 3) << 13 >> 16) - 0x3000;
    Func_020002f4(
        leader->x, leader->y, leader->z, velocity.x, velocity.y, velocity.z,
        EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | 1,
        &options);
  }
  Func_02005f94(188);
  Func_02005ec4(gPartyState.currentOwner, 257);
  Object_SetModeById(gPartyState.currentOwner, 22);
  Func_02005dcc(0x50000, 0x50000, 0x10000);
  Func_02005dcc(-1, -1, 0xe666);
  Func_02005dd4();
  Func_02005ec4(gPartyState.currentOwner, 256);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  Func_02005e1c();
}

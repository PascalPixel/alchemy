// Draft of overlay 6a5's Func_0200539c, the twin of overlay 6aa's Func_020042f8
// (resource_6aa_c_020042f8.c), whose header says what differs: differ 16 of 169
// (was 63).
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"
#include "field_effect.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetModeById(u32 id, s32 mode);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 use_setter);
void WaitTicks(s32 ticks);
void FieldEvent_WaitTicks(s32 ticks);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_02004bd4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
void Func_0200577c(void);
void Func_020057ac(s32 a, s32 b, s32 c);
void Func_020057b4(void);
void Func_02005814(void);
void Func_0200581c(void);
void Func_02005904(s32 id, s32 value);
void Func_02005984(s32 a0);
void Func_020059f4(s32 cue);

static __inline__ void Camera_SetBounds(s32 x, s32 y, s32 z, s32 use_setter) {
  Camera_MoveWithinMapBounds(x, y, z, use_setter);
}

static __inline__ void Func_020057ac_(s32 a, s32 b, s32 c) {
  Func_020057ac(a, b, c);
}

void Func_0200539c(void) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct EffectOptions options;
  struct Vector3 velocity;
  u32 i;

  Func_02005814();
  Func_02005984(0);
  Camera_SetBounds(-1, -1, -1, 0);
  Func_0200577c();
  WaitTicks(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_020059f4(204);
  leader->motionFlags = 3;
  FieldEvent_WaitTicks(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  for (i = 0; i <= 16; i++) {
    s32 angle = i << 12;

    velocity.x = Math_Cosine(angle);
    velocity.y = 0;
    velocity.z = Math_Sine(angle);
    velocity.x += velocity.x >> 1;
    velocity.x += ((u32)(Random16Far() * 3 << 14) >> 16) - 0x6000;
    velocity.z += ((u32)(Random16Far() * 3 << 13) >> 16) - 0x3000;
    Func_02004bd4(leader->x, leader->y, leader->z, velocity.x, velocity.y,
                  velocity.z, 0x1090001, &options);
  }
  Func_020059f4(188);
  Func_02005904(gPartyState.currentOwner, 0x101);
  Object_SetModeById(gPartyState.currentOwner, 49);
  Func_020057ac_(0x50000, 0x50000, 0x10000);
  Func_020057ac_(-1, -1, 0xe666);
  Func_020057b4();
  Func_02005904(gPartyState.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  FieldEvent_WaitTicks(10);
  Object_SetModeById(gPartyState.currentOwner, 1);
  Func_0200581c();
}

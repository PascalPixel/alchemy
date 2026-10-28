// Draft, differ 4 of 169 under -mthumb-slow-loads: overlay 6a2's copy of the function drafted in
// resource_698_c_02001674.c (overlay 698's Func_02001674, differ 10 there), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "adventure.h"
#include "field_effect.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02001be8(void);
void Func_02001c70(s32 value);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02001b90(void);
void WaitFrames(s32 frames);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001ca0(s32 sound);
void FieldEvent_WaitFrames(s32 frames);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
u32 Random16Far(void);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_02001018(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);
void Func_02001c30(s32 id, s32 value);
void Func_02001bc0(s32 a, s32 b, s32 c);
void Func_02001bc8(void);
void Func_02001bf0(void);

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  Func_02001bc0(a, b, c);
}

void Func_020017e0(void) {
  struct ObjectRuntime *actor = Object_GetById(gAdventure.currentOwner);
  /* FAKEMATCH(pin): i in r10, which leaves r8 to the options pointer; as a
   * plain local, 22 of 177 instructions differ in tla-en. */
  register u32 i asm("r10");
  struct EffectOptions options;

  Func_02001be8();
  Func_02001c70(0);
  CamBounds(-1, -1, -1, 0);
  Func_02001b90();
  WaitFrames(1);
  actor->y = 0x820000;
  actor->gravity = 0x8000;
  actor->bounce = 0;
  actor->motionFlags = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001ca0(204);
  actor->motionFlags = 3;
  FieldEvent_WaitFrames(24);
  options.palette = 7;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  i = 0;
  do {
    s32 angle;
    s32 velocity[3];

    angle = i << 12;
    velocity[0] = Math_Cosine(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sine(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += (Random16Far() * 0xc000 >> 16) - 0x6000;
    velocity[2] += (Random16Far() * 0x6000 >> 16) - 0x3000;
    Func_02001018(actor->x, actor->y, actor->z, velocity[0], velocity[1],
                  velocity[2], 0x1090001, &options);
    i++;
  } while (i <= 16);
  Func_02001ca0(188);
  Func_02001c30(gAdventure.currentOwner, 0x101);
  Object_SetModeById(gAdventure.currentOwner, 49);
  SetWorkTriple(0x50000, 0x50000, 0x10000);
  SetWorkTriple(-1, -1, 0xe666);
  Func_02001bc8();
  Func_02001c30(gAdventure.currentOwner, 0x100);
  actor->gravity = 0x10000;
  actor->bounce = 0x4000;
  FieldEvent_WaitFrames(10);
  Object_SetModeById(gAdventure.currentOwner, 1);
  Func_02001bf0();
}

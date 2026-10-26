// Draft, differ 10 of 165 (was 94): overlay 6a5's copy of overlay 698's
// Func_0200091c, as resource_6a2_c_02000a88.c, whose header says what is left.
#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
void Func_02005884(s32 id, s32 x, s32 z);

static __inline__ void Func_02001c10_(s32 id, s32 x, s32 z) {
  Func_02005884(id, x, z);
}

static __inline__ s32 GameFlag_Test_(s32 flag) { return GameFlag_Test(flag); }

void Func_02004644(void) {
  struct ObjectRuntime *left = Object_GetById(9);
  struct ObjectRuntime *right = Object_GetById(10);
  struct ObjectRuntime *rider = Object_GetById(23);
  struct FieldSprite *sprite = rider->animation;

  Func_02001c10_(24, 0x2000000, 0xf80000);
  Func_02001c10_(23, 0x2000000, 0xf80000);
  Func_02001c10_(9, 0x1d80000, 0x1000000);
  Func_02001c10_(10, 0x2280000, 0x1000000);
  Object_GetById(24)->scaleX = 0;
  rider->motionFlags = 0;
  rider->y = rider->terrainHeight;
  left->motionFlags = 0;
  left->y = left->terrainHeight;
  right->motionFlags = 0;
  right->y = right->terrainHeight;
  if (GameFlag_Test_(0xa0a)) {
    left->y += 0x66640;
    rider->y += 0x1eb80;
    rider->x += -0x28f40;
    sprite->rotation += 0x800;
    Animation_ApplyChildValues(Object_GetById(9), 4);
    Animation_ApplyChildValues(Object_GetById(11), 4);
  }
  if (GameFlag_Test_(0xa0b)) {
    right->y += 0x66640;
    rider->y += 0x1eb80;
    rider->x += 0x28f40;
    *(s16 *)&sprite->rotation += -0x800;
    Animation_ApplyChildValues(Object_GetById(10), 4);
    Animation_ApplyChildValues(Object_GetById(12), 4);
  }
  if (GameFlag_Test_(0xa0a) && GameFlag_Test_(0xa0b)) {
    left->y += 0x199900;
    right->y += 0x199900;
    rider->y += 0x1b8480;
    left->motionFlags = 4;
    right->motionFlags = 4;
    rider->motionFlags = 4;
  }
}

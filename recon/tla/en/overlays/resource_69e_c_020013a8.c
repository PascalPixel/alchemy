// Draft, differ 10 of 165: overlay 69e's copy of the function drafted in
// resource_698_c_0200091c.c (overlay 698's Func_0200091c, differ 10 there), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"
#include "call.h"

s32 Object_GetById();
void Func_020028f8();
s32 GameFlag_Test();
void Animation_ApplyChildValues();

void Func_020013a8(void) {
  struct ObjectRuntime *a = (struct ObjectRuntime *)Value1(Object_GetById, 9);
  struct ObjectRuntime *b = (struct ObjectRuntime *)Value1(Object_GetById, 10);
  struct ObjectRuntime *c = (struct ObjectRuntime *)Value1(Object_GetById, 23);
  struct FieldSprite *sprite = c->animation;

  Call3(Func_020028f8, 24, 0x2000000, 0xf80000);
  Call3(Func_020028f8, 23, 0x2000000, 0xf80000);
  Call3(Func_020028f8, 9, 0x1d80000, 0x1000000);
  Call3(Func_020028f8, 10, 0x2280000, 0x1000000);
  ((struct ObjectRuntime *)Value1(Object_GetById, 24))->scaleX = 0;
  c->motionFlags = 0;
  c->y = c->terrainHeight;
  a->motionFlags = 0;
  a->y = a->terrainHeight;
  b->motionFlags = 0;
  b->y = b->terrainHeight;
  if (Value1(GameFlag_Test, 0xa0a)) {
    a->y += 0x66640;
    c->y += 0x1eb80;
    c->x += -0x28f40;
    sprite->rotation += 0x800;
    Call2(Animation_ApplyChildValues, Value1(Object_GetById, 9), 4);
    Call2(Animation_ApplyChildValues, Value1(Object_GetById, 11), 4);
  }
  if (Value1(GameFlag_Test, 0xa0b)) {
    b->y += 0x66640;
    c->y += 0x1eb80;
    c->x += 0x28f40;
    sprite->rotation -= 0x800;
    Call2(Animation_ApplyChildValues, Value1(Object_GetById, 10), 4);
    Call2(Animation_ApplyChildValues, Value1(Object_GetById, 12), 4);
  }
  if (Value1(GameFlag_Test, 0xa0a) && Value1(GameFlag_Test, 0xa0b)) {
    a->y += 0x199900;
    b->y += 0x199900;
    c->y += 0x1b8480;
    a->motionFlags = 4;
    b->motionFlags = 4;
    c->motionFlags = 4;
  }
}

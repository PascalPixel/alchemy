// Draft of overlay 698's Func_0200091c, in 11 overlays of the 698 family:
// differ 10 of 165. Sets objects 9, 10, 23 and 24 on their marks and grounds
// them, then shifts and turns them for flags 2570 and 2571 (and raises them
// when both are set). The sprite of object 23 is read once at the start, as
// the native keeps it in r8; the call layer keeps each call's constants apart.
// Left: five store-to-load rows: after each store the native loads the next
// pool constant before the load through another object (ldr r2, =K; ldr r3,
// [r5, #12]), where this loads the field first. Tried: K + c->y, an empty asm
// before each, the constant through an asm temporary (10 to 14).
#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "call.h"

s32 Actor_GetById();
void Func_02001ab4();
s32 GameFlag_Test();
void Animation_ApplyChildValues();

void Func_0200091c(void) {
  struct Actor *a = (struct Actor *)Value1(Actor_GetById, 9);
  struct Actor *b = (struct Actor *)Value1(Actor_GetById, 10);
  struct Actor *c = (struct Actor *)Value1(Actor_GetById, 23);
  struct FieldSprite *sprite = c->animation;

  Call3(Func_02001ab4, 24, 0x2000000, 0xf80000);
  Call3(Func_02001ab4, 23, 0x2000000, 0xf80000);
  Call3(Func_02001ab4, 9, 0x1d80000, 0x1000000);
  Call3(Func_02001ab4, 10, 0x2280000, 0x1000000);
  ((struct Actor *)Value1(Actor_GetById, 24))->scaleX = 0;
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
    Call2(Animation_ApplyChildValues, Value1(Actor_GetById, 9), 4);
    Call2(Animation_ApplyChildValues, Value1(Actor_GetById, 11), 4);
  }
  if (Value1(GameFlag_Test, 0xa0b)) {
    b->y += 0x66640;
    c->y += 0x1eb80;
    c->x += 0x28f40;
    sprite->rotation -= 0x800;
    Call2(Animation_ApplyChildValues, Value1(Actor_GetById, 10), 4);
    Call2(Animation_ApplyChildValues, Value1(Actor_GetById, 12), 4);
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

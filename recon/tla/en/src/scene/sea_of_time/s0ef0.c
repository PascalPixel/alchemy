/// @file
/// Sends actor 8 to a point 8 pixels along z and, until Func_02001f9c says it is
/// done, raises a pair of effects at its two sides every fourth frame.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "system/field_effect.h"

extern u32 gFrameCount;
struct Actor *Actor_GetById(s32 id);
void Func_02001fac(void);
void Func_02001fb4(void);
void Func_02002044(s32 skip);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Func_02001f6c(struct Actor *actor, s32 x, s32 y, s32 z);
s32 Func_02001f9c(struct Actor *actor);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void wait(s32 frames);

void Func_02000ef0(void) {
  s32 sides[4];
  struct EffectOptions options;
  struct Actor *actor;
  s32 cosine, sine;
  s32 x, y, z;
  s32 velocity, reverseVelocity, alternate;

  actor = Actor_GetById(8);
  Func_02001fac();
  Func_02002044(0);
  Func_02001f6c(actor, actor->x, actor->y, actor->z + 0x80000);
  // A goto loop: as while (Func_02001f9c(actor) == 0), loop.c moves 0xb0000
  // out of the loop and 191 of 165 instructions differ.
  while (Func_02001f9c(actor) == 0) {
  if ((gFrameCount & 3) == 0) {
    options.priority = 2;
    options.palette = 7;
    options.startScaleX = 0x16666;
    options.startScaleY = 0x13333;
    cosine = Math_Cosine(actor->angle);
    sine = Math_Sine(actor->angle);
    sides[0] = cosine * 12;
    sides[1] = sine * 12;
    sides[2] = -(cosine * 12);
    sides[3] = -(sine * 12);
    sides[0] += actor->x;
    sides[1] += actor->z;
    sides[2] += actor->x;
    sides[3] += actor->z;
    velocity = 0x8000;
    if (sides[1] != actor->z) {
      sides[1] -= 0xc0000;
      sides[3] -= 0xc0000;
    }
    alternate = gFrameCount & 4;
    reverseVelocity = -velocity;
    x = sides[0];
    y = actor->y;
    z = sides[1];
    if (alternate) {
      Func_0200015c(x + 0x40000, y, z, velocity, 0, 0,
                    EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                        EFFECT_USE_START_SCALE,
                    &options);
      Func_0200015c(sides[2] - 0x40000, actor->y, sides[3], reverseVelocity, 0, 0,
                    EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                        EFFECT_USE_START_SCALE,
                    &options);
    } else {
      Func_0200015c(x - 0x40000, y, z, reverseVelocity, 0, 0,
                    EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                        EFFECT_USE_START_SCALE,
                    &options);
      Func_0200015c(sides[2] + 0x40000, actor->y, sides[3], velocity, 0, 0,
                    EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                        EFFECT_USE_START_SCALE,
                    &options);
    }
  }
  wait(1);
  }
  Func_02001fb4();
}

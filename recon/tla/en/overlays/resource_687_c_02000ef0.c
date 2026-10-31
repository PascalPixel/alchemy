// Near miss, differ 137 of 165 (tla-en, 2026-10-31): control flow, calls,
// fields, frame layout and constants are the native's; what remains is
// allocation. Global allocation ranks gFrameCount & 4 (16 refs over 22 insns)
// above the pos address (34 refs over 71), so pos takes r7 and &gFrameCount,
// then gFrameCount & 4, take r6: the native has them the other way round.
// Reload's choice of reload registers runs one step off from the frame block
// on: the gFrameCount & 3 mask goes through r2 rather than r1, and &options is
// built in r3, kept in r9 and copied to r5 for the four stores (one
// instruction more) where the native builds it in r2, stores through r2 and
// copies it to r9. In the offset block cosine * 12 takes r2 and sine * 12 r1,
// where the native has r3 and r2, so the actor->x load takes r3, not r1.
// Measured forms: the loop as a C while is 191 (loop.c hoists 0xb0000 out of
// it); passing -velocity at the uses is 155 (negs instead of the native's
// literal-pool loads); passing -0x8000 literally is 186 (GCSE propagates the
// constant into velocity's uses and its register disappears). Adding the
// offsets as actor->x + pos[0] and so on measures 132 and puts the mask and
// &options in the native's r1 and r2, but spills a value to a new stack slot
// (a 76-byte frame, 169 instructions), so this form is kept. Names are still
// placeholders.
#include "types.h"
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
  s32 pos[4];
  struct EffectOptions options;
  struct Actor *actor;
  s32 cosine, sine;
  s32 x0, y0, z0;
  s32 velocity, reverse;

  actor = Actor_GetById(8);
  Func_02001fac();
  Func_02002044(0);
  Func_02001f6c(actor, actor->x, actor->y, actor->z + 0x80000);
  // A goto loop: as while (Func_02001f9c(actor) == 0), loop.c moves 0xb0000
  // out of the loop and 191 of 165 instructions differ.
  goto check;
body:
  if ((gFrameCount & 3) == 0) {
    options.priority = 2;
    options.palette = 7;
    options.startScaleX = 0x16666;
    options.startScaleY = 0x13333;
    cosine = Math_Cosine(actor->angle);
    sine = Math_Sine(actor->angle);
    pos[0] = cosine * 12;
    pos[1] = sine * 12;
    pos[2] = -(cosine * 12);
    pos[3] = -(sine * 12);
    pos[0] += actor->x;
    pos[1] += actor->z;
    pos[2] += actor->x;
    pos[3] += actor->z;
    velocity = 0x8000;
    reverse = -velocity;
    if (pos[1] != actor->z) {
      pos[1] -= 0xc0000;
      pos[3] -= 0xc0000;
    }
    x0 = pos[0];
    y0 = actor->y;
    z0 = pos[1];
    if (gFrameCount & 4) {
      Func_0200015c(x0 + 0x40000, y0, z0, velocity, 0, 0,
                    EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                        EFFECT_USE_START_SCALE,
                    &options);
      Func_0200015c(pos[2] - 0x40000, actor->y, pos[3], reverse, 0, 0,
                    EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                        EFFECT_USE_START_SCALE,
                    &options);
    } else {
      Func_0200015c(x0 - 0x40000, y0, z0, reverse, 0, 0,
                    EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                        EFFECT_USE_START_SCALE,
                    &options);
      Func_0200015c(pos[2] + 0x40000, actor->y, pos[3], velocity, 0, 0,
                    EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                        EFFECT_USE_START_SCALE,
                    &options);
    }
  }
  wait(1);
check:
  if (Func_02001f9c(actor) == 0)
    goto body;
  Func_02001fb4();
}

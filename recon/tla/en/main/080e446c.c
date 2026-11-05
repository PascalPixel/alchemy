// Draft, differ 4 of 77, skeleton gap 2 (tla-en, 2026-11-01)
// What is left is one ordering before the first call: the listing issues
// ldr r3 (the routine), movs r2, mov r0, lsls r1; the draft issues lsls r1,
// movs r2, ldr r3, mov r0. sched2 ties there on LUID: the listing's order means
// 0x400 reaches r1 after r0 is set, as a constant equivalent reloaded at the
// argument copy, but this draft's pseudo takes r1 at its own set, ahead of the
// routine's address. Tried with no gain: width * height, height * width,
// width << 5, a sizeof factor, a u32 size, a size local in three positions, a
// void cast of the call. A local function pointer gives differ 2, leaving only
// lsls r1 against mov r0, and was not kept: it is a forced temporary.
// Moving width and height past the call, or swapping them, goes backwards.
// The form below is what closed 71 to 4, each part proved in the loop dump:
// shift is set twice, so position - wave is no giv and stays in the loop; base
// is a user variable set after the inner guard, so the second loop run leaves
// the multiply in the inner preheader; rowOffset keeps the constant out of the
// first run's giv, so the row step is reduced in the second run and its
// constant stays in the loop body. Loop threshold is 28 less 2 per move.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "iwram_call.h"

struct EffectSprite {
  u8 unk00[4];
  u16 attr0_0 : 13;
  u16 color : 1;
  u16 attr0_14 : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 x;
  s32 y;
  s32 scaleX;
  s32 scaleY;
  u8 unk1c[2];
  u16 depth;
  u8 unk20[8];
};

struct EffectScene {
  u16 mode;
  u8 unk02[14];
  struct Actor *mainObject;
};

struct EffectParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

void Func_080eaf98(struct EffectSprite *sprite, s32 width, s32 height,
                   u32 shape, s32 tiles);
s32 BattleEffect_GetObjectDepth(struct Actor *object);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
void Func_080eb01c(struct EffectSprite *sprite);
void Camera_WorldToScreen(s32 *position);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
void BattleEffect_IntegrateVector2(struct EffectParticle *particle, s32 frames,
                               s32 flags);
u32 Random16(void);
s32 Trig_Sin(s32 angle);

void Func_080e446c(u8 *source, u8 *destination, s32 phase) {
  s32 width = 32;
  s32 height = 32;
  s32 column;
  s32 row;

  Iwram_ClearWords(destination, 0x400);
  for (column = 0; column < width; column++) {
    s32 wave = (Trig_Sin(column << 13) << 3) + 0x80000;

    for (row = 0; row < height; row++) {
      s32 base = phase * 0x14ccc;
      s32 rowOffset = row * 0x40000 - 0x7c0000;
      s32 shift = base + rowOffset - wave;

      shift >>= 16;
      if (shift < 0) {
        destination[row * width + column] = source[row * width + column];
      } else if (shift + row < height) {
        destination[((shift + row) << 5) + column] =
            source[row * width + column];
      }
    }
  }
}

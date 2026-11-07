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

static inline s32 WrapClearWords(void *destination, s32 size) {
  /* FAKEMATCH(wrapper): the clear through this inline, so its arguments are set
   * before the entry's address is loaded; as a direct call, 4 of 77
   * instructions differ in tla-en. */
  return Iwram_ClearWords(destination, size);
}
void Func_080e446c(u8 *source, u8 *destination, s32 phase) {
  s32 width = 32;
  s32 height = 32;
  s32 column;
  s32 row;

  WrapClearWords(destination, 0x400);
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

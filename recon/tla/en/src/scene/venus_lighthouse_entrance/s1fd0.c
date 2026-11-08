#include "types.h"
#include "ram_buffer.h"
#include "actor.h"

struct GlowState {
  s32 x; ///< 16.16 pixels
  s32 y; ///< 16.16 pixels
  s32 z; ///< 16.16 pixels
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 frame;
};

struct GlowSprite {
  u8 unk00[4];
  u16 attr0 : 13;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 attr1;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[20];
  u16 unk1e;
  u8 unk20[8];
};

struct GlowWork {
  u8 unk000[0x400];
  struct GlowSprite sprites[16];
  struct GlowState states[16];
  struct Actor *object;
  struct Actor *effect;
  s32 x;
  s32 y;
  s32 z;
  s16 stage;
  s16 timer;
  u16 tileTop;
  u16 tile;
  s16 height;
  s8 follow;
};
#include "call.h"
#include "iwram_call.h"

s32 Math_Sine(s32 angle);
void Import_SoundPlay(s32 cue);
void Func_02002894(s32 a, s32 b, s32 c);
void Func_02002a14(struct GlowSprite *sprite, struct GlowState *state);

void Func_02001fd0(void) {
  struct GlowWork *work = Ram_HeapSlots->menuRuntime;
  struct Actor *object = work->object;
  struct Actor *effect = work->effect;
  s32 i, y;
  s32 scale;
  s32 frameStep;
  s32 mask;
  struct GlowState *state;
  struct GlowSprite *sprite;

  switch (work->stage) {
  case 0:
    if (work->timer == 0) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 1:
    work->height += 4;
    if (work->height > 31) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 2:
    if (work->timer == 90) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 3:
    if (work->timer == 0) {
      object->motionFlags = 3;
      object->velocityY = 0x180000;
      work->follow = 0;
      Import_SoundPlay(134);
    }
    work->height += 8;
    if (work->height > 79) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 4: {
    s16 *timer = &work->timer;

    if (work->timer == 10) {
      object->z = 0;
      object->x = 0;
    }
    if (*timer == 30) {
      work->stage++;
      work->timer = -1;
    }
    break;
  }
  case 5:
    work->height -= 8;
    if (work->height <= 0) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 6: {
    s16 *timer;

    scale = effect->scaleX;
    if (scale > 0)
      scale -= 0x51e;
    else
      scale = 0;
    effect->scaleX = scale;
    effect->scaleY = scale;
    /* FAKEMATCH(barrier): an empty asm after the scale's stores, which the
     * timer's address follows; as no asm, 2 of 449 instructions differ in
     * tla-en. */
    asm("");
    timer = &work->timer;
    if (work->timer == 10)
      Import_SoundPlay(310);
    if (*timer == 90) {
      work->stage++;
      work->timer = -1;
    }
    break;
  }
  case 7: {
    s16 *timer = &work->timer;

    if (work->timer == 0) {
      Import_SoundPlay(144);
      Call3((void (*)())Func_02002894, 0x30000, 0x30000, 0x10000);
    }
    if (*timer == 30) {
      Call3((void (*)())Func_02002894, -1, -1, SPLIT_CONSTANT(0xe600, 102));
      if (work->timer == 30) {
        work->stage = 999;
        work->timer = -1;
      }
    }
    break;
  }
  }
  object->angle += 0x1000;
  if (work->height > 0) {
    for (i = 0, y = 0; i < 16 && y < work->height - 8; i++, y += 16) {
      state = &work->states[i];
      state->x = work->x;
      state->y = work->y + (y << 16) + 0x80000;
      state->z = work->z;
      sprite = &work->sprites[i];
      state->frame++;
      frameStep = state->frame >> 2;
      mask = 3;
      sprite->tile = work->tileTop + (frameStep & mask) * 8;
      Func_02002a14(sprite, state);
    }
    if (i <= 15) {
      state = &work->states[i];
      state->x = work->x;
      state->y = work->y + (work->height << 16);
      state->z = work->z;
      state->frame++;
      state->y += Value2((s32 (*)())Iwram_MulQ16Entry, 0xfffe0000,
                         Math_Sine(state->frame << 12)) +
                  0xfffe0000;
      sprite = &work->sprites[i];
      sprite->tile = work->tile + ((state->frame >> 2) & 3) * 8;
      Func_02002a14(sprite, state);
      if (work->follow != 0)
        object->y = state->y;
    }
  }
  work->timer++;
}

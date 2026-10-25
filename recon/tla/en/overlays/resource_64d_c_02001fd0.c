// Draft, differ 415 of 449: the Venus Lighthouse entrance glow's scheduled
// update: steps through its eight stages (rise, wait, lift the object, hold,
// fall, fade the effect, shake) by timer, spins the object and draws the
// column of glow sprites, the top one bobbing on a sine. Left: the native
// computes each case's timer address once and shares the stage tails through
// it, and saves only r8 and r10; this draft's allocation differs throughout.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

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
  struct ObjectRuntime *object;
  struct ObjectRuntime *effect;
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
#include "iwram_call.h"

s32 Math_Sine(s32 angle);
void Engine_AudioPlayCue(s32 cue);
void Func_02002894(s32 a, s32 b, s32 c);
void Func_02002a14(struct GlowSprite *sprite, struct GlowState *state);

void Func_02001fd0(void) {
  struct GlowWork *work = Ram_HeapSlots->menuRuntime;
  struct ObjectRuntime *object = work->object;
  struct ObjectRuntime *effect = work->effect;
  s32 i, y;

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
      Engine_AudioPlayCue(134);
    }
    work->height += 8;
    if (work->height > 79) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 4:
    if (work->timer == 10) {
      object->z = 0;
      object->x = 0;
    }
    if (work->timer == 30) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 5:
    work->height -= 8;
    if (work->height <= 0) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 6:
    if (effect->scaleX > 0)
      effect->scaleX = effect->scaleX - 0x51e;
    else
      effect->scaleX = 0;
    effect->scaleY = effect->scaleX;
    if (work->timer == 10)
      Engine_AudioPlayCue(310);
    if (work->timer == 90) {
      work->stage++;
      work->timer = -1;
    }
    break;
  case 7:
    if (work->timer == 0) {
      Engine_AudioPlayCue(144);
      Func_02002894(0x30000, 0x30000, 0x10000);
    }
    if (work->timer == 30) {
      Func_02002894(-1, -1, 0xe666);
      if (work->timer == 30) {
        work->stage = 999;
        work->timer = -1;
      }
    }
    break;
  }
  object->angle += 0x1000;
  if (work->height > 0) {
    i = 0;
    for (y = 0; y < work->height - 8; y += 16) {
      struct GlowState *state = &work->states[i];
      struct GlowSprite *sprite = &work->sprites[i];

      state->x = work->x;
      state->y = work->y + (y << 16) + 0x80000;
      state->z = work->z;
      state->frame++;
      sprite->tile = work->tileTop + ((state->frame >> 2) & 3) * 8;
      Func_02002a14(sprite, state);
      i++;
      if (i > 15)
        goto done;
    }
    if (i <= 15) {
      struct GlowState *state = &work->states[i];
      struct GlowSprite *sprite;

      state->x = work->x;
      state->y = work->y + (work->height << 16);
      state->z = work->z;
      state->frame++;
      state->y +=
          Iwram_MulQ16(0xfffe0000, Math_Sine(state->frame << 12)) + 0xfffe0000;
      sprite = &work->sprites[i];
      sprite->tile = work->tile + ((state->frame >> 2) & 3) * 8;
      Func_02002a14(sprite, state);
      if (work->follow != 0)
        object->y = state->y;
    }
  }
done:
  work->timer++;
}

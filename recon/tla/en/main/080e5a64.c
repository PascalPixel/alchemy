// Differ 8 of 302 (tla-en, 2026-11-08, -mtune=gba): the phase read through a pointer set before
// the offset (12 before), and an empty once-only block in place of the empty asm after the
// palette store (no block, 10). Left: red finished after green in the palette store, where
// the native finishes it first. Tried 8 November: red finished in its own local, the loop
// setup reordered, the colour built in steps (8 to 20).
// Near miss, differ 16 of 302 (tla-en, 2026-11-05), with untagged empty asms, one-pass
// blocks and locals found by search; each needs its S2 tag on adoption, measured then.
// Near miss, differ 28 of 302 (54 before; 2026-10-26). The palette loop is a
// goto loop, as the native rebuilds the palette address from i each pass and
// loads it from the pool (a for loop strength-reduces it, 54), with 0x90000
// and 0x7c00 held in registers across the loop as the native holds them; the
// 0x90000 is set in two steps so that it is not rematerialized at each use
// (assigned once, 62). Left: instruction order in the palette store and in
// the switch's stores.
// A task for the heap slot 0x5c work. In state 1 it
// cycles object palette 14's 32 colours, each a blue with red and green
// waving on the same sine; while the sparkles are on, each of the 12
// particles younger than 16 frames shows its sprite and drifts. State 0
// lifts the work's object 3.5 a frame and, on frame 8, sets the blend work's
// two scales to 0x2000, starts the screen fade (cue 567) and turns the
// sparkles on; state 1 grows the scales by 0x1000 a frame up to 0xcccc;
// state 2 sets the blend mode 3 for 32 frames; state 3 starts the second
// fade and waits 10 frames; state 4 drops the object 1.5 a frame and parks
// at 999 after 13 frames.
#include "types.h"
#include "fixed_math.h"
#include "actor.h"
#include "ram_buffer.h"

struct ShineParticle {
  s32 x;
  s32 y;
  u8 unk08[16];
  s32 timer;
};

struct ShineSprite {
  u8 unk00[12];
  s32 x;
  s32 y;
  u8 unk14[20];
};

struct ShineWork {
  u8 unk0000[0x1000];
  struct ShineParticle particles[12];
  u8 unk1150[0x230];
  struct ShineSprite sprites[12];
  u8 unk1560[0x348];
  struct Actor *object;
  s16 state;
  s16 timer;
  s8 sparkles;
  u8 fading;
};

struct BlendWork {
  u8 unk00[160];
  u32 phase;
  u8 unka4[12];
  s32 scaleX;
  s32 scaleY;
  u8 unkb8[9];
  u8 mode;
};

void Func_080eb01c(struct ShineSprite *sprite);
void BattleEffect_IntegrateVector2(struct ShineParticle *particle, s32 frames,
                               s32 flags);
void Map_SetScreenShakeFar(s32 a, s32 b, s32 c);

void Func_080e5a64(void) {
  struct ShineWork *work = (struct ShineWork *)Ram_HeapSlots->battleFxWork[0];
  struct BlendWork *blend = Ram_HeapSlots->unkec[1];
  struct ShineParticle *particle;
  struct ShineSprite *sprite;
  s32 red, green;
  s32 i;
  s32 base, blue;
  s32 color;
  u32 *phase;

  work->fading = 0;
  if (work->state == 1) {
    i = 0;
    // The phase read through a pointer kept across the loop.
    phase = &blend->phase;
    /* FAKEMATCH(temporary): the phase offset built in two steps, so it stays
     * in a register across the loop; as base = 0x90000, 52 of 302
     * instructions differ in tla-en. */
    base = 9;
    base <<= 16;
    blue = 0x7c00;
  palette:
    red = Trig_Sin(((((*phase >> 1) + i) << 16) + base) >> 5) * 12;
    green = Trig_Sin(((((*phase >> 1) + i) << 16) + base) >> 5) * 12;
    color = (((green >> 16) + 16) << 5) | blue | ((red >> 16) + 16);
    *(u16 *)(0x050003c0 + i * 2) = color;
    // An empty once-only block after the palette store.
    do {
    } while (0);
    if (++i < 32)
      goto palette;
  }
  if (work->sparkles) {
    particle = work->particles;
    sprite = work->sprites;
    for (i = 0; i < 12; i++, sprite++, particle++) {
      if ((u32)particle->timer < 16) {
        sprite->x = particle->x;
        sprite->y = particle->y;
        Func_080eb01c(sprite);
        BattleEffect_IntegrateVector2(particle, 64, 0);
      }
      particle->timer++;
    }
  }
  switch (work->state) {
  case 0:
    work->object->y -= 0x38000;
    if (work->timer == 8) {
      blend->scaleX = 0x2000;
      blend->scaleY = 0x2000;
      Map_SetScreenShakeFar(0x14ccc, 0x14ccc, 0x10000);
      Sound_PlayFar(567);
      work->sparkles = 1;
      work->fading = 1;
    }
    if (work->timer == 8) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 1:
    blend->scaleX += 0x1000;
    blend->scaleY = blend->scaleX;
    if (blend->scaleX > 0xcccb) {
      blend->scaleX = 0xcccc;
      blend->scaleY = 0xcccc;
      work->state++;
      work->timer = -1;
    }
    break;
  case 2:
    blend->mode = 3;
    if (work->timer == 32) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 3:
    if (work->timer == 0)
      Map_SetScreenShakeFar(1, 1, 1);
    if (work->timer == 10) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 4:
    work->object->y += 0x18000;
    if (work->timer == 13)
      work->state = 999;
    break;
  }
  work->timer++;
}

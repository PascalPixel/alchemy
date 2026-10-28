// Draft of BattleEffect_RunFrost, effect kind 6.

// Twin: The Broken Seal's BattleEffect_RunFrost (effect.c), call for
// call. The Lost Age reads the effect from its argument, takes its heap
// blocks as Ram_HeapSlots fields, orders the callback 0xc80, ends 50
// frames after the burst (The Broken Seal: 75) and begins the results
// with phase 133 (132). ResourceId_IceChipSheet and ResourceId_SparkleDots
// are rows 0x17c and 0x17e of directory.s (assets BattleEffect_IceChipSheet and
// BattleEffect_SparkleDots), not yet named there.

// Draft: differ 34 of 753 (draft 753), tla-en, 2026-10-26 (78 under
// -mthumb-slow-loads before the tagged dead store below).

// What moved it: the native shares one call between the two pillar draws
// (the MAX one jumps into the other's mov lr, r4; bl). jump2 cross-jumps
// them only when the second draw's block does not end in its call: flow
// puts a no-op use after a call that ends a block before a label, and that
// use stops the cross-jump. A dead store after the second draw, which
// life analysis deletes after the blocks are made, leaves the call last
// and the two calls shared: 78 to 34, the instruction count exact.

// Left: the prologue's argument spill before the heap base and single
// rows where the native issues a store before a load (the frame loop's
// setup, the draws' stacked arguments): the known store-then-load
// scheduling limit.

// Tried (2026-10-15):
// - greedy declaration-order sweep, two passes: no gain.
// - a separate counter for the frame loop: 288.
// Tried (2026-10-26): the timing and spot tables as [][3] and [][2]
// arrays (no change); the second draw through draw[0] (97); the MAX draw
// followed by continue (no change), or wrapped in a one-pass block (no
// change: its labels are gone before flow makes the blocks).

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "task.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "graphics.h"
#include "io_reg.h"
#include "mem.h"
#include "actor.h"
#include "ram_buffer.h"
#include "system.h"

// The canvas layer and its helpers; the comments give The Broken Seal's
// names where the twin is known.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_FetchRectangleBlitters(
    s32 side,
    DrawRectangle *pair);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // BattleEffect_TintBattler
void Func_081180e8(s32 phase);       // BattlePlayback_StartFar

// A row of Data_08198348, one for each variant.
enum {
  RISE_MOTES,   // the rising motes
  RISE_COLUMNS, // the pillars
  RISE_FRAME    // the frame they burst
};

#define CHIPS ((struct EffectStep *)Ram_MapCellBuffer)
#define HI(v) (((s16 *)&(v))[1]) // the pixel half of a 16.16 value
#define TIMING(field) Data_08198348[effect->variant * 3 + (field)]

extern char ResourceId_IceChipSheet;
extern char ResourceId_SparkleDots;
extern u8 Data_08198348[];  // gFrost_Timings
extern u8 Data_08198322[];  // gFrost_ColumnSpots
extern u16 Data_08198362[]; // gFrost_MoteCells
extern u8 Data_08198351[];  // gFrost_MoteWidths
extern u8 Data_08198359[];  // gFrost_MoteHeights
extern u16 Data_08198372[]; // gFrost_ColumnCells
extern u8 Data_08198378[];  // gFrost_ColumnHeights
extern u16 Data_0819838c[]; // gFrost_ChipCells
extern u8 Data_0819837b[];  // gFrost_ChipWidths
extern u8 Data_08198383[];  // gFrost_ChipHeights
extern u16 Data_0819744c[]; // gBattleEffect_PuffCells
extern u8 Data_0819745e[];  // gBattleEffect_PuffSizes

// Motes swirl up around the targets while ice pillars rise one by one;
// then everything bursts.

// Setup: each mote gets an angle, a radius of 56-119 and a height of -64 to
// -33 pixels. Frame loop: until the burst the motes close in and climb five
// pixels a frame while they turn, and from frame 36 a pillar rises every
// three frames. At the burst the motes spread and fall, 32 chips leap up
// from below, and smoke rises half a pixel a frame through the height the
// motes and chips cover. The variant chooses the number of motes and
// pillars and the frame of the burst.
void BattleEffect_RunFrost(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  u8 *dots;
  DrawRectangle *draw;
  s32 count;
  s32 ymin;
  s32 ymax;
  s32 xyz[3];
  struct EffectPosition screen;
  s32 point[3];
  DrawRectangle pair[2];
  struct EffectStep *mote;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  dots = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_IceChipSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, dots, 0, 0);
  BattleEffect_FetchRectangleBlitters(0, draw = pair);

  // A mote: x holds its angle, z its radius, y its height.
  for (i = 0; i != 64; i++) {
    struct EffectStep *seed = &work->particles[i];

    seed->x = Random16() & 0xffff;
    seed->z = (Random16() & 63) + 56;
    seed->y = ((Random16() & 31) - 64) << 16;
  }

  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);
  if (effect->side == 1)
    REG_BG2X = -0x7000;

  for (frame = 0; frame != TIMING(RISE_FRAME) + 50; frame++) {
    ymin = 120 << 16;
    ymax = 0;
    if (frame == TIMING(RISE_FRAME) + 11)
      Func_081180e8(133);
    point[0] = 0;
    point[1] = 0;
    point[2] = 512 << 16;
    Render_ResetTransformState();
    SceneTransform_ApplyPosition(point);
    if (frame >= 36 && frame < 64 && (frame & 3) == 0)
      Sound_PlayFar(115);
    if (frame == 85)
      Sound_PlayFar(136);
    for (i = 0; i != effect->count; i++) {
      if (frame == i * 4 + 40)
        Func_0814cd48(effect->actors[i], 9, 5, -1, 0);
    }

    // The motes: before the burst they close in and climb to 48 pixels as
    // they turn; after it they spread and fall, and their heights bound the
    // smoke.
    if (frame < TIMING(RISE_FRAME))
      count = TIMING(RISE_MOTES);
    else
      count = 16;
    if (frame < TIMING(RISE_FRAME) + 35) {
      for (i = 0; i != count; i++) {
        if (frame > i) {
          s32 cel = i % 8;

          mote = &work->particles[i];
          if (mote->y < (48 - i / 2) << 16 && mote->y > -(48 << 16)) {
            struct EffectPosition *pos;
            u32 width;
            u32 height;

            xyz[0] = mote->z * Trig_Sin(mote->x);
            xyz[1] = mote->y;
            xyz[2] = mote->z * Trig_Cos(mote->x);
            EffectPosition_ProjectPoint(xyz, pos = &screen);
            pos->x = (pos->x >> 17) + 64;
            pos->y = HI(pos->y) + 60;
            draw[1](canvas, work->sheet + Data_08198362[cel],
                    pos->x - ((width = Data_08198351[cel]) >> 1),
                    pos->y - ((height = Data_08198359[cel]) >> 1), width,
                    height);
          }
          if (frame < TIMING(RISE_FRAME)) {
            if (frame > i + 16) {
              if (mote->z > 4)
                mote->z -= 2;
              if (mote->y < 48 << 16)
                mote->y += 5 << 16;
              mote->x += 0x200;
            }
          } else {
            mote->z += 8;
            mote->y -= (i % 5 + 2) << 16;
            if (ymin > mote->y)
              ymin = mote->y;
            if (ymax < mote->y)
              ymax = mote->y;
          }
        }
      }
    }
    ymin += 64 << 16;
    ymax += 64 << 16;

    // The pillars, 32 pixels wide: one more every three frames from frame
    // 36, drawn MAX for the last seven frames before the burst.
    if (frame < TIMING(RISE_FRAME)) {
      for (i = 0; i != TIMING(RISE_COLUMNS); i++) {
        if (i < (frame - 36) / 3) {
          s32 cel = i % 3;
          u32 height;

          if (frame >= TIMING(RISE_FRAME) - 7) {
            draw[1](canvas, work->sheet + Data_08198372[cel],
                    Data_08198322[i * 2],
                    Data_08198322[i * 2 + 1] - (height = Data_08198378[cel]),
                    32, height);
          } else {
            pair[0](canvas, work->sheet + Data_08198372[cel],
                    Data_08198322[i * 2],
                    Data_08198322[i * 2 + 1] - (height = Data_08198378[cel]),
                    32, height);
            /* FAKEMATCH(dead): a store flow deletes once the blocks are made,
             * so this block ends in its call with no no-op after it and jump2
             * shares the call with the MAX draw, as the native does; as plain
             * C, 78 of 753 instructions differ in tla-en. */
            height = 0;
          }
        }
      }
    }

    // The chips: at the burst 32 leap up from below (24 are drawn).
    if (frame == TIMING(RISE_FRAME)) {
      for (i = 0; i != 32; i++) {
        struct EffectStep *chip = &CHIPS[i];

        chip->x = (Random16() & 127) << 16;
        chip->y = ((Random16() & 15) + 80) << 16;
        chip->z = ((Random16() & 63) - 32) << 12;
        chip->velocityY = ((-Random16() & 15) - 16) << 13;
        chip->variant = (Random16() & 15) + 16;
      }
    }
    if (frame >= TIMING(RISE_FRAME)) {
      for (i = 0; i != 24; i++) {
        struct EffectStep *chip = &CHIPS[i];

        if (chip->variant >= 0) {
          s32 cel = i % 8;

          draw[1](canvas, work->sheet + Data_0819838c[cel], HI(chip->x),
                  HI(chip->y), Data_0819837b[cel], Data_08198383[cel]);
          chip->x += chip->velocityX;
          chip->y += chip->velocityY;
          chip->variant--;
        }
        if (ymin > chip->y)
          ymin = chip->y;
        if (ymax < chip->y)
          ymax = chip->y;
      }
    }
    ymin >>= 16;
    ymax >>= 16;
    if (ymax <= ymin)
      ymax = ymin + 1;

    // The smoke: from the burst it fills the height of the motes and chips
    // and rises half a pixel a frame.
    if (frame == TIMING(RISE_FRAME)) {
      for (i = 0; i != 32; i++) {
        struct EffectStep *puff = &work->particles[i];

        puff->velocityX = (Random16() & 127) << 16;
        if (ymax == ymin)
          puff->velocityY = ymin << 16;
        else
          puff->velocityY = (Random16() % (ymax - ymin) + ymin) << 16;
        puff->variant = (Random16() & 15) + 20;
      }
    }
    if (frame >= TIMING(RISE_FRAME)) {
      s32 rise = (frame - TIMING(RISE_FRAME)) / 2;

      for (i = 0; i != 32; i++) {
        struct EffectStep *puff = &work->particles[i];

        if (puff->variant >= 0 && puff->variant <= 17) {
          s32 cel = (17 - puff->variant) / 2;
          u32 size;

          draw[1](canvas, dots + Data_0819744c[cel],
                  HI(puff->velocityX) - ((size = Data_0819745e[cel]) >> 1),
                  HI(puff->velocityY) - (size >> 1) - rise, size, size);
        }
        puff->variant--;
        if ((puff->variant == -1 || puff->variant == 17) &&
            frame < TIMING(RISE_FRAME) + 35) {
          puff->variant = 17;
          puff->velocityX = (Random16() & 127) << 16;
          puff->velocityY = (Random16() % (ymax - ymin) + ymin) << 16;
        }
      }
    }
    work->transferPending = 1;
    WaitFrames(1);
  }

  HeapSlot_Release(188);
  HeapSlot_Release(104);
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

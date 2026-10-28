// Draft of Func_081548d0, the body of a two-kind family: vines (mode 0) or
// green pillars (mode 1) shoot up out of the ground, left and right in
// turn four frames apart, each throwing up dust at its root. The number of
// columns grows with the strength. Its wrappers BattleEffect_RunGrowth (mode 0) and
// BattleEffect_RunPunji (mode 1) are C in fxmoded.c.

// Twin: The Broken Seal's BattleEffect_RunGrowingColumns (effect.c), with the same
// tables, frame counts and fade.
// Listing 514 instructions, draft 515, differ 237.

// Changes from the twin: the effect argument is read directly (the Lost
// Age never stores work->effect), blitters load through BattleEffect_LoadBlitter with
// the Lost Age's mode numbers, the callback order is 0xc80 and the cue is
// 133 where the twin has 132. The puff scan walks a plain pointer: the
// twin's tagged reuse of length as the pointer is not the Lost Age's form.

// What remains:
// - the column draws (22 rows each): the native keeps i % 3 in r0 from
// __modsi3 and takes the width into it, with side = i & 1 in r4; here
// shape is copied to r4 and side takes r0.
// - the reaction loop: n and start + 4 trade r5 and r6.
// - the puff draws (about 30 rows): the native builds the cel offset
// 0x1e59 into r5 before the mode test and keeps the cel index in r7;
// here the order and registers differ.
// - one instruction more: the column loop's step reloads the count's
// table once more than the native.
// - order rows: the prologue's argument spills before the heap base,
// pool loads before register addresses.

// Tried (differ):
// a first port written freely from the twin                    436
// the twin's body as is, with the Lost Age's names             289
// declaration order, greedy scan                               283
// fadeStart, fadeEnd, cueFrame declared after draw (the native
// frame slots 20, 16, 12)                                      275
// the frame marks written inline in the tests                  289
// the puff scan through a plain pointer (kept)                 237
// without the pair pointer device (rectangle[side])            255
// the mirrored puff through draw[1]                            237
// shape as s32, or declared apart from its set                 no change
// declaration order, greedy scan of the kept form              no gain

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "ram_buffer.h"

// The dust puffs at the map cell buffer: 1024 cleared, the first 512 used.
#define PUFFS ((struct EffectStep *)Ram_MapCellBuffer)

// Directory rows 0x144 BattleEffect_GreenPillarSheet and 0x145 BattleEffect_VineSheet.
extern char ResourceId_GreenPillarSheet;
extern char ResourceId_VineSheet;

// The Broken Seal's gGrowth_* tables, in effect_table.s.
extern u8 Data_081983ea[];  // _ColumnCounts: columns by strength
extern u8 Data_081983ed[];  // _ColumnX: each column's root
extern u8 Data_081983d7[];  // _HeightLimits: by i % 3
extern u16 Data_081983da[]; // _VineCells
extern u8 Data_081983d4[];  // _VineWidths
extern s8 Data_081983fd[];  // _PillarHeights: by i & 7
extern u16 Data_081983e4[]; // _PillarCells
extern u8 Data_081983e0[];  // _PillarWidths
extern u16 Data_08198414[]; // _PuffCells
extern s8 Data_08198405[];  // _PuffWidths
extern u8 Data_0819840c[];  // _PuffHeights

void BattleEffect_StartCanvasLayer(s32 control);
void BattleEffect_EndCanvasLayer(void);
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeFlags);
s32 BattleEffect_ProcessPendingGraphicsTransfer(void);
s32 Task_Add(s32 (*callback)(void), s32 order);
s32 Task_Remove(s32 (*callback)(void));
void HeapSlot_Release(s32 slot);
void Sound_PlayFar(s32 cue);
void Func_081180e8(s32 phase);
u32 Random16(void);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames);
void Func_08158ce0(s32 x, s32 y);
void ObjectGroup_CountDownMemberTimers(void);
void WaitFrames(s32 frames);

// A growing column, laid over struct EffectStep in work->particles.
struct DualColumn {
  s32 x; // root on the canvas
  s32 y;
  u8 unk08[8];
  s32 height; // full length
  u8 unk14[4];
  s32 start; // the frame it starts to grow
};

#define COLUMN_COUNT 16
#define COLUMN_INTERVAL 4
#define PUFF_COUNT 512
#define PUFF_LIFE 14      // seven cels, two frames each
#define PHASE_RESULTS 133 // Func_081180e8: the results phase begins
#define CUE_COLUMN 133

// BattleEffect_LoadBlitter(slot, mode << 4 | flags) loads a blitter into a heap slot.
#define BLITTER_TRANSPARENT (2 << 4)

void Func_081548d0(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  void *rectangle[2];
  void *secondSlot;
  s32 frame;
  s32 duration;
  void **draw;
  s32 fadeStart;
  s32 fadeEnd;
  s32 cueFrame;
  s32 start;
  struct DualColumn *root;
  s32 rootX;
  s32 height;
  s32 limit;
  s32 puffCells;
  struct DualColumn *column;
  s32 length;
  s32 reach;
  s8 pillarHeight;
  s32 n;
  s32 reactFrame;
  s32 i;
  struct EffectStep *puff;
  s32 puffLife;
  s32 cel;

  // Set up: the canvas layer, the sheet for the mode, and the canvas moved
  // 112 pixels right for the other side.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = 0;
  if (mode == 1) {
    Resource_LoadAndDecompress((s32)&ResourceId_GreenPillarSheet, work->sheet,
                               1, 1);
  } else {
    Resource_LoadAndDecompress((s32)&ResourceId_VineSheet, work->sheet, 1, 1);
  }
  if (effect->side == 1) {
    REG_BG2X = -0x7000;
  }
  // Odd columns draw mirrored.
  BattleEffect_LoadBlitter(104, BLITTER_TRANSPARENT | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
  rectangle[0] = Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(188, BLITTER_TRANSPARENT | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y |
                         COMPOSITE_FLIP_X);
  secondSlot = Ram_HeapSlots->unkb8[1];
  /* FAKEMATCH: the columns draw through a second pointer to the pair,
   * whose address the frame keeps in a stack slot, as the twin's tagged
   * form: rectangle[side] at the columns differs 255 for 237. */
  draw = rectangle;
  draw[1] = secondSlot;

  duration = Data_081983ea[effect->variant] * COLUMN_INTERVAL + 56;

  // Every puff starts free.
  for (i = 0; i != 1024; i++) {
    PUFFS[i].variant = -1;
  }

  // Each column's root, full length (random, capped by its shape) and the
  // frame it starts to grow.
  i = 0;
  root = (struct DualColumn *)work->particles;
  do {
    rootX = (Data_081983ed[i] + (7 & Random16())) - 4;
    root->y = i / 2 + 108;
    root->x = rootX;
    height = (63 & Random16()) + 55;
    root->height = height;
    limit = Data_081983d7[i % 3];
    if (limit < height) {
      root->height = limit;
    }
    limit = i * COLUMN_INTERVAL + 8;
    root->start = limit;
    i++;
    root++;
  } while (i != COLUMN_COUNT);

  work->transferMode = 1; // clear the canvas to 0 every frame
  work->transferValue = 0;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  frame = 0;
  if (duration != 0) {
    cueFrame = duration - 64;
    fadeStart = duration - 20;
    fadeEnd = duration - 4;
    do {
      if (frame == cueFrame) {
        Func_081180e8(PHASE_RESULTS);
      }
      if (frame >= fadeStart && frame < fadeEnd) {
        // Fade the canvas out over the last 16 frames.
        REG_BLDCNT = 0x3f44;
        REG_BLDALPHA = ((duration - frame) - 5) | 0x1000;
      }
      // Column i grows 8 pixels a frame from frame i * 4 + 8 to its full
      // length (a pillar no taller than its height table allows); odd
      // columns are mirrored.
      if (frame < fadeEnd) {
        i = 0;
        if (Data_081983ea[effect->variant] != 0) {
          do {
            column = (struct DualColumn *)((u8 *)work->particles +
                                           i * sizeof(struct DualColumn));
            start = i * COLUMN_INTERVAL + 8;
            if (frame == i * COLUMN_INTERVAL + 9) {
              work->shakeFrames = 2;
            }

            if (frame > start) {
              u32 shape = i % 3;

              length = (frame - start) * 8;
              reach = column->height;
              if (length > reach) {
                length = reach;
              }
              if (mode == 0) {
                {
                  s32 side = 1 & i;
                  s32 cell = Data_081983da[shape];
                  shape = Data_081983d4[shape];
                  ((DrawRectangle)draw[side])(
                      canvas, work->sheet + cell, column->x - (shape >> 1),
                      column->y - length, shape, length);
                }
              } else {
                pillarHeight = Data_081983fd[7 & i];
                if (length > pillarHeight) {
                  length = pillarHeight;
                }
                {
                  s32 side = 1 & i;
                  s32 cell = Data_081983e4[shape];
                  shape = Data_081983e0[shape];
                  ((DrawRectangle)draw[side])(
                      canvas, work->sheet + cell, column->x - (shape >> 1),
                      column->y - length, shape, length);
                }
              }
            }

            // The targets flinch four frames after the column starts; even
            // columns sound the cue.
            n = 0;
            if (effect->count != 0) {
              reactFrame = start + 4;
              do {
                if (frame == reactFrame) {
                  if (!(i & 1)) {
                    Sound_PlayFar(CUE_COLUMN);
                  }
                  Func_0814cd48(effect->actors[n], 7, 5, n, 3);
                }
                n++;
              } while (n != effect->count);
            } else {
              reactFrame = start + 4;
            }

            // Four and eight frames in, a puff of dust rises at the root.
            if ((frame == reactFrame) || (frame == (start + 8))) {
              struct EffectStep *p = PUFFS;
              n = 0;
              while (n != PUFF_COUNT) {
                if (p->variant == -1) {
                  p->x = ((Random16() & 0xf) + column->x) - 8;
                  p->y = (Random16() & 0xf) + 80;
                  p->variant = 0;
                  break;
                }
                p++;
                n++;
              }
            }
            i++;
          } while (i != Data_081983ea[effect->variant]);
        }
      }

      // The dust: seven cels, two frames each, drawn mirrored about x.
      i = 0;
      puff = PUFFS;
      do {
        puffLife = puff->variant;
        if (puffLife >= 0) {
          cel = puffLife / 2;
          puffCells = 0x1e59;
          if (mode != 0) {
            puffCells = 0xaff;
          }
          ((DrawRectangle)rectangle[0])(
              canvas, work->sheet + (Data_08198414[cel] + puffCells),
              puff->x - Data_08198405[cel],
              puff->y - (s8)Data_0819840c[cel] / 2, Data_08198405[cel],
              (s8)Data_0819840c[cel]);
          ((DrawRectangle)rectangle[1])(
              canvas, work->sheet + (Data_08198414[cel] + puffCells), puff->x,
              puff->y - (s8)Data_0819840c[cel] / 2, Data_08198405[cel],
              (s8)Data_0819840c[cel]);
          puff->variant++;
          if (puff->variant == PUFF_LIFE) {
            puff->variant = -1;
          }
        }
        puff++;
        i++;
      } while (i != PUFF_COUNT);

      Func_08158ce0(4, 4);
      ObjectGroup_CountDownMemberTimers();
      work->transferPending = 1;
      WaitFrames(1);
      frame++;
    } while (frame != duration);
  }
  // Tear down: stop the transfer, free both blitters, close the layer.
  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

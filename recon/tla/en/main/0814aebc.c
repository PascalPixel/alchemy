// Near miss, differ 20 of 300 (effect kind 37, the bouncing sheep).

// ☀️ twin: BattleFx_RunSleep (games/tbs/src/effect/effect.c), shape
// 0.776, structure 0.977. ⚓️'s changes: the canvas layer at priority 1; the
// effect runs 98 frames where ☀️ runs 148; the sheep follow the middle of
// the targets (Func_0815e22c) where ☀️ moves BG2 for side 1: the middle,
// less 64, is clamped to 0..112 into BG2's x, and what the clamp cut off
// becomes a scroll added to every sheep's x; the cels are at sheet + 0x800;
// the blitters are transparent, ⚓️ mode 2 (☀️ 1).

// Declaration order sets the native frame: the middle above the position
// record, and drawB, drawA, scroll spilled in that order. What is left:
// the argument spill (str r0) that the native issues before the
// 0x03000000 base, the canvas slot load and store one row apart, the
// REG_BG2PA pool word before its address, the BLDCNT value's address, the
// literal pool's branch placed before the second BattleFx_LoadBlitter call where
// agscc places it after (arm_reorg's length estimate), and pool offsets
// that move with them.

// Tried (differ):
// trial 0's port with ☀️'s declaration order (2026-10-15)            42
// the declaration order above                                        22
// BattleFx_LoadBlitter declared returning s32 as ☀️'s LoadWork does         22
// the sheep in the gMapCellBuffer symbol (extern)                    56
// the sheep through Ram_MapCellBuffer, a constant, as the tree's
// ram_buffer.h writes the buffer                                   20
// greedy declaration-order scan, every local through every place     20
// alchemy permute, 10 minutes, 2 jobs: nothing below 20 (its best
// weighted candidate, an added integer mask, is 21)

#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"
#include "mem.h"
#include "effect_step.h"
#include "motobj.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_display.h"
#include "graphics.h"
#include "transform.h"

#define HEAP_BLITTER 104           // ☀️ HEAP_SLOT_BLITTER
#define HEAP_BLITTER_ALTERNATE 188 // ☀️ HEAP_SLOT_BLITTER_ALTERNATE

// The Lost Age numbers its blitter modes in the reverse of The Broken Seal's
// order: Func_08196404 builds additive for 0, max for 1, transparent for 2.
enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY };

#define CALLBACK_ORDER 0xc80
#define SHEEP_COUNT 12
#define SHEEP_FRAMES 98
#define GROUND_Y (92 << 16)

extern char ResourceId_SheepCardSheet; // directory row 0xe3

void BattleFx_BeginCanvasLayer(s32 bgControl);
void BattleFx_EndCanvasLayer(void);               // ☀️ BattleFx_EndCanvasLayer
void BattleFx_ProcessPendingGraphicsTransfer(void);               // ☀️ ProcessPendingGraphicsTransfer
void Func_081180e8(s32 phase);          // ☀️ BattleEventRuntime_BeginPhaseFar
s32 BattleFx_LoadBlitter(s32 slot, s32 flags); // ☀️ BattleFx_LoadWork
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames); // ☀️ ObjectGroup_UpdateMembers
void Func_0815e22c(struct EffectPosition *middle); // the targets' middle
void SceneTransform_ApplyPosition(s32 *position);
void Audio_PlayCue(s32 cue);

// A flock of sheep cards drops in over the targets, one every four frames,
// and bounces on the ground, tumbling through eight cels; at frame 80 the
// next phase begins, and each target turns back to its own palette in turn.
void BattleFx_RunSleep(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  struct EffectPosition middle;
  s32 record[3];
  struct EffectStep *one;
  struct EffectStep *sheep;
  s32 i;
  s32 frame;
  DrawRectangle drawB;
  DrawRectangle drawA;
  s32 scroll;
  s32 member;

  // Set up: the canvas layer, the sheep sheet and a transparent blitter
  // pair, the second mirrored.
  sheep = (struct EffectStep *)Ram_MapCellBuffer;
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  scroll = 0;
  BattleFx_BeginCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDCNT = scroll;
  Resource_LoadAndDecompress((s32)&ResourceId_SheepCardSheet, work->sheet, 1,
                             1);
  BattleFx_LoadBlitter(HEAP_BLITTER,
                BLITTER_TRANSPARENT << 4 | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
  drawA = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleFx_LoadBlitter(HEAP_BLITTER_ALTERNATE, BLITTER_TRANSPARENT << 4 |
                                            COMPOSITE_CLIP_X |
                                            COMPOSITE_CLIP_Y | COMPOSITE_FLIP);
  drawB = (DrawRectangle)Ram_HeapSlots->unkb8[1];

  // The sheep wait above the screen with a random spin and starting cel.
  for (i = 0; i != 32; i++) {
    one = &sheep[i];
    one->x = ((Random16() & 63) + 32) << 16;
    one->y = -0x200000;
    one->velocityY = Random16() & 0;
    one->z = Random16() & 3;
    one->variant = Random16() & 255;
  }

  // Centre the canvas on the targets; what BG2 cannot reach moves the sheep.
  Func_0815e22c(&middle);
  middle.x -= 64;
  if (middle.x < 0) {
    scroll = middle.x;
    middle.x = 0;
  } else if (middle.x > 112) {
    scroll = middle.x - 112;
    middle.x = 112;
  }
  REG_BG2X = -middle.x << 8;
  work->transferMode = 1;
  work->transferValue = 0;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, CALLBACK_ORDER);
  Audio_PlayCue(142);

  for (frame = 0; frame != SHEEP_FRAMES; frame++) {
    struct BattleCamera *camera;

    camera = Ram_HeapSlots->cameraWork;
    if (frame == 80)
      Func_081180e8(0);

    // Place each target, and from frame 64 restore its palette, one
    // target every 16 frames.
    for (member = 0; member != effect->count; member++) {
      struct MotionObject *object;

      object = GetBattleObjectSlotFar(effect->actors[member])->object;
      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      record[0] = object->x;
      record[1] = 160 << 14;
      record[2] = object->z;
      SceneTransform_ApplyPosition(record);
      if (frame == member * 16 + 64)
        Func_0814cd48(effect->actors[member], 0, 5, -1, 0);
    }

    // The sheep fall and bounce, each spinning through its eight cels:
    // the first four drawn as they are, the last four mirrored.
    one = sheep;
    for (i = 0; i != SHEEP_COUNT; i++, one++) {
      if (frame > i * 4 && one->y <= 0x7fffff) {
        s32 cel;

        cel = (one->variant / 16) & 7;
        if (cel < 4)
          drawA(canvas, work->sheet + (cel << 10) + 0x800,
                scroll + (one->x >> 16) - 16, (one->y >> 16) - 16, 32, 32);
        else
          drawB(canvas, work->sheet + (cel << 10) - 0x800,
                scroll + (one->x >> 16) - 16, (one->y >> 16) - 16, 32, 32);
        one->y += one->velocityY;
        one->velocityY += 0x2000;
        one->variant += one->z;
        if (one->y > GROUND_Y && one->velocityY == 0) {
          s32 speed = one->velocityY + 1;
          one->y = GROUND_Y;
          one->z += 4;
          one->velocityY = -speed / 2;
        }
      }
    }
    work->transferPending = 1;
    WaitFrames(1);
  }

  // Tear down.
  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  BattleFx_EndCanvasLayer();
}

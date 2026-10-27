// Near miss, differ 48 of 518.
//
// Measured 2026-10-25 under -mthumb-slow-loads: differ 48. Most rows left are
// loads from memory or the stack that alchemy-gcc issues one slot earlier than
// the native, which issues only its pool loads early. The first is the
// argument's spill, which the native stores before it starts the 0x03000000
// base; an empty asm before the first statement closes that row (differ 46).

// Effect kinds 38 and 218. Twin: The Broken Seal's
// BattleEffect_RunBind, ported call for call; the body is
// BattleEffect_RunTriangleStrike's (TriangleStrike) with the prelude. The Lost
// Age's changes are those of BattleEffect_RunTriangleStrike, plus: the canvas is
// prepared by Func_0815585c (kind 3 | side << 4, then two three-word arrays)
// where The Broken Seal calls BattleEffect_RunDjinniEntrance(effect, 3, side, 0,
// &x, &y); the Djinn sheet is unpacked with its palette; and the prelude's
// blitter pair comes from BattleEffect_FetchRectangleBlitters.

// Left: order only, as in BattleEffect_RunTriangleStrike: single swaps of a load with its
// neighbour.

// Tried: the prelude arrays declared before the anchor, which gives the
// native's slots (148, 136, then the anchor at 124): 104 -> 98; the
// tagged heap pointer for the second blitter's read (see BattleEffect_RunTriangleStrike)
// 98 -> 84.

#include "battle.h"
#include "graphics.h"
#include "lib.h"
#include "resource.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "motobj.h"
#include "ram_buffer.h"

// Heap blocks are named by their byte offset in Ram_HeapSlots.
#define HEAP_OFFSET(field) ((s32)&((struct HeapSlots *)0)->field)
#define HEAP_BLITTER HEAP_OFFSET(unk64[1])
#define HEAP_BLITTER_ALTERNATE HEAP_OFFSET(unkb8[1])

// The blitter generator's modes, The Broken Seal's in reverse order.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };
#define BLITTER(mode, flags) ((mode) << 4 | (flags))
#define CLIP (COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y)

// The canvas routines every effect shares, with their Broken Seal names.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);      // BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(void);      // ..ProcessPendingGraphicsTransfer
void Func_081180e8(s32 phase); // BattleEventRuntime_StartPhaseFar
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_081496c8(s32 frame, s32 redPhase, s32 greenPhase,
                   s32 bluePhase); // Graphics_UpdatePhasePalette
void Func_080150e4(s32 angle);     // SceneTransform_ApplyRoll
void Func_080151ac(s32 *scale);    // SceneTransform_ApplyScale
void SceneTransform_ApplyYaw(s32 angle);
void BattleEffect_FetchRectangleBlitters(
    s32 flip, DrawRectangle *draw); // ..FetchRectangleBlitters
void Func_0815585c(struct BattleEffectArgument *effect, s32 kindAndSide,
                   s32 *origin, s32 *size); // BattleEffect_RunDjinniEntrance
void Audio_PlayCue(s32 cue);

extern char ResourceId_RuneSheet;
extern char ResourceId_Data134;
extern char ResourceId_ParticleSpritesD;
extern char ResourceId_JupiterDjinnSheet;
extern s32 Data_0819790c[]; // the triangle's first corner: (0, 24, 0)
extern u16 Data_08197424[]; // flare sizes 1 to 5: cell offsets
#define FLARE_SHEET 0x1000  // the flares, after the rune picture
#define DJINN_SHEET 0x2000  // the Jupiter Djinn, after the flares
#define DJINN_SIZE 40

#define CORNERS 3
#define EDGE_STEPS 24 // flares drawn along each edge

// Spinning triangle (kinds 38 and 218): as BattleEffect_RunTriangleStrike, a triangle of
// flares spins down onto the target and leaves the rune picture where it
// lands; when the effect has a prelude, the Jupiter Djinn also circles
// above its place for the first frames and rises away after frame 32.
void BattleEffect_RunBind(struct BattleEffectArgument *effect) {
  s32 origin[3];
  s32 originSize[3];
  struct EffectPosition anchor;
  s32 vector[3];
  struct EffectPosition screen;
  s32 scale[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 member;
  s32 *camera;
  s32 shift;
  u8 *graphics;
  struct MotionObject *object;
  s32 tick;
  struct EffectStep *point;
  s32 size;
  s32 j;
  s32 k;
  struct HeapSlots *heap;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  graphics = Ram_HeapSlots->unk64[0];
  camera = Ram_HeapSlots->cameraWork;
  /* FAKEMATCH: a pointer to the heap slots held from here, used only for
   * the second blitter's read: spilled, it makes reload rebuild the
   * 0x03000000 base at that read (movs, lsls, adds #188) as the native does,
   * where agscc otherwise keeps the base in r5 across the second
   * BattleEffect_LoadBlitter. Measured (2026-10-15): TriangleStrike 94 -> 66,
   * SpinningTriangle 98 -> 84; the pointer for both reads, 95 and 103. */
  heap = Ram_HeapSlots;

  // Set up: the rune picture with its palette, the dots, the flares, the
  // Djinn with its palette, and a transfer that fades the canvas by 4 each
  // frame.
  BattleEffect_StartCanvasLayer(1);
  if (effect->prelude == 1)
    Func_0815585c(effect, effect->side << 4 | 3, origin, originSize);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + FLARE_SHEET, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_JupiterDjinnSheet,
                             work->sheet + DJINN_SHEET, 1, 0);
  work->transferMode = 3;
  work->transferValue = 0x04040404;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // The background is shifted so that the target stands at x = 64.
  EffectPosition_ProjectScaledBattler(effect->actors[0], &anchor);
  shift = 64 - anchor.x;
  REG_BG2X = shift << 8;
  Audio_PlayCue(142);

  for (frame = 0; frame != effect->count * 20 + 72; frame++) {
    if (frame == 64)
      Func_081180e8(0);
    Func_081496c8(frame, 0xaaab, 0x5555, 0);

    // The prelude: the Djinn circles where the effect was prepared, and
    // rises away after frame 32; its blitters live for this frame only.
    if (effect->prelude == 1) {
      s32 x;
      s32 y;

      x = ((Trig_Sin(frame << 11) * 20) >> 16) + origin[0] + shift - 20;
      y = ((Trig_Cos(frame << 11) << 2) >> 16) + origin[1] - 24;
      BattleEffect_FetchRectangleBlitters(effect->side, draw);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[0](canvas, work->sheet + DJINN_SHEET, x, y, DJINN_SIZE, DJINN_SIZE);
      if (frame <= 3)
        draw[1](canvas, work->sheet + DJINN_SHEET, x, y, DJINN_SIZE,
                DJINN_SIZE);
      Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
      Runtime_ReleaseHeapBlock(HEAP_BLITTER);
    }

    // A fresh pair of max blitters each frame, the second flipped.
    BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_MAX, CLIP));
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                  BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
    draw[1] = (DrawRectangle)heap->unkb8[1];
    if (frame > 16 && (frame & 15) == 0)
      work->transferValue += 0x01010101;

    for (member = 0; member != 1; member++) {
      tick = frame - member * 8;
      object = GetBattleObjectSlotFar(effect->actors[member])->object;
      if (tick >= 0 && tick < 96) {
        Render_ResetTransformState();
        Transform_SetLookAt(camera, camera + 3);
        vector[0] = object->x;
        vector[1] = object->y;
        vector[2] = object->z;
        EffectPosition_ProjectPoint(vector, &screen);
        screen.x = anchor.x + shift;
        screen.y -= 24;

        // The three corners turn and shrink toward the target until frame
        // 64, then hold; flares are drawn along the edges between them.
        if (tick <= 67) {
          for (j = 0; j != CORNERS; j++) {
            point = &work->particles[member * 32 + j];
            Render_ResetTransformState();
            if (tick <= 63) {
              scale[0] = 0x2a000 - (tick * 3 << 9);
              scale[1] = 0x2a000 - (tick * 3 << 9);
              scale[2] = 0x2a000 - (tick * 3 << 9);
              Func_080151ac(scale);
              Func_080150e4((64 - tick) << 9);
              SceneTransform_ApplyYaw((64 - tick) << 9);
            }
            Func_080150e4(j * 0x5555);
            EffectPosition_ProjectPoint(Data_0819790c,
                                               (struct EffectPosition *)vector);
            point->velocityX = vector[0] + screen.x;
            point->velocityY = vector[1] + screen.y + 16;
          }
          for (j = 0; j != CORNERS; j++) {
            struct EffectStep *from = &work->particles[j + member * 32];
            struct EffectStep *to = &work->particles[(j + 1) % 3 + member * 32];

            size = 5 - tick / 16;
            for (k = 0; k != EDGE_STEPS; k++) {
              s32 x =
                  from->velocityX + k * (to->velocityX - from->velocityX) / 24;
              s32 y =
                  from->velocityY + k * (to->velocityY - from->velocityY) / 24;

              draw[0](canvas,
                      work->sheet + Data_08197424[size - 1] + FLARE_SHEET,
                      x - size, y - size, size * 2, size * 2);
            }
          }
        }

        // Landed: the rune picture, its right half mirrored from the left.
        if (tick > 63) {
          draw[0](canvas, work->sheet, screen.x - 24, screen.y - 24, 24, 48);
          draw[1](canvas, work->sheet, screen.x, screen.y - 24, 24, 48);
        }
      }
    }
    Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_BLITTER);
    work->transferPending = 1;
    WaitTicks(1);
  }

  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

// Near miss, differ 30 of 428.
//
// Measured 2026-10-25 under -mthumb-slow-loads: differ 30. Most rows left are
// loads from memory or the stack that alchemy-gcc issues one slot earlier than
// the native, which issues only its pool loads early. The first is the
// argument's spill, which the native stores before it starts the 0x03000000
// base; an empty asm before the first statement closes that row (differ 28).

// Effect kind 67. Twin: The Broken Seal's BattleEffect_RunTriangleStrike, ported
// call for call. The Lost Age's changes: three Resource_LoadAndDecompress
// loads (the rune picture with its palette into the work sheet, resource
// 0x134 into the sheet slot, ParticleSpritesD after the rune picture), the
// callback at 0xc80, BattleEffect_LoadBlitter for the two blitters, the landed picture
// drawn from work->sheet, and the effect argument read directly.

// Left: order only. The frame is the native's (132 bytes); the prologue's
// argument spill, the object's three stores and about a dozen more places
// are a load one row from its neighbour.

// Tried (2026-10-15): alchemy permute 10 minutes (56, through four
// rewrites: REG_BG2PA written before StartCanvasLayer, which would undo the
// layer's own 0x80 and was rejected; the work pointer copied; &vector
// through a pointer temporary; and a 0x03000000 integer held from the setup
// for the second blitter's read). The last, written as the tagged heap
// pointer, takes 94 to 66; the others, alone, 88 (rejected), 94 and 94.

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
void Audio_PlayCue(s32 cue);

extern char ResourceId_RuneSheet;
extern char ResourceId_Data134;
extern char ResourceId_ParticleSpritesD;
extern s32 Data_08197918[]; // the triangle's first corner: (0, 24, 0)
extern u16 Data_08197424[]; // flare sizes 1 to 5: cell offsets
#define FLARE_SHEET 0x1000  // the flares, after the rune picture

#define CORNERS 3
#define EDGE_STEPS 24 // flares drawn along each edge

// Triangle strike (kind 67): a triangle of flares spins down onto the
// target, shrinking, for 64 frames; then the rune picture is stamped in two
// halves where it landed. The caster approaches the target at frame 46, the
// palette pulses throughout and the trail fades a step every 16 frames.
void BattleEffect_RunTriangleStrike(struct BattleEffectArgument *effect) {
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

  // Set up: the rune picture with its palette, the dots, the flares, and a
  // transfer that fades the canvas by 4 each frame.
  BattleEffect_StartCanvasLayer(0);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_RuneSheet, work->sheet, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD,
                             work->sheet + FLARE_SHEET, 0, 0);
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
    if (frame == 46)
      BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0);
    Func_081496c8(frame, 0xaaab, 0x5555, 0);

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
            EffectPosition_ProjectPoint(Data_08197918,
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

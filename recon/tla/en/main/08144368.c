// Draft, differ 13 of 294, skeleton gap 3 removed, 2 added (tla-en, 2026-11-01)

// Effect kind 34. Twin: The Broken Seal's BattleEffect_RunUnusedOrbitingSpirals
// (planet_diver.c), the same calls, data and control flow, ported call for
// call; the declaration order is the twin's. The Lost Age's changes: the
// effect argument read directly, the spiral sheet unpacked with its palette by
// Resource_LoadAndDecompress, two blitters from BattleEffect_LoadBlitter (max,
// then max clipped and flipped both ways), both callbacks at 0xc80 and
// Func_0814cd48 for the members' timers.

// Left: one scheduling pair, nothing else. Before the member loop the native
// has movs r3, #36; movs r2, #56; str r3, [sp, #12]; add r2, sp; mov sl, r2.
// Here the point's address comes first, so its two reload instructions sit
// side by side and become add r2, sp, #56, one instruction shorter; every
// other difference is a branch distance or pool offset that follows from it.
// The 36 is strength reduction's start for effect->actors, added after the
// point's hoisted address, so the order is not the source's to choose directly.
//
// The spin is (frame + i * 32) << 9, not the twin's (frame << 9) + (i << 14):
// the twin's form lengthens the screen address's life by one instruction, and
// with this game's shorter member loop loop.c then hoists it out and spills it
// (72-byte frame, skeleton 7/10). Measured from there: this form 95 -> 47, the
// twin's declaration order 47 -> 13.
// Tried on the last pair, no change: point and screen in the frame, member and
// if blocks; screen before point; a for or do loop for the members and the
// cels; the if around the member loop dropped; &point[0], *point, a pointer
// local and a struct for the point; the other spellings of the spin.

#include "battle.h"
#include "graphics.h"
#include "lib.h"
#include "resource.h"
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
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void); // ..ProcessPendingGraphicsTransfer
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleEffect_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 duration, s32 timer,
                   s32 frames); // BattleEffect_TintBattler
void BattleEffect_ArmBg2AffineHBlankDma(void);

extern char ResourceId_SpiralSheet;

#define SPIRAL_SIZE 32
#define SPIRAL_CELS 4

// Member orbit: over each target in turn, sixteen frames apart, four
// spirals circle a point 40 pixels above it for 60 frames, the blitter
// flipping every sixteen frames; the background waves line by line.
void BattleEffect_RunOrbitingSpirals(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  DrawRectangle draw[2];
  s32 point[3];
  struct EffectPosition screen;
  s32 member;
  s32 offsetX;
  s32 frame;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];

  // Set up: the spiral sheet with its palette, the two blitters, the wave's
  // HBlank DMA and the transfer at half strength.
  BattleEffect_StartCanvasLayer(0);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_SpiralSheet, work->sheet, 1, 1);
  BattleEffect_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_MAX, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleEffect_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, 0xc80);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // On the far side the background and the spirals move 112 pixels left.
  if (effect->side == 1) {
    REG_BG2X = -0x6800;
    offsetX = -112;
  } else {
    offsetX = 0;
  }

  for (frame = 0; frame != (effect->count * 16) + 48; frame++) {
    s32 *camera;
    s32 *row;
    s32 i;

    // The background waves sideways line by line.
    camera = Ram_HeapSlots->cameraWork;
    row = work->bg2xLines;
    if (effect->side == 0) {
      s32 angle;
      s32 base;

      for (i = 0, base = 0x80000, angle = frame << 10; i != 160; i++) {
        *row++ = (base - (Trig_Sin(angle) << 3)) >> 10;
        angle += 1024;
      }
    } else {
      s32 angle;

      for (i = 0, angle = frame << 10; i != 160; i++) {
        *row++ = ((Trig_Sin(angle) << 3) >> 10) - 0x7000;
        angle += 1024;
      }
    }

    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);
    member = 0;
    if (effect->count != 0) {
      while (member != effect->count) {
        struct MotionObject *target;

        target = GetBattleObjectSlotFar(effect->actors[member])->object;
        if (frame > member * 16 && frame < (member * 16) + 60) {
          s32 spin;

          // Halfway through, the target's flash stops.
          if (frame == (member * 16) + 32)
            Func_0814cd48(effect->actors[member], 0, 5, -1, 0);
          point[0] = target->x;
          point[1] = 40 << 16;
          point[2] = target->z;
          EffectPosition_ProjectPoint(point, &screen);

          // The four spirals, a quarter turn apart, each a 32 x 32 cel of
          // the sheet's four-frame animation.
          for (i = 0; i != SPIRAL_CELS; i++) {
            s32 x;
            s32 y;
            s32 half;

            spin = (frame + i * 32) << 9;
            x = (screen.x + ((Trig_Sin(spin) << 4) >> 16)) + offsetX;
            y = screen.y + ((Trig_Cos(spin) << 4) >> 16);
            half = frame / 16;
            draw[half & 1](canvas,
                           work->sheet + (((frame / 4) - (half * 4)) << 10),
                           x - SPIRAL_SIZE / 2, y - SPIRAL_SIZE / 2,
                           SPIRAL_SIZE, SPIRAL_SIZE);
          }
        }
        member++;
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  HeapSlot_Release(HEAP_BLITTER_ALTERNATE);
  HeapSlot_Release(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

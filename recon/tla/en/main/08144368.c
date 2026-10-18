// Draft, differ 101 of 294 (the draft has 297).

// Effect kind 34. Twin: The Broken Seal's BattleFx_RunMemberOrbit, ported
// call for call. The Lost Age's changes: the effect argument read directly,
// the spiral sheet unpacked with its palette by Resource_LoadAndDecompress,
// two blitters from BattleFx_LoadBlitter (max, then max clipped and flipped both
// ways), both callbacks at 0xc80 and Func_0814cd48 for the members' timers.

// Left: allocation. The instructions are the native's. The native keeps the
// effect argument in r9, the point array's address in r10, and takes the
// screen position's address inside the member loop (add r5, sp, #44, then
// r11 for the four cels), in a 68-byte frame; here the screen address is
// hoisted out of the frame loop and spilled (72 bytes) and the argument takes
// r11. Tried: BattleFx_LoadBlitter returning s32 (123 -> 119); screen declared in
// the member block (no change); a greedy scan of the declaration order
// (canvas before work, offsetX before member) 119 -> 101.

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
void BattleFx_BeginCanvasLayer(s32 bgControl);
void BattleFx_EndCanvasLayer(void); // BattleFx_EndCanvasLayer
void BattleFx_ProcessPendingGraphicsTransfer(void); // ..ProcessPendingGraphicsTransfer
s32 BattleFx_LoadBlitter(s32 slot, s32 modeAndFlags); // BattleFx_BuildBlitter
void Func_0814cd48(s32 actor, s32 palette, s32 duration, s32 timer,
                   s32 frames); // ObjectGroup_UpdateMembers
void BattleFx_ArmBg2AffineHBlankDma(void);       // BattleFx_ArmBg2AffineHBlankDma

extern char ResourceId_SpiralSheet;

#define SPIRAL_SIZE 32
#define SPIRAL_CELS 4

// Member orbit: over each target in turn, sixteen frames apart, four
// spirals circle a point 40 pixels above it for 60 frames, the blitter
// flipping every sixteen frames; the background waves line by line.
void BattleFx_RunOrbitingSpirals(struct BattleEffectArgument *effect) {
  void *canvas;
  struct BattleEffectWork *work;
  DrawRectangle draw[2];
  s32 point[3];
  struct EffectPosition screen;
  s32 offsetX;
  s32 member;
  s32 frame;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];

  // Set up: the spiral sheet with its palette, the two blitters, the wave's
  // HBlank DMA and the transfer at half strength.
  BattleFx_BeginCanvasLayer(0);
  REG_BG2PA = 0x100;
  Resource_LoadAndDecompress((s32)&ResourceId_SpiralSheet, work->sheet, 1, 1);
  BattleFx_LoadBlitter(HEAP_BLITTER, BLITTER(BLIT_MAX, CLIP));
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  BattleFx_LoadBlitter(HEAP_BLITTER_ALTERNATE,
                BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP));
  draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];
  Scheduler_AddOrUpdateCallback(BattleFx_ArmBg2AffineHBlankDma, 0xc80);
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0xc80);

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

            spin = (frame << 9) + (i << 14);
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
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Scheduler_RemoveCallback(BattleFx_ArmBg2AffineHBlankDma);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  BattleFx_EndCanvasLayer();
}

// Near miss, differ 62 of 652.

// The body of three effect kinds (the wrappers in series_c.c pass modes 0 to
// 2). Twin: The Broken Seal's BattleFx_RunMemberBeam, ported call for call.
// The Lost Age's changes: a third mode (the Mars Djinn); every picture
// unpacked with its palette by Resource_LoadAndDecompress; the canvas
// prepared by Func_0815585c (mode | (side ^ prelude) << 4, then two
// three-word arrays) where The Broken Seal calls
// BattleFx_PrepareCanvasEffect; Func_08144aac for the Djinn's pair; the
// callback at 0xc80; the effect argument read directly; the camera read
// each frame.

// Left: order only: single swaps of a load (the effect's fields, the work
// pointer's spill, a table) with the constant beside it, about thirty
// places.

// Tried (2026-10-15): the plain port 182; the Djinn arrays declared first
// (the native's slots 128 and 116) 170; the prelude in its own local, not
// The Broken Seal's reuse of k (the native keeps it in r7 within the Djinn
// block) 152; the tagged heap pointer 62.

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

// The motes live in the map buffer, 64 for each target.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

// The canvas routines every effect shares, with their Broken Seal names.
void BattleFx_BeginCanvasLayer(s32 bgControl);
void Func_08143bb8(void);      // BattleFx_EndCanvasLayer
void Func_08143000(void);      // ..ProcessPendingGraphicsTransfer
void Func_081180e8(s32 phase); // BattleEventRuntime_BeginPhaseFar
s32 Func_081963ec(s32 slot, s32 modeAndFlags); // BattleEffect_LoadWork
void Func_0814cd48(s32 actor, s32 palette, s32 duration, s32 timer,
                   s32 frames);                    // ObjectGroup_UpdateMembers
void Func_08144aac(s32 flip, DrawRectangle *draw); // ..FetchRectangleBlitters
void Func_0815585c(struct BattleEffectArgument *effect, s32 kindAndSide,
                   s32 *origin, s32 *size); // BattleFx_PrepareCanvasEffect
void SceneTransform_ApplyPosition(s32 *position);
void Audio_PlayCue(s32 cue);

extern char ResourceId_EarthWallSheet;
extern char ResourceId_CrescentMoonSheet;
extern char ResourceId_VenusDjinnSheet;
extern char ResourceId_MercuryDjinnSheet;
extern char ResourceId_MarsDjinnSheet;
extern u8 Data_0819786c[];  // the mote's picture for each step of its sway
extern u16 Data_08197874[]; // the beam's head: three pictures, offsets
extern u8 Data_0819787a[];  // ... their widths
extern u8 Data_0819787d[];  // ... their heights

// The pictures in the work sheet, after the beam (EarthWallSheet).
#define PIC_MOTE 0x2710  // the crescent motes, 24 x 24
#define PIC_DJINN 0x65c0 // the Djinn, 40 x 40
#define MOTES 8          // per target

// Member beam (kinds through three wrappers; the mode picks the Djinn:
// 0 Venus, 1 Mercury, 2 Mars): with a prelude, the Djinn appears, sways and
// rises away; for each target in turn, sixteen frames apart, eight motes
// drift down swaying; from frame 72 a beam of light falls on each target,
// flashing it, and narrows from frame 88.
void Func_08144b14(struct BattleEffectArgument *effect, s32 mode) {
  s32 djinn[3];
  s32 djinnSize[3];
  struct EffectPosition one;
  struct EffectPosition pos;
  s32 ground[3];
  s32 point[3];
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 offsetX;
  s32 i;
  s32 k;
  struct HeapSlots *heap;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  /* FAKEMATCH: a pointer to the heap slots held from here, used only for
   * the second blitter's read: spilled, it makes reload rebuild the
   * 0x03000000 base there, as the native does (see Func_0814627c).
   * Measured (2026-10-15): 152 without it, 62 with it. */
  heap = Ram_HeapSlots;

  // Set up: the canvas for the Djinn when there is a prelude, then the beam,
  // the motes and the mode's Djinn, each with its palette.
  BattleFx_BeginCanvasLayer(0);
  if (effect->prelude == 1)
    Func_0815585c(effect, (effect->side ^ effect->prelude) << 4 | mode, djinn,
                  djinnSize);
  Resource_LoadAndDecompress((s32)&ResourceId_EarthWallSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_CrescentMoonSheet,
                             work->sheet + PIC_MOTE, 1, 1);
  if (mode == 0)
    Resource_LoadAndDecompress((s32)&ResourceId_VenusDjinnSheet,
                               work->sheet + PIC_DJINN, 1, 1);
  else if (mode == 1)
    Resource_LoadAndDecompress((s32)&ResourceId_MercuryDjinnSheet,
                               work->sheet + PIC_DJINN, 1, 1);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_MarsDjinnSheet,
                               work->sheet + PIC_DJINN, 1, 1);

  // The motes start 80 pixels up, each at its own phase of the sway.
  for (i = 0; i != 512; i++) {
    PARTICLES[i].x = 0;
    PARTICLES[i].y = 80 << 16;
    PARTICLES[i].z = (Random16() | -32) << 14;
    PARTICLES[i].variant = Random16() & 255;
  }

  // With one target, BG2 is shifted so that it stands at x = 64.
  REG_BG2PA = 0x100;
  if (effect->count == 1) {
    EffectPosition_ApplyStepAndYOffset(effect->actors[0], &one);
    offsetX = 64 - one.x;
  } else {
    if (effect->side == 1)
      offsetX = -112;
    else
      offsetX = 0;
  }
  REG_BG2X = offsetX << 8;
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);

  for (frame = 0; frame != effect->count * 16 + 116; frame++) {
    s32 *camera = Ram_HeapSlots->cameraWork;
    s32 prelude;

    if (frame == 64)
      Audio_PlayCue(212);
    if (frame == 80)
      Func_081180e8(0);

    // The Djinn sways 20 pixels either way and rises from frame 32.
    prelude = effect->prelude;
    if (prelude == 1) {
      s32 angle = frame << 11;
      s32 x = (-Trig_Sin(angle) * 20 >> 16) + djinn[0] + offsetX - 20;
      s32 y = (Trig_Cos(angle) * 4 >> 16) + djinn[1] - 24;

      Func_08144aac(effect->side ^ prelude, draw);
      if (frame > 32)
        y = y - frame * 2 + 64;
      draw[1](canvas, work->sheet + PIC_DJINN, x, y, 40, 40);
      if (frame <= 3)
        draw[1](canvas, work->sheet + PIC_DJINN, x, y, 40, 40);
      Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
      Runtime_ReleaseHeapBlock(HEAP_BLITTER);
    }
    Func_081963ec(HEAP_BLITTER, BLITTER(BLIT_MAX, CLIP));
    draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
    Func_081963ec(HEAP_BLITTER_ALTERNATE,
                  BLITTER(BLIT_MAX, CLIP | COMPOSITE_FLIP_X));
    draw[1] = (DrawRectangle)heap->unkb8[1];

    // A mote every eight frames drifts down a pixel a frame, swaying 16
    // pixels, until it is 8 pixels from the ground.
    for (k = 0; k != effect->count; k++) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(effect->actors[k])->object;
      s32 start = k * 16;

      Render_ResetTransformState();
      Graphics_PrepareTransferInIwramWork(camera, camera + 3);
      point[0] = target->x;
      point[1] = 0;
      point[2] = target->z;
      SceneTransform_ApplyPosition(point);
      if (frame > start) {
        for (i = 0; i != MOTES; i++) {
          struct EffectStep *mote = &PARTICLES[k * 64 + i];

          if (frame > i * 8 + start && mote->y > 8 << 16) {
            s32 sway;

            EffectPosition_ApplyBaseAndYOffset((s32 *)mote, &pos);
            pos.x += offsetX;
            sway = Trig_Sin(mote->variant << 10) * 16 >> 16;
            if (i & 1)
              pos.x -= sway;
            else
              pos.x += sway;
            draw[i & 1](canvas,
                        work->sheet +
                            Data_0819786c[mote->variant / 8 & 7] * 576 +
                            PIC_MOTE,
                        pos.x - 12, pos.y - 12, 24, 24);
            mote->y -= 1 << 16;
            mote->variant++;
          }
        }
      }
    }

    // The beam: from the top of the screen to the target's feet, drawn in
    // two mirrored halves, six pixels wide until frame 88, then narrowing a
    // pixel every three frames; its head flickers at the feet.
    for (i = 0; i != effect->count; i++) {
      s32 start = i * 16;

      if (frame >= start + 72) {
        struct MotionObject *target =
            GetBattleObjectSlotFar(effect->actors[i])->object;

        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(camera, camera + 3);
        if (frame == start + 72)
          Func_0814cd48(effect->actors[i], 1, -1, -1, 0);
        if (frame == start + 88)
          Func_0814cd48(effect->actors[i], 0, -1, -1, 0);
        ground[0] = target->x;
        ground[1] = 0;
        ground[2] = target->z;
        EffectPosition_ApplyBaseAndYOffset(ground, &pos);
        pos.x += offsetX;
        if (frame < start + 104) {
          s32 pic = frame / 4;
          s32 width = 6;

          if (frame > start + 88)
            width = 6 - (frame - start - 88) / 3;
          if (pic > 2)
            pic = (pic & 1) + 1;
          if (frame < start + 100) {
            draw[0](canvas, work->sheet + Data_08197874[pic],
                    pos.x - Data_0819787a[pic], pos.y - Data_0819787d[pic] + 8,
                    Data_0819787a[pic], Data_0819787d[pic]);
            draw[1](canvas, work->sheet + Data_08197874[pic], pos.x,
                    pos.y - Data_0819787d[pic] + 8, Data_0819787a[pic],
                    Data_0819787d[pic]);
          }
          for (k = 0; k != pos.y; k++) {
            draw[0](canvas, work->sheet + 5, pos.x - width, k, width, 1);
            draw[1](canvas, work->sheet + 5, pos.x, k, width, 1);
          }
        }
      }
    }
    Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_BLITTER);
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(Func_08143000);
  Func_08143bb8();
}

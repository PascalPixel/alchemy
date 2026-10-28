// Near miss, differ 34 of 355 (the draft has 356).
//
// Measured 2026-10-25 under -mthumb-slow-loads: differ 34. Most rows left are
// loads from memory or the stack that alchemy-gcc issues one slot earlier than
// the native, which issues only its pool loads early. The first is the
// argument's spill, which the native stores before it starts the 0x03000000
// base; an empty asm before the first statement closes that row (differ 32).

// Effect kind 39. Twin: The Broken Seal's BattleEffect_RunHaunt, ported
// call for call. The Lost Age's changes: work and canvas read directly, the
// camera read again each frame; the skull sheet unpacked with its palette by
// Resource_LoadAndDecompress and the pink burst palette copied after it;
// BattleEffect_FetchRectangleBlitters for the max and additive pair; both callbacks
// at 0xc80; the effect runs count * 32 + 64 frames (96 there) and begins the
// results at frame 92 (96 there); the actor loops read the effect argument.

// Left: order and one instruction. The native takes &pos as add r1, sp, #64
// where agscc builds it as movs r1, #64; add r1, sp (the extra instruction:
// in the native the address passes through a low register before r8, in the
// draft r8's pseudo is reloaded); 0xc80 is built whole before the first
// call in the native; the rest are single swaps of a load with its
// neighbour.

// Tried (2026-10-15): alchemy permute 10 minutes (nothing below 75 kept: a
// temporary for target->z, a block, a moved statement); a greedy scan of
// the declaration order (k first) 75 -> 69; pos before the point, which
// gives the native's slots (64 and 52), is 69 too and is kept.

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
#include "iwram_call.h"
#include "motobj.h"
#include "ram_buffer.h"

// The skulls live in the map buffer, 64 for each target.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)

// The canvas routines every effect shares, with their Broken Seal names.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);      // ..ProcessPendingGraphicsTransfer
void BattleEffect_CountDownBattlerTints(void);
void Func_081180e8(s32 phase); // BattlePlayback_StartFar
void Func_0814cd48(s32 actor, s32 palette, s32 duration, s32 timer,
                   s32 frames);                    // BattleEffect_TintBattler
void BattleEffect_FetchRectangleBlitters(
    s32 flip, DrawRectangle *draw); // ..FetchRectangleBlitters
void BattleEffect_ArmBg2AffineHBlankDma(void);
void SceneTransform_ApplyPosition(s32 *position);
void SceneTransform_ApplyPitch(s32 angle);
void Sound_PlayFar(s32 cue);

extern char ResourceId_SkullSheet;
extern char ResourceId_PinkBurstSheet;

#define SKULLS 8 // per target
#define SKULL_SIZE 24
#define SKULL_CELL (24 * 24)

// Closing skulls: for each target in turn, 32 frames apart, eight skulls
// appear one every four frames from all around, up to 64 pixels away, and
// close in on a point 20 pixels above the target while the view of them
// turns; the background waves line by line throughout.
void BattleEffect_RunHaunt(struct BattleEffectArgument *effect) {
  s32 k;
  struct EffectPosition pos;
  s32 point[3];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  DrawRectangle draw[2];
  s32 *camera;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];

  // Set up: the skull sheet with its palette, the pink burst palette, the
  // max and additive blitters, the wave's HBlank DMA and a transfer that
  // fades the canvas by 4 each frame.
  BattleEffect_StartCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_SkullSheet, work->sheet, 1, 1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet), 128);
  BattleEffect_FetchRectangleBlitters(0, draw);
  Task_Add(BattleEffect_ArmBg2AffineHBlankDma, 0xc80);
  work->transferMode = 3;
  work->transferValue = 0x04040404;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  // Scatter every skull up to 64 pixels from its centre.
  for (i = 0; i != 512; i++) {
    PARTICLES[i].x = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].y = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].z = ((Random16() & 255) - 127) << 15;
  }
  Sound_PlayFar(142);

  for (frame = 0; frame != effect->count * 32 + 64; frame++) {
    s32 *row;

    camera = Ram_HeapSlots->cameraWork;
    if (frame == 92)
      Func_081180e8(0);

    // The background waves sideways, one sine step per line.
    row = work->bg2xLines;
    if (effect->side == 0) {
      for (i = 0; i != 160; i++)
        *row++ = (0x60000 - Trig_Sin((frame + i) << 11) * 6) >> 10;
    } else {
      for (i = 0; i != 160; i++)
        *row++ = Trig_Sin((frame + i) << 11) * 6 >> 10;
    }

    for (k = 0; k != effect->count; k++) {
      struct MotionObject *target =
          GetBattleObjectSlotFar(effect->actors[k])->object;

      // Each target's skulls turn about a point 20 pixels above it.
      Render_ResetTransformState();
      Transform_SetLookAt(camera, camera + 3);
      point[0] = target->x;
      point[1] = 20 << 16;
      point[2] = target->z;
      SceneTransform_ApplyPosition(point);
      if (frame > k * 32) {
        SceneTransform_ApplyPitch(frame << 9);
        if (frame == k * 32 + 32)
          Func_0814cd48(effect->actors[k], 7, 5, k, 32);

        // A skull moves one part in its distance toward the centre each
        // frame, and is gone once it arrives.
        for (i = 0; i != SKULLS; i++) {
          struct EffectStep *skull = &PARTICLES[k * 64 + i];

          if (frame > (k * 8 + i) * 4) {
            s32 x = (skull->x >> 8) * (skull->x >> 8);
            s32 y = (skull->y >> 8) * (skull->y >> 8);
            s32 z = (skull->z >> 8) * (skull->z >> 8);
            s32 dist = Iwram_Sqrt(x + y + z) >> 8;

            if (dist != 0) {
              EffectPosition_ProjectPoint((s32 *)skull, &pos);
              pos.x >>= 1;
              draw[0](canvas, work->sheet + i % 3 * SKULL_CELL,
                      pos.x - SKULL_SIZE / 2, pos.y - SKULL_SIZE / 2,
                      SKULL_SIZE, SKULL_SIZE);
              skull->x -= skull->x / dist;
              skull->y -= skull->y / dist;
              skull->z -= skull->z / dist;
              skull->variant++;
            }
          }
        }
      }
    }
    BattleEffect_CountDownBattlerTints();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Task_Remove(BattleEffect_ArmBg2AffineHBlankDma);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

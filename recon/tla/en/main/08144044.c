/* Near miss, differ 69 of 355 (the draft has 356).
 *
 * Effect kind 39. Twin: The Broken Seal's BattleFx_RunClosingSkulls, ported
 * call for call. The Lost Age's changes: work and canvas read directly, the
 * camera read again each frame; the skull sheet unpacked with its palette by
 * Resource_LoadAndDecompress and the pink burst palette copied after it;
 * Func_08144aac for the max and additive pair; both callbacks at 0xc80; the
 * effect runs count * 32 + 64 frames (96 there) and begins the results at
 * frame 92 (96 there); the actor loops read the effect argument.
 *
 * Left: order and one instruction. The native takes &pos as add r1, sp, #64
 * where agscc builds it as movs r1, #64; add r1, sp (the extra instruction:
 * in the native the address passes through a low register before r8, in the
 * draft r8's pseudo is reloaded); 0xc80 is built whole before the first
 * call in the native; the rest are single swaps of a load with its
 * neighbour.
 *
 * Tried (2026-10-15): alchemy permute 10 minutes (nothing below 75 kept: a
 * temporary for target->z, a block, a moved statement); a greedy scan of
 * the declaration order (k first) 75 -> 69; pos before the point, which
 * gives the native's slots (64 and 52), is 69 too and is kept.
 */
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
void BattleFx_BeginCanvasLayer(s32 bgControl);
void Func_08143bb8(void);      // BattleFx_EndCanvasLayer
void Func_08143000(void);      // ..ProcessPendingGraphicsTransfer
void Func_081434f8(void);      // ObjectGroup_TickMemberTimers
void Func_081180e8(s32 phase); // BattleEventRuntime_BeginPhaseFar
void Func_0814cd48(s32 actor, s32 palette, s32 duration, s32 timer,
                   s32 frames);                    // ObjectGroup_UpdateMembers
void Func_08144aac(s32 flip, DrawRectangle *draw); // ..FetchRectangleBlitters
void Func_08152474(void); // BattleFx_ArmBg2AffineHBlankDma
void SceneTransform_ApplyPosition(s32 *position);
void SceneTransform_ApplyPitch(s32 angle);
void Audio_PlayCue(s32 cue);

extern char ResourceId_SkullSheet;
extern char ResourceId_PinkBurstSheet;

#define SKULLS 8 // per target
#define SKULL_SIZE 24
#define SKULL_CELL (24 * 24)

// Closing skulls: for each target in turn, 32 frames apart, eight skulls
// appear one every four frames from all around, up to 64 pixels away, and
// close in on a point 20 pixels above the target while the view of them
// turns; the background waves line by line throughout.
void Func_08144044(struct BattleEffectArgument *effect) {
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
  BattleFx_BeginCanvasLayer(0);
  Resource_LoadAndDecompress((s32)&ResourceId_SkullSheet, work->sheet, 1, 1);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet), 128);
  Func_08144aac(0, draw);
  Scheduler_AddOrUpdateCallback(Func_08152474, 0xc80);
  work->transferMode = 3;
  work->transferValue = 0x04040404;
  Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);

  // Scatter every skull up to 64 pixels from its centre.
  for (i = 0; i != 512; i++) {
    PARTICLES[i].x = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].y = ((Random16() & 255) - 127) << 15;
    PARTICLES[i].z = ((Random16() & 255) - 127) << 15;
  }
  Audio_PlayCue(142);

  for (frame = 0; frame != effect->count * 32 + 64; frame++) {
    s32 *row;

    camera = Ram_HeapSlots->cameraWork;
    if (frame == 92)
      Func_081180e8(0);

    // The background waves sideways, one sine step per line.
    row = work->bg2X;
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
      Graphics_PrepareTransferInIwramWork(camera, camera + 3);
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
              EffectPosition_ApplyBaseAndYOffset((s32 *)skull, &pos);
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
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(Func_08143000);
  Scheduler_RemoveCallback(Func_08152474);
  Runtime_ReleaseHeapBlock(188);
  Runtime_ReleaseHeapBlock(104);
  Func_08143bb8();
}

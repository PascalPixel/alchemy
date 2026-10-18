// Draft, differ 679 of 718 (the draft has 719).

// The body of ten effect kinds (the wrappers in series_d.c pass modes 0 to
// 9). Twin: The Broken Seal's BattleFx_RunSevenMode (shape 0.67), rebuilt
// by The Lost Age: three more modes; a four-byte record per mode
// (Data_08197880: shards or smoke, the shots' blitter, shots per target,
// frames) where The Broken Seal has two bytes and fixed counts; up to four
// shots per target, each with its own start; mode 7 prepares a canvas with
// Func_0815585c and sets BG2CNT; the shots' height by mode; the pictures and
// palettes unpacked by Resource_LoadAndDecompress; the pair from
// BattleFx_FetchRectangleBlitters; modes 4 and 7 clear the canvas each frame
// with blending off; modes 3 and 5 clear both canvases and run a summon canvas
// effect (SummonFx_RunCanvasMode1 or 3) in place of the canvas teardown.

// Left: the calls, their order, the loops and the tables are the native's;
// the allocation is not. The native frame is 100 bytes, this one 108: the
// native keeps mode * 4 in r11 for the whole effect and the shot's start
// (k * 8 + i * 4) in r9, where agscc spills mode * 4 and one more value;
// every frame offset after that differs. The palette chain keeps separate
// branches for modes 2 and 8 in the native (one load each), where agscc
// merges them.

// Tried (2026-10-15): the plain draft 697; the shot's aim in depth through
// a local set straight after Random16, as the native orders it, 679; the
// record offset (mode * 4) as a local, 688 declared last, 702 after the
// camera.

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

// The shots live in the map buffer, 16 for each target, and the bursts
// after them, 32 for each target.
#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)
#define BURSTS ((struct EffectStep *)(Ram_MapCellBuffer + 0x1c00))

// The canvas routines every effect shares, with their Broken Seal names.
void BattleFx_BeginCanvasLayer(s32 bgControl);
void BattleFx_EndCanvasLayer(void);         // BattleFx_EndCanvasLayer
void BattleFx_ProcessPendingGraphicsTransfer(void);         // ..ProcessPendingGraphicsTransfer
void Palette_StepFadeTransfer(void);         // Palette_StepFadeTransfer
void ObjectGroup_TickMemberTimers(void);         // ObjectGroup_TickMemberTimers
void Func_08158ce0(s32 x, s32 y); // Camera_ApplyShake
void Func_081180e8(s32 phase);    // BattleEventRuntime_BeginPhaseFar
void Func_0814cd48(s32 actor, s32 palette, s32 duration, s32 timer,
                   s32 frames);                    // ObjectGroup_UpdateMembers
void BattleFx_FetchRectangleBlitters(
    s32 flip, DrawRectangle *draw); // ..FetchRectangleBlitters
void Func_0815585c(struct BattleEffectArgument *effect, s32 kindAndSide,
                   s32 *origin, s32 *size); // BattleFx_RunDjinniEntrance
void BattleFxKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void SummonFx_RunCanvasMode1(void *action);
void SummonFx_RunCanvasMode3(void *action);
void Audio_PlayCue(s32 cue);

extern char ResourceId_GoldShellSheet;
extern char ResourceId_SmokeSheet;
extern char ResourceId_BluePalette;
extern char ResourceId_SpiderWebSheet;
extern char ResourceId_LimePalette;
extern char ResourceId_GreenPalette;
extern char ResourceId_MarsDjinnSheet;
extern char ResourceId_PinkBurstSheet;

// Each mode's record: whether its shots burst into shards (else into smoke),
// the shots' blitter, shots per target, and frames before the targets'
// share.
extern s8 Data_08197880[];
#define RECORD_SHARDS 0
#define RECORD_SHOT_BLITTER 1
#define RECORD_SHOTS 2
#define RECORD_FRAMES 3
extern u8 Data_081978a8[];  // shard widths
extern u8 Data_081978ba[];  // shard heights
extern u16 Data_081978cc[]; // shard cell offsets

#define SMOKE_SHEET 0x3e80 // the smoke, after the gold shells
#define SHARD_SHEET 0x1e00 // the shards, among the gold shells

// Shell shot (the body of ten kinds; the mode picks the colours and the
// record above the rest): shots arc from the caster to each target, one or
// four per target eight frames apart, turning as they fly; each bursts where
// it lands into smoke or shards, flashing the target. Mode 7 prepares a
// canvas for its prelude first; modes 3 and 5 hand the canvas to a summon
// canvas effect at the end.
void Func_0814512c(struct BattleEffectArgument *effect, s32 mode) {
  struct EffectPosition position;
  s32 preludeSize[3];
  s32 preludeOrigin[3];
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 k;
  s32 total;
  struct MotionObject *source;
  s32 *camera;
  s32 palette;
  s32 i;
  s32 height;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  camera = Ram_HeapSlots->cameraWork;

  // Set up: the canvas (mode 7 with its prelude), the gold shells and the
  // smoke with their palettes, then the mode's palette.
  if (mode == 7) {
    BattleFx_BeginCanvasLayer(0);
    Func_0815585c(effect, effect->side << 4 | 0x44, preludeOrigin, preludeSize);
    REG_BG2CNT = 0x785;
  } else {
    BattleFx_BeginCanvasLayer(1);
    REG_BLDALPHA = 0x1010;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_GoldShellSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_SmokeSheet,
                             work->sheet + SMOKE_SHEET, 1, 1);
  if (mode == 0)
    palette = (s32)&ResourceId_BluePalette;
  else if (mode == 1)
    palette = (s32)&ResourceId_SpiderWebSheet;
  else if (mode == 2)
    palette = (s32)&ResourceId_LimePalette;
  else if (mode == 8)
    palette = (s32)&ResourceId_LimePalette;
  else if (mode == 3)
    palette = (s32)&ResourceId_GreenPalette;
  else if (mode == 4 || mode == 7)
    palette = (s32)&ResourceId_GoldShellSheet;
  else if (mode == 6)
    palette = (s32)&ResourceId_MarsDjinnSheet;
  else if (mode == 9)
    palette = (s32)&ResourceId_PinkBurstSheet;
  else
    palette = (s32)&ResourceId_GreenPalette;
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(palette), 128);

  // The shots leave the caster at the mode's height, each aimed at its
  // target with a little spread in depth.
  for (i = 0; i != 1024; i++)
    PARTICLES[i].variant = -1;
  source = GetBattleObjectSlotFar(effect->actor)->object;
  if (mode == 7)
    height = 0x280000;
  else if (mode == 8)
    height = 0x140000;
  else
    height = 0x40000;
  for (k = 0; k != effect->count; k++) {
    struct MotionObject *target =
        GetBattleObjectSlotFar(effect->actors[k % effect->count])->object;

    for (i = 0; i != 16; i++) {
      struct EffectStep *shot = &PARTICLES[k * 16 + i];
      s32 aimZ;

      shot->x = source->x;
      shot->y = height;
      shot->z = source->z;
      aimZ = target->z + (((Random16() & 31) - 16) << 17);
      shot->velocityX = (target->x - source->x) >> 4;
      shot->velocityY = 0x40000;
      shot->velocityZ = (aimZ - source->z) >> 4;
      shot->variant = 0;
    }
  }
  for (i = 0; i != 256; i++)
    BURSTS[i].variant = -1;

  // The blitter pair for the caster's side; modes 4 and 7 clear the canvas
  // each frame and turn blending off, the others fade it to three quarters.
  BattleFx_FetchRectangleBlitters(effect->side, draw);
  if (mode == 4 || mode == 7) {
    work->transferMode = 1;
    work->transferValue = 0;
    REG_BLDCNT = 0;
  } else {
    work->transferMode = 2;
    work->transferValue = 75;
  }
  Scheduler_AddOrUpdateCallback(BattleFx_ProcessPendingGraphicsTransfer, 0xc80);
  total = Data_08197880[mode * 4 + RECORD_FRAMES] + effect->count * 8;
  Audio_PlayCue(103);

  for (frame = 0; frame != total; frame++) {
    struct EffectStep *burst;

    Render_ResetTransformState();
    Transform_SetLookAt(camera, camera + 3);

    // The shots: each starts four frames after the last, flies under
    // gravity and turns through ten pictures; at the ground it bursts.
    for (k = 0; k != effect->count; k++) {
      for (i = 0; i != Data_08197880[mode * 4 + RECORD_SHOTS]; i++) {
        s32 start = k * 8 + i * 4;

        if (frame >= start) {
          struct EffectStep *shot = &PARTICLES[k * 16 + i];

          if (frame == start + 17) {
            Func_0814cd48(effect->actors[k % effect->count], 7, 5, k, 16);
            Func_081180e8(133);
          }
          if (shot->variant >= 0) {
            s32 step = (frame - start) / 3;

            if (step > 9)
              step = 9;
            EffectPosition_ProjectPoint((s32 *)shot, &position);
            position.x >>= 1;
            position.y -= 8;
            if (step > 4)
              draw[Data_08197880[mode * 4 + RECORD_SHOT_BLITTER]](
                  canvas, work->sheet + step * 768, position.x - 16,
                  position.y - 20, 32, 24);
            else
              draw[Data_08197880[mode * 4 + RECORD_SHOT_BLITTER]](
                  canvas, work->sheet + step * 768, position.x - 12,
                  position.y - 24, 24, 32);
            if (shot->variant == 0)
              BattleFxKernels_IntegrateVector3(shot, 63, -0x8000);
            if (shot->y < 0) {
              s32 count;
              s32 n;

              shot->y = 0;
              shot->variant = 1;
              if (Data_08197880[mode * 4 + RECORD_SHARDS] == 0)
                count = 4;
              else
                count = 16;
              for (n = 0; n != count; n++) {
                burst = &BURSTS[(k * 4 + i) * 8 + n];
                burst->x = shot->x;
                burst->y = shot->y;
                burst->z = shot->z;
                if (Data_08197880[mode * 4 + RECORD_SHARDS] == 0) {
                  burst->velocityX = ((Random16() & 63) - 32) << 11;
                  burst->velocityY = 0;
                  burst->velocityZ = ((Random16() & 63) - 32) << 11;
                } else {
                  burst->velocityX = ((Random16() & 63) - 32) << 13;
                  burst->velocityY = ((Random16() & 31) + 32) << 12;
                  burst->velocityZ = ((Random16() & 63) - 32) << 13;
                }
                burst->variant = 0;
              }
            }
          }
        }
      }
    }

    // The bursts: smoke rises slowly and spreads; shards fly up, fall and
    // tumble, facing the way they move. Each lives 45 frames.
    for (i = 0, burst = BURSTS; i != 256; burst++, i++) {
      if ((u32)burst->variant <= 44 && burst->y >= 0) {
        EffectPosition_ProjectPoint((s32 *)burst, &position);
        position.x >>= 1;
        if (Data_08197880[mode * 4 + RECORD_SHARDS] == 0) {
          draw[1](canvas, work->sheet + burst->variant / 8 * 1152 + SMOKE_SHEET,
                  position.x - 12, position.y - 24, 24, 48);
        } else {
          s32 picture = burst->variant / 5;
          s32 flip;

          if (i & 1)
            picture += 9;
          flip = effect->side;
          if (burst->velocityX > 0)
            flip ^= 1;
          draw[flip](canvas, work->sheet + Data_081978cc[picture] + SHARD_SHEET,
                     position.x - Data_081978a8[picture] / 2,
                     position.y - Data_081978ba[picture] / 2,
                     Data_081978a8[picture], Data_081978ba[picture]);
        }
        if (Data_08197880[mode * 4 + RECORD_SHARDS] == 0)
          BattleFxKernels_IntegrateVector3(burst, 62, 0x800);
        else
          BattleFxKernels_IntegrateVector3(burst, 62, -0x8000);
        burst->variant++;
      }
    }
    Func_08158ce0(4, 4);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(BattleFx_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(188);
  Runtime_ReleaseHeapBlock(104);

  // Modes 3 and 5 clear both canvases and go on into a summon canvas effect.
  if (mode == 3 || mode == 5) {
    Iwram_ClearWords((void *)0x06004000, 0x3c00);
    Iwram_ClearWords(canvas, 0x3c00);
    Scheduler_RemoveCallback(Palette_StepFadeTransfer);
    if (mode == 3)
      SummonFx_RunCanvasMode1(effect);
    else
      SummonFx_RunCanvasMode3(effect);
  } else {
    BattleFx_EndCanvasLayer();
  }
}

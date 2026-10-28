// Draft: listing 1123 instructions, draft 1136, differ 855. ☀️ twin:
// BattleEffect_RunDragonCloud (effect.c, structure 0.725): the puff arc,
// the faded copies of the pillar sheet, the head flying the path and its trail,
// the camera's turn and the embers are ☀️'s. ⚓️ moved the embers to start at the
// target, sends the head to the target before it dives, and added a second
// variant with orange colours and three showers of sparks. The native stack frame
// (248) holds the trail at sp+88, the target's screen position at 76, the
// head's at 64, the blitter pair at 56, effect 52, canvas 48, frame 44, drift
// 40, the path 36, x 32, y 28, graphics 24, the blitter pair's address 20, the
// camera 16 and frame - 18 at 12; work stays in r11. Written from the listing
// on the ☀️ body (2026-10-15, fx-n3). Every statement is in the native's order,
// but work is spilled to sp+48 where the native keeps it in r11 all through,
// and the reloads (13 more instructions) and the shifted slots account for most
// rows. Tried, no gain: work declared first (855); canvas loaded first (853);
// ☀️'s heapCache and cursor idiom (865). Next to try: what takes r11 and r10 in
// this draft's greg dump (alchemy diff --rtl), and the trail loop's addressing
// (the native walks j * 8 and j * 8 + 4 from sp+88).
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_display.h"
#include "object_runtime.h"

// The Lost Age's heap slots, by byte offset into Ram_HeapSlots.
#define HEAP_SLOT_BLITTER 104
#define HEAP_SLOT_BLITTER_ALTERNATE 188

// The transfer callback runs at this order, where ☀️'s runs at 0x480.
#define TASK_PASS_FRAME 0xc80

#define HI(v) (((s16 *)&(v))[1])

// The sparks live in the map cell buffer's second 16 KB: 128 at the target,
// 128 thrown up at the start and 128 shed by the head.
#define SPARKS ((struct EffectStep *)(Ram_MapCellBuffer + 0x4000))
#define SPARK_COUNT 512

// The pillar sheet's cell is 24 x 39; the faded copies follow it.
#define PILLAR_CELL_SIZE 936

struct Point2D {
  s32 x;
  s32 y;
};

extern char ResourceId_FirePillarSheetB;
extern char ResourceId_Data134;
extern char ResourceId_EmberStreakSheet;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_OrangePaletteB;
extern char ResourceId_DualStreamPath;

// The puff arc's six cells, cut from the ember streak sheet.
extern u8 gBattleEffect_PuffArcCellWidths[];
extern u8 gBattleEffect_PuffArcCellHeights[];
extern u8 gBattleEffect_PuffArcCellBiasY[];
extern u16 gBattleEffect_PuffArcCells[];

// The spark cells: offsets into the particle sheet by size.
extern u16 gData_08197410[];

// How many of the 56 embers are sparks, by variant (48 and 40); the rest
// are flashes.
extern u8 Data_08199d5c[];

struct BattleObj *GetBattleObjectSlotFar(s32 objectId);
void BattleEffect_StartCanvasLayer(s32 bgControl);
void *Resource_GetTableEntry(s32 id);
void BattleEffect_FetchRectangleBlitters(s32 side, DrawRectangle *draw);
s32 Task_Add(void (*callback)(void), s32 order);
s32 Task_Remove(void (*callback)(void));
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void Func_081180e8(s32 phase);
void Sound_PlayFar(s32 cue);
void BattlePres_SetupTransitionSceneFar(s32 a, s32 b, s32 c, s32 d);
void EffectPosition_ProjectBattler(
    s32 id, struct EffectPosition *position);
void Render_ResetTransformState(void);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);
void Func_08118088(s32 actor, s32 motion);
void Func_08158ce0(s32 randomMask, u32 shakeRange);
void ObjectGroup_CountDownMemberTimers(void);
void WaitFrames(s32 frames);
void Runtime_ReleaseHeapBlock(s32 slot);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);

// Dragon Cloud (effect kind 49): a puff arc rises over the user, then a
// dragon's head flies the path from frame 18, trailing fading copies of
// itself, while the camera turns; from frame 78 it dives onto the target.
// The puffs burst at frame 50 and the target flashes; at frame 83 the head
// strikes, the target reels, and 56 embers fly from it: sparks, then flashes.
// Variant 1, ⚓️'s own, loads orange colours, throws 128 sparks up from the
// user's side at the start, sheds four sparks a frame from the head and
// throws 128 more from the target with the embers.
void BattleEffect_RunDragonCloud(struct BattleEffectArgument *effect) {
  struct Point2D trail[20];
  struct EffectPosition target;
  struct EffectPosition position;
  DrawRectangle draw[2];
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 drift;
  s8 *path;
  s32 x;
  s32 y;
  u8 *graphics;
  struct BattleCamera *camera;
  struct ObjectRuntime *object;
  struct EffectStep *step;
  s32 i;
  s32 j;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  drift = 0;
  x = 0;
  y = 0;
  graphics = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(0);
  REG_BLDALPHA = 0x1010;

  // The pillar sheet, and nine copies of its cell, each darker than the last,
  // for the trail.
  Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetB, work->sheet, 1,
                             1);
  for (i = 1; i != 20; i++) {
    for (j = 0; j != PILLAR_CELL_SIZE; j++) {
      s32 shade = work->sheet[0x1680 + j];

      if (i > 10) {
        shade = shade - i * 4 + 40;
        if (shade < 0)
          shade = 0;
        work->sheet[0x1680 + (i - 10) * PILLAR_CELL_SIZE + j] = shade;
      }
    }
  }
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, graphics, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_EmberStreakSheet,
                             &work->sheet[0x3c00], 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             Ram_MapCellBuffer, 1, 0);
  if (effect->variant == 1)
    Iwram_CopyWords((void *)0x05000000,
                    Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB),
                    128);
  BattleEffect_FetchRectangleBlitters(effect->side, draw);
  work->transferMode = 2;
  work->transferValue = 75;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  // The 56 embers wait at the target: sparks first, then flashes, which rise
  // more slowly.
  object = GetBattleObjectSlotFar(effect->actors[0])->object;
  for (i = 0; i != 56; i++) {
    step = &work->particles[8 + i];
    step->x = object->x;
    step->y = 0;
    step->z = object->z;
    if (i < Data_08199d5c[effect->variant]) {
      step->velocityX = ((Random16() & 63) - 32) << 14;
      step->velocityY = (Random16() & 63) << 13;
    } else {
      step->velocityX = ((Random16() & 63) - 32) << 14;
      step->velocityY = (Random16() & 63) << 12;
    }
    step->velocityZ = ((Random16() & 63) - 32) << 14;
    step->variant = 1;
  }

  // The sparks: all asleep but the first 128, at the target (only variant 1
  // draws them).
  for (i = 0; i != SPARK_COUNT; i++)
    SPARKS[i].variant = -1;
  for (i = 0; i != 128; i++) {
    step = &SPARKS[i];
    step->x = object->x;
    step->y = 0;
    step->z = object->z;
    step->velocityX = ((Random16() & 63) - 32) << 14;
    step->velocityY = (Random16() & 63) << 12;
    step->velocityZ = ((Random16() & 63) - 32) << 14;
    step->variant = (Random16() & 15) + 24;
  }

  // The eight puffs, on an arc over the user, each two frames after the last.
  for (i = 0, j = -0x4000; i != 8; i++, j += 0x1000) {
    step = &work->particles[i];
    if (effect->side == 1)
      step->x = (Trig_Sin(j) * 24 >> 16) + 88;
    else
      step->x = (-(Trig_Sin(j) * 24) >> 16) + 16;
    step->y = (Trig_Cos(j) * 16 >> 16) + 40;
    step->variant = -(i * 2);
  }
  path = Resource_GetTableEntry((s32)&ResourceId_DualStreamPath);

  for (frame = 0; frame != 150; frame++) {
    camera = Ram_HeapSlots->cameraWork;
    if (frame == 83) {
      if (effect->variant == 0)
        Func_081180e8(134);
      else
        Func_081180e8(145);
    }
    if (frame == 0)
      Sound_PlayFar(136);
    if (frame == 50)
      Sound_PlayFar(136);

    // For 64 frames the camera turns away from the user's side.
    if (effect->side == 0) {
      if (frame <= 63)
        camera->yaw -= 0x100;
    } else {
      if (frame <= 63)
        camera->yaw += 0x100;
    }
    BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);

    // Frames 0-17: the puff arc rises, one cell every three frames.
    if (frame <= 17) {
      s32 cell = frame / 3;

      draw[0](canvas,
              &work->sheet[0x3c00] + gBattleEffect_PuffArcCells[cell],
              56 - gBattleEffect_PuffArcCellWidths[cell] / 2,
              gBattleEffect_PuffArcCellBiasY[cell] + 60,
              gBattleEffect_PuffArcCellWidths[cell],
              gBattleEffect_PuffArcCellHeights[cell]);
      draw[1](canvas,
              &work->sheet[0x3c00] + gBattleEffect_PuffArcCells[cell],
              64 - gBattleEffect_PuffArcCellWidths[cell] / 2,
              gBattleEffect_PuffArcCellBiasY[cell] + 60,
              gBattleEffect_PuffArcCellWidths[cell],
              gBattleEffect_PuffArcCellHeights[cell]);
    }

    // Variant 1: 128 sparks thrown up from the user's side at the start.
    if (effect->variant == 1) {
      if (frame == 0) {
        for (i = 0; i != 128; i++) {
          s32 angle;
          s32 force;

          step = &SPARKS[128 + i];
          step->x = 0x3c0000;
          step->y = 0x6c0000;
          angle = (Random16() & 0x7fff) + 0x4000;
          force = (Random16() & 255) + 127;
          step->velocityX = (Trig_Sin(angle) * force) >> 7;
          step->velocityY = (Trig_Cos(angle) * force) >> 5;
          step->variant = (Random16() & 15) + 24;
        }
      }
      for (i = 0; i != 64; i++) {
        step = &SPARKS[128 + i];
        if (step->variant >= 0 && frame >= i / 8) {
          s32 size = (step->variant >> 4) + 1;

          draw[i & 1](canvas, graphics + gData_08197410[size - 1],
                      HI(step->x) - size / 2, HI(step->y) - size, size,
                      size * 2);
          BattleEffectKernels_IntegrateVector2(step, 62, 0x4000);
          if (step->y > ((i & 3) + 108) << 16)
            step->velocityY = -step->velocityY / 2;
          step->variant--;
        }
      }
    }

    // Frames 18-58: the head flies the path, a 16-bit start and then
    // steps; from frame 78 it dives from above the target.
    if (frame >= 18 && frame <= 58) {
      if (frame == 18) {
        x = (path[0] << 8) + (u8)path[1];
        y = (path[2] << 8) + (u8)path[3] + 16;
        path += 4;
      } else {
        x += path[0];
        y += path[1];
        path += 2;
      }
    }
    if (frame >= 78 && frame <= 118) {
      EffectPosition_ProjectBattler(effect->actors[0], &target);
      if (frame == 78) {
        x = target.x / 2;
        y = 48;
      } else {
        y -= 16;
      }
    }

    // The trail: each copy takes the place of the one before, the older
    // ones darker.
    for (j = 19; j != 0; j--) {
      if (frame > j + 18 && frame <= j + 83) {
        trail[j].x = trail[j - 1].x;
        trail[j].y = trail[j - 1].y;
        if (j > 10)
          draw[0](canvas, &work->sheet[0x1680 + (j - 10) * PILLAR_CELL_SIZE],
                  trail[j].x - 12, trail[j].y, 24, 39);
        else
          draw[0](canvas, &work->sheet[0x1680], trail[j].x - 12, trail[j].y, 24,
                  39);
      }
    }
    Render_ResetTransformState();
    Transform_SetLookAt(camera->eye, camera->pos);

    // Frames 18-83: the head, tilted by how fast it climbs or falls.
    if (frame >= 18 && frame <= 83) {
      s32 lift;

      if (frame >= 78 && frame <= 118)
        position.x = x;
      else if (effect->side == 1)
        position.x = 64 - x / 2;
      else
        position.x = x / 2 + 64;
      position.y = 60 - y;
      lift = (position.y - trail[0].y - 24) / 2;
      if (lift > 2)
        lift = 2;
      if (lift < -2)
        lift = -2;
      drift += lift;
      if (drift > 8)
        drift = 8;
      if (drift < -8)
        drift = -8;
      lift = drift / 4 + 2;
      trail[0].x = position.x;
      trail[0].y = position.y - 20;
      draw[0](canvas, &work->sheet[lift * 0x480], trail[0].x - 12,
              trail[0].y - 2, 24, 48);

      // The head sheds four sparks a frame (only variant 1 draws them).
      for (i = 0; i != 4; i++) {
        s32 angle;
        s32 force;

        step = &SPARKS[((frame * 4 + i) & 127) + 256];
        step->x = position.x << 16;
        step->y = position.y << 16;
        angle = Random16();
        force = (Random16() & 255) + 127;
        step->velocityX = (Trig_Sin(angle) * force) >> 7;
        step->velocityY = (Trig_Cos(angle) * force) >> 5;
        step->variant = (Random16() & 15) + 24;
      }
    }

    // After the strike: the embers fly from the target, sized by depth;
    // the flashes play the flash sheet.
    if (frame > 82) {
      for (i = 0; i != 56; i++) {
        step = &work->particles[8 + i];
        if (step->y >= 0) {
          s32 size;

          EffectPosition_ProjectPoint(&step->x, &position);
          position.x >>= 1;
          if (position.depth <= 159)
            position.depth = 160;
          if (position.depth > 799)
            position.depth = 799;
          size = 9 - (position.depth - 160) / 64;
          if (i >= Data_08199d5c[effect->variant]) {
            s32 cell;

            if (effect->variant == 0)
              cell = step->variant / 2;
            else
              cell = step->variant / 4;
            if (cell <= 5) {
              draw[effect->variant](canvas, Ram_MapCellBuffer + (cell << 11),
                                    position.x - 16, position.y - 40, 32, 64);
              step->variant++;
              BattleEffectKernels_IntegrateVector3(step, 62, 0x2000);
            }
          } else {
            draw[0](canvas, graphics + gData_08197410[size - 1],
                    position.x - size / 2, position.y - size, size, size * 2);
            BattleEffectKernels_IntegrateVector3(step, 64, -0x2000);
          }
        }
      }

      // Variant 1's 128 sparks at the target.
      if (effect->variant == 1) {
        for (i = 0; i != 128; i++) {
          step = &SPARKS[i];
          if (step->variant >= 0) {
            s32 size;

            EffectPosition_ProjectPoint(&step->x, &position);
            size = (step->variant >> 3) + 1;
            position.x >>= 1;
            draw[i & 1](canvas, graphics + gData_08197410[size - 1],
                        position.x - size / 2, position.y - size, size,
                        size * 2);
            BattleEffectKernels_IntegrateVector3(step, 62, 0x2000);
            step->variant--;
          }
        }
      }
    }

    // Variant 1: the sparks the head shed.
    if (effect->variant == 1) {
      for (i = 0; i != 128; i++) {
        step = &SPARKS[256 + i];
        if (step->variant >= 0) {
          s32 size = (step->variant >> 4) + 1;

          draw[i & 1](canvas, graphics + gData_08197410[size - 1],
                      HI(step->x) - size / 2, HI(step->y) - size, size,
                      size * 2);
          BattleEffectKernels_IntegrateVector2(step, 60, -0x4000);
          step->variant--;
        }
      }
    }

    // Frame 83: the strike. Frame 50: the puffs burst.
    if (frame == 83) {
      work->shakeFrames = 8;
      Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
      Func_08118088(effect->actors[0], 1);
    }
    if (frame == 50) {
      work->shakeFrames = 12;
      Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
    }
    if (frame > 49) {
      for (i = 0; i != 8; i++) {
        step = &work->particles[i];
        if (step->variant >= 0 && step->variant <= 11) {
          s32 cell = step->variant / 2;

          draw[1](canvas,
                  &work->sheet[0x3c00] +
                      gBattleEffect_PuffArcCells[cell],
                  step->x - gBattleEffect_PuffArcCellWidths[cell] / 2,
                  step->y + gBattleEffect_PuffArcCellBiasY[cell],
                  gBattleEffect_PuffArcCellWidths[cell],
                  gBattleEffect_PuffArcCellHeights[cell]);
        }
        step->variant++;
      }
    }
    Func_08158ce0(8, 8);
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

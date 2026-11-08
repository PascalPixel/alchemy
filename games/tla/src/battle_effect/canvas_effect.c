/// @file
/// The dagger and shield canvas effect, behind the four wrappers of
/// summon_canvas_modes.c; the Broken Seal's twin is
/// BattleEffect_RunStatReduction, which has no mode 3.
#include "types.h"
#include "battle_display.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "battle_action.h"
#include "task.h"
#include "effect_step.h"
#include "fixed_math.h"
#include "graphics.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "heap_release.h"
#include "motobj.h"
#include "actor.h"
#include "ram_buffer.h"
#include "resource.h"
#include "system.h"
#include "transform.h"

// The canvas layer and its helpers; the comments give The Broken Seal's
// names where the twin is known.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
void BattleEffect_FetchRectangleBlitters(s32 side, DrawRectangle *pair);
void Func_081180e8(s32 phase); // BattlePlayback_StartFar
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);

#define SHARDS ((struct EffectStep *)Ram_MapCellBuffer)
#define RAMP_IMAGE 0x2580 // the shard picture, then seven darker copies
#define RAMP_SIZE 696     // one copy

extern char ResourceId_DaggerSheet;
extern char ResourceId_ShieldSheet;
extern char ResourceId_PinkBurstSheet;
extern char ResourceId_GlowOrbSheet;
extern char ResourceId_PaletteRampImage;
extern u16 Data_081974dc[]; // gStatChange_ShardCells
extern u8 Data_081974f4[];  // gStatChange_ShardWidths
extern u8 Data_08197500[];  // gStatChange_ShardHeights

/// A dagger (modes 0 and 3) or a shield (1 and 2) appears over each target
/// in turn, four frames apart, and breaks into shards that fly off and
/// darken.
///
/// Setup: seven darker copies of the shard picture are built by clamping
/// its levels; 24 shards a target start in a grid across the target and
/// scatter with random headings toward the user's side. Frame loop: a
/// target's picture plays for 24 frames; at frame 24 a chime sounds and the
/// shards fly for 36 frames, one shade darker every four frames.
void SummonEffect_RunCanvasEffect(struct BattleEffectArgument *effect,
                                  s32 mode) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 frame;
  s32 j;
  s32 xOffset;
  DrawRectangle *draw;
  struct BattleCamera *camera;
  s32 origin[3];
  struct EffectPosition screen;
  s32 point[3];
  DrawRectangle callbacks[2];
  s32 i;
  s32 palette;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleEffect_StartCanvasLayer(0);
  REG_BG2PA = 0x100;
  if (mode == 0 || mode == 3)
    Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet, work->sheet, 1, 1);
  else
    Resource_LoadAndDecompress((s32)&ResourceId_ShieldSheet, work->sheet, 1, 1);
  if (mode == 0)
    palette = (s32)&ResourceId_PinkBurstSheet;
  else if (mode == 3) {
    palette = (s32)&ResourceId_GlowOrbSheet;
    /* FAKEMATCH(barrier): the empty asm keeps the mode-3 branch apart from the
     * identical mode-1 branch; as plain C, 8 of 553 instructions differ in
     * tla-en. */
    asm("");
  } else if (mode == 1)
    palette = (s32)&ResourceId_GlowOrbSheet;
  else
    palette = (s32)&ResourceId_PinkBurstSheet;
  Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(palette), 128);
  Resource_LoadAndDecompress((s32)&ResourceId_PaletteRampImage,
                             work->sheet + RAMP_IMAGE, 0, 0);

  // Copy i holds every level clamped to 64 - 7i.
  for (i = 1; i != 8; i++) {
    for (j = 0; j != RAMP_SIZE; j++) {
      s32 level = work->sheet[RAMP_IMAGE + j];

      if (level > 64 - i * 7)
        level = 64 - i * 7;
      if (level < 0)
        level = 0;
      work->sheet[RAMP_IMAGE + i * RAMP_SIZE + j] = level;
    }
  }

  if (effect->side == 1) {
    REG_BG2X = -0x7000;
    xOffset = -112;
  } else {
    REG_BG2X = 0;
    xOffset = 0;
  }

  // The shards: rows six pixels apart, columns two (dagger) or eight
  // (shield) pixels deep, thrown toward the user's side.
  for (i = 0; i != 512; i++) {
    struct EffectStep *shard = &SHARDS[i];
    s32 radius = 192;
    s32 angle;

    angle = Random16() & 0xffff;
    shard->x = 0;
    if (mode == 0 || mode == 3) {
      shard->y = ((i & 31) / 4 * 6 - 10) << 16;
      shard->z = (i % 4 * 2 - 2) << 16;
    } else {
      shard->y = ((i & 31) / 4 * 6 - 10) << 16;
      shard->z = (i % 4 * 8 - 16) << 16;
    }
    if (effect->side == 1)
      shard->velocityX = Q16_ONE * 2;
    else
      shard->velocityX = -Q16_ONE * 2;
    shard->velocityY = ((Trig_Cos(angle) * radius) >> 6) + Q16_ONE;
    shard->velocityZ = (Trig_Sin(angle) * radius) >> 6;
    shard->variant = Random16() & 0xff;
  }

  BattleEffect_FetchRectangleBlitters(effect->side, draw = callbacks);
  work->transferMode = 2;
  work->transferValue = 50;
  Task_Add(BattleEffect_ProcessPendingGraphicsTransfer, TASK_PASS_FRAME);

  for (frame = 0; frame != effect->count * 4 + 64; frame++) {
    camera = Ram_HeapSlots->cameraWork;
    if (frame == 72)
      Func_081180e8(0);
    for (j = 0; j != effect->count; j++) {
      struct MotionObject *object;
      s32 time;
      s32 x;
      s32 y;

      time = frame - j * 4;
      object = GetBattleObjectSlotFar(effect->actors[j])->object;
      if (time > 0) {
        // The picture stands 40 pixels above the target.
        Render_ResetTransformState();
        Transform_SetLookAt(camera->eye, camera->pos);
        point[0] = object->x;
        point[1] = 160 << 13;
        point[2] = object->z;
        Render_ResetTransformState();
        Transform_SetLookAt(camera->eye, camera->pos);
        SceneTransform_ApplyPosition(point);
        origin[0] = 0;
        origin[1] = 0;
        origin[2] = 0;
        EffectPosition_ProjectPoint(origin, &screen);
        x = screen.x + xOffset;
        y = screen.y;
        if (mode == 0 || mode == 3) {
          if (time <= 26)
            callbacks[0](canvas, work->sheet + time / 4 % 7 * 960, x - 12,
                         y - 20, 24, 40);
        } else {
          if (time <= 23)
            draw[1](canvas, work->sheet + time / 4 % 6 * 1600, x - 20, y - 20,
                    40, 40);
        }
        // frame - j * 4 is time; this spelling matches here and below.
        if (frame - j * 4 == 24)
          Sound_PlayFar(143);

        // The shards fly for 36 frames, a shade darker every four.
        if (time >= 24 && time <= 60) {
          s32 level;

          level = 0;
          if (frame - j * 4 > 28) {
            level = (time - 24) / 4;
            if (level > 7)
              level = 7;
          }
          for (i = 0; i != 24; i++) {
            struct EffectStep *shard = &SHARDS[j * 32 + i];
            s32 cell;

            cell = i % 4 * 3 + (shard->variant + time) / 8 % 3;
            EffectPosition_ProjectPoint((s32 *)shard, &screen);
            x = screen.x + xOffset;
            y = screen.y;
            callbacks[0](canvas,
                         work->sheet +
                             (level * RAMP_SIZE + Data_081974dc[cell]) +
                             RAMP_IMAGE,
                         x, y, Data_081974f4[cell], Data_08197500[cell]);
            BattleEffectKernels_IntegrateVector3(shard, 60, 0);
          }
        }
      }
    }
    work->transferPending = 1;
    wait(1);
  }

  Task_Remove(BattleEffect_ProcessPendingGraphicsTransfer);
  HeapSlot_Release(188);
  HeapSlot_Release(104);
  BattleEffect_EndCanvasLayer();
}

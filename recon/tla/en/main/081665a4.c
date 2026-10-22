// Draft, differ 44 of 620: BattleEffect_RunRagnarok, effect kind 58, The Broken Seal's
// BattleEffect_RunRagnarok.

// Twin: ☀️ BattleEffect_RunRagnarok (games/tbs/src/effect/effect.c), ported:
// the strike columns are placed from the first target's position (half its x
// less 24, or less 92 on side 1), the user's mode is set to 2 (Object_SetMode,
// where ☀️ applies argument 2 to the children), the effect stays an argument
// (☀️ also forces variant 1), each target is also flashed when its column
// starts, callback BattleEffect_ProcessPendingGraphicsTransfer at 0xc80.

// Left: everything is reordered rows where the native puts a load (mostly the
// work's reload from sp+52, the pooled tables) ahead of an independent
// constant or shift: the kind Func_08164b2c's header traces to a load costing
// 3 and a store 2.
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "system.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "motobj.h"
#include "battle_session.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The pixel half of a 16.16 fixed-point value.
#define HI(v) (((s16 *)&(v))[1])

// The heap blocks the dispatcher hands every canvas effect, as byte offsets
// into Ram_HeapSlots (☀️ uses slot indices).
#define HEAP_BLITTER 104
#define HEAP_BLITTER_ALTERNATE 188

// The sheet holds the blade (64 x 128) and then the flash pictures.
#define FLASH_SHEET (64 * 128)
#define FLASH_PICTURE_BYTES (32 * 64)

// The map cell buffer holds each column's sparks.
#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer)
#define SPARKS_PER_COLUMN 340
#define SPARKS_DRAWN 256
#define FLASHES_PER_COLUMN 16
#define FLASHES_DRAWN 12

#define BLADE_FRAMES 80
#define BLADE_SWING_FRAMES 10 // the blade comes down
#define BLADE_HOLD_FRAME 20   // then holds, then creeps on
#define BLADE_LAST_FRAME 39
#define FIRST_STRIKE_FRAME 16 // each column strikes 8 frames after the last
#define RESULTS_FRAME 40

#define PHASE_RESULTS 134
#define SE_SWING 212
#define SE_STRIKE 145

extern char ResourceId_FlameBladeSheet;
extern char ResourceId_FlashBurstSheet;
extern char ResourceId_Data134; // ☀️ ParticleSpritesA
extern char ResourceId_BlastSheet;
extern u8 gData_08198a92[];  // ☀️ gRagnarok_StrikeColumns
extern u8 gData_08198a98[];  // ☀️ gPlasma_FlashCells
extern u16 gData_08197410[]; // ☀️ gBattleEffect_MoteCells

void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_ProcessPendingGraphicsTransfer(void); // BattleEffect_ProcessPendingGraphicsTransfer
void BattleEffect_FetchRectangleBlitters(
    s32 side,
    DrawRectangle *draw); // BattleEffect_FetchRectangleBlitters
void Object_SetMode(void *object, s32 mode);
void ObjectDispatch_SetAnimationSpeedFar(void *object, s32 value);
void EffectPosition_ProjectBattler(
    s32 id, struct EffectPosition *position);
void Func_081180e8(s32 phase);      // BattleEventRuntime_StartPhaseFar
void Func_0815f0a0(s32 resourceId); // BattleEffect_StepPaletteToResource
void Audio_PlayCue(s32 cue);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Func_0814cd48(s32 actor, s32 palette, s32 a, s32 timer,
                   s32 frames); // ObjectGroup_UpdateMembers
void Func_08118088(s32 actor,
                   s32 variant);  // BattleMotion_ApplyVariantMotionFar
void Func_08158ce0(s32 x, s32 y); // Camera_ApplyShake
void ObjectGroup_TickMemberTimers(void);         // ObjectGroup_TickMemberTimers
void BattleEffect_EndCanvasLayer(void);         // BattleEffect_EndCanvasLayer

// A blade of fire swings down over the targets and strikes twice, each
// strike a flash and a fountain of sparks that bounce on the ground.
//
// Setup: open the canvas on BG1, pose the user, place the strike columns
// under the first target, load the blade, flash and spark pictures, and
// prepare each column's 16 flashes (a ring) and 340 sparks. Each frame: the
// blade comes down over 10 frames, holds until frame 20, then creeps on and is
// gone after frame 39; from frame 16 the palette steps toward the blast
// colours; each column strikes 8 frames after the last (shake, its flash, its
// targets flashed) and 6 frames later throws its sparks and knocks the
// targets back.
void BattleEffect_RunRagnarok(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 col;
  u8 *cells;
  struct MotionObject *user;
  DrawRectangle draw[2];
  struct EffectPosition target;
  s32 baseX;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  cells = Ram_HeapSlots->unk64[0];

  // Open the canvas, pose the user and place the columns.
  user = GetBattleObjectSlotFar(effect->actor)->object;
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x1010;
  BattleEffect_FetchRectangleBlitters(effect->side, draw);
  Object_SetMode(user, 2);
  ObjectDispatch_SetAnimationSpeedFar(user, 48);
  EffectPosition_ProjectBattler(effect->actors[0], &target);
  if (effect->side == 0)
    baseX = target.x / 2 - 24;
  else
    baseX = target.x / 2 - 92;

  Resource_LoadAndDecompress((s32)&ResourceId_FlameBladeSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet,
                             work->sheet + FLASH_SHEET, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, cells, 0, 0);

  // Each column's flashes (a ring, 2 pixels apart, living 25 to 32 frames)
  // and sparks (from y = 88, at random speeds and angles, living 32 to 39).
  col = 0;
  do {
    i = 0;
    do {
      struct EffectStep *flash = &work->particles[col * FLASHES_PER_COLUMN + i];
      s32 radius = i * 2;
      s32 angle = Random16() & 0xffff;

      flash->x = Trig_Sin(angle) * radius;
      flash->y = -(Trig_Cos(angle) * radius);
      flash->variant = i / 2 + 25;
      i++;
    } while (i != FLASHES_PER_COLUMN);

    i = 0;
    do {
      struct EffectStep *spark = &SPARKS[col * SPARKS_PER_COLUMN + i];
      s32 speed;
      s32 angle;

      speed = (Random16() & 0x1ff) + 32;
      angle = Random16() & 0xffff;
      spark->x = (gData_08198a92[col + effect->side * 3] + baseX) << 16;
      spark->y = 88 << 16;
      spark->velocityX = (Trig_Sin(angle) * speed) >> 6;
      spark->velocityY = -(Trig_Cos(angle) * speed * 2) >> 6;
      spark->variant = (Random16() & 7) + 32;
      i++;
    } while (i != SPARKS_PER_COLUMN);
    col++;
  } while (col != 3);

  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);

  frame = 0;
  do {
    if (frame == 4)
      Audio_PlayCue(SE_SWING);
    if (frame == 8)
      work->shakeFrames = frame;
    if (frame == 18)
      Audio_PlayCue(SE_STRIKE);
    if (frame == RESULTS_FRAME)
      Func_081180e8(PHASE_RESULTS);

    // The blade: down 16 pixels a frame, held, then creeping on; nothing
    // below y = 104 is drawn.
    if (frame <= BLADE_LAST_FRAME) {
      s32 height;
      s32 x;
      s32 top;

      height = 128;
      if (effect->side == 1) {
        if (frame < BLADE_SWING_FRAMES) {
          x = frame * 10 - 8;
          top = frame * 16 - 128;
        } else if (frame > BLADE_HOLD_FRAME) {
          x = frame + 62;
          top = frame * 2 - 24;
        } else {
          x = 82;
          top = 16;
        }
      } else {
        if (frame < BLADE_SWING_FRAMES) {
          x = 128 - frame * 10;
          top = frame * 16 - 128;
        } else if (frame > BLADE_HOLD_FRAME) {
          x = 58 - frame;
          top = frame * 2 - 24;
        } else {
          x = 38;
          top = 16;
        }
      }
      if (top + 128 > 104)
        height = height - top - 24;
      if (height > 0)
        draw[0](canvas, work->sheet, x + baseX - 32, top, 64, height);
    }

    if (frame > FIRST_STRIKE_FRAME)
      Func_0815f0a0((s32)&ResourceId_BlastSheet);

    col = 0;
    do {
      s32 start = col * 8 + FIRST_STRIKE_FRAME;

      // The strike: a shake, two frames of the big flash, then the ring.
      if (frame == start)
        work->shakeFrames = 12;
      if (frame >= start) {
        if (frame < start + 2)
          draw[0](canvas, work->sheet + FLASH_SHEET,
                  gData_08198a92[col + effect->side * 3] + baseX - 16, 56, 32,
                  64);
        i = 0;
        do {
          struct EffectStep *flash =
              &work->particles[col * FLASHES_PER_COLUMN + i];
          s32 x;
          s32 life;
          s32 y;

          y = HI(flash->y);
          x = HI(flash->x) + gData_08198a92[col + effect->side * 3] + baseX;
          life = flash->variant;
          if (life >= 0 && life <= 17)
            draw[0](canvas,
                    work->sheet + (gData_08198a98[life / 3] << 11) +
                        FLASH_SHEET,
                    x - 16, y + 56, 32, 64);
          life = flash->variant;
          flash->variant = life > 0 ? life - 1 : -1;
          i++;
        } while (i != FLASHES_DRAWN);
      }

      // The sparks, from six frames after the strike, bouncing at y = 108.
      if (frame > start + 5) {
        i = 0;
        do {
          struct EffectStep *spark = &SPARKS[col * SPARKS_PER_COLUMN + i];

          if (spark->variant > 0) {
            s32 x;
            s32 y;
            s32 size;

            BattleEffectKernels_IntegrateVector2(spark, 64, 0x1000);
            spark->variant--;
            y = spark->y;
            if (y > (108 << 16)) {
              spark->velocityY = -spark->velocityY / 2;
            } else {
              x = spark->x;
              if ((x >= 0 && x <= 0x7effff) && y >= 0) {
                u8 *picture;
                s32 lane;
                s32 py = y >> 16;
                s32 px = x >> 16;

                size = spark->variant / 5 + 1;
                lane = i & 1;
                picture = cells + gData_08197410[size - 1];
                px -= size / 2;
                py -= size;
                draw[lane](canvas, picture, px, py, size, size * 2);
              }
            }
          }
          i++;
        } while (i != SPARKS_DRAWN);
      }

      // The column's targets flash at the strike and are struck by its
      // sparks.
      for (i = 0; i != effect->count; i++) {
        if (frame == start)
          Func_0814cd48(effect->actors[i], 7, 5, i, 10);
        if (frame == start + 6) {
          Func_0814cd48(effect->actors[i], 7, 5, i, 10);
          Func_08118088(effect->actors[i], 4);
        }
      }
      col++;
    } while (col != 2);

    Func_08158ce0(16, 16);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
    frame++;
  } while (frame != BLADE_FRAMES);

  ObjectDispatch_SetAnimationSpeedFar(user, 16);
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_BLITTER);
  BattleEffect_EndCanvasLayer();
}

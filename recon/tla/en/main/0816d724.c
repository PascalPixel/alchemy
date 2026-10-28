// 2026-10-26 (round 5, later): differ 58. Each spark and mote loop has its own
// block-scoped pointer (the first frame's two, the spark draw loop's, the drain
// block's): 114 to 96 for the sparks, 58 with the motes; the allocation in both
// loops now matches. Left: the SameSide arms (below), the prologue's heap base
// built after the effect's spill and scheduling rows.
// Draft of BattleEffect_RunSoulShatter, effect kind 115 (Soul Shatter), The
// Broken Seal's BattleEffect_RunUndeadSword as The Lost Age reworked it
// (structure twin 0.703): the same two mirrored halves of a column and the same
// sixteen sparks, over a longer effect in which the target is thrown up
// and 32 motes of its soul drift back into the user (pulled as
// BattleEffect_RunNirvana's orb is).

// Listing 589 instructions, draft 587, differ 114 (2026-10-26, round 5).
// The spark's blitter is draw[(i & 3) ? 0 : 1] written in the call with the
// size table read in place (the native folds the choice into a byte offset,
// 4 or 0, right after the cell clamp, sharing the divide's 3): 144 to 123.
// The test that a mote has passed the user is SameSide, an inline function
// whose two returns keep the flag values the native computes (lsrs and
// mvns) instead of a jump on each sign: 123 to 114. The native also copies
// the flag test into the first arm and puts the user-left arm first (two
// instructions this draft lacks); written with the user-left arm first, as
// a conditional, an if/else on a local or explicit returns of 1 and 0, the
// tests fold back into jumps (123 to 125). Left: allocation in the spark
// and mote loops (the native keeps the spark in r5 and the mote in r6, here
// r7; declaration order and the loop forms change nothing) and the pool
// offsets.
// Before (144): the mirror choice in a local 0/1 flag scaled at the call;
// as a byte offset into the pair 165.
// Tried (differ):
// first draft                                                       261
// position declared before screen (stack slots)                     261
// the mirror choice in a local after the clamp (the 3 is then
// shared and spilled as in the native)                             204
// work read before canvas                                            198
// the column centre read into a local before the height              186
// the user's half height scoped to the drain block (its slot after
// the position pointer's)                                          176
// the choice as &draw[(i & 3) == 0] taken early                      399
// the choice as a byte offset into the pair                          165
// alchemy permute 8 minutes, 2 jobs: 170 with one variable expanded
// in place (an unclear rewrite of the printed source; not kept)
// the pull's three deltas in locals, each shifted as ☀️'s BattleEffect_RunDrain
// shifts them (the native's load order)                           144

#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "system.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "mem.h"
#include "motobj.h"
#include "effect_step.h"
#include "battle_display.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The pixel half of a 16.16 fixed-point value.
#define HI(v) (((s16 *)&(v))[1])

// The Lost Age numbers the blitter modes from the other end of The Broken
// Seal's COMPOSITE_* list.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };

#define SPARK_OFFSET 0x460
#define SPARKS 16
#define MOTES 32

extern char ResourceId_LimeColumnSheet;
extern char ResourceId_BlastSheet;
extern char ResourceId_Data134;
extern char ResourceId_OrangePaletteB;

// The Broken Seal's gEruption_PuffCells and gCybele_EmberSizes:
// the spark pictures by stage.
extern u16 Data_08198b5e[];
extern u16 Data_08198b6c[];
// The Broken Seal's gBattleEffect_MoteCells: spark cells of the
// Data134 sheet, by size - 1.
extern u16 gData_08197410[];

// The canvas layer and its helpers, with The Broken Seal's names.
void BattleEffect_StartCanvasLayer(s32 bgControl);
void BattleEffect_EndCanvasLayer(void); // BattleEffect_EndCanvasLayer
void BattleEffect_ProcessPendingGraphicsTransfer(
    void); // BattleEffect_ProcessPendingGraphicsTransfer
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode); // BattleEffect_BuildBlitter
void BattleEffect_FetchRectangleBlitters(
    s32 flip, DrawRectangle *draw); // ..FetchRectangleBlitters
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);          // ObjectGroup_UpdateMembers
void ObjectGroup_CountDownMemberTimers(void); // ObjectGroup_CountDownMemberTimers
void Func_08158ce0(s32 mask, u32 range); // Camera_ApplyShake
void Func_081180e8(s32 phase);           // BattleEventRuntime_StartPhaseFar
s32 BattleUnit_GetEffectHeightFar(s32 actor);
void Render_ResetTransformState(void);
void Transform_SetLookAt(s32 *eye, s32 *position);
void EffectPosition_ProjectBattler(s32 actor, struct EffectPosition *position);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void BattleEffectKernels_IntegrateVector3(struct EffectStep *step, s32 damping,
                                          s32 gravity);
void Sound_PlayFar(s32 cue);

// Whether b lies on the same side of x = 0 as a.
static inline s32 SameSide(s32 a, s32 b) {
  if (a >= 0)
    return b >= 0;
  return b < 0;
}

// Soul Shatter: a column bursts up under the target in a spray of sparks
// and throws it into the air; the user glows, and from frame 32 motes of
// the target's soul fly out and are drawn back into the user.
//
// Setup: canvas layer 1 at a light blend, centred on the target; the
// column, the spark pictures after it, the mote sheet and the orange
// palette.
// Frame loop (106 frames): the column rises from frame 8, 12 pixels a
// frame, stands at 80 pixels (drawn with max blitters) and sinks from
// frame 80; the target is thrown up at frame 10 and comes down at frame
// 80.
void BattleEffect_RunSoulShatter(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  u8 *sheet;
  struct MotionObject *target;
  DrawRectangle draw[2];
  struct EffectPosition position;
  struct EffectPosition screen;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sheet = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  REG_BLDALPHA = 0x0c10;
  Resource_LoadAndDecompress((s32)&ResourceId_LimeColumnSheet, work->sheet, 1,
                             1);
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet,
                             work->sheet + SPARK_OFFSET, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sheet, 0, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer,
                                0xc80);
  target = GetBattleObjectSlotFar(effect->actors[0])->object;
  EffectPosition_ProjectBattler(effect->actors[0], &position);
  position.x /= 2;
  work->shakeFrames = 8;

  for (frame = 0; frame != 106; frame++) {
    // Frame 0: sparks leave the column's foot upward, and the motes start
    // at the target, 52 pixels up, in every direction.
    if (frame == 0) {
      target->y = 0;
      {
        struct EffectStep *spark = work->particles;

        for (i = 0; i != SPARKS; i++, spark++) {
          s32 speed = (Random16() & 0xff) + 256;
          s32 angle = (Random16() & 0x7fff) + 0x4000;

          spark->x = position.x << 16;
          spark->y = (96 - (Random16() & 31)) << 16;
          spark->velocityX = (Trig_Sin(angle) * speed) >> 7;
          spark->velocityY = (Trig_Cos(angle) * speed) >> 9;
          spark->variant = Random16() & 7;
        }
      }
      {
        struct EffectStep *mote = &work->particles[SPARKS * 2];

        for (i = 0; i != MOTES; i++, mote++) {
          s32 speed = (Random16() & 127) + 128;
          s32 angle = Random16() & 0x7fff;

          mote->x = target->x;
          mote->y = 52 << 16;
          mote->z = target->z;
          mote->velocityY = ((Random16() & 0xff) - 128) << 9;
          mote->velocityX = (Trig_Sin(angle) * speed) >> 7;
          mote->velocityZ = (Trig_Cos(angle) * speed) >> 7;
          mote->variant = 0;
        }
      }
    }

    // The cues: the burst, the target thrown up 32 pixels, the user's
    // glow, the drain and the target's fall.
    if (frame == 4)
      Sound_PlayFar(212);
    if (frame == 10) {
      work->shakeFrames = 8;
      Sound_PlayFar(144);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 80);
      target->verticalMotionStrength = 0;
      target->y = 32 << 16;
    }
    if (frame == 80)
      target->verticalMotionStrength = 0xab85;
    if (frame == 24)
      Func_0814cd48(effect->actor, -1, 3, -1, 0);
    if (frame == 40)
      Sound_PlayFar(142);
    if (frame == 32)
      Func_081180e8(134);
    if (frame == 64)
      Func_0814cd48(effect->actor, 7, -1, -1, 0);
    if (frame == 88)
      Func_0814cd48(effect->actor, 0, -1, -1, 0);

    // The column: two 14-pixel halves, the right one mirrored.
    if (frame > 7) {
      s32 centre = position.x;
      s32 height;

      if (frame <= 79)
        height = frame * 12 - 96;
      else
        height = 560 - frame * 6;
      if (height > 0) {
        u32 mode;
        s32 x;

        if (height > 80) {
          height = 80;
          mode = BLIT_MAX;
        } else {
          mode = BLIT_ADDITIVE;
        }
        for (i = 0, x = centre - 14; i != 2; i++, x += 14) {
          if (i == 0)
            Func_08196404(104, 7, 7, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, mode);
          else
            Func_08196404(
                104, 7, 7,
                COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X, mode);
          draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
          draw[0](canvas, work->sheet, x, 104 - height, 14, height);
          Runtime_ReleaseHeapBlock(104);
        }
      }
    }

    // The sparks: from frame 8, two more each frame, through seven
    // pictures three frames apiece; one in four is drawn mirrored.
    BattleEffect_FetchRectangleBlitters(effect->side, draw);
    {
      struct EffectStep *spark = work->particles;

      for (i = 0; i != SPARKS; i++, spark++) {
        if (frame >= i / 2 + 8 && spark->variant <= 28) {
          s32 cell = spark->variant / 3;
          s32 x = HI(spark->x);
          s32 y = HI(spark->y);

          if (cell > 6)
            cell = 6;
          draw[(i & 3) ? 0 : 1](
              canvas, work->sheet + Data_08198b5e[cell] + SPARK_OFFSET,
              x - Data_08198b6c[cell] / 2, y - Data_08198b6c[cell] / 2,
              Data_08198b6c[cell], Data_08198b6c[cell]);
          spark->variant++;
          BattleEffectKernels_IntegrateVector2(spark, 62, -0x2000);
        }
      }
    }

    // From frame 32 the motes fly; mote i turns toward the user's middle
    // from frame 32 + i and vanishes once it passes the user.
    if (frame > 31) {
      struct EffectStep *mote;
      struct MotionObject *user;
      s32 userHalfHeight;
      struct BattleCamera *camera;

      user = GetBattleObjectSlotFar(effect->actor)->object;
      userHalfHeight = BattleUnit_GetEffectHeightFar(effect->actor) / 2;
      camera = Ram_HeapSlots->cameraWork;
      Render_ResetTransformState();
      Transform_SetLookAt(camera->eye, camera->pos);
      mote = &work->particles[SPARKS * 2];
      for (i = 0; i != MOTES; i++, mote++) {
        if (mote->variant >= 0) {
          s32 size = (i & 1) + 9;
          s32 userX;
          s32 moteX;

          EffectPosition_ProjectPoint(&mote->x, &screen);
          draw[0](canvas, sheet + gData_08197410[size - 1],
                  (screen.x >> 1) - size / 2, screen.y - size, size, size * 2);
          BattleEffectKernels_IntegrateVector3(mote, 62, 0);
          if (frame > i + 32) {
            s32 dx = (user->x - mote->x) >> 7;
            s32 dy = (user->y + userHalfHeight - mote->y) >> 7;
            s32 dz = (user->z - mote->z) >> 7;

            mote->velocityX += dx;
            mote->velocityY += dy;
            mote->velocityZ += dz;
            moteX = mote->x;
            if (moteX < 0)
              moteX = -moteX;
            userX = user->x;
            if (userX < 0)
              userX = -userX;
            if (moteX >= userX && SameSide(user->x, mote->x))
              mote->variant = -1;
          }
        }
      }
    }

    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);
    if (frame <= 7)
      Func_08158ce0(4, 4);
    else
      Func_08158ce0(16, 16);
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  BattleEffect_EndCanvasLayer();
}

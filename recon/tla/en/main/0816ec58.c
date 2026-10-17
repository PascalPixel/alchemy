// Draft of BattleFx_RunCool, effect kind 75 (Cool), The Broken Seal's
// BattleFx_RunUndeadSword as The Lost Age reworked it (structure twin
// 0.772): the two columns became up to ten leaning ice columns, three
// more for each level of the spell, each with its own start, height and
// lean; the columns' blitters are loaded once a frame through
// Func_08196404 (BattleFx_LoadWork) itself; and the sparks wait for
// the columns.

// Listing 471 instructions, draft 469, differ 164.
// Left: allocation and one loop invariant. The native keeps the level's
// stagger (variant * 4) in r6 and builds the frame count from it after
// the position calls; here it lands in r8 with extra moves. This draft
// also hoists the heap slot base (0x03000000) out of the frame loop into
// r7, where the native rebuilds it at each slot read, which shifts the
// column loop's registers.
// Tried (differ):
// first draft                                                       208
// last position declared before first (their stack slots), and the
// stagger kept in a local for the frame count                      197
// the frame count as sinkFrame + 22                                   206
// alchemy permute 8 minutes, 2 jobs: the two positions as one array
// (ends[2]; the native reads the last x at first + 12)              164

#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "system.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "mem.h"
#include "effect_step.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The pixel half of a 16.16 fixed-point value.
#define HI(v) (((s16 *)&(v))[1])

// The Lost Age numbers the blitter modes from the other end of The Broken
// Seal's COMPOSITE_* list.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };

// The column pictures: 36 pixels wide, and a slim 17-pixel one after them.
#define SLIM_COLUMN_OFFSET 0x870
#define SPARK_OFFSET 0x3060

// How a column leans as it grows.
enum { LEAN_LEFT, LEAN_RIGHT, SLIM_LEAN_LEFT, SLIM_LEAN_RIGHT };

extern char ResourceId_Data0CE;
extern char ResourceId_BlastSheet;
extern char ResourceId_BlueFlameSheet;

// The columns, by number: x, start frame, full height and lean.
extern s16 Data_08198b9c[];
extern u8 Data_08198bb0[];
extern u8 Data_08198bba[];
extern u8 Data_08198bc4[];
// The Broken Seal's gBlastAndSmokeBomb_PuffCells and gCybele_EmberSizes:
// the spark pictures by stage.
extern u16 Data_08198bce[];
extern u16 Data_08198bdc[];

// The canvas layer and its helpers, with The Broken Seal's names.
void BattleFx_BeginCanvasLayer(s32 bgControl);
void Func_08143bb8(void); // BattleFx_EndCanvasLayer
void Func_08143000(void); // BattleFx_ProcessPendingGraphicsTransfer
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode);                       // BattleFx_LoadWork
void Func_08144aac(s32 flip, DrawRectangle *draw); // ..FetchRectangleBlitters
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);            // ObjectGroup_UpdateMembers
void Func_081434f8(void);                  // ObjectGroup_TickMemberTimers
void Func_08158ce0(s32 mask, u32 range);   // Camera_ApplyShake
void Func_08118088(s32 actor, s32 motion); // BattleMotion_ApplyVariantFar
void Func_081180e8(s32 phase);             // BattleEventRuntime_BeginPhaseFar
void EffectPosition_ApplyAlternateStepAndYOffset(
    s32 actor, struct EffectPosition *position);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Audio_PlayCue(s32 cue);

// Cool: ice columns burst up around the targets one after another, leaning
// as they grow, and stand until they sink back; sparks of frost fly up
// from the ground and drift upward.
//
// Setup: canvas layer 1 at half blend and full width, centred between the
// first and last target; the column sheet, the spark sheet after it, and
// the blue palette.
// Frame loop (54 frames and 4 more per level): the sparks start at frame
// 0; column i rises from its start frame 10 pixels a frame, strikes a
// target two frames in, and sinks 10 pixels a frame from frame 32 + 4 per
// level. The columns are drawn with max blitters until frame 24 on the
// higher levels, with additive ones otherwise.
void BattleFx_RunCool(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 stagger;
  s32 sinkFrame;
  DrawRectangle draw[2];
  struct EffectPosition ends[2];
  struct EffectStep *spark;
  s32 frame;
  s32 i;

  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  BattleFx_BeginCanvasLayer(1);
  REG_BG2PA = 0x100;
  REG_BLDALPHA = 0x1010;
  Resource_LoadAndDecompress((s32)&ResourceId_Data0CE, work->sheet, 0, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet,
                             work->sheet + SPARK_OFFSET, 1, 0);
  Iwram_CopyWords((void *)BG_PLTT,
                  Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet), 128);
  work->transferMode = 2;
  work->transferValue = 50;
  Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
  stagger = effect->variant * 4;
  sinkFrame = stagger + 32;

  // Centre the canvas between the first and the last target.
  EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[0], &ends[0]);
  EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[effect->count - 1],
                                              &ends[1]);
  ends[0].x += (ends[1].x - ends[0].x) / 2;
  REG_BG2X = (64 - ends[0].x) << 8;

  for (frame = 0; frame != stagger + 54; frame++) {
    // Frame 0: 16 sparks leave the ground in random directions.
    if (frame == 0) {
      spark = work->particles;
      for (i = 0; i != 16; i++, spark++) {
        s32 speed = (Random16() & 0x1ff) + 128;
        s32 angle = Random16() & 0xffff;

        spark->x = ((Random16() & 63) + 32) << 16;
        spark->y = 96 << 16;
        spark->velocityX = (Trig_Sin(angle) * speed) >> 8;
        spark->velocityY = (Trig_Cos(angle) * speed) >> 8;
        spark->variant = 8;
      }
    }
    if (frame == 32)
      Func_081180e8(134);

    if (frame >= 0) {
      u32 mode = BLIT_MAX;

      if (frame <= 23 && effect->variant > 0)
        mode = BLIT_ADDITIVE;
      Func_08196404(104, 7, 7, COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y, mode);
      draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
      Func_08196404(188, 7, 7,
                    COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X,
                    mode);
      draw[1] = (DrawRectangle)Ram_HeapSlots->unkb8[1];

      for (i = 0; i != effect->variant * 3 + 4; i++) {
        s32 age = frame - Data_08198bb0[i];
        s32 x = Data_08198b9c[i];
        s32 scale;
        s32 fullHeight;
        s32 height;

        // Two frames after it starts, a column strikes the next target.
        if (age == 2) {
          s32 member = i % effect->count;

          Func_0814cd48(effect->actors[member], 7, 5, member, 3);
          Func_08118088(effect->actors[member], 0);
          Audio_PlayCue(212);
          work->shakeFrames = 3;
        }

        // The slim columns stand a little over half as high.
        scale = 60;
        if (Data_08198bc4[i] > LEAN_RIGHT)
          scale = 35;
        fullHeight = Data_08198bba[i] * scale / 64;
        if (frame < sinkFrame)
          height = age * 10;
        else
          height = fullHeight - (frame - sinkFrame) * 10;
        if (height > fullHeight)
          height = fullHeight;

        switch (Data_08198bc4[i]) {
        case LEAN_LEFT:
          x -= height / 2;
          break;
        case LEAN_RIGHT:
          x += height / 2;
          break;
        case SLIM_LEAN_LEFT:
          x -= height / 4;
          break;
        case SLIM_LEAN_RIGHT:
          x += height / 4;
          break;
        }
        if (Data_08198bc4[i] <= LEAN_RIGHT)
          draw[Data_08198bc4[i] & 1](canvas, work->sheet, x - 18, 112 - height,
                                     36, height);
        else
          draw[Data_08198bc4[i] & 1](canvas, work->sheet + SLIM_COLUMN_OFFSET,
                                     x - 9, 108 - height, 17, height);
      }
      Runtime_ReleaseHeapBlock(104);
      Runtime_ReleaseHeapBlock(188);
    }

    // Spark i rises from frame 8 + 2i through seven pictures, three frames
    // apiece; past the first 16 they wait until the columns sink.
    Func_08144aac(effect->side, draw);
    spark = work->particles;
    for (i = 0; i != effect->variant * 4 + 8; i++, spark++) {
      if ((i <= 15 || frame >= sinkFrame) && frame >= i * 2 + 8 &&
          spark->variant <= 28) {
        s32 cell = spark->variant / 3;
        s32 x = HI(spark->x);
        s32 y = HI(spark->y);
        u32 size;

        if (cell > 6)
          cell = 6;
        size = Data_08198bdc[cell];
        draw[0](canvas, work->sheet + Data_08198bce[cell] + SPARK_OFFSET,
                x - size / 2, y - size / 2, size, size);
        spark->variant++;
        BattleFxKernels_IntegrateVector2(spark, 62, -0x2000);
      }
    }
    Runtime_ReleaseHeapBlock(188);
    Runtime_ReleaseHeapBlock(104);

    Func_08158ce0(4, 4);
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(Func_08143000);
  Func_08143bb8();
}

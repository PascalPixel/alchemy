// Draft, differ 68 of 699: Func_08163e14, unused (no handler, summon case or
// call reaches it), a second copy of The Broken Seal's
// BattleFx_RunNereid (summon 2's, which The Lost Age keeps at
// BattleFx_RunNereid).

// Twin: ☀️ BattleFx_RunNereid (games/tbs/src/effect/effect.c),
// ported: the palette goes by DMA 3, the blitters come from
// BattleFx_FetchRectangleBlitters(0, draw), the effect stays an argument (its
// count and actors are read from it; only Func_0814cc4c reads the work's copy),
// formation resource 649, callback Func_08143000 at 0xc80.

// The column table is not const: the native reads it again after each draw
// call, and as const data the reuse moved every register (differ 292).

// Left: everything is reordered rows where the native puts a load or a store
// ahead of an independent constant (the argument's store before 0x03000000,
// the work's reloads, the pooled values, the 58 multiply), the kind
// Func_08164b2c's header traces to a load costing 3 and a store 2.
#include "types.h"
#include "ram_buffer.h"
#include "io_reg.h"
#include "dma.h"
#include "system.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "input.h"
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

// The map cell buffer holds the embers.
#define EMBERS ((struct EffectStep *)Ram_MapCellBuffer)
#define EMBER_COUNT 1024
#define PILLAR_SHEET 0x6e4 // the fire pillar after the spray in the sheet

#define FORMATION 649
#define FORMATION_SIZE 12

#define SCENE_FRAMES 124
#define SKIP_FROM_FRAME 32 // A or B jumps from here...
#define SKIP_TO_FRAME 98   // ...to here
#define FIRST_SPOUT_FRAME 28
#define SPOUT_FRAME 32 // the embers spout and the pillars rise until frame 79
#define SPOUT_END_FRAME 80
#define RESULTS_FRAME 120

#define PHASE_SUMMON_RESULTS 134
#define SE_SUMMON 164
#define SE_SPOUT 145
#define SE_FADE 144

extern char ResourceId_WaterSpraySheet;
extern char ResourceId_FirePillarSheetA;
extern char ResourceId_Data134;    // ☀️ ParticleSpritesA
extern u8 gData_0819896a[];        // the pillars' feet, x and y
extern const u16 gData_08197410[]; // ☀️ gBattleFx_MoteCellOffsets
extern const s32 gData_08198974[]; // ☀️ gNereid_Gravity

void BattleFx_BeginCanvasLayer(s32 bgControl);
void Func_08143000(void); // BattleFx_ProcessPendingGraphicsTransfer
void BattleFx_FetchRectangleBlitters(
    s32 side,
    DrawRectangle *draw); // BattleFx_FetchRectangleBlitters
void Func_0814cc4c(
    struct BattleEffectArgument *effect); // BattleFx_SelectLivingTargets
void Func_08152404(s32 count, s32 resource,
                   s32 priority); // BattleFx_SpawnObjects
void Func_081180e8(s32 phase);    // BattleEventRuntime_BeginPhaseFar
void Audio_PlayCue(s32 cue);
void Func_0814cd48(s32 actor, s32 palette, s32 a, s32 timer,
                   s32 frames); // ObjectGroup_UpdateMembers
void Func_0816442c(s32 formation, s32 x,
                   s32 y); // BattleFx_PlaceFormationObjects
void Func_0816467c(s32 formation, s32 x,
                   s32 y); // BattleFx_RunImpactBurst
void Func_08143bb8(void);  // BattleFx_EndCanvasLayer

struct Point2D {
  s32 x;
  s32 y;
};

// A formation of twelve sweeps in from the right; three pillars of fire
// stream upward and a fountain of embers spouts from the ground, falls, and
// at the end rises away, while the targets jolt every twelve frames.
//
// Setup: open the canvas, load the spray and pillar pictures and the ember
// cells, empty the particle pools, and spawn the formation. Each frame (124,
// A or B skipping from 33 to 98): the formation glides in and slows; 32
// embers spout at frame 28 and 16 more a frame from 32 to 79; the spray rolls
// along the ground until frame 95; the pillars stream up from 32 to 79; each
// ember grows as it falls (the brighter blitter once it falls) and grows
// again toward the end; the targets flash and jump every 12 frames from 30 to
// 79. At the end the formation bursts and its objects are released.
void Func_08163e14(struct BattleEffectArgument *effect) {
  DrawRectangle draw[2];
  void *canvas;
  struct BattleEffectWork *work;
  u8 *cells;
  u8 *palette;

  canvas = Ram_HeapSlots->battleFxWork[1];
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  cells = Ram_HeapSlots->unk64[0];
  BattleFx_BeginCanvasLayer(0);
  REG_BLDALPHA = 0x1010;
  BattleFx_FetchRectangleBlitters(0, draw);

  // The spray's palette and picture, the pillar's picture, the ember cells.
  palette = Resource_GetTableEntry((s32)&ResourceId_WaterSpraySheet);
  Dma_Set(palette, (void *)BG_PLTT, 0x84000020, REG_DMA3);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet);
  palette = Resource_GetTableEntry((s32)&ResourceId_FirePillarSheetA);
  palette += 128;
  Resource_DecodeType01(palette, work->sheet + PILLAR_SHEET);
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Data134),
                        cells);
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(Func_08143000, 0xc80);
  {
    struct Point2D focus;
    struct Point2D velocity;
    s32 rise;
    s32 frame;
    s32 k;
    struct EffectStep *point;
    struct EffectStep *ember;
    struct EffectStep *embers;
    s32 *life;

    // The formation starts off the right edge, gliding left.
    rise = 0;
    focus.x = 256 << 16;
    focus.y = 88 << 16;
    velocity.x = -(16 << 16);
    velocity.y = -(4 << 16);
    for (k = 0; k != 64; k++)
      work->particles[k].variant = -1;
    for (k = 0; k != 16; k++) {
      point = &work->particles[24 + k];
      point->x = Random16() & 127;
      point->y = (Random16() & 7) + 56;
      point->variant = -(Random16() & 15);
    }
    life = &EMBERS->variant;
    for (k = 0; k != EMBER_COUNT; k++) {
      *life = -1;
      life += sizeof(struct EffectStep) / sizeof(s32);
    }
    embers = EMBERS;
    Func_0814cc4c(work->effect);
    WaitFrames(1);
    Func_08152404(FORMATION_SIZE, FORMATION, 2);

    for (frame = 0; frame != SCENE_FRAMES; frame++) {
      if ((gInput.repeat & 3) && frame > SKIP_FROM_FRAME &&
          frame < SKIP_TO_FRAME)
        frame = SKIP_TO_FRAME;
      if (frame == RESULTS_FRAME)
        Func_081180e8(PHASE_SUMMON_RESULTS);
      if (frame <= 15)
        rise += 2;

      // The formation glides in, slowing, and drifts back right as it
      // passes x = 120.
      if (frame <= 99) {
        focus.x += velocity.x;
        focus.y += velocity.y;
        velocity.x = velocity.x * 58 / 64;
        velocity.y = velocity.y * 56 / 64;
        if (focus.x < 120 << 16)
          velocity.x += 0x8000;
      }
      Func_0816442c(1, focus.x, focus.y);

      // The first spout fills the first 256 slots; then 16 embers a frame
      // from the ground at (32, 96).
      if (frame == FIRST_SPOUT_FRAME) {
        for (k = 0; k != 256; k++) {
          ember = &embers[k];
          if (ember->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & 0xffff;

            ember->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
            ember->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
            ember->velocityX = ((Random16() & 63) - 32) << 14;
            ember->velocityY = (-(Random16() & 63) - 8) << 13;
            ember->variant = 0;
          }
        }
      }
      if (frame >= SPOUT_FRAME && frame < SPOUT_END_FRAME) {
        s32 spawned = 0;

        for (k = 0; k != EMBER_COUNT; k++) {
          ember = &embers[k];
          if (ember->variant == -1) {
            s32 radius = Random16() & 63;
            s32 angle = Random16() & 0xffff;

            ember->x = ((Trig_Sin(angle) * radius) >> 3) + (32 << 16);
            ember->y = ((Trig_Cos(angle) * radius) >> 2) + (96 << 16);
            ember->velocityX = ((Random16() & 63) - 32) << 14;
            ember->velocityY = (-(Random16() & 63) - 8) << 13;
            ember->variant = 0;
            spawned++;
            if (spawned == 16)
              break;
          }
        }
      }
      if (frame == 0)
        Audio_PlayCue(SE_SUMMON);
      if (frame == SPOUT_FRAME)
        Audio_PlayCue(SE_SPOUT);
      if (frame == SPOUT_END_FRAME)
        Audio_PlayCue(SE_FADE);

      // The pillars stream upward, each a 104-pixel picture drawn twice to
      // wrap.
      if (frame >= SPOUT_FRAME && frame < SPOUT_END_FRAME) {
        for (k = 0; k != 3; k++) {
          s32 span = (frame * 16 - 256 + k * 25) % 104;

          draw[0](canvas, work->sheet + PILLAR_SHEET,
                  gData_0819896a[k * 2] - 17,
                  gData_0819896a[k * 2 + 1] - span - 104, 34, 104);
          draw[0](canvas, work->sheet + PILLAR_SHEET,
                  gData_0819896a[k * 2] - 17, gData_0819896a[k * 2 + 1] - span,
                  34, span);
        }
      }

      // The spray rolls along the ground, rising for the first 16 frames.
      if (frame <= 95) {
        for (k = 0; k != 5; k++)
          draw[0](canvas, work->sheet, k * 32 + ((frame / 4) & 31) - 32,
                  120 - rise, 32, 32);
      }

      // The embers: bigger when falling and toward the end, pulled down by
      // one of four gravities, then up after frame 80; gone below y = 104.
      for (k = 0; k != EMBER_COUNT; k++) {
        ember = &embers[k];
        if (ember->variant >= 0) {
          s32 size = k % 3 + 2;

          if (ember->velocityY > 0)
            size += 2;
          if (frame > 68 && size <= 5)
            size = 6;
          if (frame > 70 && size <= 6)
            size = 7;
          if (frame > 72 && size <= 7)
            size = 8;
          if (frame > 74 && size <= 8)
            size = 9;
          if (frame > 76)
            size = 10;
          draw[ember->velocityY > 0](canvas, cells + gData_08197410[size - 1],
                                     HI(ember->x) - size / 2,
                                     HI(ember->y) - size, size, size * 2);
          ember->x += ember->velocityX;
          ember->y += ember->velocityY;
          if (frame > SPOUT_END_FRAME)
            ember->velocityY += -0x8000;
          else
            ember->velocityY += gData_08198974[k & 3];
          ember->velocityX = ember->velocityX * 62 / 64;
          ember->velocityY = ember->velocityY * 62 / 64;
          ember->variant++;
          if (ember->velocityY > 0 && HI(ember->y) > 104)
            ember->variant = -1;
        }
      }

      // The targets flash and jump every 12 frames while the embers spout.
      if (frame < SPOUT_END_FRAME) {
        for (k = 0; k != effect->count; k++) {
          if (frame > 29) {
            s32 phase = frame % 12;

            if (phase == 0) {
              struct MotionObject *target =
                  GetBattleObjectSlotFar(effect->actors[k])->object;

              Func_0814cd48(effect->actors[k], 7, 5, -1, 0);
              target->velocityY = 0x48000;
              target->verticalMotionStrength = 0xab85;
            }
            if (phase == 6)
              Func_0814cd48(effect->actors[k], 0, 5, -1, 0);
          }
        }
      }
      work->transferPending = 1;
      WaitFrames(1);
    }

    Scheduler_RemoveCallback(Func_08143000);
    Runtime_ReleaseHeapBlock(HEAP_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_BLITTER);
    Func_0816467c(1, focus.x, focus.y);
    for (k = 0; k != FORMATION_SIZE; k++)
      ResourceObject_ReleaseFar(work->objects[k]);
  }
  Func_08143bb8();
}

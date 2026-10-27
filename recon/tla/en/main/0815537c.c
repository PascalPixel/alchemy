// Draft of BattleEffect_RunBolt, effect kind 16: a bolt of lightning strikes each
// target in turn, eight frames apart, throwing sparks that bounce on the
// ground and flashing glints of light around the target.

// Twin: The Broken Seal's BattleEffect_RunBolt (effect.c), the same
// kind, with the same tables, frame counts and cues.
// Listing 552 instructions, draft 552, differ 4 (tla-en, 2026-10-26).

// Changes from the twin: the effect argument is read directly (the Lost
// Age never stores work->effect), the blitter loads through BattleEffect_LoadBlitter
// with the Lost Age's mode numbers (max is 1), the glint blitter loads
// through Func_08196404 (the twin's BattleEffect_BuildBlitter), the callback
// order is 0xc80.

// What moved it: the glints written as the twin writes them, with one
// tagged one-pass block between the glint's position and its blitter. As
// plain C (174), cse1 merges each size table's two address reads and
// loop.c then treats the merged address as an invariant; with the block
// they stay apart until loop.c has run, cse2 merges them within the
// iteration, and the bases stay in sl and r5 across the calls with kind
// reloaded from the stack, as the native has them (101 with the earlier
// glint-pointer device; 6 with the block). The flip's random number drawn into a
// local before the block (found by alchemy permute): 4.

// What remains, one row each: the prologue's canvas read after the
// argument for the canvas layer, and the glint's height read after its
// stacked width; the known store-then-load scheduling limit (a spill store
// and an argument store before loads).

// Tried (differ):
// the twin as is, with the Lost Age's names                    204
// declaration order, greedy scan of every local                no gain
// glint table pointers declared and set at the top of the body 240
// glint pointers assigned where first read                     144
// the same, heights only                                       157
// the same, draw indexed by j & 3                              144
// 2026-10-26, from 6: barriers (plain, and on each local) and one-pass
// blocks at every statement, statement moves, const on each table, x's
// steps merged: no gain.

#include "types.h"
#include "battle_effect.h"
#include "battle_effect_work.h"
#include "effect_step.h"
#include "io_reg.h"
#include "iwram_call.h"
#include "ram_buffer.h"

// The sparks: 128 for each target at the map cell buffer.
#define SPARKS ((struct EffectStep *)Ram_MapCellBuffer)

// Directory rows 0x192 BattleEffect_TornadoSheet (named), 0x188
// BattleEffect_LightningBoltSheet and 0x134 Resource_Data134 (named), the
// spark sprites the twin calls ParticleSpritesA.
extern char ResourceId_TornadoSheet;
extern char ResourceId_LightningBoltSheet;
extern char ResourceId_Data134;

// The Broken Seal's gBolt_Counts: four bytes for each strength,
// sparks, glints, glint spread and bolts.
extern u8 Data_08198462[];
// The Broken Seal's gBolt_GlintFlips and _GlintModes.
extern u8 Data_0819846e[];
extern u8 Data_08198472[];
// The Broken Seal's gBattleEffect_GlintCells, _GlintCellWidths and
// _GlintCellHeights, and gBattleEffect_MoteCells.
extern u16 gData_08197486[];
extern u8 gData_08197492[];
extern u8 gData_08197498[];
extern u16 gData_08197410[];

void BattleEffect_StartCanvasLayer(s32 control);
void BattleEffect_EndCanvasLayer(void);
s32 BattleEffect_LoadBlitter(s32 slot, s32 modeFlags);
s32 Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                  u32 mode);
void BattleEffect_ProcessPendingGraphicsTransfer(void);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Runtime_ReleaseHeapBlock(s32 slot);
void Audio_PlayCue(s32 cue);
void Func_081180e8(s32 phase);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void Func_0814cd48(s32 actor, s32 palette, s32 frames, s32 timerSlot,
                   s32 timerFrames);
void Func_08118088(s32 actor, s32 motion);
void Func_08158ce0(s32 x, s32 y);
void ObjectGroup_CountDownMemberTimers(void);
void WaitFrames(s32 frames);
void EffectPosition_ProjectBattler(s32 id,
                                                 struct EffectPosition *pos);
void BattleEffectKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);

#define BOLT_INTERVAL 8
#define SPARK_COUNT 1024
#define SPARKS_PER_TARGET 128
#define GROUND_Y 104
#define CUE_THUNDER 138
#define PHASE_RESULTS 133 // Func_081180e8: the results phase begins

// BattleEffect_LoadBlitter(slot, mode << 4 | flags) loads a blitter into a heap slot.
#define BLITTER_MAX (1 << 4)

// The second blitter, loaded again for each glint.
#define DRAW_GLINT ((DrawRectangle)Ram_HeapSlots->unkb8[1])

void BattleEffect_RunBolt(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work;
  void *canvas;
  s32 i;
  s32 frame;
  /* FAKEMATCH(dead): a pair, as the twin has, of which only draw[0] is used:
   * the unused second word fills four bytes of the native frame; as one
   * blitter, 82 of 552 instructions differ in tla-en. */
  DrawRectangle draw[2];
  u8 *sprites;
  struct EffectPosition pos;
  s32 j;

  // Set up: the canvas layer, the bolt and spark sheets, and every spark
  // and particle free.
  work = (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  canvas = Ram_HeapSlots->battleFxWork[1];
  sprites = Ram_HeapSlots->unk64[0];
  BattleEffect_StartCanvasLayer(1);
  BattleEffect_LoadBlitter(104, BLITTER_MAX | COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];
  Resource_LoadAndDecompress((s32)&ResourceId_TornadoSheet, work->sheet, 1, 0);
  Resource_LoadAndDecompress((s32)&ResourceId_LightningBoltSheet,
                             work->sheet + 0xc56, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_Data134, sprites, 0, 0);
  for (j = 0; j != SPARK_COUNT; j++)
    SPARKS[j].variant = 0;
  for (j = 0; j != 64; j++)
    work->particles[j].variant = -1;
  work->transferMode = 2;
  work->transferValue = 75;
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, 0xc80);
  Audio_PlayCue(CUE_THUNDER);

  for (frame = 0; frame != effect->count * BOLT_INTERVAL + 40; frame++) {
    if (frame == 24)
      Func_081180e8(PHASE_RESULTS);

    // The sky flashes as each bolt starts.
    for (i = 0; i != effect->count; i++) {
      if (frame == i * BOLT_INTERVAL)
        Iwram_FillWords(canvas, 0x4000, 0x10101010);
    }

    for (i = 0; i != effect->count; i++) {
      s32 start = i * BOLT_INTERVAL;

      EffectPosition_ProjectBattler(effect->actors[i], &pos);
      pos.x /= 2;
      if (frame == start + 1)
        work->shakeFrames = 4;
      if (frame == start + 4) {
        Func_0814cd48(effect->actors[i], 7, 5, i, 6);
        Func_08118088(effect->actors[i], 6);
      }

      // The bolt, 24 wide, grows 64 pixels a frame down to the ground.
      if (frame >= start && frame < start + 16) {
        s32 height = (frame - start) << 6;

        if (height > GROUND_Y)
          height = GROUND_Y;
        for (j = 0; j != Data_08198462[effect->variant * 4 + 3]; j++) {
          draw[0](canvas,
                  work->sheet + (((i + frame + j) / 2) & 3) * 2880 + 0xc56,
                  pos.x - 12, 0, 24, height);
        }

        // Sparks fly up from the ground within 90 degrees of straight up.
        if (frame == start + 2) {
          for (j = 0; j != Data_08198462[effect->variant * 4]; j++) {
            struct EffectStep *spark = &SPARKS[i * SPARKS_PER_TARGET + j];
            s32 speed = (Random16() & 0x1ff) + 64;
            s32 angle = (Random16() & 0x7fff) - 0x4000;

            spark->x = pos.x << 16;
            spark->y = GROUND_Y << 16;
            spark->velocityX = (Trig_Sin(angle) * speed) >> 5;
            spark->velocityY = -(Trig_Cos(angle) * speed) >> 6;
            spark->variant = (Random16() & 7) + 32;
          }
        }
      }

      // Glints: at random places above the target's feet, each with a
      // random palette and flip.
      if (frame >= start + 2 && frame < start + 24) {
        for (j = 0; j != Data_08198462[effect->variant * 4 + 1]; j++) {
          s32 kind = j & 3;
          s32 drop = Random16() % Data_08198462[effect->variant * 4 + 2];
          s32 spread = Data_08198462[effect->variant * 4 + 2] - drop + 1;
          s32 y = pos.y - drop - (gData_08197498[kind] >> 1) + 8;
          u32 r = Random16();
          s32 x = pos.x;
          u32 flip;

          x += r % spread;
          x -= spread / 2;
          x -= gData_08197492[kind] >> 1;
          flip = Random16();

          /* FAKEMATCH(one-pass): the block ends cse1's block, so the two reads
           * of each size table's address stay apart until loop.c has run; as
           * plain C, 174 of 552 instructions differ in tla-en. */
          do {
          } while (0);
          Func_08196404(188, 7, 7,
                        Data_0819846e[flip & 3] | COMPOSITE_CLIP_X |
                            COMPOSITE_CLIP_Y,
                        Data_08198472[effect->variant]);
          DRAW_GLINT(canvas, work->sheet + gData_08197486[kind], x, y,
                     gData_08197492[kind], gData_08197498[kind]);
          Runtime_ReleaseHeapBlock(188);
        }
      }
    }

    // Sparks fall, bounce off the ground at half speed, and shrink as they
    // burn out.
    for (j = 0; j != SPARK_COUNT; j++) {
      struct EffectStep *spark = &SPARKS[j];

      if (spark->variant > 0) {
        spark->variant--;
        BattleEffectKernels_IntegrateVector2(spark, 60, 0x1000);
        if (spark->y > GROUND_Y << 16) {
          spark->velocityY = -spark->velocityY / 2;
        } else if ((spark->x >= 0 && spark->x < 127 << 16) && spark->y >= 0) {
          s32 size = spark->variant / 16 + 1;
          s32 x = spark->x >> 16;
          s32 y = spark->y >> 16;

          draw[0](canvas, sprites + gData_08197410[size - 1], x - size / 2,
                  y - size, size, size * 2);
        }
      }
    }
    Func_08158ce0(2, 8);
    ObjectGroup_CountDownMemberTimers();
    work->transferPending = 1;
    WaitFrames(1);
  }

  // Tear down: stop the transfer, free the blitter, close the layer.
  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(104);
  BattleEffect_EndCanvasLayer();
}

// Near miss: listing 529 instructions, draft 529, differ 12. ☀️ twin:
// BattleFx_RunHelmSplitter (effect.c, structure 0.800): the same swirl and
// spark burst, with a second variant ⚓️ added at the target.
// Statements, allocation and frame all match (effect r10, work r11, canvas at
// sp+16, the two blitters at sp+20, the position at sp+28 and its address in
// r9, frame and frame - 8 spilled); what is left (12 rows) is sched2 order in
// four places: the frame loop's preheader (the native clears frame and builds
// -8 before it starts the position's address), the second side's cell blit
// (the native loads biasX's table before 0x700), the spark spawn's entry test
// (the native builds -16 before it loads the variant) and the spark height test
// (the native loads y before 0x680000). Each is the native issuing a store or a
// load sooner than agscc's arm9tdmi costs rank it. Measured: the BLDALPHA and
// BG2PA stores take f18284c.c's blend pin (each differ 2 without); the timer
// clear through &sparks->variant, i before the pixel pointer, the pixels
// indexed and the sparks as &sparks[i] (as in BattleFx_RunHelmSplitter) are
// the source's forms (38 to 22); a one-pass block on y fixes the height test
// but moves the IntegrateVector2 arguments (18); an asm barrier on y relays out
// the function (105). 2026-10-15 (fx-n3): rewritten for reading on struct
// BattleEffectWork, with the resource ids and ☀️'s table names (effect_table.s:
// gData_08199d40 CellSourceOffsets, 08199d4e CellBiasX, 08199d55 CellBiasY,
// 08199d31 CellWidths, 08199d38 CellHeights); BLDALPHA's pin as one macro,
// BG2PA's pin alone (with the barrier too: 26, its estimated four bytes move
// the next literal pool); still 18. alchemy permute (10 min, 2 jobs, 2,732
// candidates): nothing below 18. The tables extern without const, as
// ☀️'s readers declare them: 18 to 12 (the native reloads the cell tables
// after each call).
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The Lost Age's heap slots, by byte offset into Ram_HeapSlots.
#define HEAP_SLOT_BLITTER 104
#define HEAP_SLOT_BLITTER_ALTERNATE 188

// The transfer callback runs at this order, where ☀️'s runs at 0x480.
#define TRANSFER_CALLBACK_ORDER 0xc80

// FAKEMATCH: ⚓️ loads a pooled halfword before it starts the register's
// address; the plain store starts the address first (one reordered pair,
// differ 2). Only a halfword register loads from the pool, so the value is
// pinned to the r3 it already takes, and the barrier holds r2 until it is
// loaded, as in f18284c.c.
#define REG_WRITE16_POOLED(reg, value)                                         \
  {                                                                            \
    register u16 pooled_ __asm__("r3") = (value);                              \
    __asm__("" : "+l"(pooled_) : : "r2");                                      \
    (reg) = pooled_;                                                           \
  }

extern char ResourceId_FireSwirlSheet;
extern char ResourceId_PinkStarSheet;
extern char ResourceId_SkullSheet;
extern char ResourceId_BlueFlameSheet;

// The swirl's seven growth frames, cut from one sheet. Not const: the native
// reloads them after every call.
extern u16 gFireSwirl_CellSourceOffsets[];
extern s8 gFireSwirl_CellBiasX[];
extern s8 gFireSwirl_CellBiasY[];
extern u8 gFireSwirl_CellWidths[];
extern u8 gFireSwirl_CellHeights[];

void BattleFx_BeginCanvasLayer(s32 bgControl);
void *Resource_GetTableEntry(s32 id);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void Func_08144aac(s32 side, DrawRectangle *draw);
void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
void EffectPosition_ApplyAlternateStepAndYOffset(
    s32 id, struct EffectPosition *position);
void Func_081180e8(s32 phase);
void Func_0814cd48(s32 actor, s32 palette, s32 group, s32 timerSlot,
                   s32 frames);
void Func_08118088(s32 actor, s32 motion);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void BattleFxKernels_IntegrateVector2(struct EffectStep *step, s32 damping,
                                      s32 gravity);
void Func_08158ce0(s32 randomMask, u32 shakeRange);
void Func_081434f8(void);
void WaitFrames(s32 frames);
void Runtime_ReleaseHeapBlock(s32 slot);
void Func_08143bb8(void);
void Func_08143000(void);

// Effect kind 51: the caster dashes at the target while a swirl of flame grows
// over seven frames, then bursts into sparks that fall and bounce on the
// ground. Variant 0 is ☀️'s: the swirl rises at the caster and throws pink
// stars. Variant 1 is ⚓️'s own: the swirl, recoloured into the blue flame's
// palette, rises at the target and throws 32 skulls.
void Func_08188cb0(struct BattleEffectArgument *effect) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  DrawRectangle draw[2];
  struct EffectPosition position;
  struct EffectStep *sparks;
  struct EffectStep *spark;
  s32 *life;
  s32 frame;
  s32 i;

  // Set up the canvas and load the swirl and the spark sheets.
  BattleFx_BeginCanvasLayer(1);
  REG_WRITE16_POOLED(REG_BLDALPHA, 0x1010);
  if (effect->variant == 0) {
    // FAKEMATCH: as above, but here the pin alone loads the value between
    // the halves of the address (differ 2 without; the barrier too: 26, its
    // estimated four bytes move the next literal pool).
    register u16 scale __asm__("r3") = 0x100;

    REG_BG2PA = scale;
  }
  Resource_LoadAndDecompress((s32)&ResourceId_FireSwirlSheet, work->sheet, 1,
                             1);
  if (effect->variant == 0) {
    Resource_LoadAndDecompress((s32)&ResourceId_PinkStarSheet,
                               work->sheet + 0x3e80, 1, 0);
    work->transferValue = 50;
  } else {
    // Lift every drawn pixel of the swirl into the upper half of the palette,
    // where the blue flame's colours go.
    u8 *pixels = work->sheet;

    for (i = 0; i != 16000; i++) {
      if (pixels[i] != 0) {
        pixels[i] = pixels[i] * 2 + 32;
        if (pixels[i] > 63)
          pixels[i] = 63;
      }
    }
    Resource_LoadAndDecompress((s32)&ResourceId_SkullSheet,
                               work->sheet + 0x3e80, 1, 1);
    Iwram_CopyWords((void *)0x05000000,
                    Resource_GetTableEntry((s32)&ResourceId_BlueFlameSheet),
                    128);
    work->transferValue = 75;
  }
  work->transferMode = 2;
  Scheduler_AddOrUpdateCallback(Func_08143000, TRANSFER_CALLBACK_ORDER);
  Func_08144aac(effect->side, draw);

  // No spark is alive yet.
  for (i = 0, life = &work->particles[0].variant; i != 64; i++) {
    *life = 0;
    life += 7;
  }
  sparks = work->particles;

  // The caster dashes at the target.
  BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 16, 0x70000);

  for (frame = 0; frame != 70; frame++) {
    // Frames 8-23: the swirl grows, one cell every second frame.
    if (effect->variant == 0) {
      EffectPosition_ApplyAlternateStepAndYOffset(effect->actor, &position);
      if (effect->side == 0)
        REG_BG2X = (80 - position.x) << 8;
      else
        REG_BG2X = (32 - position.x) << 8;
      if (frame >= 8 && frame <= 23) {
        s32 cell = (frame - 8) / 2;

        if (cell > 6)
          cell = 6;
        if (effect->side == 0) {
          draw[0](canvas, work->sheet + gFireSwirl_CellSourceOffsets[cell],
                  gFireSwirl_CellBiasX[cell] + 30,
                  gFireSwirl_CellBiasY[cell] + position.y - 60,
                  gFireSwirl_CellWidths[cell], gFireSwirl_CellHeights[cell]);
        } else {
          draw[0](canvas, work->sheet + gFireSwirl_CellSourceOffsets[cell],
                  -gFireSwirl_CellBiasX[cell] - gFireSwirl_CellWidths[cell] +
                      98,
                  gFireSwirl_CellBiasY[cell] + position.y - 60,
                  gFireSwirl_CellWidths[cell], gFireSwirl_CellHeights[cell]);
        }
      }
    } else {
      EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[0], &position);
      if (frame >= 8 && frame <= 23) {
        s32 cell = (frame - 8) / 2;

        if (cell > 6)
          cell = 6;
        if (effect->side == 0) {
          draw[0](canvas, work->sheet + gFireSwirl_CellSourceOffsets[cell],
                  position.x / 2 + gFireSwirl_CellBiasX[cell] - 16,
                  gFireSwirl_CellBiasY[cell] + position.y - 92,
                  gFireSwirl_CellWidths[cell], gFireSwirl_CellHeights[cell]);
        } else {
          draw[0](canvas, work->sheet + gFireSwirl_CellSourceOffsets[cell],
                  position.x / 2 - gFireSwirl_CellBiasX[cell] -
                      gFireSwirl_CellWidths[cell] + 16,
                  gFireSwirl_CellBiasY[cell] + position.y - 92,
                  gFireSwirl_CellWidths[cell], gFireSwirl_CellHeights[cell]);
        }
      }
    }

    // Frame 18: the hit. The target flashes and reels, the screen shakes and
    // the swirl bursts into sparks.
    if (frame == 18) {
      EffectPosition_ApplyAlternateStepAndYOffset(effect->actors[0], &position);
      Func_081180e8(134);
      Func_0814cd48(effect->actors[0], 7, 5, 0, 8);
      Func_08118088(effect->actors[0], 6);
      work->shakeFrames = 4;
      for (i = 0; i != effect->variant * 16 + 16; i++) {
        s32 force = (Random16() & 255) + 256;
        s32 angle = Random16() & 0xffff;

        spark = &sparks[i];
        if (effect->variant == 0)
          spark->x = 0x400000;
        else
          spark->x = (position.x / 2) << 16;
        spark->y = 0x500000;
        spark->velocityX = (Trig_Sin(angle) * force) >> 7;
        spark->velocityY = -(Trig_Cos(angle) * force) >> 6;
        spark->variant = (Random16() & 15) + 16;
      }
    }

    // The sparks fall, bounce off the ground at y = 104 and fade out.
    for (i = 0; i != 64; i++) {
      spark = &sparks[i];
      if (spark->variant > 0) {
        spark->variant--;
        BattleFxKernels_IntegrateVector2(spark, 60, 0);
        if (spark->y > 0x680000) {
          spark->velocityY = -spark->velocityY / 2;
        } else if ((spark->x >= 0 && spark->x <= 0x7effff) && spark->y >= 0) {
          s32 sx = spark->x >> 16;
          s32 sy = spark->y >> 16;

          if (effect->variant == 0)
            draw[0](canvas, work->sheet + 0x3e80 + (((frame + i) / 4 % 6) << 8),
                    sx - 8, sy - 8, 16, 16);
          else
            draw[0](canvas, work->sheet + 0x3e80 + i % 3 * 576, sx - 12,
                    sy - 12, 24, 24);
        }
      }
    }

    Func_08158ce0(8, 8);
    Func_081434f8();
    work->transferPending = 1;
    WaitFrames(1);
  }

  Scheduler_RemoveCallback(Func_08143000);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  Func_08143bb8();
}

// 2026-10-25, with the Makefile's options (-mthumb-slow-loads): differ 14 of
// 303; a greedy device search found nothing lower.
// Near miss: listing 303 instructions, draft 303, differ 16. No close ☀️
// twin: the call order is BattleEffect_RunTwoResource's (0.684), whose frame and
// one-blitter setup this shares, but the picture is ⚓️'s own.
// Allocation and frame match (effect spilled to sp+8 because the blitter pair
// holds r6 and r7; the cell buffer through a pointer the build
// rematerializes, so case 2 loads it last and is not cross-jumped with case
// 3). Left are load and store orders: effect's spill before the heap base in
// the prologue, the slot 104 argument last before Func_08196404, both BLDALPHA
// words before their register address, and the fill's IwramFillWords address
// before 0x4000. Measured: f18284c.c's blend pin with its empty asm on both
// stores: 16, but the asm's estimated four bytes move the pool after
// BattleMotion_ApproachTargetFar one instruction early; the pins alone, or
// one-pass blocks: 24 to 58.
// 2026-10-15 (fx-n3): rewritten for reading on struct BattleEffectWork, with
// the resource ids named from directory.s (0x10b GreyCrescentSheet, 0x10c
// StarCrescentSheet, 0x150 MarsDjinnSheet) and the blitter's flags and mode
// spelled out; still 24. The pooled store on Echo Cut's blend alone: 16
// (Human Hunt's plain, Echo Cut's pinned and held; on Human Hunt's too, or
// the pin alone on either: 16 or 24, all nine combinations measured). What is
// left at 16: effect's spill before the heap base, the slot 104 argument last,
// Human Hunt's blend value before its address, the pool after
// BattleMotion_ApproachTargetFar one instruction early, and the fill's
// IwramFillWords address before 0x4000. alchemy permute (10 min, 2 jobs,
// 7,474 candidates): nothing below 16 (its weighted best differs 24). The
// crescent's second half as cells + 0x4000 or the fixed 0x02014000: 16.
// 2026-10-18: alchemy-devtools permute (6 min, 2 jobs, seed 31): no candidate below
// the draft.
#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_effect.h"
#include "battle_effect_work.h"

// The Lost Age's heap slots, by byte offset into Ram_HeapSlots.
#define HEAP_SLOT_BLITTER 104

// The transfer callback runs at this order, where ☀️'s runs at 0x480.
#define TRANSFER_CALLBACK_ORDER 0xc80

// FAKEMATCH: ⚓️ loads a pooled halfword before it starts the register's
// address; the plain store starts the address first. Only a halfword register
// loads from the pool, so the value is pinned to the r3 it already takes, and
// the barrier holds r2 until it is loaded, as in f18284c.c.
#define REG_WRITE16_POOLED(reg, value)                                         \
  {                                                                            \
    register u16 pooled_ __asm__("r3") = (value);                              \
    __asm__("" : "+l"(pooled_) : : "r2");                                      \
    (reg) = pooled_;                                                           \
  }

// ⚓️'s blitter modes count the other way from ☀️'s COMPOSITE_* modes.
enum { BLIT_ADDITIVE, BLIT_MAX, BLIT_TRANSPARENT, BLIT_COPY };

struct EffectPosition;

extern char ResourceId_GreyCrescentSheet;
extern char ResourceId_StarCrescentSheet;
extern char ResourceId_MarsDjinnSheet;

void BattleEffect_StartTiledCanvas(s32 bgControl);
void Func_08196404(s32 slot, s32 widthShift, s32 heightShift, s32 flags,
                   s32 mode);
void *Resource_GetTableEntry(s32 id);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
s32 Scheduler_RemoveCallback(void (*callback)(void));
void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
void EffectPosition_ProjectBattler(
    s32 id, struct EffectPosition *position);
void Audio_PlayCue(s32 cue);
void Func_081180e8(s32 phase);
void BattleEffect_StartActorMotion(s32, s32, s32, s32, s32, s32);
void Func_08158ce0(s32 randomMask, u32 shakeRange);
void ObjectGroup_TickMemberTimers(void);
void WaitTicks(s32 ticks);
void Runtime_ReleaseHeapBlock(s32 slot);
void BattleEffect_EndCanvasLayer(void);
void BattleEffect_ProcessPendingGraphicsTransfer(void);

// The crescent slash of Human Hunt (kind 437, mode 0) and Echo Cut (kind 390,
// mode 1). The caster closes in on the target; from frame 8 a crescent is cut
// in five steps of three frames: the star crescent, a blank frame, the two
// halves of the grey crescent and the star crescent's last cell. Echo Cut
// cuts it upside down and higher, and throws the target into the air at
// frame 12. Human Hunt loads the Mars Djinn's colours into the background
// palette and blends its slash more faintly.
void Func_0818c7b8(struct BattleEffectArgument *effect, s32 mode) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  u8 *cells = Ram_MapCellBuffer;
  // The blitter pair, as a two-word array one DImode pseudo: it takes r6 and
  // r7, the second unused, which is why the native pushes r7 and spills
  // effect.
  DrawRectangle draw[2];
  s32 flags;
  s32 frame;
  s32 position[3];

  // Set up the canvas and a clipped MAX blitter, mirrored to face the target
  // and flipped upside down for Echo Cut.
  BattleEffect_StartTiledCanvas(0);
  if (mode == 0) {
    flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y;
    if (effect->side != 0)
      flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_X;
  } else {
    flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP_Y;
    if (effect->side != 0)
      flags = COMPOSITE_CLIP_X | COMPOSITE_CLIP_Y | COMPOSITE_FLIP;
  }
  Func_08196404(HEAP_SLOT_BLITTER, 7, 7, flags, BLIT_MAX);
  draw[0] = (DrawRectangle)Ram_HeapSlots->unk64[1];

  // Load both crescents and set the blend.
  Resource_LoadAndDecompress((s32)&ResourceId_GreyCrescentSheet, cells, 1, 1);
  Resource_LoadAndDecompress((s32)&ResourceId_StarCrescentSheet, work->sheet, 1,
                             0);
  if (mode == 0) {
    Iwram_CopyWords((void *)0x05000000,
                    Resource_GetTableEntry((s32)&ResourceId_MarsDjinnSheet),
                    128);
    REG_BLDALPHA = 0x810;
    work->transferMode = 2;
    work->transferValue = 50;
  } else {
    // FAKEMATCH: Echo Cut's blend takes the pooled store (24 to 16: its value
    // and the next pool move into place); on Human Hunt's store as well, or
    // the pin alone on either, it stays 16 or 24.
    REG_WRITE16_POOLED(REG_BLDALPHA, 0x1010);
    work->transferMode = 2;
    work->transferValue = 75;
  }
  Scheduler_AddOrUpdateCallback(BattleEffect_ProcessPendingGraphicsTransfer, TRANSFER_CALLBACK_ORDER);

  // The caster closes in on the target.
  BattleMotion_ApproachTargetFar(effect->actor, effect->actors[0], 8, 0x40000);

  for (frame = 0; frame != 32; frame++) {
    if (frame == 0)
      EffectPosition_ProjectBattler(
          effect->actors[0], (struct EffectPosition *)position);
    if (frame == 8)
      Audio_PlayCue(212);

    // Frame 12: the hit. Echo Cut throws the target up.
    if (frame == 12) {
      if (mode == 1)
        Func_081180e8(144);
      else
        Func_081180e8(134);
      BattleEffect_StartActorMotion(effect->actors[0], 1, 0x40000, 0x100000,
                                0x40000, 250);
      work->shakeFrames = 4;
    }

    // From frame 8: the slash, one step every three frames.
    if (frame > 7) {
      s32 step = (frame - 8) / 3;
      s32 y = mode != 0 ? 16 : 0;
      s32 x;

      if (effect->side == 0)
        x = position[0] / 2 - 48;
      else
        x = position[0] / 2 - 80;
      if (mode == 0) {
        switch (step) {
        case 0:
          draw[0](canvas, work->sheet, x, y, 120, 120);
          break;
        case 1:
          Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
          break;
        case 2:
          draw[0](canvas, cells, x - 4, y, 128, 128);
          break;
        case 3:
          draw[0](canvas, cells + 0x4000, x - 4, y, 128, 128);
          break;
        case 4:
          draw[0](canvas, work->sheet + 0x3840, x, y, 120, 120);
          break;
        }
      } else {
        switch (step) {
        case 1:
          Iwram_FillWords(canvas, 0x4000, 0x3f3f3f3f);
          break;
        case 0:
          draw[0](canvas, work->sheet, x, y, 120, 120);
          break;
        case 2:
          draw[0](canvas, cells, x - 4, y - 8, 128, 128);
          break;
        case 3:
          draw[0](canvas, cells + 0x4000, x - 4, y - 8, 128, 128);
          break;
        case 4:
          draw[0](canvas, work->sheet + 0x3840, x, y, 120, 120);
          break;
        }
      }
    }

    Func_08158ce0(8, 8);
    ObjectGroup_TickMemberTimers();
    work->transferPending = 1;
    WaitTicks(1);
  }

  Scheduler_RemoveCallback(BattleEffect_ProcessPendingGraphicsTransfer);
  Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
  BattleEffect_EndCanvasLayer();
}

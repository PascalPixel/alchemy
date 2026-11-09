#include "types.h"
#include "battle_effect_work.h"
#include "ram_buffer.h"
#include "resource.h"
#include "field_sprite.h"
#include "dma.h"
#include "io_reg.h"

void Animation_ApplyChildArgumentFar(void *, s32);

/// Creates entryCount sprites of one resource in the effect work's object
/// slots, each showing its own animation at the given priority (The Broken
/// Seal's BattleEffect_SpawnObjects).
void BattleEffect_SpawnObjects(s32 entryCount, s32 kind, u32 variant) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  s32 entryIndex = 0;

  if (entryCount == 0)
    return;
  do {
    struct FieldSprite *object =
        (struct FieldSprite *)ResourceObject_CreateFar(kind);

    work->objects[entryIndex] = object;
    if (object != NULL) {
      object->nextFrame = 0;
      Animation_ApplyChildArgumentFar(object, entryIndex);
      ((struct FieldSprite *)work->objects[entryIndex])->priority = variant;
    }
    entryIndex++;
  } while (entryIndex != entryCount);
}

void BattleEffect_DoNothing(void) {}

/// Restarts DMA channel 0 as an H-blank transfer of the effect work's line
/// table into BG2X, one word a line.
// The Broken Seal's BattleEffect_ArmBg2AffineHBlankDma.
void BattleEffect_ArmBg2AffineHBlankDma(void) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  volatile u16 *channel = REG_DMA0;
  u16 control = channel[5];
  u32 io;

  channel[5] = control & 0xc5ff;
  channel[5] &= 0x7fff;
  io = 0x80;
  (void)channel[5];
  Dma_Set(work->bg2xLines, (void *)((io << 19) + 0x28),
          DMA_HBLANK_WORD_PER_LINE, (volatile u32 *)channel);
}

/// BattleEffect_ArmBg2AffineHBlankDma for BG1HOFS and BG1VOFS: the line table moves layer 1.
void BattleEffect_ArmBg1HBlankDma(void) {
  struct BattleEffectWork *work =
      (struct BattleEffectWork *)Ram_HeapSlots->battleFxWork[0];
  volatile u16 *channel = REG_DMA0;
  u16 control = channel[5];
  u32 io;

  channel[5] = control & 0xc5ff;
  channel[5] &= 0x7fff;
  io = 0x80;
  (void)channel[5];
  Dma_Set(work->bg2xLines, (void *)((io << 19) + 0x14),
          DMA_HBLANK_WORD_PER_LINE, (volatile u32 *)channel);
}

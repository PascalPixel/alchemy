#include "types.h"
#include "task.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"

#define BG_PLTT_COLOR(bank, color)                                             \
  (*(volatile u16 *)(0x05000000 + ((bank) * 16 + (color)) * 2))

void BattleEffect_ArmBg0HBlankDma(void);
void Func_08038348(s32 first, s32 second);

/// Stops line scrolling and restores background palette 15, as The
/// Broken Seal's FieldPsynergy_RestoreMindReadPalette does.
void Func_080dcadc(void) {
  u8 *work = Ram_HeapSlots->windowTiles;

  Task_Remove((TaskFunc)BattleEffect_ArmBg0HBlankDma);
  BG_PLTT_COLOR(15, 1) = 0x7fff;  // White.
  BG_PLTT_COLOR(15, 3) = 0;       // Black.
  BG_PLTT_COLOR(15, 11) = 0x294a; // Gray (10).
  BG_PLTT_COLOR(15, 12) = 0x5294; // Gray (20).
  Func_08038348(gSequenceParentWork.unk220[5], gSequenceParentWork.unk220[6]);
  work[4] = 0;
}

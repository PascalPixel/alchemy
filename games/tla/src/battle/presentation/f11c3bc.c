#include "types.h"
#include "scene.h"
#include "battle_message.h"
#include "battle_escape.h"
#include "battle_display.h"
#include "battle_target.h"
#include "fixed_math.h"
#include "battle_side.h"
#include "system.h"
#include "battle_system.h"
#include "dma.h"
void *Unit_Get(s32 owner);

struct BattleSortedUnitEntry {
  u16 unitId;
  u16 unk02;
  s16 value;
  s16 width;
  s16 mode;
  s16 priority;
  u8 unk0c[4];
};

s32 AutoBattle_BuildActions(
    struct BattleSortedUnitEntry *entries) {
  struct BattleSortedUnitEntry swap;
  u16 unitIds[14];
  s32 firstCount;
  s32 count = 0;
  s32 secondCount;
  s32 priorityRange;
  s32 index;
  struct BattleSortedUnitEntry *entry;
  struct BattleUnit *unit;

  firstCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, unitIds);
  for (index = 0; index != 4; index++)
    Unit_Get(index);
  for (index = 0; index < firstCount; index++) {
    s32 unitId = unitIds[index];

    unit = Unit_Get(unitId);
    entry = &entries[index];
    entry->unitId = unitId;
    entry->value = unit->agi;
    entry->width = 0;
    entry->mode = 0;
    entry->priority = 0x80;
    count++;
  }
  secondCount = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, unitIds);
  entry = &entries[firstCount];
  priorityRange = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, NULL);
  for (index = 0; index < secondCount; index++) {
    s32 unitId = unitIds[index];

    unit = Unit_Get(unitId);
    entry->unitId = unitId;
    entry->value = unit->agi >> 1;
    if (entry->value != 0)
      entry->value += (u32)(Random16() * unit->agi) >> 16;
    entry->width = 0;
    entry->mode = 0;
    entry->priority = (u32)(Random16() * priorityRange) >> 16;
    count++;
    entry++;
  }
  for (index = count - 2; index > 0; index--) {
    s32 swaps = 0;
    s32 pos;

    for (pos = count - 1; pos > 0; pos--) {
      if (entries[pos].value > entries[pos - 1].value) {
        Dma_Set(&entries[pos], &swap, DMA_COPY_WORDS(4), REG_DMA3);
        Dma_Set(&entries[pos - 1], &entries[pos], DMA_COPY_WORDS(4), REG_DMA3);
        Dma_Set(&swap, &entries[pos - 1], DMA_COPY_WORDS(4), REG_DMA3);
        swaps++;
      }
    }
    if (swaps == 0)
      break;
  }
  return count;
}

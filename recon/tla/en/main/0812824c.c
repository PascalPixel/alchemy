// Draft of Func_0812824c, The Broken Seal's BattleEnemy_RecordDefeat
// (games/tbs/src/battle/battle/spoils.c), with the battle work from its heap
// slot. This game also keeps the last two defeated enemies (their ids and
// Func_08128228's first digit of their names, alternating slots, with a count
// up to 2), sets the seen flag only for enemy ids below 0x200, and compares
// item ids signed.
// Near miss, differ 151 of 300 (2026-10-19, first draft). The shape matches
// the native's; what is left is mostly register choice: the native keeps the
// enemy-class search's index in r0 and the address of the unit's enemy id in
// ip, and builds the definition's field addresses (coins, experience, item,
// drop chance) in r3 and r1 where this draft takes r4 and r0, which shifts
// the registers of everything after. Kept: the slot as the value of its
// assignment to the s8 field (differ 165 to 153), BattleRandomFar signed (2
// fewer).
#include "types.h"
#include "battle_unit.h"
#include "ram_buffer.h"

struct EnemyDefinition {
  u8 unk00[0x42];
  u16 coins;
  s16 item;
  s16 itemChance;
  u16 experience;
};

struct DefeatSpoils {
  s32 coins;
  s32 experience;
  s32 defeated;
  u16 items[4];
  u16 lastEnemyIds[2];
  u8 lastDigits[2];
  s8 lastSlot;
  s8 lastCount;
};

struct DefeatBattleWork {
  u8 unk00[0x10];
  u16 enemyClasses[6];
  u8 unk1c[0x20];
  u16 firstDefeated;
  u16 defeatState;
  u8 unk40[0x510];
  struct DefeatSpoils spoils;
};

struct BattleUnit *UnitGet(s32 unitId);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct EnemyDefinition *Enemy_GetDefinitionFar(s32 id);
s32 BattleRandomFar(void);
u32 BattleSpoils_RankItem(u32 value);
u32 Random16(void);
s32 Func_08128228(u8 *s);

static __inline__ s32 BattleEnemy_CalculateCoinReward(struct BattleUnit *unit,
                                                      u16 *reward) {
  s32 rollIndex;
  s32 bonus = 0;
  s32 base;
  s32 minimumBonus;

  for (rollIndex = 0; rollIndex < (u8)(unit->level / 10U) + 1; rollIndex++)
    bonus += ((Random16() * 6) >> 16) + 1;
  base = *reward;
  minimumBonus = base * 3 / 10;
  if (bonus < minimumBonus)
    bonus = minimumBonus;
  return bonus + base;
}

static __inline__ s32
BattleEnemy_CalculateExperienceReward(struct BattleUnit *unit, u16 *reward) {
  s32 rollIndex;
  s32 bonus = 0;
  s32 base;
  s32 minimumBonus;

  for (rollIndex = 0; rollIndex < (u8)(unit->level / 10U) + 1; rollIndex++)
    bonus += ((Random16() * 4) >> 16) + 1;
  base = *reward;
  minimumBonus = base * 3 / 10;
  if (bonus < minimumBonus)
    bonus = minimumBonus;
  return bonus + base;
}

s32 Func_0812824c(s32 unitId, s32 djinnBonus) {
  struct BattleUnit *unit;
  struct EnemyDefinition *enemy;
  struct DefeatBattleWork *battle;
  struct DefeatSpoils *spoils;
  s32 itemOrClassIndex;
  s32 dropShift;
  s32 classSlot;
  s32 lowestItemPriority;
  s32 lowestSlot;
  s32 itemPriority;
  s32 slot;

  unit = UnitGet(unitId);
  battle = (struct DefeatBattleWork *)Ram_HeapSlots->battleWork;
  spoils = &battle->spoils;
  if (unitId >= 0 && unitId < 8)
    return -1;
  if (unit->classIndex != 0)
    return -2;
  slot = spoils->lastSlot = (spoils->lastSlot + 1) % 2;
  spoils->lastEnemyIds[slot] = unit->enemyId;
  spoils->lastDigits[slot] = Func_08128228((u8 *)unit);
  if (spoils->lastCount <= 1)
    spoils->lastCount++;
  classSlot = 0;
  itemOrClassIndex = 0;
  if (battle->enemyClasses[0] != unit->enemyId) {
    do {
      if (++itemOrClassIndex > 5)
        break;
    } while (battle->enemyClasses[itemOrClassIndex] != unit->enemyId);
  }
  if (itemOrClassIndex != 6)
    classSlot = itemOrClassIndex;
  if (battle->defeatState != 2) {
    if (classSlot < battle->firstDefeated)
      battle->firstDefeated = classSlot;
    if (spoils->defeated)
      battle->defeatState = 1;
  }
  spoils->defeated++;
  if (GameFlag_Test(0x173))
    return 0;
  if (unit->enemyId < 0x200)
    GameFlag_SetBit(unit->enemyId + 0x600);
  enemy = Enemy_GetDefinitionFar(unit->enemyId);
  if (djinnBonus != 0) {
    if (enemy->coins != 0)
      spoils->coins += BattleEnemy_CalculateCoinReward(unit, &enemy->coins);
    if (enemy->experience != 0)
      spoils->experience +=
          BattleEnemy_CalculateExperienceReward(unit, &enemy->experience);
  } else {
    spoils->coins += enemy->coins;
    spoils->experience += enemy->experience;
  }
  if (enemy->item == 0 || enemy->itemChance == 0)
    return 0;
  itemOrClassIndex = 0;
  if (spoils->items[0] != enemy->item) {
    do {
      if (++itemOrClassIndex > 3)
        break;
    } while (spoils->items[itemOrClassIndex] != enemy->item);
  }
  if (itemOrClassIndex != 4)
    return 0;
  dropShift = enemy->itemChance;
  if (djinnBonus != 0)
    dropShift -= 2;
  if (dropShift < 0)
    dropShift = 0;
  if ((0x20000 >> dropShift) > (BattleRandomFar() & 0xffff)) {
    lowestSlot = -1;
    lowestItemPriority = 0x40000000;
    for (itemOrClassIndex = 0; itemOrClassIndex <= 3; itemOrClassIndex++) {
      itemPriority = BattleSpoils_RankItem(spoils->items[itemOrClassIndex]);
      if (itemPriority < lowestItemPriority) {
        lowestItemPriority = itemPriority;
        lowestSlot = itemOrClassIndex;
      }
    }
    if ((s32)BattleSpoils_RankItem(enemy->item) > lowestItemPriority)
      spoils->items[lowestSlot] = enemy->item;
  }
  return 0;
}

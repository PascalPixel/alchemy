// Draft of Func_08127cd4, The Broken Seal's BattleFormation_BuildEnemyList
// (games/tbs/src/battle/battle/formation.c), with the battle work from its
// heap slot. This game raises an enemy's levels through Func_0812786c with the
// level adjustment, or else, when flag 0x173 is clear and flag 46 set, its
// base stats through Func_08127a0c, and sets the battle work's byte at 0x868
// for enemies 101 to 103 while flag 0x173 is clear.
// Near miss, differ 45 of 456. The enemy-id range is a switch on 101 to 103,
// which gives the native's cmp r0, #101; blt (as a nested if it was
// cmp r0, #100; ble). Left: spill-slot and scheduling order in the setup (the
// work's summon count cleared after the level adjustment's slot, the record
// copied to the stack before the loop's other setup); in the default case the
// native counts the copies down in a copy of the count (adds r1, r3, #0)
// where this draft counts the count's own register down, one instruction
// fewer. Tried there: the count in a local first (45), a loop counting the
// count down (67). Kept: freeSpriteSlots declared before enemyCount (their
// stack slots).
#include "types.h"
#include "battle.h"
#include "battle_calc.h"
#include "battle_formation.h"
#include "battle_summon.h"
#include "battle_unit.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "system.h"

struct FormationBattleWork {
  u8 unk00[0x3c];
  u16 firstDefeated;
  u16 defeatState;
  u8 summonCount;
  u8 unk41;
  u8 battleType;
  u8 unk43[0x825];
  u8 unk868;
};

struct BattleUnit *Unit_Get(s32 unitId);
s32 GameFlag_Test(s32 flag);
s32 BattleFormation_SelectLevelMatchedCandidate(s32 *outMargin);
s32 BattleEnemy_ClaimNameLetter(s32 no, s32 n);
void Func_0812786c(s32 unitId, s32 levels);
void Func_08127a0c(s32 unitId);

s32 Func_08127cd4(s32 recordId) {
  u16 *enemyClasses;
  struct FormationBattleWork *battle;
  struct BattleFormationRecord *record;
  s32 freeSpriteSlots;
  s32 enemyCount;
  s32 levelAdjustment;
  u16 listBuffer[14];
  s32 extraCounts[5];
  s32 memberCounts[5];
  s32 memberOrder[5];
  s32 changed;
  s32 additionalRoom;
  s32 spriteSlots;
  s32 fittingCount;
  s32 classId;
  s32 memberOrCopyIndex;
  s32 remainingKinds;
  s32 copies;
  u32 memberOrEnemyIndex;
  s32 memberIndex;

  battle = (struct FormationBattleWork *)Ram_HeapSlots->battleWork;
  levelAdjustment = 0;
  battle->summonCount = 0;
  enemyClasses = listBuffer;
  if (GameFlag_Test(0x173))
    recordId = BattleFormation_SelectLevelMatchedCandidate(&levelAdjustment);
  if ((u32)recordId >= 660)
    recordId = 1;
  record = &gBattleFormation_Records[recordId];
  for (memberIndex = 0;
       memberIndex <= 4 && record->minimumCounts[memberIndex] == 0;
       memberIndex++)
    ;
  if (memberIndex == 5)
    record = &gBattleFormation_Records[1];

  freeSpriteSlots = 6;
  enemyCount = 0;
  for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4; memberOrEnemyIndex++) {
    if (record->minimumCounts[memberOrEnemyIndex] != 0) {
      s32 requiredSlots =
          2 - (BattleClass_IsSmall(record->memberIds[memberOrEnemyIndex] +
                                     8) != 0);

      freeSpriteSlots -=
          requiredSlots * record->minimumCounts[memberOrEnemyIndex];
    }
  }

  for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4; memberOrEnemyIndex++) {
    s32 minimum;
    s32 maximum;

    memberCounts[memberOrEnemyIndex] = minimum =
        record->minimumCounts[memberOrEnemyIndex];
    maximum = record->maximumCounts[memberOrEnemyIndex];
    additionalRoom = maximum - minimum;
    if (additionalRoom > 0) {
      s32 memberSlots =
          2 - (BattleClass_IsSmall(record->memberIds[memberOrEnemyIndex] +
                                     8) != 0);

      fittingCount = freeSpriteSlots / memberSlots;
      if (fittingCount < additionalRoom)
        additionalRoom = fittingCount;
      extraCounts[memberOrEnemyIndex] =
          ((additionalRoom + 1) * Random16()) >> 16;
    } else {
      extraCounts[memberOrEnemyIndex] = 0;
    }
  }

  do {
    changed = 0;
    for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4;
         memberOrEnemyIndex++) {
      classId = record->memberIds[memberOrEnemyIndex] + 8;
      if (extraCounts[memberOrEnemyIndex] != 0) {
        spriteSlots = 2 - (BattleClass_IsSmall(classId) != 0);
        if (spriteSlots > freeSpriteSlots) {
          extraCounts[memberOrEnemyIndex] = 0;
        } else {
          memberCounts[memberOrEnemyIndex]++;
          extraCounts[memberOrEnemyIndex]--;
          freeSpriteSlots -= spriteSlots;
          changed = 1;
        }
      }
    }
  } while (changed);

  battle->battleType = record->unk14[0];
  switch (battle->battleType) {
  case 0:
    for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4; memberOrEnemyIndex++)
      memberOrder[memberOrEnemyIndex] = memberOrEnemyIndex;
    for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 9;
         memberOrEnemyIndex++) {
      s32 first = (Random16() * 5) >> 16;
      s32 second = (Random16() * 5) >> 16;
      s32 swap = memberOrder[first];

      memberOrder[first] = memberOrder[second];
      memberOrder[second] = swap;
    }
    for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4;
         memberOrEnemyIndex++) {
      s32 copyIndex;
      s32 slot = memberOrder[memberOrEnemyIndex];

      for (copyIndex = 0; copyIndex < memberCounts[slot]; copyIndex++)
        enemyClasses[enemyCount++] = record->memberIds[slot] + 8;
    }
    break;
  case 1:
    for (;;) {
      remainingKinds = 0;
      for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4;
           memberOrEnemyIndex++) {
        if (memberCounts[memberOrEnemyIndex] != 0)
          memberOrder[remainingKinds++] = memberOrEnemyIndex;
      }
      if (remainingKinds == 0)
        break;
      memberOrCopyIndex = memberOrder[(Random16() * remainingKinds) >> 16];
      enemyClasses[enemyCount++] = record->memberIds[memberOrCopyIndex] + 8;
      memberCounts[memberOrCopyIndex]--;
    }
    break;
  default:
    for (memberOrEnemyIndex = 0; memberOrEnemyIndex <= 4;
         memberOrEnemyIndex++) {
      for (memberOrCopyIndex = 0;
           memberOrCopyIndex < memberCounts[memberOrEnemyIndex];
           memberOrCopyIndex++)
        enemyClasses[enemyCount++] = record->memberIds[memberOrEnemyIndex] + 8;
    }
    break;
  }
  enemyClasses[enemyCount] = 0;
  battle->firstDefeated = 6;
  battle->defeatState = 0;
  for (memberOrEnemyIndex = 128; memberOrEnemyIndex < 134;
       memberOrEnemyIndex++)
    Iwram_ClearWords(Unit_Get(memberOrEnemyIndex), BATTLE_UNIT_SIZE);

  memberOrEnemyIndex = 0;
  if (enemyClasses[0] == 0)
    goto assigned;
assign_enemy:
  // Repeated enemy classes get distinct suffixes within this encounter.
  {
    s32 suffixCode = BattleEnemy_ClaimNameLetter(enemyClasses[memberOrEnemyIndex], 1);
    struct BattleUnit *unit;
    s32 enemyId;

    if (suffixCode & 0x8000)
      BattleEnemy_LetterFirstOfClass(enemyClasses[memberOrEnemyIndex]);
    BattleUnit_SetFar(memberOrEnemyIndex + 128,
                     enemyClasses[memberOrEnemyIndex], suffixCode & 0x7fff);
    unit = Unit_Get(memberOrEnemyIndex + 128);
    if (levelAdjustment != 0)
      Func_0812786c(memberOrEnemyIndex + 128, levelAdjustment);
    else if (!GameFlag_Test(0x173) && GameFlag_Test(46))
      Func_08127a0c(memberOrEnemyIndex + 128);
    battle->unk868 = 0;
    enemyId = unit->enemyId;
    switch (enemyId) {
    case 101:
    case 102:
    case 103:
      if (!GameFlag_Test(0x173))
        battle->unk868 = 1;
      break;
    }
  }
  memberOrEnemyIndex++;
  if (memberOrEnemyIndex > 5)
    goto assigned;
  if (enemyClasses[memberOrEnemyIndex] != 0)
    goto assign_enemy;
assigned:
  return memberOrEnemyIndex;
}

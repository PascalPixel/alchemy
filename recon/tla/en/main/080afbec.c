// Near miss, differ 2 of 219 (one reordered pair), 2026-10-18: The Broken
// Seal's BattleUnit_Set (games/tbs/src/game/game.c) on this game's enemy
// records: fills battle unit `unitId` (0x80 to 0x86) with enemy `enemyId`
// and its suffix number; enemies 9 to 66 and 369 to 377 fight on side 2.
// Message 1068 ("???", then the enemy names) is called MsgEnemyName here as
// in The Broken Seal; the catalogs leave it unnamed, so adopting needs that
// name in the six PO files. Remaining: before storing the enemy id the
// native reloads it from the stack (add r2, sp, #8; ldrh) before it forms
// the field address (movs r1, #165); this forms the address first. A
// temporary behind an empty asm moves the add only (differ 2), an asm on a
// promoted copy extends it (differ 12 to 30).
#include "types.h"
#include "battle_unit.h"
#include "iwram_call.h"

struct EnemyRecord {
  u8 unk00[15];
  u8 level;
  u16 hp;
  u16 pp;
  u16 atk;
  u16 def;
  u16 agi;
  u8 luck;
  u8 turns;
  u8 hpRegen;
  u8 ppRegen;
  u16 items[4];
  u8 counts[4];
  u8 unk2a[34];
};

extern const u8 gEnemy_DefinitionTable[];
extern u8 MsgEnemyName[];

struct BattleUnit *Unit_Get(s32 unitId);
void UiText_DecodeMessageFar(s32 message, u16 *buffer, s32 length);
void Unit_CalcBaseElements(s32 owner, struct BattleElementStats *elements);
void Unit_Calculate(s32 unitId);

s32 Func_080afbec(s32 unitId, s32 enemyId, s32 suffix) {
  struct BattleUnit *unit;
  const struct EnemyRecord *enemy;
  u16 name[15];
  s32 i;
  u32 index;
  s32 q;
  s32 count;

  index = enemyId - 8;
  if (unitId <= 127)
    return 0;
  if (unitId > 134)
    return 0;
  if (index > 992)
    return 0;
  unit = Unit_Get(unitId);
  {
    /* FAKEMATCH: the entry taken first puts its pool load ahead of the size,
     * where the native issues it (differ 4 without) */
    s32 (*clear)(void *destination, s32 size) = Iwram_ClearWords;

    clear(unit, BATTLE_UNIT_SIZE);
  }
  if (index >= 380)
    index = 0;
  enemy = (const struct EnemyRecord *)&gEnemy_DefinitionTable[index * 76];
  unit->level = enemy->level;
  unit->baseHp = enemy->hp;
  unit->hp = enemy->hp;
  unit->maxHp = enemy->hp;
  unit->basePp = enemy->pp;
  unit->pp = enemy->pp;
  unit->maxPp = enemy->pp;
  unit->hpGauge = 0x4000;
  unit->ppGauge = 0x4000;
  unit->baseAttack = enemy->atk;
  unit->baseDefense = enemy->def;
  unit->baseAgility = enemy->agi;
  unit->baseLuck = enemy->luck;
  unit->baseActionCount = enemy->turns;
  unit->baseHpRegen = enemy->hpRegen;
  unit->basePpRegen = enemy->ppRegen;
  UiText_DecodeMessageFar(index + (s32)MsgEnemyName, name, 15);
  for (i = 0; i < 14 && name[i] != 0; i++)
    unit->name[i] = name[i];
  if (suffix <= 8) {
    unit->name[i] = '1' + suffix;
    i++;
  }
  unit->name[i] = 0;
  unit->name[14] = 0;
  count = 0;
  for (i = 0; i < 4; i++) {
    if (enemy->items[i] != 0) {
      for (q = 0; q < enemy->counts[i]; q++) {
        if (count < 15)
          unit->inventory[count++] = enemy->items[i];
      }
    }
  }
  unit->rewards = 0;
  unit->classIndex = 0;
  unit->enemyId = enemyId;
  Unit_CalcBaseElements(unitId, unit->baseElement);
  Unit_Calculate(unitId);
  unit->side = 1;
  if ((u16)(unit->enemyId - 9) <= 57)
    unit->side = 2;
  if ((u16)(unit->enemyId - 369) <= 8)
    unit->side = 2;
  return 1;
}

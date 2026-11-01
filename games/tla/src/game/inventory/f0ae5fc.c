#include "types.h"
#include "main_routine_work.h"
#include "battle_unit.h"

void *Unit_Get(s32 owner);
void Unit_Calculate(s32 unitId);
void Func_08038348(s32 a, s32 b);

void Func_080ae5fc(u8 *save) {
  struct MainRoutineWork *party = (struct MainRoutineWork *)(save + 0x240);
  struct BattleUnit *units = (struct BattleUnit *)(save + 0x520);
  u8 unused[16]; // Unused: gives the stack frame its 16 bytes.
  s32 i;
  s32 count;

  gMainRoutineWork.paletteGlow[0] = party->paletteGlow[0];
  gMainRoutineWork.paletteGlow[1] = party->paletteGlow[1];
  gMainRoutineWork.eventMode = party->eventMode;
  gMainRoutineWork.battleFxEnabled = party->battleFxEnabled;
  gMainRoutineWork.unk244[6] = party->unk244[6];
  Func_08038348(gMainRoutineWork.paletteGlow[0],
                gMainRoutineWork.paletteGlow[1]);
  count = 8;
  for (i = 0; i < count; i++) {
    struct BattleUnit *unit = Unit_Get(i);
    struct BattleUnit *saved = &units[i];

    unit->baseHp = saved->baseHp;
    unit->basePp = saved->basePp;
    unit->hpGauge = unit->baseHp;
    unit->ppGauge = unit->basePp;
    unit->baseAttack = saved->baseAttack;
    unit->baseDefense = saved->baseDefense;
    unit->baseAgility = saved->baseAgility;
    unit->baseLuck = saved->baseLuck;
    unit->level = saved->level;
    unit->experience = saved->experience;
    Unit_Calculate(i);
  }
  gMainRoutineWork.coins = party->coins;
}

#include "types.h"
#include "call.h"
#include "owner_state.h"
#include "main_routine_work.h"

void Unit_Calculate(s32 owner);

void Func_02000704(s32 levels) {
  s16 owners[16];
  s32 count = Value1((s32 (*)())Party_ListActiveOwners, (s32)owners);
  s32 i;

  for (i = 0; i < count;) {
    u16 owner = owners[i++];

    Unit_LevelUpTo(owner,
                   ((struct BattleUnit *)Unit_Get(owner))->level + levels);
    Unit_Calculate(owner);
  }
}

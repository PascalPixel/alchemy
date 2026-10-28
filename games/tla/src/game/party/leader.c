#include "types.h"
#include "main_routine_work.h"
#include "owner_state.h"

s32 Party_AddActiveOwner(s32 owner);
s32 Party_RemoveActiveOwner(s32 owner);

void Party_SetLeaderJenna(void) { gMainRoutineWork.currentOwner = 5; }

void Party_SetLeaderFelix(void) {
  gMainRoutineWork.currentOwner = 4;
  Party_AddActiveOwner(4);
  Party_RemoveActiveOwner(5);
  Party_RemoveActiveOwner(6);
}

void GameFlag_SetBit(s32 flag);
void Func_080ae358(s32 mode);
void Unit_Calculate(s32 unit);

void Func_080ae118(void) {
  s32 i;
  s32 count;

  for (i = 10; i < 12; i++) {
    GameFlag_SetBit(i + 67);
    Djinn_AddToOwner(7, 1, i - 1);
    Djinn_Activate(7, 1, i - 1);
  }
  Func_080ae358(0);
  count = Party_CountActiveOwners();
  for (i = 0; i < count; i++)
    Unit_Calculate(gMainRoutineWork.activeOwners[i]);
}

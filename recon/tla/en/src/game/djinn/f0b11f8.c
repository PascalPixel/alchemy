#include "types.h"
#include "owner_state.h"
#include "main_routine_work.h"

extern u16 gData_020023c4[];

void Unit_Calculate(s32 owner);
void GameFlag_ClearBit(s32 flag);

void Func_080b11f8(void) {
  u16 *src = gData_020023c4;
  s32 owner;

  if (*src++ == 0x6774) {
    for (owner = 4; owner <= 7; owner++) {
      u16 *dst = ((struct OwnerInventoryState *)Unit_Get(owner))->inventory;
      s32 i;

      for (i = 14; i >= 0; i--)
        *dst++ = *src++;
      Unit_UpdateClass(owner);
      Unit_Calculate(owner);
    }
    gMainRoutineWork.packedEvents[0] = *src++;
    gMainRoutineWork.packedEvents[1] = *src++;
#ifdef UBFIX
    gMainRoutineWork.activeOwners[0] = src[0];
    gMainRoutineWork.activeOwners[1] = src[0] >> 8;
    gMainRoutineWork.activeOwners[2] = src[1];
    gMainRoutineWork.activeOwners[3] = src[1] >> 8;
#else
    {
      // UB: the party order's four bytes are stored as two halfwords.
      u16 *owners = (u16 *)gMainRoutineWork.activeOwners;

      /* FAKEMATCH(temporary): both party-order halfwords stored through one
       * cursor over activeOwners; as stores through
       * *(u16 *)&gMainRoutineWork.activeOwners[0] and [2], 4 of 58
       * instructions differ in tla-en. */
      *owners++ = *src++;
      *owners = *src;
    }
#endif
    src = gData_020023c4;
    *src = 0;
  }
  GameFlag_ClearBit(0x9ff);
}

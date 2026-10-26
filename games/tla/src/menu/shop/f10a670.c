#include "types.h"
#include "game_flags.h"
#include "edition.h"

extern u16 gData_0810c3f4[][33];
void Item_AdjustCounterFar(s32 item, s32 amount);

void EventTable_ApplyRowAbilities(s32 rowNo) {
  s32 flag = rowNo + 0x400;
  s32 i;

  if (GameFlag_Test(flag) == 0) {
    GameFlag_SetBit(flag);
#if LANGUAGE == LANGUAGE_JAPANESE
    // Japanese stops at the row's first empty entry.
    for (i = 0; i <= 7; i++) {
      s16 value = gData_0810c3f4[rowNo][24 + i];

      if (value == 0)
        break;
      Item_AdjustCounterFar(value, 1);
    }
#else
    for (i = 0; i <= 7; i++) {
      s16 value = gData_0810c3f4[rowNo][24 + i];

      if (value != 0)
        Item_AdjustCounterFar(value, 1);
    }
#endif
  }
}

/* Remaining difference: the native loads the table between the shift and the add of row * 33. */
#include "types.h"
#include "game_flags.h"

extern u16 gData_0810c3f4[][33];
void Item_AdjustCounterFar(s32 item, s32 amount);

void EventTable_ApplyRowAbilities(s32 row_no)
{
    s32 flag = row_no + 0x400;
    s32 i;

    if (GameFlag_Test(flag) == 0) {
        GameFlag_SetBit(flag);
        for (i = 0; i <= 7; i++) {
            s16 value = gData_0810c3f4[row_no][24 + i];

            if (value != 0)
                Item_AdjustCounterFar(value, 1);
        }
    }
}

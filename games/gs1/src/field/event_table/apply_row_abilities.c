#include "types.h"
#include "scene.h"
#include "abi/field/event_table/apply_row_abilities.h"

extern s16 gRom[][33];

s32 GameFlag_IsSet(s32 flag_no);
void GameFlag_Set(s32 flag_no);
void Ability_GetMaximum(s32 value, s32 enabled);

void EventTable_ApplyRowAbilities(s32 row_no)
{
    s16 *row;
    s16 *entry;
    s32 value;
    s32 count;
    s32 flag;

    flag = row_no + 0x400;
    if (GameFlag_IsSet(flag) == 0) {
        GameFlag_Set(flag);
        count = 0;
        value = gRom[row_no][24];
        if (value != 0) {
            row = gRom[row_no];
            entry = row + 24;
            do {
                Ability_GetMaximum(value, 1);
                count++;
                if (count > 7) {
                    break;
                }
                entry++;
                value = *entry;
            } while (value != 0);
        }
    }
}

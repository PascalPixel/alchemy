#include "TYPES.H"

extern s16 EventTable_AbilityLoadouts[][33];
s32 GameFlag_IsSet(s32);
s32 GameFlag_Set(s32);
void Ability_GetMaximum(s32, s32);

s32 EventTable_GetRowLimit(void)
{
    return 35;
}

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
        value = EventTable_AbilityLoadouts[row_no][24];
        if (value != 0) {
            row = EventTable_AbilityLoadouts[row_no];
            entry = row + 24;
            do {
                Ability_GetMaximum(value, 1);
                count++;
                if (count > 7)
                    break;
                entry++;
                value = *entry;
            } while (value != 0);
        }
    }
}

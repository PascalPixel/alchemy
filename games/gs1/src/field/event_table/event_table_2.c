#include "types.h"
#include "scene.h"

/* field/event_table/get_row_limit.c */
/* types.h maps this semantic owner name back to Field_Run. */

int EventTable_GetRowLimit(void)
{
    return 35;
}

/* field/event_table/apply_row_abilities.c */
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

/* field/event_table/get_row_type.c */
struct EventTable {
    s16 header[32];
    s16 entries[1][33];
};

extern struct EventTable gRom;

s32 EventTable_GetRowType(s32 index)
{
    return gRom.entries[index][0];
}

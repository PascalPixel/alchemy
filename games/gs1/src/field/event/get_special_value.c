#include "types.h"

/* event/get_special_value.c */
struct Fields_0808b248 {
    u8 filler[0x1d6];
    s16 value;
};

extern struct Fields_0808b248 gCell;

s16 Event_GetSpecialValue(void)
{
    /* 作業領域0x1d6の半語を返す。 */
    return gCell.value;
}

/* battle/effects/set_special_from_table.c */
extern s32 RomWords_0809e270[];

void BattleFx_SetSpecialFromTable(s32 arg0, s32 arg1)
{
    s16 *special = (s16 *)&gCell;
    s32 target = special[224];
    s32 *table = RomWords_0809e270;
    s32 entry = *table++;
    s32 result = arg1;

    if (entry != 0 && entry != target) {
        do {
            if (entry & 0x80000000) {
                result = entry & 0xFFFF;
            }
            entry = *table++;
        } while (entry != 0 && entry != target);
    }
    special[235] = (s16)result;
}

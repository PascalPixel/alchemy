#include "types.h"

extern u8 *Data_03001e74;

s32 Func_080b5090(s32 side, s32 group);
void Func_08022768(s32 x, s32 y, s32 width, s32 height, s32 style);

#define BattleLayout_DrawPartyPanels Func_0802281c

s32 BattleLayout_DrawPartyPanels(u16 *argument)
{
    s16 placed_unit;
    s32 party_size;
    s32 unit_offset;
    s32 slot;
    s32 unit_index;
    u8 *battle;

    battle = Data_03001e74;
    party_size = Func_080b5090(1, 0);
    Func_08022768(29 - party_size * 6, 0, 25, 5, 15);

    unit_index = 0;
    if (*argument != 0xff) {
        unit_offset = 0;
loop_unit:
        placed_unit = *(s16 *)(battle + 88);
        slot = 0;
loop_slot:
        if (placed_unit == *(volatile u16 *)((u8 *)argument + unit_offset))
            goto slot_done;
        if (placed_unit == 0xff) {
            slot = 4;
            goto slot_done;
        }
        slot++;
        if (slot <= 3) {
            placed_unit = *(s16 *)(battle + slot * 2 + 88);
            goto loop_slot;
        }
slot_done:
        if (slot != 4) {
            Func_08022768(
                29 - (party_size - slot) * 6,
                0,
                7,
                5,
                14);
        }
        unit_index++;
        if (unit_index <= 3) {
            unit_offset = unit_index * 2;
            if (*(u16 *)((u8 *)argument + unit_offset) != 0xff)
                goto loop_unit;
        }
    }

    return 0;
}

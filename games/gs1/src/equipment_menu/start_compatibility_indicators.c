#include "equipment_menu.h"

extern u8 *Data_03001f2c;
void Func_08009020(void *, s32);
s32 Func_08004278(s32);

void EquipmentMenu_StartCompatibilityIndicators(void) {
    u8 *menu;
    s8 member_index;

    menu = Data_03001f2c;
    if (menu[0x219] != 0) {
        member_index = 0;
        do {
            s32 indicator_offset = member_index * 4 + 276;
            void *indicator = *(void **)(menu + indicator_offset);
            Func_08009020(indicator, 1);
            member_index++;
        } while (member_index < menu[0x219]);
    }
    Func_08004278((s32)&EquipmentMenu_CompatibilityUpdateEntry);
}

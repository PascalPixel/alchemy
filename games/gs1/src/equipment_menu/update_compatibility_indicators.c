#include "equipment_menu.h"
#include "item.h"

extern u8 *Data_03001f2c;
extern s32 Data_03001e40;
void Object_InitializeMode(void *, s32);

void EquipmentMenu_UpdateCompatibilityIndicators(void) {
    u8 *menu;
    s8 member_index;

    menu = Data_03001f2c;
    if ((Data_03001e40 & 31) == 0 && menu[0x219] != 0) {
        member_index = 0;
        do {
            s32 owner_offset = member_index * 2 + 520;

            if (Item_CanOwnerEquip(*(u16 *)(menu + owner_offset),
                                   *(u16 *)(menu + 376) & 0x1FF) != 0) {
                s32 indicator_offset = member_index * 4 + 276;
                void *indicator = *(void **)(menu + indicator_offset);
                Object_InitializeMode(indicator, 3);
            } else {
                s32 indicator_offset = member_index * 4 + 276;
                void *indicator = *(void **)(menu + indicator_offset);
                Object_InitializeMode(indicator, 1);
            }
            member_index++;
        } while (member_index < menu[0x219]);
    }
}

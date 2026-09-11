#include "scene.h"
#include "equipment_menu.h"
#include "item.h"

/* equipment_menu/update_compatibility_indicators.c */
extern u8 *gIw;
extern s32 gIw2;
void Object_InitializeMode(void *, s32);

void EquipmentMenu_UpdateCompatibilityIndicators(void)
{
    u8 *menu;
    s8 member_index;

    menu = gIw;
    if ((gIw2 & 31) == 0 && menu[0x219] != 0) {
        member_index = 0;
        do {
            s32 owner_offset = member_index * 2 + 520;

            if (Item_CanOwnerEquip(*(u16 *)(menu + owner_offset),
                                   *(u16 *)(menu + 376) & 0x1FF) != 0) {
                s32 off = member_index * 4 + 276;
                void *indicator = *(void **)(menu + off);
                Object_InitializeMode(indicator, 3);
            } else {
                s32 off = member_index * 4 + 276;
                void *indicator = *(void **)(menu + off);
                Object_InitializeMode(indicator, 1);
            }
            member_index++;
        } while (member_index < menu[0x219]);
    }
}

/* equipment_menu/start_compatibility_indicators.c */
extern u8 *gIw;
void Object_InitializeMode(void *, s32);
s32 ScheduleCallback(s32);

void EquipmentMenu_StartCompatibilityIndicators(void)
{
    u8 *menu;
    s8 member_index;

    menu = gIw;
    if (menu[0x219] != 0) {
        member_index = 0;
        do {
            s32 off = member_index * 4 + 276;
            void *indicator = *(void **)(menu + off);
            Object_InitializeMode(indicator, 1);
            member_index++;
        } while (member_index < menu[0x219]);
    }
    ScheduleCallback((s32)&EquipmentMenu_CompatibilityUpdateEntry);
}

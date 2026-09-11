#include "scene.h"
#include "abi/equipment_menu/start_compatibility_indicators.h"
#include "equipment_menu.h"

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

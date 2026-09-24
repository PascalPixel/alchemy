#include "EQUIPMENT_MENU.H"

extern u8 *gMenuWork;
void AnimationObjects_SelectAnimationFar(void *, s32);
s32 Scheduler_RemoveCallback(s32);

void EquipmentMenu_StartCompatibilityIndicators(void)
{
    u8 *menu;
    s8 member_index;

    menu = gMenuWork;
    if (menu[0x219] != 0) {
        member_index = 0;
        do {
            s32 off = member_index * 4 + 276;
            void *indicator = *(void **)(menu + off);
            AnimationObjects_SelectAnimationFar(indicator, 1);
            member_index++;
        } while (member_index < menu[0x219]);
    }
    Scheduler_RemoveCallback((s32)&EquipmentMenu_CompatibilityUpdateEntry);
}

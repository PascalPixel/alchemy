#include "MENU_TEST.H"

extern u8 MsgWarriorShopWelcome[];

void SceneState_ApplyBlockD4c(void)
{
    CommandTable_RunDirectionalInput((s32)MsgWarriorShopWelcome, (s32)MsgArmorShopWelcome - (s32)MsgWeaponShopWelcome);
}

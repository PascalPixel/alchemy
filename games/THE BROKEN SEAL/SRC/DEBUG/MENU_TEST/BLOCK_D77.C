#include "MENU_TEST.H"
extern u8 MsgWarriorArmorShopWelcome[];

void SceneState_ApplyBlockD77(void)
{
    CommandTable_RunDirectionalInput((s32)MsgWarriorArmorShopWelcome, (s32)MsgArmorShopWelcome - (s32)MsgWeaponShopWelcome);
}

#include "MENU_TEST.H"
extern u8 MsgWarriorItemShopWelcome[];

void SceneState_ApplyBlockDa2(void)
{
    CommandTable_RunDirectionalInput((s32)MsgWarriorItemShopWelcome, (s32)MsgArmorShopWelcome - (s32)MsgWeaponShopWelcome);
}

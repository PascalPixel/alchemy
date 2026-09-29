#include "MENU_TEST.H"
extern u8 MsgSanctumWelcome[];
extern u8 MsgWarriorShopWelcome[];

void SceneState_ApplyBlockD21(void)
{
    CommandTable_RunDirectionalInput((s32)MsgSanctumWelcome, (s32)MsgWarriorShopWelcome - (s32)MsgSanctumWelcome);
}

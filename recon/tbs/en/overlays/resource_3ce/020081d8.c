/* Draft of resource_3ce 0x020081d8 (SceneState_ApplyBlockDa2): it matches the
 * ROM byte for byte now that the messages it loads from the literal pool have
 * catalogue names (MsgArmorShopWelcome, MsgWarriorItemShopWelcome,
 * MsgWeaponShopWelcome). The listing keeps these rows until the draft is
 * adopted. */
#include "MENU_TEST.H"

extern u8 MsgWarriorItemShopWelcome[];

void SceneState_ApplyBlockDa2(void)
{
    CommandTable_RunDirectionalInput((s32)MsgWarriorItemShopWelcome, (s32)MsgArmorShopWelcome - (s32)MsgWeaponShopWelcome);
}

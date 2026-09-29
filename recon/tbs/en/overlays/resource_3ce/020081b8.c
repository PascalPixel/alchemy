/* Draft of resource_3ce 0x020081b8 (SceneState_ApplyBlockD77): it matches the
 * ROM byte for byte now that the messages it loads from the literal pool have
 * catalogue names (MsgArmorShopWelcome, MsgWarriorArmorShopWelcome,
 * MsgWeaponShopWelcome). The listing keeps these rows until the draft is
 * adopted. */
#include "MENU_TEST.H"

extern u8 MsgWarriorArmorShopWelcome[];

void SceneState_ApplyBlockD77(void)
{
    CommandTable_RunDirectionalInput((s32)MsgWarriorArmorShopWelcome, (s32)MsgArmorShopWelcome - (s32)MsgWeaponShopWelcome);
}

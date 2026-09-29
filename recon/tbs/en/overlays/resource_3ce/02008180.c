/* Draft of resource_3ce 0x02008180 (SceneState_ApplyBlockD21): it matches the
 * ROM byte for byte now that the messages it loads from the literal pool have
 * catalogue names (MsgSanctumWelcome, MsgWarriorShopWelcome). The listing
 * keeps these rows until the draft is adopted. */
#include "MENU_TEST.H"

extern u8 MsgSanctumWelcome[];
extern u8 MsgWarriorShopWelcome[];

void SceneState_ApplyBlockD21(void)
{
    CommandTable_RunDirectionalInput((s32)MsgSanctumWelcome, (s32)MsgWarriorShopWelcome - (s32)MsgSanctumWelcome);
}

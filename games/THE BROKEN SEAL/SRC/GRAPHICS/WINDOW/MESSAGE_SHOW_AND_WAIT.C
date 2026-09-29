#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
#include "UI.H"
s32 BattleFx_GetResourceIdFar(s32);
void UiIcon_PrepareObjectFar(void *);
s32 Menu_RunConfirmSelectionAtFar(s32, s32, s32);
extern u8 Data_03001f2c[];

/* ui/message/show_and_wait.c */
void UiWork_FinalizePendingCoreFar(void);
void UiText_OpenMessageWindowFar(s32, s32, s32, s32);
extern u8 MsgWeaponShopWelcome[];
extern u8 MsgArmorShopWelcome[];
extern u8 MsgItemShopWelcome[];
extern u8 MsgWarriorShopWelcome[];

void UiMessage_ShowAndWait(s32 arg0)
{
    s32 *state = *(s32 **)((u32)&Data_03001f2c);
    s32 value = BattleFx_GetResourceIdFar(*(u16 *)&state[233]);
    s32 result = arg0;
    s8 mode;

    UiWork_FinalizePendingCoreFar();
    mode = *(s8 *)((u8 *)state + 0x3a9);
    if (mode == 2)
        result += MsgArmorShopWelcome - MsgWeaponShopWelcome;
    if (mode == 0)
        result += MsgItemShopWelcome - MsgWeaponShopWelcome;
    if (*(s8 *)&state[235] != 0)
        result += MsgWarriorShopWelcome - MsgWeaponShopWelcome;
    UiText_OpenMessageWindowFar(result, 5, 0, (value << 16) | 0x22);
    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);
    WaitFrames(1);
}

/* ui/message/show_and_restore_state.c */

void UiMessage_ShowAndRestoreState(s32 message_id)
{
    s32 variant;
    s32 no;
    s8 mode;
    s8 flag;
    u8 saved;
    void *state;
    u8 **slot;

    state = *(void **)((u32)&Data_03001f2c);
    slot = (u8 **)((u8 *)state + 0x380);
    saved = (*slot)[5];
    no = message_id;
    variant = BattleFx_GetResourceIdFar(FIELD_AT_OFFSET(state, u16 *, 0x3A4));
    mode = FIELD_AT_OFFSET(state, s8 *, 0x3A9);
    if (mode == 2) {
        no += (s32)MsgArmorShopWelcome - (s32)MsgWeaponShopWelcome;
    }
    if (mode == 0) {
        no += (s32)MsgItemShopWelcome - (s32)MsgWeaponShopWelcome;
    }
    flag = FIELD_AT_OFFSET(state, u8 *, 0x3AC);
    if (flag != 0) {
        no += (s32)MsgWarriorShopWelcome - (s32)MsgWeaponShopWelcome;
    }
    (*slot)[5] = 0xDU;
    UiWork_FinalizePendingCoreFar();
    UiText_OpenMessageWindowFar(no, 5, 0, (variant << 0x10) | 0x22);
    while (UiWork_IsCompleteFar() == 0) {
        WaitFrames(1U);
    }
    WaitFrames(1U);
    FIELD_AT_OFFSET(FIELD_AT_OFFSET(state, void **, 0x380), u8 *, 5) = saved;
}

/* ui/message/show_choice.c */
s32 UiMessage_ShowChoice(s32 arg0)
{
    u8 **slot = (u8 **)(*(u8 **)((u32)&Data_03001f2c) + 0x380);
    u8 saved = (*slot)[5];
    UiIcon_PrepareObjectFar(*slot);
    arg0 = Menu_RunConfirmSelectionAtFar(7, 5, arg0);
    (*slot)[5] = saved;
    return arg0;
}

/* ui/message/show_choice_variant.c */
s32 UiMessage_ShowChoiceVariant(s32 arg0)
{
    u8 **slot = (u8 **)(*(u8 **)((u32)&Data_03001f2c) + 0x380);
    u8 saved = (*slot)[5];
    UiIcon_PrepareObjectFar(*slot);
    arg0 = Menu_RunConfirmSelectionAtFar(7, 7, arg0);
    (*slot)[5] = saved;
    return arg0;
}

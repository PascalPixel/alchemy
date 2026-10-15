#include "render_input.h"
extern struct ShopRuntime *gMenuWork;
extern u8 Data_03001f2c[];
extern u8 MsgSanctumWelcome[];
extern u8 MsgSanctumMoreAid[];
extern u8 MsgSanctumFarewell[];

s32 Object_GetByIdFar(s32 unitId);
s32 UiWindow_CreateWithSideObjectFar(s32 resource, s32 x, s32 y, s32 flags);
void SideObject_CreateFar(s32 a, s32 b, s32 c, s32 window, s32 d, s32 e);
void ShopCursor_SetPositionImmediate(struct ShopCursor *cursor, s32 targetX, s32 targetY);
void UiMessage_ShowResolvedAndWait(s32 message);
void Shop_InitializeCursorWork(void);
void Inn_Cleanup(void);
s32 Shop_CountUnits(void);
void Sanctum_RunPartyService(void);
s32 Menu_SelectEntry19To1cFar(s32 prev);
void UiWork_FinalizeFar(s32 window, s32 style);

s32 Shop_ConfirmAct(s32 unitId)
{
    s32 partyAction = 0;
    s32 listWindow = 0;
    struct ShopRuntime *shop;
    struct ShopCursorAnchor *cursorAnchor;

    Shop_InitializeCursorWork();
    shop = gMenuWork;
    shop->partyAction = listWindow;

    {
        s32 shown =
            *(u16 *)(*(u32 *)(*(u32 *)((u8 *)Object_GetByIdFar(unitId) + 80) + 40));
        *(u16 *)((u8 *)shop + 0x3a4) = shown;
    }

    listWindow = UiWindow_CreateWithSideObjectFar(*(u16 *)((u8 *)shop + 0x3a4), 0, 0, 0);
    if (listWindow == 0) {
        listWindow = UiWindow_CreateFar(-5, 0, 5, 5, 2);
    }
    if (listWindow == 0) {
        listWindow = UiWindow_CreateFar(0, 0, 5, 5, 2);
        SideObject_CreateFar(2, 0, 0, listWindow, -4, -4);
    }

    cursorAnchor = (struct ShopCursorAnchor *)RenderOutput_CreateFar(
        *(u16 *)((u8 *)shop + 0x390),
        0x40000000,
        (struct RenderInput *)listWindow,
        0,
        0);
    cursorAnchor->kind = 1;
    cursorAnchor->unk00[4] = 0;
    ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
    shop->cursor.anchor = cursorAnchor;
    UiMessage_ShowResolvedAndWait((s32)MsgSanctumWelcome);

    shop->moneyWindow = UiWindow_CreateFar(16, 11, 12, 4, 2);
    Shop_DrawMoney();

    for (;;) {
        partyAction = Menu_SelectEntry19To1cFar(partyAction);
        shop->partyAction = partyAction;
        if (partyAction == -1)
            break;

        {
            s32 base = 0xd24;
            s32 message = base;

            base = 0;
            UiMessage_ShowResolvedAndWait(message);
            if (Shop_CountUnits() == 0) {
                UiMessage_ShowResolvedAndWait(message + 1);
            } else {
                Sanctum_RunPartyService();
            }
        }
        shop->partyAction = 0;
        ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
        UiMessage_ShowResolvedAndWait((s32)MsgSanctumMoreAid);
    }

    UiMessage_ShowResolvedAndWait((s32)MsgSanctumFarewell);
    UiWork_FinalizeFar(shop->moneyWindow, 2);
    UiWork_FinalizeFar(listWindow, 2);
    Inn_Cleanup();
    return 0;
}

#include "TYPES.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "UI.H"

#define MENU_SUBOBJECT(menu, offset) (*(u8 **)((u8 *)(menu) + (offset)))

struct InventoryMenuState;
extern struct InventoryMenuState *gMenuWork;
extern volatile s32 gKeysRepeat;
extern volatile u32 gKeyState;
void Menu_UpdateEntryObjectTransforms(void);

void Scheduler_RemoveCallback(void (*callback)(void));
void Palette_CopyObjectBankToBackground14(void);
s32 GameFlag_TestFar(s32);
void ItemMenu_DrawItemDetails(s32, s32);
s32 RenderOutput_RedrawSavedRectFar(s32);
void UiWork_FinalizeFar(s32, s32);
void Palette_LightenBankHighlight(s32);
void Scheduler_AddOrUpdateCallback(const void *, s32);
void UiWindow_DrawFrameFar(s32, s32, s32, s32);

s32 Menu_SelectQuantity(s32 value)
{
    s32 changed = 1;
    u8 *menu = (u8 *)gMenuWork;
    u8 *confirmState = MENU_SUBOBJECT(menu, 540);
    s32 window;
    s32 quantity = 0;

    confirmState[5] = 13;
    window = UiWindow_CreateFar(0, 0, 30, 10, 2);
    Scheduler_RemoveCallback(Menu_UpdateEntryObjectTransforms);

    {
        u8 *iconState = MENU_SUBOBJECT(menu, 380);
        iconState[5] = 13;
    }
    Palette_CopyObjectBankToBackground14();
    WaitFrames(1);

    goto check_exit;

adjust:
    {
        volatile s32 *keys = &gKeysRepeat;

        if (*keys & 0x40) {
            quantity -= 1;
            changed = 1;
        }
        if (*keys & 0x80) {
            quantity += 1;
            changed = 1;
        }
    }
    WaitFrames(1);

check_exit:
    if (GameFlag_TestFar(336) != 0)
        goto done;

    if (changed != 0) {
        changed = 0;
        quantity = Math_Mod(quantity + 5, 5);
        ItemMenu_DrawItemDetails(window, value);
    }

    {
        volatile u32 *keys = &gKeyState;

        if (*keys & 1)
            goto done;
        if (*keys & 2) {
            quantity = -1;
            goto done;
        }
    }
    goto adjust;

done:
    RenderOutput_RedrawSavedRectFar(window);
    WaitFrames(1);
    UiWork_FinalizeFar(window, 1);
    RenderOutput_RedrawSavedRectFar(*(s32 *)(menu + 16));
    Palette_LightenBankHighlight(14);
    {
        s32 delay = 0xc80;

        Scheduler_AddOrUpdateCallback((const void *)Menu_UpdateEntryObjectTransforms, delay);
    }

    {
        u8 *iconState = MENU_SUBOBJECT(menu, 380);
        iconState[5] = 1;
    }
    UiWindow_DrawFrameFar(13, 0, 17, 10);

    return quantity;
}

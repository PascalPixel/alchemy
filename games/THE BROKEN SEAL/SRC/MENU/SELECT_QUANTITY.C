#include "TYPES.H"
#include "SYSTEM.H"

#define MENU_SUBOBJECT(menu, offset) (*(u8 **)((u8 *)(menu) + (offset)))

struct InventoryMenuState;
extern struct InventoryMenuState *gMenuWork;
extern volatile s32 Data_03001b04;
extern volatile u32 gKeyState;
void Menu_UpdateEntryObjectTransforms(void);

s32 UiWindow_CreateFar(s32, s32, s32, s32, s32);
void Scheduler_RemoveCallback(void (*callback)(void));
void Func_080a22f4(void);
s32 GameFlag_TestFar(s32);
s32 Math_Mod(s32, s32);
void Func_080a4924(s32, s32);
s32 RenderOutput_RedrawSavedRectFar(s32);
void UiWork_FinalizeFar(s32, s32);
void Unnamed_080a2144(s32);
void Scheduler_AddOrUpdateCallback(const void *, s32);
void Func_08015408(s32, s32, s32, s32);

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
    Func_080a22f4();
    WaitFrames(1);

    goto check_exit;

adjust:
    {
        volatile s32 *keys = &Data_03001b04;

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
        Func_080a4924(window, value);
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
    Unnamed_080a2144(14);
    {
        s32 delay = 0xc80;

        Scheduler_AddOrUpdateCallback((const void *)Menu_UpdateEntryObjectTransforms, delay);
    }

    {
        u8 *iconState = MENU_SUBOBJECT(menu, 380);
        iconState[5] = 1;
    }
    Func_08015408(13, 0, 17, 10);

    return quantity;
}

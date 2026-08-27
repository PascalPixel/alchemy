#include "types.h"
#include "shop.h"
#include "global_cells.h"
#include "battle_calc.h"

s32 Ability_GetAvailability(s32);
s32 Func_080772a8(s32, s32);
void Func_080b386c(s32, s32, s32);
s32 UiWork_Create(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);
void UiWork_FinalizePending(void);

extern u8 Value_00000075;

s32 Func_080b362c(s32 actor)
{
    struct ShopRuntime *shop = SHOP_RUNTIME;
    /* win2 declared ahead of win1 (and both ahead of object) to match the
     * reference's sp+8/sp+12/sp+16 spill-slot order for these three
     * call-result locals; declaring them in call order instead misassigns
     * object and win2 to each other's slots under this compiler. */
    s32 win2;
    s32 win1;
    u8 *object;
    s32 selection = 0;
    s32 redraw = 1;
    s32 count;
    s32 result = 0;
    s32 status;
    s32 flags;
    void *window;
    s32 x;
    s32 y;

    object = Runtime_GetObject(actor);
    win1 = UiWindow_CreateFar(14, 8, 16, 4, 2);
    win2 = UiWindow_CreateFar(0, 5, 30, 3, 2);
    shop->cursor.anchor->kind = 18;
    shop->mode = 12;

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            count = Ability_GetAvailability(actor);
            if (selection > count - 1)
                selection = count - 1;

            flags = *(u16 *)(object + 216 + selection * 2) & 0x1ff;
            window = (void *)shop->item_window;
            x = Modulo(selection, 5) << 4;
            y = (Math_Div(selection, 5) << 4) + 8;
            Shop_PlaceCursor(window, x, y);
            shop->mode = 3;
            Func_080b386c(win1, actor, selection);
            Shop_DrawMessage(win2, flags + (s32)&Value_00000075);
        }

        WaitFrames(1);

        if ((*(volatile u32 *)ADDR_03001C94 & 1) != 0) {
            status = Func_080772a8(actor, selection);
            if (status == 0) {
                Audio_PlayCue(112);
                result = selection;
                goto exit_loop;
            } else if (status == -4) {
                UiWork_Create(0xc96, 8, 1, 2);
            } else if (result == -3) {
                UiWork_Create(0xc97, 8, 1, 2);
            }
            Audio_PlayCue(113);
            while (UiWork_IsCompleteFar() == 0)
                WaitFrames(1);
            UiWork_FinalizePending();
            continue;
        }

        if ((*(volatile u32 *)ADDR_03001C94 & 2) != 0) {
            Audio_PlayCue(113);
            result = -1;
            goto exit_loop;
        }

        if ((*(volatile u32 *)ADDR_03001B04 & 0x20) != 0) {
            Audio_PlayCue(111);
            selection = Modulo(selection - 1 + count, count);
            redraw = 1;
        }
        if ((*(volatile u32 *)ADDR_03001B04 & 0x10) != 0) {
            Audio_PlayCue(111);
            selection = Modulo(selection + 1 + count, count);
            redraw = 1;
        }
        if ((*(volatile u32 *)ADDR_03001B04 & 0x40) != 0) {
            selection -= 5;
            if (selection < 0)
                selection += 15;
            while (selection >= count)
                selection -= 5;
            Audio_PlayCue(111);
            redraw = 1;
        }
        if ((*(volatile u32 *)ADDR_03001B04 & 0x80) != 0) {
            selection += 5;
            if (selection >= count)
                selection -= 15;
            while (selection < 0)
                selection += 5;
            Audio_PlayCue(111);
            redraw = 1;
        }
    }

exit_loop:
    UiWindow_Close(win2, 2);
    UiWindow_Close(win1, 2);
    WaitFrames(1);
    return result;
}

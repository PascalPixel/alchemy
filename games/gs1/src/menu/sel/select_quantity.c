#include "types.h"
#include "scene.h"

#define MENU_SUBOBJECT(menu, offset) (*(u8 **)((u8 *)(menu) + (offset)))

struct ItemMenuState;
extern struct ItemMenuState *gIw;
extern volatile s32 gIw2;
extern volatile u32 gIw3;

s32 Menu_Run(s32 value)
{
    s32 changed = 1;
    u8 *menu = (u8 *)gIw;
    u8 *confirmState = MENU_SUBOBJECT(menu, 540);
    s32 window;
    s32 quantity = 0;

    confirmState[5] = 13;
    window = Menu_SetRange(0, 0, 30, 10, 2);
    Menu_Do(Menu_unk2_4);

    {
        u8 *iconState = MENU_SUBOBJECT(menu, 380);
        iconState[5] = 13;
    }
    Menu_unk3_3();
    Menu_unk2_2(1);

    goto check_exit;

adjust:
    {
        volatile s32 *keys = &gIw2;

        if (*keys & 0x40) {
            quantity -= 1;
            changed = 1;
        }
        if (*keys & 0x80) {
            quantity += 1;
            changed = 1;
        }
    }
    Menu_unk2_2(1);

check_exit:
    if (Menu_Check(336) != 0)
        goto done;

    if (changed != 0) {
        changed = 0;
        quantity = Menu_Apply(quantity + 5, 5);
        Menu_Apply2(window, value);
    }

    {
        volatile u32 *keys = &gIw3;

        if (*keys & 1)
            goto done;
        if (*keys & 2) {
            quantity = -1;
            goto done;
        }
    }
    goto adjust;

done:
    Menu_unk2(window);
    Menu_unk2_2(1);
    Menu_Apply3(window, 1);
    Menu_unk2(*(s32 *)(menu + 16));
    Menu_unk3_2(14);
    {
        s32 delay = 0xc80;

        Menu_Apply4((const void *)Menu_unk2_4, delay);
    }

    {
        u8 *iconState = MENU_SUBOBJECT(menu, 380);
        iconState[5] = 1;
    }
    Menu_SetMode(13, 0, 17, 10);

    return quantity;
}

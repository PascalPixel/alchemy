#include "types.h"

#define MENU_SUBOBJECT(menu, offset) (*(u8 **)((u8 *)(menu) + (offset)))

extern u8 *Data_03001f2c;
extern volatile s32 Data_03001c94;
extern volatile u32 Data_03001b04;
void Func_080a19a0(void);

s32 Func_08015010(s32, s32, s32, s32, s32);
void Func_08004278(void (*callback)(void));
void Func_080a22f4(void);
void Func_080030f8(s32);
s32 Func_080770c0(s32);
s32 Func_080022fc(s32, s32);
void Func_080a4924(s32, s32);
s32 Func_08015270(s32);
void Func_08015018(s32, s32);
void Func_080a2144(s32);
void Func_080041d8(const void *, s32);
void Func_08015408(s32, s32, s32, s32);

s32 Func_080a4800(s32 value)
{
    s32 changed = 1;
    u8 *menu = Data_03001f2c;
    u8 *confirmState = MENU_SUBOBJECT(menu, 540);
    s32 window;
    s32 quantity = 0;

    confirmState[5] = 13;
    window = Func_08015010(0, 0, 30, 10, 2);
    Func_08004278(Func_080a19a0);

    {
        u8 *iconState = MENU_SUBOBJECT(menu, 380);
        iconState[5] = 13;
    }
    Func_080a22f4();
    Func_080030f8(1);

    goto check_exit;

adjust:
    {
        volatile s32 *keys = &Data_03001c94;

        if (*keys & 0x40) {
            quantity -= 1;
            changed = 1;
        }
        if (*keys & 0x80) {
            quantity += 1;
            changed = 1;
        }
    }
    Func_080030f8(1);

check_exit:
    if (Func_080770c0(336) != 0)
        goto done;

    if (changed != 0) {
        changed = 0;
        quantity = Func_080022fc(quantity + 5, 5);
        Func_080a4924(window, value);
    }

    {
        volatile u32 *keys = &Data_03001b04;

        if (*keys & 1)
            goto done;
        if (*keys & 2) {
            quantity = -1;
            goto done;
        }
    }
    goto adjust;

done:
    Func_08015270(window);
    Func_080030f8(1);
    Func_08015018(window, 1);
    Func_08015270(*(s32 *)(menu + 16));
    Func_080a2144(14);
    {
        s32 delay = 0xc80;

        Func_080041d8((const void *)Func_080a19a0, delay);
    }

    {
        u8 *iconState = MENU_SUBOBJECT(menu, 380);
        iconState[5] = 1;
    }
    Func_08015408(13, 0, 17, 10);

    return quantity;
}

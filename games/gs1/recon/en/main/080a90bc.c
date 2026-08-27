#include "shop.h"

void Func_080a8c2c(void);
s32 Func_08015270(s32 window);
void Func_080a10d0(s32 base, s32 a, s32 b, s32 c, s32 d, s32 e);
void Func_08004278(s32 a);
void Func_080a33d4(struct ShopRuntime *shop, s32 window);
void Func_08015080(s32 icon_id, s32 window, s32 x, s32 y);
void Func_080f9010(s32 message);
void Func_080041d8(s32 a, s32 b);
s32 Func_080a8f40(s32 a, s32 b, s32 c);
s32 Func_080a8d34(s32 a, s32 b, s32 c);
void Func_080030f8(s32 frames);
s32 Func_080a1fd4(s32 a, s32 b, s32 c, s32 d, s32 e);
s32 Func_080a1a40(s32 a, s32 b);
s32 Func_080770c0(s32 message);
void Func_080a9cbc(void);
s32 Func_08077008(s32 icon);
s32 Func_080a68ec(s32 field, s32 anchor, s32 b);
s32 Func_080a8b8c(s32 dst, s32 a);
void Func_080a9374(s32 a, s32 b);
s32 Modulo(s32 value, s32 divisor);
void Func_080a1804(struct ShopRuntime *shop, s32 val);
s32 Func_08015278(s32 window);
void Func_080a345c(void);

#define INPUT_NEW_KEYS (*(volatile u32 *)ADDR_03001C94)
#define INPUT_REPEAT_KEYS (*(volatile u32 *)ADDR_03001B04)

/* A larger shop confirmation/setup loop: initializes a work window, an
 * icon list, and a 32-entry pointer table, then runs an input-driven
 * update loop that adjusts an icon selection and confirms or cancels. */
s32 Func_080a90bc(void)
{
    struct ShopRuntime *shop = SHOP_RUNTIME;
    s32 result = 0;
    s32 window;
    s32 selection = 0;
    s32 field;
    u8 *flag_ptr;
    s32 i;

    Func_080a8c2c();

    window = *(s32 *)((u8 *)shop + 0x10c);
    Func_08015270(window);

    Func_080a10d0((s32)((u8 *)shop + 44), 0, 0, 30, 5, 2);

    for (i = 2; i >= 0; i--) {
        *(u16 *)((u8 *)shop + 0x1a2 - i * 2) = 0x0242;
    }

    {
        s32 *slots = (s32 *)((u8 *)shop + 72);
        for (i = 31; i >= 0; i--) {
            s32 entry = *slots++;
            if (entry != 0) {
                *(u8 *)(entry + 15) = 245;
            }
        }
    }

    Func_08004278(0x080a19a1);

    window = *(s32 *)((u8 *)shop + 0x10c);
    Func_080a33d4(shop, window);

    {
        s32 icon_base = *(s32 *)((u8 *)shop + 36);
        Func_08015080(icon_base, -24, 80, 0);
        Func_08015080(icon_base + 2, -24, 0, 0);
    }

    flag_ptr = (u8 *)shop + 0x21a;

    for (;;) {
        s32 event = 0;
        s32 blocked = 0;

        if (selection != 0) {
            selection = 0;
            if (*(u8 *)flag_ptr != 0) {
                Func_080a8f40(*(s32 *)((u8 *)shop + 36), 0, (s32)flag_ptr);
                event = 0;
            }
            Func_080a8d34(*(s32 *)((u8 *)shop + 36), 0, (s32)flag_ptr);
            Func_080030f8(1);
        }

        Func_080030f8(1);
        selection = Func_080a1fd4(0, *(s32 *)((u8 *)flag_ptr + 20), 5, 0, 0);
        field = *(s32 *)((u8 *)flag_ptr + 16);
        blocked = Func_080a1a40((field << 4) + 60, 55);

        event = 0;
        if (blocked == 1) {
            selection = 1;
            event = 1;
        }
        if (blocked == 0)
            event = 1;
        if (blocked == -1)
            event = 0;

        if ((INPUT_NEW_KEYS & 1) != 0) {
            result = 1;
            break;
        }
        if ((INPUT_NEW_KEYS & 2) != 0) {
            result = -1;
            Func_080041d8(0xc80, 0);
            break;
        }
        if ((INPUT_REPEAT_KEYS & 0x100) != 0 ||
            (INPUT_REPEAT_KEYS & 0x200) != 0) {
            s32 idx;
            s32 fld;
            s32 count;
            s32 new_idx;
            s32 new_fld;

            Func_080f9010(111);
            idx = *(s8 *)((u8 *)shop + 28);
            fld = *(u16 *)((u8 *)shop + 0x208 + idx * 2);
            *(u8 *)((u8 *)shop + fld + 0x260) =
                *(s32 *)((u8 *)flag_ptr + 24);

            if ((INPUT_REPEAT_KEYS & 0x100) != 0)
                idx = idx + 1;
            else
                idx = idx - 1;

            count = *(u8 *)((u8 *)shop + 0xc05);
            new_idx = Modulo(idx + count, count);
            new_fld = *(u16 *)((u8 *)shop + 0x208 + new_idx * 2);
            *(u32 *)((u8 *)shop + 8) = new_fld;
            *(u8 *)flag_ptr = (u8)new_fld;
            *(u8 *)((u8 *)shop + 28) = new_idx;
            Func_080a1804(shop, new_fld);
            continue;
        }

        if (Func_080770c0(0x150) != 0)
            continue;

        Func_080a9cbc();
        Func_08015270(*(s32 *)((u8 *)shop + 36));
        Func_08077008(*flag_ptr);
        *(u8 *)((u8 *)shop + 0x218) = Func_080a68ec(
            (s32)((u8 *)shop + 0x1c8), (s32)flag_ptr, 0);
        Func_080030f8(1);
        Func_080a8b8c(0, 0);
        Func_080a9374(*(s32 *)((u8 *)shop + 36), *flag_ptr);
        selection = 1;
    }

    Func_08015278(*(s32 *)((u8 *)shop + 44));
    Func_08015270(*(s32 *)((u8 *)shop + 44));
    Func_080a345c();
    Func_08015278(*(s32 *)((u8 *)shop + 0x10c));
    Func_08015270(*(s32 *)((u8 *)shop + 36));

    return result;
}

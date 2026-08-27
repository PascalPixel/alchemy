#include "shop.h"

s32 Func_08077008(s32 icon);
s32 Func_08015270(s32 window);
void Func_08015080(s32 icon_id, s32 window, s32 x, s32 y);
s32 Modulo(s32 value, s32 divisor);
s32 Func_080a8088(s32 field, s32 frame);
void Func_080a1804(struct ShopRuntime *shop, s32 val);
s32 Func_080a68ec(s32 field, s32 anchor, s32 b);
s32 Func_080a68a8(s32 anchor);
void Func_080a9b94(s32 a, s32 b, s32 c);
s32 Func_080a3d24(s32 anchor);
s32 Func_080a1a40(s32 a, s32 b);
void Func_080030f8(s32 frames);
void Func_080f9010(s32 message);

#define INPUT_NEW_KEYS (*(volatile u32 *)ADDR_03001C94)
#define INPUT_REPEAT_KEYS (*(volatile u32 *)ADDR_03001B04)

/* Runs a paged icon-selection loop over a shop-relative array, keeping the
 * current selection and count persisted in the shop struct itself rather
 * than in locals. */
void Func_080a7d68(void)
{
    struct ShopRuntime *shop = SHOP_RUNTIME;
    s32 selection = *(s8 *)((u8 *)shop + 0x1c);
    s32 count = *(s8 *)((u8 *)shop + 0x1e);
    s32 pending = 1;
    s32 saved = *(u16 *)((u8 *)shop + 0x220);
    s32 field;
    s32 anchor = (s32)((u8 *)shop + 0x1c8);
    s32 window = *(s32 *)((u8 *)shop + 0x10c);

    field = *(u16 *)((u8 *)shop + 0x208 + selection * 2);
    Func_08077008(field);

    Func_08015270(window);
    Func_08015080(0x1e, window, 0, 0);
    Func_08015080(0x1f, window, 0, 16);

    for (;;) {
        if (pending != 0) {
            pending = 0;
            selection = Modulo(selection + count, count);

            field = *(u16 *)((u8 *)shop + 0x208 + selection * 2);
            Func_08077008(field);
            saved = Modulo(saved + 3, 3);
            Func_080a8088(field, saved);
            Func_080a1804(shop, field);

            {
                s16 *dst = (s16 *)((u8 *)shop + 0x152);
                s32 i;
                for (i = 7; i >= 0; i--) {
                    *dst = 0x0c05;
                    dst--;
                }
            }
            *(s16 *)((u8 *)shop + 0x144 + selection * 2) = 0x0c05;

            field = *(u16 *)((u8 *)shop + 0x208 + selection * 2);
            Func_08077008(field);
            *(u8 *)((u8 *)shop + 0x218) = Func_080a68ec(field, anchor, 0);
            Func_080a68a8(anchor);
            Func_080a9b94(96, 96, 8);
            Func_080a3d24(anchor);
        }

        Func_080a1a40(selection * 24 - 10, 16);
        Func_080030f8(1);

        if ((INPUT_NEW_KEYS & 1) != 0) {
            Func_080f9010(112);
            break;
        }
        if ((INPUT_NEW_KEYS & 2) != 0) {
            Func_080f9010(113);
            break;
        }
        if ((INPUT_REPEAT_KEYS & 0x20) != 0) {
            Func_080f9010(111);
            if (count > 1) {
                selection--;
                pending = 1;
            }
        }
        if ((INPUT_REPEAT_KEYS & 0x10) != 0) {
            Func_080f9010(111);
            if (count > 1) {
                selection++;
                pending = 1;
            }
        }
    }

    *(u8 *)((u8 *)shop + 0x1c) = selection;
    field = *(u16 *)((u8 *)shop + 0x208 + selection * 2);
    *(u32 *)((u8 *)shop + 8) = field;
    *(u8 *)((u8 *)shop + 0x21a) = (u8)field;
}

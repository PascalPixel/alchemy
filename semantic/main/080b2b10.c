#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s32 Func_080022fc(s32, s32);
void Func_080030f8(s32);
u8 *Func_08015010(s32, s32, s32, s32, s32);
void Func_08015018(u8 *, s32);
void Func_08015120(s32, s32);
void Func_08015140(void);
void Func_08077230(s32);
void Func_080a1028(u8 *, s32, s32, s32, s32);
void Func_080a1030(void);
s32 Func_080b0664(s32);
void Func_080b0a6c(u8 *, s32, s32);
void Func_080b10cc(void);
s16 Func_080b2778(s16, s32);
s32 Func_080b27b0(s16, s32);
s32 Func_080b280c(void);
void Func_080b28d4(s32);
void Func_080b2928(s32);
void Func_080b2da8(s16, s32);
void Func_080b2e30(u8 *, s32);
void Func_080b2ed8(u8 *, s16);
void Func_080b3050(s32);
void Func_080f9010(s32);

/*
 * Run the candidate purchase/application menu.  Invalid or unaffordable
 * candidates redisplay the list; cancellation and a completed purchase close
 * both windows.
 */
s32 Func_080b2b10(void)
{
    u8 *state = *(u8 **)0x03001f2c;
    s32 count = FIELD(state, s8, 0x3a7);
    s32 category = FIELD(state, s8, 0x3aa);
    u8 *message_window;
    u8 *list_window;
    s32 selection = 0;
    s32 dirty = 1;
    s32 rebuild = 0;
    s16 candidate = 0;

    Func_080b28d4(0x0d26);
    message_window = Func_08015010(1, 0x0c, 0x0d, 3, 2);
    FIELD(FIELD(state, u8 *, 0x380), s8, 5) = 4;
    FIELD(state, u8, 0x3a8) = 1;
    Func_080a1028(message_window, 2, 0, 8, 0);
    list_window = Func_08015010(1, 0x10, 0x17, 3, 2);

    while (selection < count) {
        candidate = FIELD(state, s16, 0x36e + selection * 2);
        if (Func_080b27b0(candidate, category) != 0)
            break;
        selection++;
    }

    for (;;) {
        if (rebuild != 0) {
            rebuild = 0;
            Func_080b28d4(0x0d26);
            selection = 0;
            dirty = 1;
            while (selection < count) {
                candidate = FIELD(state, s16, 0x36e + selection * 2);
                if (Func_080b27b0(candidate, category) != 0)
                    break;
                selection++;
            }
        }

        if (dirty != 0) {
            dirty = 0;
            selection = Func_080022fc(selection + count, count);
            candidate = FIELD(state, s16, 0x36e + selection * 2);
            Func_080b0a6c(message_window, selection * 0x18 - 0x0c, 0);
            FIELD(state, u8, 0x3a8) = 3;
            Func_080b2e30(message_window, selection);
            Func_080b2ed8(list_window, candidate);
        }

        if ((*(u32 *)0x03001c94 & 1) != 0) {
            s16 price;

            Func_080030f8(1);
            price = Func_080b2778(candidate, category);
            if (Func_080b27b0(candidate, category) == 0) {
                Func_080f9010(0x71);
                continue;
            }

            Func_08015120(candidate, 1);
            Func_08015120(price, 5);
            Func_080b28d4(0x0d27);
            if (Func_080b0664(0) != 0) {
                Func_080b2928(0x0d29);
                rebuild = 1;
                continue;
            }
            if ((u32)(s32)price > *(u32 *)0x02000250) {
                Func_080f9010(0x71);
                Func_080b2928(0x0d28);
                rebuild = 1;
                continue;
            }

            Func_08015120(candidate, 1);
            Func_080b28d4(0x0d2a);
            Func_08015140();
            Func_080b2da8(candidate, category);
            Func_080b3050(selection);
            Func_08077230(-price);
            Func_080b10cc();
            Func_08015120(candidate, 1);
            Func_080b28d4(0x0d2b);
            if (Func_080b280c() != 0) {
                rebuild = 1;
                continue;
            }
            break;
        }

        if ((*(u32 *)0x03001c94 & 2) != 0) {
            Func_080f9010(0x71);
            break;
        }
        if ((*(u32 *)0x03001b04 & 0x20) != 0) {
            Func_080f9010(0x6f);
            dirty = 1;
            selection--;
        }
        if ((*(u32 *)0x03001b04 & 0x10) != 0) {
            Func_080f9010(0x6f);
            dirty = 1;
            selection++;
        }
        Func_080030f8(1);
    }

    Func_080a1030();
    Func_08015018(list_window, 2);
    Func_08015018(message_window, 2);
    Func_080030f8(1);
    return 0;
}

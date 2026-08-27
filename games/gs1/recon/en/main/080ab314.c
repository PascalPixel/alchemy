#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

/*
 * Data_03001f2c and Data_03001e8c are the same polymorphic menu-runtime and
 * UI-text-work cells used by games/gs1/src/menu/select_quantity.c and
 * games/gs1/src/ui/text/prepare_message_work.c: reused per-mode scratch
 * pointers rather than a single named struct.  This owner's own field
 * offsets (0x10, 0x30, 0x10C on the first cell) line up with named fields on
 * struct CharacterSelectorState/PsynergyMenuState (selector_window /
 * auxiliary_window / message_window) documented elsewhere, but nothing in
 * this owner's own assembly proves which of those modes it belongs to, so
 * the fields stay raw offsets here instead of importing one of those types.
 */
extern u8 *Data_03001f2c;
extern u8 *Data_03001e8c;
extern volatile u32 Data_03001b04;
extern volatile u32 Data_03001c94;

void Func_08015278(s32 window);
void Func_080030f8(s32 frames);
void Func_08015060(s32 window);
void Func_08015080(s32 message, s32 window, s32 x, s32 y);
void Func_080ab21c(s32 x, s32 y, s32 width, s32 height, s32 palette);
void Func_080ab2ec(void *origin, s32 x, s32 y, s32 width, s32 height, s32 palette);
s32 Func_08015010(s32 kind, s32 x, s32 y, s32 width, s32 height);
void Func_080152a8(void);
void Func_08015078(s32 message, s32 window, s32 x, s32 y);
s32 *Func_080153f8(s32 window, s32 message);
s32 Func_080ab1f4(void *origin, s32 x, s32 y, s32 width, s32 style, s32 alt);
void Func_080a1a40(s32 index, s32 y);
s32 Func_08015048(void);
s32 Func_080aa538(s32 value, s32 modulus);
void Func_080f9010(s32 sound_id);
s32 Resource_ResetEntry(u32 resource_index);
void Func_08015018(s32 window, s32 flag);
void Func_080a19a0(void);
void Func_080041d8(const void *callback, s32 order);

s32 Func_080ab314(void)
{
    s32 win_b;
    s32 win_a;
    u8 *state;
    u8 *work;
    s32 result;
    s32 prev_sel;
    s32 sel;
    s32 list;
    s32 *slot;
    u16 counter;
    s32 i;

    state = *(u8 **)&Data_03001f2c;
    work = *(u8 **)((u8 *)&Data_03001f2c - 0xA0);
    result = 0;
    prev_sel = 0;
    sel = 0;

    Func_08015278(M2C_FIELD(state, s32, 0x30));
    Func_080030f8(1);

    Func_08015060(M2C_FIELD(state, s32, 0x10C));
    Func_08015080(0xC30, M2C_FIELD(state, s32, 0x10C), 0, 0);
    Func_08015080(0xC31, M2C_FIELD(state, s32, 0x10C), 0, 16);

    Func_080ab21c(1, 1, 11, 3, 6);
    Func_080ab2ec((void *)M2C_FIELD(state, s32, 0x30), 0, 0, 28, 10, 6);

    list = Func_08015010(0, 9, 8, 10, 6);
    win_b = Func_08015010(8, 12, 22, 7, 2);
    win_a = Func_08015010(8, 9, 22, 3, 2);

    Func_080152a8();

    i = 0;
    do {
        Func_08015080(i + 0xC32, list, 0, i * 8);
        i++;
    } while (i <= 6);

    do {
        Func_08015060(win_a);
        Func_08015078(sel + 0xC32, win_a, 0, 0);
        slot = Func_080153f8(win_b, sel + 0xC39);
        Func_080ab1f4(list, 0, prev_sel, 6, 1, 0xF);
        Func_080ab1f4(list, 0, sel, 6, 1, 0xE);
        prev_sel = sel;

        for (;;) {
            Func_080a1a40(
                -12, (M2C_FIELD(list, u16, 0xE) + sel) * 8 + 8);
            Func_080030f8(1);

            if (Data_03001b04 & 0x90) {
                sel += 1;
                sel = Func_080aa538(sel, 7);
                Func_080f9010(0x6F);
                break;
            } else if (Data_03001b04 & 0x60) {
                sel -= 1;
                sel = Func_080aa538(sel, 7);
                Func_080f9010(0x6F);
                break;
            } else if (Data_03001c94 & 8) {
                Func_080f9010(0x71);
                result = -2;
                break;
            } else if (Data_03001c94 & 6) {
                Func_080f9010(0x71);
                result = -1;
                break;
            } else if (Data_03001c94 & 1) {
                if (Func_08015048() != 0) {
                    sel += 1;
                    sel = Func_080aa538(sel, 7);
                    Func_080f9010(0x70);
                    break;
                } else {
                    Func_080f9010(0x6F);
                }
            }
        }

        counter = M2C_FIELD(work, u16, 0x12B6);
        if (counter != 99) {
            Resource_ResetEntry(counter);
            M2C_FIELD(work, u16, 0x12B6) = 99;
        }

        M2C_FIELD(Data_03001e8c, u8, 0x12F8) = 0;
        Func_08015060(win_b);
        {
            s32 *entry = (s32 *)*slot;
            M2C_FIELD(entry, u16, 0x1A) = 0;
            M2C_FIELD(entry, u16, 0x18) = 0;
            M2C_FIELD(entry, u16, 0x14) = 0;
        }
        *slot = 0;
    } while (result == 0);

    M2C_FIELD(Data_03001e8c, u8, 0xEA6) = 1;
    Func_08015278(win_a);
    Func_08015278(win_b);
    Func_080030f8(1);
    Func_08015018(win_a, 1);
    Func_08015018(list, 1);
    Func_08015018(win_b, 1);
    Func_080152a8();

    if (result == -2) {
        Func_08015060(M2C_FIELD(state, s32, 0x10C));
        Func_08015060(M2C_FIELD(state, s32, 0x30));
        Func_08015060(M2C_FIELD(state, s32, 0x10));
        M2C_FIELD(Data_03001e8c, u8, 0xEA6) = 0;
    }

    Func_080041d8((const void *)Func_080a19a0, 0xC80);

    return result;
}

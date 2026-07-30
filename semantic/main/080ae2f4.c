#include "types.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S8_AT(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

void Func_08015278(void *);
void Func_080030f8(s32);
s16 *Func_08004970(s32);
s32 Func_08077008(s32);
s32 Func_080aae14(u16 *, u16 *, s16 *, s32 *, s32 *);
void Func_08002df0(void *);
s32 Func_080022ec(s32, s32);
void Func_080a10d0(void *, s32, s32, s32, s32, s32);
void Func_08015270(void *);
void Func_08015080(s32, void *, s32, s32);
void Func_08015060(void *);
void Func_080acab8(void *, s32, s32, u8, s32, s32, s32, s32, s32);
void Func_08015280(void *, s32, s32, s32, s32);
s32 Func_080022fc(s32, s32);
void Func_080ad5b4(s32, s32, s32, s32);
s32 Func_080aa538(s32, s32);
void Func_080072f0(s32, s32, s32, s32);
void Func_080f9010(s32);
void Func_0800352c(void);
void Func_080041d8(const void *, s32);
void Func_080a1114(void *);

s32 Func_080ae2f4(void)
{
    u8 *state = *(u8 **)0x03001f2c;
    s32 selection = 0;
    s32 counts[2] = {1, 1};
    s32 redraw = 1;
    s32 animation = 0;
    s32 frame = 0;
    s32 result;
    s32 unmatched_first;
    s32 unmatched_second;
    s16 *first_list;
    s16 *second_list;
    u8 *displayed_value = state + 0x258;

    S8_AT(PTR_AT(state, 0x14), 5) = 0x0d;
    S16_AT(state, 0x14a) = 0xc8;
    S16_AT(state, 0x148) = 0xc8;
    S16_AT(state, 0x146) = 0xc8;
    S16_AT(state, 0x144) = 0xc8;
    Func_08015278(PTR_AT(state, 0x30));
    Func_080030f8(1);

    first_list = Func_08004970(0x60);
    second_list = Func_08004970(0x14c);
    {
        u16 *records =
            (u16 *)(Func_08077008(U8_AT(state, 0x21a)) + 0x58);
        counts[0] = Func_080aae14(
            records, records, first_list,
            &unmatched_second, &unmatched_first);
        counts[1] = counts[0];
    }
    Func_08002df0(second_list);
    Func_08002df0(first_list);

    counts[0] = Func_080022ec(counts[0] - 1, 6) + 1;
    if (counts[0] == 0)
        counts[0] = 1;
    counts[1] = Func_080022ec(counts[1] - 1, 6) + 1;
    if (counts[1] == 0)
        counts[1] = 1;

    Func_080a10d0(state + 0x24, 0, 5, 0x0f, 0x0f, 2);
    Func_080a10d0(state + 0x34, 0x0f, 5, 0x0f, 0x0f, 2);
    Func_08015270(PTR_AT(state, 0x10c));
    Func_08015270(PTR_AT(state, 0x10));
    Func_08015080(0xbaa, PTR_AT(state, 0x10), 0, 0);
    Func_08015080(0xbac, PTR_AT(state, 0x10), 0, 0x10);

    for (;;) {
        u8 *engine = *(u8 **)0x03001e8c;
        u32 pressed = *(u32 *)0x03001c94;
        u32 held = *(u32 *)0x03001b04;

        if (redraw) {
            engine[0xea6] = 1;
            Func_08015060(PTR_AT(state, 0x24));
            Func_08015060(PTR_AT(state, 0x34));
            Func_080acab8(
                PTR_AT(state, 0x24), 0, 0, *displayed_value,
                0, 0, 3, 0, 1);
            Func_080acab8(
                PTR_AT(state, 0x34), 0, 0, *displayed_value,
                0, 0, 3, selection + 1, 1);
            engine[0xea6] = 0;
        }

        if (counts[0] > 1) {
            void *window = PTR_AT(state, 0x34);
            s32 i;

            for (i = 0; i < counts[0]; i++) {
                s32 tile = i <= 9 ? i + 0xf031 : 0xf030;
                if (i == selection)
                    tile -= 0x1000;
                Func_08015280(
                    window, tile,
                    U16_AT(window, 8) - counts[0] + i - 2,
                    -1, 0);
            }
            Func_08015280(
                window, 0xf128,
                U16_AT(window, 8) - counts[0] - 3,
                -1, 0);
            Func_08015280(
                window, 0xf129, U16_AT(window, 8) - 2,
                -1, 0);
            engine[0xea3] |=
                2 << (U16_AT(window, 0x0e) >> 2);
        }

        frame++;
        Func_080022fc(frame, 0x3c);
        Func_080ad5b4(0, 0x20, 0xc8, 0);
        if (redraw) {
            redraw = 0;
            animation = Func_080aa538(animation, 2);
        }

        if (!(frame & 3)) {
            if (frame & 4)
                Func_080072f0(
                    0x060052c0, 0x080af26c,
                    0x20, 0x03001388);
            else
                Func_080072f0(
                    0x060052c0, 0x20,
                    0x44444444, 0x03000168);
        }

        if (pressed & 8) {
            result = 2;
            break;
        }
        if (pressed & 0x303) {
            result = 1;
            break;
        }

        if (held & 0x20) {
            selection--;
            selection = Func_080aa538(selection, counts[0]);
            Func_080f9010(0x6f);
            Func_0800352c();
            redraw = 1;
        } else if (held & 0x10) {
            selection++;
            Func_080f9010(0x6f);
            Func_0800352c();
            redraw = 1;
            selection = Func_080aa538(selection, counts[0]);
        }
        Func_080030f8(1);
    }

    Func_080f9010(0x71);
    Func_080041d8((const void *)0x080a19a1, 0xc80);
    (*(u8 **)0x03001e8c)[0xea6] = 1;
    Func_080a1114(state + 0x10c);
    Func_080030f8(1);
    Func_080a10d0(state + 0x10c, 0x0d, 0, 0x11, 5, 2);
    Func_080a1114(state + 0x24);
    Func_080a1114(state + 0x34);
    Func_08015270(PTR_AT(state, 0x30));
    Func_08015270(PTR_AT(state, 0x28));
    Func_08015270(PTR_AT(state, 0x10));
    (*(u8 **)0x03001e8c)[0xea6] = 0;
    Func_080030f8(1);
    return -result;
}

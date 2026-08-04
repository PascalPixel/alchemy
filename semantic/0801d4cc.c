#include "types.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S8_AT(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

void Func_0801d014(void);
void *Func_080162d4(s32, s32, s32, s32, s32);
void *Func_0801d108(void);
s32 Func_08021620(s32, void *, s32, s32);
void Func_080030f8(s32);
s32 Func_080022fc(s32, s32);
void Func_080a1038(void *);
void Func_080216e8(s32, s32, s32);
s32 Func_080022ec(s32, s32);
void Func_080b0030(void *, s32, s32, s32);
void Func_080164d4(void *, s32, s32, s32, s32);
void Func_0801e7c0(s32, void *, s32, s32);
void Func_0801ccc0(s8, s8);
void Func_080164ac(void *);
void Func_0801e74c(s32, void *, s32, s32);
void Func_080216b4(s32);
void Func_080f9010(s32);
void Func_08016418(void *, s32);
void Func_0801d0f0(void);

s32 Func_0801d4cc(void)
{
    u8 *state;
    void *selection_window;
    void *settings_window;
    s32 cursor;
    s32 selection = 0;
    s32 redraw = 1;
    s32 result;

    Func_0801d014();
    state = *(u8 **)0x03001ea0;
    selection_window = Func_080162d4(1, 2, 0x1c, 3, 2);
    settings_window = Func_0801d108();
    cursor = Func_08021620(7, settings_window, 0x40, -0x30);
    Func_080030f8(1);

    for (;;) {
        if (redraw) {
            s32 i;
            s32 row_value;
            s32 x;
            s32 y;

            redraw = 0;
            selection = Func_080022fc(selection + 5, 5);
            row_value = (s8)state[0x594 + selection];
            state[0x594 + selection] = Func_080022fc(
                row_value + S8_AT(state, 0x599 + selection),
                S8_AT(state, 0x599 + selection));
            U16_AT(state, 0x574) = selection;
            if (*(u8 *)0x03001ca0)
                state[0x598] = 0;

            for (i = 0; i < 3; i++) {
                void *object = PTR_AT(state, 0x5ec + i * 4);
                S8_AT(object, 0x0f) = -5;
                Func_080a1038(object);
                Func_080216e8(
                    S8_AT((void *)0x080367c9, i),
                    U8_AT(object, 0x0e),
                    i != S8_AT(state, 0x596));
            }

            for (i = 0; i < 2; i++) {
                void *object = PTR_AT(state, 0x5f8 + i * 4);
                S8_AT(object, 0x0f) = -5;
                Func_080a1038(object);
                Func_080216e8(
                    S8_AT((void *)0x080367cc, i),
                    U8_AT(object, 0x0e),
                    i != S8_AT(state, 0x597));
            }

            for (i = 0; i < 2; i++) {
                void *object = PTR_AT(state, 0x604 + i * 4);
                S8_AT(object, 0x0f) = -5;
                Func_080a1038(object);
                Func_080216e8(
                    S8_AT((void *)0x080367ce, i),
                    U8_AT(object, 0x0e),
                    i != S8_AT(state, 0x598));
            }

            x = U16_AT(settings_window, 0x0c) * 8;
            y = U16_AT(settings_window, 0x0e) * 8;
            Func_080b0030(
                state + 0x5b4,
                x + 0x8c +
                    Func_080022ec(S8_AT(state, 0x594) * 0x3c,
                                 S8_AT(state, 0x599)),
                y + 4, 1);
            Func_080b0030(
                state + 0x5c4,
                x + 0x8c +
                    Func_080022ec(S8_AT(state, 0x595) * 0x3c,
                                 S8_AT(state, 0x59a)),
                y + 0x14, 1);

            Func_080164d4(settings_window, 0xa0, 0x28, 0xc8, 0x30);
            Func_0801e7c0(
                S8_AT(state, 0x596) + 0xc0a,
                settings_window, 0xa0, 0x28);
            Func_080164d4(settings_window, 0xa0, 0x40, 0xb8, 0x48);
            Func_0801e7c0(
                S8_AT(state, 0x597) + 0xc10,
                settings_window, 0xa0, 0x40);
            Func_080164d4(settings_window, 0xa0, 0x58, 0xb8, 0x60);
            Func_0801e7c0(
                S8_AT(state, 0x598) + 0xc13,
                settings_window, 0xa0, 0x58);

            Func_0801ccc0(S8_AT(state, 0x594), S8_AT(state, 0x595));
            y = ((selection * 3 + U16_AT(settings_window, 0x0e)) * 8) + 4;
            if (selection == 0)
                y += 8;
            Func_080b0030(
                state + 0x5a4,
                U16_AT(settings_window, 0x0c) * 8,
                y, 3);
            Func_080164ac(selection_window);
            Func_0801e74c(
                selection + 0xc15, selection_window, 0, 0);
        }

        Func_080216b4(cursor);
        Func_080030f8(1);

        if (*(u32 *)0x03001c94 & 4) {
            u16 preset;
            Func_080f9010(0x70);
            preset = U16_AT(state, 0x57e) + 1;
            U16_AT(state, 0x57e) = preset;
            if (preset > 5) {
                preset = 0;
                U16_AT(state, 0x57e) = 0;
            }
            state[0x594] = U8_AT((void *)0x080367d0, preset);
            state[0x595] = U8_AT((void *)0x080367d6, preset);
            redraw = 1;
            continue;
        }

        if (!(*(u32 *)0x03001c94 & 9)) {
            if (*(u32 *)0x03001c94 & 2) {
                result = -1;
                Func_080f9010(0x71);
                break;
            }

            if (*(u32 *)0x03001b04 & 0x40) {
                Func_080f9010(0x6f);
                selection--;
                redraw = 1;
            } else if (*(u32 *)0x03001b04 & 0x80) {
                Func_080f9010(0x6f);
                selection++;
                redraw = 1;
            } else {
                if (*(u32 *)0x03001b04 & 0x20) {
                    Func_080f9010(0x6f);
                    state[0x594 + selection]--;
                    redraw = 1;
                }
                if (*(u32 *)0x03001b04 & 0x10) {
                    Func_080f9010(0x6f);
                    state[0x594 + selection]++;
                    redraw = 1;
                }
            }
        } else {
            result = 0;
            Func_080f9010(0x70);
            break;
        }
    }

    Func_08016418(selection_window, 2);
    Func_08016418(settings_window, 2);
    if (result == 0) {
        *(u8 *)0x02000445 = state[0x594];
        *(u8 *)0x02000446 = state[0x595];
        *(u8 *)0x0200044c = state[0x596];
        *(u8 *)0x0200044a = state[0x597];
        *(u8 *)0x0200046a = state[0x598];
        *(u8 *)0x03001d08 = state[0x598];
    } else {
        Func_0801ccc0(*(s8 *)0x02000445, *(s8 *)0x02000446);
    }
    Func_0801d0f0();
    Func_080030f8(1);
    return result;
}

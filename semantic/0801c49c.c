#include "types.h"

struct ChoiceEntry {
    u16 value;
    u16 effect;
};

struct Window {
    u8 unknown_00[0x0e];
    u16 tile_y;
};

struct CursorObject {
    u8 unknown_00[8];
    s16 y_08;
    u8 unknown_0a[0x0a];
    s8 y_14;
};

struct EffectDefinition {
    u8 unknown_00[9];
    u8 icon;
};

void *Func_08004938(s32 size);
void Func_08077088(void);
s32 Func_0801c7fc(struct ChoiceEntry *);
void Func_0801c8a0(s32 *, s32 *, struct ChoiceEntry *);
struct Window *Func_080162d4(s32, s32, s32, s32, s32);
void *Func_08004080(void);
void Func_08003fa4(void *, s32, const void *);
struct CursorObject *Func_0801eadc(void *, u32, struct Window *, s32, s32);
s32 Func_08002304(s32, s32);
void Func_08016498(struct Window *);
void Func_0801e41c(struct Window *, u32, u32, u32, u32);
void Func_0801e7c0(s32, struct Window *, s32, s32);
struct EffectDefinition *Func_08077080(s32);
void Func_0801e9d4(s32, s32, struct Window *, s32, s32);
void Func_080030f8(u32);
void Func_080f9010(s32);
void Func_08016418(struct Window *, s32);
s32 Func_08002df0(void *);

s32 Func_0801c49c(void)
{
    struct ChoiceEntry *choices = Func_08004938(0x700);
    struct Window *main_window;
    struct Window *title_window;
    struct Window *icon_window;
    struct CursorObject *cursor = 0;
    s32 first = 0;
    s32 second = 0;
    s32 selected_column = 0;
    s32 redraw = 1;
    s32 count;
    volatile s32 branch_barrier = 0;
    void *resource;

    Func_08077088();
    Func_08077088();
    Func_08077088();
    Func_08077088();
    Func_08077088();
    Func_08077088();
    Func_08077088();

    count = Func_0801c7fc(choices);
    if (count == 0)
        goto cleanup;

    Func_0801c8a0(&first, &second, choices);
    main_window = Func_080162d4(4, 6, 0x14, 7, 2);
    title_window = Func_080162d4(4, 3, 0x14, 3, 2);
    icon_window = Func_080162d4(4, 0x0e, 0x14, 5, 2);

    resource = Func_08004080();
    if (resource != 0) {
        Func_08003fa4(resource, 0x80, (const void *)0x080310a4);
        cursor = Func_0801eadc(
            resource, 0x40000000, main_window, 0, 0);
    }
    Func_0801e7c0(0xb19, title_window, 0x10, 0);

    for (;;) {
        if (redraw) {
            struct ChoiceEntry *shown;
            struct EffectDefinition *effect;
            s32 cursor_y;

            redraw = 0;
            first = Func_08002304(first + count, count);
            selected_column = (selected_column + 2) & 1;
            second = Func_08002304(second + count, count);

            cursor_y =
                selected_column * 0x10 + main_window->tile_y * 8 + 0x1c;
            cursor->y_08 = cursor_y;
            cursor->y_14 = cursor_y;

            Func_08016498(main_window);
            Func_0801e41c(main_window, 1, 2, 0x11, 2);
            Func_0801e7c0(0xb1e, main_window, 0x30, 0);
            Func_0801e7c0(
                choices[first].effect + 0x333, main_window, 0x38, 0x10);
            Func_0801e7c0(
                choices[second].effect + 0x333, main_window, 0x38, 0x20);
            Func_0801e7c0(0xb1c, main_window, 0x10, 0x10);
            Func_0801e7c0(0xb1d, main_window, 0x10, 0x20);
            Func_0801e7c0(
                choices[first].value + 0x66, main_window, 0x68, 0x10);
            Func_0801e7c0(
                choices[second].value + 0x66, main_window, 0x68, 0x20);

            Func_08016498(icon_window);
            Func_0801e7c0(0xaec, icon_window, 0, 0x10);
            if (selected_column) {
                shown = &choices[second];
                effect = Func_08077080(shown->effect);
            } else {
                shown = &choices[first];
                effect = Func_08077080(shown->effect);
            }
            Func_0801e9d4(effect->icon, 2, icon_window, 0x40, 0x10);
            Func_0801e7c0(
                shown->effect + 0x53a, icon_window, 0, 0);
        }

        Func_080030f8(1);
        if (*(volatile u32 *)0x03001b04 & 0x20) {
            Func_080f9010(0x6f);
            if (selected_column)
                second--;
            else
                first--;
            redraw = 1;
        }
        if (*(volatile u32 *)0x03001b04 & 0x10) {
            Func_080f9010(0x6f);
            if (selected_column)
                second++;
            else
                first++;
            redraw = 1;
        }
        if (*(volatile u32 *)0x03001b04 & 0x40) {
            Func_080f9010(0x6f);
            selected_column--;
            redraw = 1;
        }
        if (*(volatile u32 *)0x03001b04 & 0x80) {
            Func_080f9010(0x6f);
            selected_column++;
            redraw = 1;
        }

        if (*(volatile u32 *)0x03001c94 & 1) {
            Func_080f9010(0x70);
            break;
        }
        if (*(volatile u32 *)0x03001c94 & 2) {
            Func_080f9010(0x71);
            branch_barrier = 1;
            break;
        }
        if (*(volatile u32 *)0x03001c94 & 8) {
            Func_080f9010(0x71);
            branch_barrier = 2;
            break;
        }
    }

    *(volatile u16 *)0x02000460 =
        (choices[first].value << 10) | choices[first].effect;
    *(volatile u16 *)0x02000462 =
        (choices[second].value << 10) | choices[second].effect;

    Func_08016418(main_window, 1);
    Func_08016418(title_window, 1);
    Func_08016418(icon_window, 1);
    Func_080030f8(1);
cleanup:
    (void)branch_barrier;
    return Func_08002df0(choices);
}

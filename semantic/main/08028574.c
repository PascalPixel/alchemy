typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_080030f8(u32);
void Func_08016478(void *);
void Func_0801e7c0(s32, void *, s32, s32);
void Func_080f9010(s32);

/*
 * Run the single-value cyclic selector. Redraw the current label, wrap on
 * either horizontal or vertical directions, and return the selected index on
 * confirmation or -1 on cancellation.
 */
s32 Func_08028574(s32 initial_selection) {
    u8 *state;
    void *window;
    s16 selection;
    s16 count;
    s16 label_base;
    s32 label;
    u32 pressed;
    u32 direction;

    state = *(u8 **)0x03001F38;
    window = *(void **)(state + 0x78);
    selection = (s16)initial_selection;
    count = *(s16 *)(state + 0x8E);
    label_base = *(s16 *)(state + 0x92);

redraw:
    *(s16 *)(state + 0x8C) = selection;
    Func_08016478(window);
    if (label_base != 0) {
        label = label_base + selection;
    } else {
        label = state[0x84 + selection] + 0x1F;
    }
    Func_0801e7c0(label, window, 0, 0);

    for (;;) {
        Func_080030f8(1);
        pressed = *(u32 *)0x03001C94;
        if (pressed & 1) {
            Func_080f9010(0x70);
            return selection;
        }
        if (pressed & (2 | 8)) {
            Func_080f9010(0x71);
            return -1;
        }

        direction = *(u32 *)0x03001B04;
        if (direction & (0x20 | 0x40)) {
            Func_080f9010(0x6F);
            selection--;
            if (selection < 0) {
                selection = count - 1;
            }
            goto redraw;
        }
        if (direction & (0x10 | 0x80)) {
            Func_080f9010(0x6F);
            selection++;
            if (selection >= count) {
                selection = 0;
            }
            goto redraw;
        }
    }
}

#include "types.h"

struct Display_080a4800 {
    u8 padding00[5];
    u8 status;
};

struct State_080a4800 {
    u8 padding000[0x10];
    void *background_window;
    u8 padding014[0x168];
    struct Display_080a4800 *mode_display;
    u8 padding180[0x9C];
    struct Display_080a4800 *top_display;
};

typedef char Display_080a4800_size[
    sizeof(struct Display_080a4800) == 8 ? 1 : -1
];
typedef char State_080a4800_size[
    sizeof(struct State_080a4800) == 0x220 ? 1 : -1
];

s32 Func_080022fc(s32, s32);
void Func_080030f8(s32);
s32 Func_080041d8(void (*)(void), s32);
s32 Func_08004278(void (*)(void));
void *Func_08015010(s32, s32, s32, s32, s32);
void Func_08015018(void *, s32);
void Func_08015270(void *);
void *Func_08015408(s32, s32, s32, s32);
s32 Func_080770c0(s32);
void Func_080a19a0(void);
void Func_080a2144(s32);
void Func_080a22f4(void);
void Func_080a4924(void *, s32);

s32 Func_080a4800(s32 value)
{
    struct State_080a4800 *state;
    void *window;
    s32 selection;
    s32 dirty;

    dirty = 1;
    state = *(struct State_080a4800 **)0x03001F2C;
    state->top_display->status = 13;

    window = Func_08015010(0, 0, 30, 10, 2);
    Func_08004278(Func_080a19a0);
    state->mode_display->status = 13;
    Func_080a22f4();

    selection = 0;
    Func_080030f8(1);
    for (;;) {
        if (Func_080770c0(0x150) != 0)
            break;

        if (dirty != 0) {
            dirty = 0;
            selection = Func_080022fc(selection + 5, 5);
            Func_080a4924(window, value);
        }

        if (*(u32 *)0x03001C94 & 1)
            break;
        if (*(u32 *)0x03001C94 & 2) {
            selection = -1;
            break;
        }

        if (*(u32 *)0x03001B04 & 0x40) {
            selection--;
            dirty = 1;
        }
        if (*(u32 *)0x03001B04 & 0x80) {
            selection++;
            dirty = 1;
        }
        Func_080030f8(1);
    }

    Func_08015270(window);
    Func_080030f8(1);
    Func_08015018(window, 1);
    Func_08015270(state->background_window);
    Func_080a2144(0xE);
    Func_080041d8(Func_080a19a0, 0xC80);
    state->mode_display->status = 1;
    Func_08015408(13, 0, 17, 10);
    return selection;
}

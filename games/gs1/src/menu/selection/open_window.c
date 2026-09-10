#include "types.h"

struct UiWork {
    u8 pad0[8];
    u16 x;
};

struct Node {
    u8 pad0[32];
    u16 glyph;
};

struct Screen {
    u8 pad0[0x350];
    struct UiWork *window;
    u8 pad1[0x394 - 0x354];
    u16 f394;
    u8 pad2[0x3a0 - 0x396];
    u16 f3a0;
    u8 pad3[0x3b8 - 0x3a2];
    u16 f3b8;
};

extern struct Screen *Data_03001e98;
extern u8 Data_00000050[];
extern u8 Data_00000051[];

struct UiWork *Func_080162d4(s32, s32, s32, s32, s32);
void Func_08016418(struct UiWork *, s32);
void Func_08016478(struct UiWork *);
struct Node *Func_0801b36c(struct Screen *, u32);
void Func_0801e7c0(s32, struct UiWork *, s32, s32);

void Func_0801b010(s32 mode, u32 count)
{
    struct Screen *screen;
    struct Node *node;
    struct UiWork **slot;
    struct UiWork *window;

    screen = Data_03001e98;
    node = Func_0801b36c(screen, count);
    slot = &screen->window;
    window = *slot;
    if (window == 0) {
        if (mode == 6) {
            if (screen->f3b8 != 0) {
                *slot = Func_080162d4(17, 17, 5, 3, mode);
            } else {
                *slot = Func_080162d4(17, 0, 5, 3, mode);
            }
            screen->f3a0 = 0;
            screen->f3b8 = 999;
        } else {
            *slot = Func_080162d4(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
        }
        Func_08016478(screen->window);
    } else {
        if (count != 0 && window->x != count + 2) {
            Func_08016418(window, 2);
            *slot = Func_080162d4(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
        }
        Func_08016478(screen->window);
    }
    if (screen->f394 != 0) {
        Func_0801e7c0(node->glyph, screen->window, 0, 0);
    } else {
        switch (mode) {
        case 4:
            Func_0801e7c0((s32)Data_00000051, screen->window, 0, 0);
            break;
        case 2:
            Func_0801e7c0((s32)Data_00000050, screen->window, 0, 0);
            break;
        }
    }
}

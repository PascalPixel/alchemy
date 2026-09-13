#include "types.h"
#include "scene.h"

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

extern struct Screen *gIw;
extern u8 gVal[];
extern u8 gVal2[];

struct UiWork *UiWindow_Create(s32, s32, s32, s32, s32);

struct Node *NodeChain_GetNodeAtCount(struct Screen *, u32);
void UiText_DrawCharacterAtOffset(s32, struct UiWork *, s32, s32);
void UiWork_Finalize(struct UiWork *, s32);
void RenderOutput_PrepareForRedraw(struct UiWork *);

void Menu_OpenSelectionWindow(s32 mode, u32 count)
{
    struct Screen *screen;
    struct Node *node;
    struct UiWork **slot;
    struct UiWork *window;

    screen = gIw;
    node = NodeChain_GetNodeAtCount(screen, count);
    slot = &screen->window;
    window = *slot;
    if (window == 0) {
        if (mode == 6) {
            if (screen->f3b8 != 0) {
                *slot = UiWindow_Create(17, 17, 5, 3, mode);
            } else {
                *slot = UiWindow_Create(17, 0, 5, 3, mode);
            }
            screen->f3a0 = 0;
            screen->f3b8 = 999;
        } else {
            *slot = UiWindow_Create(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
        }
        RenderOutput_PrepareForRedraw(screen->window);
    } else {
        if (count != 0 && window->x != count + 2) {
            UiWork_Finalize(window, 2);
            *slot = UiWindow_Create(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
        }
        RenderOutput_PrepareForRedraw(screen->window);
    }
    if (screen->f394 != 0) {
        UiText_DrawCharacterAtOffset(node->glyph, screen->window, 0, 0);
    } else {
        switch (mode) {
        case 4:
            UiText_DrawCharacterAtOffset((s32)gVal2, screen->window, 0, 0);
            break;
        case 2:
            UiText_DrawCharacterAtOffset((s32)gVal, screen->window, 0, 0);
            break;
        }
    }
}

#include "TYPES.H"
#include "SCENE.H"

struct SlotEntry {
    struct SlotEntry *next;
    u8 y;
    u8 affine : 2;
    u8 mode : 2;
    u8 mosaic : 1;
    u8 colors : 1;
    u8 shape : 2;
    u16 x : 9;
    u16 param : 5;
    u16 size : 2;
    u16 tile : 10;
    u16 prio : 2;
    u16 pal : 4;
};

struct MenuNode {
    s32 unknown_00;
    struct MenuNode *next;
    u16 offset;
    u16 active;
    u16 handle;
    u16 tile_id;
    s16 x, y, dx, dy, x_end, y_end;
    u8 unknown_1c[6];
    s16 scale, scale_step, scale_end;
    struct SlotEntry entry;
};

struct MenuSelection {
    struct MenuNode nodes[2];
    u8 unknown_68[0x27a];
    u16 mode;
};

extern u32 gFrameTick;
extern u8 Menu_CursorObjectTiles[], Menu_CursorLeftObjectTiles[];
extern s32 GameFlag_IsSet(s32 flag);
extern void Runtime_PushSlotEntry(s32 *entry, s32 slot);
extern s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);

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

extern struct Screen *gResQueueWork;
extern u8 MsgItemNotHeld[];
extern u8 MsgAbilityNotKnown[];
struct UiWork *UiWindow_Create(s32, s32, s32, s32, s32);
struct Node *NodeChain_GetNodeAtCount(struct Screen *, u32);
void UiText_DrawCharacterAtOffset(s32, struct UiWork *, s32, s32);
void UiWork_Finalize(struct UiWork *, s32);
void RenderOutput_PrepareForRedraw(struct UiWork *);

void MenuSelection_DrawSideMarker(struct MenuSelection *state, s32 index)
{
    u32 frame = (gFrameTick >> 2) & 7;
    struct SlotEntry *entry;
    u8 *frames;
    u8 *tmp;

    if (state->nodes[index].active == 0)
        return;
    entry = &state->nodes[index].entry;
    entry->x = state->nodes[index].x;
    entry->y = state->nodes[index].y;
    if (index != 0) {
        frames = Menu_CursorObjectTiles;
        if (state->nodes[1].offset != 0)
            entry->x = entry->x + state->nodes[1].offset;
    } else {
        frames = Menu_CursorLeftObjectTiles;
        if (state->nodes[0].offset != 0)
            entry->x = entry->x - state->nodes[0].offset;
    }
    tmp = frames + frame * 128;
    entry->tile = VramBlock_LoadCached(state->nodes[index].handle, 128, tmp);
    if (GameFlag_IsSet(0x103)) {
        if (state->mode == 1)
            entry->mode = 1;
        else
            entry->mode = 0;
    }
    Runtime_PushSlotEntry((s32 *)entry, 238);
    if (state->nodes[index].offset != 0)
        state->nodes[index].offset--;
}

void Menu_OpenSelectionWindow(s32 mode, u32 count)
{
    struct Screen *screen;
    struct Node *node;
    struct UiWork **slot;
    struct UiWork *window;

    screen = gResQueueWork;
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
            UiText_DrawCharacterAtOffset((s32)MsgAbilityNotKnown, screen->window, 0, 0);
            break;
        case 2:
            UiText_DrawCharacterAtOffset((s32)MsgItemNotHeld, screen->window, 0, 0);
            break;
        }
    }
}

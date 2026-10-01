#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"

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
    u8 unknown_2e4[0x394 - 0x2e4];
    u16 count;
    u16 first;
    u16 second;
    u16 unknown_39a;
    u16 other;
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

void Runtime_ReleaseHeapBlock(u32 value);

struct Node_0801b148 {
    u32 value0;
    struct Node_0801b148 *next;
    u16 value8;
    u16 active;
    u16 handle;
};

struct Work;
void Resource_ScheduleOwnerReset(void);
void WaitFrames(s32);
s32 Resource_ResetEntry(u32 index);
void Resource_ResetPendingTransfer(void);

void Menu_SetupSelectionSide(struct MenuSelection *state, s32 index);

/* menu/selection/set_node_coordinates.c */
struct Node_0801b1ec {
    u8 filler0[4];
    struct Node_0801b1ec *next;
    u8 filler8[8];
    u16 first1;
    u16 second1;
    u8 filler14[4];
    u16 first2;
    u16 second2;
};

struct State_0801b1ec {
    u8 filler0[0x348];
    struct Node_0801b1ec *head;
    u8 filler34c[0x4a];
    u16 first;
    u16 second;
};

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

void Resource_ResetOwnerEntries(void)
{
    u8 *state = gResQueueWork;
    struct Node_0801b148 *node;

    Resource_ScheduleOwnerReset();
    UiWork_Finalize(*(struct Work **)(state + 0x350), 2);
    WaitFrames(1);
    node = *(struct Node_0801b148 **)(state + 0x348);
    while (node != 0) {
        if (node->active != 0) {
            Resource_ResetEntry(node->handle);
            node->active = 0;
        }
        node = node->next;
    }
    node = *(struct Node_0801b148 **)(state + 0x34c);
    while (node != 0) {
        if (node->active != 0) {
            Resource_ResetEntry(node->handle);
            node->active = 0;
        }
        node = node->next;
    }
    Resource_ResetPendingTransfer();
    if (*(s16 *)(state + 18) != 0) {
        Resource_ResetEntry(*(u16 *)(state + 12));
        if (*(s16 *)(state + 18) != 0) {
            Resource_ResetEntry(*(u16 *)(state + 64));
        }
    }
    Resource_ResetEntry(*(u16 *)(state + 0x2e4));
    Runtime_ReleaseHeapBlock(18);
}

void Menu_SetNodeCoordinates(u32 first, u32 second)
{
    struct State_0801b1ec *state = gResQueueWork;
    struct Node_0801b1ec *node;

    state->first = first;
    state->second = second;
    node = state->head;
    while (node != 0) {
        node->first1 = first;
        node->first2 = first;
        node->second1 = second;
        node->second2 = second;
        node = node->next;
        first += 16;
    }
}

/* menu/selection/setup_both_sides.c */
void Menu_SetupSelectionBothSides(void)
{
    struct MenuSelection *state;

    state = (struct MenuSelection *)gResQueueWork;
    Menu_SetupSelectionSide(state, 0);
    Menu_SetupSelectionSide(state, 1);
}

extern s32 Resource_FindFreeEntry(void);

void Menu_SetupSelectionSide(struct MenuSelection *state, s32 index)
{
    struct SlotEntry *entry = &state->nodes[index].entry;
    u8 *frames = 0;
    u32 count;

    state->nodes[index].active = 0;
    if (index != 0) {
        /* FAKEMATCH: a separate local gives the early r5 load and late copy */
        u8 *right = Menu_CursorObjectTiles;

        count = state->count;
        if (state->other != 0)
            count -= state->other;
        if (count > 5) {
            state->nodes[index].active = 1;
            count = 5;
        }
        frames = right;
        state->nodes[1].x = state->first + 16 * (count - 1) + 17;
    } else {
        frames = Menu_CursorLeftObjectTiles;
        state->nodes[0].x = state->first - 9;
        if (state->other != 0)
            state->nodes[0].active = 1;
    }
    if (state->nodes[index].y == 0) {
        state->nodes[index].handle = Resource_FindFreeEntry();
        state->nodes[index].tile_id = VramBlock_LoadCached(state->nodes[index].handle, 128, frames);
        state->nodes[index].y = state->second;
        state->nodes[index].offset = 0;
        entry->mode = 0;
        entry->mosaic = 0;
        entry->colors = 1;
        entry->affine = 0;
        entry->param = 0;
        entry->size = 0;
        entry->shape = 2;
        entry->prio = 0;
    }
}

#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"
#include "SELECT.H"
#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"

extern u32 gFrameTick;
extern u8 Menu_CursorObjectTiles[], Menu_CursorLeftObjectTiles[];
extern s32 GameFlag_IsSet(s32 flag);

extern struct SelectionScreen *gResQueueWork;
extern u8 MsgItemNotHeld[];
extern u8 MsgAbilityNotKnown[];
struct SelectionNode *NodeChain_GetNodeAtCount(struct SelectionScreen *);
void UiText_DrawCharacterAtOffset(s32, struct UiWindow *, s32, s32);
void RenderOutput_PrepareForRedraw(struct UiWindow *);


struct Work;
void Resource_ScheduleOwnerReset(void);
void WaitFrames(s32);
void Resource_ResetPendingTransfer(void);

void Menu_SetupSelectionSide(struct SelectionScreen *state, s32 index);

/* menu/selection/set_node_coordinates.c */
void MenuSelection_DrawSideMarker(struct SelectionScreen *state, s32 index)
{
    u32 frame = (gFrameTick >> 2) & 7;
    struct SelectionSprite *entry;
    u8 *frames;
    u8 *tmp;

    if (state->records[index].kind == 0)
        return;
    entry = &state->records[index].sprite;
    entry->x = state->records[index].x;
    entry->y = state->records[index].y;
    if (index != 0) {
        frames = Menu_CursorObjectTiles;
        if (state->records[1].base != 0)
            entry->x = entry->x + state->records[1].base;
    } else {
        frames = Menu_CursorLeftObjectTiles;
        if (state->records[0].base != 0)
            entry->x = entry->x - state->records[0].base;
    }
    tmp = frames + frame * 128;
    entry->tile = VramBlock_LoadCached(state->records[index].slot, 128, tmp);
    if (GameFlag_IsSet(0x103)) {
        if (state->records[14].kind == 1)
            entry->mode = 1;
        else
            entry->mode = 0;
    }
    Runtime_PushSlotEntry((s32 *)entry, 238);
    if (state->records[index].base != 0)
        state->records[index].base--;
}

void Menu_OpenSelectionWindow(s32 mode, u32 count)
{
    struct SelectionScreen *screen;
    struct SelectionNode *node;
    struct UiWindow **slot;
    struct UiWindow *window;

    screen = gResQueueWork;
    node = NodeChain_GetNodeAtCount(screen);
    slot = &screen->window;
    window = *slot;
    if (window == 0) {
        if (mode == 6) {
            if (screen->locked != 0) {
                *slot = UiWindow_Create(17, 17, 5, 3, mode);
            } else {
                *slot = UiWindow_Create(17, 0, 5, 3, mode);
            }
            screen->vertical_scroll = 0;
            screen->locked = 999;
        } else {
            *slot = UiWindow_Create(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
        }
        RenderOutput_PrepareForRedraw(screen->window);
    } else {
        if (count != 0 && window->width != count + 2) {
            UiWork_Finalize(window, 2);
            *slot = UiWindow_Create(19 + ((9 - count) >> 1), 17, count + 2, 3, 6);
        }
        RenderOutput_PrepareForRedraw(screen->window);
    }
    if (screen->count != 0) {
        UiText_DrawCharacterAtOffset(node->message, screen->window, 0, 0);
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
    struct SelectionScreen *state = gResQueueWork;
    struct SelectionNode *node;

    Resource_ScheduleOwnerReset();
    UiWork_Finalize(state->window, 2);
    WaitFrames(1);
    node = state->head;
    while (node != 0) {
        if (node->kind != 0) {
            Resource_ResetEntry(node->slot);
            node->kind = 0;
        }
        node = node->next;
    }
    node = state->path;
    while (node != 0) {
        if (node->kind != 0) {
            Resource_ResetEntry(node->slot);
            node->kind = 0;
        }
        node = node->next;
    }
    Resource_ResetPendingTransfer();
    if (state->records[0].y != 0) {
        Resource_ResetEntry(state->records[0].slot);
        if (state->records[0].y != 0) {
            Resource_ResetEntry(state->records[1].slot);
        }
    }
    Resource_ResetEntry(state->records[14].slot);
    Runtime_ReleaseHeapBlock(18);
}

void Menu_SetNodeCoordinates(u32 first, u32 second)
{
    struct SelectionScreen *state = gResQueueWork;
    struct SelectionNode *node;

    state->base_x = first;
    state->base_y = second;
    node = state->head;
    while (node != 0) {
        node->x = first;
        node->target_x = first;
        node->y = second;
        node->target_y = second;
        node = node->next;
        first += 16;
    }
}

/* menu/selection/setup_both_sides.c */
void Menu_SetupSelectionBothSides(void)
{
    struct SelectionScreen *state;

    state = (struct SelectionScreen *)gResQueueWork;
    Menu_SetupSelectionSide(state, 0);
    Menu_SetupSelectionSide(state, 1);
}


void Menu_SetupSelectionSide(struct SelectionScreen *state, s32 index)
{
    struct SelectionSprite *entry = &state->records[index].sprite;
    u8 *frames = 0;
    u32 count;

    state->records[index].kind = 0;
    if (index != 0) {
        /* FAKEMATCH: a separate local gives the early r5 load and late copy.
           2026-10-02: assigning frames here directly changes allocation
           throughout the function, including the later node accesses. */
        u8 *right = Menu_CursorObjectTiles;

        count = state->count;
        if (state->top != 0)
            count -= state->top;
        if (count > 5) {
            state->records[index].kind = 1;
            count = 5;
        }
        frames = right;
        state->records[1].x = state->base_x + 16 * (count - 1) + 17;
    } else {
        frames = Menu_CursorLeftObjectTiles;
        state->records[0].x = state->base_x - 9;
        if (state->top != 0)
            state->records[0].kind = 1;
    }
    if (state->records[index].y == 0) {
        state->records[index].slot = Resource_FindFreeEntry();
        state->records[index].tile = VramBlock_LoadCached(state->records[index].slot, 128, frames);
        state->records[index].y = state->base_y;
        state->records[index].base = 0;
        entry->mode = 0;
        entry->mosaic = 0;
        entry->colors = 1;
        entry->affine = 0;
        entry->param = 0;
        entry->size = 0;
        entry->shape = 2;
        entry->priority = 0;
    }
}

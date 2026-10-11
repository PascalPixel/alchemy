#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "select.h"
#include "node_chain.h"
#include "types.h"
#include "io_reg.h"
#include "scene.h"
#include "sound_ids.h"
#include "layout_guard.h"
#include "text/msg_ids.h"
#include "vram_block.h"

#define Menu_OpenSelectionWindow ((void (*)(u16,u32))Menu_OpenSelectionWindow)

#define SELECTION_FIXED 999

#define NODE_STATUS_SCROLLING 33
#define NODE_STATUS_SHOWN 1

#define NODE_KIND_YES_NO 6

#define SELECTION_ROWS 4
#define ARROW_SCROLL_FRAMES 8

s32 Menu_SelectionLoop(s32 mode)
{
    struct SelectionScreen *screen = gResQueueWork;

    Menu_LoadSelectionNodeResource(screen, 0);
    for (;;) {
        WaitFrames(1);
        if (screen->vertical_scroll != 0) {
            continue;
        }
        if (mode != SELECTION_FIXED) {
            if (gKeysRepeat & KEY_RIGHT) {
                Menu_StepRight(screen);
            } else if (gKeysRepeat & KEY_LEFT) {
                Menu_StepLeft(screen);
            } else if (gKeyState & KEY_A) {
                return Menu_ConfirmSelection(screen);
            }
        }
        if (mode != 0 && (gKeyState & KEY_B)) {
            return -1;
        }
    }
}

u32 Menu_WaitForSelectionInput(u32 mode)
{
    struct SelectionScreen *screen = gResQueueWork;
    u32 result;

    for (;;) {
        WaitFrames(1);
        if (screen->vertical_scroll != 0) {
            continue;
        }

        if (mode != SELECTION_FIXED) {
            if (gKeysRepeat & KEY_RIGHT) {
                AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
                Menu_StepRight(screen);
            } else if (gKeysRepeat & KEY_LEFT) {
                AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
                Menu_StepLeft(screen);
            }

            if (gKeyState & KEY_A) {
                result = screen->top + screen->cursor_index;
                if (screen->head->kind == NODE_KIND_YES_NO) {
                    if (result == 0)
                        AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
                    else
                        AudioCommand_PlayFar(SOUND_MENU_CANCEL);
                } else {
                    AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
                }
                return result;
            }
        }

        if (mode != 0 && (gKeyState & KEY_B)) {
            AudioCommand_PlayFar(SOUND_MENU_CANCEL);
            return -1;
        }
    }
}

void Menu_MoveSelectionForward(struct SelectionScreen *screen)
{
    u32 end = screen->top + screen->cursor_index + 1;

    if (end == screen->count) {
        return;
    }
    Menu_ReloadNodeResource(screen, screen->cursor_index);
    screen->status = NODE_STATUS_SCROLLING;
    WaitFrames(1);
    screen->cursor_index++;
    if (screen->cursor_index == SELECTION_ROWS && end + 1 < screen->count) {
        screen->cursor_index--;
        screen->records[1].base = ARROW_SCROLL_FRAMES;
        screen->top++;
        Menu_ScrollSelectionList(screen, TRUE);
        if (screen->top + screen->cursor_index + 2 == screen->count) {
            screen->records[1].kind = FALSE;
        }
        screen->records[0].kind = TRUE;
    }
    screen->status = NODE_STATUS_SHOWN;
    Menu_LoadSelectionNodeResource(screen, screen->cursor_index);
    WaitFrames(1);
    Menu_OpenSelectionWindow(screen->head->kind, 0);
    WaitFrames(1);
}

void Menu_MoveSelectionBackward(struct SelectionScreen *screen)
{
    u32 cursor;

    if (screen->top == 0 && screen->cursor_index == 0) {
        return;
    }
    Menu_ReloadNodeResource(screen, screen->cursor_index);
    screen->status = NODE_STATUS_SCROLLING;
    WaitFrames(1);
    cursor = screen->cursor_index;
    if (cursor == 1 && screen->top != 0) {
        screen->records[0].base = ARROW_SCROLL_FRAMES;
        screen->top--;
        Menu_ScrollSelectionList(screen, FALSE);
        if (screen->top == 0) {
            screen->records[0].kind = FALSE;
        }
        screen->records[1].kind = cursor;
    } else {
        screen->cursor_index--;
    }
    screen->status = NODE_STATUS_SHOWN;
    Menu_LoadSelectionNodeResource(screen, screen->cursor_index);
    WaitFrames(1);
    Menu_OpenSelectionWindow(screen->head->kind, 0);
    WaitFrames(1);
}

void Menu_StepRight(struct SelectionScreen *state)
{
    struct SelectionNode *node;
    u16 *ids;
    s32 y;
    u32 end;

    Menu_ReloadNodeResource(state, state->cursor_index);
    state->status = 33;
    WaitFrames(1);
    state->cursor_index++;
    if (state->count > 5) {
        end = state->top + state->cursor_index;
        if (end == state->count) {
            node = state->head;
            state->records[0].kind = 0;
            while (node->next != NULL) {
                node->target_x = state->base_x;
                node->target_y = state->base_y;
                node->dx = -12;
                node = node->next;
            }
            node->target_x = state->base_x;
            node->target_y = state->base_y;
            node->dx = -12;
            while (node->target_x != node->x)
                WaitFrames(1);
            node = state->head;
            if (node != NULL) {
                ids = state->kinds;
                do {
                    MenuSelection_SetupEntry(ids[0], ids[16], node, 1);
                    node = node->next;
                    ids++;
                } while (node != NULL);
            }
            state->cursor_index = 0;
            state->top = 0;
            node = state->head;
            y = node->x + 16;
            node = node->next;
            while (node != NULL) {
                node->target_x = y;
                node->dx = 12;
                node = node->next;
                y += 16;
            }
            state->records[1].kind = 1;
        } else if (state->cursor_index == 4 && end + 1 < state->count) {
            state->cursor_index--;
            state->records[1].base = 8;
            state->top++;
            Menu_ScrollSelectionList(state, 1);
            if (state->top + state->cursor_index + 2 == state->count)
                state->records[1].kind = 0;
            state->records[0].kind = 1;
        }
    } else if (state->cursor_index == state->count) {
        state->cursor_index = 0;
    }
    state->status = 1;
    Menu_LoadSelectionNodeResource(state, state->cursor_index);
    WaitFrames(1);
    Menu_OpenSelectionWindow(state->head->kind, 0);
    WaitFrames(1);
}

void Menu_StepLeft(struct SelectionScreen *state)
{
    struct SelectionNode *node;
    s32 i;

    Menu_ReloadNodeResource(state, state->cursor_index);
    state->status = 33;
    WaitFrames(1);
    if (state->count > 5) {
        if ((state->cursor_index | state->top) != 0) {
            if (state->cursor_index == 1 && state->top != 0) {
                state->records[0].base = 8;
                state->top--;
                Menu_ScrollSelectionList(state, 0);
                if (state->top == 0)
                    state->records[0].kind = 0;
                state->records[1].kind = 1;
            } else {
                state->cursor_index--;
            }
        } else {
            node = state->head;
            i = 64;
            { register s32 z asm("r0") = 0; /* FAKEMATCH: the zero goes through r0 */
            state->records[1].kind = z; }
            while (node->next != NULL) {
                node->target_x = node->x + i;
                node->dx = 12;
                node = node->next;
                i -= 16;
            }
            node = state->head;
            while (node->x != node->target_x)
                WaitFrames(1);
            i = 0;
            while (i != state->count - 5)
                i++;
            node = state->head;
            state->top = i;
            state->cursor_index = 4;
            while (node != NULL) {
                MenuSelection_SetupEntry(state->kinds[i], state->bases[i], node, 1);
                node = node->next;
                i++;
            }
            node = state->head;
            i = state->base_x;
            while (node->next != NULL) {
                node->target_x = i;
                node->dx = -12;
                node = node->next;
                i += 16;
            }
            state->records[0].kind = 1;
        }
    } else if (state->cursor_index != 0) {
        state->cursor_index--;
    } else {
        state->cursor_index = state->count - 1;
    }
    state->status = 1;
    Menu_LoadSelectionNodeResource(state, state->cursor_index);
    WaitFrames(1);
    Menu_OpenSelectionWindow(state->head->kind, 0);
    WaitFrames(1);
}

void Menu_ReloadNodeResource(struct SelectionScreen *state, u32 index)
{
    struct SelectionNode *node = state->head;
    u32 output;
    u32 value;

    while (index != 0) {
        index--;
        node = node->next;
    }
    if (node->kind == 1 || node->kind == 6) {
        u32 first = node->message - (u32)MsgCommandName;

        value = node->slot;
        Ui_BuildPairedPatternsToSlot(first, 0, &value, &output, 1);
    }
}

void Menu_LoadSelectionNodeResource(struct SelectionScreen *state, u32 index)
{
    struct SelectionNode *node = state->head;
    u32 res;
    u32 value;

    while (index != 0) {
        index--;
        node = node->next;
    }
    if (node->kind == 1 || node->kind == 6) {
        u32 id = node->message - (u32)MsgCommandName;

        value = node->slot;
        Ui_BuildPairedPatternsToSlot(id, 0, &value, &res, 1);
        Menu_LoadSelectedResource();
    }
}

void Menu_SendNodeCountList(struct SelectionScreen *screen)
{
    u16 data[6];
    struct SelectionNode *node = screen->head;
    s32 count = 0;

    while (node != 0) {
        node = node->next;
        count++;
    }
    data[count] = 0xff;
    BattlePres_SetActorModesFar(data, 0);
}

void Menu_ScrollSelectionList(struct SelectionScreen *screen, s32 forward)
{
    struct SelectionNode *p;
    struct SelectionNode *last;
    struct SelectionNode *first;
    s32 base;
    s32 kind;
    u32 index;
    s32 y;
    s32 z;

    if (forward != 0) {
        index = screen->top + SELECTION_ROWS;
        base = screen->bases[index];
        kind = screen->kinds[index];
        p = Resource_FindFreeTransferEntry(0);
        if (p == NULL)
            return;
        MenuSelection_SetupEntry(kind, base, p, 0);
        y = screen->base_x;
        p->x = y + 80;
        z = screen->base_y;
        p->target_x = y + 64;
        p->y = z;
        p->target_y = z;
        p->scale_step = 32;
        p->scale = 32;
        p->scale_end = 256;
        p->dx = -2;
        last = p;
        p = screen->head;
        p->scale_step = -32;
        p->scale_end = 0;
        for (;;) {
            p->target_x = p->x - 16;
            p->dx = -2;
            if (p->next == NULL)
                break;
            p = p->next;
        }
        p->next = last;
        last->next = NULL;
        last->prev = p;
        p = screen->head;
        do {
            WaitFrames(1);
        } while (p->scale != 0);
        screen->head = p->next;
        Resource_ResetEntry(p->slot);
        p->kind = 0;
        p = p->next;
        p->prev = NULL;
    } else {
        index = screen->top;
        base = screen->bases[index];
        kind = screen->kinds[index];
        p = Resource_FindFreeTransferEntry(0);
        if (p == NULL)
            return;
        MenuSelection_SetupEntry(kind, base, p, 0);
        p->x = screen->base_x - 16;
        p->y = screen->base_y;
        p->target_y = p->y;
        p->dx = 2;
        p->scale = 32;
        p->scale_step = 32;
        p->target_x = p->x + 16;
        p->scale_end = 256;
        first = p;
        p = screen->head;
        p->prev = first;
        first->next = p;
        first->prev = NULL;
        screen->head = first;
        p = first;
        for (;;) {
            p->target_x = p->x + 16;
            p->dx = 2;
            if (p->next == NULL)
                break;
            p = p->next;
        }
        p->scale_end = 0;
        p->scale_step = -32;
        p = screen->head;
        do {
            WaitFrames(1);
        } while (p->scale != 256);
        while (p->next != NULL)
            p = p->next;
        Resource_ResetEntry(p->slot);
        p->kind = 0;
        p->prev->next = NULL;
    }
}

s32 Resource_LoadByMode(s32 mode, s32 value)
{
    s32 output;
    s32 result = -1;

    switch (mode) {
    case 1:
    case 6:
        Ui_BuildPairedPatternsToSlot(value, 0, &result, &output, 0);
        break;
    case 2:
        result = Resource_FindFreeEntry();
        if (result == VRAM_CACHE_ENTRY_COUNT)
            return -1;
        UiIcon_CopyResourceToSlot(value, 0x1a, result);
        break;
    case 9:
        result = Resource_FindFreeEntry();
        if (result == VRAM_CACHE_ENTRY_COUNT)
            return -1;
        RenderResource_LoadTableEntry(value, 0, result);
        break;
    case 4:
        Ability_LoadGlyph(value, 1, &result, &output, 0);
        break;
    }
    return result;
}

s32 Resource_LoadByModeIntoSlot(s32 mode, s32 value, s32 result, s32 option)
{
    s32 output;
    s32 original = result;

    if (result == -1) {
        result = Resource_FindFreeEntry();
        if (result == VRAM_CACHE_ENTRY_COUNT)
            return original;
    }

    switch (mode) {
    case 1:
    case 6:
        Ui_BuildPairedPatternsToSlot(value, option, &result, &output, 1);
        break;
    case 2:
        UiIcon_CopyResourceToSlot(value, 58, result);
        break;
    case 7:
        UiIcon_CopyResourceToSlot(value, 42, result);
        break;
    case 4:
        Ability_LoadGlyph(value, option, &result, &output, 1);
        break;
    case 8:
        Ui_BuildPatternToSlot(value, 0, result);
        break;
    case 9:
        RenderResource_LoadTableEntry(value, 0, result);
        break;
    }

    return result;
}

void MenuSelection_SetupEntry(u32 kind, s32 base, struct SelectionNode *node, s32 reuse)
{
    s32 src;
    s32 tile;
    struct SelectionSprite *obj;

    switch (kind) {
    case 1:
    case 6:
        if (reuse != 0)
            src = node->slot;
        Ui_BuildPairedPatternsToSlot(base, 0, &src, &tile, reuse);
        node->message = base + (s32)MsgCommandName;
        break;
    case 2:
        if (reuse != 0)
            src = node->slot;
        UiIcon_BuildItemIconTiles(base, 1, &src, &tile, reuse);
        node->message = base + (s32)MsgItemName;
        break;
    case 4:
        if (reuse != 0)
            src = node->slot;
        Ability_LoadGlyph(base, 1, &src, &tile, reuse);
        node->message = base + (s32)MsgAbilityName;
        break;
    }

    node->base = base;
    node->slot = src;
    node->tile = tile;
    node->kind = kind;
    node->scale = 0x100;
    node->scale_end = 0x100;

    obj = &node->sprite;
    obj->mode = 0;
    obj->colors = 0;
    obj->mosaic = 0;

    obj->size = 1;
    obj->shape = 0;
    obj->palette = 0;
    obj->tile = tile;
    obj->priority = 0;
}

void WaitFrames(s32 frames);

u32 Menu_ConfirmSelection(struct SelectionScreen *screen)
{
    struct SelectionNode *node;
    struct SelectionNode *other = NULL;
    u32 cnt = 0;
    u32 index;
    struct SelectionSprite *sprite;

    index = screen->top + screen->cursor_index;
    Menu_SendNodeCountList(screen);
    Menu_ReloadNodeResource(screen, screen->cursor_index);
    screen->status = 33;
    WaitFrames(1);
    screen->records[0].kind = 0;
    screen->records[1].kind = 0;
    screen->records[14].kind = 0;
    screen->records[14].scale = 0;
    Resource_ResetPendingTransfer();
    node = screen->head;
    while (node != NULL && screen->cursor_index != cnt) {
        node = node->next;
        cnt++;
    }
    node->home_x = node->x;
    node->home_y = node->y;
    for (other = screen->head; other != NULL; other = other->next) {
        if (other != node) {
            other->target_x = node->x;
            other->dx = (node->x - other->x) >> 1;
        }
    }
    WaitFrames(2);
    for (other = screen->head; other != NULL; other = other->next) {
        if (other != node) {
            Resource_ResetEntry(other->slot);
            other->kind = 0;
        }
    }
    screen->head = node;
    node->prev = NULL;
    node->next = NULL;
    node->target_x = 4;
    for (other = screen->path, cnt = 0; other != NULL; other = other->next) {
        node->target_x += 16;
        cnt++;
    }
    screen->path_top[cnt] = screen->top;
    screen->path_cursor[cnt] = screen->cursor_index;
    node->dx = (node->target_x - node->x) >> 1;
    screen->unknown_39a = 0;
    screen->cursor_index |= 0x80;
    WaitFrames(2);
    other = Resource_FindFreeTransferEntry(1);
    other->kind = node->kind;
    other->message = node->message;
    other->base = node->base;
    other->slot = node->slot;
    other->tile = node->tile;
    other->x = node->x;
    other->y = node->y;
    other->target_x = other->x;
    other->target_y = other->y;
    other->home_x = node->home_x;
    other->home_y = node->home_y;
    other->dx = 0;
    other->dy = 0;
    other->scale = 0x100;
    other->scale_end = 0x100;
    sprite = &other->sprite;
    sprite->mode = 0;
    sprite->colors = 0;
    sprite->mosaic = 0;
    sprite->size = 1;
    sprite->shape = 0;
    sprite->palette = 0;
    sprite->tile = other->tile;
    node->kind = 0;
    screen->head = NULL;
    if (screen->path != NULL) {
        node = screen->path;
        while (node->next != NULL)
            node = node->next;
        node->next = other;
        other->prev = node;
        other->next = NULL;
    } else {
        screen->path = other;
        other->prev = NULL;
        other->next = NULL;
    }
    return index;
}

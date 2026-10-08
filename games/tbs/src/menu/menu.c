#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "affine.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "mem.h"
#include "select.h"
#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "node_chain.h"
#include "text/msg_ids.h"
s32 GameFlag_TestFar(s32 flag);

#define UiText_DrawCharacterAtOffset ((void (*)(s32, struct UiWindow *, s32, s32))UiText_DrawCharacterAtOffset)
#define Menu_AnimatedCursorTiles ((u8 *)&Menu_AnimatedCursorTiles)

#define Menu_CursorObjectTiles ((u8 (*))&Menu_CursorObjectTiles)

extern u32 gFrameTick;

void RenderOutput_PrepareForRedraw(struct UiWindow *);

struct Work;
void WaitFrames(s32);

struct EffectPosition {
    s32 x;
    s32 y;
    s32 z;
};

void MenuSelection_DrawFrame(void)
{
    struct SelectionScreen *state = gResQueueWork;
    struct SelectionSprite *cursor_entry = &state->records[14].sprite;
    struct SelectionNode *cursor = &state->records[14];
    struct SelectionNode *transfer = &state->records[15];
    struct SelectionNode *node;
    struct SelectionSprite *e;
    struct SelectionSprite *te;
    s32 cnt = 0;
    s32 slot;
    s32 tmp;
    struct EffectPosition pos;

    node = state->head;
    while (node != 0) {
        e = &node->sprite;
        e->affine = 0;
        e->param = 0;
        e->x = node->x;
        e->y = node->y;
        slot = 240;
        if (state->vertical_scroll != 0) {
            e->y += state->vertical_scroll;
        } else if (node->x != node->target_x) {
            if (node->dx > 0) {
                if (node->x + node->dx > node->target_x) {
                    node->x = node->target_x;
                } else {
                    node->x = node->x + node->dx;
                }
            } else {
                if (node->x + node->dx < node->target_x) {
                    node->x = node->target_x;
                } else {
                    node->x = node->x + node->dx;
                }
            }
            e->x = node->x;
        } else if (cnt == state->cursor_index) {
            slot = 241;
            if (cursor->kind != 0) {
                if (BattleMotion_ProjectScaledPositionFar(node->base, &pos) != -1) {
                    cursor->target_x = pos.x;
                    cursor->target_y = pos.y;
                    if (cursor->scale == 0) {
                        cursor->x = pos.x;
                        cursor->y = pos.y;
                        cursor->scale = 1;
                    }
                }
            }
        }
        if (node->scale != 0) {
            if (GameFlag_TestFar(0x103) != 0) {
                if (state->records[14].kind == 1) {
                    e->mode = 1;
                } else {
                    e->mode = 0;
                }
                if (node->kind == 1) {
                    e->mode = 1;
                }
            }
            Runtime_PushSlotEntry((s32 *)e, slot);
        }
        node = node->next;
        cnt++;
    }
    if (transfer->kind != 0) {
        node = NodeChain_GetNodeAtCount(state);
        te = &transfer->sprite;
        te->mode = 0;
        te->affine = 0;
        te->param = 0;
        te->mosaic = 0;
        te->colors = 1;
        te->size = 2;
        te->shape = 0;
        te->priority = 0;
        te->tile = transfer->tile;
        te->x = node->x - 4;
        te->y = node->y + (Menu_CursorBobOffsets[(gFrameTick >> 1) & 15] >> 1) - 4;
        if (transfer->scale != transfer->scale_end) {
            state->matrix[0] = transfer->scale;
            state->matrix[1] = transfer->scale;
            state->matrix[2] = 0;
            te->param = AffineMatrix_BuildForEffect((struct AffineTransform *)state->matrix);
            te->affine = 3;
            te->x = te->x + 0xfff0;
            te->y = te->y + 0xf0;
            transfer->scale = transfer->scale + transfer->scale_step;
        }
        if (GameFlag_TestFar(0x103) != 0) {
            te->mode = 1;
        }
        Runtime_PushSlotEntry((s32 *)te, 248);
    }
    MenuSelection_DrawSideMarker(state, 0);
    MenuSelection_DrawSideMarker(state, 1);
    node = state->path;
    while (node != 0) {
        e = &node->sprite;
        if (node->x != node->target_x) {
            node->x = node->x + node->dx;
        }
        if (node->y != node->target_y) {
            node->y = node->y + node->dy;
        }
        e->x = node->x;
        e->y = node->y;
        if (node->scale != node->scale_end) {
            node->scale = node->scale + node->scale_step;
            state->matrix[0] = node->scale;
            state->matrix[1] = node->scale;
            state->matrix[2] = 0;
            e->param = AffineMatrix_BuildForEffect((struct AffineTransform *)state->matrix);
            e->affine = 3;
            e->x = e->x + 0xfff8;
            e->y = e->y + 0xf8;
        } else {
            e->affine = 0;
            e->param = 0;
        }
        if (GameFlag_TestFar(0x103) != 0) {
            if (state->records[14].kind == 1) {
                e->mode = 1;
            } else {
                e->mode = 0;
            }
            if (node->kind == 1) {
                e->mode = 1;
            }
        }
        Runtime_PushSlotEntry((s32 *)e, 240);
        node = node->next;
    }
    if (cursor->kind != 0) {
        cnt = (s32)Menu_AnimatedCursorTiles;
        {
            void *src = (void *)(cnt + (((gFrameTick >> 2) & 15) << 8));

            cursor_entry->tile = VramBlock_LoadCached(cursor->slot, 0x100, src);
        }
        if (cursor->target_x != cursor->x) {
            tmp = (cursor->target_x - cursor->x) >> 1;
            if (tmp != 0) {
                cursor->x = cursor->x + tmp;
            } else {
                cursor->x = cursor->target_x;
            }
        }
        if (cursor->target_y != cursor->y) {
            if ((tmp = (cursor->target_y - cursor->y) >> 1) != 0) {
                cursor->y = cursor->y + tmp;
            } else {
                cursor->y = cursor->target_y;
            }
        }
        cursor_entry->y = cursor->y + Menu_CursorBobOffsets[(gFrameTick >> 2) & 15] - 32;
        cursor_entry->x = cursor->x - 4;
        if (GameFlag_TestFar(0x103) != 0) {
            if (state->records[14].kind == 1) {
                cursor_entry->mode = 1;
            } else {
                cursor_entry->mode = 0;
            }
        }
        Runtime_PushSlotEntry((s32 *)cursor_entry, 248);
    }
    state->status += 1;
}

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
    if (GameFlag_TestFar(0x103)) {
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
        /* FAKEMATCH: the separate local preserves the early r5 load and late copy; direct assignment changes later node registers. */
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

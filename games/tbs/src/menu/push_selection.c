#include "battle.h"
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
#include "types.h"

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

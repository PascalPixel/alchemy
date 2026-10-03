#include "RESOURCE.H"
#include "SELECT.H"
#include "TYPES.H"

/* The sprite attributes a selection record keeps for its icon. */
/* One icon of the selection screen: an entry of the list being chosen from,
   or an earlier choice kept on the path above it. MENU2.C describes the same
   52-byte record for the code that scrolls the list. */
/* The selection screen's work block. It holds its sixteen records itself:
   the two arrows, the list, the path, and one more. */
void WaitFrames(s32 frames);
void Menu_SendNodeCountList(struct SelectionScreen *screen);
void Menu_ReloadNodeResource(struct SelectionScreen *screen, u32 index);
void Resource_ResetPendingTransfer(void);
struct SelectionNode *Resource_FindFreeTransferEntry(s32 kind);

/* Confirm the entry under the cursor. The other entries slide onto it and
   are released, the chosen one moves up under the earlier choices, and a
   copy of it joins the end of the path. The list's position and cursor are
   kept for that depth, and the entry's index in the whole list is returned. */
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

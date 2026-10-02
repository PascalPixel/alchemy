#include "TYPES.H"

/* The sprite attributes a selection record keeps for its icon. */
struct SelectionSprite {
    u8 unk_00[5];
    u8 unk_05_lo : 2;
    u8 blend_mode : 2;
    u8 mosaic : 1;
    u8 full_color : 1;
    u8 shape : 2;
    u8 unk_06;
    u8 unk_07_lo : 6;
    u8 size : 2;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
};

/* One icon of the selection screen: an entry of the list being chosen from,
   or an earlier choice kept on the path above it. MENU2.C describes the same
   52-byte record for the code that scrolls the list. */
struct SelectionNode {
    struct SelectionNode *prev;
    struct SelectionNode *next;
    u16 base;
    u16 kind;
    u16 slot;
    u16 tile;
    s16 y;
    u16 z;
    s16 speed;
    u16 speed_z;
    s16 target_y;
    u16 target_z;
    u16 home_y;
    u16 home_z;
    u16 unk_20;
    u16 scale;
    u8 unk_24[2];
    u16 scale_target;
    struct SelectionSprite sprite;
};

/* The selection screen's work block. It holds its sixteen records itself:
   the two arrows, the list, the path, and one more. */
struct SelectionScreen {
    struct SelectionNode records[16];
    u8 unk_340[8];
    struct SelectionNode *nodes;
    struct SelectionNode *path;
    u8 unk_350[0x4a];
    u16 unk_39a;
    u16 top;
    u16 cursor;
    u8 unk_3a0[2];
    u16 status;
    u16 path_top[5];
    u16 path_cursor[5];
};

void WaitFrames(s32 frames);
void Menu_SendNodeCountList(struct SelectionScreen *screen);
void Menu_ReloadNodeResource(struct SelectionScreen *screen, u32 index);
void Resource_ResetPendingTransfer(void);
s32 Resource_ResetEntry(u32 index);
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

    index = screen->top + screen->cursor;
    Menu_SendNodeCountList(screen);
    Menu_ReloadNodeResource(screen, screen->cursor);
    screen->status = 33;
    WaitFrames(1);
    screen->records[0].kind = 0;
    screen->records[1].kind = 0;
    screen->records[14].kind = 0;
    screen->records[14].scale = 0;
    Resource_ResetPendingTransfer();
    node = screen->nodes;
    while (node != NULL && screen->cursor != cnt) {
        node = node->next;
        cnt++;
    }
    node->home_y = node->y;
    node->home_z = node->z;
    for (other = screen->nodes; other != NULL; other = other->next) {
        if (other != node) {
            other->target_y = node->y;
            other->speed = (node->y - other->y) >> 1;
        }
    }
    WaitFrames(2);
    for (other = screen->nodes; other != NULL; other = other->next) {
        if (other != node) {
            Resource_ResetEntry(other->slot);
            other->kind = 0;
        }
    }
    screen->nodes = node;
    node->prev = NULL;
    node->next = NULL;
    node->target_y = 4;
    for (other = screen->path, cnt = 0; other != NULL; other = other->next) {
        node->target_y += 16;
        cnt++;
    }
    screen->path_top[cnt] = screen->top;
    screen->path_cursor[cnt] = screen->cursor;
    node->speed = (node->target_y - node->y) >> 1;
    screen->unk_39a = 0;
    screen->cursor |= 0x80;
    WaitFrames(2);
    other = Resource_FindFreeTransferEntry(1);
    other->kind = node->kind;
    other->unk_20 = node->unk_20;
    other->base = node->base;
    other->slot = node->slot;
    other->tile = node->tile;
    other->y = node->y;
    other->z = node->z;
    other->target_y = other->y;
    other->target_z = other->z;
    other->home_y = node->home_y;
    other->home_z = node->home_z;
    other->speed = 0;
    other->speed_z = 0;
    other->scale = 0x100;
    other->scale_target = 0x100;
    sprite = &other->sprite;
    sprite->blend_mode = 0;
    sprite->full_color = 0;
    sprite->mosaic = 0;
    sprite->size = 1;
    sprite->shape = 0;
    sprite->palette = 0;
    sprite->tile = other->tile;
    node->kind = 0;
    screen->nodes = NULL;
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

/* Draft, not exact (2026-09-25): 580 of 580 bytes, 268 differing halfwords.
   Menu_PushSelectedNode: walks to the node under the cursor, slides the other
   nodes onto it and releases their resources, keeps it as the only node,
   records top and cursor for this depth, and appends a copy (a free transfer
   entry with a size-1 sprite) to the path list. Returns top + cursor.
   What lined up: every statement, store order and the pool; the zero stores
   after the pending transfer reset come from the count local (reset to 0 and
   stored) as the ROM keeps them in r8; chained copies for x/target_x.
   Remaining: register allocation only. The ROM gives state r5, node r6,
   other r7, cnt r8, index r9 and the cursor address sl; here the other/entry
   pseudo outranks state (65 refs over 111 insns against 22 over 172), so
   state lands in r7, the cursor address in r5 and index in sl. Splitting the
   entry pseudo, a work-pointer copy, an array for top/cursor and reusing node
   for the tail walk moved nothing. */
#include "TYPES.H"

struct PushSprite {
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

struct PushNode {
    struct PushNode *prev;
    struct PushNode *next;
    u16 unk_08;
    u16 active;
    u16 id;
    u16 tile;
    s16 x;
    u16 y;
    s16 step;
    u16 speed;
    s16 target_x;
    u16 target_y;
    u16 home_x;
    u16 home_y;
    u16 unk_20;
    u16 scale_x;
    u8 unk_24[2];
    u16 scale_y;
    struct PushSprite sprite;
};

struct PushMenu {
    u8 unk_000[10];
    u16 more_above;
    u8 unk_00c[0x32];
    u16 more_below;
    u8 unk_040[0x2a2];
    u16 unk_2e2;
    u8 unk_2e4[0x16];
    u16 unk_2fa;
    u8 unk_2fc[0x4c];
    struct PushNode *nodes;
    struct PushNode *path;
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
void Menu_SendNodeCountList(struct PushMenu *state);
void Menu_ReloadNodeResource(struct PushMenu *state, u32 index);
void Resource_ResetPendingTransfer(void);
s32 Resource_ResetEntry(u32 index);
struct PushNode *Resource_FindFreeTransferEntry(s32 kind);

u32 Menu_PushSelectedNode(struct PushMenu *state)
{
    struct PushNode *node;
    struct PushNode *other;
    struct PushNode *last;
    u32 index;
    u32 cnt;

    index = state->top + state->cursor;
    cnt = 0;
    Menu_SendNodeCountList(state);
    Menu_ReloadNodeResource(state, state->cursor);
    state->status = 33;
    WaitFrames(1);
    state->more_above = 0;
    state->more_below = 0;
    state->unk_2e2 = 0;
    state->unk_2fa = 0;
    Resource_ResetPendingTransfer();
    for (node = state->nodes; node != NULL && state->cursor != cnt; cnt++)
        node = node->next;
    node->home_x = node->x;
    node->home_y = node->y;
    for (other = state->nodes; other != NULL; other = other->next) {
        if (other != node) {
            other->target_x = node->x;
            other->step = (node->x - other->x) >> 1;
        }
    }
    WaitFrames(2);
    cnt = 0;
    for (other = state->nodes; other != NULL; other = other->next) {
        if (other != node) {
            Resource_ResetEntry(other->id);
            other->active = cnt;
        }
    }
    state->nodes = node;
    node->prev = NULL;
    node->next = NULL;
    node->target_x = 4;
    cnt = 0;
    for (other = state->path; other != NULL; other = other->next) {
        node->target_x += 16;
        cnt++;
    }
    state->path_top[cnt] = state->top;
    state->path_cursor[cnt] = state->cursor;
    cnt = 0;
    node->step = (node->target_x - node->x) >> 1;
    state->unk_39a = cnt;
    state->cursor |= 0x80;
    WaitFrames(2);
    other = Resource_FindFreeTransferEntry(1);
    other->active = node->active;
    other->unk_20 = node->unk_20;
    other->unk_08 = node->unk_08;
    other->id = node->id;
    other->tile = node->tile;
    other->target_x = other->x = node->x;
    other->target_y = other->y = node->y;
    other->home_x = node->home_x;
    other->home_y = node->home_y;
    other->step = cnt;
    other->speed = cnt;
    other->scale_x = 0x100;
    other->scale_y = 0x100;
    other->sprite.blend_mode = 0;
    other->sprite.full_color = 0;
    other->sprite.mosaic = 0;
    other->sprite.shape = 0;
    other->sprite.size = 1;
    other->sprite.palette = 0;
    other->sprite.tile = other->tile;
    node->active = cnt;
    state->nodes = (struct PushNode *)cnt;
    if (state->path != NULL) {
        last = state->path;
        while (last->next != NULL)
            last = last->next;
        last->next = other;
        other->prev = last;
    } else {
        state->path = other;
        other->prev = NULL;
    }
    other->next = NULL;
    return index;
}

/* Draft, not exact (2026-10-01, slice-2): 580 of 580 bytes, score 2470,
   176 instructions differ, every one a register. The listing carries two
   labels at one address (Menu_ConfirmSelection, Menu_PushSelectedNode); one
   has to go at adoption. Rewritten plainly: the screen holds its sixteen
   52-byte records itself (the four zero stores are records 0, 1 and 14),
   zeros are literal, and the other pointer starts null, which is the
   register the reference stores those zeros from. The statements, stores,
   loops and pool now line up.
   Remaining: the reference gives state r5, node r6, other r7, the count
   r8, the cursor address sl and the index r9; here node, other and state
   take r5, r6, r7 because the allocator ranks node (51 references over 151
   instructions) and other (66 over 268) above state (22 over 171). No
   spelling tried moves state first; the earlier draft (141 instructions,
   score 2885) stored the zeros through the count and had the same order. */
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
    struct PushSprite sprite;
};

struct PushMenu {
    struct PushNode records[16];
    u8 unk_340[8];
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
    struct PushNode *other = NULL;
    u32 cnt = 0;
    u32 index;
    struct PushSprite *sprite;

    index = state->top + state->cursor;
    Menu_SendNodeCountList(state);
    Menu_ReloadNodeResource(state, state->cursor);
    state->status = 33;
    WaitFrames(1);
    state->records[0].kind = 0;
    state->records[1].kind = 0;
    state->records[14].kind = 0;
    state->records[14].scale = 0;
    Resource_ResetPendingTransfer();
    node = state->nodes;
    while (node != NULL && state->cursor != cnt) {
        node = node->next;
        cnt++;
    }
    node->home_y = node->y;
    node->home_z = node->z;
    for (other = state->nodes; other != NULL; other = other->next) {
        if (other != node) {
            other->target_y = node->y;
            other->speed = (node->y - other->y) >> 1;
        }
    }
    WaitFrames(2);
    for (other = state->nodes; other != NULL; other = other->next) {
        if (other != node) {
            Resource_ResetEntry(other->slot);
            other->kind = 0;
        }
    }
    state->nodes = node;
    node->prev = NULL;
    node->next = NULL;
    node->target_y = 4;
    cnt = 0;
    for (other = state->path; other != NULL; other = other->next) {
        node->target_y += 16;
        cnt++;
    }
    state->path_top[cnt] = state->top;
    state->path_cursor[cnt] = state->cursor;
    node->speed = (node->target_y - node->y) >> 1;
    cnt = 0;
    state->unk_39a = 0;
    state->cursor |= 0x80;
    WaitFrames(2);
    other = Resource_FindFreeTransferEntry(1);
    other->kind = node->kind;
    other->unk_20 = node->unk_20;
    other->base = node->base;
    other->slot = node->slot;
    other->tile = node->tile;
    other->target_y = other->y = node->y;
    other->target_z = other->z = node->z;
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
    sprite->shape = 0;
    sprite->size = 1;
    sprite->palette = 0;
    sprite->tile = other->tile;
    node->kind = 0;
    state->nodes = NULL;
    if (state->path != NULL) {
        node = state->path;
        while (node->next != NULL)
            node = node->next;
        node->next = other;
        other->prev = node;
    } else {
        state->path = other;
        other->prev = NULL;
    }
    other->next = NULL;
    return index;
}

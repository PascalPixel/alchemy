/* 2026-09-30 (Mercury's helper): EXACT, 292 of 292 bytes, one FAKEMATCH (a
   separate local for the right-hand cursor tiles gives the reference's
   early r5 load and late mov fp, r5). Its structs follow MENU.C's
   SlotEntry/MenuNode/MenuSelection, with MenuSelection extended by
   count/first/second/other at 0x394..0x39c; MENU.C's own MenuSelection
   keeps mode at 0x2e2, inside this file's unknown_68 pad, so the two need
   merging on adoption. The natural host, SELECTION_SET_NODE_COORDINATES.C,
   declares it s32 Menu_SetupSelectionSide(s32, s32) and calls it twice;
   that prototype has to become void (struct MenuSelection *, s32) first.
   Compile it under #if defined(TBS_EDITION_EN) until the other editions
   adopt theirs. */
#include "TYPES.H"

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
    u8 unknown_68[0x394 - 0x68];
    u16 count;
    u16 first;
    u16 second;
    u16 unknown_39a;
    u16 other;
};

extern u8 Menu_CursorObjectTiles[], Menu_CursorLeftObjectTiles[];
extern s32 Resource_FindFreeEntry(void);
extern s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);

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

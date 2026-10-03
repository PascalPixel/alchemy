#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
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
    u8 unknown_68[0x27a];
    u16 mode;
};
extern u32 gFrameTick;
extern u8 Menu_CursorObjectTiles[], Menu_CursorLeftObjectTiles[];
extern s32 GameFlag_IsSet(s32 flag);

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

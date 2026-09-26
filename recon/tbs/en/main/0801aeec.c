/* DRAFT: MenuSelection_DrawSideMarker, complete 292-byte owner split from
 * 0801a98c. Reuses the parent's 52-byte menu-node and OAM bitfield model.
 * 2026-09-26: 292/292 bytes, 23 differing halfwords / 23 aligned edits.
 * The explicit shared x mask restores the reference's separate move into
 * ip and copy back to r3; all pool positions and call/branch topology match.
 * Remaining: node-offset/x-value registers r1/r2 versus r2/r1, mask reload
 * r2 versus r0, mask-copy scheduling, and later reload cycle (handle index,
 * state +0x2e2 address, decrement constant). No stack-frame difference.
 * Baseline implicit bitfield mask was 37 halfwords / 17 aligned edits, but
 * lacked the ip-to-r3 copy and had an extra alignment halfword instead.
 * Unsigned x/y fields and explicit VramBlock_LoadCached prototype did not
 * change that baseline. Allocator dump: implicit mask pseudo 57 lands in
 * ip; reload uses r3 then later r2. Stop before register-order permutations.
 */
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
extern u32 Data_03001800;
extern u8 Data_080342f8[], Data_08033ef8[];
extern u8 Value_00000103[], Value_0000ffff[];
extern s32 GameFlag_TestFar(s32 flag);
extern void Runtime_PushSlotEntry(s32 *entry, s32 slot);
extern s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);

void MenuSelection_DrawSideMarker(struct MenuSelection *state, s32 index)
{
    u32 frame = (Data_03001800 >> 2) & 7;
    struct SlotEntry *entry;
    u8 *frames;
    u32 x_mask;

    if (state->nodes[index].active == 0)
        return;
    x_mask = 0x1ff;
    entry = &state->nodes[index].entry;
    entry->x = state->nodes[index].x & x_mask;
    entry->y = state->nodes[index].y;
    if (index != 0) {
        frames = Data_080342f8;
        if (state->nodes[1].offset != 0)
            entry->x = (entry->x + state->nodes[1].offset) & x_mask;
    } else {
        frames = Data_08033ef8;
        if (state->nodes[0].offset != 0)
            entry->x = (entry->x - state->nodes[0].offset) & x_mask;
    }
    entry->tile = VramBlock_LoadCached(state->nodes[index].handle, 128,
                                     frames + frame * 128);
    if (GameFlag_TestFar((s32)Value_00000103)) {
        if (state->mode == 1)
            entry->mode = 1;
        else
            entry->mode = 0;
    }
    Runtime_PushSlotEntry((s32 *)entry, 238);
    if (state->nodes[index].offset != 0)
        state->nodes[index].offset += (s32)Value_0000ffff;
}

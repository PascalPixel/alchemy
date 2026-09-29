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
 * The exact Render_PlaceSpritePartPair family's u16 y:8/attribute bitfields
 * also emit the identical candidate here; field storage width is not the
 * remaining cause.
 * 2026-09-29 (alchemy permute scorer): 75, 11 register-only rows plus the
 * unlabelled left-marker tiles (was 525). Plain constants replace the link
 * symbols: GameFlag_IsSet(0x103) loads 0x103 from the pool by itself, and
 * the offset's decrement is the reference's halfword add of 0xffff. The
 * frame tick and the right-marker tiles are the linked gFrameTick and
 * Menu_CursorObjectTiles; the left tiles are the last 0x400 bytes of the
 * Data_08032224 scaffold block and need their own label there. A search
 * then found the x mask set after the entry pointer and the frame address
 * in its own local. Remaining: the node offset (index * 52) is in r1 and
 * the node's x in r2 where the reference has r2 and r1; a node pointer, a
 * local x, mask types and statement orders do not swap them, and a further
 * 400-second search (34,000 candidates) found nothing below 75.
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
extern u32 gFrameTick;
extern u8 Menu_CursorObjectTiles[], Data_08033ef8[];
extern s32 GameFlag_IsSet(s32 flag);
extern void Runtime_PushSlotEntry(s32 *entry, s32 slot);
extern s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);

void MenuSelection_DrawSideMarker(struct MenuSelection *state, s32 index)
{
    u32 frame = (gFrameTick >> 2) & 7;
    struct SlotEntry *entry;
    u8 *frames;
    u32 x_mask;
    u8 *tmp;

    if (state->nodes[index].active == 0)
        return;
    entry = &state->nodes[index].entry;
    x_mask = 0x1ff;
    entry->x = state->nodes[index].x & x_mask;
    entry->y = state->nodes[index].y;
    if (index != 0) {
        frames = Menu_CursorObjectTiles;
        if (state->nodes[1].offset != 0)
            entry->x = (entry->x + state->nodes[1].offset) & x_mask;
    } else {
        frames = Data_08033ef8;
        if (state->nodes[0].offset != 0)
            entry->x = (entry->x - state->nodes[0].offset) & x_mask;
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

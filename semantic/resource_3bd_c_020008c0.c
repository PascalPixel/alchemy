#include "types.h"

/* Six footprint kinds are selected from the object's signed kind id.  Each
 * matching row supplies the four signed fixed-point extents used below. */
struct Resource3bdFootprint {
    s32 x0;
    s32 z0;
    s32 x1;
    s32 z1;
};

/*
 * Find the footprint containing the actor's current position and submit the
 * corresponding scene rectangles.  The complete owner starts at the saved
 * high-register prologue 0x020008c0 and reaches its single return at
 * 0x020009c0..0x020009ce.  Its three directly referenced literal words are
 * at 0x020009d0..0x020009db; the next owner begins at 0x020009dc, so the
 * measured owner span is 284 bytes.
 *
 * The kind search consumes the six entries at Data_0200bef4.  A missing kind
 * leaves the machine's sentinel value 7 and returns zero.  A match selects
 * one four-word row from Data_0200bf0c.  The two local rectangle helpers are
 * an in-overlay owner at 0x02000244; the resolver identifies both calls as
 * that prologue rather than as the listing's misleading PC-relative names.
 *
 * All frame stores are local spills or outgoing stack arguments.  The only
 * persistent effects are the three ordered calls after the rectangle math;
 * their six-argument ABIs are written positionally here.
 */

extern const s32 Data_0200bef4[];
extern const struct Resource3bdFootprint Data_0200bf0c[];
extern u8 *Data_03001e70;

extern u8 *Func_0808a080(s32 id);
extern void Func_080091c0(s32 arg0, s32 arg1, s32 arg2,
                          s32 arg3, s32 arg4, s32 arg5);
extern s32 Func_02000244(u32 arg0, s32 arg1, s32 arg2,
                         u32 arg3, u32 arg4, s32 arg5);

static s32 add_wrapped(s32 left, s32 right)
{
    return (s32)((u32)left + (u32)right);
}

static s32 absolute_wrapped(s32 value)
{
    return value < 0 ? (s32)(0u - (u32)value) : value;
}

static s32 shift_left_16_wrapped(s32 value)
{
    return (s32)((u32)value << 16);
}

s32 Func_020008c0(s32 id)
{
    u8 *actor = Func_0808a080(id);
    s32 kind = *(s16 *)(*(u8 **)(actor + 0x50) + 0x28);
    u32 index;
    const struct Resource3bdFootprint *footprint;
    s32 actorX;
    s32 actorZ;
    s32 adjustedX;
    s32 adjustedZ;
    s32 cellX;
    s32 cellZ;
    s32 width;
    s32 height;
    u8 *world;
    s32 worldX;
    s32 worldZ;

    for (index = 0; index < 6; index++) {
        if (kind == Data_0200bef4[index]) {
            break;
        }
    }

    if (index >= 6) {
        return 0;
    }

    actorZ = *(s32 *)(actor + 16);
    footprint = &Data_0200bf0c[index];
    actorX = *(s32 *)(actor + 8);

    cellX = adjustedX >> 20;
    adjustedX = add_wrapped(shift_left_16_wrapped(footprint->x0), actorX);
    adjustedZ = add_wrapped(shift_left_16_wrapped(footprint->z0), actorZ);
    cellZ = adjustedZ >> 20;

    width = add_wrapped(absolute_wrapped(footprint->x0),
                        absolute_wrapped(footprint->x1)) >> 4;
    height = add_wrapped(absolute_wrapped(footprint->z0),
                         absolute_wrapped(footprint->z1)) >> 4;

    world = Data_03001e70;
    worldX = add_wrapped(*(s32 *)(world + 0x13c) >> 20, cellX);
    worldZ = add_wrapped(*(s32 *)(world + 0x140) >> 20, cellZ);

    Func_080091c0(cellX, cellZ, width, height, worldX, worldZ);
    Func_02000244(0, adjustedX, adjustedZ, width, height, 255);
    return 1;
    Func_02000244(2, adjustedX, adjustedZ, width, height, 255);
}

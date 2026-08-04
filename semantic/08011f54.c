// __call_via_rN veneer site, resolved per-site against the ROM.
//
// The `bl Func_080072f0` at 0x08011fb8 is `__call_via_r3` -- an INDIRECT call
// through r3, not a call to a function at 0x080072f0. The draft read the
// callee load as a fourth argument, which is the standard shape of this error
// for r0-r3 (the callee occupies the argument slot matching the veneer's
// register index only because r0-r3 are the argument registers).
//
// The callee: `ldr r3, [r4, r3]` at 0x08011fb2, with r4 = 0x080134fc from the
// literal pool and r3 = (attribute & 15) << 2. 0x080134fc is a SIXTEEN-entry
// ROM function-pointer table, read directly:
//
//   0x080134fc  0x08011ce1 0x08011ced 0x08011d11 0x08011d35
//   0x0801350c  0x08011d61 0x08011d95 0x08011ddd 0x08011e2d
//   0x0801351c  0x08011e51 0x08011e6d 0x08011e89 0x08011ed1
//   0x0801352c  0x08011f15 0x08011f29 0x08011f3d 0x08011f49
//   0x0801353c  0x00000000   <- terminator; the table is exactly 16 wide
//
// All sixteen have the Thumb bit set and all lie in 0x08011ce0..0x08011f48,
// the run immediately preceding this function. The index cannot exceed 15
// (`ands` with 15 at 0x08011fa6), so no entry past the terminator is reachable.
// This site is therefore RESOLVED, not an uncertainty.
//
// ARITY, from two independent instruments:
//   (1) caller side -- r0, r1, r2 are all freshly set at the branch
//       (0x08011fb0 r0, 0x08011fb4 r1, 0x08011fb6 r2) and nothing writes r3
//       as an argument;
//   (2) callee side -- entry 0 (0x08011ce0) reads only r0; entry 1
//       (0x08011cec) reads r0 and r1; entry 2 (0x08011d10) reads r0 and r2.
//       Across the family the consumed set is exactly {r0, r1, r2}.
// Two methods agreeing, not two drafts agreeing.
//
// RETURN TYPE CORRECTED. Every table entry leaves its result in r0, and this
// function's epilogue (`pop {r5,r6,r7}; pop {r1}; bx r1` at 0x08011fbc) never
// touches r0. The call is in tail position: this function returns the entry's
// result. The draft declared `void`.
#include "types.h"

/// One of the sixteen samplers at 0x080134fc, selected by the tile's low
/// attribute nibble. Takes the tile's parameter bytes and the sub-tile
/// coordinates; returns the sampled value in 16.16-style fixed point.
typedef s32 (*TileSampler)(const void *parameters, s32 subX, s32 subY);

extern u8 *Data_03001e70;
extern u8 Data_0202c000[];
extern u8 Data_0202c001[];
extern const TileSampler Data_080134fc[];

s32 Func_08011f54(s32 index, s32 x, s32 y)
{
    u8 *table;
    u8 *map;
    s32 roundedX;
    s32 roundedY;
    s32 offset;
    u32 entryOffset;

    table = Data_03001e70;
    x >>= 16;
    y >>= 16;
    map = (u8 *)0x02010000;
    if (table != NULL) {
        offset = (index & 3) * 48 + 304;
        map = *(u8 **)(table + offset);
    }

    roundedX = x;
    if (x < 0)
        roundedX += 15;
    roundedY = y;
    if (y < 0)
        roundedY += 15;
    entryOffset = map[(((roundedY >> 4) << 7) + (roundedX >> 4)) * 4 + 3] * 4;
    return Data_080134fc[Data_0202c000[entryOffset] & 15](
        Data_0202c001 + entryOffset, x & 15, y & 15);
}

/* 2026-09-29 alchemy permute: score 115 on the permuter's scorer (10
   register-only, 1 reordered), unchanged after 73,810 candidates from seed
   1 and 47,253 from seed 11 (15 minutes). By hand: the local and global
   register dumps show the symbol address (a block-local pseudo) takes r2
   before tile is allocated, so tile loses r2 to r4; the reference
   evidently freed r3 for it by moving variant to r5. Register or unsigned
   parameters, callee prototype widths, ternaries, per-branch calls and
   loads without the data local all leave it or regress. */
/* NONMATCHING: 13 halfwords, register allocation only. The reference keeps
 * tile in r2 and copies variant to r5 and y to r6; here variant ties to r3 and
 * tile moves to r4. Testing variant twice keeps tile in r2 but gives variant r1.
 */
#include "TYPES.H"

extern u8 *Data_03001f2c;

struct MarkerObject {
    u8 pad00[4];
    u8 state;
    u8 active;
    u8 pad06[6];
    u16 timer;
};

struct MarkerObject *Func_080150c8(u32 resource, u32 flags, s32 x, s32 y, s32 tile);

s32 UiIcon_DrawVariantWithTileOffset(s32 x, s32 y, s32 tile, s32 variant)
{
    struct MarkerObject *object;
    u32 resource;
    u8 *data = Data_03001f2c;

    if (variant == 0) {
        resource = *(u16 *)(data + 0x392);
        tile -= 3;
    } else {
        resource = *(u16 *)(data + 0x394);
        tile -= 4;
    }
    object = Func_080150c8(resource, 0x40000000, x, y, tile);
    if (object == 0)
        return -1;
    object->state = 0;
    object->timer = 0;
    object->active = 1;
    return 1;
}

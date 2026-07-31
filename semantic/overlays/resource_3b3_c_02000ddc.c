typedef unsigned char u8;
typedef unsigned int u32;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 3b3 unindexed helper at 0x02000ddc (236 bytes incl. pool,
 * 2 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,r7,lr}` plus `r8` spill at 0x02000ddc, epilogue `pop {r3} /
 * mov r8,r3 / pop {r5,r6,r7} / pop {r1} / bx r1` at
 * 0x02000eb2-0x02000eba. The trailing pool at 0x02000ebc-0x02000ec4
 * (0x03001e70 [the `Data_03001e70` workspace pointer], 0x0200ace0
 * [`BucketKeyTable_0200ace0`, the same table referenced by the parked
 * 0x02000630], 0x0200acf8 [`BucketGeomTable_0200acf8`]) is included per
 * the usual pool rule, immediately followed by the next owner's push
 * {r5,r6,r7,lr} at 0x02000ec8, already this overlay's row
 * `0x02000ec8 | 14 calls`, so the span is 0x02000ddc-0x02000ec8,
 * 236 bytes.
 *
 * Six arguments (two on the stack, per AAPCS): `a0` selects an object
 * via `Func_0200390a`; `out1`/`out2` are scalar out-pointers, `out3` is
 * a 3-field out-struct (bucket index at +0, position fields at +8/+16
 * copied from the object), and `out4`/`out5` (stack args) are two more
 * scalar out-pointers filled from an unrelated `Data_03001e70` table
 * lookup at the end, independent of the bucket search.
 *
 * The bucket search scans `BucketKeyTable_0200ace0[0..5]` for the
 * object's `+0x50 -> +0x28` halfword; on a match, `out3->bucket` is the
 * matching index and the function proceeds to fill in the rest. No
 * match (all 6 checked) leaves the search's own sentinel at 7, which
 * fails the `<= 6` gate and returns 0 (out3/out1/out2 partially
 * written -- `out3->bucket` holds 7, the rest untouched). On a match,
 * `BucketGeomTable_0200acf8[bucket]` (a 4-`s32` row) supplies two
 * radius-like sums (`|row[1]|+|row[3]|` for `out2`, `|row[0]|+|row[2]|`
 * for `out1`, both `>> 4`) and two position updates folded into
 * `out3->pos_x`/`out3->pos_z` (`(obj_field + row[k]<<16) >> 20`).
 *
 * Raw callee naming.
 */

extern u8 *Data_03001e70;
extern s32 BucketKeyTable_0200ace0[];
extern s32 BucketGeomTable_0200acf8[][4];

u8 *Func_0200390a();

s32 Func_02000ddc(s32 a0, s32 *out1, s32 *out2, s32 *out3, s32 *out4, s32 *out5)
{
    u8 *obj = Func_0200390a(a0);
    s16 *keyfield;
    s32 bucket = 7;
    s32 i;

    keyfield = *(s16 **)(*(u8 **)(obj + 0x50) + 0x28);

    for (i = 0; i <= 5; i++) {
        if (*keyfield == BucketKeyTable_0200ace0[i]) {
            bucket = i;
            break;
        }
    }
    out3[0] = bucket;

    if ((u32)bucket > 6) {
        return 0;
    }

    out3[2] = *(s32 *)(obj + 8);
    out3[3] = *(s32 *)(obj + 12);
    out3[4] = *(s32 *)(obj + 16);

    {
        s32 row0 = BucketGeomTable_0200acf8[bucket][0];
        s32 row1 = BucketGeomTable_0200acf8[bucket][1];
        s32 row2 = BucketGeomTable_0200acf8[bucket][2];
        s32 row3 = BucketGeomTable_0200acf8[bucket][3];
        s32 abs1 = row1 < 0 ? -row1 : row1;
        s32 abs3 = row3 < 0 ? -row3 : row3;
        s32 abs0 = row0 < 0 ? -row0 : row0;
        s32 abs2 = row2 < 0 ? -row2 : row2;

        *out2 = (abs1 + abs3) >> 4;
        *out1 = (abs0 + abs2) >> 4;

        out3[2] = (out3[2] + (row0 << 16)) >> 20;
        out3[4] = (out3[4] + (row1 << 16)) >> 20;
    }

    {
        u8 *workspace = Data_03001e70;
        *out4 = *(s32 *)(workspace + 316) >> 20;
        *out5 = *(s32 *)(workspace + 320) >> 20;
    }

    return 1;
}

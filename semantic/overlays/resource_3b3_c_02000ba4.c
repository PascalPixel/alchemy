typedef unsigned char u8;
typedef unsigned int u32;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 3b3 unindexed helper at 0x02000ba4 (284 bytes, 15 calls --
 * the highest call count in this overlay's item-28 queue). Converted
 * from a parked scope note (work/claude/notes/resource_3b3_ba4.md) per
 * Vale's steer, built directly on the bucket-search shape already
 * drafted for 0x02000ddc (same `BucketKeyTable_0200ace0` search,
 * `BucketGeomTable_0200acf8` geometry lookup, and sentinel-7 idiom).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,r7,lr}` + `sl`/`r8` spill, 32-byte stack frame at 0x02000ba4,
 * epilogue `add sp,#32 / pop {r3,r5} / mov r8,r3 / mov sl,r5 / pop
 * {r5,r6,r7} / pop {r1} / bx r1` at 0x02000ca6-0x02000cb2. The trailing
 * pool at 0x02000cb4-0x02000cbc (0x03001e70 [`Data_03001e70`], 0x0200ace0
 * [`BucketKeyTable_0200ace0`], 0x0200acf8 [`BucketGeomTable_0200acf8`])
 * is included per the usual pool rule, immediately followed by the
 * next owner's push {r5,r6,r7,lr} at 0x02000cc0, already this
 * overlay's row `0x02000cc0` (parked this session), so the span is
 * 0x02000ba4-0x02000cc0, 284 bytes.
 *
 * Same 6-way bucket search over `BucketKeyTable_0200ace0[0..5]` as
 * 0x02000ddc (sentinel 7 on no match, `> 6` gate returns 0). On a
 * match, `BucketGeomTable_0200acf8[bucket]`'s four fields yield two
 * radius sums (`radius1` from rows 0/2, `radius2` from rows 1/3, both
 * `>>4`) and fold the object's `+8`/`+16` position into `x`/`z`
 * (`(field + row<<16) >> 20`), same as 0x02000ddc. Additionally reads
 * two `Data_03001e70`-workspace fields (`+316`/`+320`, `>>20`) and adds
 * them to `x`/`z` for a third pair of "world-relative" coordinates,
 * then runs three finishing calls instead of 0x02000ddc's plain
 * out-pointer writes.
 *
 * Raw callee naming.
 */

extern u8 *Data_03001e70;
extern s32 BucketKeyTable_0200ace0[];
extern s32 BucketGeomTable_0200acf8[][4];

u8 *Func_020036d2();
void Func_02003740();
void Func_020011b8();
void Func_020011ca();

s32 Func_02000ba4(s32 a0)
{
    u8 *obj = Func_020036d2(a0);
    u8 *workspace = Data_03001e70;
    s16 *keyfield = *(s16 **)(*(u8 **)(obj + 0x50) + 0x28);
    s32 bucket = 7;
    s32 i;

    for (i = 0; i <= 5; i++) {
        if (*keyfield == BucketKeyTable_0200ace0[i]) {
            bucket = i;
            break;
        }
    }

    if ((u32)bucket > 6) {
        return 0;
    }

    {
        s32 x = *(s32 *)(obj + 8);
        s32 z = *(s32 *)(obj + 16);
        s32 row0 = BucketGeomTable_0200acf8[bucket][0];
        s32 row1 = BucketGeomTable_0200acf8[bucket][1];
        s32 row2 = BucketGeomTable_0200acf8[bucket][2];
        s32 row3 = BucketGeomTable_0200acf8[bucket][3];
        s32 abs0 = row0 < 0 ? -row0 : row0;
        s32 abs1 = row1 < 0 ? -row1 : row1;
        s32 abs2 = row2 < 0 ? -row2 : row2;
        s32 abs3 = row3 < 0 ? -row3 : row3;
        s32 radius1 = (abs0 + abs2) >> 4;
        s32 radius2 = (abs1 + abs3) >> 4;
        s32 wsum_x;
        s32 wsum_z;

        x = (x + (row0 << 16)) >> 20;
        z = (z + (row1 << 16)) >> 20;

        wsum_x = (*(s32 *)(workspace + 316) >> 20) + x;
        wsum_z = (*(s32 *)(workspace + 320) >> 20) + z;

        Func_02003740(x, z, radius1, radius2, wsum_x, wsum_z);
        Func_020011b8(0, x, z, radius1, radius2, 255);
        Func_020011ca(2, x, z, radius1, radius2, 255);
    }

    return 1;
}

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef int s32;

/*
 * resource_3a1 owner at 0x0200013c, 472 bytes: the SPAWNER-AND-INITIALISER
 * for this overlay's per-frame step function. It allocates a record through
 * Func_080090c8, fills its fixed fields, installs the step function in the
 * record's +108 callback slot, and then applies an optional-field block gated
 * one bit at a time by a caller-supplied flags word.
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7,fp / mov r6,sl / mov r5,r9 /
 * push {r5, r6, r7} / mov r7,r8 / push {r7} / sub sp,#8` at
 * 0x0200013c-0x0200014b through `add sp,#8 / pop {r3, r5, r6, r7} / mov r8,r3
 * / mov r9,r5 / mov sl,r6 / mov fp,r7 / pop {r5, r6, r7} / pop {r0} / bx r0`
 * at 0x020002f6-0x02000306, followed by the three-word literal pool
 * 0x02000308-0x02000313. Span 0x02000314 - 0x0200013c = 0x1d8 = 472 bytes.
 *
 * The 2-byte halfword at 0x0200013a is NOT part of this owner. It is
 * alignment padding after the `bx lr` at 0x02000138 that ends the previous
 * row 0x02000104, and it belongs to that row. Sweep D reports the gap as
 * 0x13a-0x313 = 474 bytes for exactly that reason; the owner is 472 and the
 * difference is the pad. Do not fold it in here.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts resource_3a1 13c
 * 314`'s `+2` rule, which reports sites=10 distinct_targets=6 veneer=10 over
 * exactly these bounds.
 *
 * EIGHT ARGUMENTS, four of them on the stack. The frame is 40 bytes of push
 * plus an 8-byte local, so the incoming stack arguments are at [sp,#40],
 * [sp,#44], [sp,#48] and [sp,#52] and all four are read. The 8-byte local
 * holds a3 spilled at [sp,#4] and the address `record + 100` cached at
 * [sp,#0]. A reader who stops at r0-r3 gets a four-argument function that
 * silently drops the flags word, which is the argument the whole second half
 * of the row is about.
 *
 * WHAT IT INSTALLS, and the corroboration for it. The +108 slot is written
 * with the pool word 0x02008105, which under the tree's base + 0x8000 rule is
 * image offset 0x105 = the owner at 0x02000104 with the Thumb bit -- the row
 * immediately before this one. That row is the per-frame step: it adds +68,
 * +72 and +76 into +8, +12 and +16, adds +48 and +52 into +24 and +28, and
 * accumulates the record's u16 at +100 into the +80 sub-record's +30. Every
 * one of those source fields is written HERE: +68/+72/+76 from a3/a4/a5,
 * +48/+52 from the two Func_03000380 results, +24/+28 from the source struct,
 * and +100 zeroed. The two rows were read independently and they agree field
 * for field, which is better evidence than either read alone.
 *
 * The in-image table at 0x0200876c (image offset 0x76c) is indexed by the low
 * four bits of the flags word, and the SAME entry is used twice: once as the
 * descriptor handed to Func_08009098, and once for its +12 field, which is
 * the rate argument to Func_03000380. The index is computed once and cached
 * in fp across the intervening calls.
 *
 * TWO OPTION BITS ARE COUPLED, and it is a register reuse that says so. The
 * `128 << 12` test at 0x02000250 leaves its result in r2, and the `128 << 11`
 * block re-tests THAT SAME r2 at 0x0200026e without recomputing it -- nothing
 * writes r2 in between. So the `128 << 11` block measures from the offset the
 * `128 << 12` block just wrote when both bits are set, and from a fixed
 * -65536 when only `128 << 11` is. Read as two independent bits the else arm
 * looks like a default; it is not, it is the other bit's absence.
 *
 * The pool word 0xffff0000 at 0x02000310 does DOUBLE DUTY: it is the mask for
 * the "any option bit set" early-out at 0x020001fa and the addend in the else
 * arm at 0x0200028c/0x0200029a. It is written here as `(s32)0xffff0000` in
 * both roles so a grep of the file's constants against the pool listing finds
 * it, rather than as -65536.
 *
 * `movs r3,#13 / negs r3` at 0x020001de is the 0xfffffff3 mask the tree
 * already documents as a TWO-BIT FIELD WRITE AT BITS 2-3, not a sign
 * manipulation. It appears twice here, both times writing bits 2-3 of the
 * +80 sub-record's byte +9 -- once copied from another record's byte +9, once
 * from the low two bits of the source struct's byte 0.
 *
 * Uncertainty: none of the six callees are identified beyond call shape.
 * Func_03000380 is the IWRAM quotient helper the tree already names, used
 * here as `(distance) / table_entry->rate`, not as the proportional-decay
 * form. Record offsets are raw; no layout is established beyond the field
 * agreement with 0x02000104 described above. The selector constant 222 and
 * the `source + 24` signed halfword that replaces it are both unexplained.
 */

extern u8 *Data_0200876c[];

extern s32 Func_03000380(s32 delta, s32 rate);
extern void Func_08009080(u8 *record, s32 arg1);
extern void Func_08009098(u8 *record, const void *descriptor);
extern u8 *Func_080090c8(s32 kind, s32 a0, s32 a1, s32 a2);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a160(u8 *record, s32 arg1);

void Func_0200013c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 flags,
                   u8 *source)
{
    u8 *owner;
    u8 *record;
    u8 *sub;
    u8 *entry;
    s32 kind;
    s32 index;
    s32 have_offset;
    s32 distance;

    owner = Func_0808a080(0);

    if ((flags & (128 << 13)) != 0 && source != 0)
        kind = *(s16 *)(source + 24);
    else
        kind = 222;

    record = Func_080090c8(kind, a0, a1, a2);
    if (record == 0)
        return;

    sub = *(u8 **)(record + 80);

    Func_08009080(record, (flags + 1) & 15);

    index = flags & 15;
    Func_08009098(record, Data_0200876c[index]);

    *(u8 *)(record + 85) = 0;
    *(u8 *)(sub + 38) = 0;
    *(s32 *)(record + 108) = 0x02008105;   /* -> Func_02000104 */

    *(s32 *)(record + 68) = a3;
    *(s32 *)(record + 72) = a4;
    *(s32 *)(record + 76) = a5;

    /* bits 2-3 of the sub-record's byte +9, copied from the id-0 record's
     * own sub-record */
    *(u8 *)(sub + 9) = (*(u8 *)(sub + 9) & ~12)
                     | (*(u8 *)(*(u8 **)(owner + 80) + 9) & 12);

    *(s32 *)(record + 48) = 0;
    *(s32 *)(record + 52) = 0;
    *(u16 *)(record + 100) = 0;

    /* every remaining field is optional and needs both a high flag bit and a
     * source struct */
    if ((flags & (s32)0xffff0000) == 0)
        return;
    if (source == 0)
        return;

    if ((flags & (128 << 9)) != 0)
        Func_0808a160(record, *(s32 *)(source + 4));

    if ((flags & (128 << 10)) != 0) {
        *(u8 *)(record + 35) &= 0xfe;
        *(u8 *)(sub + 9) = (*(u8 *)(sub + 9) & ~12)
                         | ((*(u8 *)source & 3) << 2);
    }

    have_offset = flags & (128 << 12);
    if (have_offset != 0) {
        *(s32 *)(record + 24) = *(s32 *)(source + 8);
        *(s32 *)(record + 28) = *(s32 *)(source + 12);
    }

    if ((flags & (128 << 11)) != 0) {
        entry = Data_0200876c[index];
        /* the two arms differ only in what they measure from; the SECOND
         * Func_03000380 is shared between them, so there are three calls
         * here and not four */
        if (have_offset != 0) {
            *(s32 *)(record + 48) = Func_03000380(
                *(s32 *)(source + 16) - *(s32 *)(record + 24),
                *(s32 *)(entry + 12));
            distance = *(s32 *)(source + 20) - *(s32 *)(record + 28);
        } else {
            *(s32 *)(record + 48) = Func_03000380(
                *(s32 *)(source + 16) + (s32)0xffff0000,
                *(s32 *)(entry + 12));
            distance = *(s32 *)(source + 20) + (s32)0xffff0000;
        }
        *(s32 *)(record + 52) = Func_03000380(distance, *(s32 *)(entry + 12));
    }

    if ((flags & (128 << 14)) != 0) {
        Func_08009080(record, 1);
        Func_08009098(record, *(const void **)(source + 28));
    }

    if ((flags & (128 << 15)) != 0)
        *(u16 *)(sub + 30) = *(u16 *)(source + 32);

    if ((flags & (128 << 16)) != 0)
        *(u16 *)(record + 100) = *(u16 *)(source + 34);

    if ((flags & (128 << 17)) != 0)
        *(s32 *)(record + 108) = *(s32 *)(source + 36);
}

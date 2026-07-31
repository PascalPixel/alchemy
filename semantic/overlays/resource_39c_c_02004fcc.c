typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;

/*
 * resource_39c owner at 0x02004fcc, 396 bytes: reserve a record via
 * Func_0200a996 (its field 0 set to point at `arg1`, otherwise unused
 * here) and check a lookup by id 265. If the lookup fails, kick a raw
 * DMA3 word-count-7 transfer from a local stack word into `arg1` and
 * store `arg0` into `arg1`'s field 4. If the lookup succeeds, use a
 * fixed-point-packed table index to find a "slot" record and, from it,
 * conditionally spawn up to two secondary objects into `arg1`'s fields
 * 0x14/0x18 -- the same two-spawn shape resource_39c_c_02005388.c
 * already documents, but with different per-slot gating for each spawn.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02004fcc through `pop
 * {r3, r5, r6} / mov r8,r3 / mov r9,r5 / mov sl,r6 / pop {r5, r6, r7} /
 * pop {r0} / bx r0` at 0x02005146-0x02005154, followed by one alignment
 * halfword; the next owner's prologue (this overlay's own
 * resource_39c_c_02005158.c) is at 0x02005158. Unusually, this owner's
 * ten-word literal pool (0x02005118-0x0200513f) sits INLINE mid-body,
 * between the two spawn blocks, reached only by forward branches that
 * skip over it -- the same inline-pool shape
 * resource_39c_c_02002844.c/02002e08.c already document, just for a much
 * larger owner. Two arguments (r0, r1), void.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2`
 * rule.
 *
 * The DMA3 register block (0x040000d4 = REG_DMA3SAD, +4 = REG_DMA3DAD,
 * +8 = REG_DMA3CNT) is public GBA hardware, not project-proprietary --
 * spelled out by address per this project's convention of naming
 * hardware registers directly rather than inventing a symbol.
 * `0x85000007` as the control word (bit 31 enable, count 7) is recorded
 * as read, not decoded further.
 *
 * The `idx` table lookup (`Data_02000240 + 0x1f4`, the same fixed cell
 * resource_39c_c_02005900.c/02005458.c use) feeds `Func_0200ac2a(idx)`
 * -- a DIFFERENT slot-lookup shape than this overlay's usual
 * `Data_03001ebc`-array indexing, worth noting as a second lookup path
 * for the same kind of "current slot" concept. The packed table at
 * 0x02010000 (stride 4, indexed by
 * `Fixed20(slot->field16) * 128 + Fixed20(slot->field8)`) is not
 * identified; only its byte at offset 2 is read here, compared against
 * the low byte of `arg0`.
 *
 * The first spawn's owner-record[9] update (`(field9 & ~0xc) | 4`) is
 * the same idiom resource_39c_c_02005388.c/02005458.c/02003788.c use
 * elsewhere in this overlay; the second spawn's owner-record update
 * deliberately omits it (only clears field 0x26), recorded exactly as
 * compiled.
 *
 * Uncertainty: none of the nine callees are identified beyond call
 * shape; `Func_0200a996`'s returned record and `arg1`'s own fields
 * (0, 4, 0x14, 0x18) are inferred only from this call shape.
 */

extern void *Func_0200a996(s32 arg0, s32 arg1);
extern void *Func_0200aa70(s32 arg0);
extern void *Func_0200ac2a(s32 idx);
extern u8 *Func_0200aa32(s32 kind, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0200aa3c(u8 *object, void *table);
extern void Func_0200aa50(u8 *record, s32 arg1);
extern u8 *Func_0200aaa4(s32 kind, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0200aaae(u8 *object, void *table);
extern void Func_0200aac4(u8 *record, s32 arg1);

static s32 Fixed20(s32 raw)
{
    if (raw < 0) {
        raw += 0xfffff;
    }
    return raw >> 20;
}

void Func_02004fcc(s32 arg0, u8 *arg1)
{
    void **reserved = (void **)Func_0200a996(0x23, 4);
    void *lookup;

    *reserved = arg1;

    lookup = Func_0200aa70(0x109);
    if (lookup == 0) {
        volatile u32 *dma3 = (volatile u32 *)0x040000d4;
        s32 stackWord = 0;

        dma3[0] = (u32)&stackWord;
        dma3[1] = (u32)arg1;
        dma3[2] = 0x85000007;
        *(s32 *)(arg1 + 4) = arg0;
        return;
    }

    {
        s32 idx = *(s32 *)((u8 *)0x02000240 + 0x1f4);
        u8 *slot = (u8 *)Func_0200ac2a(idx);
        s32 field16 = *(s32 *)(slot + 16);
        s32 field8 = *(s32 *)(slot + 8);
        s32 packed = Fixed20(field16) * 128 + Fixed20(field8);
        u8 *tableEntry = (u8 *)0x02010000 + packed * 4;
        u8 *object;
        u8 *ownerRecord;

        if (*(s32 *)(arg1 + 0) != 0 && *(s32 *)(arg1 + 0x14) != 0) {
            object = Func_0200aa32(26, field8, *(s32 *)(slot + 12) + 0x180000, field16);
            if (object != 0) {
                *(s32 *)(object + 20) = *(s32 *)(slot + 20);
                ownerRecord = *(u8 **)(object + 0x50);
                Func_0200aa3c(object, (void *)0x0200de38);

                object[0x55] = 4;
                *(u8 **)(object + 0x68) = slot;
                *(s32 *)(object + 12) = *(s32 *)(object + 12) - 0x8000;

                if (ownerRecord != 0) {
                    Func_0200aa50(ownerRecord, 6 - *(s32 *)(arg1 + 0));
                    ownerRecord[0x26] = 0;
                    ownerRecord[9] = (ownerRecord[9] & ~0x0c) | 4;
                }

                *(u8 **)(arg1 + 0x14) = object;
            } else {
                *(s32 *)(arg1 + 0x14) = 0;
            }
        } else {
            *(s32 *)(arg1 + 0x14) = 0;
        }

        if (tableEntry[2] == arg0 && *(s32 *)(arg1 + 0x18) != 0) {
            object = Func_0200aaa4(26, field8, *(s32 *)(slot + 12), field16);
            if (object != 0) {
                *(s32 *)(object + 20) = *(s32 *)(slot + 20);
                ownerRecord = *(u8 **)(object + 0x50);
                Func_0200aaae(object, (void *)0x0200de20);

                object[0x55] = 0;
                *(short *)(object + 0x64) = 0;
                object[0x23] = 2;
                *(s32 *)(object + 0x30) = 0x40000;

                if (ownerRecord != 0) {
                    Func_0200aac4(ownerRecord, 6);
                    ownerRecord[0x26] = 0;
                }

                *(u8 **)(arg1 + 0x18) = object;
            } else {
                return;
            }
        } else {
            *(s32 *)(arg1 + 0x18) = 0;
        }
    }
}

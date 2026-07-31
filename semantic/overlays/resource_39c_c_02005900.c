typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;

/*
 * resource_39c owner at 0x02005900, 72 bytes: tick a countdown timer on the
 * current slot record, and reroll it to a fresh random 10-39 count once it
 * reaches zero.
 *
 * Complete owner: `push {r5, lr}` at 0x02005900 through `pop {r5} / pop {r0}
 * / bx r0` at 0x200593a-0x200593e; the next owner's prologue is at
 * 0x02005948, after the three-word literal pool 0x02005940-0x02005947. No
 * incoming arguments are read before being overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * `record = *Data_03001edc` is this overlay's "current object" global (a
 * pointer read once at 0x03001edc, dereferenced once more to reach the live
 * record); the slot lookup `((s32 **)(Data_03001ebc + 0x14))[idx]` is the
 * exact indexing resource_39c_c_02000350.c already documents for this
 * overlay's runtime slot table, with `idx` read here from the fixed cell at
 * 0x02000434 (0x02000240 + 0x1f4) rather than a loop variable.
 *
 * Uncertainty: Func_0200aa80 and Func_0200b2c0 are not identified; the
 * latter's single-word result is scaled `(rnd * 30) >> 16 + 10`, the
 * standard "shrink a 16-bit random value to a small inclusive range" idiom,
 * so Func_0200b2c0 is presumed to be a random-number source but that is not
 * established here. The role of the timer field (record[2], byte offset 8)
 * beyond "counts down, and something fires through the looked-up slot when
 * it reaches zero" is not established either.
 */

extern s32 **Data_03001edc;
extern u8 *Data_03001ebc;
extern u32 Data_02000434;

extern void Func_0200aa80(s32 *slot);
extern u32 Func_0200b2c0(void);

void Func_02005900(void)
{
    s32 *record = *Data_03001edc;
    u32 idx = Data_02000434;
    s32 *slot = ((s32 **)(Data_03001ebc + 0x14))[idx];

    if (record[2] != 0) {
        record[2] = record[2] - 1;
    } else {
        Func_0200aa80(slot);
        record[2] = (s32)((Func_0200b2c0() * 30) >> 16) + 10;
    }
}

typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

/*
 * resource_3c9 owner at 0x02002200, 308 bytes: spawn a fixed PAIR of
 * objects from the same three caller-record fields, then cross-configure
 * both from the caller's own owner record. The one row in this overlay's
 * queue with a real (2-iteration) loop and dense bitfield packing.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + the r8/r9/sl/fp spill and an
 * 8-byte stack frame at 0x02002200 through the matching unwind and
 * `pop {r0} / bx r0` at 0x2002326-0x2002328; two trailing pool words
 * (0x0200a1ad, 0x0200a15d -- both odd, thumb continuation pointers for
 * the two spawned objects' field 0x6c) end at 0x2002333, next owner's
 * prologue (`push {lr}`) at 0x02002334.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * Shape notes, all verified against the disassembly:
 * - The two spawned object pointers live in an sp[2] array (sl is aliased
 *   to sp and indexed r7*4); a failed spawn stores its NULL and skips the
 *   per-object setup, but the post-loop cross-configure dereferences both
 *   slots unconditionally -- kept exactly as compiled.
 * - The per-record bitfield block: record[0x1c] is first passed (old
 *   value) to Func_02007ebc, then overwritten from the u16 at
 *   globalCtx+0x46 (byte store); record[0x1d] |= 1; then the NEW
 *   record[0x1c] indexes the 4-byte-stride IWRAM table at 0x03001b10,
 *   whose u16 at +2 supplies bits: field8 (u16 at record+8) =
 *   (field8 & 0xfc00) | ((entry >> 5) & 0x3ff), via the lsls#17/lsrs#22
 *   pair. record[5] = (record[5] & 0x1f) | 0x40 (the compiled form
 *   clears bit 5 twice, once via ~0x21 and once via the &0x3f mask --
 *   folded here to the net masks, same result on every input byte);
 *   record[7] = (record[7] & 0x3f) | 0x80; and byte 0x16 of the record's
 *   own field-0x28 pointer is zeroed.
 * - Post-loop: both spawned records take the caller record's field-9
 *   0x0c bits through the familiar clear-two-bits-then-copy idiom
 *   (resource_39c/resource_3c9's drafted owners), object 0's field 0x6c
 *   gets 0x0200a1ad, object 1's gets 0x0200a15d, and only object 1's
 *   byte 0x23 is set to 2.
 *
 * Uncertainty: the four callees, the globalCtx record at *(u32*)0x03001f30,
 * the 0x03001b10 table's meaning, and the two continuation pointers are
 * identified by shape only.
 */

extern void Func_020080e8(s32 arg0);
extern u8 *Func_02007e9c(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02007ebe(u8 *record, s32 arg1);
extern void Func_02007ebc(s32 arg0);

void Func_02002200(u8 *caller)
{
    u8 *globalCtx = *(u8 **)0x03001f30;
    u8 *slots[2];
    u8 *object;
    u8 *record;
    s32 i;

    Func_020080e8(0x83);

    for (i = 0; i <= 1; i++) {
        object = Func_02007e9c(26, *(s32 *)(caller + 8), *(s32 *)(caller + 12),
                               *(s32 *)(caller + 16));
        slots[i] = object;
        if (object == 0) {
            continue;
        }

        *(s32 *)(object + 0x14) = *(s32 *)(caller + 0x14);
        record = *(u8 **)(object + 0x50);
        object[0x55] = 0;
        *(u16 *)(object + 0x64) = 0;
        *(u8 **)(object + 0x68) = caller;
        if (record == 0) {
            continue;
        }

        Func_02007ebe(record, 0);
        record[0x26] = 0;
        Func_02007ebc(record[0x1c]);
        record[0x1c] = *(u16 *)(globalCtx + 0x46);
        record[0x1d] |= 1;
        {
            u16 entry = *(u16 *)((u8 *)0x03001b10 + record[0x1c] * 4 + 2);
            *(u16 *)(record + 8) =
                (*(u16 *)(record + 8) & 0xfc00) | ((entry >> 5) & 0x3ff);
        }
        record[5] = (record[5] & 0x1f) | 0x40;
        record[7] = (record[7] & 0x3f) | 0x80;
        (*(u8 **)(record + 0x28))[0x16] = 0;
    }

    *(u32 *)(slots[0] + 0x6c) = 0x0200a1ad;
    {
        u8 *callerRecord = *(u8 **)(caller + 0x50);
        u8 *record0 = *(u8 **)(slots[0] + 0x50);
        u8 *record1;

        record0[9] = (record0[9] & ~0x0c) | (callerRecord[9] & 0x0c);

        record1 = *(u8 **)(slots[1] + 0x50);
        record1[9] = (record1[9] & ~0x0c) | (callerRecord[9] & 0x0c);
        *(u32 *)(slots[1] + 0x6c) = 0x0200a15d;
        slots[1][0x23] = 2;
    }
}

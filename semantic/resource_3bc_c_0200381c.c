#include "types.h"

/*
 * resource_3bc owner at 0x0200381c, 190 bytes: fetch the caller's scene
 * record, run a one-shot-flag/else cutscene-primitive branch, then stamp
 * three fields of the record from a shared task's state words and reset the
 * rest of its presentation fields before starting its presentation.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x0200381c through the matching
 * interworking return `pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x20038d4-0x20038d8, followed by the two-word literal pool
 * 0x20038c4-0x20038c7 (mid-body, branched over by nothing -- it is simply
 * unreferenced-after padding before the trailing four pool words) that
 * closes the 190-byte span.  One argument (r0), void.
 *
 * Not found by the structural inventory walk (unindexed); nine call sites
 * over seven distinct targets, all resolved with the `+2` rule against the
 * raw image (this owner starts
 * outside a recorded row, so overlay_call_targets.ts's own table does not
 * cover it -- resolved with `bun tools/overlay-show resource_3bc 381c |
 * cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3bc 381c 38d8 --annotate`):
 *   0x2003824 -> veneer -> Func_0808a080(arg0)         scene-record accessor, established
 *   0x2003840 -> veneer -> Func_0808a100(arg0, 1)       established shape (resource_37a/3c8/37b)
 *   0x2003858 -> veneer -> Func_0808a1b8(arg0, 0x4000, 30)   established shape (resource_37a/37b)
 *   0x2003860 -> veneer -> Func_0808a100(arg0, 3)
 *   0x2003866 -> veneer -> Func_0808a010(30)            established, frame-count wait/delay (resource_3c1: "wait for the beat to settle")
 *   0x20038a8 -> veneer -> Func_080091e0(record, 1)     established (record, phase), resource_3ba_c_02000a3c.c
 *   0x20038b0 -> veneer -> Func_08009080(record, 0)     established (record, mode), same file
 *   0x20038b8 -> veneer -> Func_08009080(record, 1)     called twice, modes 0 then 1
 *   0x20038be -> veneer -> Func_080000c0(1)             established, advance the task scheduler (resource_3cd_c_020000ec.c)
 *
 * `runtime` (0x03001e68) is the same generic per-mode IWRAM state slot
 * documented across the main image (semantic/main/0800aa0c.c,
 * 080aa56c.c, 0800c150.c, 08029554.c, 0800c880.c, 080a7478.c all read it as
 * `*(TYPE **)0x03001e68` for whatever subsystem currently owns it); this
 * owner only touches its bytes at +6/+7, so its specific owner here is not
 * established beyond that.
 *
 * Data_0200dc44/Data_0200db9c/Data_0200dbc8 are the same three-word group
 * referenced from resource_3bc's parked sibling 0x02003640 (see
 * a prior working note's neighbourhood -- 0x02003640 was
 * disassembled but not drafted in this reconstruction); copied here into the record's
 * fields at +8/+16/+6 respectively, the last one truncated from a full word
 * read to a stored halfword.
 *
 * Uncertainty: Data_02000240[498] (byte) is a one-shot flag read-and-cleared
 * on the "already primed" arm; its broader role and the record's numeric
 * field offsets (+0x22, +0x24, +0x2c, +0x38, +0x40, +0x55) are not otherwise
 * cross-referenced from this owner alone.
 */

extern u8 Data_02000240[];
extern u32 Data_0200dc44;
extern u32 Data_0200db9c;
extern u32 Data_0200dbc8;

/* Call symbols are per-site (the raw disassembly shows a DIFFERENT veneer
 * target at every occurrence, including the repeated Func_0808a100 and
 * Func_08009080 calls) -- declared/named as the literal per-site targets,
 * not the shared ultimate-destination symbol. */
u8 *Func_0200823e();           /* Func_0808a080 veneer */
void Func_020082b2();          /* Func_0808a100 veneer #1 */
void Func_0200832a();          /* Func_0808a1b8 veneer */
void Func_020082d2();          /* Func_0808a100 veneer #2 */
void Func_02008260();          /* Func_0808a010 veneer */
void Func_02008202();          /* Func_080091e0 veneer */
void Func_02008192();          /* Func_08009080 veneer #1 */
void Func_0200819a();          /* Func_08009080 veneer #2 */
void Func_02008100();          /* Func_080000c0 veneer */

void Func_0200381c(s32 arg0)
{
    u8 *runtime;
    u8 *record;

    runtime = *(u8 **)0x03001e68;
    record = Func_0200823e(arg0);

    if (Data_02000240[498] == 1) {
        Data_02000240[498] = 0;
        Func_020082b2(arg0, 1);
    } else {
        Func_0200832a(arg0, 0x4000, 30);
        Func_020082d2(arg0, 3);
        Func_02008260(30);
    }

    runtime[7] = 0;
    runtime[6] = 15;

    *(u32 *)(record + 8) = Data_0200dc44;
    *(u32 *)(record + 16) = Data_0200db9c;
    *(u16 *)(record + 6) = (u16)Data_0200dbc8;

    *(u32 *)(record + 56) = 0x80000000;
    record[0x55] = 3;
    *(u32 *)(record + 64) = 0x80000000;

    record[0x22] = 0;
    *(u32 *)(record + 36) = 0;
    *(u32 *)(record + 44) = 0;
    *(u32 *)(record + 12) = 0;
    *(u32 *)(record + 20) = 0;

    Func_02008202(record, 1);
    Func_02008192(record, 0);
    Func_0200819a(record, 1);

    Func_02008100(1);
}

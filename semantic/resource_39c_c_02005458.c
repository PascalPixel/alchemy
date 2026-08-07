#include "types.h"

/*
 * resource_39c owner at 0x02005458, 288 bytes: drive the current record's
 * linked-object state machine -- spawn the linked object if missing, run
 * a short countdown against it, then transition the record to its
 * terminal state.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02005458 through `pop
 * {r3, r5, r6} / mov r8,r3 / mov r9,r5 / mov sl,r6 / pop {r5, r6, r7} /
 * pop {r0} / bx r0` at 0x02005550-0x0200555e; the six-word literal pool
 * 0x02005560-0x02005577 follows, then the next owner's prologue (this
 * overlay's own resource_39c_c_02005578.c) at 0x02005578. No incoming
 * arguments are read before being overwritten, so `void`. Only runs when
 * `record->field0` is 0, 1, or 2 (the unsigned `bhi` guard at the top);
 * on any other value the owner is a no-op.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/lib/overlay_call_targets.ts`'s `+2` rule.
 *
 * `record = *Data_03001edc` and the `slots[idx]` lookup are the same
 * current-object/slot-table pair resource_39c_c_02005900.c and
 * resource_39c_c_02005578.c already document. The spawn's owner-record[9]
 * update (`(field9 & ~0xc) | 4`) is the same idiom
 * resource_39c_c_02005388.c uses elsewhere in this overlay.
 *
 * `object[0x54]` is stored the constant 1 on every one of the loop's (up
 * to three) iterations; since the loop never changes what it stores
 * there, this is hoisted to a single store before the loop rather than
 * repeated -- a behaviour-preserving simplification, not a structural
 * change (the loop's other two calls per iteration are kept exactly as
 * compiled, including that the loop counter resumes from the record's
 * current state rather than always starting at 0).
 *
 * Uncertainty: none of the eight callees are identified beyond call
 * shape; `Func_0808a018`, `Func_02005388` and `Func_0808a020` are called
 * with no freshly-set register and are declared old-style rather than
 * credited with a phantom argument, per this project's own "arities vary
 * per site" convention (resource_3cb_c_02000580.c).
 */

extern s32 **Data_03001edc;
extern u8 *Data_03001ebc;
extern s32 Data_02000434;

extern void Func_0808a018();
extern u8 *Func_080090c8(s32 kind, s32 arg1, s32 arg2, s32 arg3);
extern void Func_08009098(u8 *object, void *table);
extern void Func_02005388();
extern void Func_080000c0(s32 arg0);
extern void Func_08009080(u8 *object, s32 arg1);
extern void Func_080770c8(s32 arg0);
extern void Func_0808a020();

void Func_02005458(void)
{
    s32 permuted_1;
    s32 idx = Data_02000434;
    u8 *slot = ((u8 **)(Data_03001ebc + 0x14))[idx];
    s32 *record = *Data_03001edc;
    u8 *object;
    s32 counter;
    s32 i;

    if ((u32)record[0] > 2) {
        return;
    }

    Func_0808a018();

    object = (u8 *)record[5];
    if (object == 0) {
        u8 *ownerRecord;

        object = Func_080090c8(26, *(s32 *)(slot + 8), *(s32 *)(slot + 12) + 0x180000, *(s32 *)(slot + 16));
        if (object != 0) {
            ownerRecord = *(u8 **)(object + 0x50);
            permuted_1 = *(s32 *)(slot + 20);
            Func_08009098(object, (void *)0x0200de38);
            *(s32 *)(object + 20) = permuted_1;

            *(u8 **)(object + 0x68) = slot;
            object[0x55] = 4;
            *(s32 *)(object + 12) = *(s32 *)(object + 12) - 0x8000;

            if (ownerRecord != 0) {
                ownerRecord[0x26] = 0;
                ownerRecord[9] = (ownerRecord[9] & ~0x0c) | 4;
            }

            object[0x54] = 0;
            record[5] = (s32)object;
        }
    }

    object = (u8 *)record[5];

    counter = record[0];
    if (counter <= 2) {
        object[0x54] = 1;
        for (i = counter; i <= 2; i++) {
            Func_02005388();
            Func_080000c0(30);
            Func_08009080(object, 5 - i);
        }
    }

    Func_080770c8(0x161);
    Func_0808a020();
    record[0] = 3;
    *(s32 *)(object + 12) = (*(s32 *)(object + 8) & (s32)0xfff00000) + 0x80000;
    *(s32 *)(object + 16) = (*(s32 *)(object + 16) & (s32)0xfff00000) + 0x80000;
}

#include "types.h"

/*
 * resource_3bb owner at 0x02002ba8, 472 bytes: reserve a record from a
 * fixed global, seed a handful of fixed cells from its fields, run a
 * setup pair, clear-and-set two bits in the owner record's fields 5 and
 * 0x11, append up to three entries (one one-shot, one per loop
 * iteration across an 8-iteration loop, one more one-shot) to a shared,
 * 32-capped fixed-size table -- each append guarded by a brief REG_IME
 * toggle -- and finally write the object's fields 0x18/0x1c twice more
 * around two more setup calls.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + the r8/r9/sl/fp spill at
 * 0x02002ba8 through `pop {r3, r5, r6, r7} / mov r8,r3 / mov r9,r5 /
 * mov sl,r6 / mov fp,r7 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x02002d54-0x02002d62; the six-word literal pool 0x02002d64-0x02002d7f
 * follows; the next owner's prologue is at 0x02002d80. Three arguments
 * (r0, r1, r2), void.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * `current = *Data_03001e68` is the same single-dereference global
 * resource_3bb_c_02002d84.c documents. The owner-record[5]/[0x11]
 * `(field & ~0xc) | 4` update is the same idiom
 * resource_3bb_c_02003138.c and resource_39c's siblings use.
 *
 * The REG_IME sequence (0x04000208, GBA's public Interrupt Master
 * Enable register) recurs three times with the identical shape
 * `saved = REG_IME; REG_IME = (u16)&REG_IME; <maybe append an entry>;
 * REG_IME = saved;` -- confirmed by re-reading the raw halfword-store
 * operand order against an unambiguous same-function example
 * (`strh r5, [r7, #6]`, source-then-base) rather than assumed. The
 * middle write's source is genuinely the register's own address
 * (truncated to 16 bits, 0x0208, nonzero), not zero or an unrelated
 * value -- written here exactly as compiled (`BeginCritical`/
 * `EndCritical` below) rather than normalised into the canonical
 * "disable interrupts" (`REG_IME = 0`) idiom, since the actual byte
 * pattern briefly forces IME *nonzero* (enabled) instead. Why the
 * source program does this rather than a plain `1` is not established.
 * All three sites share one 32-capped, 12-byte-stride table at a fixed
 * counter cell (0x02002090); `AppendIfRoom` below collects that shape.
 *
 * Uncertainty: none of the thirteen callees are identified beyond call
 * shape; `resultObj`'s fields (6, 8, 16, 0x23, 0x50, 0x18, 0x1c) and the
 * shared table's entry layout are inferred only from this call shape.
 */

extern u8 *Data_03001e68;

extern u8 *Func_0808a080(void);
extern void Func_0808a1e0(s32 arg0, s32 arg1);
extern void Func_080091e0(u8 *object, s32 arg1);
extern void Func_08009080(u8 *object, s32 arg1);
extern void Func_0808a0f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a148(s32 arg0, s32 arg1, s32 arg2);
extern void Func_080f9010(s32 arg0);
extern void Func_080000c0(s32 arg0);
extern void Func_0808a010(s32 arg0);
extern void Func_0808a110(s32 arg0, s32 arg1);

static u16 BeginCritical(volatile u16 *regIME)
{
    u16 saved = *regIME;
    *regIME = (u16)(u32)regIME;
    return saved;
}

static void EndCritical(volatile u16 *regIME, u16 saved)
{
    *regIME = saved;
}

static void AppendIfRoom(u16 *counter, u32 word0, u32 word1, u32 word2)
{
    if (*counter <= 31) {
        u8 *entry = (u8 *)counter + *counter * 12 + 4;
        *(u32 *)entry = word0;
        *(u32 *)(entry + 4) = word1;
        *(u32 *)(entry + 8) = word2;
        *counter += 1;
    }
}

void Func_02002ba8(s32 arg0, s32 arg1, s32 arg2)
{
    u8 *current = Data_03001e68;
    u8 *resultObj;
    u8 *ownerRecord;
    volatile u16 *regIME = (volatile u16 *)0x04000208;
    u16 *counter = (u16 *)0x02002090;
    u16 saved;
    s32 i;

    resultObj = Func_0808a080();
    current[6] = 1;

    current[7] = 4;
    *(u32 *)0x0200cca4 = *(s32 *)(resultObj + 8);
    *(u32 *)0x0200cbfc = *(s32 *)(resultObj + 16);
    ownerRecord = *(u8 **)(resultObj + 0x50);
    *(u32 *)0x0200cc28 = *(u16 *)(resultObj + 6);

    Func_0808a1e0(arg0, 2);

    resultObj[0x23] |= 1;
    *(u16 *)(resultObj + 6) = 0x4000;

    Func_080091e0(resultObj, 3);
    Func_08009080(resultObj, 0);
    Func_08009080(resultObj, 1);

    Func_0808a0f0(arg0, arg1 << 16, arg2 << 16);
    Func_0808a148(0, 0x4000, 0);

    saved = BeginCritical(regIME);
    AppendIfRoom(counter, 0xf00, 0x04000050, 0x20000);
    EndCritical(regIME, saved);

    ownerRecord[5] = (ownerRecord[5] & ~0x0c) | 4;
    ownerRecord[0x11] = (ownerRecord[0x11] & ~0x0c) | 4;

    Func_080f9010(252);

    for (i = 0; i <= 14; i += 2) {
        *(s32 *)(resultObj + 0x18) = i * 0x1000 + 0x1000;
        *(s32 *)(resultObj + 0x1c) = 0x1f000 - i * 0x1000;

        saved = BeginCritical(regIME);
        AppendIfRoom(counter, ((15 - i) << 8) | (i + 1), 0x04000052, 0x20000);
        EndCritical(regIME, saved);

        Func_080000c0(1);
    }

    saved = BeginCritical(regIME);
    AppendIfRoom(counter, 16, 0x04000052, 0x20000);
    EndCritical(regIME, saved);

    *(s32 *)(resultObj + 0x18) = 0x11000;
    *(s32 *)(resultObj + 0x1c) = 0xf000;
    Func_0808a010(1);

    *(s32 *)(resultObj + 0x18) = 0x10000;
    *(s32 *)(resultObj + 0x1c) = 0x10000;
    Func_0808a010(13);

    Func_0808a110(arg0, 3);
    Func_0808a010(20);
}

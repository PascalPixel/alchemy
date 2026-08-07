#include "types.h"

/*
 * resource_3c9 owner at 0x02003bc8, 724 bytes: another long, fully
 * linear scripted-scene setup (no branches, one inline literal-pool
 * word reached only by a forward branch, the usual shape) -- a run of
 * id-tagged record field setups sharing several cached constants
 * across sibling calls, two records (24/25) sharing almost every
 * field value, and a closing run of id-19/20 setup calls that reuse a
 * single cached value (152<<17) across three unrelated uses.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02003bc8 through `pop {r5} /
 * pop {r0} / bx r0` at 0x02003e7c-0x02003e86, followed by the
 * five-word literal pool 0x02003e8c-0x02003e9b; the next owner's
 * prologue is exactly at 0x02003e9c (this overlay's own
 * resource_3c9_c_02003e9c.c candidate, not yet drafted). No incoming
 * arguments are read before being overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `bun tools/lib/overlay_call_targets.ts resource_3c9
 * 3bc8 3e9c`'s `+2` rule.
 *
 * `Func_0808a228`'s result is used as a record pointer here (`+85`
 * byte write, `str r0,[r5,...]`-style follow-on), unlike
 * resource_3c9_c_02003924.c's call to the same import where the
 * result is never touched afterward -- declared here with a return
 * type per this file's own call shape, per this project's "arities
 * and apparent behaviour vary per site" convention rather than
 * reconciling the two declarations.
 *
 * SHARED IDIOMS: the id-tagged `Func_0808a0f0`/`Func_0808a100` setup
 * calls and the `Func_0808a080`/`Func_080091e0` id-record
 * pass-through are the same shapes already cited in this overlay's
 * other drafts.
 *
 * Uncertainty: none of the twenty-odd callees are identified beyond
 * call shape; every id/constant is recorded as a passed value; the
 * three fields records 24 and 25 share verbatim (+28, +24, +0x55) are
 * not otherwise explained.
 */

extern void Func_080f9010(s32 arg0);
extern void Func_080091c0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern u8 *Func_0808a080();
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_080091e0(void *record, s32 arg1);
extern void Func_0808a158(s32 id, s32 arg1);
extern void Func_0808a1e0(s32 id, s32 arg1);
extern u8 *Func_0808a228();
extern void Func_080000d0(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_080000c0(s32 arg0);
extern void Func_08009128(void);
extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_0808a010(s32 frames);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a170(s32 dialogue_id);
extern void Func_02000894(s32 id);
extern void Func_0808a188(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a088(s32 id);

void Func_02003bc8(void)
{
    u8 *record;
    s32 sharedValue = 152 << 17; /* reused across ids 24, 25 and a later call */
    s32 sharedField16 = 152 << 16;
    s32 sharedField12 = 224 << 13;
    s32 zero = 0;

    Func_080f9010(141);
    Func_080091c0(17, 10, 4, 2, 17, 8);

    record = Func_0808a080(0);
    *(u16 *)(record + 6) = 160 << 8;

    record = Func_0808a080(1);
    *(u16 *)(record + 6) = 160 << 8;
    Func_0808a0f0(1, 164 << 17, 168 << 16);

    record = Func_0808a080(2);
    *(u16 *)(record + 6) = 160 << 8;
    Func_0808a0f0(2, 170 << 17, 196 << 16);

    record = Func_0808a080(3);
    *(u16 *)(record + 6) = 160 << 8;
    Func_0808a0f0(3, 163 << 17, 204 << 16);

    record = Func_0808a080(21);
    *(u16 *)(record + 6) = 208 << 8;
    Func_0808a0f0(21, 200 << 16, 216 << 16);

    record = Func_0808a080(6);
    *(u16 *)(record + 6) = 208 << 8;
    Func_0808a0f0(6, 200 << 16, 216 << 16);

    record = Func_0808a080(20);
    *(u16 *)(record + 6) = 192 << 6;
    Func_0808a0f0(20, 155 << 17, 158 << 16);

    record = Func_0808a080(19);
    *(u16 *)(record + 6) = 192 << 6;
    Func_0808a0f0(19, 146 << 17, 158 << 16);

    /* Record 24. */
    Func_080091e0(Func_0808a080(24), 0);
    Func_0808a158(24, 7);
    Func_0808a1e0(24, 1);
    record = Func_0808a080(24);
    *(s32 *)(record + 28) = (s32)0xffff0000;
    *(s32 *)(record + 24) = 0x3333;
    record[0x55] = (u8)zero;
    *(s32 *)(record + 8) = sharedValue;
    *(s32 *)(record + 12) = 128 << 10;
    *(s32 *)(record + 16) = 192 << 15;

    /* Record 25, sharing three fields verbatim with record 24. */
    Func_080091e0(Func_0808a080(25), 0);
    Func_0808a158(25, 7);
    Func_0808a1e0(25, 1);
    record = Func_0808a080(25);
    *(s32 *)(record + 28) = (s32)0xffff0000;
    record[0x55] = (u8)zero;
    *(s32 *)(record + 24) = 0x3333;
    *(s32 *)(record + 8) = sharedValue;
    *(s32 *)(record + 12) = 136 << 14;
    *(s32 *)(record + 16) = 192 << 15;

    Func_080000d0(0x0200b6d1, 200 << 4);

    record = Func_0808a228();
    record[0x55] = (u8)zero;
    Func_0808a210(sharedValue, 128 << 14, 180 << 16, 0);

    Func_080000c0(1);
    Func_08009128();
    Func_080000c0(1);
    Func_080091f0(128 << 9, 128 << 9, 128 << 9);

    Func_0808a360();
    Func_0808a370();
    Func_0808a010(40);

    Func_0808a138(20, 2);
    Func_0808a010(10);
    Func_0808a170(0x2809);
    Func_02000894(20);
    Func_0808a138(19, 3);
    Func_0808a010(20);
    Func_0808a188(19, 0, 40);
    Func_080f9010(17);

    record = Func_0808a080(20);
    *(s32 *)(record + 8) = 154 << 17;
    *(s32 *)(record + 12) = sharedField12;
    *(s32 *)(record + 16) = sharedField16;
    Func_0808a100(20, 10);
    Func_0808a010(20);

    *(s32 *)(record + 12) = sharedField12;
    *(s32 *)(record + 8) = 153 << 17;
    *(s32 *)(record + 16) = sharedField16;
    Func_0808a100(20, 11);
    Func_0808a010(12);

    *(s32 *)(record + 8) = sharedValue;
    *(s32 *)(record + 12) = 168 << 13;
    *(s32 *)(record + 16) = sharedField16;
    Func_0808a100(20, 12);
    Func_0808a010(8);
    Func_0808a088(20);

    record = Func_0808a080(19);
    *(s32 *)(record + 12) = sharedField12;
    *(s32 *)(record + 8) = 147 << 17;
    *(s32 *)(record + 16) = sharedField16;
    Func_0808a100(19, 8);
    Func_0808a010(20);

    *(s32 *)(record + 8) = 150 << 17;
    *(s32 *)(record + 12) = 216 << 13;
    *(s32 *)(record + 16) = sharedField16;
    Func_0808a100(19, 9);
    Func_0808a010(12);

    *(s32 *)(record + 8) = sharedValue;
    *(s32 *)(record + 12) = 136 << 13;
    *(s32 *)(record + 16) = sharedField16;
    Func_0808a100(19, 10);
    Func_0808a010(8);
    Func_0808a088(19);

    Func_0808a010(160);
}

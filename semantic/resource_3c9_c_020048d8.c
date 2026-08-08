#include "types.h"

/*
 * resource_3c9 owner at 0x020048d8, 592 bytes: a large linear
 * scripted-scene setup, no branches -- seven six-argument stack-passed
 * calls, a run of four id-0..3 spawn/reset blocks each writing three
 * fixed-point fields and clearing byte +99 before calling the leaf
 * helper this overlay's own resource_3c9_c_020059f0.c already names
 * (Func_02005688), then per-id cleanup and closing calls. Called from
 * this overlay's own resource_3c9_c_020038dc.c.
 *
 * Complete owner: `push {r5, r6, lr} / mov r6,sl / mov r5,r9 / push
 * {r5, r6} / mov r6,r8 / push {r6} / sub sp,#8` at
 * 0x020048d8-0x020048e6 (the 8-byte frame carries the two stack
 * arguments of the six-argument calls) through `add sp,#8 / pop {r3,
 * r5, r6} / mov r8,r3 / mov r9,r5 / mov sl,r6 / pop {r5, r6} / pop {r0}
 * / bx r0` at 0x02004b0c-0x02004b1a, followed by the three-word literal
 * pool 0x02004b1c-0x02004b27; the next owner's prologue is exactly at
 * 0x02004b28 (this overlay's own resource_3c9_c_02004b28.c, already
 * drafted). No incoming arguments are read before being overwritten, so
 * `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3c9
 * 48d8 4b28`'s `+2` rule.
 *
 * SHARED IDIOMS: `Func_0808a080`/`Func_080091e0` id-record pass-through
 * (cited from resource_3b9_c_02001298.c), `Func_0808a210(-1,-1,-1,0)`
 * (also cited from there, same four-argument shape). The
 * workspace+456 write here is a single direct store of 1, not the
 * chained additive/subtractive idiom -- recorded as a plain write since
 * the source computes the address in one step rather than caching a
 * displacement.
 *
 * Uncertainty: none of the seventeen callees are identified beyond call
 * shape; the four record blocks' three written fields (+8, +12, +16)
 * and the two cleared bytes (+40's cache aside, +98/+99) are raw
 * offsets, no layout established.
 */

extern u8 *Data_03001ebc;

extern void Func_0808a088(s32 id);
extern void Func_080f9010(s32 arg0);
extern void Func_080091c0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_08009180(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a5b8(void);
extern void Func_08009128(void);
extern void Func_080000c0(s32 arg0);
extern void Func_0808a100(s32 id, s32 arg1);
extern u8 *Func_0808a080();
extern void Func_080091e0(void *record, s32 arg1);
extern void Func_02005688(void *record);
extern void Func_0808a158(s32 arg0, s32 arg1);
extern void Func_080000d0(s32 arg0, s32 arg1);
extern void Func_0808a360(void);
extern void Func_0808a338(s32 arg0, s32 arg1);
extern void Func_0808a330(s32 arg0, s32 arg1);
extern void Func_0808a348(s32 arg0);

void Func_020048d8(void)
{
    u8 *workspace = Data_03001ebc;
    u8 *record;
    s32 shared_pos = 128 << 14;    /* 2097152, shared across the four record blocks */
    s32 shared_size = 128 << 10;   /* 131072, shared across the four record blocks */
    s32 field16_2;

    Func_0808a088(20);
    Func_0808a088(19);
    Func_080f9010(141);

    Func_080091c0(17, 10, 4, 2, 17, 8);
    Func_08009180(102, 4, 74, 4, 18, 23);
    Func_08009180(39, 72, 11, 72, 16, 21);

    Func_080091c0(19, 6, 3, 7, 22, 6);
    Func_080091c0(19, 6, 3, 7, 13, 6);
    Func_080091c0(19, 6, 3, 7, 22, 13);
    Func_080091c0(19, 6, 3, 7, 13, 13);

    Func_080091f0(128 << 10, 128 << 10, 128 << 9);
    Func_0808a210(-1, -1, -1, 0);
    Func_0808a5b8();
    Func_08009128();
    Func_080000c0(1);
    Func_080f9010(138);

    Func_0808a100(0, 19);
    Func_0808a100(1, 18);
    Func_0808a100(2, 18);
    Func_0808a100(3, 18);

    Func_080091e0(Func_0808a080(0), 0);
    Func_080091e0(Func_0808a080(1), 0);
    Func_080091e0(Func_0808a080(2), 0);
    Func_080091e0(Func_0808a080(3), 0);

    /* id 0 */
    record = Func_0808a080(0);
    *(s32 *)(record + 8) = 173 << 17;
    *(s32 *)(record + 12) = shared_pos;
    *(s32 *)(record + 16) = 205 << 16;
    Func_02005688(record);
    *(u8 *)(record + 99) = 0;
    *(s32 *)(record + 40) = shared_size;

    /* id 1 */
    record = Func_0808a080(1);
    *(s32 *)(record + 8) = 178 << 17;
    *(s32 *)(record + 12) = shared_pos;
    *(s32 *)(record + 16) = 192 << 16;
    Func_02005688(record);
    *(u8 *)(record + 99) = 0;
    *(s32 *)(record + 40) = shared_size;

    /* id 2 */
    record = Func_0808a080(2);
    *(s32 *)(record + 8) = 180 << 17;
    field16_2 = 222 << 16;
    *(s32 *)(record + 16) = field16_2;
    *(s32 *)(record + 12) = shared_pos;
    Func_02005688(record);
    *(u8 *)(record + 99) = 0;
    *(s32 *)(record + 40) = shared_size;

    /* id 3 */
    record = Func_0808a080(3);
    *(s32 *)(record + 8) = 167 << 17;
    *(s32 *)(record + 16) = field16_2;
    *(s32 *)(record + 12) = shared_pos;
    Func_02005688(record);
    *(u8 *)(record + 99) = 0;
    *(s32 *)(record + 40) = shared_size;

    *(u8 *)(Func_0808a080(21) + 98) = 0;
    *(u8 *)(Func_0808a080(6) + 98) = 0;

    record = Func_0808a080(23);
    *(u8 *)(record + 85) |= 4;
    Func_0808a158(23, 4);

    Func_080000d0((s32)0x0200da29, 200 << 4);

    *(u32 *)(workspace + 456) = 1;

    Func_0808a360();
    Func_0808a338(0x7fff, 0);
    Func_0808a330(128 << 9, 0);
    Func_0808a348(40);
    Func_080000c0(60);
}

#include "types.h"

/*
 * resource_3c9 owner at 0x02003924, 676 bytes: a long, fully linear
 * scripted-scene setup (no branches) -- bracket open, two 6-argument
 * setup calls shared verbatim with resource_3c9_c_020048d8.c, a run of
 * id-tagged record setups (0,1,2,3 sharing cached +12/+16 constants
 * across the id-2/id-3 pair), an id-8/9/10/11 position-offset run
 * (note the sign flip: ids 8/9 subtract the delta, ids 10/11 add it),
 * a story-flag set, and a closing call into this overlay's own
 * 0x02004bec candidate (not yet drafted).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02003924 through `pop {r5} /
 * pop {r0} / bx r0` at 0x02003ba8-0x02003bb2, followed by the five-word
 * literal pool 0x02003bb4-0x02003bc7; the next owner's prologue is
 * exactly at 0x02003bc8 (this overlay's own resource_3c9_c_02003bc8.c
 * candidate, not yet drafted). No incoming arguments are read before
 * being overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3c9
 * 3924 3bc8`'s `+2` rule.
 *
 * SHARED IDIOMS cited from resource_3c9_c_020048d8.c's header: the two
 * `Func_08009180` and four `Func_080091c0` six-argument setup calls are
 * near-identical to that file's own (one constant differs: this
 * owner's second `Func_08009180` call ends `...,16,20` where
 * 020048d8.c has `...,16,21`). The additive/subtractive workspace-write
 * idiom (first named in resource_377_c_02000a0c.c) recurs here with
 * the SAME values as resource_3c9_c_020038dc.c: workspace+448 = 512,
 * workspace+456 = 24.
 *
 * Record 0's pointer (`Func_0808a080(0)`) is cached once at the top of
 * the owner and reused for its own field-setup block later without a
 * second lookup call -- the only id in this owner treated that way.
 * The +12 field constant (128<<14) and, separately, the +16 field
 * constant used by ids 2/3 (222<<16) are likewise computed once and
 * reused across sibling record blocks.
 *
 * Uncertainty: none of the eighteen callees are identified beyond call
 * shape; every id/constant is recorded as a passed value.
 */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a228(void);
extern void Func_08009180(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_080091c0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_080000c0(s32 arg0);
extern void Func_08009128(void);
extern void Func_0808a088(s32 id);
extern void Func_0808a100(s32 id, s32 arg1);
extern u8 *Func_0808a080();
extern void Func_080091e0(void *record, s32 arg1);
extern void Func_02005688(void *record);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a158(s32 id, s32 arg1);
extern void Func_080000d0(s32 arg0, s32 arg1);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_0808a010(s32 frames);
extern void Func_02004bec();
extern void Func_080770c8(s32 flag_id);
extern void Func_0808a248(s32 arg0);
extern u8 *Data_03001ebc;

void Func_02003924(void)
{
    u8 *record0 = Func_0808a080(0);
    u8 *record;
    s32 shared12;
    s32 shared16;

    Func_0808a018();
    Func_0808a228();
    record0[0x55] = 0;

    Func_08009180(102, 4, 74, 4, 18, 23);
    Func_08009180(39, 72, 11, 72, 16, 20);

    Func_080091c0(19, 6, 3, 7, 22, 6);
    Func_080091c0(19, 6, 3, 7, 13, 6);
    Func_080091c0(19, 6, 3, 7, 22, 13);
    Func_080091c0(19, 6, 3, 7, 13, 13);

    Func_080000c0(1);
    Func_0808a210(192 << 16, (s32)0xffc00000, 238 << 16, 0);
    Func_080000c0(1);
    Func_08009128();
    Func_080000c0(1);

    Func_0808a088(20);
    Func_0808a088(19);
    Func_0808a100(0, 19);

    Func_080091e0(Func_0808a080(0), 0);
    shared12 = 128 << 14;
    *(s32 *)(record0 + 8) = 173 << 17;
    *(s32 *)(record0 + 16) = 205 << 16;
    *(s32 *)(record0 + 12) = shared12;
    *(u16 *)(record0 + 6) = 192 << 7;
    Func_02005688(record0);
    Func_0808a100(1, 18);

    Func_080091e0(Func_0808a080(1), 0);
    record = Func_0808a080(1);
    *(s32 *)(record + 8) = 178 << 17;
    *(s32 *)(record + 16) = 192 << 16;
    *(u16 *)(record + 6) = 160 << 8;
    *(s32 *)(record + 12) = shared12;
    Func_02005688(record);
    Func_0808a100(2, 18);

    Func_080091e0(Func_0808a080(2), 0);
    record = Func_0808a080(2);
    shared16 = 222 << 16;
    *(s32 *)(record + 8) = 180 << 17;
    *(u16 *)(record + 6) = 128 << 6;
    *(s32 *)(record + 12) = shared12;
    *(s32 *)(record + 16) = shared16;
    Func_02005688(record);
    Func_0808a100(3, 18);

    Func_080091e0(Func_0808a080(3), 0);
    record = Func_0808a080(3);
    *(s32 *)(record + 8) = 167 << 17;
    *(u16 *)(record + 6) = 128 << 8;
    *(s32 *)(record + 12) = shared12;
    *(s32 *)(record + 16) = shared16;
    Func_02005688(record);

    Func_0808a0f0(21, 196 << 16, 220 << 16);
    Func_0808a100(21, 5);
    Func_0808a0f0(6, 188 << 16, 158 << 17);
    Func_0808a100(6, 5);
    Func_080091e0(Func_0808a080(6), 0);

    record = Func_0808a080(8);
    *(s32 *)(record + 8) += (s32)0xfff00000;
    Func_02005688(record);

    record = Func_0808a080(9);
    *(s32 *)(record + 8) += (s32)0xfff00000;
    Func_02005688(record);

    record = Func_0808a080(10);
    *(s32 *)(record + 8) += 128 << 13;
    Func_02005688(record);

    record = Func_0808a080(11);
    *(s32 *)(record + 8) += 128 << 13;
    Func_02005688(record);

    Func_080091f0(128 << 9, 128 << 9, 128 << 9);
    Func_080091e0(Func_0808a080(23), 0);

    record = Func_0808a080(23);
    record[0x55] = 4;
    Func_0808a158(23, 4);

    record = Func_0808a080(23);
    *(s32 *)(record + 12) = 160 << 14;
    Func_080000d0((s32)0x0200da29, 200 << 4);

    {
        u8 *workspace = Data_03001ebc;
        *(s32 *)(workspace + 448) = 512;
        *(s32 *)(workspace + 456) = 24;
    }

    Func_0808a360();
    Func_0808a370();
    Func_0808a010(40);
    Func_02004bec();
    Func_080770c8(0x9a7);
    Func_0808a248(2);
}

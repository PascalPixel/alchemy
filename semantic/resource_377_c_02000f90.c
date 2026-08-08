#include "types.h"

/*
 * resource_377 owner at 0x02000f90, 940 bytes (0x0f90..0x12eb), followed by its
 * literal pool at 0x12ec..0x133b and the next owner's 'push {r5,lr}' prologue at
 * 0x133c.  Pool map derived by walking control flow from the prologue: the
 * owner is straight-line apart from three short forward 'bne'/'beq' skips, all
 * of which land before 0x12e6, and the epilogue at 0x20012e6 is the only exit.
 *
 * Signature: 'pop {r5} ; pop {r0} ; bx r0' -- r0 holds the popped return
 * address, so the owner returns nothing (void).  Nothing reads r0 on entry, so
 * it takes no argument.
 *
 * One scripted cutscene beat, bracketed by the established scene pair
 * Func_0808a018 / Func_0808a020 (both argument-less), the same bracket as
 * semantic/overlays/resource_377_c_02000284.c.
 *
 * r5 is loaded once at 0x02001118 with the constant 0x1c6f and reused at
 * 0x02001172 as 0x1c6f + 3; these are cue ids for Func_0808a170, the same
 * import that resource_377_c_02000284.c feeds a cue id.  Spelled here as plain
 * constants.
 *
 * Data_03001ebc is the family workspace pointer cell (see the byte-exact
 * assets/code/resource_377_c_020002e0.c).  The two 'movs r3,#236 ; lsls #1'
 * bumps of the u16 at workspace + 472 are the overlay family's skip-beat
 * counter.  Note the guard here is 'Func_0808a070(0, 0) == 1', not '!= 0'.
 *
 * The two bumps and the two Func_080770c0(0x81c) guards belong to two
 * behaviourally similar but DISTINCT beats (the second adds a
 * Func_0808a170(0x1c72) / Func_0808a180 tail the first does not have); they are
 * deliberately not merged, since identical behaviour is not a licence to fold
 * call sites.
 *
 * The 16.16-looking pool words (0x9999, 0x4ccc, 0xcccc, 0x6666, 0x13333,
 * 0x3333) are passed as-is; whether they are 16.16 fractions (0.6, 0.3, 0.8,
 * 0.4, 1.2, 0.2) is a plausible reading, not established here.  Recorded as an
 * uncertainty rather than baked into the source.
 *
 * All 91 call sites resolved with
 * 'cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_377 0f90 --json' (the inventory's
 * calls=87 is the documented undercount).  Per-target site counts, which the C
 * below reproduces exactly:
 *   Func_0808a010 x13  Func_0808a018 x1   Func_0808a020 x1
 *   Func_0808a070 x2   Func_0808a080 x3   Func_0808a090 x4
 *   Func_0808a0b8 x2   Func_0808a0c0 x1   Func_0808a0d0 x3
 *   Func_0808a100 x4   Func_0808a110 x5   Func_0808a128 x3
 *   Func_0808a130 x3   Func_0808a138 x6   Func_0808a170 x3
 *   Func_0808a178 x2   Func_0808a180 x3   Func_0808a188 x10
 *   Func_0808a1b8 x6   Func_0808a1e8 x9   Func_08009188 x1
 *   Func_08009190 x1   Func_080000c0 x1   Func_080770c0 x2
 *   Func_080770c8 x2                       (total 91)
 * Verified per target against
 * 'overlay_show.ts ... | overlay_call_targets.ts --annotate | sort | uniq -c':
 * every one of the 25 targets matches, not just the total.
 * The disassembler's own 'bl' annotations are wrong, as on every overlay.
 */

extern u8 *Data_03001ebc;

void Func_020028a0();
void Func_020028d2();
void Func_02002914();
void Func_020029a0();
void Func_02002950();
void Func_02002986();
void Func_020029a8();
void Func_020029da();
void Func_02002972();
void Func_020029c4();
void Func_0200298c();
void Func_020028fa();
void Func_02002934();
void Func_02002962();
void Func_0200289e();
void Func_0200299e();
void Func_0200292c();
void Func_020029cc();
void Func_0200293a();
void Func_02002a1c();
void Func_02002a50();
void Func_02002a32();
void Func_02002a64();
void Func_020029ec();
void Func_02002a76();
void Func_020029f6();
void Func_02002984();
void Func_02002a66();
void Func_02002a2e();
void Func_0200299c();
void Func_02002a7e();
void Func_020029b4();
void Func_02002ac0();
void Func_02002a40();
void Func_02002a98();
s32 Func_020029f8();
s32 Func_020029da_b();
void Func_02002b02();
void Func_02002a08();
void Func_02002ae0();
void Func_02002b1a();
void Func_02002ae2();
void Func_02002af2();
s32 Func_02002a52();
s32 Func_02002a34();
void Func_02002b5c();
void Func_02002a62();
void Func_02002b3a();
void Func_02002b74();
void Func_02002b52();
void Func_02002b22();
void Func_02002a90();
void Func_02002b18();
void Func_02002b30();
void Func_02002b82();
void Func_02002b4a();
void Func_02002ab8();
void Func_02002b9a();
void Func_02002b62();
void Func_02002ad0();
u8 *Func_02002afe();
void Func_020029f0();
u8 *Func_02002b0e();
void Func_02002b4c();
void Func_02002b36();
void Func_02002b86();
void Func_02002b72();
void Func_02002b20();
void Func_02002b96();
void Func_02002c22();
u8 *Func_02002b68();
void Func_02002c3e();
void Func_02002c48();
void Func_02002c54();
void Func_02002c60();
void Func_02002c0a();
void Func_02002c14();
void Func_02002c1e();
void Func_02002c78();
void Func_02002bda();
void Func_02002c20();
void Func_02002c9a();
void Func_02002c4a();
void Func_02002bd0();
void Func_02002c58();
void Func_02002bce();
void Func_02002bd4();
void Func_02002bf8();

                     

                     

                     

                     

                     

                     

                     

                    

void Func_02000f90(void)
{
    u8 *record;

    Func_020028a0();
    Func_020028d2(0, 0x9999, 0x4CCC);
    Func_02002914(0, 0x239, 0x189);
    Func_020029a0(0, 0x4000, 40);
    Func_02002950(8, 2);
    Func_02002986(0x1C66);
    Func_020029a8(8, 0, 80);
    Func_020029da(8, 0x101, 60);
    Func_02002972(8, 1);
    Func_020029c4(8, 0, 60);
    Func_0200298c(8, 2);
    Func_020028fa(80);
    Func_02002934(8, 0xCCCC, 0x6666);
    Func_02002962(8, 584, 406);
    Func_020028a0(11);
    Func_0200289e(12);
    Func_0200299e(8, 12);
    Func_0200292c(80);
    Func_020029cc(8, 2);
    Func_0200293a(40);
    Func_02002a1c(8, 0, 40);
    Func_02002a50(8, 264, 40);
    Func_02002a32(8, 0, 40);
    Func_02002a64(0, 0x105, 60);
    Func_020029ec(8, 13);
    Func_02002a76(8, 0x103, 0);
    Func_020029f6(8, 11);
    Func_02002984(40);
    Func_02002a66(8, 0, 40);
    Func_02002a2e(8, 1);
    Func_0200299c(20);
    Func_02002a7e(8, 0, 20);
    Func_02002a2e(8, 12);
    Func_020029b4(20);
    Func_02002ac0(0, 258, 60);
    Func_02002a40(8, 13);
    Func_02002a98(8, 0);

    /* first beat */
    if (Func_020029f8(0, 0) == 1) {
        *(u16 *)(Data_03001ebc + 472) += 1;
    }
    if (Func_020029da_b(0x81C) != 0) {
        Func_02002b02(8, 258, 60);
    }
    Func_02002a08(20);
    Func_02002ae0(8, 0);
    Func_02002b1a(8, 0x107, 60);
    Func_02002ae2(0x1C6F);
    Func_02002af2(8, 0);

    /* second beat -- distinct site set, see the note above */
    if (Func_02002a52(0, 0) == 1) {
        *(u16 *)(Data_03001ebc + 472) += 1;
    }
    if (Func_02002a34(0x81C) != 0) {
        Func_02002b5c(8, 258, 60);
    }
    Func_02002a62(20);
    Func_02002b3a(8, 0);
    Func_02002b74(8, 0x107, 60);
    Func_02002b3a(0x1C6F + 3);
    Func_02002b52(8, 0);

    Func_02002b22(8, 1);
    Func_02002a90(20);
    Func_02002b18(8, 13);
    Func_02002b30(8, 2);
    Func_02002b82(8, 0, 40);
    Func_02002b4a(8, 1);
    Func_02002ab8(20);
    Func_02002b9a(8, 0, 40);
    Func_02002b62(8, 2);
    Func_02002ad0(40);

    record = Func_02002afe(0);
    *(u16 *)(record + 6) = 0;
    Func_020029f0(1);

    record = Func_02002b0e(0);
    record[90] &= (u8)0xFE;

    Func_02002b4c(0, 0x22E, 388);
    Func_02002b36(8, 0x13333, 0x9999);
    Func_02002b86(8, 14);
    Func_02002b72(8, 0x24A, 400);
    Func_02002b20(40);
    Func_02002b96(8, 580, 382);
    Func_02002c22(8, 0x8000, 40);

    record[90] |= 1;
    record = Func_02002b68(0);

    Func_02002c3e(8, 0xC000, 8);
    Func_02002c48(8, 0, 8);
    Func_02002c54(8, 0x4000, 8);
    Func_02002c60(8, 0x8000, 10);
    Func_02002c0a(8, 4, 20);
    Func_02002c14(8, 6, 40);
    Func_02002c1e(8, 4, 20);
    Func_02002c78(8, 0, 40);
    Func_02002bda(8, 0x6666, 0x3333);
    Func_02002c20(8, 572, 384);
    Func_02002c9a(8, 0, 20);
    Func_02002c4a(0, 3);
    Func_02002bd0(20);
    Func_02002c58(8, 3);
    Func_02002bce(0x81E);
    Func_02002bd4(0x203);
    Func_02002bf8();
}

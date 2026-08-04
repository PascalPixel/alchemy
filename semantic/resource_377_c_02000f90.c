typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

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
 * 'bun tools/overlay_call_targets.ts resource_377 0f90 --json' (the inventory's
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

void Func_0808a018();
void Func_0808a020();
void Func_0808a010();
s32 Func_0808a070();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0c0();
void Func_0808a0d0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a130();
void Func_0808a138();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a188();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_08009188();
void Func_08009190();
void Func_080000c0();
s32 Func_080770c0();
void Func_080770c8();

void Func_02000f90(void)
{
    u8 *record;

    Func_0808a018();
    Func_0808a090(0, 0x9999, 0x4CCC);
    Func_0808a0d0(0, 0x239, 0x189);
    Func_0808a1b8(0, 0x4000, 40);
    Func_0808a130(8, 2);
    Func_0808a170(0x1C66);
    Func_0808a188(8, 0, 80);
    Func_0808a1e8(8, 0x101, 60);
    Func_0808a130(8, 1);
    Func_0808a188(8, 0, 60);
    Func_0808a138(8, 2);
    Func_0808a010(80);
    Func_0808a090(8, 0xCCCC, 0x6666);
    Func_0808a0b8(8, 584, 406);
    Func_08009190(11);
    Func_08009188(12);
    Func_0808a100(8, 12);
    Func_0808a010(80);
    Func_0808a138(8, 2);
    Func_0808a010(40);
    Func_0808a188(8, 0, 40);
    Func_0808a1e8(8, 264, 40);
    Func_0808a188(8, 0, 40);
    Func_0808a1e8(0, 0x105, 60);
    Func_0808a110(8, 13);
    Func_0808a1e8(8, 0x103, 0);
    Func_0808a100(8, 11);
    Func_0808a010(40);
    Func_0808a188(8, 0, 40);
    Func_0808a138(8, 1);
    Func_0808a010(20);
    Func_0808a188(8, 0, 20);
    Func_0808a110(8, 12);
    Func_0808a010(20);
    Func_0808a1e8(0, 258, 60);
    Func_0808a100(8, 13);
    Func_0808a178(8, 0);

    /* first beat */
    if (Func_0808a070(0, 0) == 1) {
        *(u16 *)(Data_03001ebc + 472) += 1;
    }
    if (Func_080770c0(0x81C) != 0) {
        Func_0808a1e8(8, 258, 60);
    }
    Func_0808a010(20);
    Func_0808a180(8, 0);
    Func_0808a1e8(8, 0x107, 60);
    Func_0808a170(0x1C6F);
    Func_0808a178(8, 0);

    /* second beat -- distinct site set, see the note above */
    if (Func_0808a070(0, 0) == 1) {
        *(u16 *)(Data_03001ebc + 472) += 1;
    }
    if (Func_080770c0(0x81C) != 0) {
        Func_0808a1e8(8, 258, 60);
    }
    Func_0808a010(20);
    Func_0808a180(8, 0);
    Func_0808a1e8(8, 0x107, 60);
    Func_0808a170(0x1C6F + 3);
    Func_0808a180(8, 0);

    Func_0808a138(8, 1);
    Func_0808a010(20);
    Func_0808a110(8, 13);
    Func_0808a130(8, 2);
    Func_0808a188(8, 0, 40);
    Func_0808a138(8, 1);
    Func_0808a010(20);
    Func_0808a188(8, 0, 40);
    Func_0808a138(8, 2);
    Func_0808a010(40);

    record = Func_0808a080(0);
    *(u16 *)(record + 6) = 0;
    Func_080000c0(1);

    record = Func_0808a080(0);
    record[90] &= (u8)0xFE;

    Func_0808a0b8(0, 0x22E, 388);
    Func_0808a090(8, 0x13333, 0x9999);
    Func_0808a100(8, 14);
    Func_0808a0c0(8, 0x24A, 400);
    Func_0808a010(40);
    Func_0808a0d0(8, 580, 382);
    Func_0808a1b8(8, 0x8000, 40);

    record = Func_0808a080(0);
    record[90] |= 1;

    Func_0808a1b8(8, 0xC000, 8);
    Func_0808a1b8(8, 0, 8);
    Func_0808a1b8(8, 0x4000, 8);
    Func_0808a1b8(8, 0x8000, 10);
    Func_0808a128(8, 4, 20);
    Func_0808a128(8, 6, 40);
    Func_0808a128(8, 4, 20);
    Func_0808a188(8, 0, 40);
    Func_0808a090(8, 0x6666, 0x3333);
    Func_0808a0d0(8, 572, 384);
    Func_0808a188(8, 0, 20);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a110(8, 3);
    Func_080770c8(0x81E);
    Func_080770c8(0x203);
    Func_0808a020();
}

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

/*
 * Resource 37b cutscene at 0x02000150.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02000150 through the matching
 * interworking return `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x0200060a.
 * The popped register is r0, so it holds the return address: the owner is
 * `void`.  There is no stack frame beyond the saved registers.
 *
 * TWO literal pools sit inside the span, and both matter:
 *  - 0x02000552-0x02000597 (a two-byte alignment `nop` at 0x02000552 then 17
 *    words).  The `b.n 0x02000598` at 0x02000550 branches over it MID-SEQUENCE,
 *    so register state must be carried ACROSS it: r7 still holds the pool word
 *    0x03001ebc loaded at 0x02000156 and is dereferenced at 0x020005f4, and r6
 *    still holds 16 — assigned at 0x020002c0, more than 800 bytes earlier — and
 *    is the value stored at 0x02000604.  Neither is reloaded after the pool.
 *  - 0x02000610-0x02000613, after the `bx r0`.  The span (1,220 bytes) ends at
 *    0x02000614, exactly the next prologue, so both pools are inside it.
 * None of these words is modelled as an instruction.
 *
 * Note also that r6 is REASSIGNED mid-owner: it is 448 from 0x0200015e (used
 * as the store displacement for the two workspace writes at 0x02000164 and
 * 0x020002be) and becomes 16 at 0x020002c0 (used as a stored VALUE from then
 * on).  This is the documented long-lived-alias trap; the register is tracked
 * per use below, not as one variable.
 *
 * Call sites resolved with `tools/overlay_call_targets.ts` (an overlay `bl`
 * stores `target_offset - 2`).  127 sites, 27 distinct targets: 116 import
 * veneers and 11 sites of one real in-overlay routine, 0x020023a4 — a
 * two-argument helper (`push {r5, lr}` at 0x020023a4, `pop {r5} / pop {r0} /
 * bx r0` at 0x020023b4) that lives among the veneers but is genuine code, not
 * an eight-byte `ldr r4 / bx r4 / .word` entry.  The inventory reports
 * calls=117, a lower bound.  Completeness was taken as a multiset over the
 * tool's per-site list against the span's `bl` halfword pairs: 127 = 127, and
 * every site appears exactly once below.  Per-target counts, for the record:
 * 27 Func_0808a010, 11 Func_020023a4, 9 Func_0808a1b8, 8 Func_0808a210,
 * 7 Func_0808a0f0, 7 Func_0808a110, 5 Func_0808a208, 5 Func_0808a370,
 * 4 each Func_0808a090/0f0-class helpers 0808a0d0, 0808a100, 0808a1e8,
 * 0808a218, 3 each Func_080000c0, Func_0808a128, Func_0808a138,
 * Func_0808a360, 2 each Func_08009128, Func_0808a080, Func_0808a0b8,
 * Func_0808a0e8, Func_0808a150, Func_0808a368, and 1 each Func_0808a018,
 * Func_0808a020, Func_0808a170, Func_080770d0.
 *
 * 0x03001ebc is the overlay workspace pointer.  The owner brackets itself with
 * writes to workspace + 448 and workspace + 456 (256/32 on entry, 0x202/16 in
 * the middle, 516/16 on exit).  At 0x020005fa the same register-reuse trap as
 * in 0x0200105c appears: r2 is first the displacement 448, then advanced to
 * 516 as the stored VALUE, then reduced by 60 to 456 as the next displacement.
 *
 * Uncertainties recorded as found:
 *  - Func_0808a018 (0x02000152), Func_0808a218 (four sites) and
 *    Func_08009128 (two sites) are reached with no argument register set by
 *    this owner and are spelled with none.  In the Func_08009128 cases the
 *    registers still hold the previous call's arguments; whether the callee
 *    reads them is unverified, and the dataflow is left as written.
 *  - Func_0808a080(0) returns the scene-state record (same accessor as
 *    0x02001624 / 0x02000c8c); the s16 fields at +10 and +18 read here are the
 *    integer parts of the 16.16 words at +8 and +16, which is the documented
 *    halfword-coordinate-view idiom.
 *  - The many pooled constants passed to Func_0808a208/Func_0808a210 are
 *    fade/blend parameters; their exact meaning is not established.
 */

/* Imports.  Old-style declarations: arity varies per site in this overlay. */
s32 *Func_0808a080();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0d0();
void Func_0808a0e8();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a138();
void Func_0808a150();
void Func_0808a170();
void Func_0808a1b8();
void Func_0808a1e8();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_0808a360();
void Func_0808a368();
void Func_0808a370();
void Func_08009128();
void Func_080000c0();
void Func_080770d0();

/* In-overlay helper at 0x020023a4 (two arguments). */
void Func_020023a4();

void Func_02000150(void)
{
    u8 *workspace;
    s32 *record;

    Func_0808a018();

    workspace = *(u8 **)0x03001ebc;
    *(u32 *)(workspace + 448) = 256;
    *(u32 *)(workspace + 456) = 32;

    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);

    Func_0808a0f0(8, 0x2400000, 0xe80000);
    Func_0808a010(1);
    Func_0808a170(0x101a);
    Func_020023a4(8, 6);
    Func_0808a0f0(8, 0x2400000, 0x1180000);
    Func_0808a208(0xcccc, 0x1999);
    Func_0808a210(0x23e0000, -1, 0xb40000, 1);
    Func_0808a090(8, 0x10000, 0x8000);
    Func_0808a0d0(8, 0x240, 216);
    Func_0808a010(20);
    Func_0808a128(5, 2, 0);
    Func_0808a010(30);
    Func_020023a4(5, 6);
    Func_0808a138(8, 2);
    Func_0808a010(6);
    Func_0808a1b8(8, 0x9000, 0);
    Func_0808a010(10);
    Func_0808a208(0x59999, 0xb333);
    Func_0808a210(0x11f0000, -1, 0xb00000, 1);
    Func_0808a218();
    Func_0808a010(60);
    Func_0808a210(0x23e0000, -1, 0xb40000, 1);
    Func_0808a218();
    Func_0808a010(20);
    Func_0808a110(8, 3);
    Func_0808a010(10);
    Func_0808a1b8(8, 0xc000, 0);
    Func_0808a010(10);
    Func_0808a128(8, 6, 0);
    Func_0808a090(8, 0x30000, 0x20000);
    Func_0808a0d0(8, 0x240, 184);
    Func_0808a010(40);
    Func_020023a4(8, 6);
    Func_0808a1b8(8, 0x8000, 0);
    Func_0808a010(40);

    /* Mid-cutscene workspace change; 16 stays live in r6 to the very end. */
    workspace = *(u8 **)0x03001ebc;
    *(u32 *)(workspace + 448) = 0x202;
    *(u32 *)(workspace + 456) = 16;

    Func_0808a368();
    Func_0808a370();
    Func_0808a210(0x11f0000, -1, 0xb00000, 0);
    Func_08009128();
    Func_080000c0(1);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(40);
    Func_0808a0f0(8, 0x1a80000, 0xc80000);
    Func_080000c0(1);
    Func_020023a4(8, 20);
    Func_0808a368();
    Func_0808a370();
    Func_0808a0f0(8, 0x2400000, 0xb80000);
    Func_0808a210(0x23e0000, -1, 0xb40000, 0);
    Func_08009128();
    Func_080000c0(1);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);
    Func_0808a208(0x13333, 0x2666);
    Func_0808a210(0x23e0000, -1, 0x9d0000, 1);
    Func_0808a218();
    Func_0808a010(20);
    Func_0808a138(1, 2);
    Func_020023a4(1, 20);
    Func_0808a110(5, 3);
    Func_020023a4(5, 6);
    Func_0808a138(8, 2);
    Func_0808a010(40);
    Func_0808a110(8, 4);
    Func_020023a4(8, 80);
    Func_0808a1e8(8, 258, 0);
    Func_0808a010(60);
    Func_0808a1b8(8, 0xc000, 0);
    Func_0808a010(30);
    Func_020023a4(8, 20);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 0);
    Func_0808a1e8(5, 0x101, 0);
    Func_0808a010(60);
    Func_0808a1b8(8, 0xc000, 0);
    Func_0808a010(30);
    Func_0808a110(8, 4);
    Func_020023a4(8, 6);
    Func_0808a150(0, 1, 0);
    Func_0808a010(40);
    Func_0808a150(0, 5, 0);
    Func_0808a010(40);
    Func_0808a110(8, 3);
    Func_020023a4(8, 6);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(5, 0x4000, 0);
    Func_0808a1b8(1, 0x4000, 0);
    Func_0808a010(40);
    Func_0808a100(1, 3);
    Func_0808a100(5, 3);
    Func_0808a110(0, 3);
    Func_0808a010(20);
    Func_0808a110(8, 3);
    Func_020023a4(8, 6);
    Func_0808a208(0x19999, 0x3333);
    Func_0808a210(0x2400000, -1, 0xd70000, 1);
    Func_0808a1b8(8, 0x4000, 0);
    Func_0808a010(10);
    Func_0808a128(8, 6, 0);
    Func_0808a0d0(8, 0x240, 217);
    Func_0808a010(20);
    Func_0808a0d0(8, 0x240, 0x141);
    Func_0808a0f0(8, 0, 0);
    Func_0808a208(0x39999, 0x7333);
    Func_0808a210(0x2400000, -1, 0x880000, 1);
    Func_0808a218();
    Func_0808a010(20);
    Func_0808a090(1, 0x10000, 0x8000);
    Func_0808a090(5, 0x10000, 0x8000);
    Func_0808a100(1, 2);

    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0b8(1, *(s16 *)((u8 *)record + 10),
                      *(s16 *)((u8 *)record + 18));
    }
    Func_0808a0e8(1);
    Func_0808a0f0(1, 0, 0);

    Func_0808a100(5, 2);
    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0b8(5, *(s16 *)((u8 *)record + 10),
                      *(s16 *)((u8 *)record + 18));
    }
    Func_0808a0e8(5);
    Func_0808a0f0(5, 0, 0);

    Func_080770d0(0x12f);

    workspace = *(u8 **)0x03001ebc;
    *(u32 *)(workspace + 448) = 516;
    *(u32 *)(workspace + 456) = 16;

    Func_0808a020();
}

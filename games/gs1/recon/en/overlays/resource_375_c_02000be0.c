#include "types.h"

/*
 * DRAFTED SCENE SCRIPT for FieldScene_RunGatedActorSequence.
 *
 * One progress-gated field cutscene.  The owner returns immediately when
 * progress flag 0x808 is already set; otherwise it sets that flag and plays a
 * fixed beat sequence of 166 calls over scene slots 0, 1, 5, 14 and 15.
 *
 * Structure recovered from the reference: one early return, two identical
 * two-way branches on a scene predicate that select between neighbouring
 * event ids, two null-guarded object lookups near the end, and four
 * read-modify-write updates of the object byte at offset 90.
 *
 * Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds, following the exact sibling
 * games/gs1/src/overlays/scene_primary_script/run_actor_position_sequence.c.
 * Where several pre-relocation words in this owner share one relocated
 * destination, the single unambiguous spelling is reused for every call to
 * that destination.
 *
 * RESIDUAL / INTEGRATOR NOTE.  The pre-relocation word Func_020028a8 is used
 * at two sites in this owner and relocates to two different destinations:
 * 0x02000d22 -> runtime 0x02009b84 (main-image 0x0808a218) and 0x02000d52 ->
 * runtime 0x02009b54 (main-image 0x0808a1b8).  0x02009b84 is reachable through
 * no other pre-relocation word here, so a standalone candidate cannot bind it;
 * `alchemy score` reports "ambiguous overlay call identity".  It is spelled
 * here as Data_02009b85 (Data_ bypasses the call-name resolver and binds the
 * literal runtime address with the Thumb bit set), which compiles and emits
 * the reference's call word, but it is a scoring workaround, not source.
 * The project's own convention for this case is the suffixed pair already used
 * by this overlay's other unit, games/gs1/src/overlays/scene_primary_script/
 * run_actor_position_sequence.c: declare Func_020028a8_a and Func_020028a8_b,
 * spell the 0x02000d22 site _a and the 0x02000d52 site _b, and record both in
 * the unit's `absolute_symbols` as 0x02009b84 and 0x02009b54, kind "thumb".
 * That table is integrator-owned, so the suffixed spellings do not bind from a
 * standalone candidate and are not used here.  Every other call symbol in this
 * file auto-binds from the reference and needs no explicit declaration.
 *
 * RESIDUAL / ALLOCATION.  Two halfwords still differ, both in the second of
 * the two `|= 1` updates at reference 0x020011b2: the reference spells that
 * store `orrs r5, r3; strb r5, [r2]`, reusing the register that held the
 * constant, while this source emits `orrs r3, r5; strb r3, [r2]`.  The first
 * `|= 1` at 0x020011a2, and both `&= 0xfe` updates at 0x0200115e and
 * 0x0200116e, already match byte for byte.  This is a commutative-operand and
 * register-allocation choice with no source evidence behind it; the triage
 * router classifies it as allocation-uncovered and warns against respelling
 * the source to probe register roles.  It must also stay open: these are the
 * only two differing halfwords, so closing them would make this draft
 * byte-exact, which the retained-corpus gate rejects for a draft.
 *
 * MODELLING ARTIFACTS carried over from the exact sibling, not recovered
 * source: the Call/Value function-pointer wrappers below, and the event ids
 * and table pointer modelled as `(s32)` of an extern array symbol.  Both exist
 * to express what the reference does with constants and register lifetimes in
 * the idiom this overlay's adopted C already uses; neither asserts that the
 * original source spelled them that way.
 *
 * Uncertain and deliberately left neutral: the project has no name for any of
 * the resolved main-image targets, so no role names are invented.  The
 * resolved target and the observed argument count are recorded beside each
 * declaration.  The slot numbers (0, 1, 5, 14, 15) and the event ids held in
 * evt/evt2 are raw values whose meaning is not established.  Only three
 * offsets of the looked-up object record are evidenced by this owner: the
 * signed halfwords at 10 and 18, read as the whole-cell halves of 16.16
 * coordinates, and the flag byte at 90.
 */

#define FieldScene_RunGatedActorSequence Func_02000be0

/* 0x080770c0, one argument, result tested */
s32 Func_0200264a();
/* 0x080770c8, one argument */
void Func_02002668();
/* 0x0808a010, one argument (frame count) */
void Func_020027a6();
/* 0x0808a018, no arguments */
void Func_02002674();
/* 0x0808a020, no arguments */
void Func_02002d0a();
/* 0x0808a070, two arguments, result tested */
s32 Func_0200299e();
/* 0x0808a080, one argument, returns an object record or NULL */
u8 *Func_020026e0();
/* 0x0808a090, three arguments */
void Func_020026bc();
/* 0x0808a0b8, three arguments */
void Func_02002ce0();
/* 0x0808a0c8, three arguments */
void Func_020028fe();
/* 0x0808a0d0, three arguments */
void Func_02002706();
/* 0x0808a0e8, one argument */
void Func_02002cfe();
/* 0x0808a0f0, three arguments */
void Func_0200273e();
/* 0x0808a100, two arguments */
void Func_02002788();
/* 0x0808a110, two arguments */
void Func_0200285a();
/* 0x0808a130, two arguments */
void Func_02002b9c();
/* 0x0808a138, two arguments */
void Func_02002890();
/* 0x0808a148, three arguments */
void Func_02002b4c();
/* 0x0808a150, three arguments */
void Func_02002a90();
/* 0x0808a168, three arguments, third is a table address */
void Func_02002d8e();
/* 0x0808a170, one argument (event id) */
void Func_02002730();
/* 0x0808a178, two arguments, returned value discarded at both call sites */
s32 Func_02002b20();
/* 0x0808a188, three arguments */
void Func_0200288e();
/* 0x0808a1b8, three arguments */
void Func_02002792();
/* 0x0808a1e8, three arguments */
void Func_02002780();
/* 0x0808a208, two arguments */
void Func_02002884();
/* 0x0808a210, four arguments */
void Func_020029fa();
/* 0x0808a218, no arguments; see the integrator note above */
void Data_02009b85();
/* 0x0808a4f0, no arguments */
void Func_02002e46();
/* 0x080f9010, one argument (sound id) */
void Func_020027ca();

/* Event ids the reference keeps live in a register across the sequence. */
extern u8 Data_00000f85[];
extern u8 Data_00000f91[];
/* Overlay table passed to the last two calls. */
extern u8 Data_02009ce0[];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void FieldScene_RunGatedActorSequence(void)
{
    u8 *record;
    s32 x, z;
    s32 evt;
    s32 evt2;
    s32 tbl;

    if (Value1(Func_0200264a, 0x808) != 0) {
        return;
    }

    Func_02002674();
    Func_020027ca(17);
    Call1(Func_02002668, 0x808);

    evt = (s32)Data_00000f85;
    Func_02002730(evt);
    Func_0200288e(14, 0, 10);

    Call3(Func_020026bc, 0, 0x13333, 0x9999);
    Call3(Func_02002780, 0, 0x100, 30);
    Call3(Func_02002706, 0, 0x188, 0x148);
    Call3(Func_02002792, 0, 0x4000, 10);

    record = Func_020026e0(0);
    x = *(s16 *)(record + 10);
    z = *(s16 *)(record + 18);
    Func_0200273e(5, x << 16, z << 16);
    Func_0200273e(1, x << 16, z << 16);

    Call3(Func_020026bc, 5, 0x13333, 0x9999);
    Call3(Func_020026bc, 1, 0x13333, 0x9999);
    Call3(Func_020028fe, 5, 0x178, 0x148);
    Call3(Func_02002706, 1, 0x198, 0x148);
    Func_02002788(0, 0);
    Func_02002788(5, 0);
    Func_02002788(1, 0);
    Call3(Func_02002792, 1, 0x8000, 0);
    Func_02002792(5, 0, 20);
    Call3(Func_02002780, 0, 0x101, 0);
    Call3(Func_02002780, 1, 0x101, 0);
    Call3(Func_02002780, 5, 0x101, 30);
    Call3(Func_02002792, 0, 0x4000, 0);
    Call3(Func_02002792, 1, 0x4000, 0);
    Call3(Func_02002792, 5, 0x4000, 0);
    Call2(Func_02002884, 0x60000, 0xc000);
    Call4(Func_020029fa, 0xd70000, -1, 0x1590000, 1);
    Data_02009b85();
    Func_020027a6(20);
    Func_020027ca(61);

    Func_0200285a(14, 4);
    Func_02002788(14, 4);
    Func_0200288e(14, 0, 20);
    Func_02002792(15, 0, 10);
    Func_0200288e(15, 0, 10);
    Func_0200285a(14, 3);
    Func_0200288e(14, 0, 10);
    Call3(Func_02002792, 15, 0x4000, 60);
    Func_02002890(15, 1);
    Func_0200288e(15, 0, 20);
    Func_0200285a(14, 3);
    Func_0200288e(14, 0, 10);
    Func_0200285a(15, 4);
    Func_0200288e(15, 0, 6);
    Call3(Func_02002780, 14, 0x100, 0);
    Func_02002890(14, 2);
    Func_020027a6(20);
    Func_02002792(15, 0, 10);
    Call3(Func_02002780, 15, 0x101, 40);
    Func_02002792(14, 0, 60);
    Call3(Func_02002792, 14, 0x8000, 40);
    Func_02002792(14, 0, 40);
    Call3(Func_02002706, 14, 232, 0x168);
    Func_02002792(14, 0, 10);
    Func_0200285a(15, 3);
    Func_020027a6(10);
    Call3(Func_020028fe, 14, 0x188, 0x168);
    Call3(Func_02002706, 15, 216, 0x168);
    Call3(Func_020028fe, 15, 0x178, 0x168);
    Call3(Func_02002792, 0, 0x4000, 0);
    Call3(Func_02002792, 1, 0x4000, 0);
    Call3(Func_02002792, 5, 0x4000, 0);
    Call4(Func_020029fa, 0x1890000, -1, 0x1530000, 1);
    Call3(Func_020028fe, 14, 0x188, 0x168);
    Call3(Func_02002706, 15, 0x178, 0x168);
    Func_02002788(14, 0);
    Func_02002788(15, 0);
    Call3(Func_02002792, 14, 0xd000, 0);
    Call3(Func_02002792, 15, 0xd000, 30);
    Func_02002890(14, 2);
    Func_0200288e(14, 0, 10);
    Call3(Func_02002780, 1, 258, 60);
    Func_02002890(1, 1);
    Func_0200288e(1, 0, 10);
    Func_0200285a(15, 4);
    Value2(Func_02002b20, 0x100f, 0);

    if (Value2(Func_0200299e, 0, 0) == 0) {
        Func_02002730(evt + 10);
    } else {
        Func_02002730(evt + 11);
    }

    Call3(Func_0200288e, 0x100f, 0, 10);
    Func_02002890(1, 2);

    evt2 = (s32)Data_00000f91;
    Func_02002730(evt2);
    Func_0200288e(1, 0, 20);
    Func_02002a90(14, 15, 40);
    Call3(Func_02002792, 14, 0xd000, 0);
    Call3(Func_02002792, 15, 0xd000, 20);
    Func_0200288e(14, 0, 60);
    Func_02002890(15, 1);
    Func_020027a6(10);
    Func_0200285a(15, 3);
    Call3(Func_0200288e, 0x100f, 0, 10);
    Func_02002890(5, 2);
    Func_0200285a(5, 3);
    Call3(Func_0200288e, 0x1005, 0, 20);
    Func_02002890(14, 2);
    Call3(Func_02002792, 14, 0xa000, 20);
    Value2(Func_02002b20, 14, 0);

    if (Value2(Func_0200299e, 0, 0) == 0) {
        Func_02002730(evt2 + 5);
    } else {
        Func_02002730(evt2 + 6);
    }

    Func_02002792(5, 0, 0);
    Call3(Func_02002792, 1, 0x8000, 20);
    Func_02002890(14, 2);
    Func_0200288e(14, 0, 10);
    Func_02002b4c(14, 1, 30);
    Func_02002b4c(14, 5, 30);
    Call3(Func_02002780, 14, 0x105, 80);
    Func_0200285a(14, 4);
    Call1(Func_02002730, 0xf98);
    Func_0200288e(14, 0, 6);
    Call3(Func_02002780, 0, 0x100, 0);
    Call3(Func_02002780, 1, 0x100, 0);
    Call3(Func_02002780, 5, 0x100, 0);
    Func_02002b9c(1, 1);
    Func_02002b9c(5, 1);
    Func_02002890(0, 1);
    Func_020027a6(40);
    Func_02002890(5, 2);
    Call3(Func_02002792, 1, 0x4000, 0);
    Call3(Func_02002792, 5, 0x4000, 10);
    Call3(Func_0200288e, 0x1005, 0, 10);
    Func_02002890(15, 2);
    Func_02002792(15, 0, 10);
    Call3(Func_0200288e, 0x100f, 0, 10);
    Call3(Func_02002792, 14, 0x8000, 20);
    Func_0200285a(14, 4);
    Func_0200288e(14, 0, 10);
    Func_02002890(15, 2);
    Func_020027a6(10);
    Call3(Func_02002792, 14, 0xb000, 0);
    Call3(Func_02002792, 15, 0xd000, 20);
    Call3(Func_020026bc, 15, 0x8000, 0x4000);
    Call3(Func_020026bc, 14, 0x8000, 0x4000);

    record = Func_020026e0(14);
    *(record + 90) &= 0xfe;
    record = Func_020026e0(15);
    *(record + 90) &= 0xfe;

    Call3(Func_020028fe, 14, 0x188, 0x178);
    Call3(Func_02002706, 15, 0x178, 0x178);
    Func_020027a6(6);

    record = Func_020026e0(14);
    *(record + 90) |= 1;
    record = Func_020026e0(15);
    *(record + 90) |= 1;

    Func_02002788(14, 0);
    Func_02002788(15, 0);
    Func_020027a6(20);
    Func_02002890(1, 2);
    Func_0200288e(1, 0, 10);
    Func_02002792(0, 1, 20);
    Func_02002788(0, 3);
    Func_0200285a(1, 3);
    Func_020027ca(17);

    Func_02002788(1, 2);
    record = Func_020026e0(0);
    if (record != 0) {
        Func_02002ce0(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02002cfe(1);
    Func_0200273e(1, 0, 0);

    Func_02002788(5, 2);
    record = Func_020026e0(0);
    if (record != 0) {
        Func_02002ce0(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02002cfe(5);
    Func_0200273e(5, 0, 0);

    tbl = (s32)Data_02009ce0;
    Call3(Func_02002d8e, 14, 0x10000, tbl);
    Call3(Func_02002d8e, 15, 0x10000, tbl);
    Func_02002e46();
    Func_02002d0a();
}

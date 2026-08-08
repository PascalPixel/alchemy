#include "types.h"

/*
 * resource_380 owner at 0x020006f4, 880 bytes: the overlay's other
 * scene-setup cinematic, and the last unowned owner in resource_380.
 *
 * PUBLISHED, NOT CALLED. No `bl` in the image reaches it; its Thumb
 * pointer is written into a script record. Found by
 * `cargo run --release --manifest-path tools/overlay-published/Cargo.toml -- resource_380`.
 *
 * IT WAS BLOCKED UNTIL THE TOOL COULD MEASURE AN EXACT ROW. The sweep
 * used to report this address UNRULED: it sits immediately behind the
 * exact-C row at 0x02000390, whose extent no data in the tree states,
 * so nothing could tell an address inside that row from the address
 * just after it. `overlayCSpans` now compiles the row -- it is 868
 * bytes and therefore ends at exactly 0x020006f4. This is a genuine
 * undrafted owner starting where that row stops, not an interior
 * address of it. Drafting it before that was established would have
 * risked duplicating an exact owner.
 *
 * SPAN MEASURED. `push {r5,r6,lr}` plus the r8/r9/sl spill and
 * `sub sp,#8` at 0x020006f4, through
 * `add sp,#8 / pop {r3,r5,r6} / mov r8,r3 / mov r9,r5 / mov sl,r6 /
 * pop {r5,r6} / pop {r0} / bx r0` at 0x02000a1a-0x02000a28, a zero
 * alignment halfword at 0x02000a2a, then the fourteen-word literal
 * pool 0x02000a2c-0x02000a63; the next owner's prologue is at
 * 0x02000a64 (an exact-C row). 0x02000a64 - 0x020006f4 = 880.
 *
 * THE POOL CONTAINS THE KNOWN TRAP, and it is worth naming because it
 * is the second sighting. The last pool word, 0x0000e666 at
 * 0x02000a58, disassembles as `b.n 0x02000728` -- a backward branch
 * landing inside this function's own loop body. The identical value
 * played the same trick in resource_3af's 0x02002618. Anyone measuring
 * this span by following branches would conclude the function
 * re-enters itself after the epilogue. It is a constant: the same
 * 0xe666 is loaded into r2 at 0x0200092a as an argument.
 *
 * SIBLING OF 0x02002400, diffed rather than assumed. Both open with
 * Func_0808a018 / Func_080f9010(141) and the same six-iteration
 * Func_0808a330 / Func_0808a348 / Func_0808a010 loop, and both close on
 * a workspace+448 write. They then diverge completely, and two
 * differences inside the shared part are real:
 *   - The loop constant is 0x00404a4e here, 0x004039d2 there.
 *   - The counter is a plain `adds r5,#1` here, but `lsls #24 / lsrs
 *     #24` there. That is a byte-typed counter in one and a word-typed
 *     one in the other, so the two are NOT the same source text.
 *
 * Shape: three near-identical presentation blocks, each
 * Func_08009178(table, ...) / Func_080091c0(...) / Func_08009180(...)
 * against the descriptor table 0x0200d088 held in sl, separated by
 * fades; then the workspace write; then three Func_0808a210 /
 * Func_08009128 / Func_080000c0(1) / Func_0808a360 / Func_0808a370
 * teardown groups wrapped around a Func_02004260 spawn and one line of
 * dialogue.
 *
 * The workspace+448 write is the ADDITIVE form and the value is 514:
 * `movs r2,#224 / lsls r2,#1` builds the displacement 448, then
 * `adds r2,#66` turns the same register into 514. Its sibling at
 * 0x02002400 writes 256 to the same slot by the subtractive form.
 * Transcribed as arithmetic, never assumed to be the canonical 513.
 *
 * 0x02000a10 SETS story flag 0x83d, which is how this cinematic marks
 * itself done. The overlay's talk callbacks test 0x83e and 0x83c;
 * whether 0x83d gates anything in this overlay is not established here.
 *
 * ONE COMPILER ARTEFACT WORTH NOT PROPAGATING: at 0x020009cc the
 * dialogue id is produced by `subs r5,#2` against the 0x1076 already in
 * r5 from the earlier Func_08015040 call. That is common-subexpression
 * reuse of a nearby constant, not an expression in the source, so it is
 * written here as the literal 0x1074.
 *
 * All 84 `bl` targets resolved through the overlay's import-veneer
 * table under the +2 rule (cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --). Two are
 * direct intra-image branches (Func_02004260, Func_02004248), visible
 * only in the resolved target.
 *
 * Uncertainty: the roles of Func_0808a208/0808a210/0808a218,
 * Func_08009178/080091c0/08009180 and the teardown group are open; the
 * fixed-point constants are transcribed, not interpreted; 0x0200d088 is
 * a descriptor table by use (an even pool word, so data rather than a
 * published function pointer) and its layout is unread.
 */

extern void Func_080000c0(s32 arg0);
extern void Func_08009128(void);
extern void Func_08009178(s32 table, s32 arg1, s32 arg2);
extern void Func_08009180(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_080091f8(void);
extern void Func_08015040(s32 arg0, s32 arg1);
extern void Func_08015120(s32 arg0, s32 arg1);
extern void Func_080770c8(s32 flagId);
extern void Func_080f9010(s32 arg0);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a128(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a218(void);
extern void Func_0808a330(s32 arg0, s32 arg1);
extern void Func_0808a348(s32 arg0);
extern void Func_0808a360(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern s32 Func_02004260(s32 template_id, s32 x, s32 y, s32 z);
extern void Func_02004248(s32 arg0, s32 arg1);

extern u8 *Data_03001ebc;

void Func_020006f4(void)
{
    s32 table = 0x0200d088;
    s32 step;
    s32 object;

    Func_0808a018();
    Func_080f9010(141);

    do {
        Func_0808a330(0x00404a4e, 1);
        Func_0808a348(8);
        Func_0808a010(8);
        Func_0808a330(0x10000, 1);
        Func_0808a348(8);
        Func_0808a010(8);
        if (step == 1)
            Func_080091f0(0x10000, 0x10000, 0x10000);
        step += 1;
    } while (step != 6);
    step = 0;

    Func_080091f0(0x10000, 0x20000, 0x10000);
    Func_0808a010(30);
    Func_080091f0(0, 0, 0);

    Func_0808a208(0x59999, 0xb333);
    Func_0808a210(0x1d80000, -1, 0x620000, 1);
    Func_0808a218();
    Func_080091f0(0x10000, 0x20000, 0x10000);
    Func_0808a010(20);
    Func_080f9010(144);
    Func_08009178(table, 84, 4);
    Func_080091c0(0, 0, 1, 1, 29, 4);
    Func_08009180(87, 42, 29, 6, 1, 2);
    Func_0808a010(40);

    Func_080091f0(0, 0, 0);
    Func_0808a210(0x1570000, -1, 0x1710000, 1);
    Func_0808a218();
    Func_080091f0(0x10000, 0x20000, 0x10000);
    Func_0808a010(20);
    Func_080f9010(144);
    Func_08009178(table, 76, 21);
    Func_080091c0(0, 0, 1, 1, 21, 21);
    Func_08009180(87, 42, 21, 23, 1, 2);
    Func_0808a010(40);

    Func_080091f0(0, 0, 0);
    Func_0808a208(0x33333, 0x6666);
    Func_0808a210(0x1570000, -1, 0x1f10000, 1);
    Func_0808a218();
    Func_080091f0(0x10000, 0x20000, 0x10000);
    Func_0808a010(20);
    Func_080f9010(144);
    Func_08009178(table, 76, 29);
    Func_080091c0(0, 0, 1, 1, 21, 29);
    Func_08009180(87, 42, 21, 31, 1, 2);
    Func_0808a010(40);

    *(s32 *)(Data_03001ebc + 448) = 514;

    Func_0808a368();
    Func_0808a370();
    Func_0808a210(0x2c80000, -1, 0x980000, 0);
    Func_08009128();
    Func_080000c0(1);
    Func_080091f0(0x20000, 0x10000, 0x10000);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(40);

    Func_080f9010(0x121);
    Func_080091f0(-1, -1, 0xe666);
    Func_080091f8();
    Func_0808a010(20);
    Func_08009180(0, 40, 43, 46, 3, 3);
    Func_0808a010(20);

    object = Func_02004260(221, 0x2c80000, 0x100000, 0x900000);
    Func_0808a010(40);
    Func_08015120(object, 1);
    Func_08015040(0x1076, 1);
    Func_0808a1b8(9, 0xe000, 0);
    Func_0808a1b8(5, 0xe000, 20);

    Func_0808a368();
    Func_0808a370();
    Func_0808a210(0x1ce0000, -1, 0x15e0000, 0);
    Func_08009128();
    Func_080000c0(1);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(40);

    Func_0808a128(9, 4, 30);
    Func_0808a170(0x1074);
    Func_02004248(9, 20);

    Func_0808a368();
    Func_0808a370();
    Func_0808a210(0x2c80000, -1, 0x980000, 0);
    Func_08009128();
    Func_080000c0(1);
    Func_0808a360();
    Func_0808a370();

    Func_080770c8(0x83d);
    Func_0808a020();
}

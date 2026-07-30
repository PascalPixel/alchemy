typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

/*
 * resource_38d owner at 0x02000338, 180 bytes: code 0x02000338-0x020003e7 and
 * the single pool word 0x03001ebc at 0x020003e8.  A control-flow walk from the
 * prologue reaches that word only through `ldr r3, [pc, #172]`; the last
 * instruction before it is `bx r0`, so it is never decoded as an instruction.
 *
 * The overlay's scene-entry beat.  It reads the scene selector - the signed
 * halfword at workspace + 364 - and picks a sound cue from it, suppressing the
 * cue entirely on selector 9 when story flag 0x200 is already set.  When a cue
 * is played it also disables channels 1 and 2 for the duration, re-enabling
 * them after the bracket closes.  Inside the bracket it scales and re-presents
 * entity 0 and starts one of two moves, again chosen on selector 9, then hands
 * the selector itself to Func_0808a248.
 *
 * The workspace pointer is the word at 0x03001ebc (IWRAM); it is loaded once
 * into r6 and kept live across the whole owner, so the three reads of
 * workspace + 364 below share it.  364 is built as 182 << 1 each time and every
 * read is `ldrsh` through a zero index register, i.e. a signed halfword.
 *
 * Call accounting: 15 `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_38d 0338 --json`: Func_08009188
 * x2, Func_08009190 x2, Func_0808a010 x2, and one each of Func_080770c0,
 * Func_080f9010, Func_0808a018, Func_0808a090, Func_0808a100, Func_0808a0e0,
 * Func_0808a0d8, Func_0808a248, Func_0808a020 - 11 distinct targets.  Every
 * site appears below exactly once and the per-target multisets match, which
 * needs the `cue` join to be written once: 0x02000360 is a single site reached
 * from two arms with r0 = 188 and r0 = 158, so spelling it per arm would
 * inject a phantom call.  No intra-overlay call and no `call_via` slot -
 * neither r3 nor r4 ever holds an IWRAM code address before a branch (r3 is
 * only the workspace scratch).
 *
 * The epilogue is `pop {r5, r6} / pop {r0} / bx r0`, so r0 holds the popped
 * return address and the owner is void.  The prologue is `push {r5, r6, lr}`
 * and no incoming register is read before it is overwritten, so the owner
 * takes no arguments.
 *
 * Immediates are written as the assembly builds them: 128 << 2 = 512,
 * 128 << 8 = 0x8000, 128 << 7 = 0x4000, and -16 by `movs r2, #16 / negs r2, r2`.
 *
 * UNCERTAINTIES:
 *  - `played` (r5) is set only on the paths that reach the cue, and it gates
 *    the Func_08009188 pair; the flag's own meaning is read from that gating,
 *    nothing more.
 *  - Func_08009188 / Func_08009190 are called with 1 and 2 in the same order
 *    on both sides of the bracket, which reads as a disable/enable pair; the
 *    imports' interfaces are not established.
 *  - Func_0808a0e0 and Func_0808a0d8 take the same shape (0, k, -16) with
 *    k = 0 and k = 3; whether they are two imports or one selected by scene is
 *    not decidable here - the two veneers are distinct.
 */

/* Imports. Old-style declarations: one name can take different argument
 * counts at different sites in this overlay. */
void Func_08009188();           /* channel control, entering the scene */
void Func_08009190();           /* channel control, leaving the scene */
void Func_0808a010();           /* wait n frames */
void Func_0808a018();           /* scripted-scene bracket: open */
void Func_0808a020();           /* scripted-scene bracket: close */
void Func_0808a090();           /* 16.16 scale pair */
void Func_0808a0d8();           /* start a move; interface not established */
void Func_0808a0e0();           /* start a move; interface not established */
void Func_0808a100();           /* per-record scene request */
void Func_0808a248();           /* scene request keyed by the selector */
s32 Func_080770c0();            /* story-flag test; used in a condition */
void Func_080f9010();           /* play a sound cue */

void Func_02000338(void)
{
    u8 *workspace;
    s32 played;
    s32 cue;

    workspace = *(u8 **)0x03001ebc;
    played = 0;

    if (*(s16 *)(workspace + 364) != 9) {
        cue = 158;
        goto play;
    }
    if (Func_080770c0(512) == 0) {
        cue = 188;
play:
        /* One call site, reached from both arms above. */
        Func_080f9010(cue);
        played = 1;
    }

    if (played != 0) {
        Func_08009188(1);
        Func_08009188(2);
    }

    Func_0808a018();

    Func_0808a010(10);
    Func_0808a090(0, 0x8000, 0x4000);
    Func_0808a100(0, 2);

    if (*(s16 *)(workspace + 364) == 9) {
        Func_0808a0e0(0, 0, -16);
    } else {
        Func_0808a0d8(0, 3, -16);
    }

    Func_0808a010(16);
    Func_0808a248(*(s16 *)(workspace + 364));

    Func_0808a020();

    Func_08009190(1);
    Func_08009190(2);
}

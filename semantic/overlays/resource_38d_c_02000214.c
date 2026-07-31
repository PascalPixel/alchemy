typedef unsigned short u16;
typedef unsigned char u8;
typedef signed int s32;

/*
 * resource_38d owner at 0x02000214, 120 bytes: code 0x02000214-0x0200026d
 * (0x0200026e is the two-byte alignment `movs r0, r0`) and the seven-word
 * literal pool at 0x02000270-0x0200028b.  A control-flow walk from the
 * prologue reaches the pool only through `ldr rN, [pc, #imm]`; every forward
 * branch lands at or before 0x0200025e and the last instruction before the
 * pool is `bx r0`, so no pool word is decoded as an instruction.
 *
 * One dialogue beat on slot 17, the sibling of the owner at 0x020002cc: three
 * story flags choose the line, all arms join at 0x0200025e, and the slot's
 * action is then waited on once.  The difference is the third arm, which
 * bumps a scene counter instead of showing a second line - the "empty else
 * that only increments something" skip-beat idiom, and it is the same u16 at
 * workspace + 472 that the same idiom uses elsewhere in the project.
 *
 * The workspace pointer is the word at 0x03001ebc (IWRAM), loaded and
 * dereferenced here exactly as the rest of this overlay family loads it; 472
 * is built as 236 << 1 and the access is an unsigned halfword load/store.
 *
 * Call accounting: 9 `bl` sites, all resolved with
 * `bun tools/overlay_call_targets.ts resource_38d 0214 --json` (five distinct
 * import veneers: Func_080770c0 x3, Func_0808a170 x3, and one each of
 * Func_0808a018, Func_0808a180, Func_0808a020).  Every site appears below
 * exactly once and the per-target multisets match.  No intra-overlay call and
 * no `call_via` slot: r3 is written here only as an offset/counter register
 * for the halfword bump, never loaded with an IWRAM code address before a
 * branch.
 *
 * The epilogue is `pop {r0} / bx r0`, so r0 holds the popped return address
 * and the owner is void.  The prologue is `push {lr}` and the first branch is
 * taken with no argument register written, so the owner takes no arguments.
 *
 * UNCERTAINTY: the counter's meaning is not established - only that it is the
 * same field the sibling scenes bump on the arm where no line is shown.
 */

/* Imports. Old-style declarations: one name can take different argument
 * counts at different sites in this overlay. */
void Func_0808a018();           /* scripted-scene bracket: open */
void Func_0808a020();           /* scripted-scene bracket: close */
void Func_0808a170();           /* show a dialogue line by id */
void Func_0808a180();           /* wait for the slot's action to finish */
s32 Func_080770c0();            /* story-flag test; used in a condition */

void Func_02000214(void)
{
    u8 *workspace;

    Func_0808a018();

    if (Func_080770c0(0x202) != 0) {
        Func_0808a170(0x174b);
    } else if (Func_080770c0(0x84e) != 0) {
        Func_0808a170(0x176e);
    } else {
        Func_0808a170(0x1432);
        if (Func_080770c0(0x84d) != 0) {
            workspace = *(u8 **)0x03001ebc;
            *(u16 *)(workspace + 472) = (u16)(*(u16 *)(workspace + 472) + 1);
        }
    }

    Func_0808a180(17, 0);
    Func_0808a020();
}

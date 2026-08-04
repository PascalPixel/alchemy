typedef int s32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * resource_3b9 owner at 0x0200039c, 204 bytes: the fourth published
 * talk handler in the bank at 0x020002a0-0x020006bc, for actor 16, and
 * the first in the bank with real state.
 *
 * PUBLISHED, NOT CALLED. Found by
 * `bun tools/overlay_published.ts resource_3b9`. The bank is described
 * in resource_3b9_c_020002a0.c.
 *
 * IT IS A ONCE-ONLY GRANT, and that is what makes it the longest member
 * so far. Under the bank's shared flag 0x962 it tests a SECOND flag,
 * 0x3c0: if 0x3c0 is already set the handler just says the repeat line
 * 0x225e; if not, it plays the first-time sequence and SETS 0x3c0 at
 * the end with Func_080770c8. That is the classic once-only guard, and
 * the branch senses were read individually -- `beq` past the first
 * test, `beq` into the not-yet-granted arm on the second.
 *
 * TWO SKIP-BEAT GATES, and they behave differently from each other.
 * Both are `if (Func_0808a070(0, 0) == 0)`. The first gate guards the
 * whole first-time body; the second guards only a second increment of
 * the counter. So the workspace+472 u16 is bumped once or twice
 * depending on how far the player lets the beat run, not exactly once.
 * That is a genuine departure from the converging shape recorded for
 * the resource_3c9 copies, where the bump is duplicated so it happens
 * once either way. Do not assume the converging form.
 *
 * The counter is the known skip-beat slot: `movs r2,#236 / lsls r2,#1`
 * builds the displacement 472 against the shared workspace pointer at
 * 0x03001ebc, and the value read, incremented and stored back is a u16.
 * Transcribed as arithmetic.
 *
 * The pool word 0x03001ebc is held in r5 across the whole body, which
 * is why this handler needs `push {r5, lr}` where its neighbours need
 * only `push {lr}`.
 *
 * 0x3c0 is built as `movs r0,#240 / lsls r0,#2` at both the test and
 * the set, so it is 960 and not a pool constant. Note it is adjacent to
 * the bank's shared flag 0x962 without being it.
 *
 * The second argument to Func_0808a1e8 is `movs r1,#128 / lsls r1,#1`,
 * so 256 -- the same build-a-small-constant shape whose sibling in
 * 0x020002e4 yields 258. Two adjacent handlers, two different values
 * from the same instruction pair; transcribe, never pattern-match.
 *
 * Complete owner: `push {r5, lr}` at 0x0200039c through
 * `pop {r5} / pop {r0} / bx r0` at 0x0200044c-0x02000450, a zero
 * alignment halfword at 0x02000452, then the five-word literal pool
 * 0x02000454-0x02000467 (0x962, 0x225e, 0x225a, 0x03001ebc, 0x205e);
 * the next owner's prologue is at 0x02000468.
 *
 * All `bl` targets resolved through the import-veneer table under the
 * +2 rule (tools/overlay_call_targets.ts).
 *
 * Uncertainty: what Func_0808a070(0, 0) actually reports is not
 * established beyond its use as the skip gate; the roles of
 * Func_0808a178 and Func_0808a1e8 are open.
 */

extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern void Func_0808a170(s32 dialogueId);
extern void Func_0808a178(s32 id, s32 arg1);
extern void Func_0808a180(s32 id, s32 arg1);
extern void Func_0808a190(s32 id, s32 arg1);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);

extern u8 *Data_03001ebc;

void Func_0200039c(void)
{
    u8 *workspace;

    Func_0808a018();
    if (Func_080770c0(0x962)) {
        if (Func_080770c0(960)) {
            Func_0808a170(0x225e);
        } else {
            Func_0808a170(0x225a);
            Func_0808a178(16, 0);
            if (Func_0808a070(0, 0) == 0) {
                workspace = Data_03001ebc;
                *(u16 *)(workspace + 472) += 1;
                Func_0808a1e8(16, 256, 40);
                Func_0808a178(16, 0);
                if (Func_0808a070(0, 0) == 0)
                    *(u16 *)(workspace + 472) += 1;
                Func_0808a010(40);
                Func_0808a180(16, 0);
                Func_080770c8(960);
            } else {
                Func_0808a180(16, 0);
            }
        }
    } else {
        Func_0808a170(0x205e);
        Func_0808a190(16, 0);
    }
    Func_0808a020();
}

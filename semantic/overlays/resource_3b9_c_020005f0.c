typedef int s32;

/*
 * resource_3b9 owner at 0x020005f0, 88 bytes: the ninth published
 * handler in the bank at 0x020002a0-0x020006bc, for actor 17.
 *
 * PUBLISHED, NOT CALLED. Found by
 * `bun tools/overlay_published.ts resource_3b9`. The bank is described
 * in resource_3b9_c_020002a0.c. Straight-line, no flag test.
 *
 * ONE CALL DOES NOT TAKE ACTOR 17. Every other call in this handler
 * passes 17, but the first Func_0808a110 at 0x02000610 is preceded by
 * `movs r0,#0`, so it is Func_0808a110(0, 3) -- id 0 -- and the very
 * next call is Func_0808a110(17, 3). Two consecutive calls to the same
 * callee with different ids is exactly the shape a reader smooths over,
 * so it is called out here rather than left to be noticed.
 *
 * Complete owner: `push {lr}` at 0x020005f0 through `pop {r0} / bx r0`
 * at 0x0200063e-0x02000640, a zero alignment halfword at 0x02000642,
 * then the one-word literal pool at 0x02000644 (line id 0x211f); the
 * next owner's prologue is at 0x02000648.
 *
 * All `bl` targets resolved through the import-veneer table under the
 * +2 rule; the Func_02001c48 and Func_02001c5c calls are direct
 * intra-image branches.
 *
 * Uncertainty: whether id 0 names an actor at all, or something else
 * indexed in the same space, is not established.
 */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a150(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a170(s32 dialogueId);
extern void Func_02001c48(s32 arg0);
extern void Func_02001c5c(s32 arg0, s32 arg1);

void Func_020005f0(void)
{
    Func_0808a018();
    Func_0808a150(17, 0, 20);
    Func_0808a170(0x211f);
    Func_02001c48(17);
    Func_0808a110(0, 3);
    Func_0808a110(17, 3);
    Func_02001c48(17);
    Func_0808a138(17, 1);
    Func_02001c48(17);
    Func_02001c5c(17, 0x5000);
    Func_0808a020();
}

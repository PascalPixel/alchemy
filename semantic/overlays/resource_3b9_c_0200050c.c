typedef int s32;

/*
 * resource_3b9 owner at 0x0200050c, 80 bytes: the seventh published
 * handler in the bank at 0x020002a0-0x020006bc, for actor 15.
 *
 * PUBLISHED, NOT CALLED. Found by
 * `bun tools/overlay_published.ts resource_3b9`. The bank is described
 * in resource_3b9_c_020002a0.c. Straight-line, no flag test.
 *
 * It leans on this overlay's own trampolines: three Func_02001c48(15)
 * calls interleaved with the imported helpers, and a closing
 * Func_02001c5c(15, 0x5000). The 0x5000 is built as
 * `movs r1,#160 / lsls r1,#7`, and the same 0x5000 closes three more
 * members of this bank (0x020005f0, 0x02000648, and the long arm of
 * 0x0200055c). That recurrence is real but it is a shared closing
 * argument, not evidence about what the value means.
 *
 * Complete owner: `push {lr}` at 0x0200050c through `pop {r0} / bx r0`
 * at 0x02000552-0x02000554, a zero alignment halfword at 0x02000556,
 * then the one-word literal pool at 0x02000558 (line id 0x2118); the
 * next owner's prologue is at 0x0200055c.
 *
 * All `bl` targets resolved through the import-veneer table under the
 * +2 rule; the four calls to this overlay's own trampolines are direct
 * intra-image branches.
 *
 * Uncertainty: the roles of Func_0808a110 and Func_0808a100 are open.
 */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a150(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a170(s32 dialogueId);
extern void Func_02001c48(s32 arg0);
extern void Func_02001c5c(s32 arg0, s32 arg1);

void Func_0200050c(void)
{
    Func_0808a018();
    Func_0808a170(0x2118);
    Func_02001c48(15);
    Func_0808a150(15, 0, 20);
    Func_02001c48(15);
    Func_0808a110(15, 3);
    Func_0808a100(15, 0);
    Func_02001c48(15);
    Func_02001c5c(15, 0x5000);
    Func_0808a020();
}

typedef int s32;

/*
 * resource_380 owner at 0x02000d5c, 560 bytes: a two-track scene
 * configuration -- every call in the first half is issued once for
 * track 0 and once for track 1 with staggered values (offsets 0 vs
 * 40/60/80, ids 0x1df vs 0x1eb, rates 0x109/0x1c5 vs 282/0x1d5), one
 * conditional in the middle forwards a context's +8/+16 pair when
 * Func_0808a080(0) returns an object, and the tail places the same
 * four actors (5/9/11/10) at the same fixed-point coordinates as the
 * 0x0200227c owner before the closing fade pair.
 *
 * CORRECTION to the earlier park note: the "b.n 0x2000c50 shared tail
 * into 0x02000a98" recorded in work/claude/notes/resource_380_notes.md
 * was a misread -- 0x2000f80 is this owner's own pool word 0x0000e666,
 * not an instruction.  The row is self-contained.
 *
 * Complete owner: `push {lr}` at 0x02000d5c through `pop {r0} / bx r0`
 * at 0x02000f48, then the sixteen-word literal pool
 * 0x02000f4c-0x02000f8b; next owner's prologue at 0x02000f8c.
 *
 * CORRECTION (name sweep): this file's callee names came from a naive
 * pc-relative decode and were wrong; they are resolved here through the
 * overlay's import-veneer table under the +2 rule
 * (tools/overlay_call_targets.ts) to their main-ROM identities.  The
 * earlier note that one callee took "four arguments and later three"
 * was an artifact of that decode -- the phantom name covered two
 * different functions (Func_0808a210 and Func_0808a1b8).  Nothing here
 * is declared old-style.
 *
 * Uncertainty: callee roles beyond call shape remain open.
 */

extern void Func_0808a010(s32 arg0);
extern s32 Func_0808a080(s32 arg0);
extern void Func_0808a090(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a0c8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 arg0, s32 arg1);
extern void Func_0808a110(s32 arg0, s32 arg1);
extern void Func_0808a128(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a130(s32 arg0, s32 arg1);
extern void Func_0808a138(s32 arg0, s32 arg1);
extern void Func_0808a1b8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a218(void);
extern void Func_080f9010(s32 arg0);

void Func_02000d5c(void)
{
    s32 context;

    Func_080f9010(17);
    Func_0808a090(0, 0x8000, 0x4000);
    Func_0808a0d0(0, 231, 490);
    Func_0808a1b8(0, 0xc000, 30);
    Func_0808a110(0, 3);
    Func_0808a010(180);
    Func_0808a138(0, 2);
    Func_0808a010(80);
    Func_0808a1e8(0, 0x101, 60);
    Func_0808a090(0, 0x8000, 0x4000);
    Func_0808a0d0(0, 246, 0x1df);
    Func_0808a1b8(0, 0xe000, 10);
    context = Func_0808a080(0);
    if (context != 0)
        Func_0808a0f0(1,
            *(s32 *)(context + 8), *(s32 *)(context + 16));
    Func_0808a090(1, 0x10000, 0x8000);
    Func_0808a0d0(1, 0x101, 0x1eb);
    Func_0808a1b8(0, 0x2000, 0);
    Func_0808a1b8(1, 0xa000, 40);
    Func_0808a130(0, 2);
    Func_0808a138(1, 2);
    Func_0808a010(20);
    Func_0808a1e8(0, 0x101, 0);
    Func_0808a1e8(1, 0x101, 80);
    Func_0808a100(0, 3);
    Func_0808a110(1, 4);
    Func_0808a090(0, 0x13333, 0x9999);
    Func_0808a090(1, 0x13333, 0x9999);
    Func_0808a0c8(0, 0x109, 0x1c5);
    Func_0808a0d0(1, 282, 0x1d5);
    Func_0808a100(0, 1);
    Func_0808a1b8(0, 0xe000, 0);
    Func_0808a1b8(1, 0xe000, 40);
    Func_0808a1e8(0, 256, 0);
    Func_0808a1e8(1, 256, 0);
    Func_0808a128(0, 6, 0);
    Func_0808a128(1, 6, 60);
    Func_0808a0f0(5, 0x01db0000, 0x14c0000);
    Func_0808a0f0(9, 0x01eb0000, 0x14c0000);
    Func_0808a0f0(11, 0x01cb0000, 0x15c0000);
    Func_0808a0f0(10, 0x01fb0000, 0x15c0000);
    Func_0808a208(0x73333, 0xe666);
    Func_0808a210(0x01e50000, -1, 0x01590000, 1);
    Func_0808a1b8(5, 0x6000, 0);
    Func_0808a1b8(9, 0x5000, 0);
    Func_0808a1b8(11, 0x5000, 0);
    Func_0808a1b8(10, 0x5000, 0);
    Func_0808a218();
    Func_0808a010(40);
}

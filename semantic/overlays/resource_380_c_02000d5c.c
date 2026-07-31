typedef int s32;

/*
 * resource_380 owner at 0x02000d5c, 560 bytes: a two-track scene
 * configuration -- every call in the first half is issued once for
 * track 0 and once for track 1 with staggered values (offsets 0 vs
 * 40/60/80, ids 0x1df vs 0x1eb, rates 0x109/0x1c5 vs 282/0x1d5), one
 * conditional in the middle forwards a context's +8/+16 pair when
 * Func_02005794(0) returns an object, and the tail places the same
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
 * Uncertainty: callees unidentified beyond call shape; Func_020059b8
 * is called with four arguments and later with three, so it is
 * declared old-style.
 */

extern void Func_02005876(s32 arg0);
extern void Func_02005734(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005770(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005804(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020057a4(s32 arg0, s32 arg1);
extern void Func_0200572a(s32 arg0);
extern void Func_020057ca(s32 arg0, s32 arg1);
extern void Func_02005738(s32 arg0);
extern void Func_0200583a(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005780(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020057ba(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200584e(s32 arg0, s32 arg1, s32 arg2);
extern s32 Func_02005794(s32 arg0);
extern void Func_020057ea(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020057b8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020057f2(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005886(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005892(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005842(s32 arg0, s32 arg1);
extern void Func_02005852(s32 arg0, s32 arg1);
extern void Func_020057c0(s32 arg0);
extern void Func_020058c2(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020058cc(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005854(s32 arg0, s32 arg1);
extern void Func_02005864(s32 arg0, s32 arg1);
extern void Func_0200581e(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005828(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200585a(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200586e(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200588e(s32 arg0, s32 arg1);
extern void Func_0200590a(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005916(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005932(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200593e(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020058d8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020058e2(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020058d6(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020058ee(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020058fa(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020059a2(s32 arg0, s32 arg1);
extern void Func_020059b8();
extern void Func_02005994(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020059a0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020059ac(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020059f4(void);
extern void Func_020058da(s32 arg0);

void Func_02000d5c(void)
{
    s32 context;

    Func_02005876(17);
    Func_02005734(0, 0x8000, 0x4000);
    Func_02005770(0, 231, 490);
    Func_02005804(0, 0xc000, 30);
    Func_020057a4(0, 3);
    Func_0200572a(180);
    Func_020057ca(0, 2);
    Func_02005738(80);
    Func_0200583a(0, 0x101, 60);
    Func_02005780(0, 0x8000, 0x4000);
    Func_020057ba(0, 246, 0x1df);
    Func_0200584e(0, 0xe000, 10);
    context = Func_02005794(0);
    if (context != 0)
        Func_020057ea(1,
            *(s32 *)(context + 8), *(s32 *)(context + 16));
    Func_020057b8(1, 0x10000, 0x8000);
    Func_020057f2(1, 0x101, 0x1eb);
    Func_02005886(0, 0x2000, 0);
    Func_02005892(1, 0xa000, 40);
    Func_02005842(0, 2);
    Func_02005852(1, 2);
    Func_020057c0(20);
    Func_020058c2(0, 0x101, 0);
    Func_020058cc(1, 0x101, 80);
    Func_02005854(0, 3);
    Func_02005864(1, 4);
    Func_0200581e(0, 0x13333, 0x9999);
    Func_02005828(1, 0x13333, 0x9999);
    Func_0200585a(0, 0x109, 0x1c5);
    Func_0200586e(1, 282, 0x1d5);
    Func_0200588e(0, 1);
    Func_0200590a(0, 0xe000, 0);
    Func_02005916(1, 0xe000, 40);
    Func_02005932(0, 256, 0);
    Func_0200593e(1, 256, 0);
    Func_020058d8(0, 6, 0);
    Func_020058e2(1, 6, 60);
    Func_020058d6(5, 0x01db0000, 0x14c0000);
    Func_020058e2(9, 0x01eb0000, 0x14c0000);
    Func_020058ee(11, 0x01cb0000, 0x15c0000);
    Func_020058fa(10, 0x01fb0000, 0x15c0000);
    Func_020059a2(0x73333, 0xe666);
    Func_020059b8(0x01e50000, -1, 0x01590000, 1);
    Func_02005994(5, 0x6000, 0);
    Func_020059a0(9, 0x5000, 0);
    Func_020059ac(11, 0x5000, 0);
    Func_020059b8(10, 0x5000, 0);
    Func_020059f4();
    Func_020058da(40);
}

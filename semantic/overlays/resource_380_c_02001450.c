typedef int s32;

/*
 * resource_380 owner at 0x02001450, 828 bytes: a branch-free scripted
 * scene beat of roughly ninety calls -- actor placements for ids
 * 5/9/10/11/12, per-actor mode and animation runs, three
 * position-with-hold blocks around fixed-point targets (0x15a0000/
 * 0x1b80000, 0x1180000/0x1d60000, 0x14e0000 pair), two dialogue-style
 * pairs against ids 0x01050000/0x109b/0x01dd0000, and a long
 * choreographed tail that walks the same actor set through
 * mode-2/mode-3 transitions with 20-tick holds before the closing
 * placement.  Same linear-scene-bracket family as the 0x02000d5c and
 * 0x0200227c owners; the actor coordinates reuse that family's
 * vocabulary.
 *
 * Complete owner: `push {lr}` at 0x02001450 through `pop {r0} / bx r0`
 * at 0x2001778-0x200177c, alignment halfword, then the three-word
 * literal pool 0x02001780-0x0200178b (0x01050000, 0x109b, 0x01dd0000);
 * next owner's prologue at 0x0200178c.
 *
 * Uncertainty: callees unidentified beyond call shape; all are in the
 * 0x2005xxx-0x2006xxx resident neighborhood.  Four are called at two
 * arities inside this one owner (Func_02005f08, Func_02005f2a,
 * Func_02006020, Func_020060a2, plus Func_020060b6/Func_02006168) and
 * are declared old-style per the overlay precedent.
 */

extern void Func_02005ed8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005ee4(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005ef0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005efc(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005f08();
extern void Func_02005ec0(s32 arg0, s32 arg1);
extern void Func_020056e4(s32 arg0, s32 arg1);
extern void Func_02005ed0(s32 arg0, s32 arg1);
extern void Func_02005f42(s32 arg0, s32 arg1);
extern void Func_02005e48(s32 arg0);
extern void Func_02005704(s32 arg0, s32 arg1);
extern void Func_02005efa(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005f04(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005f0e(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005e74(s32 arg0);
extern void Func_02005f14(s32 arg0, s32 arg1);
extern void Func_02005f70(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200574c(s32 arg0, s32 arg1);
extern void Func_02005f8a(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005f2a();
extern void Func_02005eb0(s32 arg0);
extern void Func_02005fa2(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005fae(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005f5e(s32 arg0, s32 arg1);
extern void Func_02005f6e(s32 arg0, s32 arg1);
extern void Func_02005edc(s32 arg0);
extern void Func_02005fd0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005fdc(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005fe8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005f88(s32 arg0, s32 arg1);
extern void Func_020057c4(s32 arg0, s32 arg1);
extern void Func_02006014(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02006026(s32 arg0, s32 arg1);
extern void Func_02005fc6(s32 arg0, s32 arg1);
extern void Func_02005f34(s32 arg0);
extern void Func_02005fb2(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005e18(s32 arg0);
extern void Func_02006020();
extern void Func_02006044(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02006050(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020060c4(void);
extern void Func_020060d0(void);
extern void Func_02005fee(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020060a6(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02005ed2(void);
extern void Func_02005e68(s32 arg0);
extern void Func_020060ec(void);
extern void Func_02006100(void);
extern void Func_02005fa6(s32 arg0);
extern void Func_0200609a(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020060b6();
extern void Func_0200605e(s32 arg0, s32 arg1);
extern void Func_020060ae(s32 arg0, s32 arg1);
extern void Func_02005fd4(s32 arg0);
extern void Func_0200605c(s32 arg0, s32 arg1);
extern void Func_02005fe2(s32 arg0);
extern void Func_020060ca(s32 arg0, s32 arg1);
extern void Func_02005ff0(s32 arg0);
extern void Func_020060ce(s32 arg0, s32 arg1);
extern void Func_020060f2(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020060a2();
extern void Func_020060b2(s32 arg0, s32 arg1);
extern void Func_02006114(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02006120(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02006194(void);
extern void Func_020061a0(void);
extern void Func_02006168();
extern void Func_02005f94(void);
extern void Func_020061ae(void);
extern void Func_020061c2(void);
extern void Func_02006068(s32 arg0);
extern void Func_02006108(s32 arg0, s32 arg1);
extern void Func_02006076(s32 arg0);
extern void Func_0200616a(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200593e(s32 arg0, s32 arg1);
extern void Func_0200617c(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200611c(s32 arg0, s32 arg1);
extern void Func_0200595e(s32 arg0, s32 arg1);
extern void Func_0200614a(s32 arg0, s32 arg1);
extern void Func_020060b8(s32 arg0);
extern void Func_02005974(s32 arg0, s32 arg1);
extern void Func_02006160(s32 arg0, s32 arg1);
extern void Func_020061bc(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020061c8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020059a4(s32 arg0, s32 arg1);
extern void Func_02006188(s32 arg0, s32 arg1);
extern void Func_02006198(s32 arg0, s32 arg1);
extern void Func_020061f4(s32 arg0, s32 arg1, s32 arg2);

void Func_02001450(void)
{
    Func_02005ed8(5, 0x6000, 0);
    Func_02005ee4(9, 0x5000, 0);
    Func_02005ef0(10, 0x8000, 0);
    Func_02005efc(12, 0x5000, 40);
    Func_02005f08(11, 0xd000, 40);
    Func_02005ec0(11, 2);
    Func_020056e4(11, 20);
    Func_02005ed0(12, 2);
    Func_02005f42(12, 258);
    Func_02005e48(60);
    Func_02005704(12, 10);
    Func_02005efa(10, 12, 0);
    Func_02005f04(5, 12, 0);
    Func_02005f0e(9, 12, 0);
    Func_02005e74(40);
    Func_02005f14(10, 1);
    Func_02005f70(10, 0x8000, 10);
    Func_02005f08(10, 3);
    Func_0200574c(10, 10);
    Func_02005f8a(11, 0, 10);
    Func_02005f2a(11, 3);
    Func_02005eb0(10);
    Func_02005fa2(5, 0, 0);
    Func_02005fae(9, 0x8000, 20);
    Func_02005f5e(5, 2);
    Func_02005f6e(9, 2);
    Func_02005edc(10);
    Func_02005fd0(5, 0xc000, 0);
    Func_02005fdc(9, 0xb000, 10);
    Func_02005fe8(10, 0xb000, 40);
    Func_02005f88(10, 3);
    Func_020057c4(10, 10);
    Func_02006014(5, 256, 20);
    Func_02006026(12, 258);
    Func_02005fc6(12, 3);
    Func_02005f34(40);
    Func_02005fb2(1, 0x15a0000, 0x1b80000);
    Func_02005e18(1);
    Func_02006020(1, 0);
    Func_02006044(0, 0x2000, 0);
    Func_02006050(1, 0xa000, 20);
    Func_020060c4();
    Func_020060d0();
    Func_02005fee(1, 0x1180000, 0x1d60000);
    Func_020060a6(0x01050000, -1, 0x1d20000, 0);
    Func_02005ed2();
    Func_02005e68(1);
    Func_020060ec();
    Func_02006100();
    Func_02005fa6(20);
    Func_0200609a(0, 0x3000, 10);
    Func_020060b6(1, 258, 0);
    Func_0200605e(1, 3);
    Func_020060ae(1, 0);
    Func_02005fd4(10);
    Func_0200605c(1, 4);
    Func_02005fe2(10);
    Func_020060ca(1, 0);
    Func_02005ff0(10);
    Func_020060b6(0x109b, 1);
    Func_020060ce(11, 0);
    Func_020060f2(11, 0xd000, 10);
    Func_020060a2(0, 2);
    Func_020060b2(0, 2);
    Func_02006020(10);
    Func_02006114(0, 0xe000, 0);
    Func_02006120(1, 0xe000, 20);
    Func_02006194();
    Func_020061a0();
    Func_02006168(0x01dd0000, -1, 0x14e0000, 0);
    Func_02005f94();
    Func_02005f2a(1);
    Func_020061ae();
    Func_020061c2();
    Func_02006068(20);
    Func_02006108(9, 2);
    Func_02006076(10);
    Func_0200616a(9, 0x8000, 30);
    Func_0200593e(9, 20);
    Func_0200617c(5, 0, 40);
    Func_0200611c(5, 4);
    Func_020060a2(20);
    Func_0200595e(5, 10);
    Func_0200614a(12, 2);
    Func_020060b8(80);
    Func_02005974(12, 20);
    Func_02006160(5, 2);
    Func_020061bc(5, 0xc000, 0);
    Func_020061c8(9, 0xb000, 20);
    Func_02006168(12, 3);
    Func_020059a4(12, 20);
    Func_02006188(5, 2);
    Func_02006198(9, 2);
    Func_020061f4(12, 0xb000, 40);
}

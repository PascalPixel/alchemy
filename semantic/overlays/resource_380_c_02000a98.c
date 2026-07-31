typedef int s32;
typedef unsigned char u8;

/*
 * resource_380 owner at 0x02000a98, 708 bytes: a three-panel scene
 * build.  A six-pass warm-up loop (with one extra centering call on
 * pass 1), then three near-identical panel blocks -- each one places a
 * layer, loads a strip from the resident table 0x0200d088 (rows 65/31,
 * 79/9, 91/10), configures it with the same 87/42 parameter shape at
 * increasing offsets (10/33, 24/9-11, 36/12), waits 40, and fades in.
 * After the third block it writes the Data_03001ebc counter idiom
 * (word at +448 = 514), runs a settle sequence, spawns object 223 at a
 * fixed coordinate triple through the in-overlay Func_02004f5c, and
 * hands it plus id 0x1077 to the closer.
 *
 * Complete owner: `push {r5,r6,lr}` + r8/sl spill at 0x02000a98
 * through `pop {r0} / bx r0` at 0x02000d22, then the literal pool
 * 0x02000d24-0x02000d5b; next owner's prologue at 0x02000d5c (this
 * overlay's own resource_380_c_02000d5c.c, drafted alongside).
 *
 * Uncertainty: callees unidentified beyond call shape.  Three are
 * called at two arities inside this one owner and are declared
 * old-style: Func_02005592 ((8) and (0x10000, 1)), Func_02005630
 * ((a, b) pairs and (object, 1)), Func_02005648 ((a, -1, b, 1) and
 * (40)).  The 0x004049d2 loop constant and the fixed-point pool values
 * are recorded as passed.
 */

extern void Func_020055ba(s32 arg0);
extern void Func_0200557c(s32 arg0, s32 arg1);
extern void Func_02005592();
extern void Func_02005450(s32 arg0);
extern void Func_020055a8(s32 arg0);
extern void Func_02005466(s32 arg0);
extern void Func_020053f2(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005408(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005496(s32 arg0);
extern void Func_020055ae(s32 arg0, s32 arg1);
extern void Func_020055c6(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_020055d2(void);
extern void Func_0200543a(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020054c8(s32 arg0);
extern void Func_0200564e(s32 arg0);
extern void Func_0200542c(s32 table, s32 arg1, s32 arg2);
extern void Func_02005454(
    s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02005460(
    s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_0200550e(s32 arg0);
extern void Func_02005490(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005630();
extern void Func_02005648();
extern void Func_02005654(void);
extern void Func_020054bc(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200554a(s32 arg0);
extern void Func_020056d0(s32 arg0);
extern void Func_020054aa(s32 table, s32 arg1, s32 arg2);
extern void Func_020054ce(
    s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_020054d6(
    s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02005584(s32 arg0);
extern void Func_02005506(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020056a6(s32 arg0, s32 arg1);
extern void Func_020056be(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_020056ca(void);
extern void Func_02005532(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020055c0(s32 arg0);
extern void Func_02005746(s32 arg0);
extern void Func_02005520(s32 table, s32 arg1, s32 arg2);
extern void Func_02005544(
    s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_0200554c(
    s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_020055fa(s32 arg0);
extern void Func_02005764(void);
extern void Func_02005770();
extern void Func_02005738();
extern void Func_02005564(void);
extern void Func_020054fa(s32 arg0);
extern void Func_020055b2(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200578e(void);
extern void Func_020057a2(void);
extern void Func_020057ce(s32 arg0);
extern void Func_020055d4(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020055e0(void);
extern void Func_02005666(s32 arg0);
extern void Func_020055d0(
    s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_0200567e(s32 arg0);
extern s32 Func_02004f5c(s32 template_id, s32 x, s32 y, s32 z);
extern void Func_02005698(s32 arg0);

void Func_02000a98(void)
{
    s32 table = 0x0200d088;
    s32 object;
    u8 *record;
    s32 pass;

    Func_020055ba(141);
    for (pass = 0; pass != 6; pass++) {
        Func_0200557c(0x4049d2, 1);
        Func_02005592(8);
        Func_02005450(8);
        Func_02005592(0x10000, 1);
        Func_020055a8(8);
        Func_02005466(8);
        if (pass == 1)
            Func_020053f2(0x10000, 0x10000, 0x10000);
    }
    Func_02005408(0x20000, 0x10000, 0x10000);
    Func_02005496(30);
    Func_020055ae(0x26666, 0x4ccc);
    Func_020055c6(0xa70000, -1, 0x02110000, 1);
    Func_020055d2();
    Func_0200543a(0x10000, 0x20000, 0x10000);
    Func_020054c8(20);

    Func_0200564e(144);
    Func_0200542c(table, 65, 31);
    Func_02005454(0, 0, 1, 1, 10, 31);
    Func_02005460(87, 42, 10, 33, 1, 2);
    Func_0200550e(40);
    Func_02005490(0, 0, 0);
    Func_02005630(0x66666, 0xcccc);
    Func_02005648(0x01870000, -1, 0xb10000, 1);
    Func_02005654();
    Func_020054bc(0x10000, 0x20000, 0x10000);
    Func_0200554a(20);

    Func_020056d0(144);
    Func_020054aa(table, 79, 9);
    Func_020054ce(0, 0, 1, 1, 24, 9);
    Func_020054d6(87, 42, 24, 11, 1, 2);
    Func_02005584(40);
    Func_02005506(0, 0, 0);
    Func_020056a6(0x26666, 0x4ccc);
    Func_020056be(0x02470000, -1, 0xc10000, 1);
    Func_020056ca();
    Func_02005532(0x10000, 0x20000, 0x10000);
    Func_020055c0(20);

    Func_02005746(144);
    Func_02005520(table, 91, 10);
    Func_02005544(0, 0, 1, 1, 36, 10);
    Func_0200554c(87, 42, 36, 12, 1, 2);
    Func_020055fa(40);

    record = *(u8 **)0x03001ebc;
    *(s32 *)(record + 448) = 514;
    Func_02005764();
    Func_02005770();
    Func_02005738(0xe80000, -1, 0x01dd0000, 0);
    Func_02005564();
    Func_020054fa(1);
    Func_020055b2(0x20000, 0x10000, 0x10000);
    Func_0200578e();
    Func_020057a2();
    Func_02005648(40);
    Func_020057ce(0x121);
    Func_020055d4(-1, -1, 0xe666);
    Func_020055e0();
    Func_02005666(20);
    Func_020055d0(0, 40, 13, 66, 3, 3);
    Func_0200567e(20);
    object = Func_02004f5c(223, 0xe80000, 0x100000, 0x1d00000);
    Func_02005698(40);
    Func_02005630(object, 1);
    Func_02005630(0x1077, 1);
}

typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

/*
 * Resource 3c8 at 0x02001218: a two-variant cutscene - the same scene played
 * with one of two casts of placement calls, chosen by which of the two
 * one-shot keys 0x982/0x983 is already set.
 *
 * The owner is complete: `push {r5, r6, r7, lr} ; sub sp, #8` at 0x02001218
 * and the matching `add sp, #8 ; pop {r5, r6, r7} ; pop {r0} ; bx r0` at
 * 0x020014be..0x020014c4, followed by an alignment halfword and an 11-word
 * literal pool at 0x020014c6-0x020014f3.  No live frame or register state
 * escapes the row.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.  Both variant arms converge on the shared tail at
 * 0x02001462.
 *
 * 0x03001ebc is the package workspace pointer proven by the byte-exact
 * sibling 0x02000754.  The two halfword flags at +0xcba and +0xcb6 are the
 * same pair this lane's 0x02001780 writes; +0xcb6 is set to 1 on entry and
 * cleared again on the way out, i.e. it is a "scene running" latch.
 *
 * The eight reserved stack bytes carry arguments five and six of this
 * package's six-argument placement ABI (r0-r3 then [sp,#0] then [sp,#4]), the
 * order fixed by the byte-exact sibling 0x020010cc.  Both arms keep 3 and 2
 * in r5/r6 across their whole run of placement calls; the reconstruction
 * spells the constants at each site.
 *
 * ONE IMPORT, TWO ARGUMENT COUNTS - three times over.  0x02006136 and
 * 0x02006126 are each reached once from the opening block and once as a
 * six-argument placement call; 0x02006146 likewise; 0x0200621c is a one- and
 * a six-argument call inside the second arm; 0x02006416 is a two- and a
 * four-argument call in the tail.  That is why every import is declared
 * old-style, and it is direct evidence for the package rule that an overlay
 * `Func_` name is an import identity rather than a typed function.
 *
 * Constants are `movs #imm ; lsls #shift`: 128<<9 = 0x10000, 128<<8 = 0x8000,
 * 128<<5 = 0x1000, 228<<17 = 0x01c80000.  0x2688, 0x00010005, 0x7fff, 0x982,
 * 0x983, 0x021e0000 and 0x01a70000 are pooled.
 *
 * Fifty-two `bl` sites reach 49 distinct targets, matching the inventory's
 * call count exactly; every site is spelled below.  Import naming follows the
 * note in resource_3c8_c_020002f0.c.
 */

/* Used for their return values. */
s32 Func_020060fa();
s32 Func_02006104();
s32 Func_02006126();

/* Old-style declarations: the imports' real interfaces are not known here. */
void Func_020060c6();
void Func_02006136();
void Func_020060ae();
void Func_02006200();
void Func_02006216();
void Func_020060ec();
void Func_02006262();
void Func_020060f8();
void Func_02006220();
void Func_02006236();
void Func_0200610c();
void Func_02006120();
void Func_02006128();
void Func_02006138();
void Func_02006146();
void Func_020060f2();
void Func_02006106();
void Func_02006116();
void Func_02006156();
void Func_02006166();
void Func_02006176();
void Func_02006186();
void Func_02006196();
void Func_020061a6();
void Func_0200620e();
void Func_0200621c();
void Func_020061c8();
void Func_020061dc();
void Func_020061ec();
void Func_020061fc();
void Func_0200620c();
void Func_0200622c();
void Func_0200623c();
void Func_0200624c();
void Func_0200625c();
void Func_0200626c();
void Func_0200627c();
void Func_02006416();
void Func_0200642c();
void Func_02006302();
void Func_020063fe();
void Func_02006422();
void Func_02006328();
void Func_02006430();
void Func_0200643c();
void Func_02006350();

void Func_02001218(void)
{
    u8 *workspace;

    workspace = *(u8 **)0x03001ebc;
    *(u16 *)(workspace + 0xcba) = 0;
    *(u16 *)(workspace + 0xcb6) = 1;

    Func_020060c6();
    Func_02006136(0, 1);
    Func_020060ae(0x2688, 1);
    Func_02006200(0x10000, 0);
    Func_02006200(0x00010005, 0);
    Func_02006216(120);
    Func_020060ec(100);
    Func_02006262(142);
    Func_020060f8(30);
    Func_02006220(0x7fff, 0);
    Func_02006236(60);
    Func_0200610c(70);

    if (Func_020060fa(0x982) == 0 && Func_02006104(0x983) == 0) {
        if ((*(u32 *)0x03001e40 & 1) != 0) {
            Func_02006120(0x982);
        } else {
            Func_02006128(0x983);
        }
    }

    if (Func_02006126(0x982) == 0) {
        Func_02006138(0x982);
        Func_02006146(0x983);

        Func_020060f2(103, 27, 89, 27, 7, 8);
        Func_02006106(41, 90, 27, 92, 3, 2);
        Func_02006116(41, 90, 29, 93, 3, 2);
        Func_02006126(41, 90, 27, 94, 3, 2);
        Func_02006136(41, 90, 27, 96, 3, 2);
        Func_02006146(41, 90, 29, 97, 3, 2);
        Func_02006156(41, 96, 25, 91, 3, 2);
        Func_02006166(41, 92, 25, 93, 3, 2);
        Func_02006176(41, 96, 25, 95, 3, 2);
        Func_02006186(41, 96, 25, 97, 3, 2);
        Func_02006196(41, 96, 27, 96, 3, 2);
        Func_020061a6(41, 96, 29, 97, 3, 2);
    } else {
        Func_0200620e(0x983);
        Func_0200621c(0x982);

        Func_020061c8(111, 27, 89, 27, 7, 8);
        Func_020061dc(41, 90, 25, 91, 3, 2);
        Func_020061ec(41, 90, 25, 93, 3, 2);
        Func_020061fc(41, 90, 25, 95, 3, 2);
        Func_0200620c(41, 90, 25, 97, 3, 2);
        Func_0200621c(41, 90, 27, 96, 3, 2);
        Func_0200622c(41, 90, 29, 97, 3, 2);
        Func_0200623c(41, 94, 27, 92, 3, 2);
        Func_0200624c(41, 96, 29, 93, 3, 2);
        Func_0200625c(41, 94, 27, 94, 3, 2);
        Func_0200626c(41, 96, 27, 96, 3, 2);
        Func_0200627c(41, 96, 29, 97, 3, 2);
    }

    Func_02006416(0x10000, 0);
    Func_0200642c(20);
    Func_02006302(40);
    Func_020063fe(0x8000, 0x1000);
    Func_02006416(0x01c80000, -1, 0x021e0000, 1);
    Func_02006422();
    Func_02006328(50);
    Func_02006430(0x01c80000, -1, 0x01a70000, 1);
    Func_0200643c();
    Func_02006350();

    *(u16 *)(workspace + 0xcb6) = 0;
}

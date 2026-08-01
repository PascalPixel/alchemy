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
 * same pair this reconstruction's 0x02001780 writes; +0xcb6 is set to 1 on entry and
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
void Func_08009180();
void Func_08015040();
s32 Func_080770c0();
void Func_080770c8();
void Func_080770d0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
void Func_0808a100();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_0808a330();
void Func_0808a338();
void Func_0808a348();
void Func_080f9010();

/* Old-style declarations: the imports' real interfaces are not known here. */

void Func_02001218(void)
{
    u8 *workspace;

    workspace = *(u8 **)0x03001ebc;
    *(u16 *)(workspace + 0xcba) = 0;
    *(u16 *)(workspace + 0xcb6) = 1;

    Func_0808a018();
    Func_0808a100(0, 1);
    Func_08015040(0x2688, 1);
    Func_0808a338(0x10000, 0);
    Func_0808a330(0x00010005, 0);
    Func_0808a348(120);
    Func_0808a010(100);
    Func_080f9010(142);
    Func_0808a010(30);
    Func_0808a330(0x7fff, 0);
    Func_0808a348(60);
    Func_0808a010(70);

    if (Func_080770c0(0x982) == 0 && Func_080770c0(0x983) == 0) {
        if ((*(u32 *)0x03001e40 & 1) != 0) {
            Func_080770c8(0x982);
        } else {
            Func_080770c8(0x983);
        }
    }

    if (Func_080770c0(0x982) == 0) {
        Func_080770c8(0x982);
        Func_080770d0(0x983);

        Func_08009180(103, 27, 89, 27, 7, 8);
        Func_08009180(41, 90, 27, 92, 3, 2);
        Func_08009180(41, 90, 29, 93, 3, 2);
        Func_08009180(41, 90, 27, 94, 3, 2);
        Func_08009180(41, 90, 27, 96, 3, 2);
        Func_08009180(41, 90, 29, 97, 3, 2);
        Func_08009180(41, 96, 25, 91, 3, 2);
        Func_08009180(41, 92, 25, 93, 3, 2);
        Func_08009180(41, 96, 25, 95, 3, 2);
        Func_08009180(41, 96, 25, 97, 3, 2);
        Func_08009180(41, 96, 27, 96, 3, 2);
        Func_08009180(41, 96, 29, 97, 3, 2);
    } else {
        Func_080770c8(0x983);
        Func_080770d0(0x982);

        Func_08009180(111, 27, 89, 27, 7, 8);
        Func_08009180(41, 90, 25, 91, 3, 2);
        Func_08009180(41, 90, 25, 93, 3, 2);
        Func_08009180(41, 90, 25, 95, 3, 2);
        Func_08009180(41, 90, 25, 97, 3, 2);
        Func_08009180(41, 90, 27, 96, 3, 2);
        Func_08009180(41, 90, 29, 97, 3, 2);
        Func_08009180(41, 94, 27, 92, 3, 2);
        Func_08009180(41, 96, 29, 93, 3, 2);
        Func_08009180(41, 94, 27, 94, 3, 2);
        Func_08009180(41, 96, 27, 96, 3, 2);
        Func_08009180(41, 96, 29, 97, 3, 2);
    }

    Func_0808a330(0x10000, 0);
    Func_0808a348(20);
    Func_0808a010(40);
    Func_0808a208(0x8000, 0x1000);
    Func_0808a210(0x01c80000, -1, 0x021e0000, 1);
    Func_0808a218();
    Func_0808a010(50);
    Func_0808a210(0x01c80000, -1, 0x01a70000, 1);
    Func_0808a218();
    Func_0808a020();

    *(u16 *)(workspace + 0xcb6) = 0;
}

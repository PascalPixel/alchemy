#include "types.h"

/*
 * Resource 3b8 cutscene at 0x02000674.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02000674 and the matching
 * `pop {r5, r6} / pop {r0} / bx r0` interworking return at 0x02000ad4.  Two
 * literal pools sit inside the span and are branched over: 0x020006da-0x020006e7
 * (the branch at 0x020006d8 jumps to 0x020006e8) and 0x02000ada-0x02000af7,
 * which follows the return.  Both are data.
 *
 * Resolver-confirmed main-image symbols name every external call below.
 * Old-style declarations keep the source honest where a shared engine helper
 * is reached with more than one argument shape.
 *
 * The only two absolute addresses this owner touches are the GBA display
 * control register at 0x04000000 and the IWRAM workspace pointer at
 * 0x03001ebc, the same pointer 0x020040b4 uses.
 */

void Func_02004a58();
void Func_02004b16();
u8 *Func_02004a84();
u8 *Func_02004a90();
void Func_020049da();
void Func_02004b54();
u8 *Func_02004ab6();
u8 *Func_02004ac2();
void Func_02004b2e();
u8 *Func_02004ad4();
void Func_02004a62();
void Func_02004b50();
void Func_02004b5e();
void Func_02004b6c();
void Func_02004bce();
void Func_02004bd8();
void Func_02004be4();
void Func_02004c34();
void Func_02004c40();
void Func_02004b26();
void Func_02004b6a();
void Func_02004c9c();
void Func_02004c28();
void Func_02004b56();
void Func_02004c06();
void Func_02004b64();
void Func_02004c3c();
void Func_02004b72();
u8 *Func_02004ba0();
u8 *Func_02004bae();
void Func_02004c1c();
void Func_02004b9a();
void Func_02004c32();
void Func_02004ba8();
void Func_02004c38();
void Func_02004bb6();
void Func_02004c66();
void Func_02004bc4();
void Func_02004bd2();
void Func_02004cbc();
void Func_02004be2();
void Func_02004cce();
void Func_02004cc6();
void Func_02004bfc();
void Func_02004c94();
void Func_02004c0a();
void Func_02004ce2();
void Func_02004c18();
void Func_02004d04();
void Func_02004d10();
void Func_02004d14();
void Func_02004c42();
void Func_02004cda();
void Func_02004c50();
void Func_02004d28();
void Func_02004c5e();
void Func_02004d42();
void Func_02004c70();
void Func_02004d20();
void Func_02004c7e();
void Func_02004d60();
void Func_02004d6c();
void Func_02004c9a();
void Func_02004d6a();
s32 Func_02004ccc();
void Func_02004cb6();
void Func_02004d4e();
void Func_02004cc4();
void Func_02004d5c();
void Func_02004cd2();
void Func_02004daa();
void Func_02004ce0();
void Func_02004d78();
void Func_02004cee();
void Func_02004d86();
void Func_02004cfc();
void Func_02004dd4();
void Func_02004d1a();
void Func_02004db2();
void Func_02004d36();
void Func_02004dce();
void Func_02004d44();
void Func_02004e1c();
void Func_02004d52();
void Func_02004d90();
void Func_02004ec4();
void Func_02004e4e();
void Func_02004d7c();
void Func_02004e1e();
void Func_02004dc0();
void Func_02004ef4();
u8 *Func_02004dca();
void Func_02004d48();
void Func_02004e8c();
void Func_02004e98();
void Func_02004dc6();
void Func_02004e56();
void Func_02004e64();
void Func_02004e6c();
void Func_02004e7c();
void Func_02004df2();
void Func_02004e2c();
void Func_02004e36();
void Func_02004e40();
void Func_02004ea0();
u8 *Func_02004e46();
void Func_02004e80();
void Func_02004eae();
void Func_02004ec0();
void Func_02004ed0();
u8 *Func_02004e76();
void Func_02004eb0();
void Func_02004ede();
void Func_02004ef0();
void Func_02004f00();
u8 *Func_02004ea6();
void Func_02004ee0();
void Func_02004f0e();
void Func_02004f20();
void Func_02004ea6_b();
void Func_02004eba();

                     

                    

                     

                     

                     

                     

                     

void Func_02000674(void)
{
    volatile u16 *displayControl = (volatile u16 *)0x04000000;
    u8 **workspace = (u8 **)0x03001ebc;
    u8 *entry;
    s32 gate;

    Func_02004a58();
    Func_02004b16(0x2280);

    entry = Func_02004a84(0);
    entry[0x54] = 0;
    entry = Func_02004a90(10);
    entry[0x54] = 0;

    *displayControl = 0x0140;
    Func_020049da(1);
    *displayControl = 0x1140;
    Func_02004b54(-1, 0);

    entry = Func_02004ab6(0);
    entry[0x54] = 1;
    entry = Func_02004ac2(10);
    entry[0x54] = 1;

    Func_02004b2e(0, 31);
    /* Pass the selected scene record directly to the engine handoff. */
    Func_02004a62(Func_02004ad4(0), 0);

    Func_02004b50(1, 0x780000, 0x680000);
    Func_02004b5e(3, 0x680000, 0x500000);
    Func_02004b6c(2, 0x780000, 0x780000);
    Func_02004bce(1, 0, 0);
    Func_02004bd8(3, 0, 0);
    Func_02004be4(2, 0xe000, 0);

    Func_02004c34();
    Func_02004c40();
    *(u32 *)(*workspace + 0x1c8) = 60;
    Func_02004b26(20);
    *(u32 *)(*workspace + 0x1c8) = 24;

    Func_02004b6a(3, 0x10000, 0x8000);
    Func_02004c9c(3, 16, 0);
    Func_02004c28(3, 0x2000, 0);
    Func_02004b56(20);
    Func_02004c06(3, 2);
    Func_02004b64(30);
    Func_02004c3c(3, 0);
    Func_02004b72(10);

    /* 0xfffd0000 is a negative fixed-point bias applied to two different
     * fields of two different records. */
    entry = Func_02004ba0(0);
    *(u32 *)(entry + 16) += 0xfffd0000;
    entry = Func_02004bae(0);
    *(u32 *)(entry + 64) += 0xfffd0000;

    Func_02004c1c(0, 32);
    Func_02004b9a(40);
    Func_02004c32(0, 34);
    Func_02004ba8(30);
    Func_02004c38(0, 33);
    Func_02004bb6(50);
    Func_02004c66(1, 2);
    Func_02004bc4(30);
    Func_02004c9c(1, 0);
    Func_02004bd2(10);
    Func_02004cbc(0, 0x105, 60);
    Func_02004be2(20);
    Func_02004cce(1, 0x102, 60);
    Func_02004cc6(1, 0);
    Func_02004bfc(10);
    Func_02004c94(1, 4);
    Func_02004c0a(20);
    Func_02004ce2(1, 0);
    Func_02004c18(10);
    Func_02004d04(0, 0x102, 80);
    Func_02004d10(2, 0x106, 60);
    Func_02004d14(2, 0xc000, 0);
    Func_02004c42(30);
    Func_02004cda(2, 4);
    Func_02004c50(20);
    Func_02004d28(2, 0);
    Func_02004c5e(10);
    Func_02004d42(1, 0x4000, 0);
    Func_02004c70(30);
    Func_02004d20(1, 2);
    Func_02004c7e(45);
    Func_02004d60(1, 0, 0);
    Func_02004d6c(2, 0xe000, 0);
    Func_02004c9a(30);
    Func_02004d6a(1, 0);

    gate = Func_02004ccc(-1, 0);
    if (gate != 0) {
        Func_02004cb6(10);
        Func_02004d4e(0, 34);
        Func_02004cc4(20);
        Func_02004d5c(1, 3);
        Func_02004cd2(20);
        Func_02004daa(1, 0);
        Func_02004ce0(10);
        Func_02004d78(0, 33);
        Func_02004cee(30);
        Func_02004d86(1, 3);
        Func_02004cfc(20);
        *(u16 *)(*workspace + 0x1d8) += 1;
        *(u16 *)(*workspace + 0x1d8) += 2;
        Func_02004dd4(1, 0);
    } else {
        Func_02004d1a(10);
        Func_02004db2(0, 33);
        Func_02004d36(30);
        Func_02004dce(1, 3);
        Func_02004d44(20);
        Func_02004e1c(1, 0);
    }

    Func_02004d52(10);
    Func_02004d90(1, 0x10000, 0x8000);
    Func_02004ec4(1, -16, 0);
    Func_02004e4e(1, 0, 0);
    Func_02004d7c(35);
    Func_02004e1e(0, 6, 0);
    Func_02004dc0(0, 0x1e666, 0xf333);
    Func_02004ef4(0, -32, 0);
    /* Repeat the handoff after the camera transition. */
    Func_02004d48(Func_02004dca(0), 1);
    Func_02004e8c(3, 0x4000, 0);
    Func_02004e98(2, 0xc000, 0);
    Func_02004dc6(40);
    Func_02004e56(0, 3);
    Func_02004dd4(30);
    Func_02004e64(2, 3);
    Func_02004e6c(1, 3);
    Func_02004e7c(3, 3);
    Func_02004df2(30);
    Func_02004e2c(1, 0x13333, 0x9999);
    Func_02004e36(3, 0x13333, 0x9999);
    Func_02004e40(2, 0x13333, 0x9999);

    Func_02004ea0(1, 2);
    entry = Func_02004e46(0);
    if (entry != 0)
        Func_02004e80(1, *(s16 *)(entry + 10), *(s16 *)(entry + 18));
    Func_02004eae(1);
    Func_02004ec0(1, 0, 0);

    Func_02004ed0(3, 2);
    entry = Func_02004e76(0);
    if (entry != 0)
        Func_02004eb0(3, *(s16 *)(entry + 10), *(s16 *)(entry + 18));
    Func_02004ede(3);
    Func_02004ef0(3, 0, 0);

    Func_02004f00(2, 2);
    entry = Func_02004ea6(0);
    if (entry != 0)
        Func_02004ee0(2, *(s16 *)(entry + 10), *(s16 *)(entry + 18));
    Func_02004f0e(2);
    Func_02004f20(2, 0, 0);

    /* Pause once more after restoring the record state. */
    Func_02004ea6_b(10);
    Func_02004eba();
}

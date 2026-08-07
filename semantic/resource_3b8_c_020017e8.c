#include "types.h"

/*
 * Resource 3b8 cutscene at 0x020017e8.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x020017e8 and the matching
 * `pop {r5, r6, r7} / pop {r0} / bx r0` interworking return at 0x02001ff6.
 * Two mid-function literal pools are branched over, 0x02001a8a-0x02001aaf and
 * 0x02001d7a-0x02001d87, and a third at 0x02001ffc-0x02002013 follows the
 * return.  All three are data.
 *
 * Workspace access goes through the IWRAM pointer array at 0x03001ebc, the
 * same base the other owners in this overlay use: word 0 is the scene record
 * (fields at +0x1c0, +0x1c8 and the halfword counter at +0x1d8) and word 4,
 * i.e. the pointer at 0x03001ecc, is a second record whose halfwords at
 * +0x52a/+0x534/+0x536 this owner drives.  The two counted loops write the
 * cursor at +0x52a *after* each call, so they leave 5 and 31 behind
 * respectively.
 *
 * Resolver-confirmed main-image symbols name every external call below.
 * Old-style declarations keep the source honest where a shared engine helper
 * is reached with more than one argument shape.
 */

void Func_02005bcc();
void Func_02005c8a();
void Func_02005c5a();
u8 *Func_02005c00();
void Func_02005b7e();
void Func_02005c6c();
void Func_02005c7a();
void Func_02005c88();
void Func_02005cec();
void Func_02005cf6();
void Func_02005d02();
void Func_02005bae();
void Func_02005d5c();
void Func_02005d82();
void Func_02005d8e();
void Func_02005be2();
void Func_02005d38();
void Func_02005c96();
void Func_02005c0a();
void Func_02005cce();
void Func_02005dba();
void Func_02005d94();
void Func_02005cf2();
void Func_02005cf8();
void Func_02005de2();
void Func_02005de6();
void Func_02005d14();
void Func_02005dec();
void Func_02005d22();
void Func_02005dfa();
void Func_02005e0e();
void Func_02005d3c();
void Func_02005d4a();
void Func_02005d88();
void Func_02005ebc();
void Func_02005ec6();
void Func_02005e52();
void Func_02005d80();
void Func_02005e58();
u8 *Func_02005db6();
u8 *Func_02005dc4();
void Func_02005e32();
void Func_02005db0();
void Func_02005e48();
void Func_02005dbe();
void Func_02005e4e();
void Func_02005dcc();
void Func_02005e64();
void Func_02005dda();
void Func_02005eb2();
void Func_02005de8();
void Func_02005ed4();
void Func_02005ee0();
void Func_02005ed0();
s32 Func_02005e32_b();
void Func_02005e1c();
void Func_02005eb4();
void Func_02005e2a();
void Func_02005f14();
void Func_02005ed6();
void Func_02005f20();
void Func_02005e8c();
void Func_02005f24();
void Func_02005e9a();
void Func_02005f84();
void Func_02005f46();
void Func_02005f50();
void Func_02005f9e();
void Func_02005fb8();
void Func_02005ee6();
void Func_02005f7e();
void Func_02005ef4();
void Func_02005fd8();
void Func_02005f06();
void Func_02005fb6();
void Func_02005ff8();
void Func_02006004();
void Func_02005f32();
void Func_02005fca();
void Func_02005f40();
void Func_02006018();
void Func_02005f4e();
void Func_02006032();
void Func_02005f60();
void Func_02006044();
void Func_02005f72();
void Func_0200605e();
void Func_0200601c();
void Func_02005f92();
void Func_0200606a();
void Func_02005fa0();
void Func_02006050();
void Func_02005fae();
void Func_0200607e();
s32 Func_02005fe0();
void Func_02006062();
void Func_020060c2();
void Func_020060ba();
void Func_0200609c();
void Func_02006012();
void Func_020060fc();
void Func_02006106();
void Func_0200603c();
void Func_02006120();
void Func_0200604e();
void Func_020060e6();
void Func_0200605c();
void Func_02006140();
void Func_0200606e();
void Func_0200611e();
void Func_0200607c();
void Func_02006160();
void Func_0200616c();
void Func_0200609a();
void Func_02006132();
void Func_020060a8();
void Func_02006180();
void Func_020060b6();
void Func_0200619a();
void Func_020060c8();
void Func_020061ac();
void Func_020060da();
void Func_020061c6();
void Func_020060ec();
void Func_02006184();
void Func_020060fa();
void Func_020061d2();
void Func_0200610c();
void Func_020061a4();
void Func_0200611a();
void Func_020061f2();
void Func_02006128();
void Func_020061d8();
void Func_02006136();
void Func_0200620e();
void Func_02006174();
void Func_0200620c();
void Func_02006182();
void Func_0200625a();
void Func_02006190();
void Func_02006240();
void Func_0200619e();
void Func_02006276();
void Func_02006298();
void Func_020061be();
void Func_02006256();
void Func_020061cc();
void Func_020062a4();
void Func_020061da();
void Func_0200629c();
void Func_020062c8();
void Func_020062d4();
void Func_02006202();
void Func_020062c2();
void Func_02006264();
void Func_0200639a();
u8 *Func_02006270();
void Func_020061ee();
void Func_02006254();
void Func_02006338();
void Func_02006344();
void Func_02006350();
void Func_0200627e();
void Func_02006284();
void Func_02006334();
void Func_02006292();
void Func_0200636a();
void Func_020062a0();
void Func_020062ae();
void Func_02006386();
void Func_020062bc();
void Func_02006354();
void Func_020062ca();
void Func_020063ae();
void Func_020063ba();
void Func_020062e8();
void Func_020063c0();
void Func_020062f6();
void Func_0200638e();
void Func_0200639e();
void Func_02006314();
void Func_0200634e();
void Func_02006358();
void Func_02006362();
void Func_020063c2();
u8 *Func_02006368();
void Func_020063a2();
void Func_020063d0();
void Func_020063e2();
void Func_020063f2();
u8 *Func_02006398();
void Func_020063d2();
void Func_02006400();
void Func_02006412();
void Func_02006422();
u8 *Func_020063c8();
void Func_02006402();
void Func_02006430();
void Func_02006442();
void Func_020063c8_b();
void Func_020063dc();

                     

                    

                     

                     

                     

                     

                     

                     

void Func_020017e8(void)
{
    u32 *slotBase = (u32 *)0x03001ebc;
    u8 *state;
    u16 *cursor;
    u8 *record;
    s32 index;
    s32 branch;

    Func_02005bcc();
    Func_02005c8a(0x228c);
    Func_02005c5a(0, 31);
    /* Pass the selected scene record directly to the engine handoff. */
    Func_02005b7e(Func_02005c00(0), 0);
    Func_02005c6c(1, 0x680000, 0x680000);
    Func_02005c7a(3, 0x580000, 0x780000);
    Func_02005c88(2, 0x780000, 0x780000);
    Func_02005cec(1, 0x4000, 0);
    Func_02005cf6(3, 0, 0);
    Func_02005d02(2, 0x8000, 0);

    *(u32 *)((u8 *)slotBase[0] + 0x1c8) = 12;
    *(u32 *)((u8 *)slotBase[0] + 0x1c0) = 0x100;

    Func_02005bae(1);
    Func_02005d5c(9);

    /* A second workspace, from the pointer one word further on at
     * 0x03001ecc. */
    state = (u8 *)slotBase[4];
    *(u16 *)(state + 0x536) = 1;
    Func_02005d82();
    cursor = (u16 *)(state + 0x52a);
    *(u16 *)(state + 0x534) = 0x1f1f;
    Func_02005d8e();
    *cursor = 0;

    for (index = 1; index <= 5; index++) {
        *cursor = (u16)index;
        Func_02005be2(3);
    }

    Func_02005c88(40);
    Func_02005d38(0, 2);
    Func_02005c96(30);

    *(u16 *)(state + 0x536) = 31;
    for (index = 5; index <= 31; index++) {
        *cursor = (u16)index;
        Func_02005c0a(3);
    }

    *(u32 *)((u8 *)slotBase[0] + 0x1c0) = 0x209;
    *(u32 *)((u8 *)slotBase[0] + 0x1c8) = 24;

    Func_02005cce(20);
    Func_02005dba(1, 256, 50);
    Func_02005d94(1, 0, 40);
    Func_02005d94(1, 2);
    Func_02005cf2(30);
    Func_02005cf8(10);
    Func_02005de2(2, 0x101, 40);
    Func_02005de6(2, 0xe000, 0);
    Func_02005d14(30);
    Func_02005dec(2, 0);
    Func_02005d22(10);
    Func_02005dfa(3, 0);
    Func_02005e0e(3, 0xe000, 0);
    Func_02005d3c(30);
    Func_02005dec(3, 2);
    Func_02005d4a(30);
    Func_02005d88(3, 0x10000, 0x8000);
    Func_02005ebc(3, 0, -40);
    Func_02005ec6(3, 32, 0);
    Func_02005e52(3, 0x2000, 0);
    Func_02005d80(10);
    Func_02005e58(3, 0);

    *(u32 *)(record + 16) += 0xfffd0000;
    record = Func_02005db6(0);
    *(u32 *)(record + 64) += 0xfffd0000;
    record = Func_02005dc4(0);

    Func_02005e32(0, 32);
    Func_02005db0(40);
    Func_02005e48(0, 34);
    Func_02005dbe(30);
    Func_02005e4e(0, 33);
    Func_02005dcc(40);
    Func_02005e64(1, 4);
    Func_02005dda(20);
    Func_02005eb2(1, 0);
    Func_02005de8(10);
    Func_02005ed4(0, 0x102, 80);
    Func_02005ee0(2, 256, 50);
    Func_02005ed0(2, 0);

    if (Func_02005e32_b(-1, 0) == 0) {
        Func_02005e1c(10);
        Func_02005eb4(0, 33);
        Func_02005e2a(20);
        Func_02005f14(1, 0x103, 40);
        Func_02005ed6(1, 4, 13);
        Func_02005ee0(1, 4, 30);
        Func_02005f20(1, 0);
        *(u16 *)((u8 *)slotBase[0] + 0x1d8) += 1;
    } else {
        Func_02005e8c(10);
        Func_02005f24(0, 34);
        Func_02005e9a(20);
        Func_02005f84(1, 0x103, 40);
        Func_02005f46(1, 4, 13);
        Func_02005f50(1, 4, 30);
        Func_02005f9e(1, 0);
        *(u16 *)((u8 *)slotBase[0] + 0x1d8) += 1;
    }

    Func_02005ed4(10);
    Func_02005fb8(2, 0xa000, 0);
    Func_02005ee6(30);
    Func_02005f7e(2, 4);
    Func_02005ef4(20);
    Func_02005fd8(1, 0x2000, 0);
    Func_02005f06(30);
    Func_02005fb6(1, 2);
    Func_02005f14(40);
    Func_02005ff8(1, 0xe000, 0);
    Func_02006004(2, 0xe000, 0);
    Func_02005f32(30);
    Func_02005fca(1, 3);
    Func_02005f40(30);
    Func_02006018(1, 0);
    Func_02005f4e(10);
    Func_02006032(3, 0x6000, 0);
    Func_02005f60(50);
    Func_02006044(3, 0x2000, 0);
    Func_02005f72(35);
    Func_0200605e(3, 264, 50);
    Func_02005f84(10);
    Func_0200601c(3, 3);
    Func_02005f92(30);
    Func_0200606a(3, 0);
    Func_02005fa0(10);
    Func_02006050(2, 2);
    Func_02005fae(20);
    Func_0200607e(2, 0);

    if (Func_02005fe0(-1, 0) == 0) {
        Func_02005fca(10);
        Func_02006062(0, 33);
        Func_02005fd8(20);
        *(u16 *)((u8 *)slotBase[0] + 0x1d8) += 1;
        Func_020060c2(1, 0x107, 40);
        Func_020060ba(1, 0);
    } else {
        branch = 0;
        Func_02006004(10);
        Func_0200609c(0, 34);
        Func_02006012(20);
        Func_020060fc(1, 0x107, 40);
        *(u16 *)((u8 *)slotBase[0] + 0x1d8) += 1;
        branch = 1;
        Func_02006106(1, 0);
    }

    Func_0200603c(10);
    Func_02006120(2, 0xa000, 0);
    Func_0200604e(30);
    Func_020060e6(2, 4);
    Func_0200605c(20);
    Func_02006140(1, 0x2000, 0);
    Func_0200606e(30);
    Func_0200611e(1, 2);
    Func_0200607c(40);
    Func_02006160(1, 0xe000, 0);
    Func_0200616c(2, 0xe000, 0);
    Func_0200609a(30);
    Func_02006132(1, 3);
    Func_020060a8(30);
    Func_02006180(1, 0);
    Func_020060b6(10);
    Func_0200619a(3, 0x6000, 0);
    Func_020060c8(50);
    Func_020061ac(3, 0x2000, 0);
    Func_020060da(35);
    Func_020061c6(3, 264, 50);
    Func_020060ec(10);
    Func_02006184(3, 3);
    Func_020060fa(30);
    Func_020061d2(3, 0);

    if (branch == 0) {
        Func_0200610c(10);
        Func_020061a4(2, 3);
        Func_0200611a(30);
        Func_020061f2(2, 0);
        Func_02006128(10);
        Func_020061d8(1, 2);
        Func_02006136(30);
        Func_0200620e(1, 0);
        *(u16 *)((u8 *)slotBase[0] + 0x1d8) += 2;
    } else {
        *(u16 *)((u8 *)slotBase[0] + 0x1d8) += 2;
        Func_02006174(10);
        Func_0200620c(2, 4);
        Func_02006182(20);
        Func_0200625a(2, 0);
        Func_02006190(10);
        Func_02006240(1, 2);
        Func_0200619e(20);
        Func_02006276(1, 0);
    }

    Func_020061ac(10);
    Func_02006298(0, 258, 60);
    Func_020061be(10);
    Func_02006256(1, 3);
    Func_020061cc(30);
    Func_020062a4(1, 0);
    Func_020061da(20);
    Func_0200629c(2, 3, 40);
    Func_020062c8(3, 0x2000, 0);
    Func_020062d4(2, 0xe000, 0);
    Func_02006202(30);

    /* Busy wait on the IWRAM status word at 0x03001ae8, pumping the frame
     * import until any of bits 4-7 is set. */
    while ((*(volatile u32 *)0x03001ae8 & 0xf0) == 0)
        Func_02006180(1);

    Func_020062c2(0, 6, 0);
    Func_02006264(0, 0x1e666, 0xf333);
    Func_0200639a(0, -32, -8);
    /* Same r0 carry idiom as the head. */
    Func_020061ee(Func_02006270(0), 1);
    Func_02006254(20);
    Func_02006338(0, 0xc000, 0);
    Func_02006344(3, 0x4000, 0);
    Func_02006350(2, 0xc000, 0);
    Func_0200627e(20);
    Func_02006284(10);
    Func_02006334(3, 2);
    Func_02006292(20);
    Func_0200636a(3, 0);
    Func_020062a0(10);
    Func_02006350(1, 2);
    Func_020062ae(20);
    Func_02006386(1, 0);
    Func_020062bc(20);
    Func_02006354(2, 3);
    Func_020062ca(30);
    Func_020063ae(0, 0x4000, 0);
    Func_020063ba(1, 0x2000, 0);
    Func_020062e8(30);
    Func_020063c0(2, 0);
    Func_020062f6(20);
    Func_02006386(0, 3);
    Func_0200638e(1, 3);
    Func_0200639e(3, 3);
    Func_02006314(30);
    Func_0200634e(1, 0x13333, 0x9999);
    Func_02006358(3, 0x13333, 0x9999);
    Func_02006362(2, 0x13333, 0x9999);

    Func_020063c2(1, 2);
    record = Func_02006368(0);
    if (record != 0)
        Func_020063a2(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    Func_020063d0(1);
    Func_020063e2(1, 0, 0);

    Func_020063f2(3, 2);
    record = Func_02006398(0);
    if (record != 0)
        Func_020063d2(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
    Func_02006400(3);
    Func_02006412(3, 0, 0);

    Func_02006422(2, 2);
    record = Func_020063c8(0);
    if (record != 0)
        Func_02006402(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    Func_02006430(2);
    Func_02006442(2, 0, 0);

    /* Reached with r0 = 10, not as the record query above. */
    Func_020063c8_b(10);
    Func_020063dc();
}

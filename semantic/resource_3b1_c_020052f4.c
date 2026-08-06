#include "types.h"





























extern u8 *Data_03001ebc;

/*
 * resource_3b1 owner at 0x020052f4, 912 bytes including its thirteen-word
 * trailing pool. It stages five actors and actor 27, preserves the answer
 * branch's scene-counter increment, and finishes by publishing state 0x209.
 * The sole return is at 0x0200564e. All 80 BL sites across 27 resolved targets
 * appear below in machine order.
 */
extern void Func_02009ba2();
extern void Func_02009bb2();
extern void Func_02009bc0();
extern void Func_02009bd2();
extern void Func_02009be2();
extern void Func_02009bf6();
extern void Func_0200b8c0();
extern void Func_02009c2e();
extern void Func_0200b9b6();
extern void Func_0200b9ca();
extern void Func_0200b850();
extern void Func_0200b96c();
extern void Func_0200b978();
extern void Func_0200b984();
extern void Func_0200b990();
extern void Func_02009cb2();
extern void Func_02009c42();
extern void Func_02009cce();
extern void Func_0200b9e6();
extern void Func_0200b9fc();
extern void Func_0200b8fe();
extern void Func_0200b942();
extern void Func_0200b94e();
extern void Func_0200b95a();
extern void Func_0200ba2a();
extern void Func_0200ba40();
extern void Func_0200b97c();
extern void Func_0200b98a();
extern void Func_0200ba34();
extern void Func_0200b9ec();
extern void Func_02009cde();
extern void Func_02009d64();
extern void Func_0200b9e4();
extern void Func_0200ba0c();
extern void Func_02009cfe();
extern void Func_02009d84();
extern void Func_02009d90();
extern void Func_0200ba82();
extern void Func_0200ba3a();
extern void Func_0200ba12();
extern void Func_0200b9ee();
extern void Func_02009d6e();
extern void Func_0200ba6e();
extern void Func_0200bac0();
extern void Func_02009df2();
extern void Func_0200ba72();
extern void Func_0200b9d8();
extern void Func_0200bae2();
extern void Func_0200baaa();
extern void Func_0200b9f0();
extern void Func_0200ba98();
extern void Func_0200b9fe();
extern void Func_02009dc8();
extern void Func_0200bb00();
extern int Func_0200ba38();
extern void Func_0200bac4();
extern void Func_02009dd6();
extern void Func_0200bad4();
extern void Func_02009df6();
extern void Func_02009e7c();
extern void Func_0200bb1c();
extern void Func_0200baf4();
extern void Func_02009e16();
extern void Func_02009e9c();
extern void Func_0200babe();
extern void Func_0200bac6();
extern void Func_0200bade();
extern void Func_0200bbce();
extern void Func_0200bbe8();
extern void Func_0200baee();
extern void Func_0200bb34();
extern void Func_0200bb38();
extern void Func_0200bad6();
extern void Func_02009f16();
extern void Func_0200bae4();
extern void Func_0200baea();
extern void Func_0200bc58();
void Func_020052f4(void)
{
    Func_02009ba2((void *)0, 0x1bc, 0x12c, 0);
    Func_02009bb2((void *)1, 0x1ca, 0x136, 0);
    Func_02009bc0((void *)2, 0x1bc, 0x14a, 0);
    Func_02009bd2((void *)3, 0x1b0, 0x136, 0);
    Func_02009be2((void *)27, 0x1b8, 0x86, 0x8000);
    Func_02009bf6((void *)10, 0x1c6, 0xf8, 0x3000);
    Func_0200b8c0(10, 6);
    Func_02009c2e(0x01b80000, -1, 0x01340000, 0x01000001);
    Func_0200b9b6();
    Func_0200b9ca();
    Func_0200b850(20);
    Func_0200b96c(0, 0x2000, 0);
    Func_0200b978(1, 0xa000, 0);
    Func_0200b984(2, 0xa000, 0);
    Func_0200b990(3, 0x2000, 40);
    Func_02009cb2(2, 1, 20);
    Func_0200b978(0x1e6e);
    Func_02009c42(27);
    Func_02009cce(1, 0xc000, 0);
    Func_0200b9e6(0x26666, 0x4ccc);
    Func_0200b9fc(0x01b80000, -1, 0x00b00000, 1);
    Func_0200b8fe(27, 0x19999, 0xcccc);
    Func_0200b942(27, 0x198, 0x86);
    Func_0200b94e(27, 0x198, 0x98);
    Func_0200b95a(27, 0x1a8, 0xa4);
    Func_0200ba2a(0x19999, 0x3333);
    Func_0200ba40(0x01b80000, -1, 0x012c0000, 1);
    Func_0200b97c(27, 0x1a8, 0xde);
    Func_0200b98a(27, 0x1a8, 0x106);
    Func_0200ba34(27, 0x3000, 20);
    Func_0200b9ec(27, 1);
    Func_02009cde(27);
    Func_02009d64(2, 1, 20);
    Func_0200b9e4(27, 3);
    Func_0200ba0c(27, 1);
    Func_02009cfe(27);
    Func_02009d84(3, 2, 60);
    Func_02009d90(1, 0xe000, 60);
    Func_0200ba82(27, 0, 40);
    Func_0200ba3a(27, 1);
    Func_0200ba12(27, 2);
    Func_0200b9ee(27, 0x1b0, 0x10c);
    Func_0200b9fc(27, 0x1c4, 0x10c);
    Func_0200ba34(27, 1);
    Func_02009d6e(27, 0xd000);
    Func_0200ba6e(27, 2);
    Func_0200bac0(27, 0, 20);
    Func_02009df2(1, 0xc000, 20);
    Func_0200ba72(27, 4);
    Func_0200b9d8(40);
    Func_0200bae2(27, 0, 80);
    Func_0200baaa(27, 1);
    Func_0200b9f0(20);
    Func_0200ba98(27, 3);
    Func_0200b9fe(10);
    Func_02009dc8(27, 0x5000);
    Func_0200bb00(27, 0);

    if (Func_0200ba38(0, 0) == 0) {
        Func_0200bac4(27, 3);
        Func_02009dd6(27);
    } else {
        Func_0200bad4(27, 4);
        Func_02009df6(27);
        *(u16 *)(Data_03001ebc + 472) += 1;
        Func_02009e7c(3, 2, 40);
        Func_0200bb1c(27, 1);
        Func_0200baf4(27, 3);
        Func_02009e16(27);
    }

    Func_02009e9c(2, 1, 20);
    Func_0200babe(1, 0x0200e7f0);
    Func_0200bac6(2, 0x0200e7f0);
    Func_0200bade(3, 0x0200e7f0);
    Func_0200bbce(0x9999, 0x1333);
    Func_0200bbe8(0x01b80000, -1, 0x00b00000, 1);
    Func_0200baee(0, 0x10000, 0x8000);
    *(s32 *)(Data_03001ebc + 448) = 0x209;
    Func_0200bb34(0, 0x1a8, 0x110);
    Func_0200bb38(0, 0x1a8, 0xa4);
    Func_0200bad6(60);
    Func_02009f16(9, 0, 0);
    Func_0200bae4(0x301);
    Func_0200baea(0x927);
    Func_0200bc58(4);
}

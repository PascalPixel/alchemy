#include "types.h"


























/*
 * resource_3b1 owner at 0x020050e4, 528 bytes including its ten-word trailing
 * pool. It initializes a four-actor tableau, copies three nullable positions,
 * then runs the staged transition and flag teardown. The sole return is at
 * 0x020052ca. All 48 BL sites across 24 targets appear in machine order.
 */
extern void Func_020099a6();
extern void Func_0200b64e();
extern void Func_0200b73a();
extern void Func_0200b60c();
extern void Func_0200b66c();
extern void Func_0200b648();
extern void Func_0200b654();
extern void Func_0200b660();
extern void Func_0200b698();
extern void Func_0200b724();
extern u8 * Func_0200b642();
extern void Func_0200b6b0();
extern u8 * Func_0200b656();
extern void Func_0200b6c4();
extern u8 * Func_0200b66a();
extern void Func_0200b6d8();
extern void Func_0200b5b6();
extern void Func_0200b6d4();
extern void Func_0200b6ae();
extern void Func_0200b6ea();
extern void Func_0200b6c8();
extern void Func_0200b70c();
extern void Func_0200b734();
extern void Func_0200b73c();
extern void Func_0200b7c8();
extern void Func_0200b7d4();
extern void Func_0200b7e0();
extern void Func_0200b7ee();
extern void Func_0200b6b2();
extern void Func_0200b6e8();
extern void Func_02009b1a();
extern void Func_02009b26();
extern void Func_0200b83e();
extern void Func_02009b00();
extern void Func_0200b7dc();
extern void Func_02009ae8();
extern void Func_0200b816();
extern void Func_02009ae0();
extern void Func_0200b84e();
extern void Func_02009b3a();
extern void Func_0200b798();
extern void Func_0200b7a0();
extern void Func_0200b7b8();
extern void Func_0200b76e();
extern void Func_0200b75c();
extern void Func_02009ba6();
extern void Func_0200b774();
void Func_020050e4(void)
{
    u8 *record;

    Func_020099a6(0x01b80000, -1, 0x00b00000, 0x01000001);
    Func_0200b64e(0, 0x01b80000, 0x00860000);
    Func_0200b73a();
    Func_0200b60c(0, 0x19999, 0xcccc);
    Func_0200b66c(0, 5);
    Func_0200b648(0, 0x198, 0x86);
    Func_0200b654(0, 0x198, 0x98);
    Func_0200b660(0, 0x1b0, 0xa6);
    Func_0200b698(0, 1);
    Func_0200b724(0, 0x4000, 0);

    record = Func_0200b642(0);
    if (record != 0)
        Func_0200b6b0(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    record = Func_0200b656(0);
    if (record != 0)
        Func_0200b6c4(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    record = Func_0200b66a(1);
    if (record != 0)
        Func_0200b6d8(3, *(s32 *)(record + 8), *(s32 *)(record + 16));

    Func_0200b5b6(1);
    Func_0200b698(2, 0x19999, 0xcccc);
    Func_0200b6d4(2, 0x1a8, 0x98);
    Func_0200b6ae(1, 0x19999, 0xcccc);
    Func_0200b6ea(1, 0x1c0, 0xa8);
    Func_0200b6c8(3, 0x20000, 0x10000);
    Func_0200b70c(3, 0x1ca, 0x98);
    Func_0200b734(1, 1);
    Func_0200b73c(2, 1);
    Func_0200b7c8(2, 0x4000, 0);
    Func_0200b7d4(1, 0x4000, 0);
    Func_0200b7e0(3, 0x4000, 40);
    Func_0200b7ee(10);
    Func_0200b6b2(0x1e46, 1, 10);
    Func_0200b6e8(10);
    Func_02009b1a(0, 0, 40);
    Func_02009b26(1, 0x4000, 20);
    Func_0200b83e(0x39999, 0x7333);
    Func_02009b00(0x01b80000, -1, 0x01400000, 0x10000014);
    Func_0200b7dc(8, 2);
    Func_02009ae8(8, 0xd000);
    Func_0200b816(0x1e47);
    Func_02009ae0(8);
    Func_0200b84e(8, 0, 20);
    Func_02009b3a(0x01b80000, -1, 0x00860000, 0x10000000);
    Func_0200b798(1, 0x0200e7c8);
    Func_0200b7a0(2, 0x0200e7c8);
    Func_0200b7b8(3, 0x0200e7c8);
    Func_0200b76e(40);
    Func_0200b75c(0x301);
    Func_02009ba6(23, 0, 0);
    Func_0200b774(0x12f);
}

#include "types.h"

extern void Func_0808a018(void); extern void Func_0808a020(void);
extern void Func_0808a0f0(s32,s32,s32); extern void Func_0808a1b8(s32,s32,s32);
extern void Func_0808a200(s32,s32); extern void Func_0808a360(void); extern void Func_0808a010(s32);
extern void Func_0808a090(s32,s32,s32); extern void Func_0808a0c8(s32,s32,s32);
extern void Func_0808a100(s32,s32); extern void Func_0808a0b8(s32,s32,s32);
extern void Func_080000d0(u8 *,s32); extern void Func_080000d8(u8 *); extern void Func_080f9010(s32);
extern void Func_0808a1e0(s32,s32); extern u8 *Func_0808a080(s32); extern void Func_0808a0e8(s32);
extern void Func_0808a1f0(s32,s32); extern void Func_0808a170(s32); extern void Func_0808a180(s32,s32);

void Func_0200131c(void)
{
    u8 *record;

    Func_0808a018();
    Func_0808a0f0(0, 0x00a00000, 0x00800000);
    Func_0808a0f0(8, 0x00980000, 0x00700000);
    Func_0808a0f0(9, 0x00a80000, 0x00700000);
    Func_0808a1b8(0, 0x4000, 0);
    Func_0808a1b8(17, 0x3000, 0);
    Func_0808a1b8(18, 0x5000, 0);
    Func_0808a200(0, 0);
    Func_0808a360();
    Func_0808a010(30);
    Func_0808a090(0, 0x1cccc, 0xe666);
    Func_0808a090(8, 0x1cccc, 0xe666);
    Func_0808a090(9, 0x1cccc, 0xe666);
    Func_0808a0c8(8, 152, 288);
    Func_0808a0c8(9, 168, 288);
    Func_0808a100(0, 4);
    Func_0808a0b8(0, 160, 296);
    Func_080000d0((u8 *)0x020092a5, 3200);
    Func_0808a010(1);
    Func_080f9010(121);
    Func_0808a010(20);
    Func_0808a1e0(8, 3);
    Func_0808a1e0(9, 3);
    Func_080f9010(121);
    Func_0808a010(30);

    record = Func_0808a080(8); record[35] |= 1;
    record = Func_0808a080(9); record[35] |= 1;
    Func_0808a100(0, 4);
    Func_080f9010(121);
    Func_0808a0e8(0);
    Func_0808a100(8, 1);
    Func_0808a100(9, 1);
    Func_080000d8((u8 *)0x020092a5);

    record = Func_0808a080(0); record[85] |= 2;
    *(s32 *)(Func_0808a080(0) + 40) = 0x60000;
    *(s32 *)(Func_0808a080(0) + 44) = 0x60000;
    Func_0808a010(1);
    goto first_descent_test;
first_descent_wait:
    Func_0808a010(1);
first_descent_test:
    if (*(s32 *)(Func_0808a080(0) + 12) != 0)
        goto first_descent_wait;

    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a100(0, 19);
    Func_080f9010(127);
    Func_0808a1f0(0, 258);
    Func_080000d0((u8 *)0x020092a5, 3200);
    Func_0808a010(2);
    *(s32 *)(Func_0808a080(0) + 40) = 0x30000;
    Func_0808a010(1);
    goto second_descent_test;
second_descent_wait:
    Func_0808a010(1);
second_descent_test:
    if (*(s32 *)(Func_0808a080(0) + 12) != 0)
        goto second_descent_wait;

    Func_0808a1f0(0, 258);
    Func_0808a010(10);
    Func_0808a100(0, 1);
    Func_080000d8((u8 *)0x020092a5);
    Func_0808a010(50);
    Func_0808a170(0x2410);
    Func_0808a180(8, 0);
    record = Func_0808a080(8); record[35] |= 1;
    record = Func_0808a080(9); record[35] |= 1;
    Func_0808a090(8, 0x10000, 0x8000);
    Func_0808a090(9, 0x10000, 0x8000);
    Func_0808a0c8(8, 144, 200);
    Func_0808a0c8(9, 176, 200);
    Func_0808a0e8(8);
    Func_0808a0e8(9);
    Func_0808a100(8, 1);
    Func_0808a100(9, 1);
    Func_0808a010(30);
    Func_0808a1b8(8, 0x3000, 0);
    Func_0808a1b8(9, 0x5000, 0);
    Func_0808a020();
}

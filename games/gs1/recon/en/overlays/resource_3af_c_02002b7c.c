#include "types.h"

/* AUDITED GENERATED CALL SCRIPT for FieldScene_ConfigureThreeActors:
 * 27 calls, 0 loops, and all explicit actor and workspace state effects.
 * Recovered from the complete decoded owner and checked against the original assembly. */

#define FieldScene_ConfigureThreeActors Func_02002b7c

void Func_0200661a();
void Func_02006652();
void Func_02006dfc();
void Func_02006e38();
void *Func_02006e6c();
void Func_02006e96();
void *Func_02006e9a();
void *Func_02006ec0();
void *Func_02006eda();
void Func_02006ef2();
void *Func_02006efc();
void Func_02006f1a();
void Func_02006f2c();
void Func_02006f34();
void Func_02006f56();
void Func_02006f60();
void Func_02006f9c();
void Func_02006fa6();
void Func_02006faa();
void Func_02006fb2();
void Func_02006fc0();
void Func_02006fd4();
void Func_02006ff4();
void Func_0200702c();
void Func_02007040();
void Func_02007054();
void Func_02007090();

void Func_02002b7c(void)
{
    void *p1;
    void *p10;
    void *p13;
    void *p8;
    u8 *workspace;

    p1 = Func_02006e6c();
    Func_02006f2c(0, 15);
    Func_02006e38(Func_02006e9a(0), 0);
    Func_02006e96(33608032);
    Func_02006dfc(1);
    Func_02006f1a(20, 12845056, 32899072);
    p8 = Func_02006ec0(20);
    *(u16 *)(p8 + 6) = 40960;
    Func_02006f34(22, 12058624, 34340864, 40960);
    p10 = Func_02006eda(22);
    *(u16 *)(p10 + 6) = 45056;
    Func_02006fc0(21, 1);
    Func_02006f56(21, 12058624, 41418752);
    p13 = Func_02006efc(21);
    *(u16 *)(p13 + 6) = 45056;
    workspace = *(u8 **)0x03001ebc;
    *(u32 *)(workspace + 448) = 514;
    Func_02007040();
    Func_02007054();
    Func_02006ef2(20);
    Func_02006f9c(22, 4, 10);
    Func_02006fa6(22, 6, 20);
    Func_02006fd4(7909);
    Func_0200661a(22);
    Func_02006fb2(20, 3);
    Func_02006f60(21, 196608, 98304);
    Func_02006faa(21, 180);
    Func_0200702c(21, 45056, 40);
    Func_02006ff4(21, 1);
    Func_02006652(21);
    Func_02007090(15);
}

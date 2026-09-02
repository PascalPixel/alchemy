#include "types.h"

#define FieldScene_RunFourEntitySequence Func_02000f8c

extern u8 Value_00000fe0;
void Func_080770c8();
void Func_080770d0();
void Func_0808a010();
void Func_0808a018();
void Func_0808a020();
int Func_0808a070();
u8 *Func_0808a080();
void Func_0808a090();
void Func_0808a0b8();
void Func_0808a0e0();
void Func_0808a0e8();
s32 Func_0808a0f0();
s32 Func_0808a100();
void Func_0808a110();
void Func_0808a128();
void Func_0808a130();
s32 Func_0808a138();
void Func_0808a150();
void Func_0808a170();
void Func_0808a178();
void Func_0808a180();
void Func_0808a188();
s32 Func_0808a1b8();
s32 Func_0808a1e8();
void Func_0808a1f0();
void Func_0808a208();
void Func_0808a210();
void Func_0808a218();
void Func_0808a360();
void Func_0808a370();

/*
 * resource_37f owner at 0x02000f8c, 1,680 bytes: full scene sequence
 * for entities 0, 1, 5 and 8.
 *
 * Complete owner: saved-link prologue at 0x02000f8c through the sole
 * interworking return at 0x02001600, alignment and six-word trailing pool
 * through 0x0200161b.  A branch at 0x020013d0 skips an eleven-word interior
 * constant pool and resumes at 0x02001400.  All 174 BL sites resolve to 30
 * import veneers and are represented below in machine order.
 */
/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */
s32 Func_02002b3c();
s32 Func_02002b6a();
s32 Func_02002b7e();
s32 Func_02002b92();
s32 Func_02002ba8();
s32 Func_02002bb2();
s32 Func_02002bbc();
s32 Func_02002bc6();
s32 Func_02002bd0();
s32 Func_02002bfe();
s32 Func_02002c02();
s32 Func_02002c06();
s32 Func_02002c0c();
s32 Func_02002c0e();
s32 Func_02002c18();
s32 Func_02002c26();
s32 Func_02002c38();
s32 Func_02002c3e();
s32 Func_02002c46();
s32 Func_02002c4c();
s32 Func_02002c4e();
s32 Func_02002c56();
s32 Func_02002cb4();
s32 Func_02002cba();
s32 Func_02002cc6();
s32 Func_02002cd2();
s32 Func_02002cdc();
s32 Func_02002cde();
s32 Func_02002ce8();
s32 Func_02002cf4();
s32 Func_02002d00();
s32 Func_02002d0c();
s32 Func_02002d10();
s32 Func_02002d18();
s32 Func_02002d1c();
s32 Func_02002d1e();
s32 Func_02002d22();
s32 Func_02002d2e();
s32 Func_02002d36();
s32 Func_02002d3a();
s32 Func_02002d46();
s32 Func_02002d52();
s32 Func_02002d54();
s32 Func_02002d5e();
s32 Func_02002d68();
s32 Func_02002d74();
s32 Func_02002d76();
s32 Func_02002d86();
s32 Func_02002d8e();
s32 Func_02002da0();
s32 Func_02002da8();
s32 Func_02002daa();
s32 Func_02002db6();
s32 Func_02002dd2();
s32 Func_02002dec();
s32 Func_02002e12();
s32 Func_02002e2a();
s32 Func_02002e34();
s32 Func_02002e36();
s32 Func_02002e4c();
s32 Func_02002e58();
s32 Func_02002e5e();
s32 Func_02002e60();
s32 Func_02002e6c();
s32 Func_02002e7e();
s32 Func_02002e88();
s32 Func_02002e8a();
s32 Func_02002e90();
s32 Func_02002e96();
s32 Func_02002ea0();
s32 Func_02002ea2();
s32 Func_02002eaa();
s32 Func_02002eac();
s32 Func_02002eb2();
s32 Func_02002ebc();
s32 Func_02002eca();
s32 Func_02002ed8();
s32 Func_02002ee4();
s32 Func_02002ee6();
s32 Func_02002f14();
s32 Func_02002f16();
s32 Func_02002f18();
s32 Func_02002f1e();
s32 Func_02002f26();
s32 Func_02002f28();
s32 Func_02002f2e();
s32 Func_02002f46();
s32 Func_02002f4c();
s32 Func_02002f52();
s32 Func_02002f70();
s32 Func_02002f7c();
s32 Func_02002f7e();
s32 Func_02002f90();
s32 Func_02002f92();
s32 Func_02002fa6();
s32 Func_02002fac();
s32 Func_02002fc2();
s32 Func_02002fcc();
s32 Func_02002fe2();
s32 Func_02002fe6();
s32 Func_02002ff0();
s32 Func_02002ff2();
s32 Func_02002ffc();
s32 Func_02003004();
s32 Func_02003020();
s32 Func_0200303e();
s32 Func_0200304c();
s32 Func_02003054();
s32 Func_02003056();
s32 Func_02003058();
s32 Func_02003060();
s32 Func_0200306e();
s32 Func_02003070();
s32 Func_0200307a();
s32 Func_0200308a();
s32 Func_020030a8();
s32 Func_020030aa();
s32 Func_020030ae();
s32 Func_020030b4();
s32 Func_020030b8();
s32 Func_020030c4();
s32 Func_020030d0();
s32 Func_020030d6();
s32 Func_020030dc();
s32 Func_020030de();
s32 Func_020030ea();
s32 Func_020030ec();
s32 Func_020030f4();
s32 Func_020030f8();
s32 Func_02003104();
s32 Func_0200310e();
s32 Func_02003118();
s32 Func_0200311a();
s32 Func_02003122();
s32 Func_0200312a();
s32 Func_02003130();
s32 Func_0200313e();
s32 Func_02003146();
s32 Func_0200314a();
s32 Func_0200314c();
s32 Func_02003156();
s32 Func_0200315c();
s32 Func_0200316a();
s32 Func_0200316c();
s32 Func_0200316e();
s32 Func_0200317a();
s32 Func_0200317c();
s32 Func_02003186();
s32 Func_0200318c();
s32 Func_02003194();
s32 Func_0200319c();
s32 Func_020031aa();
s32 Func_020031b0();
s32 Func_020031bc();
s32 Func_020031c6();
s32 Func_020031d0();
s32 Func_020031e0();
s32 Func_020031e8();
s32 Func_020031f0();

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

void Func_02000f8c(void)
{
    u8 *record;

    Value0(Func_02002b3c);
    Value0(Func_02002c38);
    Value0(Func_02002c4c);

    record = Value1(Func_02002b6a, 0);
    if (record != 0)
        Value3(Func_02002ba8, 8, *(s32 *)(record + 8), *(s32 *)(record + 16));
    record = Value1(Func_02002b7e, 0);
    if (record != 0)
        Value3(Func_02002bbc, 5, *(s32 *)(record + 8), *(s32 *)(record + 16));
    record = Value1(Func_02002b92, 0);
    if (record != 0)
        Value3(Func_02002bd0, 1, *(s32 *)(record + 8), *(s32 *)(record + 16));

    Call3(Func_02002bb2, 8, 0x9999, 0x4ccc);
    Call3(Func_02002bbc, 5, 0x9999, 0x4ccc);
    Call3(Func_02002bc6, 1, 0x9999, 0x4ccc);
    Call2(Func_02002bfe, 1, 2);
    Call2(Func_02002c06, 5, 2);
    Call2(Func_02002c0e, 8, 2);
    Call3(Func_02002c02, 1, -16, 0);
    Call3(Func_02002c0c, 5, 16, 0);
    Value3(Func_02002c18, 8, 0, -16);
    Value1(Func_02002c26, 8);
    Call2(Func_02002c3e, 8, 1);
    Call2(Func_02002c46, 0, 0);
    Call2(Func_02002c4e, 1, 0);
    Call2(Func_02002c56, 5, 0);
    Call3(Func_02002cba, 1, 0xe000, 0);
    Call3(Func_02002cc6, 5, 0xa000, 0);
    Call3(Func_02002cd2, 8, 0xc000, 30);
    Call3(Func_02002cde, 1, 0x8000, 0);
    Call3(Func_02002ce8, 5, 0, 0);
    Call3(Func_02002cf4, 0, 0x4000, 0);
    Call3(Func_02002d00, 8, 0x8000, 30);
    Call3(Func_02002d0c, 1, 0x4000, 0);
    Call3(Func_02002d18, 5, 0x8000, 0);
    Call3(Func_02002d22, 0, 0, 0);
    Call3(Func_02002d2e, 8, 0x4000, 30);
    Call3(Func_02002d3a, 1, 0xe000, 0);
    Call3(Func_02002d46, 5, 0xa000, 0);
    Call3(Func_02002d52, 0, 0xc000, 0);
    Call3(Func_02002d5e, 8, 0xc000, 40);
    Value2(Func_02002d2e, 8, 2);
    Value1(Func_02002cb4, 10);
    Call2(Func_02002d1c, 8, 2);
    Value3(Func_02002d10, 8, 0, -16);
    Value1(Func_02002d1e, 8);
    Value2(Func_02002d36, 8, 1);
    Value1(Func_02002cdc, 6);
    Call3(Func_02002da0, 8, 0x8000, 20);
    Call3(Func_02002daa, 8, 0, 20);
    Call3(Func_02002db6, 8, 0xc000, 40);
    Value2(Func_02002d86, 8, 2);
    Value1(Func_02002d0c, 20);
    Call2(Func_02002d74, 8, 2);
    Value3(Func_02002d68, 8, 0, -32);
    Value1(Func_02002d76, 8);
    Call2(Func_02002d8e, 8, 1);

    Call2(Func_02002e12, 0x20000, 0x4000);
    Call4(Func_02002e2a, 0x06310000, -1, 0x00960000, 1);
    Value0(Func_02002e36);
    Value1(Func_02002d54, 10);
    Call2(Func_02002e34, 0x13333, 0x2666);
    Call4(Func_02002e4c, 0x06550000, -1, 0x00640000, 1);
    Call0(Func_02002e58);
    Call4(Func_02002e60, 0x06b60000, -1, 0x00640000, 1);
    Call0(Func_02002e6c);
    Call2(Func_02002dec, 8, 1);
    Value4(Func_02002e7e, 0x06d80000, -1, 0x00960000, 1);
    Value0(Func_02002e8a);
    Value1(Func_02002da8, 40);
    Call2(Func_02002e88, 0x26666, 0x4ccc);
    Call4(Func_02002ea0, 0x06840000, -1, 0x01000000, 1);
    Call0(Func_02002eac);
    Value2(Func_02002e34, 8, 3);
    Value1(Func_02002dd2, 10);

    Call3(Func_02002e96, 1, 0xe000, 0);
    Call3(Func_02002ea2, 5, 0xa000, 10);
    Call3(Func_02002ebc, 1, 0x101, 20);
    Call1(Func_02002e8a, 0xfd6);
    Call3(Func_02002eac, 1, 0, 10);
    Call3(Func_02002ed8, 8, 0x102, 60);
    Call2(Func_02002e90, 8, 2);
    Call3(Func_02002eca, 8, 0, 10);
    Call2(Func_02002ea2, 0, 2);
    Call2(Func_02002eaa, 1, 2);
    Call2(Func_02002eb2, 5, 2);
    Call2(Func_02002f14, 0, 0x102);
    Call2(Func_02002f1e, 1, 0x102);
    Value2(Func_02002f28, 5, 0x102);
    Value1(Func_02002e5e, 40);
    Value2(Func_02002ee6, 8, 2);
    Value1(Func_02002e6c, 20);
    Call2(Func_02002f14, 8, 0);
    Call2(Func_02002ee4, 8, 4);
    Call3(Func_02002f2e, 8, 0, 10);
    Value3(Func_02002f18, 0, 5, 0);
    Value1(Func_02002e96, 40);
    Call2(Func_02002f16, 0, 1);
    Value2(Func_02002f26, 5, 1);
    Value1(Func_02002eac, 10);
    Call3(Func_02002f70, 0, 0xc000, 0);
    Call3(Func_02002f7c, 5, 0xa000, 20);
    Call2(Func_02002f4c, 5, 2);
    Call3(Func_02002f7e, 5, 0, 10);
    Call2(Func_02002f46, 8, 4);
    Call3(Func_02002f90, 8, 0, 10);
    Call3(Func_02002fac, 8, 0x3000, 10);
    Call3(Func_02002fa6, 8, 0, 40);
    Call3(Func_02002fc2, 8, 0xc000, 20);
    Value2(Func_02002f92, 8, 1);
    Value1(Func_02002f18, 10);
    Call2(Func_02002ff2, 8, 0x102);
    Call3(Func_02002fe6, 8, 0x8000, 20);
    Call3(Func_02002ff0, 8, 0, 20);
    Call3(Func_02002ffc, 8, 0xc000, 60);
    Value2(Func_02002fcc, 8, 2);
    Value1(Func_02002f52, 10);
    Call3(Func_02003004, 8, 0, 10);
    Call3(Func_02003020, 8, 0x4000, 20);
    Call3(Func_02002fe2, 8, 2, 20);

    Call3(Func_02003054, 8, 0, 40);
    Call3(Func_0200306e, 1, 0, 20);
    Value2(Func_02003056, 1, 0);
    if (Value2(Func_02002fe6, 0, 0) == 0) {
        Value1(Func_02003060, (s32)&Value_00000fe0);
        Call2(Func_02003058, 1, 1);
        Value3(Func_0200308a, 1, 0, 10);
    } else {
        Value1(Func_0200307a, 0xfe1);
        Call3(Func_020030ae, 5, 0x8000, 20);
        Call3(Func_020030a8, 5, 0, 10);
        Call3(Func_020030c4, 0, 0xc000, 0);
        Call3(Func_020030d0, 1, 0xe000, 0);
        Call3(Func_020030dc, 5, 0xa000, 60);
        Call3(Func_020030f8, 0, 0x102, 40);
        Value2(Func_020030b8, 1, 1);
        Value1(Func_0200303e, 10);
        Value2(Func_020030ae, 1, 3);
        Value1(Func_0200304c, 10);
        Call3(Func_0200310e, 1, 0, 0);
        Call3(Func_0200311a, 5, 0x8000, 30);
        Value2(Func_020030ea, 1, 1);
        Value1(Func_02003070, 10);
        Value3(Func_02003122, 1, 0, 10);
    }

    Call3(Func_020030b4, 8, 0x9999, 0x4ccc);
    Call2(Func_020030ec, 8, 2);
    Value3(Func_020030de, 8, 0, 48);
    Value1(Func_020030ec, 8);
    Value2(Func_02003104, 8, 1);
    Value1(Func_020030aa, 6);
    Call3(Func_0200316e, 1, 0xe000, 0);
    Value3(Func_0200317a, 5, 0xa000, 0);
    Value1(Func_02003118, 8);
    Value2(Func_02003130, 8, 1);
    Value1(Func_020030d6, 20);
    Call2(Func_0200313e, 1, 3);
    Call2(Func_02003146, 5, 3);
    Value2(Func_02003156, 0, 3);
    Value1(Func_020030f4, 6);
    Value2(Func_0200315c, 1, 2);

    record = Value1(Func_0200312a, 0);
    if (record != 0)
        Value3(Func_0200314c, 1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    Value2(Func_0200317c, 5, 2);
    record = Value1(Func_0200314a, 0);
    if (record != 0)
        Value3(Func_0200316c, 5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    Value2(Func_0200319c, 8, 2);
    record = Value1(Func_0200316a, 0);
    if (record != 0)
        Value3(Func_0200318c, 8, *(s16 *)(record + 10), *(s16 *)(record + 18));

    Value1(Func_020031aa, 8);
    Call3(Func_020031bc, 1, 0, 0);
    Call3(Func_020031c6, 5, 0, 0);
    Call3(Func_020031d0, 8, 0, 0);
    Call2(Func_020031e0, 8, 1);
    Call2(Func_020031e8, 1, 1);
    Value2(Func_020031f0, 5, 1);
    Value1(Func_02003186, 0x804);
    Value1(Func_02003194, 0x12f);
    Value0(Func_020031b0);
}


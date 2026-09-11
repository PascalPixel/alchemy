#include "object_runtime.h"

#define FieldScene_RunExtendedActorSequence Func_020008ec

void Func_02002a34();
s32 Func_02002a1c();
struct ObjectRuntime *Func_02002a54();
void Func_02002a0c();
void Func_02002b4c();
void Func_020029e4();
void Func_02002ab4();
void Func_0200299c();
void Func_02002b74();
void Func_02002b7c();
void Func_02002a64();
void Func_02002a9c();
void Func_02002a6c();
void Func_02002a74();
void Func_02002b24();
void Func_02002ae4();
void Func_02002a2c();
void Func_02002ad4();
void Func_02002b04();
void Func_020025a8();
void Func_02002ac4();
void Func_02002adc();
void Func_02002abc();
void Func_020008ac();
void Func_020008d8();
void Func_02002b34();
void Func_02002aec();
void Func_02002b3c();
void Func_020025c0();
s32 Func_02002b0c();
s32 Func_02002a4c();
void Func_02002bcc();
void Func_02002b64();
void Func_02002b6c();
s32 Func_020029a4();
void Func_02002aa4();
s32 Func_020029ac();
void Func_02002b2c();
void Func_02002a7c();
void Func_02002a84();
void Func_02002acc();
void Func_02002b14();
void Func_02002a8c();
void Func_02002aac();
s32 Func_02002a24();
void Func_02002bb4();
void Func_02002a3c();

/* The inline adapters retain argument lifetimes at constant-valued calls.
 * Their return types follow the target helpers. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ struct ObjectRuntime *Pointer1(struct ObjectRuntime *(*f)(), s32 a0)
{
    return f(a0);
}

extern s32 Data_0200b69c;

extern s32 Data_0200b68c;
extern s32 Data_0200b690;
extern s32 Data_0200b694;
extern s32 Data_0200b698;
extern s32 Data_0200b6a0;
extern u8 Data_02008801[];
extern u8 Data_0200a609[];
extern u8 Data_0200a7ad[];
extern u8 Data_0200abd4[];
extern u8 Data_0200ac08[];
extern u8 Data_0200ac3c[];
extern u8 Data_0200ac70[];
extern u8 Data_0200acfc[];
extern u8 Data_0200ad20[];
extern u8 Data_0200ad7c[];

void FieldScene_RunExtendedActorSequence(void)
{
    struct ObjectRuntime *record;
    s32 flag_addr;
    s32 mask;
    s32 value;
    s32 action_a;
    s32 action_b;
    s32 step_addr;
    s32 work_addr;
    s32 action_c;
    s32 action_d;
    s32 step_next;

    Func_02002a34();
    flag_addr = (s32)&Data_0200b69c;
    *(s32 *)flag_addr = Value1(Func_02002a1c, 3);
    record = Func_02002a54(19);
    Func_02002a0c((s32)record, 0);
    record = Func_02002a54(20);
    Func_02002a0c((s32)record, 0);
    record = Func_02002a54(21);
    Func_02002a0c((s32)record, 0);
    record = Func_02002a54(22);
    Func_02002a0c((s32)record, 0);
    Call4(Func_02002b4c, 0x680000, -1, 0x1000000, 0);
    Func_020029e4();
    Call3(Func_02002ab4, 0, 0x170000, 0xf70000);
    Func_0200299c(1);
    Func_02002b74();
    Func_02002b7c();
    Call3(Func_02002a64, 0, 0xcccc, 0x6666);
    Func_02002a9c(0, 121, 238);
    Call3(Func_02002a64, 1, 0x9999, 0x4ccc);
    Call3(Func_02002a64, 2, 0x9999, 0x4ccc);
    record = Pointer1(Func_02002a54, 0);
    if ((s32)record != 0) {
        Func_02002ab4(1, record->x, record->z);
    }
    record = Pointer1(Func_02002a54, 0);
    if ((s32)record != 0) {
        Func_02002ab4(2, record->x, record->z);
    }
    Call2(Func_02002a6c, 1, (s32)Data_0200abd4);
    Func_02002a6c(2, (s32)Data_0200ac08);
    if (*(s32 *)flag_addr != 0) {
        Call3(Func_02002a64, 3, 0x9999, 0x4ccc);
        record = Pointer1(Func_02002a54, 0);
        if ((s32)record != 0) {
            Func_02002ab4(3, record->x, record->z);
        }
        Func_02002a6c(3, (s32)Data_0200ac3c);
    }
    Func_02002a74(2);
    Call3(Func_02002b24, 0, 0xe000, 20);
    Call3(Func_02002b24, 2, 0xe000, 20);
    Call3(Func_02002b24, 1, 0x2000, 20);
    flag_addr = (s32)&Data_0200b69c;
    if (*(s32 *)flag_addr != 0) {
        Call3(Func_02002b24, 3, 0x2000, 0);
    }
    Call3(Func_02002b24, 0, 0x2000, 20);
    Call3(Func_02002b24, 2, 0x2000, 20);
    Call3(Func_02002b24, 1, 0xe000, 60);
    if (*(s32 *)flag_addr != 0) {
        Call3(Func_02002b24, 3, 0xe000, 0);
    }
    Call3(Func_02002b24, 0, 0x8000, 0);
    Func_02002ae4(2, 0, 0);
    Func_02002ae4(1, 0, 0);
    if (*(s32 *)flag_addr != 0) {
        Func_02002ae4(3, 0, 0);
    }
    Func_02002a2c(40);
    Func_02002ad4(1, 1);
    Call1(Func_02002b04, 0x1473);
    Func_020025a8(1, 20);
    Func_02002ae4(0, 1, 0);
    Func_02002ac4(0, 3);
    Func_02002a2c(30);
    Call3(Func_02002a9c, 2, 72, 0x11e);
    Call3(Func_02002a9c, 2, 72, 0x12e);
    Call3(Func_02002a9c, 2, 88, 0x136);
    Func_02002adc(2, 1);
    Func_02002ae4(2, 0, 0);
    Func_02002a2c(20);
    Func_02002ae4(0, 2, 0);
    Func_02002ae4(1, 2, 0);
    if (*(s32 *)flag_addr != 0) {
        Func_02002ae4(3, 2, 0);
    }
    Func_02002a2c(30);
    Func_02002abc(1, 3);
    if (*(s32 *)flag_addr != 0) {
        Func_02002abc(3, 3);
    }
    Func_02002ac4(0, 3);
    Func_02002a2c(20);
    Func_02002ac4(2, 3);
    Func_02002a2c(30);
    Func_02002b24(2, 0, 0);
    Func_02002a2c(20);
    Func_020008ac(2, 9);
    Func_02002a2c(40);
    Func_020008d8();
    Call3(Func_02002b34, 2, 0x100, 40);
    Call3(Func_02002a64, 2, 0x10000, 0x8000);
    Func_02002a54(2)->action_flags &= 254;
    Call3(Func_02002a9c, 2, 80, 0x136);
    mask = 1;
    Func_02002a2c(1);
    Func_02002a54(2)->action_flags |= mask;
    Call3(Func_02002b34, 1, 0x102, 40);
    Func_020025a8(1, 20);
    Call3(Func_02002b34, 2, 0x102, 40);
    Func_020025a8(2, 20);
    if (*(s32 *)flag_addr != 0) {
        Func_02002ae4(3, 0, 0);
    }
    Func_02002aec(2, 1, 60);
    if (*(s32 *)flag_addr != 0) {
        Call3(Func_02002b24, 3, 0xc000, 0);
    }
    Call3(Func_02002b24, 1, 0xc000, 0);
    Call3(Func_02002b24, 0, 0xe000, 0);
    Func_02002ad4(1, 1);
    Func_020025a8(1, 20);
    Call3(Func_02002a64, 2, 0x8000, 0x4000);
    Func_02002a54(2)->action_flags &= 254;
    Call3(Func_02002a9c, 2, 72, 0x11e);
    Func_02002a2c(1);
    Func_02002a54(2)->action_flags |= mask;
    Call2(Func_02002a6c, 2, (s32)Data_0200ac08);
    if (*(s32 *)flag_addr != 0) {
        Call3(Func_02002b34, 3, 0x105, 0);
        Func_02002a2c(60);
        Func_020025a8(3, 20);
        Func_02002abc(3, 3);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02002ae4(2, 0, 0);
    Func_02002abc(0, 3);
    Func_02002abc(1, 3);
    Func_02002ac4(2, 3);
    Func_02002a2c(20);
    Call2(Func_02002b3c, 1, 0x102);
    Func_02002a2c(10);
    Func_020025c0(1, 0x2000, 10);
    Func_020025c0(0, 0xa000, 10);
    Value2(Func_02002b0c, 1, 0);
    if (Value2(Func_02002a4c, 0, 0) == 0) {
        Func_02002a2c(20);
        Func_02002abc(1, 3);
    } else {
        Func_02002ac4(1, 4);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_020025a8(1, 40);
    Func_020025c0(2, 0x2000, 40);
    Func_020025c0(2, 0x8000, 20);
    Func_020025c0(2, 0x4000, 40);
    Call3(Func_02002b34, 2, 0x101, 0);
    Func_02002a2c(60);
    Func_02002b24(1, 0x4000, 0);
    Func_020025c0(0, 0x6000, 60);
    value = 160;
    Func_020025c0(3, 0x2000, 10);
    Func_02002b24(1, 0x2000, 0);
    Func_020025c0(0, (value << 8), 10);
    Call3(Func_02002b34, 1, 0x101, 0);
    Call3(Func_02002b34, 0, 0x101, 0);
    Func_02002a2c(40);
    Func_02002b24(1, 0x4000, 0);
    Func_020025c0(0, 0x6000, 10);
    Func_02002adc(1, 2);
    Call1(Func_02002b04, 0x147b);
    Func_020025a8(1, 10);
    Func_020025c0(2, 0xc000, 20);
    Func_02002ac4(2, 3);
    Func_020025a8(2, 20);
    Func_020025c0(1, 0, 20);
    Func_020025c0(0, (value << 8), 40);
    Func_020025c0(1, 0x4000, 20);
    Func_020025c0(0, 0x6000, 30);
    Func_020025c0(1, 0x6000, 20);
    Func_020025c0(0, 0xe000, 30);
    Func_02002adc(2, 2);
    Call3(Func_02002b34, 2, 0x100, 0);
    Func_02002a2c(40);
    Func_02002b24(1, 0x4000, 0);
    Func_020025c0(0, 0x6000, 20);
    Func_020025c0(2, 0xc000, 10);
    Func_02002bcc(17);
    Func_02002bcc(206);
    Call2(Func_02002b64, 0x7fff, 0);
    Func_02002b6c(1);
    Func_0200299c(1);
    Data_0200b6a0 = 1;
    Value2(Func_020029a4, (s32)Data_0200a609, 0xc80);
    Func_0200299c(20);
    Call2(Func_02002b64, 0x405210, 1);
    Call2(Func_02002b64, 0x10000, 2);
    Func_02002b6c(120);
    Func_0200299c(60);
    action_a = (s32)Data_0200ac70;
    Func_02002a6c(0, action_a);
    Func_02002a6c(1, action_a);
    Func_02002a6c(2, action_a);
    Func_02002a6c(3, action_a);
    Func_02002a2c(100);
    Func_020025a8(1, 20);
    Func_020025a8(2, 40);
    if (Data_0200b69c != 0) {
        Func_02002a2c(40);
        Call2(Func_02002b3c, 3, 0x102);
        Func_02002a2c(40);
        Func_020025a8(3, 40);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02002a2c(20);
    if (Data_0200b69c != 0) {
        value = 128;
        record = Func_02002a54(3);
        record->velocity_y = (value << 10);
        Func_02002a2c(10);
        Func_02002a64(3, (value << 10), (value << 10));
        Call3(Func_02002aa4, 3, -2, 0);
        Call2(Func_02002a6c, 3, (s32)Data_0200acfc);
        record = Func_02002a54(3);
        Func_02002a0c((s32)record, 0);
        Func_02002abc(3, 19);
        Func_02002a2c(10);
    }
    value = 128;
    record = Func_02002a54(0);
    record->velocity_y = (value << 10);
    Func_02002a2c(10);
    Call3(Func_02002a64, 0, (value << 10), (value << 10));
    action_b = (s32)Data_0200acfc;
    Func_02002a6c(0, action_b);
    record = Func_02002a54(0);
    Func_02002a0c((s32)record, 0);
    Func_02002abc(0, 19);
    Func_02002a2c(20);
    record = Pointer1(Func_02002a54, 1);
    record->velocity_y = (value << 10);
    Func_02002a2c(10);
    Call3(Func_02002a64, 1, (value << 10), (value << 10));
    Func_02002a6c(1, action_b);
    record = Func_02002a54(1);
    Func_02002a0c((s32)record, 0);
    Func_02002abc(1, 19);
    Func_02002a2c(40);
    record = Pointer1(Func_02002a54, 2);
    record->velocity_y = (value << 10);
    Func_02002a2c(10);
    Func_02002a6c(2, action_b);
    record = Func_02002a54(2);
    Func_02002a0c((s32)record, 0);
    Func_02002abc(2, 19);
    Data_0200b6a0 = 0;
    Func_02002a2c(160);
    Value1(Func_020029ac, (s32)Data_0200a609);
    Func_02002a2c(120);
    Call2(Func_02002b64, 0x406218, 1);
    Func_02002b6c(60);
    Func_0200299c(60);
    Data_0200b690 = 0;
    step_addr = (s32)&Data_0200b694;
    Data_0200b68c = 0x800000;
    *(s32 *)step_addr = 1;
    Value2(Func_020029a4, (s32)Data_0200a7ad, 0xc80);
    Func_02002a2c(180);
    Func_02002bcc(21);
    Func_020025a8(1, 80);
    Func_020025a8(2, 40);
    Call2(Func_02002b3c, 0, 0x102);
    Call2(Func_02002b3c, 1, 0x102);
    Call2(Func_02002b3c, 2, 0x102);
    Call2(Func_02002b3c, 3, 0x102);
    Func_02002a2c(60);
    Func_020025a8(2, 20);
    *(s32 *)step_addr = 2;
    Func_02002ad4(2, 2);
    Func_02002a2c(20);
    Func_02002ad4(1, 1);
    Func_02002a2c(40);
    Func_02002ad4(0, 2);
    Func_02002ad4(3, 1);
    Func_02002a2c(20);
    Func_02002ad4(2, 3);
    Func_02002a2c(40);
    Func_02002ad4(0, 1);
    Func_02002a2c(20);
    Call2(Func_02002ad4, 1, 2);
    Func_02002a2c(20);
    Func_02002ad4(3, 2);
    Call2(Func_02002b3c, 1, 0x102);
    Func_020025a8(1, 20);
    if (Data_0200b69c != 0) {
        Call2(Func_02002b3c, 3, 0x102);
        Func_020025a8(3, 10);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    work_addr = (s32)&Data_0200b694;
    *(s32 *)work_addr = 3;
    Func_02002a54(0)->unknown_23 &= 254;
    Func_02002a54(1)->unknown_23 &= 254;
    Func_02002a54(2)->unknown_23 &= 254;
    Func_02002a54(3)->unknown_23 &= 254;
    Func_02002b2c(0, 3);
    Func_02002b2c(1, 3);
    Func_02002b2c(2, 3);
    value = 0;
    Func_02002b2c(3, 3);
    Data_0200b698 = value;
    Value2(Func_020029a4, (s32)Data_02008801, 0xc80);
    Func_02002bcc(220);
    Func_02002a54(19)->unknown_23 &= 254;
    Func_02002b2c(19, 2);
    Call3(Func_02002ab4, 19, 0x780000, 0xf80000);
    action_c = (s32)Data_0200ad20;
    Func_02002a6c(19, action_c);
    Func_02002a54(20)->unknown_23 &= 254;
    Func_02002b2c(20, 2);
    Call3(Func_02002ab4, 20, 0x640000, 0x1120000);
    Func_02002a6c(20, action_c);
    if (Data_0200b69c != 0) {
        Func_02002a54(21)->unknown_23 &= 254;
        Func_02002b2c(21, 2);
        Call3(Func_02002ab4, 21, 0x4a0000, 0xfe0000);
        Func_02002a6c(21, action_c);
    }
    Func_02002a54(22)->unknown_23 &= 254;
    Func_02002b2c(22, 2);
    Call3(Func_02002ab4, 22, 0x5e0000, 0xe10000);
    Func_02002a6c(22, action_c);
    if (*(s32 *)work_addr != 0) {
        do {
            Func_0200299c(1);
        } while (Data_0200b694 != 0);
    }
    Call1(Func_02002a2c, 0x12c);
    Value1(Func_020029ac, (s32)Data_0200a7ad);
    Func_02002a2c(120);
    Func_02002bcc(17);
    Call2(Func_02002b64, 0x10000, 1);
    Func_02002b6c(60);
    Func_0200299c(60);
    Func_02002a7c(19);
    Func_02002a7c(20);
    if (Data_0200b69c != 0) {
        Func_02002a7c(21);
    }
    (Func_02002a7c)(22);
    Func_0200299c(1);
    action_d = (s32)Data_0200ad7c;
    (Func_02002a6c)(19, action_d);
    Func_02002a6c(20, action_d);
    if (Data_0200b69c != 0) {
        Func_02002a6c(21, action_d);
    }
    Func_02002a84(22, action_d);
    Func_02002a2c(80);
    Func_02002adc(1, 2);
    Func_02002a2c(40);
    Value2(Func_02002b0c, 1, 0);
    Call3(Func_02002ab4, 17, 0x570000, 0x8b0000);
    Call3(Func_02002ab4, 18, 0x570000, 0x8b0000);
    Func_0200299c(1);
    if (Value2(Func_02002a4c, 17, 0) == 1) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02002adc(0, 1);
    Func_02002a2c(20);
    Call2(Func_02002adc, 2, 2);
    Func_020025a8(2, 20);
    if (Data_0200b69c != 0) {
        Func_02002adc(3, 2);
        Func_02002a2c(10);
        Call1(Func_02002b04, 0x1488);
        Func_020025a8(3, 40);
    }
    Func_02002ad4(1, 1);
    Call3(Func_02002b34, 1, 0x101, 0);
    Func_02002a2c(80);
    Func_02002adc(2, 2);
    Call1(Func_02002b04, 0x1489);
    Func_020025a8(2, 40);
    Func_02002adc(1, 3);
    Func_02002a2c(40);
    Func_02002b2c(1, 2);
    Func_02002a54(1)->unknown_23 |= 1;
    record = Func_02002a54(1);
    Func_02002a0c((s32)record, 1);
    Func_02002acc(1, 6, 0);
    Func_02002abc(1, 1);
    Func_020025c0(1, 0x4000, 60);
    Func_020025a8(1, 20);
    Func_02002ad4(1, 2);
    (Func_020025a8)(1, 10);
    Func_02002adc(0, 3);
    Func_020025c0(1, 0x2000, 20);
    Call3(Func_02002b34, 1, 0x101, 0);
    Func_02002a2c(40);
    Func_020025c0(1, 0x6000, 40);
    Func_020025c0(1, 0x2000, 20);
    Func_020025c0(1, 0x6000, 20);
    Func_020025c0(1, 0x2000, 10);
    Func_02002acc(1, 2, 0);
    Func_02002a2c(40);
    Func_02002acc(1, 2, 0);
    Func_02002a2c(10);
    Func_02002acc(1, 4, 0);
    Func_02002a2c(20);
    Func_020025a8(1, 20);
    if (Data_0200b69c != 0) {
        Call3(Func_02002b34, 3, 0x100, 0);
        Func_02002a2c(60);
        Func_02002adc(3, 2);
        Func_02002a2c(80);
        Func_02002b2c(3, 2);
        Func_02002a54(3)->unknown_23 |= 1;
        record = Func_02002a54(3);
        Func_02002a0c((s32)record, 1);
        Func_02002acc(3, 4, 0);
        Call3(Func_02002aa4, 3, -2, 0);
        Func_02002abc(3, 1);
        Func_020025c0(3, 0xe000, 60);
        Func_02002adc(3, 2);
        Func_02002a2c(20);
        Func_020025a8(3, 20);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02002acc(1, 2, 0);
    Func_020025c0(1, 0x4000, 20);
    Func_02002ac4(1, 3);
    Func_020025c0(1, 0x2000, 10);
    Func_020025a8(1, 20);
    Func_02002ac4(1, 3);
    Func_02002a2c(10);
    Func_02002adc(2, 1);
    Func_02002a2c(40);
    Func_02002adc(2, 2);
    Func_02002a2c(20);
    value = 1;
    Func_02002b2c(2, 2);
    Func_02002a54(2)->unknown_23 |= value;
    record = Func_02002a54(2);
    Func_02002a0c((s32)record, 1);
    Func_02002acc(2, 4, 0);
    Func_02002abc(2, 1);
    Call3(Func_02002b24, 2, 0xc000, 0);
    Func_02002adc(0, 2);
    Func_02002a2c(10);
    Func_02002b2c(0, 2);
    Func_02002a54(0)->unknown_23 |= value;
    record = Func_02002a54(0);
    Func_02002a0c((s32)record, 1);
    Func_02002acc(0, 4, 0);
    Func_02002abc(0, 1);
    Func_020025c0(0, 0x6000, 60);
    Call3(Func_02002b34, 0, 0x105, 0);
    Call3(Func_02002b34, 2, 0x105, 0);
    Func_02002a2c(60);
    Func_020025c0(0, 0xa000, 20);
    Func_02002ac4(1, 3);
    Func_02002ac4(0, 3);
    Func_020025c0(0, 0x6000, 10);
    Func_020025c0(1, 0x4000, 10);
    Call3(Func_02002b24, 2, 0xc000, 0);
    Func_02002ac4(2, 3);
    Func_020025a8(2, 20);
    Func_02002abc(0, 3);
    Func_02002abc(3, 3);
    Func_02002ac4(1, 3);
    Func_02002a2c(20);
    Func_02002adc(2, 1);
    Func_02002a2c(20);
    Value2(Func_02002b0c, 2, 0);
    Func_02002abc(2, 3);
    Call3(Func_02002b24, 2, 0xe000, 0);
    Func_02002b24(1, 0x2000, 0);
    if (Value2(Func_02002a4c, 0, 0) == 0) {
        Func_02002abc(2, 3);
        Func_02002ac4(1, 3);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    } else {
        Func_02002adc(1, 2);
        Func_020025c0(1, 0x2000, 10);
        Func_02002ad4(1, 2);
        Func_02002b14(1, 0);
    }
    Func_020025c0(1, 0x4000, 10);
    Func_02002ac4(1, 4);
    Func_020025a8(1, 20);
    Func_020025c0(2, 0xc000, 10);
    Func_02002ac4(2, 3);
    Func_020025a8(2, 10);
    if (Data_0200b69c != 0) {
        Func_02002adc(3, 2);
        Func_020025c0(3, 0, 20);
        Func_020025c0(3, 0x2000, 10);
        Func_02002abc(3, 4);
        Func_020025a8(3, 10);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Call3(Func_02002b24, 1, 0x2000, 0);
    Func_020025c0(0, 0xa000, 10);
    Func_02002abc(0, 3);
    Func_02002ac4(1, 3);
    value = 128;
    Func_02002a2c(20);
    Call3(Func_02002b24, 0, 0x6000, 0);
    Func_020025c0(1, (value << 7), 10);
    Func_02002ac4(2, 4);
    Func_02002a2c(20);
    Call3(Func_02002b34, 0, 0x102, 0);
    Call3(Func_02002b34, 1, 0x102, 80);
    Func_020025c0(2, 0xe000, 10);
    Func_02002ad4(2, 2);
    Func_020025a8(2, 20);
    Call3(Func_02002b24, 1, 0x2000, 0);
    Func_020025c0(0, 0xa000, 40);
    Func_02002b24(1, (value << 7), 0);
    Func_020025c0(0, 0x6000, 10);
    Func_020025c0(2, 0xc000, 10);
    Func_02002ac4(2, 3);
    Func_020025a8(2, 10);
    Call2(Func_02002b3c, 1, 0x102);
    Func_02002a2c(40);
    Func_020025a8(1, 20);
    Func_02002ac4(2, 3);
    Func_02002a2c(20);
    Call2(Func_02002b3c, 1, 0x102);
    Func_02002a2c(40);
    Func_020025a8(1, 20);
    Func_02002abc(2, 3);
    Func_020025a8(2, 10);
    Func_02002adc(1, 2);
    Func_020025c0(1, 0x2000, 10);
    Value2(Func_02002b0c, 1, 0);
    Func_02002b24(0, 0xa000, 0);
    if (Value2(Func_02002a4c, 0, 0) == 0) {
        Func_02002ac4(1, 3);
    } else {
        Func_02002a2c(20);
        Func_02002adc(1, 2);
        Func_02002a2c(40);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02002b14(1, 0);
    Func_02002bcc(21);
    Call2(Func_02002b64, 0x406218, 1);
    Func_02002b6c(60);
    Func_0200299c(60);
    Data_0200b690 = 0;
    Data_0200b68c = 0x800000;
    step_next = (s32)&Data_0200b694;
    *(s32 *)step_next = 1;
    Value2(Func_020029a4, (s32)Data_0200a7ad, 0xc80);
    Func_02002a2c(80);
    Func_02002ad4(0, 2);
    Call2(Func_02002ad4, 1, 2);
    Func_02002ad4(3, 2);
    Func_02002adc(2, 2);
    Func_02002a2c(60);
    Func_020025c0(2, 0xc000, 10);
    Call1(Func_02002b04, 0x149d);
    Func_020025a8(2, 10);
    Func_020025c0(1, 0xc000, 10);
    Func_020025c0(0, 0xc000, 10);
    if (Data_0200b69c != 0) {
        Func_020025c0(3, 0xc000, 10);
    }
    Func_02002a54(0)->unknown_23 &= 254;
    Func_02002a54(1)->unknown_23 &= 254;
    Func_02002a54(2)->unknown_23 &= 254;
    Func_02002a54(3)->unknown_23 &= 254;
    Func_02002b2c(0, 3);
    Func_02002b2c(1, 3);
    Func_02002b2c(2, 3);
    Func_02002b2c(3, 3);
    *(s32 *)step_next = 2;
    Func_02002bcc(220);
    Call3(Func_02002ab4, 19, 0x780000, 0xf80000);
    action_c = (s32)Data_0200ad20;
    Func_02002a6c(19, action_c);
    Call3(Func_02002ab4, 20, 0x640000, 0x1120000);
    Func_02002a6c(20, action_c);
    if (Data_0200b69c != 0) {
        Call3(Func_02002ab4, 21, 0x4a0000, 0xfe0000);
        Func_02002a6c(21, action_c);
    }
    Call3(Func_02002ab4, 22, 0x5e0000, 0xe10000);
    Call2(Func_02002a6c, 22, action_c);
    Func_02002a2c(120);
    *(s32 *)step_next = 3;
    do {
        Func_0200299c(1);
    } while (Data_0200b694 != 0);
    Func_020025a8(17, 80);
    Func_020025a8(18, 20);
    Call3(Func_02002b34, 0, 0x101, 0);
    Call3(Func_02002b34, 1, 0x101, 0);
    Call3(Func_02002b34, 2, 0x101, 0);
    Call3(Func_02002b34, 3, 0x101, 0);
    Func_02002a2c(60);
    Func_020025a8(18, 20);
    Func_02002abc(0, 3);
    Func_02002abc(1, 3);
    Func_02002abc(3, 3);
    Func_02002ac4(2, 3);
    Func_020025a8(18, 10);
    Func_02002abc(0, 3);
    Func_02002abc(1, 3);
    Func_02002abc(3, 3);
    Func_02002ac4(2, 3);
    Func_020025a8(18, 10);
    Call3(Func_02002b34, 0, 0x100, 0);
    Call3(Func_02002b34, 1, 0x100, 0);
    Call3(Func_02002b34, 2, 0x100, 0);
    Call3(Func_02002b34, 3, 0x100, 0);
    Func_02002a2c(40);
    Func_020025a8(17, 10);
    Call3(Func_02002b24, 0, 0x8000, 0);
    Call3(Func_02002b24, 1, 0x4000, 0);
    Func_02002b24(3, 0, 0);
    Func_020025c0(2, 0xc000, 40);
    Func_020025a8(18, 10);
    Call2(Func_02002b3c, 0, 0x102);
    Call2(Func_02002b3c, 1, 0x102);
    Call2(Func_02002b3c, 2, 0x102);
    Call2(Func_02002b3c, 3, 0x102);
    Func_02002b24(0, 0xc000, 0);
    Func_02002b24(1, 0xc000, 0);
    Func_02002b24(2, 0xc000, 0);
    Func_020025c0(3, 0xc000, 80);
    Func_020025a8(18, 10);
    Call3(Func_02002b24, 0, 0x8000, 0);
    Call3(Func_02002b24, 1, 0x4000, 0);
    Func_02002b24(2, 0xc000, 0);
    Func_020025c0(3, 0, 40);
    Func_020025a8(17, 10);
    Func_02002b24(0, 0xc000, 0);
    Func_02002b24(1, 0xc000, 0);
    Func_02002b24(2, 0xc000, 0);
    Func_020025c0(3, 0xc000, 10);
    Func_02002abc(0, 4);
    Func_02002abc(1, 4);
    Func_02002abc(3, 4);
    Func_02002ac4(2, 4);
    Func_02002a2c(60);
    Func_020025a8(18, 10);
    Func_02002abc(0, 3);
    Func_02002abc(1, 3);
    Func_02002abc(3, 3);
    Func_02002ac4(2, 3);
    Func_020025a8(18, 20);
    Call3(Func_02002b24, 0, 0x8000, 0);
    Call3(Func_02002b24, 1, 0x4000, 0);
    Func_02002b24(2, 0xc000, 0);
    Func_020025c0(3, 0, 20);
    Func_020025a8(18, 10);
    Func_02002ad4(0, 2);
    Func_02002ad4(1, 2);
    Func_02002ad4(3, 2);
    Func_02002adc(2, 2);
    Func_02002b24(0, 0xc000, 0);
    Func_02002b24(1, 0xc000, 0);
    Func_02002b24(2, 0xc000, 0);
    Func_02002b24(3, 0xc000, 0);
    Func_020025a8(18, 20);
    Call3(Func_02002b24, 0, 0x8000, 0);
    Call3(Func_02002b24, 1, 0x4000, 0);
    Func_02002b24(2, 0xc000, 0);
    Func_020025c0(3, 0, 20);
    Func_020025a8(17, 20);
    Call2(Func_02002b3c, 0, 0x102);
    Call2(Func_02002b3c, 1, 0x102);
    Call2(Func_02002b3c, 3, 0x102);
    Call2(Func_02002b3c, 2, 0x102);
    Func_02002a2c(40);
    Func_020025a8(18, 10);
    Func_02002b24(0, 0xc000, 0);
    Func_02002b24(1, 0xc000, 0);
    Func_02002b24(2, 0xc000, 0);
    Func_020025c0(3, 0xc000, 10);
    Func_020025a8(18, 10);
    Func_02002abc(0, 3);
    Func_02002abc(1, 3);
    Func_02002abc(3, 3);
    Func_02002ac4(2, 3);
    Func_02002a2c(60);
    Func_02002b14(18, 0);
    Func_02002b14(17, 0);
    Value1(Func_020029ac, (s32)Data_0200a7ad);
    Func_02002a2c(80);
    Call2(Func_02002b64, 0x10000, 1);
    Func_02002b6c(60);
    Func_0200299c(80);
    Func_02002a7c(19);
    Func_02002a7c(20);
    work_addr = (s32)&Data_0200b69c;
    Func_02002a7c(21);
    Func_02002a7c(22);
    Func_0200299c(1);
    action_d = (s32)Data_0200ad7c;
    Func_02002a6c(19, action_d);
    Func_02002a6c(20, action_d);
    if (*(s32 *)work_addr != 0) {
        Func_02002a6c(21, action_d);
    }
    Func_02002a84(22, action_d);
    Func_02002a2c(20);
    Func_02002b2c(0, 2);
    Func_02002b2c(1, 2);
    Func_02002b2c(2, 2);
    value = 1;
    Func_02002b2c(3, 2);
    Func_02002a54(0)->unknown_23 |= value;
    Func_02002a54(1)->unknown_23 |= value;
    Func_02002a54(2)->unknown_23 |= value;
    Func_02002a54(3)->unknown_23 |= value;
    Func_02002adc(2, 2);
    Func_020025c0(2, 0xe000, 10);
    Value2(Func_02002b0c, 2, 0);
    Call3(Func_02002b24, 1, 0x2000, 0);
    Func_02002b24(3, 0, 0);
    if (Value2(Func_02002a4c, 0, 0) == 0) {
        Func_02002adc(1, 2);
        Func_02002a2c(10);
        Value2(Func_02002b0c, 1, 0);
        if (Value2(Func_02002a4c, 0, 0) == 0) {
            Func_020025c0(3, 0, 20);
            Call3(Func_02002b34, 1, 0x101, 0);
            Call3(Func_02002b34, 2, 0x101, 0);
            Call3(Func_02002b34, 3, 0x101, 0);
            Func_02002a2c(40);
            Func_020025c0(1, 0x4000, 20);
            Func_020025a8(1, 10);
            Func_020025c0(2, 0xc000, 20);
            Func_020025c0(2, 0xe000, 20);
            Func_02002ac4(2, 3);
            Func_020025a8(2, 20);
            Func_020025c0(1, 0x2000, 20);
        } else {
            Func_020025c0(3, 0, 20);
            Call3(Func_02002b34, 1, 0x102, 0);
            Call3(Func_02002b34, 2, 0x102, 0);
            Call3(Func_02002b34, 3, 0x102, 40);
            Func_020025c0(1, 0x4000, 20);
            Call1(Func_02002b04, 0x14b4);
            Func_020025a8(1, 20);
            Func_02002ac4(2, 3);
            Func_020025a8(2, 20);
        }
        Func_02002abc(3, 3);
        Func_02002ac4(1, 3);
    } else {
        Func_02002a2c(20);
        Func_02002ac4(1, 3);
        Func_02002a2c(10);
        Call1(Func_02002b04, 0x14b6);
        Func_020025a8(1, 10);
        Call3(Func_02002b24, 1, 0x4000, 0);
        Func_020025c0(0, 0x6000, 20);
        Func_02002abc(1, 3);
        Func_02002ac4(0, 3);
        Func_02002a2c(10);
        Func_02002ac4(2, 4);
        Value2(Func_02002b0c, 2, 0);
        if (Value2(Func_02002a4c, 0, 0) != 0) {
            goto L_0200239c;
        }
        Func_02002a2c(20);
        Call3(Func_02002b34, 2, 0x103, 0);
        Func_02002a2c(40);
        Func_020025c0(2, 0xe000, 10);
        Func_020025a8(2, 10);
        if (Data_0200b69c != 0) {
            Func_020025c0(3, 0, 10);
            Func_02002ad4(3, 3);
            Func_020025a8(3, 20);
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        Call2(Func_02002b3c, 0, 0x102);
        Call2(Func_02002b3c, 1, 0x102);
        Func_02002a2c(40);
        Func_02002adc(1, 2);
        Func_020025a8(1, 20);
        Call3(Func_02002b34, 1, 0x105, 0);
        Func_02002a2c(120);
        Func_020025a8(2, 40);
        if (Data_0200b69c != 0) {
            Func_020025c0(3, 0x2000, 10);
            Func_02002ac4(3, 4);
            Func_020025a8(3, 10);
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        Func_02002a2c(60);
        Func_02002adc(2, 2);
        if (Data_0200b69c != 0) {
            Func_020025c0(2, 0xa000, 40);
            Func_020025c0(2, 0xe000, 20);
        }
        Func_020025a8(2, 10);
        Func_02002ad4(0, 2);
        Func_02002adc(1, 2);
        Func_02002a2c(40);
        Func_020025a8(2, 20);
        Func_02002ac4(0, 3);
        Func_02002ac4(1, 3);
        Func_02002a2c(20);
        Func_02002abc(3, 3);
    }
    Func_02002ac4(2, 3);
    goto L_020024a0;
    L_0200239c:;
    Call3(Func_02002b34, 2, 0x105, 0);
    Func_02002a2c(40);
    Func_02002ac4(2, 3);
    Call1(Func_02002b04, 0x14bf);
    Func_020025a8(2, 20);
    if (Data_0200b69c != 0) {
        Func_020025c0(3, 0, 10);
        Func_02002ad4(3, 1);
        Func_020025a8(3, 20);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Call2(Func_02002b3c, 1, 0x102);
    Call2(Func_02002b3c, 0, 0x102);
    Func_02002a2c(40);
    Func_02002adc(1, 2);
    Func_020025a8(1, 20);
    Call3(Func_02002b34, 2, 0x105, 0);
    Func_02002a2c(80);
    Func_020025a8(2, 40);
    if (Data_0200b69c != 0) {
        Func_020025c0(3, 0x2000, 20);
        Func_02002abc(3, 4);
        Func_020025a8(3, 40);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02002adc(2, 2);
    Func_02002a2c(20);
    Func_020025a8(2, 20);
    Func_02002ad4(1, 2);
    Func_02002adc(0, 2);
    Func_02002a2c(40);
    Func_020025a8(2, 20);
    L_020024a0:;
    Func_02002bcc(17);
    Call3(Func_02002a64, 1, 0x13333, 0x9999);
    Call3(Func_02002a64, 2, 0x13333, 0x9999);
    Call3(Func_02002a64, 3, 0x13333, 0x9999);
    Func_02002abc(1, 2);
    record = Pointer1(Func_02002a54, 0);
    if ((s32)record != 0) {
        Func_02002a8c(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Func_02002aac(1);
    Func_02002ab4(1, 0, 0);
    Func_02002abc(2, 2);
    record = Pointer1(Func_02002a54, 0);
    if ((s32)record != 0) {
        Func_02002a8c(2, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
    }
    Func_02002aac(2);
    Func_02002ab4(2, 0, 0);
    Func_02002ab4(17, 0, 0);
    Func_02002ab4(18, 0, 0);
    if (Data_0200b69c != 0) {
        Func_02002abc(3, 2);
        record = Pointer1(Func_02002a54, 0);
        if ((s32)record != 0) {
            Func_02002a8c(3, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
        Func_02002aac(3);
        Func_02002ab4(3, 0, 0);
    }
    Value1(Func_02002a24, 0x843);
    Func_02002bb4();
    Func_02002a3c();
}

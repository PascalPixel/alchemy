#include "types.h"

#define FieldScene_RunBranchingFormationPresentation Func_02000d3c

void Func_02002c0c();
void Func_02002cec();
void Func_02002b84();
s32 Func_02002bf4();
u8 * Func_02002c24();
void Func_02002bec();
void Func_02002c34();
void Func_02002c7c();
void Func_02002bc4();
void Func_02002d04();
void Func_02002d0c();
void Func_02002c2c();
void Func_02002c64();
void Func_02002c3c();
void Func_02002780();
void Func_02002cdc();
void Func_02002c04();
void Func_02002ccc();
void Func_02002ca4();
void Func_02002cb4();
void Func_02002768();
void Func_02002c8c();
void Func_02002d24();
void Func_02002cf4();
void Func_02002cfc();
s32 Func_02002b8c();
void Func_02002c6c();
void Func_02002c84();
s32 Func_02002b94();
void Func_02002c9c();
void Func_02002cd4();
void Func_02002c44();
void Func_02002c4c();
s32 Func_02002cbc();
s32 Func_02002c1c();
void Func_02002c94();
void Func_02002cc4();
void Func_02002ce4();
void Func_02002c54();
void Func_02002c74();
s32 Func_02002bfc();
void Func_02002d14();
void Func_02002c14();

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

static __inline__ u8 * Record1(u8 * (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

void FieldScene_RunBranchingFormationPresentation(void)
{
    u8 *record;
    s32 *flag_work;
    s32 entry_action;
    s32 value;
    s32 flag;
    s32 motion_action;
    s32 *party_flag;
    s32 reset_action;
    s32 *effect_phase;
    s32 *formation_phase;
    s32 formation_action;
    s32 finish_action;
    s32 *sequence_phase;

    Func_02002c0c();
    Call4(Func_02002cec, -1, -1, -1, 0);
    Func_02002b84(1);
    Call4(Func_02002cec, 0xf60000, -1, 0x25c0000, 0);
    flag_work = (s32 *)0x200b394;
    flag = Value1(Func_02002bf4, 3);
    *flag_work = flag;
    record = Func_02002c24(13);
    Func_02002bec((s32)record, 0);
    record = Func_02002c24(14);
    Func_02002bec((s32)record, 0);
    record = Func_02002c24(15);
    Func_02002bec((s32)record, 0);
    record = Func_02002c24(16);
    Func_02002bec((s32)record, 0);
    record = Func_02002c24(17);
    Func_02002bec((s32)record, 0);
    record = Func_02002c24(18);
    Func_02002bec((s32)record, 0);
    record = Func_02002c24(19);
    Func_02002bec((s32)record, 0);
    record = Func_02002c24(20);
    Func_02002bec((s32)record, 0);
    record = Func_02002c24(21);
    Func_02002bec((s32)record, 0);
    entry_action = 0x200b024;
    Func_02002c34(17, entry_action);
    Func_02002c34(18, entry_action);
    Func_02002c34(19, entry_action);
    Func_02002c34(20, entry_action);
    Func_02002c34(21, entry_action);
    Call3(Func_02002c7c, 0, 0x740000, 0x25a0000);
    Func_02002b84(1);
    Func_02002bc4();
    Call1(Func_02002b84, 1);
    Func_02002d04();
    Func_02002d0c();
    Call3(Func_02002c2c, 0, 0xcccc, 0x6666);
    Call3(Func_02002c64, 0, 254, 0x251);
    Call3(Func_02002c2c, 1, 0x9999, 0x4ccc);
    Call3(Func_02002c2c, 2, 0x9999, 0x4ccc);
    {
        u8 *record = Record1(Func_02002c24, 0);

        if (record != 0) {
            Func_02002c7c(1, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
        }
    }
    {
        u8 *record = Record1(Func_02002c24, 0);

        if (record != 0) {
            Func_02002c7c(2, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
        }
    }
    Call2(Func_02002c34, 1, 0x200ae20);
    Func_02002c34(2, 0x200ae54);
    if (*flag_work != 0) {
        Call3(Func_02002c2c, 3, 0x9999, 0x4ccc);
        {
            u8 *record = Record1(Func_02002c24, 0);

            if (record != 0) {
                Func_02002c7c(3, *(s32 *)((s32)record + 8), *(s32 *)((s32)record + 16));
            }
        }
        Func_02002c34(3, 0x200ae88);
    }
    Func_02002c3c(2);
    Func_02002780(2, 0x2000, 40);
    Func_02002780(2, 0x8000, 20);
    Func_02002780(2, 0x4000, 40);
    Call3(Func_02002cdc, 2, 0x101, 0);
    Func_02002c04(60);
    Func_02002ccc(1, 0x4000, 0);
    Func_02002780(0, 0x6000, 60);
    value = 160;
    Func_02002780(3, 0x2000, 10);
    Func_02002ccc(1, 0x2000, 0);
    Func_02002780(0, (value << 8), 10);
    Call3(Func_02002cdc, 1, 0x101, 0);
    Call3(Func_02002cdc, 0, 0x101, 0);
    Func_02002c04(40);
    Func_02002ccc(1, 0x4000, 0);
    Func_02002780(0, 0x6000, 10);
    Func_02002ca4(1, 2);
    Call1(Func_02002cb4, 0x1474);
    Func_02002768(1, 10);
    Call1(Func_02002cb4, 0x147c);
    Func_02002780(2, 0xc000, 20);
    Func_02002c8c(2, 3);
    Func_02002768(2, 20);
    Func_02002780(1, 0, 20);
    Func_02002780(0, (value << 8), 40);
    Func_02002780(1, 0x4000, 20);
    Func_02002780(0, 0x6000, 30);
    Func_02002780(1, 0x6000, 20);
    Call3(Func_02002780, 0, 0xe000, 30);
    Func_02002ca4(2, 2);
    Call3(Func_02002cdc, 2, 0x100, 0);
    Func_02002c04(40);
    Func_02002ccc(1, 0x4000, 0);
    Func_02002780(0, 0x6000, 20);
    Func_02002780(2, 0xc000, 10);
    Func_02002d24(17);
    Func_02002d24(206);
    Call2(Func_02002cf4, 0x7fff, 0);
    Func_02002cfc(1);
    Func_02002b84(1);
    *(s32 *)0x0200b398 = 1;
    Value2(Func_02002b8c, 0x200a7c9, 0xc80);
    Func_02002b84(20);
    Call2(Func_02002cf4, 0x405210, 1);
    Call2(Func_02002cf4, 0x10000, 2);
    Func_02002cfc(120);
    Func_02002b84(60);
    motion_action = 0x200aebc;
    Func_02002c34(0, motion_action);
    Func_02002c34(1, motion_action);
    Func_02002c34(2, motion_action);
    Func_02002c34(3, motion_action);
    Func_02002c04(100);
    Func_02002768(1, 20);
    Func_02002768(2, 40);
    if (*(s32 *)0x0200b394 != 0) {
        Func_02002c04(40);
        Call3(Func_02002cdc, 3, 0x102, 0);
        Func_02002c04(40);
        Func_02002768(3, 40);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02002c04(20);
    party_flag = (s32 *)0x200b394;
    if (*party_flag != 0) {
        value = 128;
        record = Func_02002c24(3);
        *(s32 *)((s32)record + 40) = (value << 10);
        Func_02002c04(10);
        Func_02002c2c(3, (value << 10), (value << 10));
        Call3(Func_02002c6c, 3, -2, 0);
        Call2(Func_02002c34, 3, 0x200af48);
        record = Func_02002c24(3);
        Func_02002bec((s32)record, 0);
        Func_02002c84(3, 19);
        Func_02002c04(10);
    }
    value = 128;
    record = Func_02002c24(0);
    *(s32 *)((s32)record + 40) = (value << 10);
    Func_02002c04(10);
    Call3(Func_02002c2c, 0, (value << 10), (value << 10));
    reset_action = 0x200af48;
    Func_02002c34(0, reset_action);
    record = Func_02002c24(0);
    Func_02002bec((s32)record, 0);
    Func_02002c84(0, 19);
    Func_02002c04(20);
    record = Record1(Func_02002c24, 1);
    *(s32 *)((s32)record + 40) = (value << 10);
    Func_02002c04(10);
    Call3(Func_02002c2c, 1, (value << 10), (value << 10));
    Func_02002c34(1, reset_action);
    record = Func_02002c24(1);
    Func_02002bec((s32)record, 0);
    Func_02002c84(1, 19);
    Func_02002c04(40);
    record = Record1(Func_02002c24, 2);
    *(s32 *)((s32)record + 40) = (value << 10);
    Func_02002c04(10);
    Func_02002c34(2, reset_action);
    record = Func_02002c24(2);
    Func_02002bec((s32)record, 0);
    value = 0;
    Func_02002c84(2, 19);
    *(s32 *)0x0200b398 = value;
    Func_02002c04(160);
    Value1(Func_02002b94, 0x200a7c9);
    Func_02002c04(120);
    Call2(Func_02002cf4, 0x406218, 1);
    Func_02002cfc(60);
    Func_02002b84(60);
    *(s32 *)0x0200b388 = value;
    effect_phase = (s32 *)0x200b38c;
    *(s32 *)0x0200b384 = 0x800000;
    *effect_phase = 1;
    Value2(Func_02002b8c, 0x200a975, 0xc80);
    Func_02002c04(180);
    Func_02002d24(21);
    Func_02002768(1, 80);
    Func_02002768(2, 40);
    Call3(Func_02002cdc, 0, 0x102, 0);
    Call3(Func_02002cdc, 1, 0x102, 0);
    Call3(Func_02002cdc, 2, 0x102, 0);
    Call3(Func_02002cdc, 3, 0x102, 0);
    Func_02002c04(60);
    Func_02002768(2, 20);
    *effect_phase = 2;
    Func_02002c9c(2, 2);
    Func_02002c04(20);
    Func_02002c9c(1, 1);
    Func_02002c04(40);
    Func_02002c9c(0, 2);
    Func_02002c9c(3, 1);
    Func_02002c04(20);
    Func_02002c9c(2, 3);
    Func_02002c04(40);
    Func_02002c9c(0, 1);
    Func_02002c04(20);
    Func_02002c9c(1, 2);
    Func_02002c04(20);
    Func_02002c9c(3, 2);
    Call3(Func_02002cdc, 1, 0x102, 0);
    Func_02002768(1, 20);
    if (*party_flag != 0) {
        Call3(Func_02002cdc, 3, 0x102, 0);
        Func_02002768(3, 10);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    formation_phase = (s32 *)0x200b38c;
    *formation_phase = 3;
    *(u8 *)(Func_02002c24(0) + 35) &= 254;
    *(u8 *)(Func_02002c24(1) + 35) &= 254;
    *(u8 *)(Func_02002c24(2) + 35) &= 254;
    *(u8 *)(Func_02002c24(3) + 35) &= 254;
    Func_02002cd4(0, 3);
    Func_02002cd4(1, 3);
    Func_02002cd4(2, 3);
    value = 0;
    Func_02002cd4(3, 3);
    *(s32 *)0x0200b390 = value;
    Value2(Func_02002b8c, 0x200aad9, 0xc80);
    Func_02002d24(220);
    *(u8 *)(Func_02002c24(13) + 35) &= 254;
    Func_02002cd4(13, 2);
    Call3(Func_02002c7c, 13, 0xfd0000, 0x25b0000);
    formation_action = 0x200af6c;
    Func_02002c34(13, formation_action);
    *(u8 *)(Func_02002c24(14) + 35) &= 254;
    Func_02002cd4(14, 2);
    Call3(Func_02002c7c, 14, 0xe90000, 0x2750000);
    Func_02002c34(14, formation_action);
    if (*(s32 *)0x0200b394 != 0) {
        *(u8 *)(Func_02002c24(15) + 35) &= 254;
        Func_02002cd4(15, 2);
        Call3(Func_02002c7c, 15, 0xcf0000, 0x2610000);
        Func_02002c34(15, formation_action);
    }
    *(u8 *)(Func_02002c24(16) + 35) &= 254;
    Func_02002cd4(16, 2);
    Call3(Func_02002c7c, 16, 0xe30000, 0x2440000);
    Func_02002c34(16, formation_action);
    if (*formation_phase != 0) {
        do {
            Func_02002b84(1);
        } while (*formation_phase != 0);
    }
    Call1(Func_02002c04, 0x12c);
    Value1(Func_02002b94, 0x200a975);
    Func_02002c04(120);
    Func_02002d24(17);
    Call2(Func_02002cf4, 0x10000, 1);
    Func_02002cfc(60);
    Func_02002b84(60);
    Func_02002c44(13);
    Func_02002c44(14);
    party_flag = (s32 *)0x200b394;
    if (*party_flag != 0) {
        Func_02002c44(15);
    }
    Func_02002c44(16);
    Func_02002b84(1);
    finish_action = 0x200afc8;
    Func_02002c34(13, finish_action);
    Func_02002c34(14, finish_action);
    if (*party_flag != 0) {
        Func_02002c34(15, finish_action);
    }
    Func_02002c4c(16, finish_action);
    Func_02002c04(80);
    Func_02002ca4(1, 2);
    Func_02002c04(40);
    Value2(Func_02002cbc, 1, 0);
    Call3(Func_02002c7c, 11, 0xdc0000, 0x1ee0000);
    Call3(Func_02002c7c, 12, 0xdc0000, 0x1ee0000);
    Func_02002b84(1);
    if (Value2(Func_02002c1c, 11, 0) == 1) {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02002ca4(0, 1);
    Func_02002c04(20);
    Func_02002ca4(2, 2);
    Func_02002768(2, 20);
    if (*party_flag != 0) {
        Func_02002ca4(3, 2);
        Func_02002c04(10);
        Call1(Func_02002cb4, 0x1488);
        Func_02002768(3, 40);
    }
    Func_02002c9c(1, 1);
    Call3(Func_02002cdc, 1, 0x101, 0);
    Func_02002c04(80);
    Func_02002ca4(2, 2);
    Call1(Func_02002cb4, 0x1489);
    Func_02002768(2, 40);
    Func_02002ca4(1, 3);
    Func_02002c04(40);
    Func_02002cd4(1, 2);
    *(u8 *)(Func_02002c24(1) + 35) |= 1;
    record = Func_02002c24(1);
    Func_02002bec((s32)record, 1);
    Func_02002c94(1, 6, 0);
    Call3(Func_02002c6c, 1, -3, 0);
    Func_02002c84(1, 1);
    Call3(Func_02002780, 1, 0x4000, 60);
    Func_02002768(1, 20);
    Func_02002c9c(1, 2);
    Func_02002768(1, 10);
    Func_02002ca4(0, 3);
    Call3(Func_02002780, 1, 0x2000, 20);
    Call3(Func_02002cdc, 1, 0x101, 0);
    Func_02002c04(40);
    Func_02002780(1, 0x6000, 40);
    Func_02002780(1, 0x2000, 20);
    Func_02002780(1, 0x6000, 20);
    Func_02002780(1, 0x2000, 10);
    Func_02002c94(1, 2, 0);
    Func_02002c04(40);
    Func_02002c94(1, 2, 0);
    Func_02002c04(10);
    Func_02002c94(1, 4, 0);
    Func_02002c04(20);
    Func_02002768(1, 20);
    if (*party_flag != 0) {
        Call3(Func_02002cdc, 3, 0x100, 0);
        Func_02002c04(60);
        Func_02002ca4(3, 2);
        Func_02002c04(80);
        Func_02002cd4(3, 2);
        *(u8 *)(Func_02002c24(3) + 35) |= 1;
        record = Func_02002c24(3);
        Func_02002bec((s32)record, 1);
        Func_02002c94(3, 4, 0);
        Call3(Func_02002c6c, 3, -2, 0);
        Func_02002c84(3, 1);
        Call3(Func_02002780, 3, 0xe000, 60);
        Func_02002ca4(3, 2);
        Func_02002c04(20);
        Func_02002768(3, 20);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Call3(Func_02002c94, 1, 2, 0);
    Func_02002780(1, 0x4000, 20);
    Func_02002c8c(1, 3);
    Func_02002780(1, 0x2000, 10);
    Func_02002768(1, 20);
    Func_02002c8c(1, 3);
    Func_02002c04(10);
    Call2(Func_02002ca4, 2, 1);
    Func_02002c04(40);
    Func_02002ca4(2, 2);
    Func_02002c04(20);
    value = 1;
    Func_02002cd4(2, 2);
    *(u8 *)(Func_02002c24(2) + 35) |= value;
    record = Func_02002c24(2);
    Func_02002bec((s32)record, 1);
    Func_02002c94(2, 4, 0);
    Func_02002c84(2, 1);
    Call3(Func_02002ccc, 2, 0xc000, 0);
    Func_02002ca4(0, 2);
    Func_02002c04(10);
    Func_02002cd4(0, 2);
    {
        u8 *record = Func_02002c24(0);
        u8 flags = record[35];

        record[35] = (u8)(flags | value);
    }
    record = Func_02002c24(0);
    Func_02002bec((s32)record, 1);
    Func_02002c94(0, 4, 0);
    Func_02002c84(0, 1);
    Func_02002780(0, 0x6000, 60);
    Call3(Func_02002cdc, 0, 0x105, 0);
    Call3(Func_02002cdc, 2, 0x105, 0);
    Func_02002c04(60);
    Call3(Func_02002780, 0, 0xa000, 20);
    Func_02002c8c(1, 3);
    Func_02002c8c(0, 3);
    Func_02002780(0, 0x6000, 10);
    Func_02002780(1, 0x4000, 10);
    Call3(Func_02002ccc, 2, 0xc000, 0);
    Func_02002c8c(2, 3);
    Func_02002768(2, 20);
    Func_02002c84(0, 3);
    Func_02002c84(3, 3);
    Func_02002c8c(1, 3);
    Func_02002c04(20);
    Func_02002ca4(2, 1);
    Func_02002c04(20);
    Value2(Func_02002cbc, 2, 0);
    Func_02002c84(2, 3);
    Call3(Func_02002ccc, 2, 0xe000, 0);
    Func_02002ccc(1, 0x2000, 0);
    if (Value2(Func_02002c1c, 0, 0) == 0) {
        Func_02002c84(2, 3);
        Func_02002c8c(1, 3);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    } else {
        Func_02002ca4(1, 2);
        Func_02002780(1, 0x2000, 10);
        Func_02002c9c(1, 2);
        Func_02002cc4(1, 0);
    }
    Call3(Func_02002780, 1, 0x4000, 10);
    Func_02002c8c(1, 4);
    Func_02002768(1, 20);
    Call3(Func_02002780, 2, 0xc000, 10);
    Func_02002c8c(2, 3);
    Func_02002768(2, 10);
    if (*(s32 *)0x0200b394 != 0) {
        Func_02002ca4(3, 2);
        Func_02002780(3, 0, 20);
        Call3(Func_02002780, 3, 0x2000, 10);
        Func_02002c84(3, 4);
        Func_02002768(3, 10);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Call3(Func_02002ccc, 1, 0x2000, 0);
    Func_02002780(0, 0xa000, 10);
    Func_02002c84(0, 3);
    Func_02002c8c(1, 3);
    value = 128;
    Func_02002c04(20);
    Call3(Func_02002ccc, 0, 0x6000, 0);
    Func_02002780(1, (value << 7), 10);
    Func_02002c8c(2, 4);
    Func_02002c04(20);
    Call3(Func_02002cdc, 0, 0x102, 0);
    Call3(Func_02002cdc, 1, 0x102, 0);
    Func_02002c04(80);
    Call3(Func_02002780, 2, 0xe000, 10);
    Func_02002c9c(2, 2);
    Func_02002768(2, 20);
    Call3(Func_02002ccc, 1, 0x2000, 0);
    Func_02002780(0, 0xa000, 40);
    Func_02002ccc(1, (value << 7), 0);
    Func_02002780(0, 0x6000, 10);
    Call3(Func_02002780, 2, 0xc000, 10);
    Func_02002c8c(2, 3);
    Func_02002768(2, 10);
    Call2(Func_02002ce4, 1, 0x102);
    Func_02002c04(40);
    Func_02002768(1, 20);
    Func_02002c8c(2, 3);
    Func_02002c04(20);
    Call3(Func_02002cdc, 1, 0x102, 0);
    Func_02002c04(40);
    Func_02002768(1, 20);
    Func_02002c84(2, 3);
    Func_02002768(2, 10);
    Func_02002ca4(1, 2);
    Call3(Func_02002780, 1, 0x2000, 10);
    Value2(Func_02002cbc, 1, 0);
    Func_02002ccc(0, 0xa000, 0);
    if (Value2(Func_02002c1c, 0, 0) == 0) {
        Func_02002c8c(1, 3);
    } else {
        Func_02002c04(20);
        Func_02002ca4(1, 2);
        Func_02002c04(40);
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02002cc4(1, 0);
    Func_02002d24(21);
    Call2(Func_02002cf4, 0x406218, 1);
    Func_02002cfc(60);
    Func_02002b84(60);
    *(s32 *)0x0200b388 = 0;
    sequence_phase = (s32 *)0x200b38c;
    *(s32 *)0x0200b384 = 0x800000;
    *sequence_phase = 1;
    Value2(Func_02002b8c, 0x200a975, 0xc80);
    Func_02002c04(80);
    Call2(Func_02002c9c, 0, 2);
    Func_02002c9c(1, 2);
    Call2(Func_02002c9c, 3, 2);
    Func_02002ca4(2, 2);
    Func_02002c04(60);
    Func_02002780(2, 0xc000, 10);
    Call1(Func_02002cb4, 0x149d);
    Func_02002768(2, 10);
    Func_02002780(1, 0xc000, 10);
    Func_02002780(0, 0xc000, 10);
    party_flag = (s32 *)0x200b394;
    if (*party_flag != 0) {
        Func_02002780(3, 0xc000, 10);
    }
    *(u8 *)(Func_02002c24(0) + 35) &= 254;
    *(u8 *)(Func_02002c24(1) + 35) &= 254;
    *(u8 *)(Func_02002c24(2) + 35) &= 254;
    *(u8 *)(Func_02002c24(3) + 35) &= 254;
    Func_02002cd4(0, 3);
    Func_02002cd4(1, 3);
    Func_02002cd4(2, 3);
    Func_02002cd4(3, 3);
    *sequence_phase = 2;
    Func_02002d24(220);
    Call3(Func_02002c7c, 13, 0xfd0000, 0x25b0000);
    formation_action = 0x200af6c;
    Func_02002c34(13, formation_action);
    Call3(Func_02002c7c, 14, 0xe90000, 0x2750000);
    Func_02002c34(14, formation_action);
    if (*party_flag != 0) {
        Call3(Func_02002c7c, 15, 0xcf0000, 0x2610000);
        Func_02002c34(15, formation_action);
    }
    Call3(Func_02002c7c, 16, 0xe30000, 0x2440000);
    Call2(Func_02002c34, 16, formation_action);
    Func_02002c04(120);
    *sequence_phase = 3;
    do {
        Func_02002b84(1);
    } while (*sequence_phase != 0);
    Func_02002768(11, 80);
    Func_02002768(12, 20);
    Call3(Func_02002cdc, 0, 0x101, 0);
    Call3(Func_02002cdc, 1, 0x101, 0);
    Call3(Func_02002cdc, 2, 0x101, 0);
    Call3(Func_02002cdc, 3, 0x101, 0);
    Func_02002c04(60);
    Func_02002768(12, 20);
    Func_02002c84(0, 3);
    Func_02002c84(1, 3);
    Func_02002c84(3, 3);
    Func_02002c8c(2, 3);
    Func_02002768(12, 10);
    Func_02002c84(0, 3);
    Func_02002c84(1, 3);
    Func_02002c84(3, 3);
    Func_02002c8c(2, 3);
    Func_02002768(12, 10);
    Call3(Func_02002cdc, 0, 0x100, 0);
    Call3(Func_02002cdc, 1, 0x100, 0);
    Call3(Func_02002cdc, 2, 0x100, 0);
    Call3(Func_02002cdc, 3, 0x100, 0);
    Func_02002c04(40);
    Func_02002768(11, 10);
    Call3(Func_02002ccc, 0, 0x8000, 0);
    Call3(Func_02002ccc, 1, 0x4000, 0);
    Func_02002ccc(3, 0, 0);
    Func_02002780(2, 0xc000, 40);
    Func_02002768(12, 10);
    Call3(Func_02002cdc, 0, 0x102, 0);
    Call3(Func_02002cdc, 1, 0x102, 0);
    Call3(Func_02002cdc, 2, 0x102, 0);
    Call3(Func_02002cdc, 3, 0x102, 0);
    Func_02002ccc(0, 0xc000, 0);
    Func_02002ccc(1, 0xc000, 0);
    Func_02002ccc(2, 0xc000, 0);
    Func_02002780(3, 0xc000, 80);
    Func_02002768(12, 10);
    Call3(Func_02002ccc, 0, 0x8000, 0);
    Call3(Func_02002ccc, 1, 0x4000, 0);
    Func_02002ccc(2, 0xc000, 0);
    Func_02002780(3, 0, 40);
    Func_02002768(11, 10);
    Func_02002ccc(0, 0xc000, 0);
    Func_02002ccc(1, 0xc000, 0);
    Func_02002ccc(2, 0xc000, 0);
    Func_02002780(3, 0xc000, 10);
    Func_02002c84(0, 4);
    Func_02002c84(1, 4);
    Func_02002c84(3, 4);
    Func_02002c8c(2, 4);
    Func_02002c04(60);
    Func_02002768(12, 10);
    Func_02002c84(0, 3);
    Func_02002c84(1, 3);
    Func_02002c84(3, 3);
    Func_02002c8c(2, 3);
    Func_02002768(12, 20);
    Call3(Func_02002ccc, 0, 0x8000, 0);
    Call3(Func_02002ccc, 1, 0x4000, 0);
    Func_02002ccc(2, 0xc000, 0);
    Func_02002780(3, 0, 20);
    Func_02002768(12, 10);
    Func_02002c9c(0, 2);
    Func_02002c9c(1, 2);
    Func_02002c9c(3, 2);
    Func_02002ca4(2, 2);
    Func_02002ccc(0, 0xc000, 0);
    Func_02002ccc(1, 0xc000, 0);
    Func_02002ccc(2, 0xc000, 0);
    Func_02002ccc(3, 0xc000, 0);
    Func_02002768(12, 20);
    Call3(Func_02002ccc, 0, 0x8000, 0);
    Call3(Func_02002ccc, 1, 0x4000, 0);
    Func_02002ccc(2, 0xc000, 0);
    Func_02002780(3, 0, 20);
    Func_02002768(11, 20);
    Call3(Func_02002cdc, 0, 0x102, 0);
    Call3(Func_02002cdc, 1, 0x102, 0);
    Call3(Func_02002cdc, 3, 0x102, 0);
    Call3(Func_02002cdc, 2, 0x102, 0);
    Func_02002c04(40);
    Func_02002768(12, 10);
    Func_02002ccc(0, 0xc000, 0);
    Func_02002ccc(1, 0xc000, 0);
    Func_02002ccc(2, 0xc000, 0);
    Func_02002780(3, 0xc000, 10);
    Func_02002768(12, 10);
    Func_02002c84(0, 3);
    Func_02002c84(1, 3);
    Func_02002c84(3, 3);
    Func_02002c8c(2, 3);
    Func_02002c04(60);
    Func_02002cc4(12, 0);
    Func_02002cc4(11, 0);
    Value1(Func_02002b94, 0x200a975);
    Func_02002c04(80);
    Call2(Func_02002cf4, 0x10000, 1);
    Func_02002cfc(60);
    Func_02002b84(80);
    Func_02002c44(13);
    Func_02002c44(14);
    party_flag = (s32 *)0x200b394;
    Func_02002c44(15);
    Func_02002c44(16);
    Func_02002b84(1);
    finish_action = 0x200afc8;
    Func_02002c34(13, finish_action);
    Func_02002c34(14, finish_action);
    if (*party_flag != 0) {
        Func_02002c34(15, finish_action);
    }
    Func_02002c4c(16, finish_action);
    Func_02002c04(20);
    Func_02002cd4(0, 2);
    Func_02002cd4(1, 2);
    Func_02002cd4(2, 2);
    value = 1;
    Func_02002cd4(3, 2);
    *(u8 *)(Func_02002c24(0) + 35) |= value;
    *(u8 *)(Func_02002c24(1) + 35) |= value;
    *(u8 *)(Func_02002c24(2) + 35) |= value;
    {
        u8 *record = Func_02002c24(3);
        u8 flags = record[35];

        record[35] = (u8)(flags | value);
    }
    Func_02002ca4(2, 2);
    Func_02002780(2, 0xe000, 10);
    Value2(Func_02002cbc, 2, 0);
    Call3(Func_02002ccc, 1, 0x2000, 0);
    Func_02002ccc(3, 0, 0);
    if (Value2(Func_02002c1c, 0, 0) != 0) {
    } else {
        Func_02002ca4(1, 2);
        Func_02002c04(10);
        Value2(Func_02002cbc, 1, 0);
        if (Value2(Func_02002c1c, 0, 0) == 0) {
            Func_02002780(3, 0, 20);
            Call3(Func_02002cdc, 1, 0x101, 0);
            Call3(Func_02002cdc, 2, 0x101, 0);
            Call3(Func_02002cdc, 3, 0x101, 0);
            Func_02002c04(40);
            Call3(Func_02002780, 1, 0x4000, 20);
            Func_02002768(1, 10);
            Func_02002780(2, 0xc000, 20);
            Func_02002780(2, 0xe000, 20);
            Func_02002c8c(2, 3);
            Func_02002768(2, 20);
            Func_02002780(1, 0x2000, 20);
        } else {
            Func_02002780(3, 0, 20);
            Call3(Func_02002cdc, 1, 0x102, 0);
            Call3(Func_02002cdc, 2, 0x102, 0);
            Call3(Func_02002cdc, 3, 0x102, 0);
            Func_02002c04(40);
            Call3(Func_02002780, 1, 0x4000, 20);
            Call1(Func_02002cb4, 0x14b4);
            Func_02002768(1, 20);
            Func_02002c8c(2, 3);
            Func_02002768(2, 20);
        }
        Func_02002c84(3, 3);
        Func_02002c8c(1, 3);
        goto L_02002528;
    }
    Func_02002c04(20);
    Func_02002c8c(1, 3);
    Func_02002c04(10);
    Call1(Func_02002cb4, 0x14b6);
    Func_02002768(1, 10);
    Call3(Func_02002ccc, 1, 0x4000, 0);
    Call3(Func_02002780, 0, 0x6000, 20);
    Func_02002c84(1, 3);
    Func_02002c8c(0, 3);
    Func_02002c04(10);
    Func_02002c8c(2, 4);
    Value2(Func_02002cbc, 2, 0);
    if (Value2(Func_02002c1c, 0, 0) != 0) {
    } else {
        Func_02002c04(20);
        Call3(Func_02002cdc, 2, 0x103, 0);
        Func_02002c04(40);
        Func_02002780(2, 0xe000, 10);
        Func_02002768(2, 10);
        if (*party_flag != 0) {
            Func_02002780(3, 0, 10);
            Func_02002c9c(3, 3);
            Func_02002768(3, 20);
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        Call3(Func_02002cdc, 0, 0x102, 0);
        Call3(Func_02002cdc, 1, 0x102, 0);
        Func_02002c04(40);
        Func_02002ca4(1, 2);
        Func_02002768(1, 20);
        Call3(Func_02002cdc, 1, 0x105, 0);
        Func_02002c04(120);
        Func_02002768(2, 40);
        if (*(s32 *)0x0200b394 != 0) {
            Call3(Func_02002780, 3, 0x2000, 10);
            Func_02002c8c(3, 4);
            Func_02002768(3, 10);
        } else {
            *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
        }
        Func_02002c04(60);
        Func_02002ca4(2, 2);
        if (*(s32 *)0x0200b394 != 0) {
            Func_02002780(2, 0xa000, 40);
            Func_02002780(2, 0xe000, 20);
        }
        Func_02002768(2, 10);
        Func_02002c9c(0, 2);
        Func_02002ca4(1, 2);
        Func_02002c04(40);
        Func_02002768(2, 20);
        Func_02002c8c(0, 3);
        Func_02002c8c(1, 3);
        Func_02002c04(20);
        Func_02002c84(3, 3);
        L_02002528:;
        Func_02002c8c(2, 3);
        goto L_02002660;
    }
    Call3(Func_02002cdc, 2, 0x105, 0);
    Func_02002c04(40);
    Func_02002c8c(2, 3);
    Call1(Func_02002cb4, 0x14bf);
    Func_02002768(2, 20);
    if (*party_flag != 0) {
        Func_02002780(3, 0, 10);
        Func_02002c9c(3, 1);
        Func_02002768(3, 20);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Call3(Func_02002cdc, 1, 0x102, 0);
    Call3(Func_02002cdc, 0, 0x102, 0);
    Func_02002c04(40);
    Func_02002ca4(1, 2);
    Func_02002768(1, 20);
    Call3(Func_02002cdc, 2, 0x105, 0);
    Func_02002c04(80);
    Func_02002768(2, 40);
    if (*(s32 *)0x0200b394 != 0) {
        Call3(Func_02002780, 3, 0x2000, 20);
        Func_02002c84(3, 4);
        Func_02002768(3, 40);
    } else {
        *(u16 *)((*(s32 *)0x03001ebc + 0x1d8)) += 1;
    }
    Func_02002ca4(2, 2);
    Func_02002c04(20);
    Func_02002768(2, 20);
    Func_02002c9c(1, 2);
    Func_02002ca4(0, 2);
    Func_02002c04(40);
    Func_02002768(2, 20);
    L_02002660:;
    Func_02002d24(17);
    Call3(Func_02002c2c, 1, 0x13333, 0x9999);
    Call3(Func_02002c2c, 2, 0x13333, 0x9999);
    Call3(Func_02002c2c, 3, 0x13333, 0x9999);
    Func_02002c84(1, 2);
    {
        u8 *record = Record1(Func_02002c24, 0);

        if (record != 0) {
            Func_02002c54(1, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
    }
    Func_02002c74(1);
    Func_02002c7c(1, 0, 0);
    Func_02002c84(2, 2);
    {
        u8 *record = Record1(Func_02002c24, 0);

        if (record != 0) {
            Func_02002c54(2, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
        }
    }
    Func_02002c74(2);
    Func_02002c7c(2, 0, 0);
    if (*(s32 *)0x0200b394 != 0) {
        Func_02002c84(3, 2);
        {
            u8 *record = Record1(Func_02002c24, 0);

            if (record != 0) {
                Func_02002c54(3, *(s16 *)((s32)record + 10), *(s16 *)((s32)record + 18));
            }
        }
        Func_02002c74(3);
        Func_02002c7c(3, 0, 0);
    }
    Value1(Func_02002bfc, 0x843);
    Func_02002c7c(11, 0, 0);
    Func_02002c7c(12, 0, 0);
    Func_02002d14();
    Func_02002c14();
}

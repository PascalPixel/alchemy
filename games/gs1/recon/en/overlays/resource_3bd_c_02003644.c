#include "types.h"

#define FieldScene_RunScene3bdSequenceA Func_02003644

extern u8 Data_00000092[];
extern u8 Data_00000093[];
extern u8 Data_00000094[];
extern u8 Data_00000095[];
extern u8 Data_00000096[];
extern u8 Data_00000097[];
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];
void Func_0200404a();
void Func_02004050();
void Func_02004056();
void Func_020040da();
void Func_020040e0();
void Func_020040e6();
void Func_020044e2();
void Func_02004836();
void Func_02007352();
void Func_02007360();
s32 Func_02007376();
void Func_0200738c();
s32 Func_02007398();
void Func_020073b0();
s32 Func_020073be();
void Func_02007404();
s32 Func_02007414();
void Func_0200741c();
s32 Func_02007424();
void Func_0200743a();
void Func_02007440();
void Func_0200745c();
s32 Func_02007468();
u8 *Func_0200748c();
u8 *Func_0200749c();
s32 Func_020074a8();
void Func_020074ac();
void Func_020074c8();
void Func_020074e4();
s32 Func_020074ee();
void Func_020074f0();
u8 *Func_020074f4();
s32 Func_020074fe();
u8 *Func_02007506();
void Func_0200755a();
s32 Func_020075a4();
u8 *Func_020075ba();
void Func_020075f6();
void Func_02007602();
void Func_02007622();
s32 Func_02007624();
s32 Func_02007632();
s32 Func_02007634();
s32 Func_02007642();
s32 Func_0200764e();
u8 *Func_02007658();
u8 *Func_02007664();
void Func_02007666();
void Func_0200766e();
void Func_02007676();
void Func_0200767e();
void Func_02007686();
void Func_0200768e();
void Func_02007696();
void Func_0200769e();
void Func_020076a6();
void Func_020076ae();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 Func_02003644(void)
{
    u32 i;
    u8 *record;
    s32 base5_2001001;
    s32 none;
    s32 v6;
    s32 v5;
    u8 slot8[4];

    if (Data_02000240_t[225][0] == 0) {
        if (Data_02000240_t[224][0] == (s32)Data_00000093) {
            {
                s32 shown = 10;
            
                *(u16 *)0x02000402 = shown;
            }
        }
        if (((*(u16 *)0x02000400 << 16) >> 16) == (s32)Data_00000094) {
            {
                s32 shown = 20;
            
                *(u16 *)0x02000402 = shown;
            }
        }
        if (((*(u16 *)0x02000400 << 16) >> 16) == (s32)Data_00000095) {
            {
                s32 shown = 30;
            
                *(u16 *)0x02000402 = shown;
            }
        }
        if (((*(u16 *)0x02000400 << 16) >> 16) == (s32)Data_00000096) {
            {
                s32 shown = 40;
            
                *(u16 *)0x02000402 = shown;
            }
        }
        if (((*(u16 *)0x02000400 << 16) >> 16) == (s32)Data_00000097) {
            {
                s32 shown = 50;
            
                *(u16 *)0x02000402 = shown;
            }
        }
    }
    Call1(Func_02007352, 0x200);
    Call1(Func_02007360, 0x201);
    if (Data_02000240_t[224][0] == (s32)Data_00000092) {
        if (Data_02000240_t[225][0] == 1) {
            record = Value1(Func_02007376, 0x109);
            if ((s32)record == 0) {
                *(u8 *)0x02001004 = (s32)record;
            }
            Call1(Func_0200738c, 0x201);
        }
        if (Data_02000240_t[225][0] == 2) {
            if (Value1(Func_02007398, 0x109) == 0) {
                *(u8 *)0x02001004 = 5;
            }
            Call1(Func_020073b0, 0x201);
        }
    }
    if (Data_02000240_t[224][0] == (s32)Data_00000093) {
        if (Value1(Func_020073be, 0x962) != 0) {
            Func_0200745c(8, 0, 0);
        } else {
            record = Value1(Func_02007414, 8);
            *(u8 *)(*(s32 *)((s32)record + 80) + 9) = ((-13 & *(u8 *)(*(s32 *)((s32)record + 80) + 9)) | 4);
            *(u8 *)(*(s32 *)((s32)record + 80) + 38) = 2;
            {
                s32 target = *(s32 *)((s32)record + 80);
                s32 shown = 0x4000;
            
                *(u16 *)(target + 30) = shown;
            }
        }
    }
    if (Data_02000240_t[224][0] == (s32)Data_00000095) {
        Call1(Func_0200741c, 0x200);
        Func_0200404a(8);
        Func_02004050(9);
        Func_02004056(10);
        if (Value1(Func_02007424, 0x211) != 0) {
            Func_020074c8(11, 5);
            Call6(Func_02007404, 76, 16, 1, 1, 73, 17);
        } else {
            {
                u8 *record = Func_0200748c(11);
                u8 value = *(volatile u8 *)&record[35];
            
                record[35] = (u8)(value | 2);
            }
        }
        record = Func_0200749c(11);
        Func_0200743a((s32)record, 0);
        if (Value1(Func_02007468, 0x212) != 0) {
            Call6(Func_02007440, 30, 20, 1, 1, 32, 20);
        }
    }
    if (Data_02000240_t[224][0] != (s32)Data_00000097) {
    } else {
        Call1(Func_020074ac, 0x200);
        Func_020040da(8);
        Func_020040e0(9);
        Func_020040e6(10);
        record = Func_020074f4(8);
        *(s32 *)((s32)record + 108) = 0x200b611;
        record = Value1(Func_020074fe, 9);
        *(s32 *)((s32)record + 108) = 0x200b611;
        record = Func_02007506(10);
        *(s32 *)((s32)record + 108) = 0x200b611;
        if (Data_02000240_t[225][0] == 52) {
            *(s32 *)(slot8) = 0;
            *(s32 *)(0x40000d4) = slot8;
            *(s32 *)(0x40000d4 + 4) = *(s32 *)0x0200bf6c;
            *(s32 *)(0x40000d4 + 8) = -0x7afffffd;
            record = Value4(Func_020074ee, 0x109, *(s32 *)0x0200bf6c, -0x7afffffd, ((0x40000d4 + 12) - 12));
            if ((s32)record == 0) {
                *(u8 *)0x02001000 = (s32)record;
                *(u8 *)0x02001001 = (s32)record;
                *(u8 *)0x02001002 = 4;
            }
        }
        base5_2001001 = 0x2001001;
        if (*(s8 *)(0x2001001) == 99) {
            Call6(Func_020074e4, 41, 55, 3, 2, 30, 55);
            Call6(Func_020074f0, 42, 8, 1, 1, 31, 8);
        }
        if ((*(u8 *)base5_2001001 << 24) == 0x2000000) {
            record = Value2(Func_020074a8, (*(s8 *)(base5_2001001 + 1) << 16), 5);
            Func_02004836(((s32)record + 0x4000));
        }
        v6 = none;
        none = 0;
        do {
            record = Value1(Func_020075a4, (v6 + 11));
            record[85] = none;
            *(u8 *)(((s32)record + 85) + 4) = none;
            *(s32 *)((s32)record + 24) = 0x10000;
            *(s32 *)((s32)record + 28) = 0x10000;
            record = Func_020075ba((v6 + 11));
            v6 = (v6 + 1);
            Func_0200755a((s32)record, 0);
            Func_02007622((v6 + 11), v6);
        } while (v6 <= 4);
        Func_02007666(11, 1);
        Func_0200766e(12, 4);
        Func_02007676(13, 11);
        Func_0200767e(14, 2);
        Func_02007686(15, 3);
        Func_0200768e(16, 6);
        Func_02007696(17, 6);
        Func_0200769e(18, 6);
        Func_020076a6(19, 6);
        Func_020076ae(20, 6);
        record = Value1(Func_02007624, 16);
        *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
        record = Value1(Func_02007634, 20);
        *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
        v5 = 2;
        *(u8 *)(Func_02007642(16) + 35) = v5;
        *(u8 *)(Func_0200764e(20) + 35) = v5;
        record = Func_02007658(16);
        Func_020075f6((s32)record, 0);
        record = Func_02007664(20);
        Func_02007602((s32)record, 0);
    }
    if (Value1(Func_02007632, 0x200) != 0) {
        Func_020044e2();
    } else {
        *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x204;
        *(s32 *)((*(s32 *)0x03001ebc + 0x1c8)) = 24;
    }
    return 0;
}

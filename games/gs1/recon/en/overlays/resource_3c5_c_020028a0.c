#include "types.h"

#define FieldScene_RunScene3c5SequenceB Func_020028a0

extern u8 Data_00000000[];
extern u8 Data_000000ae[];
extern u8 Data_000000af[];
extern u8 Data_000000b0[];
extern u8 Data_000000b1[];
extern u8 Data_02000240[];
extern u16 Data_02000240_t[][2];
void Func_02003a7c();
void Func_02005172();
void Func_0200517c();
void Func_0200562c();
s32 Func_02005730();
s32 Func_02005754();
s32 Func_02005784();
s32 Func_020057a8();
s32 Func_020057b0();
s32 Func_020057ce();
void Func_020057d6();
s32 Func_020057de();
void Func_020057e8();
void Func_020057f0();
void Func_02005808();
s32 Func_02005818();
void Func_02005852();
s32 Func_0200585a();
void Func_02005894();
s32 Func_02005896();
void Func_020058ac();
void Func_020058d0();
void Func_020058d0_a();
s32 Func_020058e2();
s32 Func_020058e2_a();
void Func_02005906();
s32 Func_020059da();
s32 Func_020059de();
s32 Func_020059ec();
void Func_020059fe();
s32 Func_02005a04();
void Func_02005a0a();
void Func_02005a0e();
void Func_02005a18();
void Func_02005a82();
s32 Func_02005a86();
void Func_02005a90();
void Func_02005a96();
void Func_02005a9e();
void Func_02005aac();
void Func_02005aba();
s32 Func_02005ae0();
void Func_02005aea();
void Func_02005af2();
void Func_02005b08();
void Func_02005b2c();
void Func_02005b38();
s32 Func_02005b40();
void Func_02005b44();
void Func_02005b50();
void Func_02005b62();
void Func_02005b70();
void Func_02005b7c();
void Func_02005b88();
void Func_02005b94();
void Func_02005ba0();
void Func_02005bac();
void Func_02005bca();
void Func_02005be4();
void Func_02005bee();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 Func_020028a0(void)
{
    u32 i;
    s32 rec7;
    u8 *record;
    s32 v1;
    s32 v2;
    s32 v6;
    u8 slot1008[4];
    u8 slot336[416];

    Func_0200562c(1);
    *(s32 *)(*(s32 *)0x03001ebc + 0x1c0) = 0x204;
    if (*(s16 *)(0x2000240 + 448) != (s32)Data_000000b1) {
        {
            s32 shown = 1;
        
            *(u16 *)0x02000482 = shown;
        }
        v2 = (s32)Data_000000b0;
        Data_02000240_t[144][0] = (s32)Data_000000b0;
        v1 = *(s16 *)(0x2000240 + 448);
    } else {
        record = Value1(Func_02005730, 12);
        v2 = (*(s32 *)((s32)record + 8) >> 20);
        if ((*(s32 *)((s32)record + 8) >> 20) != 20) {
            goto L_02002d3c;
        }
        if ((*(s32 *)((s32)record + 16) >> 20) != 12) {
            goto L_02002d3c;
        }
        v1 = 12;
        goto L_02002af2;
    }
    if (*(s16 *)(0x2000240 + 448) != v2) {
        v6 = 0x2000240;
    } else {
        Func_020057e8(8, 6, (*(s32 *)((s32)record + 8) >> 20), (*(s32 *)((s32)record + 16) >> 20));
        Func_020057f0(9, 6);
        if (*(s16 *)(0x2000402) == 5) {
            if (Value1(Func_02005754, 0x109) == 0) {
                Call3(Func_020057d6, 9, 0x1380000, 0x1480000);
            }
        }
        Func_02005172();
        rec7 = Value1(Func_020057a8, 9);
        record = Value1(Func_020057b0, 9);
        *(s32 *)(rec7 + 12) = *(s32 *)((s32)record + 20);
        if (Value1(Func_02005784, 0x300) != 0) {
            Func_02005808(10, 4);
            *(u8 *)(Func_020057ce(10) + 89) = 254;
            Func_0200517c();
        }
        record = Value1(Func_020057de, 11);
        record[89] = 0;
        *(u8 *)((((s32)record + 89) - 54)) = 0;
        {
            s32 shown = 0;
        
            *(u16 *)((((s32)record + 89) - 54) + 59) = shown;
        }
        v6 = 12;
        *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
        *(u8 *)(*(s32 *)((s32)record + 80) + 38) = (s32)Data_00000000;
        {
            s32 target = *(s32 *)((s32)record + 80);
            s32 shown = 0xc000;
        
            *(u16 *)(target + 30) = shown;
        }
        Func_02005852(11, 0);
        record = Value1(Func_02005818, 12);
        record[89] = (s32)Data_00000000;
        *(u8 *)((((s32)record + 89) - 54)) = (s32)Data_00000000;
        {
            s32 shown = 30;
        
            *(u16 *)((s32)record + 94) = shown;
        }
        *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
        *(u8 *)(*(s32 *)((s32)record + 80) + 38) = (s32)Data_00000000;
        {
            s32 target = *(s32 *)((s32)record + 80);
            s32 shown = 0x4000;
        
            *(u16 *)(target + 30) = shown;
        }
        Func_02005894(12, 0);
        record = Value1(Func_0200585a, 13);
        record[89] = (s32)Data_00000000;
        *(u8 *)((((s32)record + 89) - 54)) = (s32)Data_00000000;
        {
            s32 shown = 60;
        
            *(u16 *)((s32)record + 94) = shown;
        }
        *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
        *(u8 *)(*(s32 *)((s32)record + 80) + 38) = (s32)Data_00000000;
        {
            s32 target = *(s32 *)((s32)record + 80);
            s32 shown = 0x8000;
        
            *(u16 *)(target + 30) = shown;
        }
        Func_020058d0(13, 0);
        record = Value1(Func_02005896, 14);
        record[89] = (s32)Data_00000000;
        *(u8 *)((((s32)record + 89) - 54)) = (s32)Data_00000000;
        {
            s32 shown = 90;
        
            *(u16 *)((s32)record + 94) = shown;
        }
        *(u8 *)(*(s32 *)((s32)record + 80) + 9) |= 12;
        *(u8 *)(*(s32 *)((s32)record + 80) + 38) = (s32)Data_00000000;
        {
            s32 target = *(s32 *)((s32)record + 80);
            s32 shown = 0x8000;
        
            *(u16 *)(target + 30) = shown;
        }
        Func_02005906(14, 0);
        goto L_02002d3c;
    }
    if (*(s16 *)(0x2000240 + 448) == (s32)Data_000000af) {
        if ((u32)(*(s16 *)((v6 + 0x1c2)) - 10) > 7) {
            goto L_02002d3c;
        }
        Call4(Func_020058e2, 0x980, v1, slot1008, slot336);
        if (Value1(Func_020058e2_a, 0x980) == 0) {
            goto L_02002d3c;
        }
        Call6(Func_020058ac, 120, 7, 109, 7, 1, 3);
        L_02002af2:;
        Call6(Func_020058d0_a, 38, 12, 1, 1, 45, 9);
    } else {
        if (v1 != (s32)Data_000000ae) {
        } else {
            *(u8 *)(Func_020059da(8) + 90) &= 254;
            *(u8 *)(Func_020059ec(9) + 90) &= 254;
            Call3(Func_02005a0a, 8, 0x10000, 0x8000);
            Call3(Func_02005a18, 9, 0x10000, 0x8000);
            if (Value1(Func_020059de, 0x109) == 0) {
                if (*(s16 *)((v6 + 0x1c2)) == 1) {
                    Call1(Func_020059fe, 0x301);
                } else {
                    Call1(Func_02005a0e, 0x301);
                }
            }
            if (Value1(Func_02005a04, 0x988) == 0) {
                Call3(Func_02005a82, 10, -0x400000, -0x400000);
                Call3(Func_02005a90, 11, 0x1180000, 0x1280000);
                Call3(Func_02005a9e, 12, 0x1380000, 0xf80000);
                Call3(Func_02005aac, 13, 0x1280000, 0xf80000);
                Call3(Func_02005aba, 14, 0x1400000, 0x1280000);
                Func_02005b2c(11, 0, 0);
                Call3(Func_02005b38, 12, 0xc000, 0);
                Call3(Func_02005b44, 13, 0xc000, 0);
                Call3(Func_02005b50, 14, 0x8000, 0);
                Func_02005a96(5);
            } else {
                if (Value1(Func_02005a86, 0x989) != 0) {
                    Call3(Func_02005b08, 10, 0x1380000, 0x1380000);
                    Call3(Func_02005b7c, 10, 0xb000, 0);
                    Call3(Func_02005b88, 11, 0xb000, 0);
                    Call3(Func_02005b94, 12, 0xb000, 0);
                    Call3(Func_02005ba0, 13, 0xb000, 0);
                    Call3(Func_02005bac, 14, 0xb000, 0);
                    Func_02005af2(5);
                }
            }
            if (Value1(Func_02005ae0, 0x985) != 0) {
                Call3(Func_02005b62, 8, 0x1180000, 0xf00000);
                Call3(Func_02005b70, 9, 0x1480000, 0xf00000);
                Call3(Func_02005be4, 8, 0x8000, 0);
                Func_02005bee(9, 0, 0);
                Call6(Func_02005aea, 81, 14, 4, 1, 17, 14);
            }
            if (*(s16 *)(0x2000402) != 3) {
                goto L_02002d3c;
            }
            record = Value1(Func_02005b40, 0x109);
            if ((s32)record != 0) {
                goto L_02002d3c;
            }
            Func_02003a7c();
            goto L_02002d3c;
        }
        Func_02005bca(12, 2);
    }
    L_02002d3c:;
    return 0;
}

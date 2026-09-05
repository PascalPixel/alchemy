#include "types.h"

#define FieldScene_RunScene3beSequenceB Func_02000df0

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000088[];
extern u8 Data_00000098[];
extern u8 Data_0000009d[];
extern u8 Data_0000009e[];
extern u8 Data_02000240[];
extern u8 Data_0200995c[];
extern u8 Data_02009974[];
extern u8 Data_020099d4[];
extern u8 Data_02009a4c[];
extern u8 Data_02009aac[];
extern u8 Data_02009b3c[];
extern u8 Data_02009b48[];
extern u8 Data_02009bcc[];
extern u8 Data_02009c80[];
extern u8 Data_02009ce0[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
s32 Func_02002234();
s32 Func_0200229c();
s32 Func_020022f2();
s32 Func_02002300();
void Func_0200232a();
s32 Func_02002356();
void Func_02002358();
void Func_02002368();
void Func_02002390();
void Func_02002394();
void Func_0200239e();
s32 Func_020023a0();
void Func_020023d2();
void Func_020023d6();
void Func_020023f0();
void Func_020023f2();
void Func_020023f4();
s32 Func_02002400();
void Func_0200240c();
s32 Func_02002412();
void Func_0200241a();
void Func_0200241c();
void Func_02002434();
void Func_02002444();
void Func_0200244e();
void Func_02002452();
void Func_02002464();
void Func_0200246c();
void Func_0200247a();
void Func_02002492();

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}



void FieldScene_RunScene3beSequenceB(void)
{
    s32 record;

    if (Value1(Func_020022f2, 0x98a) != 0) {
    } else {
        if (Value1(Func_02002300, 0x9a0) == 0) {
        } else {
            Func_0200232a();
            Call3(Func_02002358, 11, 0x10000, 0x8000);
            record = Value1(Func_02002356, 0);
            if (record != 0) {
                Func_02002394(11, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
            }
            Call3(Func_02002390, 11, -8, 16);
            Func_0200239e(11);
            Call3(Func_020023f2, 11, 0xd000, 0);
            Func_02002368(10);
            Func_020023d2(0, 11, 0);
            Call1(Func_020023f0, 0x23da);
            Value2(Func_02002400, 11, 0);
            if (Value2(Func_020023a0, 0, 0) == 0) {
                Func_0200241c(11, 0);
                Func_020023d6(11, 152, 232);
                Call1(Func_0200239e, 0x9a0);
                Func_020023f4(11);
                Func_0200240c(11, 1);
                Data_02000240_t[226][0] = (s32)Data_00000088;
                Data_02000240_t[227][0] = 30;
            } else {
                bump_step(1);
                Func_0200246c(11, 0);
                Func_02002444(11, 2);
                record = Value1(Func_02002412, 0);
                if (record != 0) {
                    Func_02002434(11, *(s16 *)(record + 10), *(s16 *)(record + 18));
                }
                Func_02002452(11);
                Func_02002464(11, 0, 0);
                Func_0200241a(30);
                Func_0200247a(0, 2);
                Func_0200246c(0, 0, 16);
                Func_0200247a(0);
                Func_02002492(0, 1);
            }
            Func_0200244e();
        }
    }
}

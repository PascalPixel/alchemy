#include "types.h"

#define FieldScene_RunOpeningAuxiliarySequence Func_02000d20

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

s32 Func_02000d20(void)
{
    if (Data_02000240_t[224][0] == (s32)Data_00000098) {
        if (Value1(Func_02002234, 0x96f) != 0) {
            return (s32)Data_020099d4;
        }
        return (s32)Data_02009974;
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_0000009d) {
            return (s32)Data_02009a4c;
        } else {
            if (Data_02000240_t[224][0] == (s32)Data_0000009e) {
                return (s32)Data_02009aac;
            } else {
            }
        }
    }
    return (s32)Data_0200995c;
}



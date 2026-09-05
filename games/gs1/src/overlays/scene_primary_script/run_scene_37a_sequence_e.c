#include "types.h"

#define FieldScene_RunScene37aSequenceE Func_02001a58

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
s32 Func_0200401a();
void Func_020040ee();
void Func_0200410c();
void Func_02004132();
void Func_02004162();
s32 Func_02004414();
void Func_02004448();
void Func_0200448c();
void Func_0200449c();
void Func_020044ac();
void Func_020044ba();
void Func_020044c2();
void Func_020044cc();
void Func_020044da();
void Func_020044e6();
void Func_020044ec();
void Func_020044f6();
void Func_020044fa();
void Func_02004500();
void Func_0200451c();
void Func_0200451e();
void Func_02004526();
void Func_0200452a();
void Func_02004534();
void Func_02004538();
void Func_02004544();
void Func_02004552();
void Func_02004556();
void Func_0200455e();
void Func_02004564();
void Func_0200456a();
void Func_0200457e();
void Func_02004588();
s32 Func_0200458e();
void Func_02004594();
void Func_0200459e();
void Func_020045ac();
void Func_020045b0();
void Func_020045c0();
void Func_020045ce();
void Func_020045e0();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunScene37aSequenceE(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02004414, 0x810) != 0) {
    } else {
        if (Value0(Func_0200401a) == 0) {
        } else {
            Func_02004448();
            Call3(Func_0200449c, 16, 0x2410000, 0x930000);
            Call3(Func_02004500, 16, 0x4000, 1);
            Call4(Func_02004538, 0x23e0000, -1, 0xb80000, 1);
            Call1(Func_020044f6, 0x1027);
            Call3(Func_020044ba, 0, 0x240, 232);
            Func_020044da(0, 0);
            Func_0200455e();
            Func_0200448c(10);
            Call3(Func_020044c2, 16, 0x10000, 0x8000);
            Call3(Func_020044e6, 16, 0x240, 152);
            Func_020044ac(6);
            Func_0200451e(16, 6, 30);
            Func_020040ee(16, 6);
            Func_02004526(0, 3);
            Func_020044cc(2);
            Func_02004534(16, 4);
            Func_0200410c(16, 6);
            Call2(Func_0200459e, 0, 0x102);
            Func_020044ec(40);
            Func_02004564(16, 2);
            Func_020044fa(30);
            Func_02004132(16, 6);
            Func_0200456a(0, 3);
            Call3(Func_02004556, 16, 0x240, 184);
            Func_0200451c(6);
            Func_02004594(16, 2);
            Func_0200452a(40);
            Call2(Func_02004162, 0x4010, 6);
            Call3(Func_0200457e, 16, 0x240, 208);
            Func_02004544(40);
            Func_020045ac(0, 3);
            Func_02004552(6);
            Call3(Func_02004588, 16, 0x8000, 0x4000);
            Func_020045c0(16, 2);
            record = Value1(Func_0200458e, 0);
            if (record != 0) {
                Func_020045b0(16, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
            Func_020045ce(16);
            Func_020045e0(16, 0, 0);
            Call1(Func_02004588, 0x810);
            Func_020045ac();
        }
    }
}

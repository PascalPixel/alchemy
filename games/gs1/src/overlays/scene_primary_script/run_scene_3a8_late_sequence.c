#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];
extern volatile s32 Data_0200bd84[];
void Func_02004aba();
void Func_02004c98();
s32 Func_02006376();
void Func_0200652a();
void Func_02006d4e();
void Func_02006d8e();
s32 Func_02006dd2();
void Func_02006dd4();
void Func_02006dde();
s32 Func_02006de4();
void Func_02006de6();
void Func_02006df4();
void Func_02006e94();
s32 Func_02006e96();
void Func_02006ea2();
void Func_02006ecc();
s32 Func_02006ece();
void Func_02006ed4();
s32 Func_02006ed6();
void Func_02006ef2();
s32 Func_02006ef6();
void Func_02006ef8();
void Func_02006f02();
void Func_02006f12();
void Func_02006f22();
void Func_02006f32();
void Func_02006f42();
void Func_02006f52();
void Func_02006f62();
void Func_02006f72();
void Func_02006f82();
s32 Func_02006f88();
void Func_02006f92();
void Func_02006fa2();
void Func_02006fb8();
void Func_02006fbc();
void Func_02006fc0();
void Func_02006fc8();
void Func_02006fd8();
void Func_02006fe8();
void Func_02006ff8();
void Func_02007008();
void Func_02007018();
void Func_02007028();
void Func_02007038();
void Func_02007048();
void Func_02007058();
void Func_02007072();
void Func_02007084();
void Func_020070a0();
void Func_020070ae();
void Func_020070bc();
void Func_020070ea();
void Func_020070ee();
s32 Func_020070f6();
void Func_02007118();
void Func_02007128();
void Func_0200712c();
void Func_0200713a();
void Func_02007140();
void Func_02007150();
void Func_02007160();
void Func_02007170();
void Func_0200717e();
void Func_02007180();
void Func_02007190();
void Func_020071a0();
void Func_020071b0();
void Func_020071c6();
void Func_020071d6();
void Func_020071e6();
void Func_020071f6();
void Func_02007206();
void Func_02007216();
void Func_02007226();
void Func_02007236();
void Func_02007250();
void Func_02007262();
void Func_0200727e();
void Func_0200728c();
void Func_020072cc();
void Func_020072e4();

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunLateSequence(void)
{
    s32 tmp[3];
    s32 record;
    s32 rec;
    s32 idx;
    s32 w;
    s32 w2;
    s32 a;
    s32 *dst;
    s32 zero;
    s32 k;

    record = Value1(Func_02006de4, 0);
    idx = (s32)((u32)*(u16 *)(record + 6) >> 12);
    a = *(s16 *)(record + 10);
    w = Data_0200bd84[idx];
    rec = Value2(Func_02006376,
                 (a + (w >> 16)) >> 4,
                 (*(s16 *)(record + 18) + (s32)(s16)w) >> 4);
    if (rec != 0) {
        zero = 0;
        *(u8 *)(rec + 34) = 2;
        dst = tmp;
        w2 = Data_0200bd84[idx];
        dst[0] = *(volatile s32 *)(rec + 8) + (w2 & -0x10000);
        dst[1] = *(volatile s32 *)(rec + 12);
        dst[2] = *(volatile s32 *)(rec + 16) + (w2 << 16);
        if (Value2(Func_02006dd2, rec, (s32)dst) <= 0) {
            Func_02006d8e(record, 8);
            k = 0x3333;
            Func_02006d4e(15);
            Func_02006fbc(185);
            *(volatile s32 *)(rec + 48) = k;
            *(volatile s32 *)(rec + 52) = k;
            Func_02006dd4(rec, dst[0], dst[1], dst[2]);
            *(volatile s32 *)(record + 48) = k;
            *(volatile s32 *)(record + 52) = k;
            Func_02006de6(record, dst[0], dst[1], dst[2]);
            Func_02006df4(rec);
            Func_02006fd8();
            *(volatile s32 *)(rec + 8) = dst[0];
            *(volatile s32 *)(rec + 16) = dst[2];
            *(s32 *)(rec + 36) = zero;
            *(s32 *)(rec + 44) = zero;
            Func_02006dde(record, 1);
            Func_0200652a();
        }
    }
}


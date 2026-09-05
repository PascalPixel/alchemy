#include "types.h"

#define FieldScene_RunLiftedActorCoordinator Func_020016d4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
void Func_020034de();
s32 Func_020036f4();
void Func_02003918();
s32 Func_02003992();
s32 Func_02004310();
void Func_020045d4();
s32 Func_02004902();
void Func_02005628();
void Func_02005654();
void Func_0200565e();
void Func_020056d6();
void Func_02005702();
s32 Func_0200571c();
void Func_0200571e();
void Func_0200572a();
void Func_0200573a();
void Func_02005744();
void Func_02005750();
void Func_02005752();
void Func_0200575e();
void Func_0200576a();
void Func_0200576c();
void Func_0200576e();
void Func_0200577a();
s32 Func_0200577e();
void Func_020057b6();
void Func_020057c2();
void Func_020057d0();
void Func_020057e0();
void Func_020057ea();
void Func_02005816();
void Func_02005818();
void Func_0200582a();
void Func_0200583c();
void Func_0200583e();
void Func_02005854();
void Func_02005862();
void Func_02005868();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
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

void FieldScene_RunLiftedActorCoordinator(s32 a0)
{
    u32 i;
    s32 rec;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        Func_020034de();
    } else {
        Func_02005628();
        rec = Value2(Func_020036f4, a0, 4);
        if (rec != 0) {
        } else {
            Call1(Func_020056d6, 0x20aa);
            Call2(Func_0200572a, 0x30000, 0x6000);
            Call4(Func_02005744, 0x2180000, -1, 0xf00000, 1);
            Func_02005750();
            Func_0200565e(45);
            Call2(Func_02005752, 0x10000, 0x2000);
            Call4(Func_0200576c, 0x2180000, -1, 0xc00000, 1);
            Func_0200577a();
            Func_0200573a(a0, 0);
            Func_02004310(0, 0x278, 0x108);
            Call3(Func_020056d6, 0, 0x10000, 0x8000);
            Call3(Func_02005702, 0, 0x268, 0x108);
            Value3(Func_0200577e, 0, 0xc000, 20);
            Func_020057ea();
            Call2(Func_020057b6, 0x4000, 0x800);
            Call4(Func_020057d0, 0x2180000, -1, 0xa00000, 1);
            Call3(Func_0200571e, 0, 0x8000, 0x4000);
            Func_0200575e(0, 10);
            record = Func_0200571c(0);
            Func_02005654(record, *(s32 *)(record + 8), (*(s32 *)(record + 12) + 0x400000), *(s32 *)(record + 16));
            Func_0200576a(0);
            Func_0200583e();
            Call4(Func_02005818, -1, -1, -1, 0);
            Func_020057e0(a0, 0);
            Call3(Func_0200576e, 0, 0x18000, 0xc000);
            Value3(Func_02004902, 0, 0x1e8, 248);
            Call3(Func_02005816, 0, 0x4000, 20);
            Func_020057e0(0, 6, 0);
            Call4(Func_02005862, 0x2180000, -1, 0xa00000, 1);
            Func_0200582a(a0, 0);
            Func_020045d4(0);
            Func_02005868(0, 0);
            Func_02003918(a0, 4);
            goto L_02001876;
        }
        if (rec == 1) {
            Call1(Func_0200583c, 0x20a9);
            Func_02005854(a0, 0);
        }
        L_02001876:;
        Value3(Func_02003992, rec, a0, 4);
        Func_020057c2();
    }
}

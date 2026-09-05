#include "types.h"

#define FieldScene_RunMapEntranceCoordinator Func_02001214

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_02000240[];
extern u8 Data_03001ebc[];
extern s16 Data_02000240_t[][1];
void Func_02002d86();
s32 Func_02002f9c();
void Func_020031cc();
s32 Func_02003246();
s32 Func_02003b8c();
void Func_02003e7a();
void Func_02003ece();
void Func_02003ed8();
void Func_02003f1a();
void Func_02003f24();
s32 Func_02004146();
s32 Func_02004150();
s32 Func_0200415c();
void Func_02004c14();
void Func_02004c1e();
void Func_02004c60();
void Func_02004c6a();
void Func_02004ed0();
void Func_02004f34();
void Func_02004f52();
void Func_02004f5a();
void Func_02004f76();
void Func_02004f7e();
void Func_02004fa4();
void Func_02004fb0();
void Func_02004fb4();
void Func_02004fc2();
void Func_02004fdc();
void Func_02004fe8();
void Func_02004ff0();
void Func_02005002();
void Func_02005008();
void Func_0200501e();
void Func_0200502e();
void Func_02005038();
void Func_0200503a();
void Func_02005042();
void Func_02005068();
void Func_02005076();
void Func_02005080();
void Func_0200509c();
void Func_020050b4();
void Func_020050c8();
void Func_020050e8();
void Func_020050fe();
void Func_02005100();

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

void FieldScene_RunMapEntranceCoordinator(s32 a0)
{
    u32 i;
    s32 rec;
    s32 record;

    if (Data_02000240_t[225][0] == 2) {
        Func_02002d86();
    } else {
        Func_02004ed0();
        rec = Value2(Func_02002f9c, a0, 1);
        if (rec != 0) {
        } else {
            Call1(Func_02004f76, 0x208c);
            Call2(Func_02004fc2, 0x30000, 0x6000);
            Call4(Func_02004fdc, 0x1480000, -1, 0xa80000, 1);
            Func_02004fe8();
            Func_02004fb0(a0, 0);
            Value3(Func_02003b8c, 0, 0x118, 200);
            Call3(Func_02004f52, 0, 0x10000, 0x8000);
            Call3(Func_02004f7e, 0, 0x168, 200);
            Func_02004f34(30);
            Call3(Func_02005008, 0, 0x102, 60);
            Func_02004ff0(a0, 0);
            Call3(Func_02004fa4, 0, 0x138, 200);
            Func_02004f5a(30);
            Call3(Func_0200501e, 0, 0xc000, 10);
            Call3(Func_0200503a, 0, 0x106, 60);
            Call3(Func_02004fb4, 0, 0x18000, 0xc000);
            Func_02004146(0, 0x128, 184);
            Value3(Func_02004150, 0, 0x128, 152);
            Value3(Func_0200415c, 0, 0x138, 152);
            Call3(Func_02005068, 0, 0x4000, 15);
            Func_02004c14();
            Func_02003ece(0);
            Func_02004c1e();
            Func_02003ed8(0);
            Call3(Func_02005002, 0, 0x18000, 0xc000);
            Call3(Func_0200502e, 0, 0x130, 184);
            Func_02005038(0, 0x128, 192);
            Func_02005042(0, 0x128, 200);
            Func_020050b4(0, 0, 15);
            Func_02004c60();
            Func_02003f1a(0);
            Func_02004c6a();
            Func_02003f24(0);
            Func_02005080(0, 1);
            Func_020050c8(a0, 0);
            Func_02003e7a(0);
            Func_020050fe(0, 0);
            Call3(Func_0200509c, 9, 0x1380000, 0xa80000);
            Func_020031cc(a0, 1);
            goto L_020013c2;
        }
        if (rec == 1) {
            Call1(Func_020050e8, 0x208b);
            Func_02005100(a0, 0);
        }
        L_020013c2:;
        Value3(Func_02003246, rec, a0, 1);
        Func_02005076();
    }
}

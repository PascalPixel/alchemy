#include "types.h"

#define FieldScene_StagePairedActors Func_02000d5c

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
void Func_0200572a();
void Func_02005734();
void Func_02005738();
void Func_02005770();
void Func_02005780();
u8 *Func_02005794();
void Func_020057a4();
void Func_020057b8();
void Func_020057ba();
void Func_020057c0();
void Func_020057ca();
void Func_020057ea();
void Func_020057f2();
void Func_02005804();
void Func_0200581e();
void Func_02005828();
void Func_0200583a();
void Func_02005842();
void Func_0200584e();
void Func_02005852();
void Func_02005854();
void Func_0200585a();
void Func_02005864();
void Func_0200586e();
void Func_02005876();
void Func_02005886();
void Func_0200588e();
void Func_02005892();
void Func_020058c2();
void Func_020058cc();
void Func_020058d6();
void Func_020058d8();
void Func_020058da();
void Func_020058e2();
void Func_020058ee();
void Func_020058fa();
void Func_0200590a();
void Func_02005916();
void Func_02005932();
void Func_0200593e();
void Func_02005994();
void Func_020059a0();
void Func_020059a2();
void Func_020059ac();
void Func_020059b8();
void Func_020059f4();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

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
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_StagePairedActors(void)
{
    u32 i;
    u8 *record;

    Func_02005876(17);
    Call3(Func_02005734, 0, 0x8000, 0x4000);
    Call3(Func_02005770, 0, 231, 0x1ea);
    Call3(Func_02005804, 0, 0xc000, 30);
    Func_020057a4(0, 3);
    Func_0200572a(180);
    Func_020057ca(0, 2);
    Func_02005738(80);
    Call3(Func_0200583a, 0, 0x101, 60);
    Call3(Func_02005780, 0, 0x8000, 0x4000);
    Call3(Func_020057ba, 0, 246, 0x1df);
    Call3(Func_0200584e, 0, 0xe000, 10);
    record = Func_02005794(0);
    if (record != 0) {
        Func_020057ea(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_020057b8, 1, 0x10000, 0x8000);
    Call3(Func_020057f2, 1, 0x101, 0x1eb);
    Call3(Func_02005886, 0, 0x2000, 0);
    Call3(Func_02005892, 1, 0xa000, 40);
    Func_02005842(0, 2);
    Func_02005852(1, 2);
    Func_020057c0(20);
    Call3(Func_020058c2, 0, 0x101, 0);
    Call3(Func_020058cc, 1, 0x101, 80);
    Func_02005854(0, 3);
    Func_02005864(1, 4);
    Call3(Func_0200581e, 0, 0x13333, 0x9999);
    Call3(Func_02005828, 1, 0x13333, 0x9999);
    Call3(Func_0200585a, 0, 0x109, 0x1c5);
    Call3(Func_0200586e, 1, 0x11a, 0x1d5);
    Func_0200588e(0, 1);
    Call3(Func_0200590a, 0, 0xe000, 0);
    Call3(Func_02005916, 1, 0xe000, 40);
    Call3(Func_02005932, 0, 0x100, 0);
    Call3(Func_0200593e, 1, 0x100, 0);
    Func_020058d8(0, 6, 0);
    Func_020058e2(1, 6, 60);
    Call3(Func_020058d6, 5, 0x1db0000, 0x14c0000);
    Call3(Func_020058e2, 9, 0x1eb0000, 0x14c0000);
    Call3(Func_020058ee, 11, 0x1cb0000, 0x15c0000);
    Call3(Func_020058fa, 10, 0x1fb0000, 0x15c0000);
    Call2(Func_020059a2, 0x73333, 0xe666);
    Call4(Func_020059b8, 0x1e50000, -1, 0x1590000, 1);
    Call3(Func_02005994, 5, 0x6000, 0);
    Call3(Func_020059a0, 9, 0x5000, 0);
    Call3(Func_020059ac, 11, 0x5000, 0);
    Call3(Func_020059b8, 10, 0x5000, 0);
    Func_020059f4();
    Func_020058da(40);
}

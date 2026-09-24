#include "TYPES.H"
#include "EVENT_RUNTIME.H"
extern struct EventRuntime *Data_03001ebc;
void Func_0200a454();
void Func_0200a544();
void Func_0200a554();
void Func_0200a44c();
void Func_0200a4a4();
void Func_0200a4dc();
void Func_0200a3a4();
void Func_0200a514();
void Func_0200a51c();
void Func_0200a474();
void Func_0200a494();
void Func_0200a4bc();
void Func_0200a4c4();
void Func_0200a4f4();
void Func_0200a524();
void Func_0200a4b4();
void Func_0200a54c();
void Func_0200a3fc();
void Func_0200a3bc();
void Func_0200a504();
void Func_0200a4d4();
void Func_0200a4ac();
s32 Func_0200a46c();
void Func_0200a47c();
void Func_0200a49c();
void Func_0200a444();
void Func_0200a45c();
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

void FieldScene_SetupStagedActors(void)
{
    s32 record;

    Func_0200a454();
    Data_03001ebc->value_1c0 = 0x100;
    Data_03001ebc->value_1c8 = 32;
    Func_0200a544();
    Func_0200a554();
    Func_0200a44c(20);
    Call3(Func_0200a4a4, 8, 0x2400000, 0xe80000);
    Func_0200a44c(1);
    Call1(Func_0200a4dc, 0x101a);
    Func_0200a3a4(8, 6);
    Call3(Func_0200a4a4, 8, 0x2400000, 0x1180000);
    Call2(Func_0200a514, 0xcccc, 0x1999);
    Call4(Func_0200a51c, 0x23e0000, -1, 0xb40000, 1);
    Call3(Func_0200a474, 8, 0x10000, 0x8000);
    Call3(Func_0200a494, 8, 0x240, 216);
    Func_0200a44c(20);
    Func_0200a4bc(5, 2, 0);
    Func_0200a44c(30);
    Func_0200a3a4(5, 6);
    Func_0200a4c4(8, 2);
    Func_0200a44c(6);
    Call3(Func_0200a4f4, 8, 0x9000, 0);
    Func_0200a44c(10);
    Call2(Func_0200a514, 0x59999, 0xb333);
    Call4(Func_0200a51c, 0x11f0000, -1, 0xb00000, 1);
    Func_0200a524();
    Func_0200a44c(60);
    Call4(Func_0200a51c, 0x23e0000, -1, 0xb40000, 1);
    Func_0200a524();
    Func_0200a44c(20);
    Func_0200a4b4(8, 3);
    Func_0200a44c(10);
    Call3(Func_0200a4f4, 8, 0xc000, 0);
    Func_0200a44c(10);
    Func_0200a4bc(8, 6, 0);
    Call3(Func_0200a474, 8, 0x30000, 0x20000);
    Call3(Func_0200a494, 8, 0x240, 184);
    Func_0200a44c(40);
    Func_0200a3a4(8, 6);
    Call3(Func_0200a4f4, 8, 0x8000, 0);
    Func_0200a44c(40);
    Data_03001ebc->value_1c0 = 0x202;
    Data_03001ebc->value_1c8 = 16;
    Func_0200a54c();
    Func_0200a554();
    Call4(Func_0200a51c, 0x11f0000, -1, 0xb00000, 0);
    Func_0200a3fc();
    Func_0200a3bc(1);
    Func_0200a544();
    Func_0200a554();
    Func_0200a44c(40);
    Call3(Func_0200a4a4, 8, 0x1a80000, 0xc80000);
    Func_0200a3bc(1);
    Func_0200a3a4(8, 20);
    Func_0200a54c();
    Func_0200a554();
    Call3(Func_0200a4a4, 8, 0x2400000, 0xb80000);
    Call4(Func_0200a51c, 0x23e0000, -1, 0xb40000, 0);
    Func_0200a3fc();
    Func_0200a3bc(1);
    Func_0200a544();
    Func_0200a554();
    Func_0200a44c(20);
    Call2(Func_0200a514, 0x13333, 0x2666);
    Call4(Func_0200a51c, 0x23e0000, -1, 0x9d0000, 1);
    Func_0200a524();
    Func_0200a44c(20);
    Func_0200a4c4(1, 2);
    Func_0200a3a4(1, 20);
    Func_0200a4b4(5, 3);
    Func_0200a3a4(5, 6);
    Func_0200a4c4(8, 2);
    Func_0200a44c(40);
    Func_0200a4b4(8, 4);
    Func_0200a3a4(8, 80);
    Call3(Func_0200a504, 8, 0x102, 0);
    Func_0200a44c(60);
    Call3(Func_0200a4f4, 8, 0xc000, 0);
    Func_0200a44c(30);
    Func_0200a3a4(8, 20);
    Call3(Func_0200a504, 0, 0x101, 0);
    Call3(Func_0200a504, 1, 0x101, 0);
    Call3(Func_0200a504, 5, 0x101, 0);
    Func_0200a44c(60);
    Call3(Func_0200a4f4, 8, 0xc000, 0);
    Func_0200a44c(30);
    Func_0200a4b4(8, 4);
    Func_0200a3a4(8, 6);
    Func_0200a4d4(0, 1, 0);
    Func_0200a44c(40);
    Func_0200a4d4(0, 5, 0);
    Func_0200a44c(40);
    Func_0200a4b4(8, 3);
    Func_0200a3a4(8, 6);
    Call3(Func_0200a4f4, 0, 0x4000, 0);
    Call3(Func_0200a4f4, 5, 0x4000, 0);
    Call3(Func_0200a4f4, 1, 0x4000, 0);
    Func_0200a44c(40);
    Func_0200a4ac(1, 3);
    Func_0200a4ac(5, 3);
    Func_0200a4b4(0, 3);
    Func_0200a44c(20);
    Func_0200a4b4(8, 3);
    Func_0200a3a4(8, 6);
    Call2(Func_0200a514, 0x19999, 0x3333);
    Call4(Func_0200a51c, 0x2400000, -1, 0xd70000, 1);
    Call3(Func_0200a4f4, 8, 0x4000, 0);
    Func_0200a44c(10);
    Func_0200a4bc(8, 6, 0);
    Call3(Func_0200a494, 8, 0x240, 217);
    Func_0200a44c(20);
    Call3(Func_0200a494, 8, 0x240, 0x141);
    Func_0200a4a4(8, 0, 0);
    Call2(Func_0200a514, 0x39999, 0x7333);
    Call4(Func_0200a51c, 0x2400000, -1, 0x880000, 1);
    Func_0200a524();
    Func_0200a44c(20);
    Call3(Func_0200a474, 1, 0x10000, 0x8000);
    Call3(Func_0200a474, 5, 0x10000, 0x8000);
    Func_0200a4ac(1, 2);
    record = Value1(Func_0200a46c, 0);
    if (record != 0) {
        Func_0200a47c(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0200a49c(1);
    Func_0200a4a4(1, 0, 0);
    Func_0200a4ac(5, 2);
    record = Value1(Func_0200a46c, 0);
    if (record != 0) {
        Func_0200a47c(5, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0200a49c(5);
    Func_0200a4a4(5, 0, 0);
    Call1(Func_0200a444, 0x12f);
    Data_03001ebc->value_1c0 = 0x204;
    Data_03001ebc->value_1c8 = 16;
    Func_0200a45c();
}

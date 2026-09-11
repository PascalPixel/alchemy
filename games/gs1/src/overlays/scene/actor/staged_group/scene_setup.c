#include "types.h"

#define FieldScene_RunStepWithValue29df Func_020001e4
#define FieldScene_RunStepWithValueFd6 Func_0200020c
#define FieldScene_RunSlotZeroFacingSequence Func_020002e8
#define RunSceneArrivalSetup Func_02000504
#define FieldScene_DrawTilesAndRaiseActor11 Func_02003970
struct Obj {
    u8 filler00[6];
    u16 f06;
};

typedef struct Effect {
    unsigned char pad00[0xC];
    s32 y;
    unsigned char pad10[0x13];
    s8 state23;
} Effect;

extern u8 *Data_03001ebc;

void Func_02003ddc(void);
void Func_02003dac(s32, s32);
void Func_02003db4(s32, s32);
void Func_02003df8(void);
void Func_02003e04(void);
s32 Func_02003e8e(s32, s32, s32);
void Func_02003dfc(s32);
void Func_02003f6c(s32, s32);
s32 Func_02003e3c(s32, s32);
void Func_02003e30(void);
struct Obj *Func_02003f0a(s32);
void Func_02003ef6(void);
s32 Func_02003fb0(s32, s32, s32);
void Func_02003efe(s32);
void Func_02003fd4(s32);
s32 Func_02003fe4(s32, s32);
s32 Func_02003f3c(s32, s32);
void Func_02003fb8(s32, s32);
void Func_02004008(s32, s32);
void Func_02003fda(s32, s32);
void Func_0200402a(s32, s32);
void Func_02003f5e(void);
void Func_020040fe();
void Func_0200428c();
void Func_020040be();
void Func_0200403c();
void Func_020040d4();
void Func_02004052();
void Func_02004178();
u8 *Func_0200416e();
void Func_020041dc();
void Func_020041d8();
void Func_0200415e();
void Func_0200429c();
void Func_020042b0();
void Func_020042bc();
void Func_02004180();
void Func_02007532(s32, s32, s32, s32, s32, s32);
void Func_02007544(s32, s32, s32, s32, s32, s32);
void Func_02007560(Effect *, s32);
Effect *Func_02007594(s32);
void Func_0200759e(s32);

/* Contiguous unnamed leaf-owner run for resource_3a8. */

/* resource_3a8 owner at 0x02000504, 138 bytes. Scene arrival sequence. */

static __inline__ void SetOffset(s32 actor, s32 axis, s32 offset)
{
    Func_020041d8(actor, axis, offset);
}

void FieldScene_RunStepWithValue29df(void)
{
    Func_02003ddc();
    Func_02003dac(0x947, 1);
    Func_02003db4(0x29df, 1);
    Func_02003df8();
}

void FieldScene_RunStepWithValueFd6(void)
{
    Func_02003e04();
    Func_02003e8e(12, 0, 0);
    Func_02003dfc(0xfd6);
    Func_02003f6c(181, 3);
    Func_02003e3c(181, 0);
    Func_02003e30();
}

void FieldScene_RunSlotZeroFacingSequence(void)
{
    struct Obj *o;
    u32 v;
    u16 *q;

    o = Func_02003f0a(0);
    v = (o->f06 + 0xfffff000) << 16;
    if (v > 0x60000000) {
        Func_02003ef6();
        Func_02003fb0(0, 8, 0);
        Func_02003efe(10);
        Func_02003fd4(0x2584);
        Func_02003fe4(8, 0);
        if (Func_02003f3c(0, 0) == 0) {
            Func_02003fb8(8, 4);
            Func_02004008(8, 0);
        } else {
            q = (u16 *)(Data_03001ebc + 472);
            *q = *q + 1;
            Func_02003fda(8, 3);
            Func_0200402a(8, 0);
        }
        Func_02003f5e();
    }
}

void RunSceneArrivalSetup(void)
{
    s32 two = 2;

    Func_020040fe();
    Func_0200428c(188);
    Func_020040be(36, 23, 43, 12, two, two);
    Func_0200403c(5);
    Func_020040d4(39, 23, 43, 12, two, two);
    Func_02004052(5);
    Func_02004178(0, 0x8000, 0x4000);
    Func_0200416e(0)[85] = 0;
    Func_020041dc(0, 2);
    SetOffset(0, 0, -8);
    Func_0200415e(10);
    Func_0200429c(2);
    Func_020042b0();
    Func_020042bc();
    Func_02004180();
}

void FieldScene_DrawTilesAndRaiseActor11(void)
{
    Effect *effect = Func_02007594(11);

    Func_02007532(0, 0, 1, 1, 9, 14);
    Func_02007544(0, 0, 1, 1, 9, 45);
    if (effect != 0) {
        Func_02007560(effect, 0);
        effect->y -= 0x200000;
        effect->state23 = 2;
    }
    Func_0200759e(0x201);
}

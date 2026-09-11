#include "types.h"

#define F(base, type, off) (*(type *)((u8 *)(base) + (off)))
#define FieldScene_RunActorElevenAtTile5And13 Func_02001420
#define SceneActor_UseActorNinePositionWithYOffset Func_020022c4

extern u8 Data_0200e010[];
extern u8 Value_00000874;

u8 *Func_02006efc(s32);
u8 *Func_02006f0e(s32);
void Func_02006efe(void);
u8 *Func_02006f2c(s32);
u8 *Func_02006f3a_a(s32);
u8 *Func_02006f3a_b(s32);
u8 *Func_02006f42(s32);
void Func_02006e88(s32, s32, s32, s32, s32, s32);
void Func_020070ee(s32);
void Func_02006e90(s32, s32, s32);
void Func_02006ec4(s32, s32, s32, s32, s32, s32);
void Func_02006f6e(void);
s32 *Func_02007d9e(s32);
u32 Func_02007c64(void);
void Func_02005e06(s32, s32, s32);

void FieldScene_RunActorElevenAtTile5And13(void)
{
    s32 x;
    s32 y;
    u8 *p;

    x = F(Func_02006efc(11), s32, 8) / 0x100000;
    y = F(Func_02006f0e(11), s32, 16) / 0x100000;
    Func_02006efe();
    if (x == 5 && y == 13) {
        F(Func_02006f2c(11), s32, 12) += 0xfffe0000;
        p = Func_02006f3a_a(11);
        F(p, s32, 0x3c) = F(Func_02006f42(11), s32, 12);
        Func_02006e88(5, 2, 5, 11, 1, 1);
        Func_020070ee(0xd9);
        Func_02006e90((s32)Data_0200e010, 9, 7);
        {
            s32 s0 = 9;
            s32 s1 = 10;
            Func_02006ec4(9, 5, 1, 1, s0, s1);
        }
        Func_02006f3a_b((s32)&Value_00000874);
    }
    Func_02006f6e();
}

void SceneActor_UseActorNinePositionWithYOffset(void)
{
    s32 *p = Func_02007d9e(9);
    u32 v = Func_02007c64();

    s32 b = p[3] + (((v << 2) >> 16) << 16);
    s32 c = p[4];

    Func_02005e06(p[2], b, c);
}

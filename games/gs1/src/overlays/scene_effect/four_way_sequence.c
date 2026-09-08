#include "types.h"

#define FieldScene_RunFourWayEffectSequence Func_020029a4

void Func_0200b4f4(s32);
void Func_0200b5ec(s32, s32);
void Func_0200b5f4(s32);
void Func_0200b624(s32);
u8 *Func_0200b454(s32, s32, s32, s32);
void Func_0200b444(u8 *, s32);
void Func_0200b3e4(s32);
void Func_0200b5ac(s32, s32, s32);
s32 Func_0200b3ec(void (*)(), s32);
void Func_0200b3f4(void (*)());
void Func_02002ba0(void);
extern s32 Data_0200b684[][2];
extern s32 Data_0200bb40[];
extern u8 *Data_0200bb10[];

typedef struct {
    u8 unk[9];
    u8 unk0 : 2;
    u8 mode : 2;
    u8 unk4 : 4;
} SpriteMode;

static __inline__ void Call3(void (*f)(s32, s32, s32), s32 a, s32 b, s32 c)
{
    f(a, b, c);
}

void FieldScene_RunFourWayEffectSequence(u32 mode)
{
    u32 i, zero;
    s32 x, y, z;
    s32 *pos;
    u8 *obj, *sprite;

    for (i = 0; i <= 15; i++)
        Func_0200b4f4(i + 16);
    switch (mode) {
    case 0: Func_0200b5ec(0x4039d2, 1); break;
    case 1: Func_0200b5ec(0x4049d2, 1); break;
    case 2: Func_0200b5ec(0x404a4e, 1); break;
    case 3: Func_0200b5ec(0x403a52, 1); break;
    }
    Func_0200b5f4(60);
    Func_0200b624(214);
    i = 0;
    zero = i;
    for (pos = &Data_0200b684[0][0]; i <= 9; i++, pos += 2) {
        x = pos[0];
        y = pos[1];
        z = 0;
        switch (mode) {
        case 0: x += 0xe80000; z = 0x900000; break;
        case 1: x += 0xe80000; z = 0x1d00000; break;
        case 2: x += 0x2c70000; z = 0x900000; break;
        case 3: x += 0x2c70000; z = 0x1d00000; break;
        }
        Data_0200bb40[i] = zero;
        obj = Func_0200b454(284, x, y, z);
        Data_0200bb10[i] = obj;
        obj[85] = zero;
        sprite = *(u8 **)(obj + 80);
        sprite[38] = zero;
        ((SpriteMode *)sprite)->mode = 1;
        Func_0200b444(obj, 6);
        Func_0200b3e4(6);
    }
    if (mode == 0) {
        Call3(Func_0200b5ac, 0, 256, 0);
        Call3(Func_0200b5ac, 1, 256, 0);
    }
    Func_0200b3e4(20);
    Func_0200b3ec(Func_02002ba0, 3200);
    Func_0200b624(246);
    Data_0200bb40[0] = 1; Func_0200b3e4(6);
    Data_0200bb40[1] = 1; Func_0200b3e4(6);
    Data_0200bb40[2] = 1; Func_0200b3e4(6);
    Data_0200bb40[3] = 1; Func_0200b3e4(6);
    Data_0200bb40[4] = 1; Func_0200b3e4(6);
    Data_0200bb40[5] = 1; Func_0200b3e4(6);
    Data_0200bb40[6] = 1; Func_0200b3e4(6);
    Data_0200bb40[7] = 1; Func_0200b3e4(6);
    Data_0200bb40[8] = 1; Func_0200b3e4(6);
    Data_0200bb40[9] = 1; Func_0200b3e4(6);
    for (;;) {
        for (i = 0; i <= 9; i++) {
            if (Data_0200bb40[i] != 0) {
                i = 888;
                break;
            }
        }
        if (i != 888)
            break;
        Func_0200b3e4(1);
    }
    Func_0200b3e4(40);
    Func_0200b3f4(Func_02002ba0);
    Func_0200b5ec(65536, 1);
    Func_0200b5f4(40);
}

#include "types.h"

struct Effect12Vector {
    s32 values[3];
};

extern u8 *Data_03001f30;
void Func_08098698(void);
void Func_080f9010(s32);
u32 Func_08004458(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Func_08004458
void Func_0800447c(s32, s32, struct Effect12Vector *);
void *Func_08096c80(s32, s32, s32, s32);
void Func_08009098(void *, const void *);
void Func_080030f8(u32);
void Func_0809748c(void);
extern const u8 Data_0809f11c[];

#define RunBattleEffect12 Func_080985fc
void RunBattleEffect12(void)
{
    u8 *state = Data_03001f30;
    struct Effect12Vector position;
    struct Effect12Vector *positionPointer;
    s32 count;

    Func_08098698();
    Func_080f9010(0x86);
    positionPointer = &position;
    count = 4;
    do {
        void *object;
        s32 random;

        positionPointer->values[0] = *(s32 *)(state + 4);
        positionPointer->values[2] = *(s32 *)(state + 12);
        random = (Rand() * 6) + 0x40000;
        Func_0800447c(random, Rand(), positionPointer);
        positionPointer->values[1] = *(s32 *)(state + 8);
        object = Func_08096c80(
            0xD9,
            positionPointer->values[0],
            positionPointer->values[1],
            positionPointer->values[2]
        );
        if (object != 0) {
            Func_08009098(object, Data_0809f11c);
            *((u8 *)object + 0x55) = 2;
        }
        Func_080030f8((((u32)Rand() * 2) >> 16) + 2);
        count--;
    } while (count >= 0);
    Func_080030f8(0x1E);
    Func_0809748c();
}

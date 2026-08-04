#include "types.h"

typedef struct {
    u8 filler0[12];
    s32 unkC;
    u8 filler10[8];
    s32 unk18;
    s32 unk1C;
    u8 filler20[28];
    s32 unk3C;
} Ent;

extern s32 Data_0200bb40[];
extern Ent *Data_0200bb10[];

void Func_02002ba0(void)
{
    u32 i;
    s32 v;
    Ent *p;

    for (i = 0; i <= 9; i++) {
        v = Data_0200bb40[i];
        if (v != 0) {
            p = Data_0200bb10[i];
            if ((u32)v <= 8) {
                p->unk18 += -0x1ccc;
                p->unk1C += 0x8000;
                p->unkC += 0x4ccc;
                p->unk3C += 0x4ccc;
            } else {
                p->unkC += 0x140000;
                p->unk3C += 0x140000;
            }
            v = Data_0200bb40[i] + 1;
            Data_0200bb40[i] = v;
            if ((u32)v > 14) {
                Data_0200bb40[i] = 0;
            }
        }
    }
}

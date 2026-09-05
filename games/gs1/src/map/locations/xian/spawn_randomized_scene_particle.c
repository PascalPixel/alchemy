#include "types.h"

extern u8 *Func_020069c4(s32 arg0);
extern void Func_02006ac8(s32 id, s32 arg1);
extern void Func_02006ad2(s32 id, s32 arg1);
extern s32 Func_02006928(void);
extern s32 Func_02006944(void);
extern void Func_02002766(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6,
                          u32 flags, u8 *extra);

extern s32 Data_03001e40;

struct Params {
    s32 unused0;
    s32 field1;
    s32 field2;
    s32 field3;
    u8 pad[24];
};

void FieldScene_SpawnRandomizedParticle(void)
{
    struct Params params;
    u8 *record;
    s32 draw;
    s32 offset;

    record = Func_020069c4(0);

    params.field1 = 7;
    draw = (u32)(Func_02006928() * 7) >> 16;
    if ((draw & 7) == 0)
        params.field1 = 5;

    params.field2 = 0xb333;
    params.field3 = 0xcccc;

    offset = ((u32)(Func_02006944() * 8) >> 16) * 13107;

    Func_02002766(*(s32 *)(record + 8) + ((8 - (Data_03001e40 & 15)) << 16),
                  *(s32 *)(record + 12) + (192 << 13),
                  *(s32 *)(record + 16),
                  0,
                  -offset,
                  0,
                  144 << 12,
                  (u8 *)&params);

    if ((Data_03001e40 & 1) != 0)
        Func_02006ac8(0, 15);
    else
        Func_02006ad2(0, 1);
}

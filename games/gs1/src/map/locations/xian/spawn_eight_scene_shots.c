#include "types.h"

#define FieldScene_SpawnEightShots Func_02004140

extern u8 *Func_0200854c(s32 id);
extern void Func_02008540(s32 frames);
extern void Func_020086f6(s32 sound_id);
extern void Func_020042d6(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
extern void Func_02008578(s32 frames);

struct Descriptor {
    u32 field0;
    u8 unused[12];
    u32 field16;
    u32 field20;
    u16 field24;
    u16 unused26;
    u32 field28;
    u8 unused32[8];
};

void FieldScene_SpawnEightShots(void)
{
    struct Descriptor descriptor;
    u8 *record;
    u32 i;

    record = Func_0200854c(8);
    descriptor.field0 = 1;
    descriptor.field24 = 0x0119;
    descriptor.field28 = 0x0200d1d8;
    descriptor.field16 = 224 << 10;
    descriptor.field20 = 192 << 9;
    for (i = 0; i <= 7; i++) {
        Func_02008540(10);
        if (i & 1) {
            Func_020086f6(0x82);
        }
        Func_020042d6(*(s32 *)(record + 8), *(s32 *)(record + 12),
                      *(s32 *)(record + 16) + 0xffe80000, 0,
                      0x9999, 0, 0x00360001, (u8 *)&descriptor);
    }
    Func_02008578(60);
}

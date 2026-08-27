#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern void *volatile Data_03001f2c;

s32 Func_080770c0(s32);
void Func_080770d0(s32);
void Func_08015270(s32);
void Func_080030f8(s32);
void Func_08015080(s32, s32, s32, s32);
void Func_080a2268(s32, s32, s32, s32, s32, s32);

s32 Func_080a6a98(void *arg0, void *arg1, void *ctx)
{
    void *state = Data_03001f2c;
    s32 row;
    s32 spacing;

    M2C_FIELD(ctx, u32, 24) = M2C_FIELD(ctx, u32, 8) * 5 + M2C_FIELD(ctx, u32, 16);

    if (Func_080770c0(0x151) != 0) {
        Func_080770d0(0x2ff);
    } else {
        u16 tableVal;

        Func_08015270(M2C_FIELD(state, u32, 44));
        Func_080030f8(1);

        tableVal = *(u16 *)((u8 *)state + M2C_FIELD(ctx, u32, 24) * 2 + 456);
        if (tableVal != 0) {
            Func_08015080((tableVal & 0x1ff) + 0x53a, M2C_FIELD(state, u32, 44), 0, 0);
        }
    }

    {
        s32 selected = 1;

        for (row = 0, spacing = 1; row <= 4; row++, spacing += 2) {
            if (row == M2C_FIELD(ctx, u32, 16)) {
                Func_080a2268(M2C_FIELD(state, u32, 32), 0, spacing, 15, selected, 14);
            } else {
                Func_080a2268(M2C_FIELD(state, u32, 32), 0, spacing, 15, selected, 15);
            }
        }
    }

    Func_080030f8(1);
    return 1;
}

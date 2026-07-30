#include "types.h"

#define OFFSET_OF(type, member) ((u32)&(((type *)0)->member))

struct OwnerState_08077f70 {
    u8 padding00[0x14];
    u16 scaledX;
    u16 scaledY;
    u8 padding18[0x1c];
    u16 baseX;
    u16 baseY;
    u16 sourceX;
    u16 sourceY;
    u8 padding3c[0x9c];
    u16 entries[15];
};

typedef char Assert_08077f70_scaled_x[
    OFFSET_OF(struct OwnerState_08077f70, scaledX) == 0x14 ? 1 : -1];
typedef char Assert_08077f70_scaled_y[
    OFFSET_OF(struct OwnerState_08077f70, scaledY) == 0x16 ? 1 : -1];
typedef char Assert_08077f70_base_x[
    OFFSET_OF(struct OwnerState_08077f70, baseX) == 0x34 ? 1 : -1];
typedef char Assert_08077f70_source_y[
    OFFSET_OF(struct OwnerState_08077f70, sourceY) == 0x3a ? 1 : -1];
typedef char Assert_08077f70_entries[
    OFFSET_OF(struct OwnerState_08077f70, entries) == 0xd8 ? 1 : -1];

void Func_08079374(s32 flag);
s32 Func_08079358(s32 flag);
void Func_08079ae8(s32 owner);
s32 Func_08077428(s32 owner);
struct OwnerState_08077f70 *Func_08077394(s32 owner);
s32 Func_080022ec(s32 value, s32 divisor);
s32 Func_08078708(s32 owner, s32 index);
s32 Func_08078e28(s32 owner, s32 value);

void Func_08077f70(void)
{
    s32 owner;

    Func_08079374(32);
    Func_08079374(33);
    Func_08079358(0x1ff);
    Func_08079ae8(5);
    Func_08077428(5);
    Func_08079374(0x901);
    Func_08079358(0x11a);

    for (owner = 0; owner <= 1; owner++) {
        struct OwnerState_08077f70 *state = Func_08077394(owner);
        s32 value;
        s32 index;

        state->sourceX = state->baseX;
        state->sourceY = state->baseY;

        value = Func_080022ec(
            (s16)state->sourceX << 14, (s16)state->baseX);
        if (value > 0x4000)
            value = 0x4000;
        else if (value < 0)
            value = 0;
        state->scaledX = value;
        if ((s16)state->scaledX == 0 && (s16)state->sourceX != 0)
            state->scaledX = 1;

        value = Func_080022ec(
            (s16)state->sourceY << 14, (s16)state->baseY);
        if (value > 0x4000)
            value = 0x4000;
        else if (value < 0)
            value = 0;
        state->scaledY = value;
        if ((s16)state->scaledY == 0 && (s16)state->sourceY != 0)
            state->scaledY = 1;

        for (index = 0; index < 15; index++) {
            if ((state->entries[index] & 0x1ff) == 15) {
                state->entries[index] = 0x10;
                Func_08078708(owner, index);
                break;
            }
        }

        Func_08079ae8(owner);
        Func_08077428(owner);
    }

    Func_08078e28(0, 140);
    Func_08078e28(0, 149);
    Func_08078e28(1, 140);
    Func_08078e28(2, 141);

    *(volatile u32 *)0x02000250 += 300;
}

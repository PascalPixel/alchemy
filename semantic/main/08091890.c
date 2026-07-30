#include "types.h"

#define OFFSET_OF(type, member) ((u32)&(((type *)0)->member))

struct OwnerState_08091890 {
    u8 padding00[0x14];
    u16 scaledX;
    u16 scaledY;
    u8 padding18[0x1c];
    u16 baseX;
    u16 baseY;
    u16 sourceX;
    u16 sourceY;
    u8 padding3c[0xf5];
    u8 clear131;
};

struct PartyState_08091890 {
    u8 padding000[0x1f4];
    u8 fallbackOwner;
    u8 padding1f5[3];
    u8 activeOwners[8];
};

typedef char Assert_08091890_scaled_x[
    OFFSET_OF(struct OwnerState_08091890, scaledX) == 0x14 ? 1 : -1];
typedef char Assert_08091890_base_x[
    OFFSET_OF(struct OwnerState_08091890, baseX) == 0x34 ? 1 : -1];
typedef char Assert_08091890_source_y[
    OFFSET_OF(struct OwnerState_08091890, sourceY) == 0x3a ? 1 : -1];
typedef char Assert_08091890_clear[
    OFFSET_OF(struct OwnerState_08091890, clear131) == 0x131 ? 1 : -1];
typedef char Assert_08091890_fallback[
    OFFSET_OF(struct PartyState_08091890, fallbackOwner) == 0x1f4 ? 1 : -1];
typedef char Assert_08091890_active[
    OFFSET_OF(struct PartyState_08091890, activeOwners) == 0x1f8 ? 1 : -1];

s32 Func_08077168(s32 owner);
void Func_08091858(void);
struct OwnerState_08091890 *Func_08077008(s32 owner);
s32 Func_080022ec(s32 value, s32 divisor);
s32 Func_08077148(void);

void Func_08091890(s32 owner)
{
    struct PartyState_08091890 *party =
        (struct PartyState_08091890 *)0x02000240;
    struct OwnerState_08091890 *state;
    s32 value;
    s32 activeCount;
    s32 count;
    s32 index;

    Func_08077168(owner);
    Func_08091858();
    state = Func_08077008(owner);

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

    state->clear131 = 0;

    count = Func_08077148();
    activeCount = 0;
    for (index = 0; index < count; index++) {
        state = Func_08077008(party->activeOwners[index]);
        if ((s16)state->sourceX != 0)
            activeCount++;
    }

    if (activeCount == 0) {
        state = Func_08077008(party->fallbackOwner);
        state->sourceX = 1;

        value = Func_080022ec(1 << 14, (s16)state->baseX);
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
    }
}

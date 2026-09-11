#include "types.h"
#include "scene.h"
#include "abi/graphics/palette/set_tint_channels_from_counters.h"

s32 Modulo(s32, s32);
extern u8 gCell[];
extern u8 gRom[];
void GraphicsPalette_SetTintChannelsFromCounters(void *work)
{
    s16 phase; s32 bias; s32 c2, c0, c1;
    phase = Modulo(gCell[0x205] + 0xC, 0x18) * 4;
    bias = gCell[0x206] - 7;
    c0 = gRom[(s16)Modulo(phase, 0x60)];
    c1 = gRom[Modulo(phase + 0x20, 0x60)];
    c2 = gRom[Modulo(phase + 0x40, 0x60)];
    c0 += bias; c1 += bias; c2 += bias;
    if (c0 < 0) c0 = 0; if (c1 < 0) c1 = 0; if (c2 < 0) c2 = 0;
    if (c0 > 0x1F) c0 = 0x1F; if (c1 > 0x1F) c1 = 0x1F; if (c2 > 0x1F) c2 = 0x1F;
    FIELD_AT_OFFSET(work, s16 *, 0x576) = (s16)c0;
    FIELD_AT_OFFSET(work, s16 *, 0x578) = (s16)c1;
    FIELD_AT_OFFSET(work, s16 *, 0x57A) = (s16)c2;
}

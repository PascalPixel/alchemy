#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
s32 Modulo(s32, s32);
extern u8 Data_02000240[];
extern u8 Data_08036750[];
void GraphicsPalette_SetTintChannelsFromCounters(void *work) {
    s16 phase; s32 bias; s32 c2, c0, c1;
    phase = Modulo(Data_02000240[0x205] + 0xC, 0x18) * 4;
    bias = Data_02000240[0x206] - 7;
    c0 = Data_08036750[(s16) Modulo(phase, 0x60)];
    c1 = Data_08036750[Modulo(phase + 0x20, 0x60)];
    c2 = Data_08036750[Modulo(phase + 0x40, 0x60)];
    c0 += bias; c1 += bias; c2 += bias;
    if (c0 < 0) c0 = 0; if (c1 < 0) c1 = 0; if (c2 < 0) c2 = 0;
    if (c0 > 0x1F) c0 = 0x1F; if (c1 > 0x1F) c1 = 0x1F; if (c2 > 0x1F) c2 = 0x1F;
    FIELD_AT_OFFSET(work, s16 *, 0x576) = (s16) c0;
    FIELD_AT_OFFSET(work, s16 *, 0x578) = (s16) c1;
    FIELD_AT_OFFSET(work, s16 *, 0x57A) = (s16) c2;
}

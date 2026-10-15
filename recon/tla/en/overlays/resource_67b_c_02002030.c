#include "../../../../games/tla/src/scene/aqua_rock/local.h"

struct AquaRockSpray {
    s32 unknown_00;
    s32 kind;
    s32 scale_x;
    s32 scale_y;
    u8 unknown_10[24];
};

void Func_0200015c(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 d, struct AquaRockSpray *spray);

s32 Func_02002030(struct ObjectRuntime *object)
{
    struct AquaRockSpray spray;

    if (gFrameCount & 1) {
        s32 drift;

        spray.kind = 7;
        if (!(gFrameCount & 2))
            spray.kind = 5;
        spray.scale_x = 0xcccc;
        spray.scale_y = 0xcccc;
        spray.unknown_00 = 0;
        drift = ((u32)(Random16Far() << 3) >> 16) * 0x3333;
        Func_0200015c(object->x + ((8 - (gFrameCount & 15)) << 16), object->y + 0x1a0000, object->z, 0,
                      -drift, 0, 0xb0000, &spray);
    }
    return 0;
}

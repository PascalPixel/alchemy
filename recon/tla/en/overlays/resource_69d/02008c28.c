#include "types.h"
#include "field_event.h"
#include "field_effect.h"

extern u32 gFrameCount;

u32 Random16Far(void);
void Func_02001258(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ,
                   u32 flags, const struct EffectOptions *options);

s32 Func_02000c28(struct FieldActor *object)
{
    struct EffectOptions options;
    s32 x;
    s32 y;
    s32 velocityY;

    if ((gFrameCount & 7) == 0) {
        options.priority = 3;
        options.startScaleX = 0xb333;
        options.startScaleY = 0xb333;
        options.palette = 14;
        x = object->x + ((((Random16Far() * 9) >> 16) - 4) << 16);
        y = object->y + ((32 - ((Random16Far() * 20) >> 16)) << 16);
        velocityY = ((((Random16Far() * 5) >> 16) << 16) + 0x50000) / 10;
        Func_02001258(x, y, object->z, 0, velocityY, 0,
                      EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE, &options);
    }
    return 0;
}

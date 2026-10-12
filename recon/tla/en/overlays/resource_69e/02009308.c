#include "types.h"
#include "field_effect.h"

extern u32 gFrameCount;

u32 Random16Far(void);
void Func_02001c88(s32 x, s32 y, s32 z, s32 velocity_x, s32 velocity_y, s32 velocity_z,
    u32 flags, const struct EffectOptions *options);

s32 Func_02001308(struct FieldActor *object)
{
    struct EffectOptions options;
    s32 phase, x, y, speed;

    phase = gFrameCount & 7;
    if (phase != 0)
        goto done;
    options.priority = 3;
    options.start_scale_x = 0xb333;
    options.start_scale_y = 0xb333;
    options.palette = 14;
    x = object->x + (((s32)((u32)(Random16Far() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - (s32)((u32)(Random16Far() * 20) >> 16)) << 16);
    speed = (((s32)((u32)(Random16Far() * 5) >> 16) << 16) + 0x50000) / 10;
    Func_02001c88(x, y, object->z, 0, speed, phase,
        EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE, &options);
done:
    return 0;
}

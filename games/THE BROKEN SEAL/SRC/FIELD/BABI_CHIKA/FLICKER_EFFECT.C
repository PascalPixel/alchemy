#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"

/* Switch the object between animations 1 and 2 every two frames, and every fourth frame spawn a small palette-5 effect 16 pixels up its y axis, within three pixels of it in x and z. */
s32 BabiChika_UpdateFlickerEffect(struct FieldActor *object)
{
    struct EffectOptions options;
    s32 x;
    s32 z;

    if (gFrameCount & 2)
        Engine_ObjectSetAnimation(object, 1);
    else
        Engine_ObjectSetAnimation(object, 2);
    if (gFrameCount & 3)
        return 0;
    options.start_scale_x = 0x4ccc;
    options.start_scale_y = 0x4ccc;
    options.palette = 5;
    x = object->x.fixed + ((((u32)(Engine_RandomNext() * 7) >> 16) - 3) << 16);
    z = object->z.fixed + ((((u32)(Engine_RandomNext() * 7) >> 16) - 3) << 16);
    Effect_Spawn(x, object->y.fixed + 0x100000, z, 0, 0, 0, 0x90001, &options);
    return 0;
}

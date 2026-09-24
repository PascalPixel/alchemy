#include "TYPES.H"
#include "FIELD_EFFECT.H"

/* The spray's motion script. */
extern const s32 Data_0200d2cc[];

/* Every fourth frame, sprays a rising effect (type 286) from a random point
 * within three cells of actor, with cue 246 every eighth frame. */
s32 VinasuHeya_Func02004968(struct FieldActor *actor)
{
    struct EffectOptions options;
    struct EffectOptions *o = &options;
    s32 phase;
    s32 x, y, z, rise;

    o->priority = 1;
    o->palette = 5;
    o->type = 286;
    o->script = Data_0200d2cc;
    phase = *(volatile u32 *)&gFrameCount & 3;
    if (phase == 0) {
        if ((*(volatile u32 *)&gFrameCount & 7) == 0) {
            Engine_AudioPlayCue(246);
        }
        x = actor->x.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
        y = actor->y.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
        z = actor->z.fixed + ((((u32)(Engine_RandomNext() * 49) >> 16) - 24) << 16);
        rise = (((u32)(Engine_RandomNext() * 4) >> 16) << 15) + 0x8000;
        Effect_Spawn(x, y, z, 0, rise, phase, 0x330000, o);
    }
    return 0;
}

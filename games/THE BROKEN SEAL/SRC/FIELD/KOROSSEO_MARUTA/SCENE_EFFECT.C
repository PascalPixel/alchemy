/* Spawning and raising the stage's scene effects. */
#include "LOG_ROLLING.H"

void ColossoLogRollingStage_SpawnRandomSceneEffect(StageEffect *source)
{
    extern void Vector_AddPolarOffset(s32, s32, s32 *);

    s32 position[3];
    u32 random_value;

    if (source->vertical_motion >= -255 && source->vertical_motion <= 255) {
        source->state = 0;
    }
    random_value = Random_Next();
    if (random_value * 100 >> 16 <= 9) {
        StageEffect *effect;
        s32 angle;
        s32 radius;

        position[0] = source->x;
        position[1] = source->y;
        position[2] = source->z;
        angle = Random_Next();
        radius = Random_Next();
        Vector_AddPolarOffset(angle << 4, radius, position);
        {
            s32 x = position[0];
            s32 y = position[1];
            s32 z = position[2];

            effect = Engine_ObjectCreate(285, x, y, z);
        }
        if (effect != 0) {
            effect->state = 0;
            Actor_SetSpriteFlags(effect, 0);
            Object_SetScript(effect, (s32)gColossoRandomEffect);
            Object_SetAnimation(effect, 1);
            Object_SetAnimation(effect, 0);
        }
    }
}

s32 ColossoLogRollingStage_RaiseLinkedSceneEffect(StageEffect_02003d88 *source)
{
    extern void Object_SetPosition(StageEffect *, s32, s32, s32);

    StageEffect_02003d88 *effect = Engine_ActorGet(source->linked_effect_slot);

    Object_SetPosition(effect, source->x, source->y + 0x240000, source->z);
    effect->state = 0;
    Object_SetScript(effect, (s32)gColossoLinkedEffect);
    Audio_PlayCue(83);
    source->linked_effect_slot = 0;
    return 0;
}

#include "FIELDOBJ.H"
#include "SHIAN.H"

void SetEffectRecordMode(struct FieldActor *work, s32 mode)
{
    work->sprite->priority = mode;
}

void *SceneEffect_SpawnPrimary(s32 x, s32 y, s32 z, s32 kind)
{
    struct FieldActor *effect = Engine_ObjectCreate(kind, x, y, z);

    if (effect != NULL) {
        struct FieldSprite *sprite = effect->sprite;

        sprite->priority = 0;
        effect->motion_flags = 0;
        effect->collision_flags = 8;
        Engine_ActorSetSpriteFlags(effect, 0);
        ObjectGroup_SetChildValue(effect, 14);
        Engine_ObjectSetBlendMode(effect, 1);
        return effect;
    }
    return NULL;
}

void *SceneEffect_SpawnSecondary(s32 x, s32 y, s32 z, s32 kind)
{
    struct FieldActor *effect = Engine_ObjectCreate(kind, x, y, z);

    if (effect != NULL) {
        struct FieldSprite *sprite = effect->sprite;

        sprite->priority = 1;
        effect->motion_flags = 0;
        effect->collision_flags = 8;
        Engine_ActorSetSpriteFlags(effect, 0);
        ObjectGroup_SetChildValue(effect, 15);
        effect->priority_flags = (effect->priority_flags & 0xfe) | 2;
        return effect;
    }
    return NULL;
}

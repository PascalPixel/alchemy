#include "types.h"
#include "field_effect.h"
#include "field_event.h"
#include "field_scene.h"

enum {
    PALETTE_TRANSLUCENT_EFFECT = 14,
    PALETTE_EFFECT = 15
};

void Effect_SetPriority(struct FieldEffect *effect, s32 priority)
{
    effect->sprite->priority = priority;
}

struct FieldEffect *Effect_CreateTranslucent(s32 x, s32 y, s32 z, s32 type)
{
    struct FieldEffect *effect;

    effect = (struct FieldEffect *)Engine_ObjectCreate(type, x, y, z);
    if (effect != NULL) {
        effect->sprite->priority = 0;
        effect->motion_flags = 0;
        effect->collision_flags = 8;
        Engine_ActorSetSpriteFlags((struct FieldActor *)effect, 0);
        ObjectGroup_SetChildValue((struct FieldActor *)effect, PALETTE_TRANSLUCENT_EFFECT);
        Engine_ObjectSetBlendMode((struct FieldActor *)effect, OBJECT_BLEND_TRANSLUCENT);
        return effect;
    }
    return NULL;
}

struct FieldEffect *Effect_Create(s32 x, s32 y, s32 z, s32 type)
{
    struct FieldEffect *effect;

    effect = (struct FieldEffect *)Engine_ObjectCreate(type, x, y, z);
    if (effect != NULL) {
        effect->sprite->priority = 1;
        effect->motion_flags = 0;
        effect->collision_flags = 8;
        Engine_ActorSetSpriteFlags((struct FieldActor *)effect, 0);
        ObjectGroup_SetChildValue((struct FieldActor *)effect, PALETTE_EFFECT);
        effect->priority_flags =
            (effect->priority_flags & ~ACTOR_PRIORITY_AUTOMATIC) | ACTOR_PRIORITY_UNDERFOOT;
        return effect;
    }
    return NULL;
}

const s32 gShortEffectScript[] = {
    SCRIPT_WAIT(1),
    SCRIPT_REPEAT(22, 0),
    SCRIPT_WAIT(1),
    SCRIPT_SET_POSITION(0, 0, 0),
    SCRIPT_WAIT(1),
    SCRIPT_DELETE,
};

const s32 gMediumEffectScript[] = {
    SCRIPT_WAIT(1),
    SCRIPT_REPEAT(44, 0),
    SCRIPT_WAIT(1),
    SCRIPT_SET_POSITION(0, 0, 0),
    SCRIPT_WAIT(1),
    SCRIPT_DELETE,
};

const s32 gLongEffectScript[] = {
    SCRIPT_WAIT(1),
    SCRIPT_REPEAT(126, 0),
    SCRIPT_WAIT(1),
    SCRIPT_SET_POSITION(0, 0, 0),
    SCRIPT_WAIT(1),
    SCRIPT_DELETE,
};

const s32 *const gEffectScripts[] = {
    gShortEffectScript,
    gMediumEffectScript,
    gLongEffectScript,
};

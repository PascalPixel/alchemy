#include "TYPES.H"
#include "FIELD_EFFECT.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/*
 * Effect sprites for Lunpa's scenes: creating them and moving them each
 * frame, and the motion scripts a spawned effect runs, such as the dust the
 * fortress guards kick up.
 */

/* The palettes effects draw with. */
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

    effect = (struct FieldEffect *)Object_Create(type, x, y, z);
    if (effect != NULL) {
        effect->sprite->priority = 0;
        effect->motion_flags = 0;
        effect->collision_flags = 8;
        Actor_SetSpriteFlags((struct FieldActor *)effect, 0);
        Object_SetPalette((struct FieldActor *)effect, PALETTE_TRANSLUCENT_EFFECT);
        Object_SetBlendMode((struct FieldActor *)effect, OBJECT_BLEND_TRANSLUCENT);
        return effect;
    }
    return NULL;
}

struct FieldEffect *Effect_Create(s32 x, s32 y, s32 z, s32 type)
{
    struct FieldEffect *effect;

    effect = (struct FieldEffect *)Object_Create(type, x, y, z);
    if (effect != NULL) {
        effect->sprite->priority = 1;
        effect->motion_flags = 0;
        effect->collision_flags = 8;
        Actor_SetSpriteFlags((struct FieldActor *)effect, 0);
        Object_SetPalette((struct FieldActor *)effect, PALETTE_EFFECT);
        effect->priority_flags =
            (effect->priority_flags & ~ACTOR_PRIORITY_AUTOMATIC) | ACTOR_PRIORITY_UNDERFOOT;
        return effect;
    }
    return NULL;
}

/*
 * The motion scripts a spawn chooses from. Each holds the effect for its
 * lifetime, then hides and deletes it.
 */
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

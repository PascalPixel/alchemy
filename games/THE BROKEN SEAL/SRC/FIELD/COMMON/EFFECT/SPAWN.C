#include "TYPES.H"
#include "FIELD_EFFECT.H"
#include "FIELD_SCENE.H"

extern const s32 *const gEffectScripts[];

enum {
    EFFECT_DEFAULT_TYPE = 222,
    EFFECT_SCALE_ONE = 0x10000,
    EFFECT_OPTION_MASK = 0xffff0000,
    EFFECT_SCRIPT_REPEAT_COUNT = 3
};

void Effect_Spawn(s32 x, s32 y, s32 z, s32 velocity_x, s32 velocity_y, s32 velocity_z,
                  u32 flags, const struct EffectOptions *options)
{
    struct FieldActor *party = Actor_Get(ACTOR_PARTY_LEADER);
    struct FieldEffect *effect;
    struct FieldSprite *sprite;
    const s32 *script;
    u32 table_offset;

    if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
        effect = (struct FieldEffect *)Object_Create(options->type, x, y, z);
    else
        effect = (struct FieldEffect *)Object_Create(EFFECT_DEFAULT_TYPE, x, y, z);
    if (effect == 0)
        return;

    sprite = effect->sprite;
    Object_SetAnimation((struct FieldActor *)effect, (flags + 1) & EFFECT_SCRIPT_MASK);
    table_offset = (flags & EFFECT_SCRIPT_MASK) << 2;
    Object_SetScript((struct FieldActor *)effect, gEffectScripts[table_offset >> 2]);
    effect->motion_flags = 0;
    sprite->flags = 0;
    effect->update = Effect_Move;
    effect->velocity_x = velocity_x;
    effect->velocity_y = velocity_y;
    effect->velocity_z = velocity_z;
    sprite->priority = party->sprite->priority;
    effect->scale_rate_x = 0;
    effect->scale_rate_y = 0;
    effect->spin = 0;

    if ((flags & EFFECT_OPTION_MASK) == 0 || options == 0)
        return;
    if ((flags & EFFECT_USE_PALETTE) != 0)
        Object_SetPalette((struct FieldActor *)effect, options->palette);
    if ((flags & EFFECT_USE_PRIORITY) != 0) {
        effect->priority_flags &= ~ACTOR_PRIORITY_AUTOMATIC;
        sprite->priority = options->priority;
    }
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
        effect->scale_x = options->start_scale_x;
        effect->scale_y = options->start_scale_y;
    }
    if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
        script = gEffectScripts[table_offset >> 2];
        if ((flags & EFFECT_USE_START_SCALE) != 0) {
            effect->scale_rate_x = (options->target_scale_x - effect->scale_x) / script[EFFECT_SCRIPT_REPEAT_COUNT];
            effect->scale_rate_y = (options->target_scale_y - effect->scale_y) / script[EFFECT_SCRIPT_REPEAT_COUNT];
        } else {
            effect->scale_rate_x = (options->target_scale_x - EFFECT_SCALE_ONE) / script[EFFECT_SCRIPT_REPEAT_COUNT];
            effect->scale_rate_y = (options->target_scale_y - EFFECT_SCALE_ONE) / script[EFFECT_SCRIPT_REPEAT_COUNT];
        }
    }
    if ((flags & EFFECT_USE_SCRIPT) != 0) {
        Object_SetAnimation((struct FieldActor *)effect, 1);
        Object_SetScript((struct FieldActor *)effect, options->script);
    }
    if ((flags & EFFECT_USE_ROTATION) != 0)
        sprite->rotation = options->rotation;
    if ((flags & EFFECT_USE_SPIN) != 0)
        effect->spin = options->spin;
    if ((flags & EFFECT_USE_UPDATE) != 0)
        effect->update = options->update;
}

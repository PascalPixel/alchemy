#include "types.h"
#include "party_state.h"
#include "field_effect.h"

extern const s32 *const Data_0200270c[];

struct FieldActor *Object_GetById(s32 id);
struct FieldActor *Func_02001dc0(s32 type, s32 x, s32 y, s32 z);
void Func_02001da8(struct FieldActor *object, s32 mode);
void Func_02001db8(struct FieldActor *object, const s32 *script);
void ObjectDispatch_SetSingleChildFlags(struct FieldActor *object, s32 flags);
void Object_SetSpritePriority(struct FieldActor *object, s32 priority);
void Object_SetPartAttribute(struct FieldActor *object, s32 value);

static inline void Effect_SetScript(struct FieldActor *object, const s32 *script)
{
    /* FAKEMATCH: as in ☀️'s spawner, the inline script boundary keeps the table offset live until scale setup. */
    Func_02001db8(object, script);
}

void Func_02001258(s32 x, s32 y, s32 z, s32 velocity_x, s32 velocity_y, s32 velocity_z,
                   u32 flags, const struct EffectOptions *options)
{
    struct FieldActor *party = Object_GetById(gPartyState.current_owner);
    struct FieldEffect *effect;
    struct FieldSprite *sprite;
    const s32 *script;
    u32 table_offset;

    if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
        effect = (struct FieldEffect *)Func_02001dc0(options->type, x, y, z);
    else
        effect = (struct FieldEffect *)Func_02001dc0(285, x, y, z);
    if (effect == 0)
        return;

    sprite = effect->sprite;
    Func_02001da8((struct FieldActor *)effect, (flags + 1) & EFFECT_SCRIPT_MASK);
    table_offset = (flags & EFFECT_SCRIPT_MASK) << 2;
    Effect_SetScript((struct FieldActor *)effect, Data_0200270c[table_offset >> 2]);
    effect->motion_flags = 0;
    ObjectDispatch_SetSingleChildFlags((struct FieldActor *)effect, 0);
    effect->update = BattleFx_UpdateObjectMotionScaleAndLinkedAngle;
    effect->velocity_x = velocity_x;
    effect->velocity_y = velocity_y;
    effect->velocity_z = velocity_z;
    Object_SetSpritePriority((struct FieldActor *)effect, party->sprite->priority);
    effect->scale_rate_x = 0;
    effect->scale_rate_y = 0;
    effect->spin = 0;

    if ((flags & 0xffff0000) == 0 || options == 0)
        return;
    if ((flags & EFFECT_USE_PALETTE) != 0)
        Object_SetPartAttribute((struct FieldActor *)effect, options->palette);
    if ((flags & EFFECT_USE_PRIORITY) != 0) {
        effect->priority_flags &= ~1;
        Object_SetSpritePriority((struct FieldActor *)effect, options->priority);
    }
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
        effect->scale_x = options->start_scale_x;
        effect->scale_y = options->start_scale_y;
    }
    if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
        s32 target;
        s32 steps;

        script = Data_0200270c[table_offset >> 2];
        target = options->target_scale_x;
        steps = script[3];
        if ((flags & EFFECT_USE_START_SCALE) != 0) {
            effect->scale_rate_x = (target - effect->scale_x) / steps;
            effect->scale_rate_y = (options->target_scale_y - effect->scale_y) / script[3];
        } else {
            effect->scale_rate_x = (target - 0x10000) / steps;
            effect->scale_rate_y = (options->target_scale_y - 0x10000) / script[3];
        }
    }
    if ((flags & EFFECT_USE_SCRIPT) != 0) {
        Func_02001da8((struct FieldActor *)effect, 1);
        Func_02001db8((struct FieldActor *)effect, options->script);
    }
    if ((flags & EFFECT_USE_ROTATION) != 0)
        sprite->rotation = options->rotation;
    if ((flags & EFFECT_USE_SPIN) != 0)
        effect->spin = options->spin;
    if ((flags & EFFECT_USE_UPDATE) != 0)
        effect->update = options->update;
}

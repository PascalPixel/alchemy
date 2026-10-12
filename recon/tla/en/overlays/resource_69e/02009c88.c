#include "types.h"
#include "field_effect.h"
#include "field_sprite.h"
#include "party_state.h"

extern const s32 *Data_020033f4[];

struct FieldActor *Object_GetById(s32 id);
struct FieldEffect *Func_02002828(s32 type, s32 x, s32 y, s32 z);
void Func_02002810(struct FieldEffect *effect, s32 mode);
void Func_02002820(struct FieldEffect *effect, const s32 *script);
void ObjectDispatch_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Object_SetSpritePriority(struct FieldEffect *effect, s32 priority);
void Object_SetPartAttribute(struct FieldEffect *effect, s32 value);

enum {
    EFFECT_DEFAULT_TYPE = 285,
    EFFECT_SCALE_ONE = 0x10000,
    EFFECT_OPTION_MASK = 0xffff0000,
    EFFECT_SCRIPT_REPEAT_COUNT = 3
};

void Func_02001c88(s32 x, s32 y, s32 z, s32 velocity_x, s32 velocity_y, s32 velocity_z,
    u32 flags, const struct EffectOptions *options)
{
    struct FieldActor *party = Object_GetById(gPartyState.current_owner);
    struct FieldEffect *effect;
    struct FieldSprite *sprite;
    const s32 *script;
    s32 target;
    s32 count;

    if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
        effect = Func_02002828(options->type, x, y, z);
    else
        effect = Func_02002828(EFFECT_DEFAULT_TYPE, x, y, z);
    if (effect == 0)
        return;

    sprite = effect->sprite;
    Func_02002810(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
    Func_02002820(effect, Data_020033f4[flags & EFFECT_SCRIPT_MASK]);
    effect->motion_flags = 0;
    ObjectDispatch_SetSingleChildFlags(effect, 0);
    effect->update = BattleFx_UpdateObjectMotionScaleAndLinkedAngle;
    effect->velocity_x = velocity_x;
    effect->velocity_y = velocity_y;
    effect->velocity_z = velocity_z;
    Object_SetSpritePriority(effect, party->sprite->priority);
    effect->scale_rate_x = 0;
    effect->scale_rate_y = 0;
    effect->spin = 0;

    if ((flags & EFFECT_OPTION_MASK) == 0 || options == 0)
        return;
    if ((flags & EFFECT_USE_PALETTE) != 0)
        Object_SetPartAttribute(effect, options->palette);
    if ((flags & EFFECT_USE_PRIORITY) != 0) {
        effect->priority_flags &= ~1;
        Object_SetSpritePriority(effect, options->priority);
    }
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
        effect->scale_x = options->start_scale_x;
        effect->scale_y = options->start_scale_y;
    }
    if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
        script = Data_020033f4[flags & EFFECT_SCRIPT_MASK];
        target = options->target_scale_x;
        count = script[EFFECT_SCRIPT_REPEAT_COUNT];
        if ((flags & EFFECT_USE_START_SCALE) != 0) {
            effect->scale_rate_x = (target - effect->scale_x) / count;
            effect->scale_rate_y = (options->target_scale_y - effect->scale_y) / script[EFFECT_SCRIPT_REPEAT_COUNT];
        } else {
            effect->scale_rate_x = (target - EFFECT_SCALE_ONE) / count;
            effect->scale_rate_y = (options->target_scale_y - EFFECT_SCALE_ONE) / script[EFFECT_SCRIPT_REPEAT_COUNT];
        }
    }
    if ((flags & EFFECT_USE_SCRIPT) != 0) {
        Func_02002810(effect, 1);
        Func_02002820(effect, options->script);
    }
    if ((flags & EFFECT_USE_ROTATION) != 0)
        sprite->rotation = options->rotation;
    if ((flags & EFFECT_USE_SPIN) != 0)
        effect->spin = options->spin;
    if ((flags & EFFECT_USE_UPDATE) != 0)
        effect->update = options->update;
}

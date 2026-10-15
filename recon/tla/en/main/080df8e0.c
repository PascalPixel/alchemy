/* Near miss, differ 8 of 218 (2026-10-15): ☀️'s EFFECT_SPAWN (Effect_SpawnResident)
 * with ⚓️'s calls (Object_GetById(0), Func_080200c0, default type 285, the
 * Data_080f3978 scripts, Object_SetPartAttribute). Left: the listing stores
 * scale_rate_x and _y (0x30, 0x34) after the priority byte copy; here the scheduler
 * lifts both stores above the party sprite's byte load, and a barrier after the copy
 * swaps r9 and r11. One reordered pool load before it (Data_080f3978). */
#include "types.h"
#include "field_effect.h"
#include "objdisp.h"

extern const s32 *Data_080f3978[];

struct FieldActor *Object_GetById(s32 id);
struct FieldEffect *Func_080200c0(s32 type, s32 x, s32 y, s32 z);
void Object_SetPartAttribute(struct FieldActor *object, s32 value);
void BattleFx_UpdateObjectMotionScaleAndLinkedAngle(union FieldObject *object);

void Func_080df8e0(s32 x, s32 y, s32 z, s32 velocity_x, s32 velocity_y, s32 velocity_z, u32 flags,
    const struct EffectOptions *options)
{
    struct FieldActor *party = Object_GetById(0);
    struct FieldEffect *effect;
    struct FieldSprite *sprite;
    const s32 *script;

    if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
        effect = Func_080200c0(options->type, x, y, z);
    else
        effect = Func_080200c0(285, x, y, z);
    if (effect == 0)
        return;

    sprite = effect->sprite;
    Object_SetMode((struct FieldActor *)effect, (flags + 1) & EFFECT_SCRIPT_MASK);
    ObjectDispatch_InitializeFar((void *)effect, Data_080f3978[flags & EFFECT_SCRIPT_MASK]);
    effect->motion_flags = 0;
    sprite->next_frame = 0;
    effect->update = BattleFx_UpdateObjectMotionScaleAndLinkedAngle;
    effect->velocity_x = velocity_x;
    effect->velocity_y = velocity_y;
    effect->velocity_z = velocity_z;
    sprite->priority = party->sprite->priority;
    effect->scale_rate_x = 0;
    effect->scale_rate_y = 0;
    effect->spin = 0;

    if ((flags & 0xffff0000) == 0 || options == 0)
        return;
    if ((flags & EFFECT_USE_PALETTE) != 0)
        Object_SetPartAttribute((struct FieldActor *)effect, options->palette);
    if ((flags & EFFECT_USE_PRIORITY) != 0) {
        effect->priority_flags &= ~1;
        sprite->priority = options->priority;
    }
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
        effect->scale_x = options->start_scale_x;
        effect->scale_y = options->start_scale_y;
    }
    if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
        script = Data_080f3978[flags & EFFECT_SCRIPT_MASK];
        if ((flags & EFFECT_USE_START_SCALE) != 0) {
            effect->scale_rate_x = (options->target_scale_x - effect->scale_x) / script[3];
            effect->scale_rate_y = (options->target_scale_y - effect->scale_y) / script[3];
        } else {
            effect->scale_rate_x = (options->target_scale_x - 0x10000) / script[3];
            effect->scale_rate_y = (options->target_scale_y - 0x10000) / script[3];
        }
    }
    if ((flags & EFFECT_USE_SCRIPT) != 0) {
        Object_SetMode((struct FieldActor *)effect, 1);
        ObjectDispatch_InitializeFar((void *)effect, options->script);
    }
    if ((flags & EFFECT_USE_ROTATION) != 0)
        sprite->rotation = options->rotation;
    if ((flags & EFFECT_USE_SPIN) != 0)
        effect->spin = options->spin;
    if ((flags & EFFECT_USE_UPDATE) != 0)
        effect->update = options->update;
}

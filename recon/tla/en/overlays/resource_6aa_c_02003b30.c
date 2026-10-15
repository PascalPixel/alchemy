/* Draft, differ 12 of 207: overlay 6aa's effect spawner, ☀️'s EFFECT_SPAWN (scene/common/effect_spawn.h) with the leader from gPartyState.current_owner, ObjectDispatch_SetSingleChildFlags for the sprite flags, the leader's sprite priority through Object_SetSpritePriorityB, and the scale-to-target target and frame count read before the start-scale test (the ☀️ form: 40). Left: three pairs where the native schedules a load before a register copy and this form the copy first: the gPartyState load before z's copy to sl in the prologue, the sprite load before flags' copy (sched2 priority 60 against 61), and the script table address before flags' copy. Tried: an asm operand on the sprite (32); alchemy permute 6 minutes (no gain). */
#include "types.h"
#include "field_effect.h"
#include "party_state.h"

extern const s32 *Data_0200686c[];

struct FieldActor *Object_GetById(s32 id);
struct FieldEffect *Func_02004bd8(s32 type, s32 x, s32 y, s32 z);
void Func_02004bc0(struct FieldEffect *effect, s32 mode);
void Func_02004bd0(struct FieldEffect *effect, const s32 *script);
void ObjectDispatch_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Object_SetSpritePriorityB(struct FieldEffect *effect, s32 priority);
void Object_SetPartAttribute(struct FieldEffect *effect, s32 value);

void Func_02003b30(s32 x, s32 y, s32 z, s32 velocity_x, s32 velocity_y, s32 velocity_z, u32 flags,
    const struct EffectOptions *options)
{
    struct FieldActor *party = Object_GetById(gPartyState.current_owner);
    struct FieldEffect *effect;
    struct FieldSprite *sprite;
    const s32 *script;

    if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
        effect = Func_02004bd8(options->type, x, y, z);
    else
        effect = Func_02004bd8(285, x, y, z);
    if (effect == 0)
        return;

    sprite = effect->sprite;
    Func_02004bc0(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
    Func_02004bd0(effect, Data_0200686c[flags & EFFECT_SCRIPT_MASK]);
    effect->motion_flags = 0;
    ObjectDispatch_SetSingleChildFlags(effect, 0);
    effect->update = BattleFx_UpdateObjectMotionScaleAndLinkedAngle;
    effect->velocity_x = velocity_x;
    effect->velocity_y = velocity_y;
    effect->velocity_z = velocity_z;
    Object_SetSpritePriorityB(effect, party->sprite->priority);
    effect->scale_rate_x = 0;
    effect->scale_rate_y = 0;
    effect->spin = 0;

    if ((flags & 0xffff0000) == 0 || options == 0)
        return;
    if ((flags & EFFECT_USE_PALETTE) != 0)
        Object_SetPartAttribute(effect, options->palette);
    if ((flags & EFFECT_USE_PRIORITY) != 0) {
        effect->priority_flags &= ~1;
        Object_SetSpritePriorityB(effect, options->priority);
    }
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
        effect->scale_x = options->start_scale_x;
        effect->scale_y = options->start_scale_y;
    }
    if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
        s32 target;
        s32 frames;

        script = Data_0200686c[flags & EFFECT_SCRIPT_MASK];
        target = options->target_scale_x;
        frames = script[3];
        if ((flags & EFFECT_USE_START_SCALE) != 0) {
            effect->scale_rate_x = (target - effect->scale_x) / frames;
            effect->scale_rate_y = (options->target_scale_y - effect->scale_y) / script[3];
        } else {
            effect->scale_rate_x = (target - 0x10000) / frames;
            effect->scale_rate_y = (options->target_scale_y - 0x10000) / script[3];
        }
    }
    if ((flags & EFFECT_USE_SCRIPT) != 0) {
        Func_02004bc0(effect, 1);
        Func_02004bd0(effect, options->script);
    }
    if ((flags & EFFECT_USE_ROTATION) != 0)
        sprite->rotation = options->rotation;
    if ((flags & EFFECT_USE_SPIN) != 0)
        effect->spin = options->spin;
    if ((flags & EFFECT_USE_UPDATE) != 0)
        effect->update = options->update;
}

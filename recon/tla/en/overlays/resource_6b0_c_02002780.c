/* Draft, differ 12 of 207: the second Effect_Spawn in overlay 6b0, linked after its own move.c: the
 * leader's priority, Object_SetSpritePriorityB and Effect_MoveLocal, otherwise overlay 653's
 * Func_020000b8 (its draft is the base). Left: instruction order only, as in that draft and in
 * Func_02000480's: the native loads gPartyState's address before it moves z into sl, loads the
 * sprite before it copies the flags for the mode, and loads the script table before it copies the
 * flags for the index. */
#include "types.h"
#include "party_state.h"
#include "field_sprite.h"
#include "field_effect.h"

extern struct FieldEffect *Object_GetById(s32 id);
extern void Func_020032e0(struct FieldEffect *effect, s32 mode);
extern void Func_020032e8(struct FieldEffect *effect, const s32 *script);
extern const s32 *Data_02004428[];
extern void ObjectDispatch_SetSingleChildFlags(struct FieldEffect *effect, u32 flags);
extern void Object_SetSpritePriorityB(struct FieldEffect *effect, s32 priority);
extern void Object_SetPartAttribute(struct FieldEffect *effect, s32 attribute);
extern void Effect_MoveLocal(union FieldObject *object);
void Func_02002780(s32 x, s32 y, s32 z, s32 velocity_x, s32 velocity_y, s32 velocity_z, u32 flags,
    const struct EffectOptions *options)
{
    struct FieldEffect *party = Object_GetById(gPartyState.current_owner);
    struct FieldEffect *effect;
    struct FieldSprite *sprite;
    const s32 *script;
    u32 start;

    if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
        effect = (struct FieldEffect *)Engine_ObjectCreate(options->type, x, y, z);
    else
        effect = (struct FieldEffect *)Engine_ObjectCreate(285, x, y, z);
    if (effect == 0)
        return;

    sprite = effect->sprite;
    Func_020032e0(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
    Func_020032e8(effect, Data_02004428[flags & EFFECT_SCRIPT_MASK]);
    effect->motion_flags = 0;
    ObjectDispatch_SetSingleChildFlags(effect, 0);
    effect->update = Effect_MoveLocal;
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
    if ((flags & EFFECT_USE_PRIORITY) != 0)
    {
        effect->priority_flags &= ~1;
        Object_SetSpritePriorityB(effect, options->priority);
    }
    start = flags & EFFECT_USE_START_SCALE;
    if (start != 0)
    {
        effect->scale_x = options->start_scale_x;
        effect->scale_y = options->start_scale_y;
    }
    if ((flags & EFFECT_SCALE_TO_TARGET) != 0)
    {
        s32 target_x;
        s32 count;

        script = Data_02004428[flags & EFFECT_SCRIPT_MASK];
        target_x = options->target_scale_x;
        count = script[3];
        if (start != 0)
        {
            effect->scale_rate_x = (target_x - effect->scale_x) / count;
            effect->scale_rate_y = (options->target_scale_y - effect->scale_y) / script[3];
        }
        else
        {
            effect->scale_rate_x = (target_x - 0x10000) / count;
            effect->scale_rate_y = (options->target_scale_y - 0x10000) / script[3];
        }
    }
    if ((flags & EFFECT_USE_SCRIPT) != 0)
    {
        Func_020032e0(effect, 1);
        Func_020032e8(effect, options->script);
    }
    if ((flags & EFFECT_USE_ROTATION) != 0)
        sprite->rotation = options->rotation;
    if ((flags & EFFECT_USE_SPIN) != 0)
        effect->spin = options->spin;
    if ((flags & EFFECT_USE_UPDATE) != 0)
        effect->update = options->update;
}

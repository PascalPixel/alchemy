/* Spawn a scripted effect with optional palette, priority and scale rates.
 * Complete 352-byte owner, including its three-word pool, matches exactly.
 * FAKEMATCH: retain the local script-table copy and branch-local divide
 * tails so the compiler reloads the script and prepares both call arguments
 * in the observed lifetime. Shared FIELD_EFFECT types recover the remaining
 * object, sprite and options layout without private byte-offset casts. */
#include "FIELD_EFFECT.H"

void OverlayObject_AdvancePositionByDelta();

struct ScriptTable {
    const s32 *script[3];
};

void Effect_Spawn(s32 x, s32 y, s32 z, s32 velocity_x, s32 velocity_y, s32 velocity_z, u32 flags,
                  const struct EffectOptions *extra)
{
    struct ScriptTable table;
    struct FieldEffect *obj;
    struct FieldSprite *spr;
    const s32 *script;

    table = *(struct ScriptTable *)0x0200a418;
    obj = (struct FieldEffect *)Engine_ObjectCreate(222, x, y, z);
    if (obj == 0)
        return;
    spr = obj->sprite;
    Engine_ObjectSetAnimation((struct FieldActor *)obj, (flags + 1) & EFFECT_SCRIPT_MASK);
    Engine_ObjectSetScript((struct FieldActor *)obj, table.script[flags & EFFECT_SCRIPT_MASK]);
    obj->motion_flags = 0;
    spr->flags = 0;
    obj->update = OverlayObject_AdvancePositionByDelta;
    obj->velocity_x = velocity_x;
    obj->velocity_y = velocity_y;
    obj->velocity_z = velocity_z;
    obj->scale_rate_x = 0;
    obj->scale_rate_y = 0;
    spr->priority = 1;
    if ((flags & 0xffff0000) == 0 || extra == 0)
        return;
    if (flags & EFFECT_USE_PALETTE)
        Engine_ObjectSetPalette((struct FieldActor *)obj, extra->palette);
    if (flags & EFFECT_USE_PRIORITY) {
        obj->priority_flags &= ~ACTOR_PRIORITY_AUTOMATIC;
        spr->priority = extra->priority;
    }
    if (flags & EFFECT_USE_START_SCALE) {
        obj->scale_x = extra->start_scale_x;
        obj->scale_y = extra->start_scale_y;
    }
    if (flags & EFFECT_SCALE_TO_TARGET) {
        script = table.script[flags & EFFECT_SCRIPT_MASK];
        if (flags & EFFECT_USE_START_SCALE) {
            obj->scale_rate_x = Engine_MathDivide(extra->target_scale_x - obj->scale_x, script[3]);
            obj->scale_rate_y = Engine_MathDivide(extra->target_scale_y - obj->scale_y, script[3]);
        } else {
            obj->scale_rate_x = Engine_MathDivide(extra->target_scale_x - 0x10000, script[3]);
            obj->scale_rate_y = Engine_MathDivide(extra->target_scale_y - 0x10000, script[3]);
        }
    }
}

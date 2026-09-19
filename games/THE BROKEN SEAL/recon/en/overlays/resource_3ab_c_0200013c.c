#include "TYPES.H"
#include "FIELD_EFFECT.H"

struct EffectDescriptor {
    s32 unknown_00[3];
    s32 duration;
};

extern struct EffectDescriptor *Data_02009be0[];
extern struct FieldEffect *Func_02001b5e(s32);
extern struct FieldEffect *Func_02001afc(s32, s32, s32, s32);
extern void Func_02001b06(struct FieldEffect *, s32);
extern void Func_02001b20(struct FieldEffect *, struct EffectDescriptor *);
extern void Func_02001c82(struct FieldEffect *, s32);
extern s32 Func_02001b94();
extern s32 Func_02001bac();
extern s32 Func_02001bba();
extern void Func_02001c20(struct FieldEffect *, s32);
extern void Func_02001c30(struct FieldEffect *, const s32 *);

void Effect_Spawn(s32 x, s32 y, s32 z, s32 velocity_x, s32 velocity_y, s32 velocity_z,
                  u32 flags, const struct EffectOptions *options)
{
    struct FieldEffect *party = Func_02001b5e(0);
    struct FieldEffect *effect;
    struct FieldSprite *sprite;
    struct EffectDescriptor *descriptor;
    u32 table_offset;
    s32 delta;
    s32 duration;
    s32 first_delta;
    s32 accumulated;

    if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
        effect = Func_02001afc(options->type, x, y, z);
    else
        effect = Func_02001afc(222, x, y, z);
    if (effect == 0)
        return;

    sprite = effect->sprite;
    Func_02001b06(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
    table_offset = (flags & EFFECT_SCRIPT_MASK) << 2;
    Func_02001b20(effect, Data_02009be0[table_offset >> 2]);
    effect->motion_flags = 0;
    sprite->flags = 0;
    effect->update = (void (*)(union FieldObject *))0x02008105;
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
        Func_02001c82(effect, options->palette);
    if ((flags & EFFECT_USE_PRIORITY) != 0) {
        effect->priority_flags &= ~ACTOR_PRIORITY_AUTOMATIC;
        sprite->priority = options->priority;
    }
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
        effect->scale_x = options->start_scale_x;
        effect->scale_y = options->start_scale_y;
    }
    if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
        descriptor = Data_02009be0[table_offset >> 2];
        if ((flags & EFFECT_USE_START_SCALE) != 0) {
            first_delta = options->target_scale_x;
            accumulated = effect->scale_x;
            first_delta -= accumulated;
            effect->scale_rate_x = Func_02001b94(first_delta, descriptor->duration);
            delta = options->target_scale_y;
            duration = descriptor->duration;
            delta -= effect->scale_y;
        } else {
            first_delta = options->target_scale_x;
            first_delta += (s32)0xffff0000;
            effect->scale_rate_x = Func_02001bac(first_delta, descriptor->duration);
            delta = options->target_scale_y;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }
        effect->scale_rate_y = Func_02001bba(delta, duration);
    }
    if ((flags & EFFECT_USE_SCRIPT) != 0) {
        Func_02001c20(effect, 1);
        Func_02001c30(effect, options->script);
    }
    if ((flags & EFFECT_USE_ROTATION) != 0)
        sprite->rotation = options->rotation;
    if ((flags & EFFECT_USE_SPIN) != 0)
        effect->spin = options->spin;
    if ((flags & EFFECT_USE_UPDATE) != 0)
        effect->update = options->update;
}

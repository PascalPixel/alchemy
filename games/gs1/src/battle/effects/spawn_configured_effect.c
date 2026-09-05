#include "types.h"

struct BattleEffectSprite {
    u8 reserved00[9];
    u8 flags9;
    u8 reserved0a[20];
    u16 angle;
    u8 reserved20[6];
    u8 state26;
};

struct BattleEffect {
    u8 reserved00[24];
    s32 position_x;
    s32 position_y;
    u8 reserved20[3];
    u8 effect_flags;
    u8 reserved24[12];
    s32 velocity_rate_x;
    s32 velocity_rate_y;
    u8 reserved38[12];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    struct BattleEffectSprite *sprite;
    u8 reserved54;
    u8 motion_mode;
    u8 reserved56[14];
    u16 frame_step;
    u8 reserved66[6];
    u32 callback;
};

struct EffectSpawnOptions {
    u8 mode_bits;
    u8 reserved01[3];
    s32 battle_mode;
    s32 position_x;
    s32 position_y;
    s32 target30;
    s32 target34;
    s16 kind;
    u16 reserved1a;
    s32 callback_arg;
    u16 angle;
    u16 step;
    u32 callback;
};

struct EffectDescriptor {
    s32 reserved00[3];
    s32 duration;
};

extern struct EffectDescriptor *Data_0200de64[];

struct BattleEffect *Func_02005c32();
struct BattleEffect *Func_02005b58();
void Func_02005b62();
void Func_02005b7c();
void Func_02005d8e();
s32 Func_02005be8();
s32 Func_02005c00();
s32 Func_02005c0e();
void Func_02005c7c();
void Func_02005c8c();

void Func_0200013c(s32 x, s32 y,
                   s32 z, s32 vx, s32 vy, s32 vz, u32 flags,
                   const struct EffectSpawnOptions *options)
{
    u32 table_offset;
    struct BattleEffect *reference_effect;
    u32 copied_bits;
    s32 flag_mask;
    u32 block_bits;
    struct BattleEffect *effect;
    struct BattleEffectSprite *effect_sprite;
    struct BattleEffectSprite *effect_sprite_before_options;
    u32 option_bits;
    u16 *tag;
    s32 duration;
    s32 first_delta;
    s32 accumulated;
    reference_effect = Func_02005c32(0);

    if ((flags & 0x100000) != 0 && options != 0) {
        effect = Func_02005b58(options->kind, x, y, z);
    } else {
        effect = Func_02005b58(222, x, y, z);
    }
    if (effect == 0) return;

    effect_sprite = effect->sprite;
    effect_sprite_before_options = effect_sprite;

    Func_02005b62(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    Func_02005b7c(effect, Data_0200de64[table_offset >> 2]);

    effect->motion_mode = 0;
    effect_sprite->state26 = 0;
    effect->callback = 0x02008105;

    effect->velocity_x = vx;
    x = 3;
    effect->velocity_y = vy;
    effect->velocity_z = vz;

    copied_bits = reference_effect->sprite->flags9 & 12;
    block_bits = *(volatile u8 *)&effect_sprite->flags9;
    flag_mask = ~12;
    effect_sprite->flags9 = (u8)((block_bits & flag_mask) | copied_bits);

    effect->velocity_rate_x = 0;
    effect->velocity_rate_y = 0;
    effect->frame_step = 0;
    tag = &effect->frame_step;

    if ((flags & 0xffff0000) == 0 || options == 0) return;

    if ((flags & 0x10000) != 0) {
        Func_02005d8e(effect, options->battle_mode);
    }

    if ((flags & 0x20000) != 0) {
        effect->effect_flags &= 0xfe;
        option_bits = *(const u8 *)options & x;
        effect_sprite->flags9 = (u8)((*((const u8 *)effect_sprite_before_options + 9) & flag_mask)
                             | (option_bits << 2));
    }

    if ((flags & 0x80000) != 0) {
        effect->position_x = options->position_x;
        effect->position_y = options->position_y;
    }

    if ((flags & 0x40000) != 0) {
        const struct EffectDescriptor *descriptor =
            Data_0200de64[table_offset >> 2];
        s32 delta;

        if ((flags & 0x80000) != 0) {
            first_delta = *(volatile const s32 *)&options->target30;
            accumulated = *(volatile const s32 *)&effect->position_x;
            first_delta -= accumulated;
            effect->velocity_rate_x = Func_02005be8(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->position_y;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->velocity_rate_x = Func_02005c00(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }

        effect->velocity_rate_y = Func_02005c0e(delta, duration);
    }

    if ((flags & 0x200000) != 0) {
        Func_02005c7c(effect, 1);
        Func_02005c8c(effect, options->callback_arg);
    }

    if ((flags & 0x400000) != 0) {
        effect_sprite->angle = options->angle;
    }

    if ((flags & 0x800000) != 0) {
        *tag = options->step;
    }

    if ((flags & 0x1000000) != 0) {
        effect->callback = options->callback;
    }
}

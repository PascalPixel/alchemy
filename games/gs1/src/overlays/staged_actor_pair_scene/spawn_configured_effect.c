#include "staged_actor_pair_scene.h"

struct EffectSprite {
    u8 pad00[9];
    u8 flags;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state;
};

struct SceneEffect {
    u8 pad00[24];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flags;
    u8 pad24[12];
    s32 rate_x;
    s32 rate_y;
    u8 pad38[12];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    struct EffectSprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct EffectOptions {
    u8 mode_bits;
    u8 pad01[3];
    s32 mode;
    s32 accum18;
    s32 accum1c;
    s32 target30;
    s32 target34;
    s16 kind;
    u16 pad1a;
    s32 callback_arg;
    u16 angle;
    u16 step;
    u32 callback;
};

struct EffectDescriptor {
    s32 pad00[3];
    s32 duration;
};

extern struct EffectDescriptor *Data_02009778[];

struct SceneEffect *Func_02001692();
struct SceneEffect *Func_02001638();
void Func_02001642();
void Func_0200165c();
void Func_0200179e();
s32 Func_02001700();
s32 Func_02001718();
s32 Func_02001726();
void Func_0200175c();
void Func_0200176c();

void StagedActorPairScene_SpawnConfiguredEffect(
    s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz, u32 flags,
    const struct EffectOptions *options)
{
    u32 table_offset;
    struct SceneEffect *source_effect;
    u32 copied_bits;
    s32 flag_mask;
    u32 block_bits;
    struct SceneEffect *effect;
    struct EffectSprite *sprite;
    struct EffectSprite *mode_sprite;
    u32 option_bits;
    u16 *step_ptr;
    s32 duration;
    s32 first_delta;
    s32 accumulated;
    source_effect = Func_02001692(0);

    if ((flags & 0x100000) != 0 && options != 0) {
        effect = Func_02001638(options->kind, x, y, z);
    } else {
        effect = Func_02001638(222, x, y, z);
    }
    if (effect == 0) return;

    sprite = effect->sprite;
    mode_sprite = sprite;

    Func_02001642(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    Func_0200165c(effect, Data_02009778[table_offset >> 2]);

    effect->mode55 = 0;
    sprite->state = 0;
    effect->callback = 0x02008105;

    effect->velocity_x = vx;
    x = 3;
    effect->velocity_y = vy;
    effect->velocity_z = vz;

    copied_bits = source_effect->sprite->flags & 12;
    block_bits = *(volatile u8 *)&sprite->flags;
    flag_mask = ~12;
    sprite->flags = (u8)((block_bits & flag_mask) | copied_bits);

    effect->rate_x = 0;
    effect->rate_y = 0;
    effect->step64 = 0;
    step_ptr = &effect->step64;

    if ((flags & 0xffff0000) == 0 || options == 0) return;

    if ((flags & 0x10000) != 0) {
        Func_0200179e(effect, options->mode);
    }

    if ((flags & 0x20000) != 0) {
        effect->flags &= 0xfe;
        option_bits = *(const u8 *)options & x;
        sprite->flags = (u8)((*((const u8 *)mode_sprite + 9) & flag_mask)
                              | (option_bits << 2));
    }

    if ((flags & 0x80000) != 0) {
        effect->accum18 = options->accum18;
        effect->accum1c = options->accum1c;
    }

    if ((flags & 0x40000) != 0) {
        const struct EffectDescriptor *descriptor =
            Data_02009778[table_offset >> 2];
        s32 delta;

        if ((flags & 0x80000) != 0) {
            first_delta = *(volatile const s32 *)&options->target30;
            accumulated = *(volatile const s32 *)&effect->accum18;
            first_delta -= accumulated;
            effect->rate_x = Func_02001700(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->accum1c;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->rate_x = Func_02001718(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }

        effect->rate_y = Func_02001726(delta, duration);
    }

    if ((flags & 0x200000) != 0) {
        Func_0200175c(effect, 1);
        Func_0200176c(effect, options->callback_arg);
    }

    if ((flags & 0x400000) != 0) {
        sprite->angle = options->angle;
    }

    if ((flags & 0x800000) != 0) {
        *step_ptr = options->step;
    }

    if ((flags & 0x1000000) != 0) {
        effect->callback = options->callback;
    }
}

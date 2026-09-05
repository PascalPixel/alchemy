#include "types.h"

#define ConfigurableEffectDescriptors Data_0200cbc4

#define Overlay380_SpawnConfiguredEffect Func_0200013c
#define GetPrimaryEffectRecord Func_02004b1a
#define SpawnConfiguredEffectObject Func_02004a48
#define SetConfiguredEffectVariant Func_02004a52
#define ApplyConfiguredEffectDescriptor Func_02004a6c
#define SetConfiguredEffectLink Func_02004c66
#define ComputeConfiguredEffectRateFromDelta Func_02004ad0
#define ComputeConfiguredEffectRateFromOffset Func_02004ae8
#define ComputeConfiguredEffectSecondRate Func_02004af6
#define SetConfiguredEffectCallbackMode Func_02004b6c
#define ApplyConfiguredEffectCallbackArgument Func_02004b7c
/*
 * Configurable spawn/copy owner at 0x0200013c.  Its code runs through the
 * unwind at 0x02000306; the following three pool words belong to this owner:
 * 0x0200cbc4, 0x02008105, and 0xffff0000.  The next owner begins at 0x02000314.
 */

struct Sprite {
    u8 pad00[9];
    u8 flags9;
    u8 pad0a[20];
    u16 angle;
    u8 pad20[6];
    u8 state26;
};

struct Effect {
    u8 pad00[24];
    s32 accum18;
    s32 accum1c;
    u8 pad20[3];
    u8 flags23;
    u8 pad24[12];
    s32 rate30;
    s32 rate34;
    u8 pad38[12];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    struct Sprite *sprite;
    u8 pad54;
    u8 mode55;
    u8 pad56[14];
    u16 step64;
    u8 pad66[6];
    u32 callback;
};

struct Options {
    u8 mode_bits;
    u8 pad01[3];
    s32 link;
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

struct Descriptor {
    s32 pad00[3];
    s32 duration;
};

extern struct Descriptor *ConfigurableEffectDescriptors[];

/* Overlay-local import veneers, retained per call site. */
extern struct Effect *GetPrimaryEffectRecord(s32 index);
extern struct Effect *SpawnConfiguredEffectObject(s32 kind, s32 x, s32 y, s32 z);
extern void SetConfiguredEffectVariant(struct Effect *effect, s32 mode);
extern void ApplyConfiguredEffectDescriptor(struct Effect *effect, struct Descriptor *descriptor);
extern void SetConfiguredEffectLink(struct Effect *effect, s32 link);
extern s32 ComputeConfiguredEffectRateFromDelta(s32 delta, s32 duration);
extern s32 ComputeConfiguredEffectRateFromOffset(s32 delta, s32 duration);
extern s32 ComputeConfiguredEffectSecondRate(s32 delta, s32 duration);
extern void SetConfiguredEffectCallbackMode(struct Effect *effect, s32 mode);
extern void ApplyConfiguredEffectCallbackArgument(struct Effect *effect, struct Descriptor *descriptor);

void Overlay380_SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                   u32 flags, const struct Options *options)
{
    u32 table_offset;
    struct Effect *party;
    u32 copied_bits;
    s32 flag_mask;
    u32 block_bits;
    struct Effect *effect;
    struct Sprite *block;
    struct Sprite *mode_block;
    u32 option_bits;
    u16 *tag;
    s32 duration;
    s32 first_delta;
    s32 accumulated;

    party = GetPrimaryEffectRecord(0);
    if ((flags & 0x100000) != 0 && options != 0) {
        effect = SpawnConfiguredEffectObject(options->kind, x, y, z);
    } else {
        effect = SpawnConfiguredEffectObject(222, x, y, z);
    }
    if (effect == 0)
        return;

    block = effect->sprite;
    mode_block = block;

    SetConfiguredEffectVariant(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    ApplyConfiguredEffectDescriptor(effect, ConfigurableEffectDescriptors[table_offset >> 2]);

    effect->mode55 = 0;
    block->state26 = 0;
    effect->callback = 0x02008105;

    effect->velocity_x = vx;
    x = 3;
    effect->velocity_y = vy;
    effect->velocity_z = vz;

    copied_bits = party->sprite->flags9 & 12;
    block_bits = *(volatile u8 *)&block->flags9;
    flag_mask = ~12;
    block->flags9 = (u8)((block_bits & flag_mask) | copied_bits);

    effect->rate30 = 0;
    effect->rate34 = 0;
    effect->step64 = 0;
    tag = &effect->step64;

    if ((flags & 0xffff0000) == 0 || options == 0)
        return;

    if ((flags & 0x10000) != 0)
        SetConfiguredEffectLink(effect, options->link);

    if ((flags & 0x20000) != 0) {
        effect->flags23 &= 0xfe;
        option_bits = *(const u8 *)options & x;
        block->flags9 = (u8)((*((const u8 *)mode_block + 9) & flag_mask)
                             | (option_bits << 2));
    }

    if ((flags & 0x80000) != 0) {
        effect->accum18 = options->accum18;
        effect->accum1c = options->accum1c;
    }

    if ((flags & 0x40000) != 0) {
        const struct Descriptor *descriptor =
            ConfigurableEffectDescriptors[table_offset >> 2];
        s32 delta;

        if ((flags & 0x80000) != 0) {
            first_delta = *(volatile const s32 *)&options->target30;
            accumulated = *(volatile const s32 *)&effect->accum18;
            first_delta -= accumulated;
            effect->rate30 = ComputeConfiguredEffectRateFromDelta(first_delta, descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->accum1c;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->rate30 = ComputeConfiguredEffectRateFromOffset(first_delta, descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }

        effect->rate34 = ComputeConfiguredEffectSecondRate(delta, duration);
    }

    if ((flags & 0x200000) != 0) {
        SetConfiguredEffectCallbackMode(effect, 1);
        ApplyConfiguredEffectCallbackArgument(effect, (struct Descriptor *)options->callback_arg);
    }

    if ((flags & 0x400000) != 0)
        block->angle = options->angle;
    if ((flags & 0x800000) != 0)
        *tag = options->step;
    if ((flags & 0x1000000) != 0)
        effect->callback = options->callback;
}

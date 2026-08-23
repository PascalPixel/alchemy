#include "types.h"

/*
 * resource_39c owner at 0x0200013c, 472 bytes: the overlay's general
 * "spawn and fully configure" routine -- the same spawn-then-owner-record
 * shape resource_39c_c_02000048.c/02005158.c/02005388.c already document,
 * generalised to eight caller-supplied parameters (three spawn arguments,
 * three plain copy-through fields, a bitmask of optional features, and a
 * pointer to a record of further optional per-feature values) instead of
 * pulling everything from a fixed "current_value slot" lookup.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + the r8/r9/sl/fp spill at
 * 0x0200013c through the matching unwind and `pop {r0} / bx r0` at
 * 0x2000304-0x2000306; the three-word literal pool 0x02000308-0x02000313
 * follows, then the next owner's prologue at 0x02000314. Eight arguments
 * (r0-r3, plus four stack words at, in the caller's address order, SP+0,
 * SP+4, SP+8, SP+12 -- read here in the order SP+8, SP+12, SP+0, SP+4),
 * void.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`'s `+2` rule.
 *
 * `entry = table[idx]` where `table` is the fixed pointer array at
 * 0x0200de64 and `idx = flags & 0xf` -- the low nibble of the `flags`
 * bitmask parameter selects a variant record, while bits 16 and up are
 * independent boolean feature switches, each gating initial_value optional
 * initialisation step below. Bit 20 (tested once, before the spawn, using
 * the caller's `extra` pointer) picks the spawn call's own first argument
 * and is never tested again afterward.
 *
 * `Object_SetCallback(0)`'s battle_result is read once, through its own field 0x50,
 * for two bits (0x0c) of the owner record's field 9 -- the same
 * clear-two-bits-then-copy idiom resource_39c_c_02000048.c's adopted
 * Func_02005a2e pair and resource_39c_c_02005158.c already use on this
 * overlay's owner-record field 9, just sourced from a different place. The
 * bit-17 block below can overwrite those same two bits again afterward
 * with a battle_value taken from `extra`'s own byte field 0 -- both updates are
 * kept exactly as compiled, not merged.
 *
 * Fields 0x30 and 0x34 on the spawned object are zeroed unconditionally
 * first and then, only under the bit-18 gate, recomputed through three
 * physical calls to `Func_03000380`. The first input pair depends on whether
 * bit 19 populated fields 0x18/0x1c; the second call is shared after that
 * branch, matching the converged machine control flow.
 *
 * Uncertainty: the six imported callees, `Object_SetCallback`'s battle_result, `entry`,
 * or `extra`'s field roles are identified beyond this call shape; the
 * default spawn-argument constant 0xde is recorded as read, not decoded
 * further.
 *
 * Correction (caught while adapting this routine's byte-identical
 * resource_3c9 copy): the bit-19 copy and the bit-18 subtraction read
 * object fields 0x18/0x1c ([r6, #24]/[r6, #28]), not 0x24/0x28 as this
 * file's first draft said -- four sites fixed against the disassembly.
 */

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

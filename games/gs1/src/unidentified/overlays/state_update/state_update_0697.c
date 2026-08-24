#include "types.h"

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

struct Descriptor {
    s32 pad00[3];
    s32 duration;
};

extern struct Descriptor *Data_0200a120[];

struct Effect *Func_02001ed2();
struct Effect *Func_02001e80();
void Func_02001e8a();
void Func_02001ea4();
void Func_02001fee();
s32 Func_02001f00();
s32 Func_02001f18();
s32 Func_02001f26();
void Func_02001fa4();
void Func_02001fb4();

void Func_0200013c(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
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

    party = Func_02001ed2(0);
    if ((flags & 0x100000) != 0 && options != 0) {
        effect = Func_02001e80(options->kind, x, y, z);
    } else {
        effect = Func_02001e80(222, x, y, z);
    }
    if (effect == 0) return;

    block = effect->sprite;
    mode_block = block;
    Func_02001e8a(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    Func_02001ea4(effect, Data_0200a120[table_offset >> 2]);

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

    if ((flags & 0xffff0000) == 0 || options == 0) return;

    if ((flags & 0x10000) != 0) {
        Func_02001fee(effect, options->mode);
    }

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
        const struct Descriptor *descriptor = Data_0200a120[table_offset >> 2];
        s32 delta;

        if ((flags & 0x80000) != 0) {
            first_delta = *(volatile const s32 *)&options->target30;
            accumulated = *(volatile const s32 *)&effect->accum18;
            first_delta -= accumulated;
            effect->rate30 = Func_02001f00(first_delta,
                                            descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->accum1c;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->rate30 = Func_02001f18(first_delta,
                                            descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }
        effect->rate34 = Func_02001f26(delta, duration);
    }

    if ((flags & 0x200000) != 0) {
        Func_02001fa4(effect, 1);
        Func_02001fb4(effect, options->callback_arg);
    }
    if ((flags & 0x400000) != 0) {
        block->angle = options->angle;
    }
    if ((flags & 0x800000) != 0) {
        *tag = options->step;
    }
    if ((flags & 0x1000000) != 0) {
        effect->callback = options->callback;
    }
}

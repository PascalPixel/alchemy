#include "types.h"

/*
 * resource_39f owner at 0x02000ae8, 472 bytes: code 0x02000ae8-0x02000cb3 and
 * the three pool words 0x0200b058, 0x02008ab1 and 0xffff0000 at
 * 0x02000cb4-0x02000cbf.
 *
 * The overlay's effect spawner, and the one routine every cutscene beat in this
 * overlay funnels through: 0x02000e18, 0x02001150, 0x02001244, 0x02001328,
 * 0x020015d0, 0x020016f0, 0x02001880, 0x02001d04, 0x02001de0, 0x02001ef0,
 * 0x02002078 and 0x020021b0 all call it with four register arguments and four
 * stack words.
 *
 * It creates an effect record at (x, y, z), gives it the constant velocity
 * (vx, vy, vz) that the per-frame handler at 0x02000ab0 integrates, and then
 * applies whichever of the optional fields `flags` selects from `options`.
 *
 * See resource_39f_c_02000030.c for the link base and the `bl` encoding rule.
 * Two further witnesses for the 0x02008000 base appear here: the installed
 * handler pool word 0x02008ab1 is 0x02000ab0 plus the Thumb bit, and 0x2ab0
 * really is the start of a leaf routine that adds the +68/+72/+76 velocity into
 * the +8/+12/+16 position; and Data_0200b134 (file offset 0x3058) is a table
 * whose first three words, 0x0200afb0, 0x0200afe8 and 0x0200b020, are the
 * in-image descriptors at offsets 0x2fb0, 0x2fe8 and 0x3020.
 *
 * Func_03000380 is an ARM-mode helper relocated into IWRAM, reached through the
 * veneer at 0x02002ce4 - the same family as the 0x030001d8 square root used by
 * 0x02000030.  It is called with a distance and the descriptor's word at +12,
 * and its result becomes a per-frame step, so it is the division helper.
 *
 * The epilogue is `add sp, #8 / pop {r3, r5, r6, r7} / ... / pop {r0} / bx r0`,
 * so the owner is void.
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

extern struct Descriptor *Data_0200b134[];

/* Returns the party record; only its presentation block at +80 is read. */
struct Effect *Func_0200395a();
struct Effect *Func_020038d8();
void Func_020038e2();
void Func_020038fc();
void Func_02003a96();
s32 Func_020039a0();
s32 Func_020039b8();
s32 Func_020039c6();
void Func_020039fc();
void Func_02003a0c();

/* Creates the effect record and returns it, or 0 on failure. */

                     

/* Relocated IWRAM helper: turns a distance and a descriptor duration into a
 * per-frame step. */

void Func_02000ae8(s32 x, s32 y,
                   s32 z, s32 vx, s32 vy, s32 vz, u32 flags,
                   const struct Options *options)
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
    party = Func_0200395a(0);

    /* 128 << 13.  With that bit set and an options block present the effect's
     * kind comes from the options rather than from the default 222. */
    if ((flags & 0x100000) != 0 && options != 0) {
        effect = Func_020038d8(options->kind, x, y, z);
    } else {
        effect = Func_020038d8(222, x, y, z);
    }
    if (effect == 0) return;

    block = effect->sprite;
    mode_block = block;

    Func_020038e2(effect, (flags + 1) & 15);
    table_offset = (flags & 15) << 2;
    Func_020038fc(effect, Data_0200b134[table_offset >> 2]);

    effect->mode55 = 0;
    block->state26 = 0;

    /* 0x02008ab1 is Func_02000ab0 with the Thumb bit: the per-frame
     * integrator. */
    effect->callback = 0x02008ab1;

    effect->velocity_x = vx;
    x = 3;
    effect->velocity_y = vy;
    effect->velocity_z = vz;

    /* Bits 2 and 3 of the effect's mode byte are copied from the party's. */
    copied_bits = party->sprite->flags9 & 12;
    block_bits = *(volatile u8 *)&block->flags9;
    flag_mask = ~12;
    block->flags9 = (u8)((block_bits & flag_mask) | copied_bits);

    effect->rate30 = 0;
    effect->rate34 = 0;
    effect->step64 = 0;
    tag = &effect->step64;

    /* Everything below is optional detail: the whole block is skipped unless
     * some high flag bit is set and an options record was supplied. */
    if ((flags & 0xffff0000) == 0 || options == 0) return;

    if ((flags & 0x10000) != 0) {                   /* 128 << 9 */
        Func_02003a96(effect, options->mode);
    }

    if ((flags & 0x20000) != 0) {                   /* 128 << 10 */
        effect->flags23 &= 0xfe;
        option_bits = *(const u8 *)options & x;
        block->flags9 = (u8)((*((const u8 *)mode_block + 9) & flag_mask)
                             | (option_bits << 2));
    }

    if ((flags & 0x80000) != 0) {                   /* 128 << 12 */
        effect->accum18 = options->accum18;
        effect->accum1c = options->accum1c;
    }

    if ((flags & 0x40000) != 0) {                   /* 128 << 11 */
        const struct Descriptor *descriptor =
            Data_0200b134[table_offset >> 2];
        s32 delta;

        /* The 0x80000 test is the same register the previous block left live:
         * with a destination supplied the step is measured from it, otherwise
         * the target is biased by -1.0 in 16.16. */
        if ((flags & 0x80000) != 0) {
            first_delta = *(volatile const s32 *)&options->target30;
            accumulated = *(volatile const s32 *)&effect->accum18;
            first_delta -= accumulated;
            effect->rate30 = Func_020039a0(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta -= effect->accum1c;
        } else {
            first_delta = options->target30;
            first_delta += (s32)0xffff0000;
            effect->rate30 = Func_020039b8(first_delta,
                                           descriptor->duration);
            delta = options->target34;
            duration = descriptor->duration;
            delta += (s32)0xffff0000;
        }

        /* Only the FIRST call is per-arm.  The `b.n 0x02000c4c` at the end of
         * the first arm joins both arms onto the single second call site, so
         * the second delta is computed in each arm and the call is spelled
         * once. */
        effect->rate34 = Func_020039c6(delta, duration);
    }

    if ((flags & 0x200000) != 0) {                  /* 128 << 14 */
        Func_020039fc(effect, 1);
        Func_02003a0c(effect, options->callback_arg);
    }

    if ((flags & 0x400000) != 0) {                  /* 128 << 15 */
        block->angle = options->angle;
    }

    if ((flags & 0x800000) != 0) {                  /* 128 << 16 */
        *tag = options->step;
    }

    if ((flags & 0x1000000) != 0) {                 /* 128 << 17 */
        effect->callback = options->callback;
    }
}

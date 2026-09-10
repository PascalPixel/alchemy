#include "types.h"

#define SceneEffect_SpawnConfigured Func_02000118
/*
 * Effect spawning for overlay resource_39f.  Every cutscene beat in the
 * overlay creates its effects through this routine.
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
    s32 dur;
};

extern struct Descriptor *Data_0200d1d4[];

struct Effect *Func_02004fe6();
struct Effect *Func_02004f44();
void Func_02004f4e();
void Func_02004f68();
void Func_02005132();
s32 Func_02004ff4();
s32 Func_0200500c();
s32 Func_0200501a();
void Func_02005068();
void Func_02005078();

/* Returns the party record; only its presentation block at +80 is read. */

/*
 * Creates an effect at (x, y, z) with the constant velocity (vx, vy, vz) that
 * the per-frame integrator applies, then sets whichever optional fields
 * `flags` selects from `options`.  The 472-byte owner includes its three pool
 * words 0x0200b058, 0x020080e1 and 0xffff0000.
 */
void SceneEffect_SpawnConfigured(s32 x, s32 y,
                   s32 z, s32 vx, s32 vy, s32 vz, u32 flags,
                   const struct Options *options)
{
    u32 off;
    struct Effect *party;
    u32 bits;
    s32 mask;
    u32 tmp;
    struct Effect *effect;
    struct Sprite *block;
    struct Sprite *blk;
    u32 opt;
    u16 *tag;
    s32 dur;
    s32 delta0;
    s32 acc;
    party = Func_02004fe6(0);

    /* 128 << 13.  With this bit set and an options block present the effect's
     * kind comes from the options rather than from the default 222. */
    if ((flags & 0x100000) != 0 && options != 0) {
        effect = Func_02004f44(options->kind, x, y, z);
    } else {
        effect = Func_02004f44(222, x, y, z);
    }
    if (effect == 0) return;

    block = effect->sprite;
    blk = block;

    Func_02004f4e(effect, (flags + 1) & 15);
    off = (flags & 15) << 2;
    Func_02004f68(effect, Data_0200d1d4[off >> 2]);

    effect->mode55 = 0;
    block->state26 = 0;

    /* 0x020080e1 is the per-frame integrator's loader-relocated call word,
     * not a runtime address. */
    effect->callback = 0x020080e1;

    effect->velocity_x = vx;
    x = 3;
    effect->velocity_y = vy;
    effect->velocity_z = vz;

    /* Bits 2 and 3 of the effect's mode byte are copied from the party's. */
    bits = party->sprite->flags9 & 12;
    tmp = *(volatile u8 *)&block->flags9;
    mask = ~12;
    block->flags9 = (u8)((tmp & mask) | bits);

    effect->rate30 = 0;
    effect->rate34 = 0;
    effect->step64 = 0;
    tag = &effect->step64;

    /* The rest is optional detail, skipped unless a high flag bit is set and
     * an options record was supplied. */
    if ((flags & 0xffff0000) == 0 || options == 0) return;

    if ((flags & 0x10000) != 0) {                   /* 128 << 9 */
        Func_02005132(effect, options->mode);
    }

    if ((flags & 0x20000) != 0) {                   /* 128 << 10 */
        effect->flags23 &= 0xfe;
        opt = *(const u8 *)options & x;
        block->flags9 = (u8)((*((const u8 *)blk + 9) & mask)
                             | (opt << 2));
    }

    if ((flags & 0x80000) != 0) {                   /* 128 << 12 */
        effect->accum18 = options->accum18;
        effect->accum1c = options->accum1c;
    }

    if ((flags & 0x40000) != 0) {                   /* 128 << 11 */
        const struct Descriptor *desc =
            Data_0200d1d4[off >> 2];
        s32 delta;

        /* This 0x80000 test reads the register the previous block left live
         * and must not be respelled as a fresh load of `flags`.  With a
         * destination supplied the step is measured from it, otherwise the
         * target is biased by -1.0 in 16.16. */
        if ((flags & 0x80000) != 0) {
            delta0 = *(volatile const s32 *)&options->target30;
            acc = *(volatile const s32 *)&effect->accum18;
            delta0 -= acc;
            effect->rate30 = Func_02004ff4(delta0,
                                           desc->dur);
            delta = options->target34;
            dur = desc->dur;
            delta -= effect->accum1c;
        } else {
            delta0 = options->target30;
            delta0 += (s32)0xffff0000;
            effect->rate30 = Func_0200500c(delta0,
                                           desc->dur);
            delta = options->target34;
            dur = desc->dur;
            delta += (s32)0xffff0000;
        }

        /* Only the first call is per-arm.  Both arms join onto one second
         * call site, so each arm computes its own delta and the call is
         * spelled once here. */
        effect->rate34 = Func_0200501a(delta, dur);
    }

    if ((flags & 0x200000) != 0) {                  /* 128 << 14 */
        Func_02005068(effect, 1);
        Func_02005078(effect, options->callback_arg);
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

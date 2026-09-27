/* NONMATCHING: 1264 bytes, candidate 1264, 201 differing halfwords, 87
 * halfword edits (2026-09-27). CommandInterpolationRenderer_Update, meant
 * for FIELD/KOROSSEO_KAWA/F_021B8.C as a single-overlay unit binding its
 * names at their runtime addresses (an import veneer's listing offset plus
 * 0x8000). Remaining: Rebuilt command loop, three signed interpolation
 * channels, affine parameter bitfields, sprite records and IO queue from
 * disassembly. Verified helper prototypes and every literal. Complete size
 * matches; remaining register lifetimes, zero-load width, and instruction
 * scheduling differ.
 * WALL: Register lifetimes across interpolation and sprite emission;
 * preserve the separate per-case shifted affine index. Separate channel
 * locals restore duration/start registers. Computing delta before the step
 * regressed to 1252 bytes and 180 edits. Runtime bindings are registered in
 * korosseo-command-renderer-candidate.
 * 2026-09-27 H1: transfer IO_WRITE_QUEUE.C read boundary and count-store
 * alias to both tail publications. Baseline 1264/240/122 becomes
 * 1264/241/127. Both saved copies now precede masking, and the complete
 * trailing pool order matches, but queue/IME become r4/r0 instead of r0/r1;
 * the shared entry cursor still forces extra copies. Interpolation and
 * sprite emission are unchanged. Keep this negative witness in history.
 * 2026-09-27 H2: request-local saved IME/count/entry cursor, following the
 * exact world-map QueueTransfer scope, fixes both queue publications. The
 * complete tail at +0x452 through return and its trailing pool now match;
 * queue/IME/saved are r0/r1/r4. Whole score is 1264/201/87, equal topology.
 * Stop this bounded queue axis: remaining interpolation and sprite work is
 * independent, and no complete owner is adopted or credited. This family
 * also occurs at resource_3bb:02002450 and resource_3bc:02002ee8; do not
 * adopt the twins until the complete canonical owner is exact.
 * 2026-09-27 division-interface H1: exact COMMON/EFFECT/SPAWN.C and the
 * local import table prove ordinary signed division at resident 03000380.
 * Replace all three interpolation helper calls with C / duration and bind
 * __divsi3 to the same 0200bb00 veneer. The full 1264-byte candidate is
 * byte-identical to the prior model (cmp): 201 halfwords / 87 edits. Thus
 * the scale r9/fp and counter/endpoint scratch lifetimes do not change;
 * unlike the palette caller-save witness, this body gains no new save.
 * The complete +452..+4f0 queue tail and pool remain exact. Keep the
 * proven arithmetic interface, close this axis without a spelling sweep,
 * and do not instantiate production twins or credit any bytes.
 * 2026-09-27 interpolation-source audit: rechecked the complete normalized
 * diff and all three channels against exact SRC precedents. BLEND.C uses
 * unsigned-byte countdown/volatile endpoints; DisplayTransition_Update-
 * FromCentre uses signed-byte state and checks completion before advancing;
 * BattleFx_StepRatioTransition has signed-halfword step/duration but s32
 * endpoints and computes delta before the increment (already rejected
 * above). Window geometry uses a fixed-point reciprocal and its caller
 * advances the counter after rendering. None proves a new inline-channel
 * lifetime model for this owner. Here each frame is incremented, stored as
 * s16, then sign-extended for (target-start)*frame/duration; frame>=duration
 * only clears duration after evaluating the result, without clamping it.
 * The reference still differs in scale/duration r9/fp ownership, frame/
 * endpoint loads and single-sprite clipping-coordinate lifetimes. Baseline
 * remains 1264/1264 bytes, 201 halfwords / 87 aligned edits, with the full
 * +0x452..+0x4f0 queue tail and pool exact. No new supported transfer was
 * found: no variant compiled, no aggregate/register permutations, no twin
 * propagation. Preserve the body and the 3792-byte family as not-yet-C.
 * 2026-09-27 shared-draft transfer: the three ROM owners have 531 identical
 * instruction positions and 182 pool/alignment bytes, differing only in
 * proven calls and literal bindings. Separate not-yet-C units now score
 * this one source at 3ba:020021b8, 3bb:02002450 and 3bc:02002ee8. All three
 * independently compile to 1264 bytes with 201 differing halfwords and
 * 87 aligned edits. The queue-tail solution is shared without copying the
 * draft; the remaining interpolation and sprite lifetimes are unchanged.
 * No owner is adopted, no production instance or DONE credit is added. */
#include "TYPES.H"
#include "IO_WRITE_QUEUE.H"

extern volatile u16 Data_04000208;

struct Sprite { u32 words[3]; };
struct SpriteTile { u16 pad, base; };
struct SpriteTransform { unsigned x : 16; unsigned y : 16; unsigned angle : 16; unsigned pad : 16; };
/* FAKEMATCH: a halfword zero aggregate keeps the interior literal pools. */
struct Half { u16 value; };
extern struct SpriteTile Data_03001b10[];
extern s16 Data_0200c57c, Data_0200c79c, Data_0200c7f8, Data_0200c76c;
extern s16 Data_0200c7f0, Data_0200c77c, Data_0200c778, Data_0200c768;
extern s16 Data_0200c754, Data_0200c7fc, Data_0200c794, Data_0200c798;
extern s16 Data_0200c7a8, Data_0200c784, Data_0200c790, Data_0200c764;
extern s32 Data_0200c770;
extern s16 *Data_0200c7a0;
extern u32 Data_0200c7c0[];
extern s32 Main_080000d8(void (*fn)(void));
extern void Main_080001b8(s32 slot);
extern s32 Main_080001e0(struct SpriteTransform *work);
extern void Main_080001e8(void *sprite, s32 priority);

/* FAKEMATCH: transfer the exact queue read boundary and count-store alias.
 * Each publication owns its cursor; only the hardware pointers persist. */
#define QueueRegister(address, value) \
{ \
    u32 saved; \
    s32 cnt; \
    do { saved = *ime; } while (0); \
    *ime = (u16)(u32)ime; \
    cnt = queue->count; \
    if (cnt < 32) { \
        u32 *entry = (u32 *)((u8 *)queue + cnt * 12 + 4); \
        *(u16 *)&queue->count = cnt + 1; \
        *entry++ = (value); \
        *entry++ = (address); \
        *entry = 0x20000; \
    } \
    *ime = saved; \
}

void CommandInterpolationRenderer_Update(void)
{
    u32 *write = Data_0200c7c0;
    struct Sprite *sprite = (struct Sprite *)write;
    s32 tile = Data_03001b10[Data_0200c57c].base >> 5;
    s32 scale, blend, pos;
    s32 matrix, i, x, y, left;
    u32 flags;
    struct SpriteTransform work;
    struct IoWriteQueue *queue;
    volatile u16 *ime;

commands:
    if (Data_0200c79c != 0)
        goto render;
    {
        switch (*Data_0200c7a0++) {
        case 0x4000:
            Data_0200c770 = *Data_0200c7a0++ << 8;
            Data_0200c7f8 = *Data_0200c7a0++;
            Data_0200c76c = 0;
            break;
        case 0x3000:
            Data_0200c7f0 = Data_0200c7f8;
            Data_0200c7f8 = *Data_0200c7a0++;
            Data_0200c76c = *Data_0200c7a0++;
            Data_0200c77c = 0;
            break;
        case 0x1000:
            Data_0200c768 = Data_0200c778;
            Data_0200c778 = *Data_0200c7a0++;
            Data_0200c754 = *Data_0200c7a0++;
            Data_0200c7fc = 0;
            break;
        case 0x2000:
            Data_0200c798 = Data_0200c794;
            Data_0200c794 = *Data_0200c7a0++;
            Data_0200c7a8 = *Data_0200c7a0++;
            Data_0200c784 = 0;
            break;
        case 0x7fff:
            Data_0200c79c = *Data_0200c7a0++;
            break;
        case -1:
            Main_080000d8(CommandInterpolationRenderer_Update);
            Main_080001b8(Data_0200c57c);
            return;
        }
    }
    goto commands;
render:
    Data_0200c79c--;
    if (Data_0200c754 == 0) {
        scale = Data_0200c778;
    } else {
        struct Half zero = { 0 };
        s32 duration, start, progress;
        duration = Data_0200c754;
        start = Data_0200c768;
        x = Data_0200c778;
        progress = ++Data_0200c7fc;
        scale = start + (x - start) * progress / duration;
        if (progress >= duration)
            Data_0200c754 = zero.value;
    }
    if (Data_0200c7a8 == 0) {
        blend = Data_0200c794;
    } else {
        struct Half zero = { 0 };
        s32 duration, start, progress;
        duration = Data_0200c7a8;
        start = Data_0200c798;
        x = Data_0200c794;
        progress = ++Data_0200c784;
        blend = start + (x - start) * progress / duration;
        if (progress >= duration)
            Data_0200c7a8 = zero.value;
    }
    if (Data_0200c76c == 0) {
        pos = Data_0200c7f8;
    } else {
        struct Half zero = { 0 };
        s32 duration, start, progress;
        duration = Data_0200c76c;
        start = Data_0200c7f0;
        x = Data_0200c7f8;
        progress = ++Data_0200c77c;
        pos = start + (x - start) * progress / duration;
        if (progress >= duration)
            Data_0200c76c = zero.value;
    }
    work.angle = 0;
    work.x = scale;
    work.y = scale;
    matrix = (s16)Main_080001e0(&work);
    Data_0200c770 += pos;
    pos = Data_0200c770 / 256;
    switch (Data_0200c790) {
    case 1: {
        u32 attr = matrix << 25;
        y = 56;
        flags = 0x80004000;
        for (i = 0; i < 4; i++) {
            x = pos + scale * (i * 32 - 48) / 256;
            left = x + 88;
            if ((u32)(x + 152) < 304) {
                x = left & 511;
                *write++ = 0;
                *write++ = (x << 16) | y | flags | attr | 0x700;
                *write++ = 0xf400 | tile;
                Main_080001e8(sprite++, 236);
            }
            tile += 8;
        }
        break;
    }
    case 3: {
        u32 attr = matrix << 25;
        y = 48;
        flags = 0x80004000;
        for (i = 0; i < 2; i++) {
            x = pos + scale * (i * 32 - 16) / 256;
            left = x + 88;
            if ((u32)(x + 152) < 304) {
                x = left & 511;
                *write++ = 0;
                *write++ = (x << 16) | y | flags | attr | 0x700;
                *write++ = 0xf400 | (tile + Data_0200c764);
                Main_080001e8(sprite++, 236);
            }
            tile += 8;
        }
        break;
    }
    case 4:
        y = 48;
        flags = 0xc0004000;
        if ((u32)(pos + 120) < 304) {
            x = (pos + 56) & 511;
            *write++ = 0;
            *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
            *write++ = 0xf400 | (tile + Data_0200c764);
            Main_080001e8(sprite, 236);
        }
        break;
    case 2:
        y = 48;
        flags = 0x80000000;
        if ((u32)(pos + 152) < 304) {
            x = (pos + 88) & 511;
            *write++ = 0;
            *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
            *write++ = 0xf400 | (tile + Data_0200c764);
            Main_080001e8(sprite, 236);
        }
        break;
    }
    queue = &gIoWriteQueue;
    ime = &Data_04000208;
    QueueRegister(0x04000050, 0x3f00)
    QueueRegister(0x04000052, ((16 - blend) << 8) | blend)
}

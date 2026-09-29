/* Astra scale-view test (2026-09-27): a union of signed word and
 * word-based low/high halfword fields keeps full-width scale arithmetic
 * while feeding work.x/work.y from the low-halfword view. Complete output
 * remains 1264/1264 bytes, 17 halfwords / 17 aligned edits: scale is still
 * r9, duration and affine index fp. All control flow and pools agree, but
 * the representation supplies no new allocation boundary. Reject this
 * single trial and restore the scalar; do not propagate to either twin.
 * NONMATCHING: 1264 bytes, candidate 1264, 17 differing halfwords, 17
 * halfword edits (2026-09-27). CommandInterpolationRenderer_Update, meant
 * for FIELD/KOROSSEO_KAWA/F_021B8.C as a single-overlay unit binding its
 * names at their runtime addresses (an import veneer's listing offset plus
 * 0x8000). Remaining: scale occupies r9 instead of fp; the three separate
 * duration pointers and per-case affine index occupy fp instead of r9.
 * The dependent affine-index initialization also schedules differently.
 * Complete extent, all literal pools, command parsing, interpolation load
 * order, clipping/placement and both IO publications match structurally.
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
 * No owner is adopted, no production instance or DONE credit is added.
 * 2026-09-27 Sol renderer H1: allocator dump pseudo 54 proves that shared
 * x owns both interpolation targets and sprite coordinates (r1, target r3).
 * Channel-local target lifetimes recover all three endpoint/counter load
 * sequences and the interior pool order. They also remove the extra two
 * coordinate copies in each sprite loop. Complete diff: 1260/1264 bytes,
 * 183 differing halfwords / 57 edits. Remaining: scale/duration fp/r9
 * roles and independent single-sprite clipping/placement coordinates; the
 * queue tail is structurally exact but shifted by -4 bytes. Not adopted.
 * 2026-09-27 Sol renderer H2: compute single-sprite left coordinates before
 * clipping, as the loop modes do. Their independent live values reproduce
 * both single-sprite blocks exactly and restore the full 1264-byte extent.
 * Complete diff: 17 halfwords / 17 edits, solely the exchanged scale fp/r9
 * and duration-pointer r9/fp roles plus the dependent affine-index moves.
 * All pools, the command loop and complete queue tail are exact. Not adopted.
 * 2026-09-27 Sol renderer H3: one duration pointer across all three channels
 * puts scale in fp, but over-prioritizes the pointer into r8 and displaces
 * tile/sprite to sl/r9 (28 halfwords / 28 edits). The approved compiler's
 * global.c allocno_compare uses floor_log2(n_refs)*n_refs/live_length:
 * timer 9/138 has priority 0.196, tile 15/324 0.139, sprite 18/680 0.106,
 * scale 7/168 0.083. This is a negative witness, not an accepted repair.
 * Keep the exact source topology and pools; no register spelling sweep.
 * 2026-09-27 Sol renderer H4: sharing the duration pointer for scale/blend
 * only gives 6 references across 92 instructions, priority 0.130. Tile r8
 * and scale fp match, but sprite r9 and duration sl exchange the reference's
 * sl/r9 roles: 21 halfwords / 21 edits. Complete extent and every pool match.
 * Priority alone does not predict the final allocation; the finite repair
 * catalog has no unambiguous source shape for this reciprocal pointer swap.
 * This remains a negative lifetime witness, with no adoption or credit.
 * 2026-09-27 Sol renderer H5: owning the initial cursor through the typed
 * Sprite record, then write = sprite->words, emits identical bytes to H4.
 * Sprite remains 18 references / 680 instructions; reversing the dependency
 * changes only its pseudo ID. ARM REG_ALLOC_ORDER is r8, sl, r9, fp: H4's
 * timer priority between tile and sprite therefore predicts the observed
 * swap exactly. Close this pointer-sharing axis and restore H2's 17-edit
 * candidate. H5's complete change was struct Sprite *sprite =
 * (struct Sprite *)Data_0200c7c0 followed by u32 *write = sprite->words;
 * its output is identical to H4.
 * 2026-09-27 Sol renderer H6: work.y = work.x gives the same scale allocation
 * lifetime, 7 references / 168 instructions, not the predicted reduction.
 * It also feeds the second packed write through the first record value and
 * changes scratch roles: 1264 bytes, 27 halfwords / 27 edits, all pools exact.
 * Close the affine-record aliasing axis; H2 remains the best 17-edit source.
 * No complete owner is adopted, and no native or alignment credit is added.
 * 2026-09-27 Sol renderer closing checkpoint: restored H2 independently
 * scores 1264 bytes, 17 halfwords / 17 edits against all three ROM owners,
 * including every pool. For twins, select this source explicitly with
 * score <this-file> --owner resource_3bb:02002450 (or resource_3bc:02002ee8).
 * Owner-only resolution selects their older address-named drafts instead
 * of this registered shared candidate. No production tooling is changed.
 * Bounded pointer ownership/sharing and affine aliasing axes are closed;
 * resume only with a new source-lifetime fact, not spelling permutations.
 * 2026-09-27 Sol renderer H7: one reusable static __inline__ s32 evaluator
 * took s16 *timer, *from, *to, *step. It assigned a helper-local result in
 * both branches and returned it after the late duration clear; its active
 * branch retained Half zero, duration/start/target loads, ++*step and signed
 * comparison. Predicted: the returned-result boundary separates channel
 * scratch from final scale and reduces its 7-reference allocation priority.
 * Complete diff instead gives 1248/1264 bytes, 485 differing halfwords and
 * 141 aligned edits. Initial RTL has result 179 -> scale 49 after the clear;
 * local allocation coalesces scale 49 into result 179: 7 references across
 * 167 instructions, still r9 (baseline 7/168). Timer 175/204/233 remains
 * 3 references / 46 instructions and fp. Real pointer arguments materialize
 * endpoint/counter addresses before the branch; the first pool shifts +8,
 * and the final sprite/queue region shifts -16. Increment/store/sign-extend
 * semantics remain, but load order and pools fail the complete-owner gate.
 * No allocator fact supports a follow-up that separates the returned result.
 * Close the returned-result helper axis, restore H2, and stop this bounded
 * experiment. All three owners remain not-yet-C; 0 function/alignment credit.
 * Astra phase trial (2026-09-27): wrap only the first complete scale
 * evaluator in do/while(0), transferring the proven one-pass boundary.
 * Full result 1264/1264 bytes, 66 halfwords / 65 aligned edits: GCC moves
 * the zero-duration arm before the evaluator, reverses the entry branch,
 * changes all channel reload registers and one pool's order. Scale is
 * still r9 and duration fp. Reject this boundary, retain H2, and do not
 * transfer the failed phase or sweep wrappers across the other channels.
 * Astra outer-channel trial: share timer only between scale and position.
 * Prediction: the intervening blend channel lengthens its lifetime and
 * puts timer below sprite but above scale. Result 1264/1264, 21 halfwords /
 * 21 edits: allocator counts only the active segments, still 6 references
 * over 92 instructions. Sprite/timer exchange sl/r9 just as H4; the gap
 * adds no lifetime. The finite repair command refuses the volatile source;
 * its reciprocal catalog targets XOR temporaries, not this pointer pair.
 * No volatile qualifier or tool guard was changed. Retain the 17-edit body. */
/* 2026-09-27 renderer continuation: sharing the timer pointer between blend
 * and position keeps 1264 bytes but exchanges sprite/timer sl-r9 roles
 * (21 edits). Reusing it for the scene switch expands to 1268/129 edits.
 * One cursor for both word writes and sprite submission expands to 1272/141
 * edits and loses the required 20-byte frame. These distinct ownership
 * models fail; retain the exact-topology 17-edit draft for all three twins.
 * Sharing only test/store access through a timer pointer across scale/blend,
 * then sharing only active-branch duration/store access, both compile to the
 * same 21-edit output as full pointer sharing. CSE restores the full pointer
 * lifetime, so the predicted four-use allocation boundary never appears. */
#include "TYPES.H"
#include "IO_WRITE_QUEUE.H"

extern volatile u16 RegIme;

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
        s32 duration, start, progress, target;
        duration = Data_0200c754;
        start = Data_0200c768;
        target = Data_0200c778;
        progress = ++Data_0200c7fc;
        scale = start + (target - start) * progress / duration;
        if (progress >= duration)
            Data_0200c754 = zero.value;
    }
    if (Data_0200c7a8 == 0) {
        blend = Data_0200c794;
    } else {
        struct Half zero = { 0 };
        s32 duration, start, progress, target;
        duration = Data_0200c7a8;
        start = Data_0200c798;
        target = Data_0200c794;
        progress = ++Data_0200c784;
        blend = start + (target - start) * progress / duration;
        if (progress >= duration)
            Data_0200c7a8 = zero.value;
    }
    if (Data_0200c76c == 0) {
        pos = Data_0200c7f8;
    } else {
        struct Half zero = { 0 };
        s32 duration, start, progress, target;
        duration = Data_0200c76c;
        start = Data_0200c7f0;
        target = Data_0200c7f8;
        progress = ++Data_0200c77c;
        pos = start + (target - start) * progress / duration;
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
        left = pos + 56;
        if ((u32)(pos + 120) < 304) {
            x = left & 511;
            *write++ = 0;
            *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
            *write++ = 0xf400 | (tile + Data_0200c764);
            Main_080001e8(sprite, 236);
        }
        break;
    case 2:
        y = 48;
        flags = 0x80000000;
        left = pos + 88;
        if ((u32)(pos + 152) < 304) {
            x = left & 511;
            *write++ = 0;
            *write++ = (x << 16) | y | flags | (matrix << 25) | 0x700;
            *write++ = 0xf400 | (tile + Data_0200c764);
            Main_080001e8(sprite, 236);
        }
        break;
    }
    queue = &gIoWriteQueue;
    ime = &RegIme;
    QueueRegister(0x04000050, 0x3f00)
    QueueRegister(0x04000052, ((16 - blend) << 8) | blend)
}

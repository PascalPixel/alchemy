#include "types.h"

/*
 * Battle-presentation scene at main:080d52c8 (1768 bytes), a member of the
 * 0x03001eec "battle work" family already partly recovered in
 * games/gs1/recon/en/main/080ca60c.c, 080ea0d8.c and 080d5c48.c.  The shared
 * prologue (work = heap[0], draw destination = heap[1], effect state
 * republished at work + 0x7828), the `Value_XXXXXXXX` pooled resource-id
 * idiom, the Func_080ed408(46/47, ...) blit-routine publication into
 * Data_03001e50[] and the Data_080edebe/eca/ed0 decoration tables are all
 * reused from those owners unchanged.
 *
 * Behaviour: the second argument selects one of eight scene variants.  The
 * variant chooses the palette resource fetched through Func_08002f40 and
 * copied to 0x05000000, how many extra graphics sets are decompressed into
 * the work buffer, how many particles per member are animated (16, 24 or
 * 32), the seeding scale of the 512-record particle field at 0x02010000
 * (eight groups of 64, one group per member), the sprite geometry of the
 * per-particle blit, and whether the particles converge on the origin
 * (velocity decayed by 1/distance) or simply fall.
 *
 * The frame loop runs (state->count * 8 + 64) frames for the converging
 * variants and (state->count * 8 + 32) for the falling ones, one frame per
 * Func_080030f8(1).  Each frame it rebuilds a 160-entry sine ramp at
 * work + 0x6980, then walks the members: member `i` starts at frame i * 8,
 * fires Func_080d6888 at frame i * 8 + 16, and animates its own 64-record
 * particle group.  Variant 3 additionally scatters two random decorations
 * per member during the member's first 32 frames.
 *
 * `Func_080072f0` is not a real callee: it is the r3 entry of the
 * `_call_via_rN` trampoline bundle at games/gs1/asm/080072e4.s, so the two
 * sites that reach it are spelled here as what they are -- typed indirect
 * calls to the fixed IWRAM block copier at 0x03001388 and the fixed IWRAM
 * integer square root at 0x030001d8, the same reading already adopted in
 * games/gs1/recon/en/main/080d1714.c and 08093af8.c.  The six-argument
 * rectangle blits reach the r4 entry (0x080072f4) and are ordinary indirect
 * calls through the DrawRectangleFn locals.
 *
 * Uncertain: the roles of the effect-state fields other than count (20),
 * field_08 (8) and member[] (36); the meaning of the work-buffer control
 * words at 0x7780, 0x7784, 0x77a8 and 0x7824; and whether the small
 * pool-loaded resource numbers spelled `&Value_XXXXXXXX` are link-time ids
 * (the family precedent) or plain literals the compiler chose to pool.
 * The asymmetric >> 17 / >> 16 pair in the variant-3 decoration placement is
 * what the reference computes, not a transcription slip.  `frame & 3` is
 * spelled as the constant it is; both compilers substitute the `kind`
 * register for the 3 inside the variant-3 block, which is why the reference
 * reads `ands r7, r1` there.
 *
 * Residual: this draft emits 1760 of the reference's 1768 bytes with the same
 * frame size (116), the same local-array offsets (80 / 92 / 104), the same
 * loop structure and the same call sequence, but it is not byte-exact
 * (505 differing halfwords).  What remains is allocation and cross-jumping:
 *   - `work` lands in fp where the reference uses r9 (the loop counter and
 *     `work` are ranked the other way round), which renames one operand in
 *     most of the body;
 *   - the reference keeps an extra un-reduced `i * 8` in its own stack slot
 *     beside the induction variable this draft leaves as the only copy;
 *   - the reference cross-jumps variant 5 into the default variant's shared
 *     Func_080e0524 tail and leaves variants 1 and 4 separate; this draft
 *     does the opposite, which is where the eight missing bytes are.
 * Six source hypotheses were tried against these three; separating the
 * decoration angle from the ramp angle and loading the particle component
 * into a temporary before the decay call both closed real structure, and
 * three further splits of the shared loop counter made the allocation worse
 * and were reverted.
 */

#define BattleEffect_RunParticleBurstScene Func_080d52c8

typedef void (*WordCopyFn)(void *dest, const void *src, s32 size);
typedef s32 (*IntegerSqrtFn)(s32 value);
typedef void (*DrawRectangleFn)(
    void *dest, const void *src, s32 x, s32 y, s32 width, s32 height);

/* Heap-block cache: Data_03001e50[kind] holds the block Func_080ed408
   published for that display kind. */
extern void *Data_03001e50[];

extern const u16 Data_080edebe[];
extern const u8 Data_080edeca[];
extern const u8 Data_080eded0[];
extern const u8 Data_080ee2ae[];

/* Value_ symbols carry a resource number the reference loads from its pool
   rather than materializing with a mov. */
extern u8 Value_0000008d;
extern u8 Value_0000009e;
extern u8 Value_000000a0;
extern u8 Value_000000a3;
extern u8 Value_000000aa;
extern u8 Value_000000b7;
extern u8 Value_000000bb;
extern u8 Value_000000c0;
extern u8 Value_000000cd;
extern u8 Value_000000ce;

/* Callees the project has not named yet keep their address spelling; the
   names in comments are the ones `alchemy inspect` resolves. */
s32 Func_080022ec(s32 numerator, s32 denominator);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
/* Runtime_ReleaseHeapBlock */
void Func_08002dd8(s32 kind);
/* get */
void *Func_08002f40(s32 id);
void Func_080030f8(s32 frames);
/* Scheduler_AddOrUpdateCallback */
void Func_080041d8(void *callback, s32 interval);
/* Scheduler_RemoveCallback */
void Func_08004278(void *callback);
/* random_16 */
u32 Func_08004458(void);
/* Render_ResetTransformState */
void Func_080049ac(void);
/* SceneTransform_ApplyPitch */
void Func_08004bd4(s32 angle);
/* SceneTransform_ApplyYaw */
void Func_08004c1c(s32 angle);
/* SceneTransform_ApplyPosition */
void Func_08004cb4(const s32 *position);
/* Graphics_PrepareTransferInIwramWork */
void Func_080051d8(void *source, void *target);
void Func_080b5078(s32 a, s32 member_id, s32 c, s32 d);
void Func_080b5088(s32 member_id, s32 flag);
void **Func_080b5098(s32 member_id);
void Func_080b50e8(s32 id);
/* ObjectGroup_TickMemberTimers */
void Func_080cd52c(void);
void Func_080cd594(s32 mode);
s32 Func_080cdbc0(void);
/* update_members */
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
/* load_and_decompress */
void Func_080e0524(s32 resource_id, void *target, s32 flag_a, s32 flag_b);
/* Camera_ApplyShake */
void Func_080e155c(s32 a, s32 b);
/* apply_base_and_y_offset */
void Func_080e3944(const void *source, s32 *screen);
/* apply_step_and_y_offset */
void Func_080e396c(s32 member_id, s32 *screen);
s32 Func_080ed408(s32 kind, s32 a, s32 b, s32 c, s32 d);
void Func_080f9010(s32 cue);

/* 28-byte animation record.  512 of them live at 0x02010000: eight groups of
   64, one group per battle member. */
typedef struct {
    s32 x;
    s32 y;
    s32 z;
    s32 field_0c;
    s32 field_10;
    s32 field_14;
    s32 tick;
} Particle;

/* The effect state handed in by the caller and republished at work + 0x7828
   for the rest of the subsystem. */
typedef struct {
    s32 field_00;
    s32 field_04;
    s32 field_08;
    s32 field_0c;
    s32 field_10;
    s32 count;
    s32 field_18;
    s32 field_1c;
    s32 field_20;
    s16 member[8];
} Efx;

#define WORK_EFX (*(Efx **)((s8 *)work + 0x7828))
#define WORK_S32(off) (*(s32 *)((s8 *)work + (off)))
/* One unsigned 8-bit random sample, re-centred around zero. */
#define RANDOM_BIASED(bias) ((s32)(Func_08004458() & 0xFF) - (bias))

void BattleEffect_RunParticleBurstScene(void *object, u32 kind)
{
    void **heap;
    void **cursor;
    void *work;
    void *destination;
    void *iwram;
    void *member_object;
    Particle *p;
    s32 *ramp;
    DrawRectangleFn blit[2];
    s32 resource_id;
    s32 count;
    s32 total;
    s32 frame;
    s32 base_frame;
    s32 angle;
    s32 seed;
    s32 radius;
    s32 distance;
    s32 cel;
    s32 span;
    s32 v;
    s32 x;
    s32 y;
    s32 i;
    s32 j;
    s32 place[3];
    s32 screen[3];
    s32 base[3];

    heap = (void **)0x03001EEC;
    cursor = heap;
    work = *cursor++;
    destination = *cursor;
    count = 16;
    WORK_EFX = (Efx *)object;
    Func_080cd594(0);
    Func_080e0524((s32)&Value_0000009e, work, 1, 1);

    if (kind == 0) {
        resource_id = (s32)&Value_000000a0;
    } else if (kind == 1) {
        resource_id = (s32)&Value_000000bb;
    } else if (kind == 2) {
        resource_id = (s32)&Value_000000a3;
    } else if (kind == 3) {
        resource_id = (s32)&Value_000000c0;
    } else if (kind == 4) {
        resource_id = (s32)&Value_000000bb;
    } else if (kind == 5) {
        resource_id = (s32)&Value_000000b7;
        Func_080e0524(resource_id, work, 1, 0);
    } else if (kind == 7) {
        count = 24;
        Func_080e0524((s32)&Value_000000b7, work, 1, 0);
        resource_id = (s32)&Value_0000008d;
    } else {
        resource_id = (s32)&Value_000000cd;
        count = 32;
        Func_080e0524(resource_id, work, 1, 0);
    }

    ((WordCopyFn)0x03001388)(
        (void *)0x05000000, Func_08002f40(resource_id), 128);

    if (kind == 4) {
        Func_080e0524((s32)&Value_000000aa, work, 1, 1);
    }
    if (kind == 3) {
        Func_080e0524((s32)&Value_000000ce, (s8 *)work + (150 << 6), 1, 0);
    }

    p = (Particle *)0x02010000;
    j = 0;
    do {
        if (kind <= 1 || kind == 4 || kind == 5 || kind == 6 || kind == 7) {
            p->x = RANDOM_BIASED(127) << 15;
            p->y = RANDOM_BIASED(127) << 14;
            p->z = RANDOM_BIASED(127) << 15;
        } else {
            p->x = RANDOM_BIASED(127) << 13;
            p->y = RANDOM_BIASED(255) << 13;
            p->z = RANDOM_BIASED(127) << 13;
        }
        p->tick = 0;
        j++;
        p++;
    } while (j != 512);

    if (kind <= 1 || kind == 4 || kind == 5 || kind == 6 || kind == 7) {
        total = (WORK_EFX->count << 3) + 64;
    } else {
        total = (WORK_EFX->count << 3) + 32;
    }

    if (!(kind <= 1 || kind == 3)) {
        Func_080041d8((void *)0x080DBB9D, 0x480);
    }
    WORK_S32(0x7780) = 2;
    WORK_S32(0x7784) = 75;
    Func_080041d8((void *)0x080CD261, 0x480);
    Func_080f9010(142);

    for (frame = 0; frame != total; frame++) {
        iwram = *(void **)0x03001E80;

        if (kind == 7) {
            if (frame == total - 46) {
                Func_080b5078(
                    WORK_EFX->field_08, WORK_EFX->member[0], 16, 0);
            }
            if (frame == total - 32) {
                Func_080b50e8(134);
                Func_080b5088(WORK_EFX->member[0], 4);
                WORK_S32(0x77A8) = 8;
            }
        } else if (frame == total - 32) {
            Func_080b50e8(133);
        }

        ramp = (s32 *)((s8 *)work + (211 << 7));
        angle = frame << 12;
        j = 0;
        do {
            *ramp++ = ((128 << 11) - (Func_08002322(angle) << 2)) >> 10;
            angle += 128 << 4;
            j++;
        } while (j != 160);

        i = 0;
        if (WORK_EFX->count != 0) {
            do {
                base_frame = i * 8;
                member_object = *Func_080b5098(WORK_EFX->member[i]);

                if (kind == 3 && frame > base_frame
                    && frame < base_frame + 32) {
                    Func_080e396c(WORK_EFX->member[i], base);
                    j = 0;
                    do {
                        seed = (s32)(Func_08004458() & 0xFFFF);
                        radius = (s32)(Func_08004458() & 31) + 4;
                        x = ((base[0] / 2)
                                + ((radius * Func_08002322(seed)) >> 17))
                            - (Data_080edeca[frame & 3] >> 1);
                        y = (base[1] - ((radius * Func_0800231c(seed)) >> 16))
                            - (Data_080eded0[frame & 3] >> 1);
                        Func_080ed408(47, 7, 7,
                            3 | Data_080ee2ae[Func_08004458() & 3], 2);
                        ((DrawRectangleFn)Data_03001e50[47])(destination,
                            ((s8 *)work + Data_080edebe[frame & 3])
                                + (150 << 6),
                            x, y + 16, Data_080edeca[frame & 3],
                            Data_080eded0[frame & 3]);
                        Func_08002dd8(47);
                        j++;
                    } while (j != 2);
                }

                Func_080ed408(46, 7, 7, 3, 3);
                blit[0] = (DrawRectangleFn)Data_03001e50[46];
                Func_080ed408(47, 7, 7, 3, 2);
                blit[1] = (DrawRectangleFn)Data_03001e50[47];

                Func_080049ac();
                Func_080051d8(iwram, (s8 *)iwram + 12);

                screen[0] = *(s32 *)((s8 *)member_object + 8);
                screen[1] = 160 << 13;
                screen[2] = *(s32 *)((s8 *)member_object + 16);
                Func_08004cb4(screen);

                if (frame > base_frame) {
                    Func_08004c1c(frame << 9);
                    if (kind <= 1 || kind == 4) {
                        Func_08004bd4(frame << 9);
                    }

                    j = 0;
                    if (count != 0) {
                        p = (Particle *)0x02010000 + i * 64;
                        do {
                            if (frame > base_frame + j) {
                                distance = ((IntegerSqrtFn)0x030001D8)(
                                               ((p->x >> 8) * (p->x >> 8))
                                               + ((p->y >> 8) * (p->y >> 8))
                                               + ((p->z >> 8) * (p->z >> 8)))
                                    >> 9;
                                if (distance != 0 && p->tick <= 23) {
                                    cel = p->tick / 4;
                                    Func_080e3944(p, place);
                                    place[0] = place[0] >> 1;

                                    if (kind == 5 || kind == 7) {
                                        blit[1](destination,
                                            (s8 *)work + (cel * 25) * 64,
                                            place[0] - 20,
                                            place[1] - 20, 40, 40);
                                    } else if (kind == 6) {
                                        blit[1](destination,
                                            (s8 *)work + (192 << 4),
                                            place[0] - 6,
                                            place[1] - 12, 12, 24);
                                    } else if (kind == 4) {
                                        blit[1](destination, work,
                                            place[0] - 11,
                                            place[1] - 21, 22, 42);
                                    } else {
                                        blit[(j & 3) != 0](destination,
                                            (s8 *)work + (cel * 9) * 128,
                                            place[0] - 12,
                                            place[1] - 24, 24, 48);
                                    }

                                    if (kind <= 1 || kind == 4 || kind == 5
                                        || kind == 6) {
                                        v = p->x;
                                        p->x = v - Func_080022ec(v, distance);
                                        v = p->y;
                                        p->y = v - Func_080022ec(v, distance);
                                        v = p->z;
                                        p->z = v - Func_080022ec(v, distance);
                                    } else {
                                        p->y += 128 << 9;
                                    }

                                    p->tick = p->tick + 1;
                                    if (p->tick == 24) {
                                        if (kind <= 1 || kind == 4 || kind == 5
                                            || kind == 6) {
                                            p->tick = 0;
                                        } else {
                                            p->x = RANDOM_BIASED(127) << 13;
                                            p->y = RANDOM_BIASED(255) << 12;
                                            p->z = RANDOM_BIASED(127) << 13;
                                        }
                                    }
                                }
                            }
                            j++;
                            p++;
                        } while (j != count);
                    }
                }

                Func_08002dd8(47);
                Func_08002dd8(46);

                if (frame == base_frame + 16) {
                    span = total - frame;
                    if (span > 31) {
                        span = 31;
                    }
                    Func_080d6888(WORK_EFX->member[i], 7, 5, i, span);
                }

                i++;
            } while (i != WORK_EFX->count);
        }

        Func_080e155c(16, 16);
        Func_080cd52c();
        WORK_S32(0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((void *)0x080CD261);
    if (!(kind <= 1 || kind == 3)) {
        Func_08004278((void *)0x080DBB9D);
    }
    Func_080cdbc0();
}

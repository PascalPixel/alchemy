#include "types.h"

#define BattlePres_RunRingAndSparkScene Func_080e40a4

/*
 * Battle-presentation ring-and-spark scene at 0x080e40a4.
 *
 * One argument, the effect state pointer, parked at work + 0x7828 like
 * every other member of the 0x03001eec "battle work" family (see
 * games/gs1/recon/en/main/080e2974.c and 080e08c0.c for the shared
 * heap-cache cursor prologue, the Value_xxxxxxxx resource-id idiom and
 * the _call_via_rN trampoline spelling reused here).
 *
 * Setup: latch whether the state's s32 at 0 exceeds 199, publish the
 * state, run one paced frame, program BG1 control, then register the two
 * blit routines 0x2E and 0x2F with a shade that depends on the state's
 * s32 at 4 and keep their entry points.  Three more paced frames pull
 * resource 0x49 over the work block, resource 0x4A into 0x02010000 and,
 * when the state's s32 at 8 exceeds 7, one 128-word palette through the
 * IWRAM copy routine at 0x03001388; resource 0x76 then lands on the
 * sprite source.  The 0x7780 / 0x7784 pacing pair is set to (1, 0) and
 * the 0x080CD261 scheduler callback is installed.
 *
 * First phase (only when the state's s32 at 0 exceeded 199): 64 records
 * of 28 bytes at work + 0x7080 are seeded with a radius of 16..79 and
 * three random 16-bit angles, the actor context's fields 0x24, 0x28,
 * 0x2C, 0x34 and 0x48 are saved and cleared around a mode change, the
 * actor's screen position is resolved once, and the 0x03001AD4 /
 * 0x03001AD6 camera-shake pair is primed from it.  Thirty-two frames
 * then rotate each record by its three angles, project it, bias it by
 * the anchor and the resolved y, clamp the projected z into [-60, 60],
 * shift it by 60 and blit a (5 or 6)-radius cell; every drawn record's
 * radius drops by 4, and a record only draws once the frame index has
 * reached index / 4.  The saved actor fields are restored afterwards.
 *
 * Second phase (always): the draw target and 0x06004000 are cleared, the
 * pacing pair becomes (2, 75), BG1 control is reprogrammed, the actor
 * anchor is resolved from the state's s16 at 0x24 and the shake offset is
 * clamped into [-128, 0].  The same 64 records are reseeded as sparks at
 * the actor's world position with random velocities and a stagger timer
 * of index + 16.  Thirty-two frames cue audio on frame 5, poke the actor
 * on frame 4, redraw one of four full 120x120 backdrops for the first
 * eight frames, and from frame 4 onwards step and draw the live sparks
 * under gravity.  That loop runs 64 times over the first 32 records, so
 * each live spark is drawn, advanced and decremented twice per frame; the
 * blit routine is chosen by the record index, not by the visit, so both
 * visits to one record use the same routine and neighbouring records
 * alternate between the two.  The scene closes by removing the callback,
 * releasing blit blocks 0x2E and 0x2F, and running seven paced frames of
 * a fading cue read from the pointer at 0x03001E74, plus 1608.
 *
 * Neither Func_080072f0, Func_080072f4 nor Func_080072f8 is a real
 * function symbol: they are entries of the _call_via_rN trampoline bundle
 * at games/gs1/asm/080072e4.s, so they are spelled here as calls through
 * typed pointers - the IWRAM word copier at 0x03001388, the IWRAM clear
 * routine at 0x03000164, and the two blit routines Func_080ed408
 * publishes into heap_cache[46] and heap_cache[47].
 *
 * The resource ids 0x49, 0x4A, 0x76 and 0x8E reach their calls through
 * the literal pool rather than a movs, which the project's external
 * Value_xxxxxxxx spelling reproduces; the original source form of those
 * ids is not recovered.
 *
 * The `val` scratch is a measured spelling, not a recovered one.  Thumb has
 * no store-immediate halfword, so the reference's `movs`/`ldr` then `strh`
 * pairs are not by themselves evidence of a source-level temporary; what is
 * evidence is the codegen.  Writing those four halfword stores directly
 * costs 28 bytes and 100 wrong instructions (measured: 1652 candidate
 * bytes, 758 differing halfwords, against 1624 and 588 with the scratch),
 * so the extra live word is load-bearing.  The first BG1 control word is
 * the exception
 * that is written directly, which restores the early four-word literal pool
 * at function offset 0x7c.  The original source form of both is unknown.
 *
 * Still uncertain: the role of the 28-byte record's first word, which the
 * first phase uses as a shrinking radius and the second phase as a world
 * x; why the first phase computes and stores a clamped, biased projected
 * z that the blit never reads; the meaning of the 0x7780 / 0x7784 /
 * 0x77B4 / 0x77B8 / 0x7824 work words; and why the final camera-shake
 * duration is written with the frame counter's terminal value.
 *
 * Residual against the reference: the recovered control flow, calls,
 * effects and aggregate offsets line up instruction for instruction, and
 * the four s32[3] scratch arrays land in the reference's order, but this
 * compile keeps the phase counter in memory instead of a high register.
 * The reference holds it in r9 and spills the address of `pos` to sp+12;
 * this compile does the reverse, keeping &pos in sl and giving the counter
 * its own frame slot at sp+20 (`sub sp, #116` against the reference's
 * `#112`).  That reload appears at every counter read and shifts the
 * scalar slots below the arrays by four or eight bytes; the arrays and the
 * saved-field block are otherwise laid out the same way.  Two secondary,
 * purely cosmetic consequences of the same assignment: the member context
 * sits in a low register here and in r8 in the reference, and the final
 * camera-shake duration is written as the literal 32 rather than reused
 * from the counter register.  Six source spellings were measured against
 * this - dropping the s32 scratch, collapsing the saved actor fields into
 * an array, a separate phase-1 frame counter, a pooled BG1 control word,
 * reusing the terminal loop values for the 64 and the 32 (no change at
 * all), and reordering the declarations to the reference's implied slot
 * order (587/482, a wash) - and none moved the allocation.  Moving it
 * would need a compiler-level fact, not another source spelling.
 */

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

/* The state pointer this owner parks in, and reloads from, work + 0x7828. */
#define STATE (*(void **)(work + 0x7828))

/* Camera-shake parameter block; only the offset and duration are used. */
struct ShakeParams {
    u8 unknown_0000[4];
    u16 offset;
    u16 duration;
};

#define SHAKE (*(struct ShakeParams *)0x03001AD0)

typedef void (*BlitFn)(
    void *dest, const void *src, s32 x, s32 y, s32 w, s32 h);
typedef void (*CopyFn)(void *dest, const void *src, s32 count);
typedef void (*ClearFn)(void *dest, s32 count);

/* Heap-allocation cache: Data_03001e50[kind] holds kind's block address.
   This owner reads kinds 9, 12, 39 (its work block), 40 (the draw target),
   41 (the sprite source) and the two blit entries 46 and 47.  The family
   spells the 39.. window through its own base, which is what the
   reference's 0x03001eec pool word is. */
extern void *Data_03001e50[];

/* Sixteen halfword cell offsets shared with the rest of the family. */
extern u16 Data_080ede5c[];

/* Value_ symbols carry a literal the reference loads from its pool rather
   than materializing with a mov. */
extern u8 Value_00000049;
extern u8 Value_0000004a;
extern u8 Value_00000076;
extern u8 Value_0000008e;

/* 28-byte records at work + 0x7080, reused by both phases: the first
   phase treats the first word as a shrinking radius and words 3..5 as
   rotation angles, the second phase as a world position, a velocity and
   a stagger timer. */
struct Particle {
    s32 x;
    s32 y;
    s32 z;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 timer;
};

/* Only the world position this owner copies out of the member context;
   the 0x24..0x48 window it saves and clears is reached by offset. */
struct Member {
    s32 field_0000;
    s32 field_0004;
    s32 x;
    s32 y;
    s32 z;
};

void Func_080b5030(s32 a, s32 b, s32 c);
void Func_080030f8(s32 frames);
void Func_080cdd58(void);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080e0524(s32 id, void *target, s32 flag_a, s32 flag_b);  /* load_and_decompress */
void *Func_08002f40(s32 id);                                       /* get */
void Func_080072f0(void *dest, const void *src, s32 count, CopyFn copier);
void Func_080041d8(void *callback, s32 interval);                  /* Scheduler_AddOrUpdateCallback */
struct Member **Func_080b5098(s32 member);
u32 Func_08004458(void);                                           /* random_16 */
void Func_08009088(struct Member *member, s32 mode);
void Func_080e396c(s32 source, s32 *out);                          /* apply_step_and_y_offset */
void Func_080f9010(s32 id);
void Func_080049ac(void);                                          /* Render_ResetTransformState */
void Func_08004c6c(s32 angle);
void Func_08004bd4(s32 angle);                                     /* SceneTransform_ApplyPitch */
void Func_08004c1c(s32 angle);                                     /* SceneTransform_ApplyYaw */
void Func_080e3944(const void *particle, s32 *out);                /* apply_base_and_y_offset */
void Func_08004278(void *callback);                                /* Scheduler_RemoveCallback */
s32 Func_080b5070(s32 member);
void Func_080b50e8(s32 id);
void Func_080b5088(s32 member, s32 kind);
void Func_080051d8(void *a, void *b);                              /* Graphics_PrepareTransferInIwramWork */
void Func_080e38b8(void *particle, s32 a, s32 b);                  /* advance_with_gravity_3d */
void Func_08002dd8(s32 id);                                        /* Runtime_ReleaseHeapBlock */
void Func_080b5048(s32 cue, s32 level);
void Func_080cdd14(void);                                          /* BattleFx_SetTransitionFlagAndDisplay */

void BattlePres_RunRingAndSparkScene(void *object)
{
    void **heap;
    void **cursor;
    u8 *work;
    void *draw_target;
    void *sprite_src;
    void *xfer;
    struct Member *member;
    struct Particle *p;
    const u16 *cue;
    BlitFn blit[2];
    s32 pos[3];
    s32 anchor[3];
    s32 tmp[3];
    s32 tmp2[3];
    s32 saved24;
    s32 saved28;
    s32 saved2c;
    s32 saved34;
    s32 saved48;
    s32 shake;
    s32 half;
    s32 size;
    s32 timer;
    s32 frame;
    s32 big;
    s32 i;
    s32 j;
    s32 val;

    heap = &Data_03001e50[39];
    cursor = heap;
    work = (u8 *)*cursor++;
    draw_target = *cursor;
    sprite_src = heap[2];
    xfer = heap[-27];

    big = 1;
    if (M2C_FIELD(object, s32 *, 0) <= 199) {
        big = 0;
    }

    STATE = object;
    Func_080b5030(
        M2C_FIELD(object, s32 *, 8), M2C_FIELD(object, s32 *, 12), 130);
    Func_080030f8(1);
    Func_080cdd58();
    *(u16 *)0x0400000A = 0x1F80;

    if (M2C_FIELD(STATE, s32 *, 4) == 0) {
        Func_080ed408(46, 7, 7, 3, 3);
        Func_080ed408(47, 7, 7, 3, 2);
    } else {
        Func_080ed408(46, 7, 7, 7, 3);
        Func_080ed408(47, 7, 7, 7, 2);
    }
    blit[0] = (BlitFn)Data_03001e50[46];
    blit[1] = (BlitFn)Data_03001e50[47];

    Func_080b5030(
        M2C_FIELD(STATE, s32 *, 8), M2C_FIELD(STATE, s32 *, 12), 130);
    Func_080030f8(1);
    Func_080e0524((s32)&Value_00000049, work, 1, 0);

    Func_080b5030(
        M2C_FIELD(STATE, s32 *, 8), M2C_FIELD(STATE, s32 *, 12), 130);
    Func_080030f8(1);
    Func_080e0524((s32)&Value_0000004a, (void *)0x02010000, 1, 1);

    if (M2C_FIELD(STATE, s32 *, 8) > 7) {
        Func_080072f0((void *)0x05000000,
            Func_08002f40((s32)&Value_0000008e), 128, (CopyFn)0x03001388);
    }

    Func_080b5030(
        M2C_FIELD(STATE, s32 *, 8), M2C_FIELD(STATE, s32 *, 12), 130);
    Func_080030f8(1);
    Func_080e0524((s32)&Value_00000076, sprite_src, 0, 0);

    Func_080b5030(
        M2C_FIELD(STATE, s32 *, 8), M2C_FIELD(STATE, s32 *, 12), 130);
    Func_080030f8(1);
    M2C_FIELD(work, s32 *, 0x7780) = 1;
    M2C_FIELD(work, s32 *, 0x7784) = 0;
    Func_080041d8((void *)0x080CD261, 0x480);

    if (big == 1) {
        /* Seed the ring: a shrinking radius and three random angles. */
        member = *Func_080b5098(M2C_FIELD(STATE, s32 *, 8));
        p = (struct Particle *)(work + 0x7080);
        i = 0;
        do {
            p->x = (s32)(Func_08004458() & 63) + 16;
            p->y = 0;
            p->z = 0;
            p->vx = (s32)(Func_08004458() & 0xFFFF);
            p->vy = (s32)(Func_08004458() & 0xFFFF);
            p->vz = (s32)(Func_08004458() & 0xFFFF);
            i++;
            p++;
        } while (i != 64);

        Func_08009088(member, 0);
        saved24 = M2C_FIELD(member, s32 *, 0x24);
        saved28 = M2C_FIELD(member, s32 *, 0x28);
        saved2c = M2C_FIELD(member, s32 *, 0x2C);
        saved48 = M2C_FIELD(member, s32 *, 0x48);
        saved34 = M2C_FIELD(member, s32 *, 0x34);
        M2C_FIELD(member, s32 *, 0x24) = 0;
        M2C_FIELD(member, s32 *, 0x28) = 0;
        M2C_FIELD(member, s32 *, 0x2C) = 0;
        M2C_FIELD(member, s32 *, 0x34) = 0;
        M2C_FIELD(member, s32 *, 0x48) = 0;

        Func_080e396c(M2C_FIELD(STATE, s32 *, 8), pos);
        shake = 64 - pos[0];
        SHAKE.offset = (u16)shake;
        val = 80;
        SHAKE.duration = val;
        M2C_FIELD(work, s32 *, 0x77B4) = 24;
        M2C_FIELD(work, s32 *, 0x77B8) = 0;
        Func_080041d8((void *)0x080CD4B5, 0xC80);
        Func_080f9010(212);

        frame = 0;
        do {
            Func_080b5030(M2C_FIELD(STATE, s32 *, 8),
                M2C_FIELD(STATE, s32 *, 12), 130);
            p = (struct Particle *)(work + 0x7080);
            i = 0;
            do {
                if (p->x >= 0 && frame >= i / 4) {
                    size = (i & 1) + 5;
                    Func_080049ac();
                    Func_08004c6c(p->vz);
                    Func_08004bd4(p->vx);
                    Func_08004c1c(p->vy);
                    Func_080e3944(p, tmp);
                    tmp[0] = tmp[0] + 64;
                    tmp[1] = tmp[1] + pos[1] + 24;
                    if (tmp[2] < -60) {
                        tmp[2] = -60;
                    }
                    if (tmp[2] > 60) {
                        tmp[2] = 60;
                    }
                    tmp[2] = tmp[2] + 60;
                    blit[0](draw_target,
                        (u8 *)sprite_src + Data_080ede5c[size - 1],
                        tmp[0] - size, tmp[1] - size, size * 2, size * 2);
                    p->x = p->x - 4;
                }
                i++;
                p++;
            } while (i != 64);
            M2C_FIELD(work, s32 *, 0x7824) = 1;
            Func_080030f8(1);
            frame++;
        } while (frame != 32);

        Func_08004278((void *)0x080CD4B5);
        Func_08009088(member, 16);
        M2C_FIELD(member, s32 *, 0x24) = saved24;
        M2C_FIELD(member, s32 *, 0x28) = saved28;
        M2C_FIELD(member, s32 *, 0x2C) = saved2c;
        M2C_FIELD(member, s32 *, 0x34) = saved34;
        M2C_FIELD(member, s32 *, 0x48) = saved48;
    }

    ((ClearFn)0x03000164)(draw_target, 0x4000);
    ((ClearFn)0x03000164)((void *)0x06004000, 0x4000);
    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    val = 0x1F81;
    *(u16 *)0x0400000A = val;

    Func_080e396c(M2C_FIELD(STATE, s16 *, 0x24), anchor);
    if (M2C_FIELD(STATE, s32 *, 4) == 0) {
        shake = 32 - anchor[0];
    } else {
        shake = 96 - anchor[0];
    }
    if (shake > 0) {
        shake = 0;
    }
    if (shake < -128) {
        shake = -128;
    }
    anchor[0] = anchor[0] + shake;
    val = 80;
    SHAKE.duration = val;
    SHAKE.offset = (u16)shake;

    /* Reseed the same records as sparks at the actor's world position. */
    member = *Func_080b5098(M2C_FIELD(STATE, s16 *, 0x24));
    half = Func_080b5070(M2C_FIELD(STATE, s16 *, 0x24)) / 2;
    p = (struct Particle *)(work + 0x7080);
    i = 0;
    do {
        p->x = member->x;
        p->y = member->y + half;
        p->z = member->z;
        p->vx = (s32)(Func_08004458() & 255) << 10;
        p->vy = (s32)(Func_08004458() & 255) << 10;
        p->vz = ((s32)(Func_08004458() & 255) - 127) << 10;
        if (p->x > 0) {
            p->vx = -p->vx;
        }
        p->timer = i + 16;
        i++;
        p++;
    } while (i != 64);

    frame = 0;
    do {
        if (frame == 5) {
            Func_080b50e8(134);
        }
        if (frame == 4) {
            Func_080b5088(M2C_FIELD(STATE, s16 *, 0x24), 0);
        }
        Func_080e396c(M2C_FIELD(STATE, s32 *, 8), pos);
        pos[1] = pos[1] + 16;

        if (frame <= 1) {
            blit[0](draw_target, work, 0, 0, 120, 120);
        } else if (frame <= 3) {
            blit[0](draw_target, work + 0x3840, 0, 0, 120, 120);
        } else if (frame <= 5) {
            blit[0](draw_target, (void *)0x02010000, 0, 0, 120, 120);
        } else if (frame <= 7) {
            blit[0](draw_target, (void *)0x02013840, 0, 0, 120, 120);
        }

        Func_080049ac();
        Func_080051d8(xfer, (u8 *)xfer + 12);

        if (frame >= 4 && frame <= 31) {
            i = 0;
            do {
                j = i / 2;
                p = (struct Particle *)(work + 0x7080) + j;
                timer = p->timer;
                if (timer > 0) {
                    Func_080e3944(p, tmp2);
                    tmp2[0] = tmp2[0] + shake;
                    size = (timer >> 3) + 2;
                    tmp2[1] = tmp2[1] + 16;
                    blit[j & 1](draw_target,
                        (u8 *)sprite_src + Data_080ede5c[size - 1],
                        tmp2[0] - size, tmp2[1] - size, size * 2, size * 2);
                    Func_080e38b8(p, 60, -0x400);
                    p->timer = p->timer - 1;
                }
                i++;
            } while (i != 64);
        }

        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
        frame++;
    } while (frame != 32);

    Func_08004278((void *)0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    val = 32;
    SHAKE.duration = val;

    cue = (const u16 *)((u8 *)*(void **)0x03001E74 + 1608);
    i = 0;
    do {
        Func_080b5048(*cue, 6 - i);
        Func_080030f8(1);
        i++;
    } while (i != 7);

    Func_080cdd14();
}

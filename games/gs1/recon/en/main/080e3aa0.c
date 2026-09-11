#include "types.h"

#define BattlePres_RunBeamScene Func_080e3aa0

/*
 * Battle-presentation beam scene at 0x080e3aa0 (1540 bytes).
 *
 * One argument, the effect object.  Like every other member of the
 * 0x03001eec "battle work" family (see the nearest drafted siblings
 * games/gs1/recon/en/main/080e2974.c and 080e4e0c.c for the shared
 * prologue, the heap-cache cursor pair, the work + 0x7828 state slot and
 * the callee evidence reused here; both of those are themselves
 * non-exact drafts, so they are a spelling precedent and not proof) the
 * owner parks the object at work + 0x7828 and then reloads it from there
 * at every use.
 *
 * Setup: read the scene selector out of the object's first word, publish
 * the object, resolve the actor record through Runtime_GetObject, wait a
 * frame, run the two presentation helpers Func_080c9048 / Func_080cdd58,
 * and program BG1CNT with 0x1F80.  The two rectangle-blit routines 0x2E
 * and 0x2F are then registered with a shade that depends both on the
 * scene selector (5 or not) and on the state's s32 at 4 -- the "which
 * side of the field" flag the whole family reads -- and their entry
 * points are kept in a two-element table on the stack.  ("shade" names
 * only the argument position; its role is not recovered.)  A per-scene
 * resource is decompressed over the work block, a 0x80-byte palette is
 * copied to 0x05000000 through the IWRAM copy routine at 0x03001388
 * (resource 0x8E when the state's s32 at 8 is greater than 7, otherwise
 * 0x4A), the sprite source gets resource 0x76 and 0x02010000 gets
 * resource 0x99.  The 0x7780 / 0x7784 pacing words are set to 2 and 50,
 * the 0x080CD261 scheduler callback is installed every 0x480 units, and
 * BG1CNT is reprogrammed to 0x1F81.
 *
 * The actor's screen anchor is resolved once through
 * EffectPosition_ApplyAnimationAndYOffset.  The horizontal correction dx
 * pulls that anchor toward 96 (or 32 on the other side), is clamped to
 * [-128, 0], is folded into the anchor and is published as the camera
 * record halfword at 0x03001AD4 with a fixed 80 in the halfword after it.  Then
 * 64 twenty-eight byte sparks are seeded at work + 0x7080 from the
 * actor's world position, lifted by half the actor's height, with random
 * velocities and a staggered timer of i + 16.  The reference negates the
 * spark's x velocity conditionally on a positive x and then negates it
 * again unconditionally; that pair is reproduced literally.
 *
 * The frame loop runs 32 frames.  Frame 5 passes one of two ids (134 or
 * 133) to Func_080b50e8, selected by Func_080b5058 applied to the actor
 * record's byte at 296; neither callee has a resolved project name, so
 * calling them a cue is a guess.  Frame 4 calls Func_080b5088 on the
 * actor with kind 0.  Every frame re-resolves the state's own
 * screen position through EffectPosition_ApplyStepAndYOffset, lifts it
 * by 16, and draws the beam body with blit 0x2F: for selector 4 a
 * 48x16 strip walked backwards through the work block for the first
 * twelve frames, for selectors 0..2 and 5 a 48x72 cell for the first
 * twelve frames, and otherwise an eighteen-frame sequence whose cell
 * offset, x offset, width and height come from the four parallel tables
 * at 0x080EEDBE, 0x080EEDCA, 0x080EEDB2 and 0x080EEDB8 indexed by
 * frame / 3.  Each of those three forms has a mirrored variant selected
 * by the same state s32 at 4.  Frames 4..15 additionally draw the
 * 40x48 impact sequence decompressed at 0x02010000 with blit 0x2E, and
 * frames 4..31 step and draw the sparks: each of 64 iterations works on
 * spark i / 2, so every live spark is drawn and ticked twice per frame,
 * with a size that grows with the remaining timer and a source offset
 * read from the shared cell table at 0x080EDE5C.  The frame ends by
 * setting work + 0x7824 and waiting one frame.  Afterwards the callback
 * is removed, blits 0x2F and 0x2E are released, the finished loop
 * counter (32) is written back over the 0x03001AD6 halfword and the
 * scene closes through BattleFx_SetTransitionFlagAndDisplay.
 *
 * `Func_080072f0` and `Func_080072f4` are not real function symbols: they
 * are entries of the `_call_via_rN` trampoline bundle at
 * games/gs1/asm/080072e4.s, i.e. indirect calls through whatever pointer
 * the compiler kept in r3 and r4.  Here they are the IWRAM copy routine
 * at 0x03001388 and the two blit entries Func_080ed408 publishes into
 * heap_cache[46] and heap_cache[47], so they are spelled as calls through
 * typed pointers.
 *
 * The resource ids 0x4A, 0x6B, 0x76, 0x8E, 0x99, 0xB5, 0xB6 and 0xC5 all
 * reach their calls through the literal pool rather than the one-word
 * `movs rN,#imm8` an ordinary small integer constant would use, so each
 * one is a relocated symbol address, spelled the way the family's
 * siblings already spell the same argument.  The original source form of
 * those ids is not recovered.
 *
 * Still uncertain: the roles of the 0x7780 / 0x7784 / 0x7824 words and of
 * the 0x03001AD0 camera record's two halfwords; what the four 0x080EEDxx
 * tables describe beyond the rectangle they select; why the sparks are
 * stepped twice per frame rather than once; and the exact source form of
 * the selector cascade -- the reference peels 4 and then 3 ahead of a
 * three-label comparison tree over {0,1}, {2} and {5}, which is what the
 * peeled `if` plus inner `switch` below reproduces exactly, but the
 * original may have been one statement the compiler split.
 *
 * Residual, measured with `alchemy score ... --owner 080e3aa0`: candidate
 * 1572 bytes against reference 1540 (102.1 %), topology equal,
 * differing_halfwords 623, wrong_instructions 498.  Every reference
 * branch, loop, switch, call and store is represented, with the same
 * multiplicities and the same block topology; the whole remaining
 * residual is one register-allocation tie-break plus the slot renumbering
 * it drags behind it.
 *
 * Both sides spill exactly ten values into sp+8..sp+44 and lay the same
 * 44 bytes of named locals into sp+48..sp+91 (blit, pos, anchor, tmp, in
 * that order -- getting that order right was worth 2 wrong instructions).
 * The reference gives fp to `work` and spills the address of `pos`; this
 * draft gives fp to the address of `pos` and spills `work`.  That single
 * swap accounts for the entire +32 byte overshoot: sixteen `add rN, fp`
 * become `ldr rN,[sp,#44]` plus `adds`, and because the remaining spill
 * values then land in different slots, nearly every sp-relative access
 * reads as a difference even where the instruction is otherwise
 * identical.  The aligned diff confirms the shape exactly: the reference
 * side is missing sixteen `add rN, fp` and three `mov rN, fp`, and the
 * candidate side carries the matching `ldr rN,[sp,#44]` plus `adds`.
 * Three source-level attempts to flip it were measured and reverted:
 * splitting the seed-loop counter into a third variable made it worse
 * (520 wrong instructions against 516); sharing the seed counter with
 * the spark counter instead, which is what the reference's sl / r9 pair
 * literally does, also made it worse (502 against 498); and no ordinary
 * C lifetime change shortens `work`, which is live from the second
 * instruction to the last use.  Forcing the register is not permitted
 * and was not attempted.  Two bounded searches on this axis have now
 * produced no new structural fact, so it is closed pending new evidence.
 *
 * One repair in this class did land: reaching both 0x03001AD0 halfwords
 * through one relocated struct base instead of two folded absolute
 * addresses removed two literal-pool words and six instructions
 * (1596 -> 1572 bytes, 637 -> 623 differing halfwords).
 */

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

/* The object pointer this owner parks in, and reloads from, work + 0x7828. */
#define STATE (*(void **)(work + 0x7828))

/* The 64 spark records live inside the work block at 0x7080. */
#define SPARKS ((struct Spark *)(work + 0x7080))

typedef void (*BlitFn)(
    void *dest, const void *src, s32 x, s32 y, s32 w, s32 h);
typedef void (*CopyFn)(void *dest, const void *src, s32 count);

#define REG_BG1CNT (*(volatile u16 *)0x0400000A)
#define IWRAM_COPY ((CopyFn)0x03001388)


/* Heap-allocation cache: Data_03001e50[kind] holds kind's block address.
   This owner reads kinds 12 (the transfer work), 39 (its work block),
   40 (the draw target), 41 (the sprite source) and the two blit entries
   46 and 47.  The family spells the 39.. window through its own base,
   which is what the reference's single 0x03001eec pool word is. */
extern void *Data_03001e50[];

/* Sixteen halfword cell offsets shared with the rest of the family. */
extern const u16 Data_080ede5c[];

/* Four parallel per-step tables: cell offset, x offset, width, height. */
extern const u16 Data_080eedbe[];
extern const u8 Data_080eedca[];
extern const u8 Data_080eedb2[];
extern const u8 Data_080eedb8[];

/* Value_ symbols carry a literal the reference loads from its pool rather
   than materializing with a mov. */
extern u8 Value_0000004a;
extern u8 Value_0000006b;
extern u8 Value_00000076;
extern u8 Value_0000008e;
extern u8 Value_00000099;
extern u8 Value_000000b5;
extern u8 Value_000000b6;
extern u8 Value_000000c5;

struct Spark {
    s32 x;
    s32 y;
    s32 z;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 timer;
};

/* The shared record at 0x03001AD0.  This owner only writes the two
   halfwords at 4 and 6, and reaches both through the one relocated base the
   reference keeps in a register, so they are spelled as struct members.
   This layout is NOT agreed across the corpus: the draft
   games/gs1/recon/en/main/080cdbc0.c declares the same address as a
   `struct Position` and writes s32 words at 0 and 4, which would overlap
   the halfword at 6 written here.  Both readings are drafts; the halfword
   widths below are what this owner's own strh instructions show, and the
   shared owner of the record is unresolved.  Any shared header must settle
   the conflict before either spelling is adopted. */
struct CameraRecord {
    s32 field_0000;
    s16 field_0004;
    s16 field_0006;
};

extern struct CameraRecord Data_03001ad0;

/* Only the world position this owner copies out of the member context. */
struct Member {
    s32 field_0000;
    s32 field_0004;
    s32 x;
    s32 y;
    s32 z;
};

/* Declared with the address spelling the link uses; the trailing comment is
   the name `alchemy inspect` resolves for that target, where it has one.
   The ones with no comment have no resolved project name. */
void Func_080030f8(s32 frames);                      /* WaitFrames */
s32 Func_080022ec(s32 numerator, s32 denominator);   /* FixedPoint_Ratio */
void Func_08002dd8(s32 id);                          /* Runtime_ReleaseHeapBlock */
void *Func_08002f40(s32 id);                         /* get */
void Func_080041d8(s32 callback, s32 interval);      /* Scheduler_AddOrUpdateCallback */
void Func_08004278(s32 callback);                    /* Scheduler_RemoveCallback */
u32 Func_08004458(void);                             /* random_16 */
void Func_080049ac(void);                            /* Render_ResetTransformState */
void Func_080051d8(void *a, void *b);                /* Graphics_PrepareTransferInIwramWork */
void Func_08005268(const struct Spark *position, s32 *out);
void *Func_08077008(s32 id);                         /* Runtime_GetObject */
s32 Func_080b5058(s32 id);
s32 Func_080b5070(s32 id);
void Func_080b5088(s32 member, s32 kind);
struct Member **Func_080b5098(s32 member);
void Func_080b50e8(s32 id);
void Func_080c9048(void);
void Func_080cdd14(void);   /* BattleFx_SetTransitionFlagAndDisplay */
void Func_080cdd58(void);
void Func_080e0524(s32 id, void *target, s32 flag_a, s32 flag_b); /* load_and_decompress */
void Func_080e38b8(struct Spark *spark, s32 a, s32 b); /* advance_with_gravity_3d */
void Func_080e3958(s32 value, s32 *out);             /* apply_animation_and_y_offset */
void Func_080e396c(s32 value, s32 *out);             /* apply_step_and_y_offset */
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);

void BattlePres_RunBeamScene(void *object)
{
    void **heap;
    void **cursor;
    u8 *work;
    void *draw_target;
    void *sprite_src;
    void *xfer;
    void *record;
    struct Member *member;
    struct Spark *spark;
    BlitFn blit[2];
    s32 pos[3];
    s32 anchor[3];
    s32 tmp[3];
    s32 kind;
    s32 dx;
    s32 half;
    s32 size;
    s32 timer;
    s32 idx;
    s32 slot;
    s32 i;
    s32 j;

    heap = &Data_03001e50[39];
    cursor = heap;
    work = (u8 *)*cursor++;
    draw_target = *cursor;
    sprite_src = heap[2];
    xfer = heap[-27];
    kind = M2C_FIELD(object, s32 *, 0);
    STATE = object;
    record = Func_08077008(M2C_FIELD(object, s32 *, 8));
    Func_080030f8(1);
    Func_080c9048();
    Func_080cdd58();
    REG_BG1CNT = 0x1F80;
    Func_080030f8(1);

    /* Register the two rectangle blits for the beam body. */
    if (kind == 5) {
        if (M2C_FIELD(STATE, s32 *, 4) == 0) {
            Func_080ed408(46, 7, 7, 11, 3);
            Func_080ed408(47, 7, 7, 11, 2);
        } else {
            Func_080ed408(46, 7, 7, 15, 3);
            Func_080ed408(47, 7, 7, 15, 2);
        }
    } else {
        if (M2C_FIELD(STATE, s32 *, 4) == 0) {
            Func_080ed408(46, 7, 7, 3, 3);
            Func_080ed408(47, 7, 7, 3, 2);
        } else {
            Func_080ed408(46, 7, 7, 7, 3);
            Func_080ed408(47, 7, 7, 7, 2);
        }
    }
    blit[0] = (BlitFn)Data_03001e50[46];
    blit[1] = (BlitFn)Data_03001e50[47];
    Func_080030f8(1);

    /* The beam graphic itself, one resource per scene selector. */
    if (kind == 4) {
        Func_080e0524((s32)&Value_0000006b, work, 1, 1);
    } else if (kind == 3) {
        Func_080e0524((s32)&Value_000000c5, work, 0, 0);
    } else {
        switch (kind) {
        case 0:
        case 1:
        case 5:
            Func_080e0524((s32)&Value_000000b5, work, 1, 1);
            break;
        case 2:
            Func_080e0524((s32)&Value_000000b6, work, 1, 1);
            break;
        }
    }

    if (M2C_FIELD(STATE, s32 *, 8) > 7) {
        IWRAM_COPY((void *)0x05000000,
            Func_08002f40((s32)&Value_0000008e), 0x80);
    } else {
        IWRAM_COPY((void *)0x05000000,
            Func_08002f40((s32)&Value_0000004a), 0x80);
    }
    Func_080030f8(1);
    Func_080e0524((s32)&Value_00000076, sprite_src, 0, 0);
    Func_080e0524((s32)&Value_00000099, (void *)0x02010000, 1, 0);
    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 50;
    Func_080041d8(0x080CD261, 0x480);
    REG_BG1CNT = 0x1F81;

    Func_080e3958(M2C_FIELD(STATE, s16 *, 0x24), anchor);
    if (M2C_FIELD(STATE, s32 *, 4) == 0) {
        dx = 96 - anchor[0];
    } else {
        dx = 32 - anchor[0];
    }
    if (dx > 0) {
        dx = 0;
    }
    if (dx < -128) {
        dx = -128;
    }
    anchor[0] = anchor[0] + dx;
    Data_03001ad0.field_0004 = (s16)dx;
    Data_03001ad0.field_0006 = 80;
    Func_080030f8(1);

    /* Seed the sparks from the actor's world position. */
    member = *Func_080b5098(M2C_FIELD(STATE, s16 *, 0x24));
    half = Func_080b5070(M2C_FIELD(STATE, s16 *, 0x24)) / 2;
    spark = SPARKS;
    i = 0;
    do {
        spark->x = member->x;
        spark->y = member->y + half;
        spark->z = member->z;
        spark->vx = (s32)(Func_08004458() & 0xFF) << 10;
        spark->vy = ((s32)(Func_08004458() & 0xFF) - 32) << 10;
        spark->vz = ((s32)(Func_08004458() & 0xFF) - 127) << 10;
        if (spark->x > 0) {
            spark->vx = -spark->vx;
        }
        spark->vx = -spark->vx;
        spark->timer = i + 16;
        i++;
        spark++;
    } while (i != 64);

    i = 0;
    do {
        if (i == 5) {
            if (Func_080b5058(M2C_FIELD(record, u8 *, 296)) != 0) {
                Func_080b50e8(134);
            } else {
                Func_080b50e8(133);
            }
        }
        if (i == 4) {
            Func_080b5088(M2C_FIELD(STATE, s16 *, 0x24), 0);
        }

        Func_080e396c(M2C_FIELD(STATE, s32 *, 8), pos);
        pos[1] = pos[1] + 16;

        /* The beam body, one rectangle per scene family. */
        if (kind == 4) {
            if (i <= 11) {
                if (M2C_FIELD(STATE, s32 *, 4) == 0) {
                    blit[1](draw_target, work + (5 - i / 2) * 768,
                        pos[0] + dx - 48, pos[1] - 8, 48, 16);
                } else {
                    blit[1](draw_target, work + (5 - i / 2) * 768,
                        pos[0] + dx, pos[1] - 8, 48, 16);
                }
            }
        } else if ((kind >= 0 && kind <= 2) || kind == 5) {
            if (i <= 11) {
                if (M2C_FIELD(STATE, s32 *, 4) == 0) {
                    blit[1](draw_target, work + (i / 2) * 3456,
                        pos[0] + dx - 48, pos[1] - 40, 48, 72);
                } else {
                    blit[1](draw_target, work + (i / 2) * 3456,
                        pos[0] + dx, pos[1] - 40, 48, 72);
                }
            }
        } else {
            if (i <= 17) {
                idx = Func_080022ec(i, 3);
                if (M2C_FIELD(STATE, s32 *, 4) == 0) {
                    blit[1](draw_target, work + Data_080eedbe[idx],
                        pos[0] + Data_080eedca[idx] + dx - 58,
                        pos[1] - Data_080eedb8[idx] / 2,
                        Data_080eedb2[idx], Data_080eedb8[idx]);
                } else {
                    blit[1](draw_target, work + Data_080eedbe[idx],
                        pos[0] - Data_080eedca[idx] + dx
                            - Data_080eedb2[idx] + 58,
                        pos[1] - Data_080eedb8[idx] / 2,
                        Data_080eedb2[idx], Data_080eedb8[idx]);
                }
            }
        }

        /* The impact sequence decompressed at 0x02010000. */
        if (i >= 4 && i <= 15) {
            blit[0](draw_target,
                (u8 *)0x02010000 + ((i - 4) / 2) * 1920,
                anchor[0] - 16, anchor[1] - 24, 40, 48);
        }

        Func_080049ac();
        Func_080051d8(xfer, (u8 *)xfer + 12);

        /* Step and draw the live sparks; each one is visited twice. */
        if (i >= 4 && i <= 31) {
            j = 0;
            do {
                slot = j / 2;
                spark = &SPARKS[slot];
                timer = spark->timer;
                if (timer > 0) {
                    Func_08005268(spark, tmp);
                    size = (timer >> 3) + 2;
                    tmp[0] = tmp[0] + dx;
                    blit[slot & 1](draw_target,
                        (u8 *)sprite_src + Data_080ede5c[size - 1],
                        tmp[0] - size, tmp[1] - size, size * 2, size * 2);
                    Func_080e38b8(spark, 60, -0x400);
                    spark->timer = spark->timer - 1;
                }
                j++;
            } while (j != 64);
        }

        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
        i++;
    } while (i != 32);

    Func_08004278(0x080CD261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Data_03001ad0.field_0006 = (s16)i;
    Func_080cdd14();
}

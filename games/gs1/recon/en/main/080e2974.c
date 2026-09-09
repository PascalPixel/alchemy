#include "types.h"

#define BattlePresentation_RunBurstScene Func_080e2974

/*
 * Battle-presentation burst scene at 0x080e2974.
 *
 * Two arguments: the effect state pointer, which this owner parks at
 * work + 0x7828 exactly like every other member of the 0x03001eec
 * "battle work" family (see games/gs1/recon/en/main/080e302c.c and
 * games/gs1/src/battle/effects/puff_arc/run.c for the shared prologue,
 * the heap-cache cursor pair and the callee evidence reused here), and a
 * scene selector that indexes the seven-byte-per-entry table at 0x080eed3e.
 *
 * Setup: publish the state, program BLDALPHA, optionally run the
 * kind-7 presentation helper Func_080de2f8 when the state's s32 at 0x1C
 * is 1, decompress resource 0x73 over heap_cache[41] and resource 0x99
 * into the work block, then repack 288 forty-byte rows of the freshly
 * decompressed image into 288 twenty-byte rows at work + 0x5100 (each
 * destination byte is written twice, by the even and then the odd source
 * byte of the pair; the reference is unambiguous about that).  The scene
 * record then selects resource 0xB5 or 0xB6 over the work block, pulls
 * resource 0x6B into 0x02015E00, and copies one of four palette
 * resources to 0x05000000 through the IWRAM copy routine at 0x03001388.
 *
 * After one waited frame the actor's screen anchor is resolved once,
 * the 0x7780 / 0x7784 pacing words are set, the 0x080CD261 scheduler
 * callback is installed, and 768 twenty-eight byte sparks are seeded at
 * 0x02010000 from the actor's world position with random velocities and
 * a dormant timer of -1.
 *
 * The frame loop runs table[scene * 7 + 5] frames.  Each frame re-resolves the
 * state's own screen position, halves its x, registers the two blit
 * routines 0x2E and 0x2F with a shade that depends on the state's s32 at
 * 4, and keeps their entry points.  It then walks table[+2] wave
 * slots: wave i is drawn for six frames starting at i * table[+4],
 * from the decompressed 0x99 image at the head of the work block
 * (frame d at work + d * 3456, so the six frames fill work[0 .. 0x5100))
 * when (i & 3) <= 1 or table[+0] is 1, and from the 0x02015E00 image
 * otherwise; on the wave's third frame it
 * optionally floods the draw target with 0x2F2F2F2F, pokes the actor,
 * cues audio and wakes table[+3] of that wave's sparks; and for the
 * twelve frames after that it draws the twenty-by-forty-eight cell
 * sequence built at work + 0x5100.  The frame then re-registers the two
 * blits for the spark pass, advances and draws the first 512 sparks
 * under gravity, halving and inverting vy once y <= 0x7FFFF, applies the camera
 * shake, ticks the object group and waits one frame.
 *
 * Neither `Func_080072f0` nor `Func_080072f4` is a real function symbol:
 * they are entries 3 and 4 of the `_call_via_rN` trampoline bundle at
 * games/gs1/asm/080072e4.s, i.e. indirect calls through whatever pointer
 * the compiler kept in that register.  Here they are the IWRAM copy
 * routine at 0x03001388, the IWRAM fill routine at 0x03000168, and the
 * two blit routines Func_080ed408 publishes into heap_cache[46] and
 * heap_cache[47], so they are spelled as calls through typed pointers.
 *
 * The resource ids 0x6B, 0x73, 0x8D, 0x99, 0xA0, 0xB4, 0xB5 and 0xB6
 * reach their calls through the literal pool rather than `movs`, which
 * the project's external `Value_xxxxxxxx` spelling reproduces; the
 * original source form of those ids is not recovered.
 *
 * Still uncertain: the roles of the table entry's first two bytes
 * beyond the resource and wave-image choices they make here; why the
 * wave draw mixes the fixed actor anchor's x with the per-frame
 * position's y in its second form, which the reference is explicit
 * about; the meaning of the 0x7780 / 0x7784 / 0x77A8 / 0x7824 words; and
 * why the sparks are seeded 768 deep but only the first 512 are ever
 * stepped and drawn.
 *
 * Residual: this draft emits the owner's full 1720 bytes with equal
 * block topology and every reference branch, loop, call and store
 * accounted for, but roughly 690 halfwords still differ.  Two classes
 * remain and neither is expressible from ordinary C:
 *
 *   1. Spill-slot numbering.  Both sides allocate exactly twenty
 *      four-byte spill slots at sp+8..sp+84 and the same 52 bytes of
 *      named locals at sp+88..sp+139, but reload assigns the twenty
 *      values to different slots -- the reference parks the frame
 *      counter at sp+72, the scene index times eight at sp+60 and the
 *      wave count at sp+52, where this draft lands on sp+60, sp+56 and
 *      sp+64.  Every sp-relative access therefore reads as a
 *      difference even where the instruction is otherwise identical.
 *
 *   2. Register naming.  The same permutation runs through the whole
 *      body (the seed loop's r5/r6 pair, the wave blit's r0/r1 pair,
 *      the spark step's r4/r5 width pair), so operand-identical
 *      instructions encode differently.
 *
 * One smaller shape difference survives in the repack loop: the
 * reference keeps 0x5100 and the trip count 288 in ip and lr and adds
 * 0x5100 into the byte index, while this draft keeps the trip count in
 * ip and uses 0x5100 as the store's index register, costing about four
 * instructions in that loop.  Spelling the source read as work[i * 40 + j]
 * instead of a walking pointer was tried and made the whole function
 * worse (1728 bytes, 781 differing halfwords), so it was reverted.
 */

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

/* The state pointer this owner parks in, and reloads from, work + 0x7828. */
#define STATE (*(void **)(work + 0x7828))

typedef void (*BlitFn)(
    void *dest, const void *src, s32 x, s32 y, s32 w, s32 h);
typedef void (*CopyFn)(void *dest, const void *src, s32 count);
typedef void (*FillFn)(void *dest, s32 count, s32 value);

/* Heap-allocation cache: Data_03001e50[kind] holds kind's block address.
   This owner reads kinds 12, 39 (its work block), 40 (the draw target),
   41 (the sprite source) and the two blit entries 46 and 47.  The family
   spells the 39.. window through its own base, which is what the
   reference's single 0x03001eec pool word is. */
extern void *Data_03001e50[];

/* Sixteen halfword cell offsets shared with the rest of the family. */
extern u16 Data_080ede48[];

/* Seven bytes per scene selector, read as a flat table because the ARM
   structure-size boundary would round a seven-member struct up to eight
   and the reference's index arithmetic is unambiguously scene * 7:
     +0 wave-image and resource choice
     +1 palette resource selector
     +2 wave count
     +3 sparks woken per wave
     +4 frames between wave starts
     +5 total frames
     +6 flood-the-target flag */
extern u8 Data_080eed3e[];

/* Value_ symbols carry a literal the reference loads from its pool rather
   than materializing with a mov. */
extern u8 Value_0000006b;
extern u8 Value_00000073;
extern u8 Value_0000008d;
extern u8 Value_00000099;
extern u8 Value_000000a0;
extern u8 Value_000000b4;
extern u8 Value_000000b5;
extern u8 Value_000000b6;

/* 0x1C-byte spark records at a fixed EWRAM address; 768 are seeded and
   the first 512 are stepped and drawn. */
#define SPARKS ((struct Spark *)0x02010000)

struct Spark {
    s32 x;
    s32 y;
    s32 z;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 timer;
};

/* Only the world position this owner copies out of the member context. */
struct Member {
    s32 field_0000;
    s32 field_0004;
    s32 x;
    s32 y;
    s32 z;
};

/* Declared with the address spelling the link uses; the trailing comment is
   the name `alchemy inspect` resolves for that target, where it has one. */
void Func_080cd594(s32 mode);
void Func_080de2f8(void *object, s32 a, s32 b, s32 c, s32 *out_a, s32 *out_b);
void Func_080e0524(s32 id, void *target, s32 flag_a, s32 flag_b); /* load_and_decompress */
void *Func_08002f40(s32 id);                                      /* get */
void Func_080030f8(s32 frames);
void Func_080e396c(s32 source, s32 *out);        /* apply_step_and_y_offset */
void Func_080041d8(s32 callback, s32 interval);  /* Scheduler_AddOrUpdateCallback */
struct Member **Func_080b5098(s32 member);
u32 Func_08004458(void);                         /* random_16 */
void Func_080b5078(s32 a, s32 member, s32 c, s32 d);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080d6888(s32 member, s32 a, s32 b, s32 index, s32 e); /* update_members */
void Func_080b5088(s32 member, s32 kind);
void Func_080b50e8(s32 id);
void Func_080f9010(s32 id);
void Func_08002dd8(s32 id);                      /* Runtime_ReleaseHeapBlock */
void Func_080049ac(void);                        /* Render_ResetTransformState */
void Func_080051d8(void *a, void *b);            /* Graphics_PrepareTransferInIwramWork */
void Func_080e3944(const void *source, s32 *out); /* apply_base_and_y_offset */
void Func_080e38b8(void *particle, s32 a, s32 b); /* advance_with_gravity_3d */
void Func_080e155c(s32 x, s32 y);                /* Camera_ApplyShake */
void Func_080cd52c(void);                        /* ObjectGroup_TickMemberTimers */
void Func_08004278(s32 callback);                /* Scheduler_RemoveCallback */
void Func_080cdbc0(void);

void BattlePresentation_RunBurstScene(void *object, s32 scene)
{
    void **heap;
    void **cursor;
    u8 *work;
    void *draw_target;
    void *sprite_src;
    void *xfer;
    const u8 *src;
    struct Member *member;
    struct Spark *spark;
    BlitFn blit[2];
    s32 pos[3];
    s32 anchor[3];
    s32 tmp[3];
    s32 out_a;
    s32 out_b;
    s32 res;
    s32 cnt;
    s32 step;
    s32 size;
    s32 timer;
    s32 fp;
    s32 i;
    s32 j;
    s32 d;

    heap = &Data_03001e50[39];
    cursor = heap;
    work = (u8 *)*cursor++;
    draw_target = *cursor;
    sprite_src = heap[2];
    xfer = heap[-27];
    STATE = object;
    Func_080cd594(0);
    *(u16 *)0x04000052 = 0x1010;
    if (M2C_FIELD(STATE, s32 *, 0x1C) == 1) {
        Func_080de2f8(
            object, 7, M2C_FIELD(STATE, s32 *, 4), 2, &out_a, &out_b);
    }
    Func_080e0524((s32)&Value_00000073, sprite_src, 0, 0);
    Func_080e0524((s32)&Value_00000099, work, 1, 0);

    /* 288 rows of forty source bytes folded onto twenty destination
       bytes each, at work + 0x5100. */
    i = 0;
    do {
        src = &work[i * 40];
        j = 0;
        do {
            work[j / 2 + i * 20 + 0x5100] = *src++;
            j++;
        } while (j != 40);
        i++;
    } while (i != 288);

    if (Data_080eed3e[scene * 7] == 0) {
        Func_080e0524((s32)&Value_000000b5, work, 1, 1);
    } else {
        Func_080e0524((s32)&Value_000000b6, work, 1, 1);
    }
    Func_080e0524((s32)&Value_0000006b, (void *)0x02015E00, 1, 0);

    switch (Data_080eed3e[scene * 7 + 1]) {
    case 0:
        res = (s32)&Value_0000008d;
        break;
    case 1:
        res = (s32)&Value_000000a0;
        break;
    case 2:
        res = (s32)&Value_000000b6;
        break;
    default:
        res = (s32)&Value_000000b4;
        break;
    }
    ((CopyFn)0x03001388)((void *)0x05000000, Func_08002f40(res), 0x80);
    Func_080030f8(1);

    Func_080e396c(M2C_FIELD(STATE, s16 *, 0x24), anchor);
    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    Func_080041d8(0x080CD261, 0x480);

    /* Seed the sparks from the actor's world position. */
    member = *Func_080b5098(M2C_FIELD(STATE, s16 *, 0x24));
    spark = SPARKS;
    i = 0;
    do {
        spark->x = member->x;
        spark->y = member->y + 0x190000;
        spark->z = member->z;
        spark->vx = (s32)(Func_08004458() & 0xFF) << 12;
        spark->vy = ((s32)(Func_08004458() & 0xFF) - 127) << 12;
        spark->vz = ((s32)(Func_08004458() & 0xFF) - 127) << 12;
        if (spark->x > 0) {
            spark->vx = -spark->vx;
        }
        spark->timer = -1;
        i++;
        spark++;
    } while (i != 768);

    Func_080b5078(
        M2C_FIELD(STATE, s32 *, 8), M2C_FIELD(STATE, s16 *, 0x24), 4, 0);

    fp = 0;
    if (Data_080eed3e[scene * 7 + 5] != 0) {
        do {
            cnt = Data_080eed3e[scene * 7 + 2];
            Func_080e396c(M2C_FIELD(STATE, s32 *, 8), pos);
            pos[0] = pos[0] / 2;
            if (M2C_FIELD(STATE, s32 *, 4) == 0) {
                Func_080ed408(46, 7, 7, 3, 2);
                Func_080ed408(47, 7, 7, 11, 2);
            } else {
                Func_080ed408(46, 7, 7, 7, 2);
                Func_080ed408(47, 7, 7, 15, 2);
            }
            blit[0] = (BlitFn)Data_03001e50[46];
            blit[1] = (BlitFn)Data_03001e50[47];

            i = 0;
            while (i != cnt) {
                step = i * Data_080eed3e[scene * 7 + 4];

                /* Six frames of the wave image itself. */
                if (fp >= step && fp < step + 6) {
                    d = fp - step;
                    if ((i & 3) <= 1 || Data_080eed3e[scene * 7] == 1) {
                        if (M2C_FIELD(STATE, s32 *, 4) == 0) {
                            blit[i & 1](draw_target, work + (d * 3456),
                                (anchor[0] / 2) - 16, anchor[1] - 40, 48, 72);
                        } else {
                            blit[i & 1](draw_target, work + (d * 3456),
                                (anchor[0] / 2) - 32, anchor[1] - 40, 48, 72);
                        }
                    } else {
                        if (M2C_FIELD(STATE, s32 *, 4) == 0) {
                            blit[i & 1](draw_target,
                                (u8 *)0x02015E00 + (d * 768),
                                (anchor[0] / 2) - 16, pos[1] - 8, 48, 16);
                        } else {
                            blit[i & 1](draw_target,
                                (u8 *)0x02015E00 + (d * 768),
                                (anchor[0] / 2) - 32, pos[1] - 8, 48, 16);
                        }
                    }
                }

                /* The wave's third frame is its impact. */
                if (fp == step + 2) {
                    if (Data_080eed3e[scene * 7 + 6] == 1) {
                        ((FillFn)0x03000168)(draw_target, 0x4000, 0x2F2F2F2F);
                    }
                    Func_080d6888(M2C_FIELD(STATE, s16 *, 0x24), 7, 5, 0, 4);
                    if (i == cnt - 1) {
                        Func_080b5088(M2C_FIELD(STATE, s16 *, 0x24), 4);
                        M2C_FIELD(work, s32 *, 0x77A8) = 8;
                        Func_080b50e8(134);
                    } else {
                        if (i & 1) {
                            Func_080b5088(M2C_FIELD(STATE, s16 *, 0x24), 7);
                        }
                        M2C_FIELD(work, s32 *, 0x77A8) = 4;
                        Func_080f9010(134);
                    }
                    j = 0;
                    if (Data_080eed3e[scene * 7 + 3] != 0) {
                        do {
                            SPARKS[i * 32 + j].timer =
                                (s32)(Func_08004458() & 7) + 15;
                            j++;
                        } while (j != Data_080eed3e[scene * 7 + 3]);
                    }
                }

                /* Twelve frames of the twenty-by-forty-eight sequence. */
                if (fp >= step + 2 && fp < step + 14) {
                    d = (fp - step - 2) / 2;
                    blit[0](draw_target, work + (0x5100 + (d * 960)),
                        (anchor[0] / 2) - 10, anchor[1] - 24, 20, 48);
                }
                i++;
            }

            Func_08002dd8(47);
            Func_08002dd8(46);
            Func_080049ac();
            Func_080051d8(xfer, (u8 *)xfer + 12);
            Func_080ed408(46, 7, 7, 3, 3);
            Func_080ed408(47, 7, 7, 3, 2);
            blit[0] = (BlitFn)Data_03001e50[46];
            blit[1] = (BlitFn)Data_03001e50[47];

            /* Advance and draw the live sparks. */
            spark = SPARKS;
            i = 0;
            do {
                timer = spark->timer;
                if (timer > 0) {
                    Func_080e3944(spark, tmp);
                    size = (timer >> 3) + 1;
                    tmp[0] = tmp[0] / 2;
                    blit[(i / 2) & 1](draw_target,
                        (u8 *)sprite_src
                            + *(u16 *)((u8 *)Data_080ede48
                                + ((size * 2) - 2)),
                        tmp[0] - (size / 2), tmp[1] - size, size, size * 2);
                    Func_080e38b8(spark, 60, -0x400);
                    if (spark->y <= 0x7FFFF) {
                        spark->vy = (-spark->vy) / 2;
                    }
                    spark->timer = spark->timer - 1;
                }
                i++;
                spark++;
            } while (i != 512);

            Func_08002dd8(47);
            Func_08002dd8(46);
            Func_080e155c(8, 8);
            Func_080cd52c();
            M2C_FIELD(work, s32 *, 0x7824) = 1;
            Func_080030f8(1);
            fp++;
        } while (fp != Data_080eed3e[scene * 7 + 5]);
    }

    Func_08004278(0x080CD261);
    Func_080cdbc0();
}

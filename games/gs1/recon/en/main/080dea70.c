#include "types.h"

#define BattleEffect_RunProjectileVolley Func_080dea70

/*
 * Battle-presentation sub-effect at 0x080dea70 (3656 bytes, one function,
 * no split).
 *
 * Confirmed member of the 0x03001eec "battle work" subsystem family already
 * recovered at games/gs1/recon/en/main/080d85d0.c, 080e0c84.c, 080e01e4.c,
 * 080ddde0.c and 080d82b0.c: identical heap_cache=(void**)0x03001EEC /
 * cursor / work / draw_destination prologue, the same
 * M2C_FIELD(work, ..., 0x7828) = object republish, the same
 * Func_080cd594() / Func_080041d8(0x080CD261, 0x480) /
 * Func_08004278(0x080CD261) / Func_08002dd8(46) / Func_080cdbc0() bracket,
 * and the same Data_03001e50[46] / Data_03001e50[47] blit-callback slots
 * (0x03001f08 / 0x03001f0c) that Func_080ed408(id, ...) installs and
 * Func_08002dd8(id) releases.
 *
 * Behaviour: a `kind`-parameterised projectile volley.  One projectile
 * record per (member, shot) pair lives in the work buffer at
 * work+0x7080, stride 28 (x, y, z, vx, vy, vz, hit-flag).  Per kind the
 * launch height and the vertical aim are randomised differently, the
 * per-frame sprite differs, and the impact spawns a burst into the shared
 * 0x02010000 spark pool (stride 28: x.16, y.16, frame, vx, vy, -, life).
 * Kinds 0/5/8 additionally run a 12-frame two-pass impact flare over the
 * first STATE->count*shots spark slots.
 *
 * Every `Func_080072f0` / `Func_080072f4` / `Func_080072f8` /
 * `Func_080072fc` / `Func_08007300` call site is an indirect call through
 * the value the reference loads into r3/r4/r5/r6/r7 immediately before the
 * `bl`, not a real callee: those five addresses are consecutive slots of
 * the `_call_via_rN` trampoline bank at games/gs1/asm/080072e4.s
 * (r3 at +0x0c, r4 at +0x10, r5 at +0x14, r6 at +0x18, r7 at +0x1c).
 * They are modelled here as ordinary calls through `draw_cb` /
 * `flash_cb` / the IWRAM word copier at 0x03001388, which is the same
 * treatment 080d85d0.c and 080e0c84.c already use.
 *
 * The two `bl` instructions at 0x080defe0 and 0x080df860 target addresses
 * inside this owner (0x080df864 and 0x080defea).  They are GAS's Thumb-1
 * long-branch encoding, not calls -- 0x080defe0 is `if (total == 0) goto
 * tail;` and 0x080df860 is the frame loop's back edge.  The reference
 * epilogue confirms it: it pops the saved lr into r0 and does `bx r0`
 * rather than popping straight into pc, because lr no longer holds the
 * return address.
 *
 * Every Func_080e0524 resource id is loaded from the literal pool rather
 * than built with a `movs` immediate even where the value would fit one,
 * so they are spelled `(s32)&Value_XXXXXXXX` per the family precedent.
 *
 * Uncertain / not established here:
 *   - the roles of Data_080eebec's five bytes per kind (shot count, aim
 *     divisor, per-shot stagger, per-member stagger, burst size) are read
 *     off their use sites only; the table itself was not decoded.
 *   - Data_080eec28 / 2f / 36 / 3d (per-half-frame byte geometry) and
 *     Data_080eec44 (per-half-frame u16 tile offset) likewise.
 *   - the halfword store of 0 to 0x04000050 (BLDCNT) is emitted from a
 *     pool word in the reference, which in this family means a
 *     `&Value_00000000` spelling; it is written as a plain 0 here because
 *     the readable form is worth one halfword.
 *
 * Measured state (alchemy score --owner 080dea70): candidate 3648 bytes vs
 * reference 3656, 1440 differing halfwords, 983 wrong instructions,
 * 1706 candidate instructions vs 1711 reference.  Every reference call
 * target is present at the reference multiplicity; the two apparent
 * exceptions are both allocation artefacts, not missing work:
 *   - three of the sixteen indirect blits pick a different `_call_via_rN`
 *     slot (candidate r6 x3 / r7 x1, reference r5 x2 / r6 x1 / r7 x1);
 *     the total indirect-call count is identical.
 *   - the reference tail-merges the variant-0 and variant-1
 *     Func_080e0524 sprite loads into one `bl`, so it has eight where the
 *     candidate has nine.
 * The dominant residual is whole-function register allocation, not
 * structure: the reference parks `kind` in r8 and the aim divisor in sl,
 * this candidate does the reverse, and the reference frame is 156 bytes
 * against the candidate's 140 because it spills four more CSE temporaries
 * (notably the &work[0x7828] address and `kind * 4`).  Reordering the
 * local declarations to reproduce the reference slot order was tried and
 * moved nothing (1443 vs 1440), and inlining the two masked
 * Func_08004458() spreads into their enclosing expressions made it worse
 * (1581), so both were reverted.  Forcing either allocation from source
 * would be contrivance; this stays an honest draft.
 */

#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((s8 *)(expr) + (offset)))
#define STATE M2C_FIELD(work, void **, 0x7828)

typedef void (*WordCopyFn)(void *dest, void *src, s32 size);
typedef void (*DrawRectangleFn)(
    void *dest, void *src, s32 x, s32 y, s32 width, s32 height);

void Func_080cd594(s32 mode);
void Func_080de2f8(void *object, s32 a, s32 b, s32 c, s32 *out_x, s32 *out_y);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080e0524(s32 resource_id, void *target, s32 flag_a, s32 flag_b);
void *Func_08002f40(s32 id);
void Func_080072f0(void *dest, void *src, s32 size, WordCopyFn copier);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);
void Func_080b50e8(s32 id);
void Func_080f9010(s32 id);
void **Func_080b5098(s32 member_id);
s32 Func_080b5070(s32 member_id);
u32 Func_08004458(void);
s32 Func_080022ec(s32 numerator, s32 denominator);
s32 Func_080022fc(s32 value, s32 divisor);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_080049ac(void);
void Func_080051d8(s32 a, s32 b);
void Func_080e3944(void *source, s32 *out_pair);
void Func_080e3908(void *particle, s32 a, s32 b);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080b5088(s32 member_id, s32 flag);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);

extern u8 Value_0000005b;
extern u8 Value_0000005c;
extern u8 Value_0000005d;
extern u8 Value_00000068;
extern u8 Value_00000073;
extern u8 Value_0000007f;
extern u8 Value_00000080;
extern u8 Value_00000081;
extern u8 Value_00000092;
extern u8 Value_000000b8;
extern u8 Value_000000b9;
extern u8 Value_000000c7;

extern void *Data_03001e50[];
extern u8 Data_080eebec[];  /* [kind][5]: shots, aim divisor, shot stagger,
                             * member stagger, burst size */
extern u8 Data_080eebe9[];  /* per-variant impact flare height */
extern u8 Data_080eec28[];  /* per-half-frame flare width  */
extern u8 Data_080eec2f[];  /* per-half-frame flare height */
extern u8 Data_080eec36[];  /* per-half-frame flare span   */
extern u8 Data_080eec3d[];  /* per-half-frame flare offset */
extern u16 Data_080eec44[]; /* per-half-frame flare tile offset */
extern u16 Data_080eec52[]; /* per-shot spark tile offset, 4 entries */
extern u16 Data_080ede48[]; /* shared spark-size tile table */

s32 Func_080dea70(void *object, s32 kind)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    void *extra_target;
    void *launcher;
    DrawRectangleFn draw_cb;
    DrawRectangleFn flash_cb;
    s32 facing;
    s32 variant;
    s32 cooldown;
    s32 anchor_x;
    s32 anchor_y;
    s32 shots;
    s32 aim_divisor;
    s32 shot_stagger;
    s32 member_stagger;
    s32 launch_half;
    s32 member;
    s32 shot;
    s32 member_id_offset;
    s32 total;
    s32 frame;
    s32 pos[2];

    heap_cache = (void **) 0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    facing = *(s32 *)((s8 *)heap_cache - 108);
    extra_target = heap_cache[2];
    cooldown = 0;
    variant = M2C_FIELD(object, s32 *, 24);
    STATE = object;

    if (kind == 10) {
        Func_080cd594(0);
    } else {
        Func_080cd594(1);
    }

    if (M2C_FIELD(STATE, s32 *, 28) == 1) {
        Func_080de2f8(object, 1, M2C_FIELD(STATE, s32 *, 4), 2,
            &anchor_x, &anchor_y);
    }

    if (kind == 5) {
        if (M2C_FIELD(STATE, s32 *, 4) == 1) {
            Func_080ed408(46, 7, 7, 7, 2);
        } else {
            Func_080ed408(46, 7, 7, 3, 2);
        }
    } else {
        if (M2C_FIELD(STATE, s32 *, 4) == 1) {
            Func_080ed408(46, 7, 7, 7, 3);
        } else {
            Func_080ed408(46, 7, 7, 3, 3);
        }
    }
    draw_cb = (DrawRectangleFn) Data_03001e50[46];

    Func_080e0524((s32) &Value_00000073, extra_target, 0, 0);

    if (kind == 0 || kind == 5 || kind == 8) {
        if (kind == 5) {
            variant = 2;
        }
        if (kind == 8) {
            variant = 0;
        }
        if (variant == 0) {
            Func_080e0524((s32) &Value_0000007f, (s8 *)work + (128 << 5), 1, 1);
        } else if (variant == 1) {
            Func_080e0524((s32) &Value_00000080, (s8 *)work + (128 << 5), 1, 1);
        } else {
            Func_080e0524((s32) &Value_00000081, (s8 *)work + (128 << 5), 1, 1);
        }
        if (kind == 5) {
            Func_080072f0((void *)(160 << 19),
                Func_08002f40((s32) &Value_000000b9), 128,
                (WordCopyFn) 0x03001388);
        }
        Func_080e0524((s32) &Value_000000c7, (s8 *)work + (128 << 6), 1, 0);
        if (kind == 5) {
            Func_080072f0((void *)(160 << 19),
                Func_08002f40((s32) &Value_000000b9), 128,
                (WordCopyFn) 0x03001388);
        }
        M2C_FIELD(work, s32 *, 0x7780) = 2;
        M2C_FIELD(work, s32 *, 0x7784) = 75;
    } else if (kind == 1) {
        Func_080e0524((s32) &Value_0000005d, work, 1, 1);
        *(u16 *) 0x04000050 = 0;
        M2C_FIELD(work, s32 *, 0x7780) = kind;
        M2C_FIELD(work, s32 *, 0x7784) = 0;
    } else if (kind == 2) {
        Func_080072f0((void *)(160 << 19),
            Func_08002f40((s32) &Value_0000007f), 128,
            (WordCopyFn) 0x03001388);
        Func_080e0524((s32) &Value_0000005c, work, 0, 0);
        M2C_FIELD(work, s32 *, 0x7780) = kind;
        M2C_FIELD(work, s32 *, 0x7784) = 50;
    } else {
        if (kind == 3 || kind == 4 || kind == 11) {
            Func_080e0524((s32) &Value_0000005b, work, 1, 1);
        } else if (kind == 6) {
            Func_080e0524((s32) &Value_00000068, work, 1, 1);
        } else {
            Func_080e0524((s32) &Value_000000b8, work, 1, 1);
            Func_080e0524((s32) &Value_00000092, (s8 *)work + 0x65C0, 1, 0);
        }
        M2C_FIELD(work, s32 *, 0x7780) = 2;
        M2C_FIELD(work, s32 *, 0x7784) = 50;
    }

    Func_080041d8((void *) 0x080CD261, 144 << 3);

    launcher = *Func_080b5098(M2C_FIELD(STATE, s32 *, 8));
    shots = Data_080eebec[kind * 5];
    aim_divisor = Data_080eebec[kind * 5 + 1];
    shot_stagger = Data_080eebec[kind * 5 + 2];
    member_stagger = Data_080eebec[kind * 5 + 3];
    launch_half = Func_080b5070(M2C_FIELD(STATE, s32 *, 8));
    if (M2C_FIELD(STATE, s32 *, 20) * shots > 63) {
        M2C_FIELD(STATE, s32 *, 20) = 1;
    }

    /* ---- lay out one projectile per (member, shot) ---- */
    member = 0;
    if (M2C_FIELD(STATE, s32 *, 20) != 0) {
        s32 shot_base;
        s32 id_offset;

        id_offset = 36;
        shot_base = 0;
        do {
            void *target;
            s32 target_half;
            s32 half;
            s32 *projectile;

            target = *Func_080b5098(M2C_FIELD(STATE, s16 *, id_offset));
            target_half =
                Func_080b5070(M2C_FIELD(STATE, s16 *, id_offset));
            shot = 0;
            if (shots != 0) {
                half = launch_half / 2;
                projectile = (s32 *)((s8 *)work + 0x7080 + shot_base * 28);
                do {
                    projectile[0] = M2C_FIELD(launcher, s32 *, 8);
                    if (kind == 7) {
                        projectile[1] = M2C_FIELD(launcher, s32 *, 12)
                            + ((s32)(Func_08004458() & 15) << 16)
                            + (232 << 14);
                    } else if (kind == 10) {
                        projectile[1] =
                            M2C_FIELD(launcher, s32 *, 12) + half;
                    } else if (kind == 6) {
                        projectile[1] =
                            M2C_FIELD(launcher, s32 *, 12) + half;
                    } else if (kind == 9) {
                        projectile[1] = M2C_FIELD(launcher, s32 *, 12) + half
                            + ((16 - (s32)(Func_08004458() & 31)) << 16);
                    } else if (kind == 3 || kind == 4) {
                        projectile[1] = M2C_FIELD(launcher, s32 *, 12) + half
                            + ((16 - (s32)(Func_08004458() & 31)) << 16);
                    } else if (kind == 11) {
                        projectile[1] = M2C_FIELD(launcher, s32 *, 12) + half
                            + ((32 - (s32)(Func_08004458() & 63)) << 16);
                    } else if (kind == 5) {
                        projectile[1] =
                            M2C_FIELD(launcher, s32 *, 12) + half;
                    } else {
                        projectile[1] =
                            M2C_FIELD(launcher, s32 *, 12) + launch_half;
                    }
                    projectile[2] = M2C_FIELD(launcher, s32 *, 16);
                    projectile[3] = Func_080022ec(
                        M2C_FIELD(target, s32 *, 8) - projectile[0],
                        aim_divisor);

                    if (kind == 7) {
                        projectile[4] = Func_080022ec(
                            (M2C_FIELD(target, s32 *, 12)
                                + ((s32)(Func_08004458() & 63) << 16))
                                - projectile[1] - (12 << 16),
                            aim_divisor);
                    } else if (kind == 8) {
                        projectile[4] = Func_080022ec(
                            (M2C_FIELD(target, s32 *, 12)
                                + ((s32)(Func_08004458() & 7) << 16))
                                - projectile[1] + (176 << 13),
                            aim_divisor);
                    } else if (kind == 9) {
                        projectile[4] = Func_080022ec(
                            (M2C_FIELD(target, s32 *, 12)
                                + ((64 - (s32)(Func_08004458() & 63)) << 16))
                                - projectile[1],
                            aim_divisor);
                    } else if (kind == 10) {
                        projectile[4] = Func_080022ec(
                            (M2C_FIELD(target, s32 *, 12)
                                + ((s32)(Func_08004458() & 31) << 16))
                                - projectile[1] + (128 << 11),
                            aim_divisor);
                    } else if (kind == 3 || kind == 4 || kind == 11
                            || kind == 5) {
                        projectile[4] = 0;
                    } else if (kind == 6) {
                        s32 spread;

                        spread = (s32)(Func_08004458() & 15);
                        projectile[4] = Func_080022ec(
                            (M2C_FIELD(target, s32 *, 12) + target_half / 2
                                + (spread << 16))
                                - projectile[1],
                            aim_divisor);
                    } else {
                        s32 spread;

                        spread = (s32)(Func_08004458() & 15);
                        projectile[4] = Func_080022ec(
                            (M2C_FIELD(target, s32 *, 12) + target_half
                                - (spread << 16))
                                - projectile[1],
                            aim_divisor);
                    }

                    projectile[5] = Func_080022ec(
                        M2C_FIELD(target, s32 *, 16) - projectile[2],
                        aim_divisor);
                    projectile[6] = 0;

                    shot++;
                    projectile = (s32 *)((s8 *)projectile + 28);
                } while (shot != shots);
            }

            shot_base += shots;
            id_offset += 2;
            member++;
        } while (member != M2C_FIELD(STATE, s32 *, 20));
    }

    /* ---- clear the shared spark pool ---- */
    {
        s32 *spark;

        spark = (s32 *) 0x02010018;
        member = 0;
        do {
            *spark = 0;
            member++;
            spark = (s32 *)((s8 *)spark + 28);
        } while (member != 1024);
    }

    if (kind == 6) {
        total = M2C_FIELD(STATE, s32 *, 20) * member_stagger
            + shot_stagger * shots + 32;
    } else {
        total = M2C_FIELD(STATE, s32 *, 20) * member_stagger
            + shot_stagger * shots + 16;
    }

    frame = 0;
    if (total != 0) {
        do {
            s32 pool_index;
            s32 member_gate;
            s32 *spark;

            if (cooldown > 0) {
                cooldown--;
            }

            if (kind == 6) {
                if (frame == 4) {
                    Func_080f9010(136);
                }
                if (frame == 32) {
                    Func_080b50e8(134);
                }
            } else if (kind == 7) {
                if (frame == 48) {
                    Func_080b50e8(133);
                }
            } else if (kind != 5) {
                if (frame == 16) {
                    Func_080b50e8(133);
                }
            }

            /* the caster's own muzzle sprite, on an orbiting arc */
            if (M2C_FIELD(STATE, s32 *, 28) == 1) {
                s32 angle;
                s32 muzzle_x;
                s32 muzzle_y;

                angle = frame << 11;
                muzzle_x = ((-Func_08002322(angle) << 2) >> 16)
                    + anchor_x / 2 - 10;
                muzzle_y = ((Func_0800231c(angle) << 1) >> 16) + anchor_y - 24;
                if (frame > 69) {
                    muzzle_y = muzzle_y - (frame << 1) + 138;
                }

                if (M2C_FIELD(STATE, s32 *, 4) == 1) {
                    Func_080ed408(47, 7, 7, 7, 3);
                } else {
                    Func_080ed408(47, 7, 7, 3, 3);
                }
                flash_cb = (DrawRectangleFn) Data_03001e50[47];
                if (frame <= 3) {
                    flash_cb(draw_destination, (s8 *)work + 0x65C0,
                        muzzle_x, muzzle_y, 20, 40);
                }
                flash_cb(draw_destination, (s8 *)work + 0x65C0,
                    muzzle_x, muzzle_y, 20, 40);
                Func_08002dd8(47);
            }

            Func_080049ac();
            Func_080051d8(facing, facing + 12);

            /* ---- advance and draw every live projectile ---- */
            pool_index = 0;
            member = 0;
            if (M2C_FIELD(STATE, s32 *, 20) != 0) {
                member_id_offset = 36;
                member_gate = 0;
                do {
                    void *target;
                    s32 gate;
                    s32 tick;
                    s32 *impact;

                    target = *Func_080b5098(
                        M2C_FIELD(STATE, s16 *, member_id_offset));
                    shot = 0;
                    if (shots != 0) {
                        impact = (s32 *)((s8 *) 0x02010000 + pool_index * 28);
                        gate = member_gate;
                        tick = 0;
                        do {
                            s32 *projectile;

                            if (tick + gate < frame) {
                                projectile = (s32 *)((s8 *)work + 0x7080
                                    + (shots * member + shot) * 28);
                                Func_080e3944(projectile, pos);
                                pos[0] = pos[0] >> 1;
                                projectile[0] += projectile[3];
                                projectile[1] += projectile[4];
                                projectile[2] += projectile[5];

                                /* kind 6 leaves a continuous smoke trail */
                                if (kind == 6) {
                                    s32 *trail;
                                    s32 slot;
                                    s32 made;

                                    trail = (s32 *) 0x02013800;
                                    slot = 512;
                                    made = 0;
                                    do {
                                        if (trail[6] == 0) {
                                            trail[0] = pos[0] << 16;
                                            trail[1] = pos[1] << 16;
                                            trail[3] = ((s32)(Func_08004458()
                                                & 255) - 128) << 11;
                                            trail[4] = ((s32)(Func_08004458()
                                                & 255) - 128) << 11;
                                            trail[6] = (s32)(Func_08004458()
                                                & 7) + 32;
                                            made++;
                                            if (made == 2) {
                                                break;
                                            }
                                        }
                                        slot++;
                                        trail = (s32 *)((s8 *)trail + 28);
                                    } while (slot != 1024);
                                }

                                /* impact test: the projectile has crossed
                                 * the target's own x on the side it started */
                                if (projectile[6] == 0) {
                                    s32 target_x;

                                    target_x = M2C_FIELD(target, s32 *, 8);
                                    if (target_x < 0
                                            ? projectile[0] < 0
                                            : projectile[0] >= 0) {
                                        s32 travelled;
                                        s32 reach;

                                        travelled = projectile[0];
                                        if (travelled < 0) {
                                            travelled = -travelled;
                                        }
                                        reach = target_x;
                                        if (reach < 0) {
                                            reach = -reach;
                                        }
                                        if (travelled >= reach) {
                                            s32 made;
                                            s32 shift;
                                            s32 slot;
                                            s32 *spark2;

                                            made = 0;
                                            projectile[6] = 1;

                                            if (kind == 5) {
                                                Func_080b50e8(134);
                                            } else if (kind != 6) {
                                                if (cooldown == 0) {
                                                    cooldown = 8;
                                                    Func_080f9010(132);
                                                }
                                            }

                                            if (kind == 2) {
                                                s32 kick;

                                                kick = ((s32)(Func_08004458()
                                                    & 31) + 32) << 12;
                                                if ((Func_08004458() & 1)
                                                        != 0) {
                                                    projectile[4] += kick;
                                                } else {
                                                    projectile[4] -= kick;
                                                }
                                                projectile[3] =
                                                    -projectile[3];
                                            }

                                            impact[6] = 1;
                                            impact[0] = pos[0];
                                            impact[1] = pos[1];
                                            impact[2] = 0;

                                            if (kind != 7) {
                                                M2C_FIELD(work, s32 *,
                                                    0x77A8) = 2;
                                            }
                                            Func_080d6888(
                                                M2C_FIELD(STATE, s16 *,
                                                    member_id_offset),
                                                7, 5, member, 8);
                                            if (kind == 7 || kind == 9
                                                    || kind == 10) {
                                                /* no reaction pose */
                                            } else if (kind == 5) {
                                                Func_080b5088(
                                                    M2C_FIELD(STATE, s16 *,
                                                        member_id_offset), 4);
                                            } else {
                                                Func_080b5088(
                                                    M2C_FIELD(STATE, s16 *,
                                                        member_id_offset), 5);
                                            }

                                            shift = kind == 5 ? 12 : 11;
                                            spark2 = (s32 *) 0x02010AF0;
                                            slot = 100;
                                            do {
                                                if (spark2[6] == 0) {
                                                    spark2[0] = pos[0] << 16;
                                                    spark2[1] = pos[1] << 16;
                                                    spark2[3] =
                                                        ((s32)(Func_08004458()
                                                        & 255) - 128) << shift;
                                                    spark2[4] =
                                                        ((s32)(Func_08004458()
                                                        & 255) - 128) << shift;
                                                    spark2[6] =
                                                        (s32)(Func_08004458()
                                                        & 7) + 16;
                                                    made++;
                                                    if (made
                                                        == Data_080eebec[
                                                            kind * 5 + 4]) {
                                                        break;
                                                    }
                                                }
                                                slot++;
                                                spark2 = (s32 *)
                                                    ((s8 *)spark2 + 28);
                                            } while (slot != 512);
                                        }
                                    }
                                }

                                /* ---- per-kind projectile sprite ---- */
                                if (kind == 0 || kind == 5 || kind == 8) {
                                    if (M2C_FIELD(STATE, s32 *, 4) == 1) {
                                        Func_080ed408(47, 7, 7, 7, 2);
                                    } else {
                                        Func_080ed408(47, 7, 7, 3, 2);
                                    }
                                    ((DrawRectangleFn) Data_03001e50[47])(
                                        draw_destination,
                                        (s8 *)work + (128 << 5),
                                        pos[0] - 16,
                                        pos[1] - Data_080eebe9[variant],
                                        32, Data_080eebe9[variant]);
                                    Func_08002dd8(47);

                                    if (M2C_FIELD(STATE, s32 *, 4) == 1) {
                                        Func_080ed408(47, 7, 7, 15, 2);
                                    } else {
                                        Func_080ed408(47, 7, 7, 11, 2);
                                    }
                                    ((DrawRectangleFn) Data_03001e50[47])(
                                        draw_destination,
                                        (s8 *)work + (128 << 5),
                                        pos[0] - 16, pos[1],
                                        32, Data_080eebe9[variant]);
                                    Func_08002dd8(47);
                                } else if (kind == 1) {
                                    s32 travelled;
                                    s32 reach;

                                    travelled = projectile[0];
                                    if (travelled < 0) {
                                        travelled = -travelled;
                                    }
                                    reach = M2C_FIELD(target, s32 *, 8);
                                    if (reach < 0) {
                                        reach = -reach;
                                    }
                                    if (travelled <= reach) {
                                        s32 cell;

                                        cell = Func_080022fc(frame, 6);
                                        draw_cb(draw_destination,
                                            (s8 *)work + cell * 768,
                                            pos[0] - 16, pos[1] - 12, 32, 24);
                                    }
                                } else if (kind == 7 || kind == 9
                                        || kind == 10) {
                                    draw_cb(draw_destination,
                                        (s8 *)work
                                            + Data_080eec52[shot & 3],
                                        pos[0] - 4, pos[1] - 4, 8, 8);
                                } else if (kind == 2) {
                                    s32 cell;

                                    cell = Func_080022fc(shot, 6);
                                    draw_cb(draw_destination,
                                        (s8 *)work + (cell << 7),
                                        pos[0] - 4, pos[1] - 8, 8, 16);
                                } else if (kind == 3) {
                                    draw_cb(draw_destination,
                                        (s8 *)work + 96,
                                        pos[0] - 9, pos[1] - 7, 18, 13);
                                } else if (kind == 4) {
                                    draw_cb(draw_destination, work,
                                        pos[0] - 6, pos[1] - 4, 12, 8);
                                } else if (kind == 11) {
                                    draw_cb(draw_destination,
                                        (s8 *)work + (165 << 1),
                                        pos[0] - 15, pos[1] - 12, 29, 23);
                                } else {
                                    draw_cb(draw_destination, work,
                                        pos[0] - 20, pos[1] - 32, 40, 64);
                                }
                            }

                            tick += shot_stagger;
                            shot++;
                            impact = (s32 *)((s8 *)impact + 28);
                            pool_index++;
                        } while (shot != shots);
                    }

                    member_id_offset += 2;
                    member_gate += member_stagger;
                    member++;
                } while (member != M2C_FIELD(STATE, s32 *, 20));
            }

            /* ---- 12-frame two-pass impact flare (kinds 0/5/8) ---- */
            if (kind == 0 || kind == 5 || kind == 8) {
                member = 0;
                if (M2C_FIELD(STATE, s32 *, 20) * shots != 0) {
                    s32 *impact;

                    impact = (s32 *) 0x02010000;
                    do {
                        if (impact[6] == 1) {
                            s32 half;

                            half = impact[2] / 2;
                            if ((member & 1) == 0) {
                                Func_080ed408(47, 7, 7, 3, 2);
                                ((DrawRectangleFn) Data_03001e50[47])(
                                    draw_destination,
                                    (s8 *)work + Data_080eec44[half],
                                    impact[0] - Data_080eec28[half],
                                    impact[1] - Data_080eec3d[half],
                                    Data_080eec28[half],
                                    Data_080eec2f[half]);
                                Func_08002dd8(47);

                                Func_080ed408(47, 7, 7, 15, 2);
                                ((DrawRectangleFn) Data_03001e50[47])(
                                    draw_destination,
                                    (s8 *)work + Data_080eec44[half],
                                    impact[0],
                                    impact[1] + Data_080eec36[half]
                                        - Data_080eec3d[half],
                                    Data_080eec28[half],
                                    Data_080eec2f[half]);
                                Func_08002dd8(47);
                            } else {
                                Func_080ed408(47, 7, 7, 3, 2);
                                ((DrawRectangleFn) Data_03001e50[47])(
                                    draw_destination,
                                    (s8 *)work + Data_080eec44[half] + 0x128A,
                                    impact[0] - Data_080eec3d[half],
                                    impact[1] - Data_080eec28[half],
                                    Data_080eec36[half],
                                    Data_080eec28[half]);
                                Func_08002dd8(47);

                                Func_080ed408(47, 7, 7, 15, 2);
                                ((DrawRectangleFn) Data_03001e50[47])(
                                    draw_destination,
                                    (s8 *)work + Data_080eec44[half] + 0x128A,
                                    impact[0] + Data_080eec36[half]
                                        - Data_080eec3d[half],
                                    impact[1],
                                    Data_080eec2f[half],
                                    Data_080eec28[half]);
                                Func_08002dd8(47);
                            }

                            impact[2]++;
                            if (impact[2] == 12) {
                                impact[6] = 0;
                            }
                        }
                        member++;
                        impact = (s32 *)((s8 *)impact + 28);
                    } while (member != M2C_FIELD(STATE, s32 *, 20) * shots);
                }
            }

            /* ---- burst sparks, then the kind-6 smoke trail ---- */
            member = 100;
            spark = (s32 *) 0x02010AF0;
            do {
                s32 life;

                life = spark[6];
                if (life > 0) {
                    s32 size;

                    size = (life >> 3) + 1;
                    draw_cb(draw_destination,
                        (s8 *)extra_target + Data_080ede48[size - 1],
                        M2C_FIELD(spark, s16 *, 2) - size / 2,
                        M2C_FIELD(spark, s16 *, 6) - size,
                        size, size << 1);
                    Func_080e3908(spark, 60, 128 << 5);
                    if (spark[1] > (224 << 15)) {
                        spark[4] = (-spark[4]) / 2;
                    }
                    spark[6]--;
                }
                member++;
                spark = (s32 *)((s8 *)spark + 28);
            } while (member != 512);

            spark = (s32 *) 0x02013800;
            do {
                s32 life;

                life = spark[6];
                if (life > 0) {
                    s32 size;

                    size = (life >> 4) + 1;
                    draw_cb(draw_destination,
                        (s8 *)extra_target + Data_080ede48[size - 1],
                        M2C_FIELD(spark, s16 *, 2) - size / 2,
                        M2C_FIELD(spark, s16 *, 6) - size,
                        size, size << 1);
                    Func_080e3908(spark, 60, -16384);
                    spark[6]--;
                }
                member++;
                spark = (s32 *)((s8 *)spark + 28);
            } while (member != 1024);

            Func_080e155c(4, 4);
            Func_080cd52c();
            M2C_FIELD(work, s32 *, 0x7824) = 1;
            Func_080030f8(1);

            frame++;
        } while (frame != total);
    }

    Func_08004278((void *) 0x080CD261);
    Func_08002dd8(46);
    return Func_080cdbc0();
}

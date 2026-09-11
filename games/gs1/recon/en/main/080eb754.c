#include "shared-aggregates.h"

/*
 * Battle-presentation scene at 0x080eb754.
 *
 * The routine takes over the frame scheduler for the length of one canned
 * battle presentation. It runs two timed loops over the 0x03001eec battle work
 * block: a 0x78-frame introduction that sweeps nine attached objects across the
 * screen, grows a ring of sprites and animates a 64-entry spark pool at
 * work+0x7080, and a 0x60-frame main phase that animates the 320-entry pool at
 * 0x02010000, emits a 32-record burst for each living member as the rising row
 * passes it, and finally releases the objects, the scheduler callback and the
 * scene heap block.
 *
 * Uncertain: the exact meaning of most work-block fields (0x7780/0x7784 look
 * like a mode/timer pair, 0x77a8 like a shake counter, 0x7824 like a redraw
 * request and 0x7828 like the current target-list object); the roles of the
 * two nine-entry ROM displacement tables at 0x080eef56 / 0x080eef5f; and the
 * meaning of the 0x080ede48 sprite-size table. Names are reading aids.
 *
 * Call bindings used here, from the owner's own resolved call sites:
 *   Func_080041d8 Scheduler_AddOrUpdateCallback   Func_08004278 Scheduler_RemoveCallback
 *   Func_08004458 random_16                       Func_08002dd8 Runtime_ReleaseHeapBlock
 *   Func_080dbb24 BattleFx_SpawnObjects       Func_080d6750 BattleFx_SelectLivingTargets
 *   Func_080e0524 load_and_decompress             Func_080e3980 apply_alternate_step_and_y_offset
 *   Func_080d6888 update_members                  Func_080e155c Camera_ApplyShake
 *   Func_080cd52c ObjectGroup_TickMemberTimers
 *
 * Indirect calls: the cached word at absolute_03001e50.field_00b8 is a six
 * argument rectangle blitter. Every retained call site branches through the
 * `bx rN` veneer bank at 0x080072e4 (0x080072f4 is the r4 slot, 0x08007300 the
 * r7 slot), so this is one function pointer used five times, not two ROM
 * functions. See games/gs1/recon/en/main/080ed104.c for the same idiom.
 *
 * Pooled small constants: the reference loads 0, 7, 0x3a, 0x73, 0x80, 0x95,
 * 0xca and 0xf0 from its literal pool at sites where an ordinary integer
 * literal would have compiled to `mov rd, #imm8`. They are modelled with the
 * project's Value_<address> absolute-symbol idiom, matching 080ed104.c, so the
 * operands stay link-time values instead of inline immediates. Neighbouring
 * plain immediates (0x20, 0xf0 at 0x03001ce0+0x10) are left as literals
 * because the reference does emit `mov` for those.
 *
 * Stack layout recovered from the reference frame (176 bytes): outgoing
 * arguments 0x00-0x07, spill slots 0x08/0x0c, seventeen word locals
 * 0x10-0x50, the two eight-byte position pairs at 0x54 and 0x5c, the two
 * sixteen-byte object placement records at 0x64 and 0xa0, the fourteen-entry
 * per-member flag array at 0x74, the sixteen-entry jitter array at 0x84 and
 * the twelve-byte member position scratch at 0x94. The declaration order
 * below reproduces GCC 2.96's slot assignment for that frame.
 *
 * Residual, measured and not claimed as exact: candidate 2440 of 2444 bytes,
 * topology equal, 1158 differing halfwords. The remaining size gap is literal
 * pool placement - the reference spills nine inline pools with their jump-over
 * branches and alignment fill, the candidate fewer - and the candidate frame is
 * 180 bytes because GCC needs one extra spill slot in the first loop, which
 * shifts every sp-relative offset by four. Both are allocation and pool
 * scheduling decisions, not missing statements: every reference branch, loop,
 * call and side effect is represented here.
 */

/* Only the m2c spelling this draft actually uses. */
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

#define SPARK_POOL 0x7080  /* 64 records inside the battle work block */
#define OBJECT_LIST 0x77D8 /* nine attached presentation objects */
#define SCENE_POOL ((struct ScenePoint *)0x02010000) /* 320 records */

struct EffectPos {
    s32 x;
    s32 y;
};

/* 0x1c-byte animation record; used by both pools in this scene. */
struct ScenePoint {
    s32 x;         /* 0x00 */
    s32 y;         /* 0x04, 16.16 in the scene pool */
    s32 field_08;  /* 0x08 */
    s32 vx;        /* 0x0c */
    s32 vy;        /* 0x10 */
    s32 field_14;  /* 0x14 */
    s32 life;      /* 0x18 */
};

typedef void (*BlitRectFn)(s32 dest, void *src, s32 x, s32 y, s32 w, s32 h);

/* Link-time operands the reference keeps in its literal pool. */
extern u8 Value_00000000;
extern u8 Value_00000007;
extern u8 Value_0000003a;
extern u8 Value_00000073;
extern u8 Value_00000080;
extern u8 Value_00000095;
extern u8 Value_000000ca;
extern u8 Value_000000f0;

void Func_080eb754(s32 arg0) {
    u8 place[0x10];  /* object placement record, first loop */
    u8 mpos[0x0C];  /* member position scratch */
    u8 jit_buf[0x10];  /* per-column jitter, 16 entries */
    u8 hit_buf[14];    /* per-member "already emitted" flags */
    u8 place2[0x10];  /* object placement record, second loop */
    struct EffectPos pos1;
    struct EffectPos pos2;
    s32 dest;
    void *work;
    BlitRectFn blit;
    u32 sheet;
    s32 base_x;
    s32 grow;
    s32 saved_row;
    void *ctrl;
    s32 shift;
    s32 wave;
    s32 radius_x;
    s32 radius_y;
    s32 half;
    s32 row_y;
    u8 *hit;
    u8 *jitter;
    s32 slide;
    s32 pool_ofs;
    s32 frame;
    s32 cnt;
    s32 num;
    s32 amp;
    s32 ang;
    s32 lift;
    s32 lo;
    s32 hi;
    s32 tmp;
    s32 idx;
    s32 k;
    s32 m;
    s32 slot;
    s32 *objs;
    struct ScenePoint *sp;

    dest = M2C_FIELD(&absolute_03001ef0, s32 *, 0);
    work = *(void **)0x03001EEC;
    sheet = absolute_03001ef0.field_0004;
    M2C_FIELD(work, s32 *, 0x7828) = arg0;
    Func_080cd594(0);
    Func_080c9048();
    absolute_0400000c.field_0000 = 0x784;
    M2C_FIELD((void *)0x05000000, s16 *, 0) = (s16) (s32) &Value_00000000;
    M2C_FIELD((void *)0x05000000, s16 *, 2) = (s16) (s32) &Value_00000000;
    M2C_FIELD(work, s32 *, 0x7780) = 0;
    Func_080041d8(0x080CD261, 0x480);
    Func_080cd104(1, 0);
    Func_080dbb24(9, 0x175, 1);
    absolute_03001ce0.field_0010 = 0xF0;
    Func_080d6750(M2C_FIELD(work, s32 *, 0x7828));
    absolute_04000048.field_0000 = 0x2737;
    *(u16 *)0x04000038 = (u16) (s32) &Value_000000ca;
    Func_080030f8(1);
    Func_080b5040(1, (s32) &Value_0000003a, 0);
    Func_080cd104(1, 1);
    Func_080e0524((s32) &Value_00000073, sheet, 0, 0);
    Func_080e0524((s32) &Value_00000095, (u32) work, 1, 1);
    M2C_FIELD((void *)0x04000000, s16 *, 0) = 0x7741;
    M2C_FIELD((void *)0x04000000, s16 *, 0x20) = (s16) (s32) &Value_00000080;
    M2C_FIELD((void *)0x04000020, s16 *, 0x32) = 0x100E;
    M2C_FIELD((void *)0x04000020, s16 *, 0x30) = 0x3F44;
    base_x = 0;
    grow = 0;
    saved_row = (s32) absolute_03001ad0.field_0004;
    ctrl = M2C_FIELD(&absolute_03001ef0, void **, 0x10);
    shift = 0;
    M2C_FIELD(work, s32 *, 0x7780) = 1;
    M2C_FIELD(work, s32 *, 0x7784) = base_x;
    M2C_FIELD(ctrl, s32 *, 0x10) = 1;

    sp = (struct ScenePoint *)((s8 *)work + SPARK_POOL);
    for (k = 0; k != 0x40; k++) {
        sp[k].x = (s32) ((Func_08004458() & 0x1F) + 0x10);
        sp[k].y = (s32) (((Func_08004458() & 0x1F) + 0x30) << 0x10);
        sp[k].vy = (s32) (((Func_08004458() & 0x1F) - 0x10) << 0x10);
        sp[k].life = (s32) (Func_08002304(Func_08004458(), 0x30) + 2);
    }

    Func_080ed408(0x2E, 7, 7, 3, 3);
    blit = (BlitRectFn) absolute_03001e50.field_00b8;
    absolute_0400000c.field_0000 = 0x786;

    for (frame = 0; frame != 0x78; frame++) {
        wave = 0;
        if (frame == 0) {
            Func_080f9010(0x88);
        }
        if (frame == 0x1A) {
            Func_080f9010(0x8D);
        }
        if (frame == 0x28) {
            Func_080f9010(0x9A);
        }
        if (frame == 0x48) {
            Func_080f9010(0x9A);
        }
        if (frame == 0x68) {
            Func_080f9010(0x9A);
        }
        if ((*(s32 *)0x03001B04 & 3) != 0) {
            if (frame > 0x10) {
                break;
            }
        }
        if ((u32) (frame - 0x18) <= 0x1FU) {
            shift += 1;
        }
        if (shift > 0x18) {
            shift = 0x18;
        }
        if (frame <= 0x87) {
            absolute_03001ad0.field_0004 -= shift;
            grow += shift;
        }
        if (frame <= 0x95) {
            pos1 = *(const struct EffectPos *)0x080EDAD8;
            lift = 0;
            if (frame > 0x67) {
                lift = (frame * 0x10) + 0xFFFFF980;
            }
            if ((u32) (frame - 8) <= 0x17U) {
                base_x = (base_x + shift) - 8;
            }
            if (frame > 7) {
                amp = 0x60;
                if (frame <= 0x68) {
                    amp = 0x20;
                }
                ang = ((frame << 0xA) + 0xFFFFE000) & 0xFFFF;
                if (ang > 0x8000) {
                    ang += 0xFFFF8000;
                }
                wave = (s32) (amp * Func_08002322(ang)) >> 0x10;
                if ((0x1F & frame) == 8) {
                    M2C_FIELD(work, s32 *, 0x77A8) = 4;
                }
            }
            M2C_FIELD(&place, s32 *, 0xC) = 0;
            M2C_FIELD(&place, s32 *, 4) = 0xFF0000;
            objs = (s32 *)((s8 *)work + OBJECT_LIST);
            for (k = 0; k != 9; k++) {
                M2C_FIELD(&place, s32 *, 0) =
                    (s32) ((((base_x + *(u8 *)(0x080EEF56 + k)) - lift) << 0x10) + 0xE00000);
                M2C_FIELD(&place, s32 *, 8) =
                    (s32) (((*(u8 *)(0x080EEF5F + k) - wave) << 0x10) + 0x480000);
                Func_08009008(*objs++, &place, &pos1, 0);
            }
        }
        if (frame <= 0x1A) {
            cnt = grow + 4;
            num = frame * 8;
            if (cnt > 0xA) {
                cnt = 0xA;
            }
            if (num > 0x40) {
                num = 0x40;
            }
            if (num != 0) {
                radius_x = grow * 2;
                radius_y = (grow * 0xC) + 0x30;
                idx = cnt * 2;
                half = (s32) (cnt + ((u32) cnt >> 0x1F)) >> 1;
                for (k = 0; k != num; k++) {
                    ang = k << 0xA;
                    lo = ((s32) ((radius_x + 8) * Func_08002322(ang)) >> 0x10) + grow;
                    hi = ((s32) (radius_y * Func_0800231c(ang)) >> 0x10) + 0x40;
                    blit(dest,
                         (void *) (sheet + *(u16 *)(0x080EDE48 + (idx - 2))),
                         (lo + 0x60) - half, hi - cnt, cnt, idx);
                }
            }
        }
        if (frame == 0x18) {
            M2C_FIELD(work, s32 *, 0x7780) = 2;
            M2C_FIELD(work, s32 *, 0x7784) = 0x32;
        }
        if (frame == 0x1C) {
            absolute_0400000c.field_0000 = 0x784;
        }
        if (frame > 0x11) {
            sp = (struct ScenePoint *)((s8 *)work + SPARK_POOL);
            for (k = 0; k != 0x30; k++) {
                if (sp->life == 0) {
                    lo = Func_080022fc(k, 3) + 1;
                    hi = lo * 2;
                    blit(dest,
                         (void *) (sheet + *(u16 *)(0x080EDE48 + (hi - 2))),
                         sp->x, M2C_FIELD(sp, s16 *, 6) - lo, lo, hi);
                    tmp = sp->vy;
                    sp->x = (s32) (sp->x + 2);
                    sp->y = (s32) (sp->y + tmp);
                    tmp = tmp * 0x30;
                    if (tmp < 0) {
                        tmp += 0x3F;
                    }
                    sp->vy = (s32) (tmp >> 6);
                } else {
                    sp->life = (s32) (sp->life - 1);
                }
                if ((sp->x > 0x80) || (sp->life == 1)) {
                    sp->x = (s32) ((Func_08004458() & 0x1F) + base_x + 0xAC);
                    sp->y = (s32) ((((Func_08004458() & 0x1F) - wave) + 0x38) << 0x10);
                    sp->vy = (s32) (((Func_08004458() & 0x1F) - 0x10) << 0xF);
                }
                sp++;
            }
        }
        if (frame > 0x1F) {
            tmp = frame - 0x20;
            lift = (s32) (tmp + (tmp >> 0x1F)) >> 1;
            if (lift > 0x28) {
                lift = 0x28;
            }
            hi = 0;
            for (k = 0; k != 6; k++) {
                lo = (Func_08004458() & 3) * 3;
                blit(dest, (s8 *)work + (lo << 9), 0x78 - lift, hi, 0x30, 0x20);
                hi += 0x12;
            }
        }
        tmp = M2C_FIELD(work, s32 *, 0x77A8);
        if (tmp > 0) {
            M2C_FIELD(work, s32 *, 0x77A8) = (s32) (tmp - 1);
            absolute_03001ad0.field_0006 =
                (Func_08004458() & (s32) &Value_00000007) + 0x1C;
        } else {
            absolute_03001ad0.field_0006 = 0x20;
        }
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
    }

    absolute_03001ad0.field_0004 = saved_row;
    M2C_FIELD(ctrl, s32 *, 0x10) = 0;
    Func_080d67dc();
    absolute_04000040.field_0000 = (u16) (s32) &Value_000000f0;

    objs = (s32 *)((s8 *)work + OBJECT_LIST);
    for (k = 0; k != 9; k++) {
        void *obj = (void *) *objs++;
        M2C_FIELD(obj, u8 *, 9) = (u8) (M2C_FIELD(obj, u8 *, 9) | 0xC);
    }

    hit = hit_buf;
    row_y = 0xE0;
    for (k = 0; k != 14; k++) {
        hit[k] = 0;
    }
    jitter = jit_buf;
    for (k = 0; k != 0x10; k++) {
        jitter[k] = (u8) (Func_08004458() & 0x1F);
    }
    for (k = 0; k != 0x140; k++) {
        SCENE_POOL[k].life = 0;
    }
    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 0x4B;
    absolute_0400000c.field_0000 = 0x784;
    absolute_0400000c.field_0046 = 0x1010;
    slide = 0xFFFFFE20;

    for (frame = 0; frame != 0x60; frame++) {
        if (frame <= 0x17) {
            pos2 = *(const struct EffectPos *)0x080EDAE0;
            row_y -= 0x10;
            if (frame <= 8) {
                tmp = frame << 0xB;
                ang = tmp + 0x4000;
                if (ang > 0x8000) {
                    ang = tmp + 0xFFFFC000;
                }
                lo = Func_08002322(ang) << 6;
            } else {
                tmp = frame << 0xB;
                ang = tmp + 0x4000;
                if (ang > 0x8000) {
                    ang = tmp + 0xFFFFC000;
                }
                lo = Func_08002322(ang) << 5;
            }
            hi = lo >> 0x10;
            M2C_FIELD(&place2, s32 *, 0xC) = 0;
            M2C_FIELD(&place2, s32 *, 4) = 0xFF0000;
            objs = (s32 *)((s8 *)work + OBJECT_LIST);
            for (k = 0; k != 9; k++) {
                M2C_FIELD(&place2, s32 *, 0) =
                    (s32) ((row_y + *(u8 *)(0x080EEF56 + k)) << 0x10);
                M2C_FIELD(&place2, s32 *, 8) =
                    (s32) (((*(u8 *)(0x080EEF5F + k) - hi) << 0x10) + 0x480000);
                Func_08009008(*objs++, &place2, &pos2, 0);
            }
        }
        if (frame == 8) {
            M2C_FIELD(work, s32 *, 0x77A8) = frame;
            Func_080f9010(0x91);
        }
        if (frame == 0xB) {
            Func_080f9010(0x91);
        }
        if (frame == 0x2E) {
            Func_080f9010(0x89);
        }
        m = 0;
        if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x14) != 0) {
            pool_ofs = 0;
            slot = 0x24;
            do {
                if (hit[m] == 0) {
                    Func_080e3980(
                        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, slot),
                        &mpos);
                    if ((s32) M2C_FIELD(&mpos, s32 *, 0) > row_y) {
                        hit[m] = 1;
                        sp = (struct ScenePoint *)((s8 *)SCENE_POOL + pool_ofs);
                        for (k = 0; k != 0x20; k++) {
                            sp->x = (s32) (M2C_FIELD(&mpos, s32 *, 0) << 0xF);
                            sp->y = (s32) ((M2C_FIELD(&mpos, s32 *, 4) - 0x10) << 0x10);
                            sp->vx = (s32) ((Func_08004458() - 0x80) << 0xA);
                            tmp = Func_08004458() - 0xC0;
                            sp->vy = (s32) (tmp << 0xB);
                            sp->x = (s32) (sp->x + (sp->vx * 4));
                            sp->y = (s32) (sp->y + (tmp << 0xD));
                            sp->life = (s32) ((0xF & Func_08004458()) + 8);
                            sp++;
                        }
                        Func_080b5088(
                            M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, slot), 1);
                        Func_080f9010(0x86);
                    }
                }
                pool_ofs += 0x380;
                slot += 2;
                m += 1;
            } while (m != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x14));
        }
        sp = SCENE_POOL;
        for (k = 0; k != 0xC0; k++) {
            if (sp->life > 0) {
                blit(dest,
                     (void *) (sheet + M2C_FIELD((void *)0x080EDE48, u16 *, 4)),
                     M2C_FIELD(sp, s16 *, 2) - 1, M2C_FIELD(sp, s16 *, 6) - 3, 3, 6);
                sp->x = (s32) (sp->x + sp->vx);
                sp->y += sp->vy;
                sp->life -= 1;
            }
            sp++;
        }
        if (frame == 0x30) {
            Func_080f9010(0x88);
        }
        if (frame > 0x28) {
            M2C_FIELD(work, s32 *, 0x7780) = 0;
            M2C_FIELD(work, s32 *, 0x7784) = 0x4B;
            hi = -8;
            for (k = 0; k != 0x10; k++) {
                blit(dest, (s8 *)work + (((Func_08004458() & 3) * 3) << 9),
                     (jitter[k] - slide) + 0x78, hi, 0x30, 0x20);
                hi += 8;
            }
        }
        if (frame > 0x40) {
            M2C_FIELD(work, s32 *, 0x7780) = 2;
        }
        if (frame == 0x3A) {
            m = 0;
            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x14) != 0) {
                slot = 0x24;
                do {
                    Func_080d6888(
                        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, slot),
                        0xE, 5, -1, 0);
                    m += 1;
                    slot += 2;
                } while (m != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x14));
            }
        }
        Func_080e155c(8, 8);
        Func_080cd52c();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
        slide += 0xC;
    }

    Func_080b50e8(0x86);
    objs = (s32 *)((s8 *)work + OBJECT_LIST);
    for (k = 0; k != 9; k++) {
        Func_08009038(*objs++);
    }
    Func_08004278(0x080CD261);
    Func_08002dd8(0x2E);
    Func_080cdbc0();
}

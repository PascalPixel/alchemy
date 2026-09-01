#include "types.h"

#define BattleEffect_RunParticleReveal Func_080e0c84

/*
 * Battle-presentation sub-effect at 0x080e0c84.
 *
 * Confirmed member of the 0x03001eec "battle work" subsystem family
 * documented in games/gs1/src/battle/effects/member_orbit/run.c (owner
 * 080ce85c) and games/gs1/src/battle/effects/puff_arc/run.c (owner
 * 080d9fc8): same heap_cache=(void**)0x03001EEC / cursor / work / canvas
 * prologue, same M2C_FIELD(work,...,0x7828)=object republish, same
 * Func_080cd594(0)/Func_080041d8(0x080CD261,0x480)/Func_08004278(0x080CD261)/
 * Func_08002dd8(id)/Func_080cdbc0() bracket, and the same
 * Func_080cef64(flag, DrawRectangleFn callbacks[2]) two-word blit-routine
 * resolver already established in games/gs1/recon/en/main/080e01e4.c.
 *
 * Unlike member_orbit's single 64-frame per-member sprite loop, this owner
 * runs a 64-slot randomly-seeded particle pool (fixed-point x/y plus a
 * sin/cos velocity pair) alongside the 64-frame animation loop, and reads
 * every particle's fixed-point position back through the upper halfword of
 * its s32 field -- the same M2C_FIELD(ptr, s16*, 2)/M2C_FIELD(ptr, s16*, 6)
 * idiom already confirmed in 080e01e4.c.
 *
 * Every `Func_080072f4`/`Func_08007314` call site is an indirect call
 * through the value the reference loads into r4/r12 immediately before the
 * `bl`, not a real function -- both addresses fall inside the
 * `_call_via_rN` trampoline bank at games/gs1/asm/080072e4.s (r4 slot at
 * +0x10, ip/r12 slot at +0x30). All such call sites here go through
 * `routine[]`, a two-entry DrawRectangleFn array Func_080cef64 fills.
 *
 * All three Func_080e0524 id arguments are loaded from the reference's
 * literal pool rather than built with a `movs` immediate, matching the
 * already-adopted Value_ idiom (puff_arc/run.c's Value_000000b4,
 * 080e01e4.c's Value_00000073/00000090/00000089): `(s32)&Value_XXXXXXXX`
 * forces the same pool load even though the values (0x6e, 0xb8, 0x92) would
 * otherwise fit an 8-bit `movs` immediate.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*DrawRectangleFn)(
    void *dest, void *src, s32 x, s32 y, s32 width, s32 height);

typedef struct {
    s32 x;
    s32 y;
    s32 rot;
    s32 vx;
    s32 vy;
    s32 unk14;
    s32 unk18;
} Particle;

extern u8 Value_0000006e;
extern u8 Value_000000b8;
extern u8 Value_00000092;
extern u8 Data_080eec5f[];
extern u8 Data_080eec63[];
extern u16 Data_080eec68[];

void Func_080cd594(s32 mode);
void Func_080de2f8(void *object, s32 a, s32 b, s32 c, s32 *out_a, s32 *out_b);
void Func_080cef64(s32 flag, DrawRectangleFn *out_callbacks);
void Func_080e0524(s32 effect_id, void *target, s32 flag_a, s32 flag_b);
void Func_080041d8(void *callback, s32 interval);
void Func_080e3980(s16 a, s32 *out_pair);
u32 Func_08004458(void);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_080e38b8(void *particle, s32 a, s32 b);
void Func_080b50e8(s32 id);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080b5088(s32 member_id, s32 b);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_08004278(void *callback);
void Func_08002dd8(s32 id);
void Func_080cdbc0(void);

void BattleEffect_RunParticleReveal(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *canvas;
    s32 spawn[3];
    s32 screen_y;
    s32 screen_x;
    DrawRectangleFn routine[2];
    Particle *p;
    s32 i;
    s32 frame;
    s32 clamp;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    M2C_FIELD(work, void **, 0x7828) = object;
    Func_080cd594(0);
    Func_080de2f8(object, 1,
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4), 2,
        &screen_x, &screen_y);
    Func_080cef64(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4), routine);
    Func_080e0524((s32)&Value_0000006e, work, 1, 1);
    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    {
        s32 interval;
        void *callback;

        interval = 0x480;
        callback = (void *)0x080CD261;
        Func_080041d8(callback, interval);
    }
    Func_080e3980(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 0x24), spawn);

    for (i = 0, p = (Particle *)0x02010000; i != 64; i++, p++) {
        s32 angle;
        s32 amp;

        angle = (Func_08004458() & 0x7FFF) + 0x4000;
        amp = (Func_08004458() & 0x1FF) + 0x80;
        p->x = ((spawn[0] / 2 + (Func_08004458() & 0xF)) - 8) << 16;
        p->y = (spawn[1] + 8) << 16;
        p->vx = (Func_08002322(angle) * amp) >> 9;
        p->vy = (Func_0800231c(angle) * amp) >> 6;
        p->rot = Func_08004458() & 0x7F;
        p->unk14 = Func_08004458() & 0x7F;
        p->unk18 = (Func_08004458() & 0xF) + 32;
    }

    for (frame = 0; frame != 64; frame++) {
        if (frame > 47) {
            M2C_FIELD((void *)0x04000052, s16 *, 0) = (64 - frame) | 0x1000;
        }
        if (frame == 1) {
            Func_080e0524((s32)&Value_000000b8, (u8 *)work + 0x400, 1, 1);
            Func_080e0524((s32)&Value_00000092, (u8 *)work + 0x65C0, 1, 0);
        }

        if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0x1C) == 1) {
            s32 orbit_angle;
            s32 x;
            s32 y;

            orbit_angle = frame << 11;
            x = (((-Func_08002322(orbit_angle)) << 2) >> 16)
                + screen_x / 2 - 10;
            y = ((Func_0800231c(orbit_angle) << 1) >> 16) + screen_y - 22;
            if (frame > 0x45) {
                y = (y - frame * 2) + 0x8A;
            }
            routine[1](canvas, (u8 *)work + 0x65C0, x, y, 20, 40);
            if (frame <= 3) {
                routine[1](canvas, (u8 *)work + 0x65C0, x, y, 20, 40);
            }
        }

        for (i = 0, p = (Particle *)0x02010000; i != 64; i++, p++) {
            if (frame >= i / 4 + 4) {
                s32 index;
                s32 w;
                s32 h;

                index = (p->rot / 128) & 3;
                routine[i & 1](
                    canvas, (u8 *)work + 0x400 + Data_080eec68[index],
                    M2C_FIELD(p, s16 *, 2) - (w = Data_080eec5f[index]) / 2,
                    M2C_FIELD(p, s16 *, 6) - (h = Data_080eec63[index]) / 2,
                    w, h);
                Func_080e38b8(p, 0x3F, 0x1000);
            }
        }

        if (frame == 8) {
            M2C_FIELD(work, s32 *, 0x77A8) = frame;
            Func_080b50e8(0x86);
            Func_080d6888(
                M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 0x24),
                7, 5, 0, 16);
            Func_080b5088(
                M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 0x24), 3);
        }

        clamp = frame * 4;
        if (clamp > 32) {
            clamp = 32;
        }
        if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 4) == 0) {
            for (i = 0; i != 5; i++) {
                routine[0](canvas, work, (i << 5) - (frame / 4 & 31),
                    120 - clamp, 32, 32);
            }
        } else {
            for (i = 0; i != 5; i++) {
                routine[0](canvas, work, ((i << 5) + (frame / 4 & 31)) - 32,
                    120 - clamp, 32, 32);
            }
        }

        Func_080e155c(4, 8);
        Func_080cd52c();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((void *)0x080CD261);
    Func_08002dd8(0x2F);
    Func_08002dd8(0x2E);
    Func_080cdbc0();
}

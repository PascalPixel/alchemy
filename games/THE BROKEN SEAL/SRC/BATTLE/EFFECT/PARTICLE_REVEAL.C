#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"

/*
 * Battle-presentation sub-effect at 0x080e0c84.
 *
 * Confirmed member of the 0x03001eec "battle work" subsystem family
 * documented in games/THE BROKEN SEAL/SRC/BATTLE/EFFECT/MEMBER_ORBIT.C (owner
 * 080ce85c) and games/THE BROKEN SEAL/SRC/BATTLE/EFFECT/PUFF_ARC.C (owner
 * 080d9fc8): same heap_cache=(void**)0x03001EEC / cursor / work / canvas
 * prologue, same the +0x7828 field=object republish, same
 * BattleFx_BeginCanvasLayer(0)/Scheduler_AddOrUpdateCallback(0x080CD261,0x480)/Scheduler_RemoveCallback(0x080CD261)/
 * Runtime_ReleaseHeapBlock(id)/BattleFx_EndCanvasLayer() bracket, and the same
 * BattleFx_FetchRectangleBlitters(flag, DrawRectangleFn callbacks[2]) two-word blit-routine
 * resolver already established in recon/tbs/en/main/080e01e4.c.
 *
 * Unlike member_orbit's single 64-frame per-member sprite loop, this owner
 * runs a 64-slot randomly-seeded particle pool (fixed-point x/y plus a
 * sin/cos velocity pair) alongside the 64-frame animation loop, and reads
 * every particle's fixed-point position back through the upper halfword of
 * its s32 field -- the same +2 / +6 halfword
 * idiom already confirmed in 080e01e4.c.
 *
 * Every `Func_080072f4`/`Func_08007314` call site is an indirect call
 * through the value the reference loads into r4/r12 immediately before the
 * `bl`, not a real function -- both addresses fall inside the
 * container-built `_call_via_rN` bank at 0x080072e4 (r4 slot at
 * +0x10, ip/r12 slot at +0x30). All such call sites here go through
 * `routine[]`, a two-entry DrawRectangleFn array BattleFx_FetchRectangleBlitters fills.
 *
 * All three Resource_LoadAndDecompress id arguments are loaded from the reference's
 * literal pool rather than built with a `movs` immediate, matching the
 * already-adopted Value_ idiom (puff_arc/run.c's Value_000000b4,
 * 080e01e4.c's Value_00000073/00000090/00000089): `(s32)&Value_XXXXXXXX`
 * forces the same pool load even though the values (0x6e, 0xb8, 0x92) would
 * otherwise fit an 8-bit `movs` immediate.
 */

typedef struct {
    s32 x;
    s32 y;
    s32 rot;
    s32 vx;
    s32 vy;
    s32 unk14;
    s32 unk18;
} Particle;

/* Fixed EWRAM scratch block holding the effect's particle records. */
#define PARTICLE_POOL ((Particle *)0x02010000)
#define PARTICLE_COUNT 64

extern u8 Value_0000006e;
extern u8 Value_000000b8;
extern u8 Value_00000092;
extern u8 ParticleReveal_CellWidths[];
extern u8 ParticleReveal_CellHeights[];
extern u16 ParticleReveal_CellSourceOffsets[];

void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_PrepareCanvasEffect(void *object, s32 a, s32 b, s32 c, s32 *out_a, s32 *out_b);
void BattleFx_FetchRectangleBlitters(s32 flag, DrawRectangleFn *out_callbacks);
void EffectPosition_ApplyAlternateStepAndYOffset(s16 a, s32 *out_pair);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void EffectStep_AdvanceWithGravity3D(void *particle, s32 a, s32 b);
void BattleEventRuntime_BeginPhaseFar(s32 id);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void BattleMotion_ApplyVariantMotionFar(s32 member_id, s32 b);
void Camera_ApplyShake(s32 a, s32 b);
void ObjectGroup_TickMemberTimers(void);
void BattleFx_EndCanvasLayer(void);

void BattleFx_RunParticleReveal(void *object)
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
    (*(void **)((u8 *)(work) + (0x7828))) = object;
    BattleFx_BeginCanvasLayer(0);
    BattleFx_PrepareCanvasEffect(object, 1,
        (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4))), 2,
        &screen_x, &screen_y);
    BattleFx_FetchRectangleBlitters(
        (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4))), routine);
    Resource_LoadAndDecompress((s32)&Value_0000006e, work, 1, 1);
    (*(s32 *)((u8 *)(work) + (0x7780))) = 2;
    (*(s32 *)((u8 *)(work) + (0x7784))) = 75;
    {
        s32 interval;
        void *callback;

        interval = 0x480;
        callback = (void *)0x080CD261;
        Scheduler_AddOrUpdateCallback(callback, interval);
    }
    EffectPosition_ApplyAlternateStepAndYOffset(
        (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (0x24))), spawn);

    for (i = 0; i != PARTICLE_COUNT; i++) {
        s32 angle;
        s32 amp;

        angle = (Random16() & 0x7FFF) + 0x4000;
        amp = (Random16() & 0x1FF) + 0x80;
        PARTICLE_POOL[i].x =
            ((spawn[0] / 2 + (Random16() & 0xF)) - 8) << 16;
        PARTICLE_POOL[i].y = (spawn[1] + 8) << 16;
        PARTICLE_POOL[i].vx = (Trig_Sin(angle) * amp) >> 9;
        PARTICLE_POOL[i].vy = (Trig_Cos(angle) * amp) >> 6;
        PARTICLE_POOL[i].rot = Random16() & 0x7F;
        PARTICLE_POOL[i].unk14 = Random16() & 0x7F;
        PARTICLE_POOL[i].unk18 = (Random16() & 0xF) + 32;
    }

    for (frame = 0; frame != 64; frame++) {
        if (frame > 47) {
            (*(s16 *)((u8 *)((void *)0x04000052) + (0))) = (64 - frame) | 0x1000;
        }
        if (frame == 1) {
            Resource_LoadAndDecompress((s32)&Value_000000b8, (u8 *)work + 0x400, 1, 1);
            Resource_LoadAndDecompress((s32)&Value_00000092, (u8 *)work + 0x65C0, 1, 0);
        }

        if ((*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (0x1C))) == 1) {
            s32 orbit_angle;
            s32 x;
            s32 y;

            orbit_angle = frame << 11;
            x = (((-Trig_Sin(orbit_angle)) << 2) >> 16)
                + screen_x / 2 - 10;
            y = ((Trig_Cos(orbit_angle) << 1) >> 16) + screen_y - 22;
            if (frame > 0x45) {
                y = (y - frame * 2) + 0x8A;
            }
            routine[1](canvas, (u8 *)work + 0x65C0, x, y, 20, 40);
            if (frame <= 3) {
                routine[1](canvas, (u8 *)work + 0x65C0, x, y, 20, 40);
            }
        }

        for (i = 0, p = PARTICLE_POOL; i != PARTICLE_COUNT; i++, p++) {
            if (frame >= i / 4 + 4) {
                s32 index;
                s32 w;
                s32 h;

                index = (p->rot / 128) & 3;
                routine[i & 1](
                    canvas, (u8 *)work + 0x400 + ParticleReveal_CellSourceOffsets[index],
                    (*(s16 *)((u8 *)(p) + (2))) - (w = ParticleReveal_CellWidths[index]) / 2,
                    (*(s16 *)((u8 *)(p) + (6))) - (h = ParticleReveal_CellHeights[index]) / 2,
                    w, h);
                EffectStep_AdvanceWithGravity3D(p, 0x3F, 0x1000);
            }
        }

        if (frame == 8) {
            (*(s32 *)((u8 *)(work) + (0x77A8))) = frame;
            BattleEventRuntime_BeginPhaseFar(0x86);
            ObjectGroup_UpdateMembers(
                (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (0x24))),
                7, 5, 0, 16);
            BattleMotion_ApplyVariantMotionFar(
                (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (0x24))), 3);
        }

        clamp = frame * 4;
        if (clamp > 32) {
            clamp = 32;
        }
        if ((*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4))) == 0) {
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

        Camera_ApplyShake(4, 8);
        ObjectGroup_TickMemberTimers();
        (*(s32 *)((u8 *)(work) + (0x7824))) = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((void *)0x080CD261);
    Runtime_ReleaseHeapBlock(0x2F);
    Runtime_ReleaseHeapBlock(0x2E);
    BattleFx_EndCanvasLayer();
}

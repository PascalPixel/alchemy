#include "types.h"
#include "scene.h"
#include "resource.h"

/* battle/effects/run_particle_field_variant0.c */
/* battle/effects/particle_field/fx_run_particle_field_variant0.c */
/* battle/effects/particle_field/variant_0.c */
s32 BattleFx_RunParticleFieldVariant(s32, s32);

void BattleFx_RunParticleFieldVariant0(s32 effect)
{
    BattleFx_RunParticleFieldVariant(effect, 0);
}

/* battle/effects/particle_field/variant_1.c */
s32 BattleFx_RunParticleFieldVariant(s32, s32);

void BattleFx_RunParticleFieldVariant1(s32 effect)
{
    BattleFx_RunParticleFieldVariant(effect, 1);
}

/* battle/effects/particle_field/variant_2.c */
s32 BattleFx_RunParticleFieldVariant(s32, s32);

void BattleFx_RunParticleFieldVariant2(s32 effect)
{
    BattleFx_RunParticleFieldVariant(effect, 2);
}

/* battle/effects/particle_field/variant_3.c */
s32 BattleFx_RunParticleFieldVariant(s32, s32);

void BattleFx_RunParticleFieldVariant3(s32 effect)
{
    BattleFx_RunParticleFieldVariant(effect, 3);
}

/* graphics/tile/transpose_copy.c */
/* graphics/tile/transpose_copy.c */
void Graphics_TransposeCopy(s32 source_address, s32 destination_address, s32 row_size, s32 column_count)
{
    s32 column_index;
    s32 row_index;
    s32 destination_column;
    s32 source_offset;
    u8 *destination;
    u8 *source;

    column_index = 0;
    if (column_count != 0) {
        source_offset = 0;
        destination_column = column_count;
        do {
            row_index = 0;
            if (row_size != 0) {
                destination = (destination_column + destination_address) - 1;
                source = source_offset + source_address;
                do {
                    row_index += 1;
                    *destination = *source;
                    source += 1;
                    destination += column_count;
                } while (row_index != row_size);
            }
            column_index += 1;
            source_offset += row_size;
            destination_column -= 1;
        } while (column_index != column_count);
    }
}

/* resource/load_and_decompress.c */
void Resource_LoadAndDecompress(void *resource_id, void *destination, s32 destination_offset, s32 copy_palette)
{
    u8 *resource = GetResource((s32)resource_id);

    if (copy_palette != 0) {
        void (*copy)(void *, void *, s32) = (void (*)(void *, void *, s32))0x03001388;
        copy((void *)0x05000000, resource, 0x80);
    }
    if (destination_offset != 0) {
        resource += 0x80;
    }
    Sys_Apply(resource, destination);
}

/* battle/effects/particle_reveal.c */
/*
 * Battle-presentation sub-effect at 0x080e0c84.
 *
 * Confirmed member of the 0x03001eec "battle work" subsystem family
 * documented in games/gs1/src/battle/effects/member_orbit/run.c (owner
 * 080ce85c) and games/gs1/src/battle/effects/puff_arc/run.c (owner
 * 080d9fc8): same heap_cache=(void**)0x03001EEC / cursor / work / canvas
 * prologue, same the +0x7828 field=object republish, same
 * Battle_Do(0)/ScheduleCallbackAfterFrames(0x080CD261,0x480)/Scheduler_RemoveCallback(0x080CD261)/
 * Runtime_ReleaseHeapBlock(id)/Battle_Run() bracket, and the same
 * Battle_Apply2(flag, DrawRectangleFn callbacks[2]) two-word blit-routine
 * resolver already established in games/gs1/recon/en/main/080e01e4.c.
 *
 * Unlike member_orbit's single 64-frame per-member sprite loop, this owner
 * runs a 64-slot randomly-seeded particle pool (fixed-point x/y plus a
 * sin/cos velocity pair) alongside the 64-frame animation loop, and reads
 * every particle's fixed-point position back through the upper halfword of
 * its s32 field -- the same +2 / +6 halfword
 * idiom already confirmed in 080e01e4.c.
 *
 * Every `Battle_unk2_4`/`Battle_unk3_4` call site is an indirect call
 * through the value the reference loads into r4/r12 immediately before the
 * `bl`, not a real function -- both addresses fall inside the
 * `_call_via_rN` trampoline bank at games/gs1/asm/080072e4.s (r4 slot at
 * +0x10, ip/r12 slot at +0x30). All such call sites here go through
 * `routine[]`, a two-entry DrawRectangleFn array Battle_Apply2 fills.
 *
 * All three Resource_LoadAndDecompress id arguments are loaded from the reference's
 * literal pool rather than built with a `movs` immediate, matching the
 * already-adopted Value_ idiom (puff_arc/run.c's Value_000000b4,
 * 080e01e4.c's Value_00000073/00000090/00000089): `(s32)&Value_XXXXXXXX`
 * forces the same pool load even though the values (0x6e, 0xb8, 0x92) would
 * otherwise fit an 8-bit `movs` immediate.
 */

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

/* Fixed EWRAM scratch block holding the effect's particle records. */
#define PARTICLE_POOL ((Particle *)0x02010000)
#define PARTICLE_COUNT 64

extern u8 Value_0000006e;
extern u8 Value_000000b8;
extern u8 Value_00000092;
extern u8 gRom[];
extern u8 gRom2[];
extern u16 gRom3[];

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
    Battle_Do(0);
    Battle_SetRect(object, 1,
        (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4))), 2,
        &screen_x, &screen_y);
    Battle_Apply2(
        (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (4))), routine);
    Resource_LoadAndDecompress((s32)&Value_0000006e, work, 1, 1);
    (*(s32 *)((u8 *)(work) + (0x7780))) = 2;
    (*(s32 *)((u8 *)(work) + (0x7784))) = 75;
    {
        s32 interval;
        void *callback;

        interval = 0x480;
        callback = (void *)0x080CD261;
        ScheduleCallbackAfterFrames(callback, interval);
    }
    EffectPosition_ApplyAlternateStepAndYOffset(
        (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (0x24))), spawn);

    for (i = 0; i != PARTICLE_COUNT; i++) {
        s32 angle;
        s32 amp;

        angle = (random_16() & 0x7FFF) + 0x4000;
        amp = (random_16() & 0x1FF) + 0x80;
        PARTICLE_POOL[i].x =
            ((spawn[0] / 2 + (random_16() & 0xF)) - 8) << 16;
        PARTICLE_POOL[i].y = (spawn[1] + 8) << 16;
        PARTICLE_POOL[i].vx = (Battle_Check(angle) * amp) >> 9;
        PARTICLE_POOL[i].vy = (Battle_unk2(angle) * amp) >> 6;
        PARTICLE_POOL[i].rot = random_16() & 0x7F;
        PARTICLE_POOL[i].unk14 = random_16() & 0x7F;
        PARTICLE_POOL[i].unk18 = (random_16() & 0xF) + 32;
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
            x = (((-Battle_Check(orbit_angle)) << 2) >> 16)
                + screen_x / 2 - 10;
            y = ((Battle_unk2(orbit_angle) << 1) >> 16) + screen_y - 22;
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
                    canvas, (u8 *)work + 0x400 + gRom3[index],
                    (*(s16 *)((u8 *)(p) + (2))) - (w = gRom[index]) / 2,
                    (*(s16 *)((u8 *)(p) + (6))) - (h = gRom2[index]) / 2,
                    w, h);
                EffectStep_AdvanceWithGravity3D(p, 0x3F, 0x1000);
            }
        }

        if (frame == 8) {
            (*(s32 *)((u8 *)(work) + (0x77A8))) = frame;
            Battle_unk4_2(0x86);
            Battle_SetRange(
                (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + (0x7828)))) + (0x24))),
                7, 5, 0, 16);
            Battle_Apply4(
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
    Battle_Run();
}

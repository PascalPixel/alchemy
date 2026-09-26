/* Draft: complete 916-byte owner and literal pool; candidate 916 bytes,
   39 differing halfwords (35 aligned edits). Three distinct projection
   buffers, shared rectangle return type, typed work/target records and
   pre-projection particle lifetime recovered. Remaining differences are
   spill slots, initial particle/actor registers and scheduling. Allocator
   inspected; bounded initialization-order hypothesis regressed. */
#include "TYPES.H"
#include "SYSTEM.H"
#include "BATTLE_EFX.H"
#include "EFFECT_STEP.H"
#include "B5_CONTEXT.H"
#include "MOTION_OBJECT.H"

struct ParticleTarget {
    u32 reserved_00;
    s32 side;
    s32 object_id;
    u8 reserved_0c[24];
    s16 target_id;
};

struct ParticleWork {
    u8 reserved_0000[0x7080];
    struct EffectStep particles[64];
    s32 phase;
    s32 timer;
    u8 reserved_7788[0x20];
    s32 flash;
    u8 reserved_77ac[0x78];
    s32 dirty;
    struct ParticleTarget *target;
};

struct ParticleRuntime {
    struct ParticleWork *work;
    void *canvas;
    u8 *source;
};

typedef s32 (*WordCopyFn)(void *, const void *, s32);
extern struct ParticleRuntime Data_03001eec;
extern BattleEffectDrawRectangle Data_03001e50[];
extern const u16 Data_080ede48[];
extern char Value_00000073, Value_00000099, Value_000000bd;
extern char Value_000000c2, Value_000000b9, Value_000000bb, Value_000000c0;
void BattleFx_BeginCanvasLayer(s32 mode);
void Func_080df9d0(void *source, void *destination, s32 width, s32 rows);
void *Resource_GetTableEntry(s32 resource);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallback(void (*callback)(void));
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_SetApproachMotion(s32 first, s32 second, s32 divisor);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
void BattleMotion_ApplyVariantMotionFar(s32 id, s32 mode);
void BattleEventRuntime_BeginPhaseFar(s32 mode);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 first, s32 last);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);
void BattleFx_EndCanvasLayer(void);

/* Draw the target's two panels, then a 64-particle burst and expanding rings. */
void Func_080dfa48(struct ParticleTarget *object, s32 variant)
{
    void **cache;
    void **cursor;
    struct ParticleWork *work;
    void *canvas;
    u8 *source;
    BattleEffectDrawRectangle rectangle[2];
    struct EffectStep *particle;
    struct EffectPosition origin;
    struct EffectPosition screen;
    struct EffectPosition projected;
    struct MotionObject *actor;
    s32 frame;
    s32 cnt;
    s32 size;
    s32 offset;
    s32 facing;
    s32 palette;

    cache = (void **)&Data_03001eec;
    cursor = cache;
    work = *cursor++;
    canvas = *cursor;
    facing = *(s32 *)((u8 *)cache - 108);
    source = cache[2];
    work->target = object;
    BattleFx_BeginCanvasLayer(0);
    if (work->target->side == 0) {
        BattleEffect_LoadWork(46, 7, 7, 3, 2);
        BattleEffect_LoadWork(47, 7, 7, 11, 2);
    } else {
        BattleEffect_LoadWork(46, 7, 7, 7, 2);
        BattleEffect_LoadWork(47, 7, 7, 15, 2);
    }
    rectangle[0] = Data_03001e50[46];
    rectangle[1] = Data_03001e50[47];
    Resource_LoadAndDecompress((s32)&Value_00000073, source, 0, 0);
    Resource_LoadAndDecompress((s32)&Value_00000099, work, 1, 0);
    Func_080df9d0(work, (void *)0x02010000, 40, 288);
    Resource_LoadAndDecompress((s32)&Value_000000bd, work, 1, 1);
    switch (variant) {
    case 0: palette = (s32)&Value_000000c2; break;
    case 1: palette = (s32)&Value_000000b9; break;
    case 2: palette = (s32)&Value_000000bb; break;
    default: palette = (s32)&Value_000000c0; break;
    }
    ((WordCopyFn)0x03001388)((void *)0x05000000, Resource_GetTableEntry(palette), 128);
    work->phase = 2;
    work->timer = 75;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    BattleFx_SetApproachMotion(work->target->object_id, work->target->target_id, 10);
    actor = Func_080b5098(work->target->target_id)->object;
    particle = work->particles;
    cnt = 0;
    do {
        particle->x = actor->x;
        particle->y = actor->y + 0xa0000;
        particle->z = actor->z;
        particle->velocity_x = (Random16() & 0x1ff) << 11;
        particle->velocity_y = ((Random16() & 255) - 64) << 11;
        particle->velocity_z = ((Random16() & 255) - 128) << 11;
        if (particle->x > 0)
            particle->velocity_x = -particle->velocity_x;
        particle->variant = cnt / 2 + 16;
        cnt++;
        particle++;
    } while (cnt != 64);
    EffectPosition_ApplyAlternateStepAndYOffset(work->target->target_id, &origin);
    frame = 0;
    do {
        if (frame <= 14) {
            EffectPosition_ApplyAlternateStepAndYOffset(work->target->object_id, &screen);
            rectangle[0](canvas, work, screen.x / 2 - 16, screen.y - 48, 40, 32);
            rectangle[1](canvas, work, screen.x / 2 - 16, screen.y - 16, 40, 32);
        }
        if (frame == 10) {
            ObjectGroup_UpdateMembers(work->target->target_id, 7, 5, 0, 8);
            BattleMotion_ApplyVariantMotionFar(work->target->target_id, 4);
            BattleEventRuntime_BeginPhaseFar(134);
            work->flash = 8;
        }
        offset = frame - 8;
        if ((u32)offset <= 11) {
            s32 ring = offset / 2;
            rectangle[0](canvas, (u8 *)0x02010000 + ring * 0x3c0,
                origin.x / 2 - 16, screen.y - 40, 20, 48);
        }
        if ((u32)offset <= 55) {
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork(facing, facing + 12);
            particle = work->particles;
            cnt = 0;
            do {
                size = particle->variant;
                if (size > 0) {
                    EffectPosition_ApplyBaseAndYOffset(&particle->x, &projected);
                    size >>= 4;
                    size += 2;
                    projected.x >>= 1;
                    rectangle[0](canvas, source + Data_080ede48[size - 1],
                        projected.x - size / 2, projected.y - size, size, size * 2);
                    EffectStep_AdvanceWithGravity3D(particle, 60, -0x200);
                    particle->variant--;
                }
                cnt++;
                particle++;
            } while (cnt != 64);
        }
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->dirty = 1;
        WaitFrames(1);
        frame++;
    } while (frame != 60);
    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

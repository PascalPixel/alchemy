/* Draft: complete 916-byte owner and literal pool; candidate 916 bytes,
   39 differing halfwords (35 aligned edits). Three distinct projection
   buffers, shared rectangle return type, typed work/target records and
   pre-projection particle lifetime recovered. Remaining differences are
   spill slots, initial particle/actor registers and scheduling. Allocator
   inspected; bounded initialization-order hypothesis regressed.
   Separate camera ownership added a literal and reached 88 aligned edits;
   a persistent local layer record reached 944 bytes / 224 edits, and
   per-loop counters reached 912 bytes / 105 edits. Keep the derived camera
   cell and shared counter lifetime until new producer evidence appears.
   2026-09-29 stock agscc, as BattleFx_RunParticleFieldVariant with the
   buffer named gMapCellBuffer, the word copy through Iwram_CopyWords and
   resources 0x99, 0xbd and 0xc2 as link-time values: 916 of 916 bytes,
   13 differing lines, 12 aligned edits. Declaring the blitter pair and
   camera before the sheet gives the reference's spill slots; the seeding
   and drawing loops each index their own particle pointer by the counter,
   which gives the seed pointer r5, the actor r6 and the reference's
   preheader order. Left: the buffer's pool load is scheduled two
   instructions early before the tile-row packing call and in the ring
   draw. With the literal address instead of the name (a probe, not a
   candidate) the ring draw matches and only the packing call's load
   stays early, so the ring difference is the name compiling as a symbol
   rather than a constant. Needs Graphics_PackTileRows' real interface
   or argument form, and a ruling on the fixed EWRAM buffer. */
#include "TYPES.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "EFFECT_STEP.H"
#include "B5_CONTEXT.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_PRESENTATION.H"

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

extern struct ParticleRuntime gBattleFxWork;
extern u8 gMapCellBuffer[];
extern BattleEffectDrawRectangle Data_03001e50[];
extern u16 ParticleStreams_CellOffsets[];
extern char Value_00000073, Value_00000099, Value_000000bd;
extern char Value_000000c2, Value_000000b9, Value_000000bb, Value_000000c0;
void BattleFx_BeginCanvasLayer(s32 mode);
void Graphics_PackTileRows(void *source, void *destination, s32 width, s32 rows);
struct B5Context *GetBattleObjectSlotFar(s32 id);
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
void BattleFx_RunParticleFieldVariant(struct ParticleTarget *object, s32 variant)
{
    void **cache;
    void **cursor;
    struct ParticleWork *work;
    void *canvas;
    BattleEffectDrawRectangle rectangle[2];
    struct BattleCamera *camera;
    u8 *source;
    struct EffectStep *particle;
    struct EffectStep *step;
    struct EffectPosition origin;
    struct EffectPosition screen;
    struct EffectPosition projected;
    struct MotionObject *actor;
    s32 frame;
    s32 cnt;
    s32 size;
    s32 offset;
    s32 palette;

    cache = (void **)&gBattleFxWork;
    cursor = cache;
    work = *cursor++;
    canvas = *cursor;
    camera = *(struct BattleCamera **)((u8 *)cache - 108);
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
    Graphics_PackTileRows(work, gMapCellBuffer, 40, 288);
    Resource_LoadAndDecompress((s32)&Value_000000bd, work, 1, 1);
    switch (variant) {
    case 0: palette = (s32)&Value_000000c2; break;
    case 1: palette = (s32)&Value_000000b9; break;
    case 2: palette = (s32)&Value_000000bb; break;
    default: palette = (s32)&Value_000000c0; break;
    }
    Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry(palette), 128);
    work->phase = 2;
    work->timer = 75;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    BattleFx_SetApproachMotion(work->target->object_id, work->target->target_id, 10);
    actor = GetBattleObjectSlotFar(work->target->target_id)->object;
    for (cnt = 0; cnt != 64; cnt++) {
        step = &work->particles[cnt];
        step->x = actor->x;
        step->y = actor->y + 0xa0000;
        step->z = actor->z;
        step->velocity_x = (Random16() & 0x1ff) << 11;
        step->velocity_y = ((Random16() & 255) - 64) << 11;
        step->velocity_z = ((Random16() & 255) - 128) << 11;
        if (step->x > 0)
            step->velocity_x = -step->velocity_x;
        step->variant = cnt / 2 + 16;
    }
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
            rectangle[0](canvas, gMapCellBuffer + ring * 0x3c0,
                origin.x / 2 - 16, screen.y - 40, 20, 48);
        }
        if ((u32)offset <= 55) {
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
            for (cnt = 0; cnt != 64; cnt++) {
                particle = &work->particles[cnt];
                size = particle->variant;
                if (size > 0) {
                    EffectPosition_ApplyBaseAndYOffset(&particle->x, &projected);
                    size >>= 4;
                    size += 2;
                    projected.x >>= 1;
                    rectangle[0](canvas, source + ParticleStreams_CellOffsets[size - 1],
                        projected.x - size / 2, projected.y - size, size, size * 2);
                    EffectStep_AdvanceWithGravity3D(particle, 60, -0x200);
                    particle->variant--;
                }
            }
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

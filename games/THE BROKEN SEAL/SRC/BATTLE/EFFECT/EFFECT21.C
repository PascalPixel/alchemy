#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "EFFECT_STEP.H"
#include "B5_CONTEXT.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_PRESENTATION.H"
#include "RAM_BUFFER.H"
#include "RESOURCE_IDS.H"

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
extern BattleEffectDrawRectangle gWorkSlot[];
extern u16 ParticleStreams_CellOffsets[];
void BattleFx_BeginCanvasLayer(s32 mode);
void Graphics_PackTileRows(void *source, void *destination, s32 width, s32 rows);
struct B5Context *GetBattleObjectSlotFar(s32 id);
void *Resource_GetTableEntry(s32 resource);
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

void BattleFx_RunParticleFieldVariant(struct ParticleTarget *object, s32 variant);

/* Variant entries of the particle field variant effect. */
void BattleFx_RunParticleFieldVariant0(s32 effect)
{
    BattleFx_RunParticleFieldVariant(effect, 0);
}

void BattleFx_RunParticleFieldVariant1(s32 effect)
{
    BattleFx_RunParticleFieldVariant(effect, 1);
}

void BattleFx_RunParticleFieldVariant2(s32 effect)
{
    BattleFx_RunParticleFieldVariant(effect, 2);
}

void BattleFx_RunParticleFieldVariant3(s32 effect)
{
    BattleFx_RunParticleFieldVariant(effect, 3);
}

/* BattleFx_RunParticleFieldVariant: draw the target's two panels, then a
   64-particle burst and expanding rings from the map cell buffer, in one of
   four palettes. The buffer is the checked constant Ram_MapCellBuffer,
   held in a local, so each use reloads it from the pool as the ROM does.
   FAKEMATCH: the blitter pair and camera are declared before the sheet,
   and the permuter's reorder of two independent statements is kept, to
   give the ROM's spill slots and preheader order. */
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
    u8 *cells;

    cells = Ram_MapCellBuffer;
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
    rectangle[0] = gWorkSlot[46];
    rectangle[1] = gWorkSlot[47];
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, source, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_YellowOrbSheet, work, 1, 0);
    Graphics_PackTileRows(work, cells, 40, 288);
    Resource_LoadAndDecompress((s32)&ResourceId_BlueBurstSheet, work, 1, 1);
    switch (variant) {
    case 0:
        palette = (s32)&ResourceId_FirePillarSheetB;
        break;
    case 1:
        palette = (s32)&ResourceId_IceBlockSheet;
        break;
    case 2:
        palette = (s32)&ResourceId_PinkBurstSheet;
        break;
    default:
        palette = (s32)&ResourceId_BlastSheet;
        break;
    }
    Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry(palette), 128);
    work->phase = 2;
    work->timer = 75;
    Scheduler_AddOrUpdateCallback((s32)(BattlePresentation_ProcessPendingGraphicsTransfer), 0x480);
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
    for (frame = 0; frame != 60; frame++) {
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
            rectangle[0](canvas, cells + ring * 0x3c0, origin.x / 2 - 16, screen.y - 40, 20, 48);
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
                    rectangle[0](canvas, source + ParticleStreams_CellOffsets[size - 1], projected.x - size / 2, projected.y - size, size, size * 2);
                    EffectStep_AdvanceWithGravity3D(particle, 60, -0x200);
                    particle->variant--;
                }
            }
        }
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->dirty = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((u32)(BattlePresentation_ProcessPendingGraphicsTransfer));
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

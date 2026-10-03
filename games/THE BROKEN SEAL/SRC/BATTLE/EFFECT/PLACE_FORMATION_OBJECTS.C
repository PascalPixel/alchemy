#include "RUNTIME_MEM.H"
#include "HEAP_STATE.H"
#include "CANVAS.H"
#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFFECT_WORK.H"
#include "EFFECT_STEP.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"

extern u8 gBattleFxWork[];


struct Scale { s32 x; s32 y; };
struct Placement { s32 x; s32 y; s32 z; s32 w; };

extern const struct Scale Data_080edab8;
extern const struct Scale Data_080edac0;
extern const u8 Data_080eee1e[];
extern const u8 Data_080eee2a[];
extern const u8 Data_080eee36[];
extern const u8 Data_080eee3e[];
extern const u8 Data_080eee46[];
extern const u8 Data_080eee4e[];


void Object_ApplyProjectedPlacementFar(void *object, struct Placement *pos, struct Scale *scale, s32 flags);

/* Places the battle objects of one of four formations around (x, z): a
   3x3 grid, or the twelve- and eight-object layouts from the offset tables. */
void BattleFx_PlaceFormationObjects(s32 formation, s32 x, s32 z)
{
    struct BattleEffectWork *work = (struct BattleEffectWork *)*(void **)gBattleFxWork;
    struct Scale normal = Data_080edab8;
    struct Scale small = Data_080edac0;
    struct Placement pos;
    s32 i;

    pos.w = 0;
    pos.y = 0xff0000;
    switch (formation) {
    case 0:
        for (i = 0; i != 9; i++) {
            pos.x = (i % 3 << 21) + x;
            pos.z = (i / 3 << 21) + z;
            Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &normal, 0);
        }
        break;
    case 1:
        for (i = 0; i != 12; i++) {
            pos.x = (Data_080eee1e[i] << 16) + x - 0x100000;
            pos.z = (Data_080eee2a[i] << 16) + z - 0x200000;
            Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &normal, 0);
        }
        break;
    case 2:
        for (i = 0; i != 8; i++) {
            pos.x = (Data_080eee36[i] << 16) + x + 0x100000;
            pos.z = (Data_080eee3e[i] << 16) + z;
            Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &normal, 0);
        }
        break;
    case 3:
        for (i = 0; i != 8; i++) {
            pos.x = (Data_080eee46[i] << 16) + x;
            pos.z = (Data_080eee4e[i] << 16) + z;
            Object_ApplyProjectedPlacementFar(work->objects[i], &pos, &small, 0);
        }
        break;
    }
}

void Audio_PlayCue(s32 cue);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);

extern struct EffectStep gMapCellBuffer[];
extern u16 ParticleStreams_CellOffsets[];
extern u8 ImpactBurst_CellWidths[];
extern u8 ImpactBurst_CellHeights[];
extern u16 ImpactBurst_CellSourceOffsets[];

/* The whole-pixel half of a 16.16 coordinate. */
#define HI(v) (((s16 *)&(v))[1])

/* Battle effect: a flare opens at the impact point while thirty sparks
   spray from it, two more every frame; from frame 36 a second, finer spray
   of sixty motes follows and three flare shards fly apart. The formation's
   objects stay placed around the point for the first 36 frames. */
void BattleEffect_RunImpactBurst(s32 formation, s32 x, s32 y)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    DrawRectangle spark;
    DrawRectangle flare;
    void *sheet;
    s32 origin_x;
    struct EffectStep *step;
    s32 frame;
    s32 i;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    origin_x = x;
    x += 0x280000;
    x /= 2;
    REG_BG2PA = 0x80;
    REG_BG2X = 0;
    REG_BLDCNT = 0x3f46;
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
    flare = heap_cache[7];
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 3);
    spark = heap_cache[8];
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_FlareSheet, work, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_FlareImage, (u8 *)work + 0x59d8, 0, 0);
    work->transfer_mode = 2;
    work->transfer_value = 50;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (i = 0; i != 64; i++) {
        s32 speed = (Random16() & 255) + 256;
        s32 angle = Random16() & 0xffff;

        work->particles[i].x = x;
        work->particles[i].y = y;
        work->particles[i].velocity_x = (Trig_Sin(angle) * speed) >> 7;
        work->particles[i].velocity_y = -((Trig_Cos(angle) * speed) >> 6);
        work->particles[i].variant = (Random16() & 15) + 16;
    }

    for (i = 0; i != 3; i++) {
        work->particles[61 + i].x = x;
        work->particles[61 + i].y = y;
        work->particles[61 + i].velocity_x = (Trig_Sin(i * 0x5555) * 32) >> 6;
        work->particles[61 + i].velocity_y = -((Trig_Cos(i * 0x5555) * 32) >> 5);
    }

    for (i = 0; i != 64; i++) {
        s32 speed = (Random16() & 255) + 32;
        s32 angle = Random16() & 0xffff;

        gMapCellBuffer[i].x = x;
        gMapCellBuffer[i].y = y;
        gMapCellBuffer[i].velocity_x = (Trig_Sin(angle) * speed) >> 6;
        gMapCellBuffer[i].velocity_y = -((Trig_Cos(angle) * speed) >> 5);
        gMapCellBuffer[i].variant = (Random16() & 15) + 20;
    }

    for (frame = 0; frame != 72; frame++) {
        if (frame == 4)
            Audio_PlayCue(0x9a);
        if (frame == 32)
            Audio_PlayCue(0xd4);
        if (frame <= 47) {
            s32 cell = (frame - 8) / 5;
            u32 width;
            u32 height;

            if (cell < 0)
                cell = 0;
            flare(canvas, (u8 *)work + ImpactBurst_CellSourceOffsets[cell],
                (x >> 16) - ((width = ImpactBurst_CellWidths[cell]) >> 1),
                (y >> 16) - ((height = ImpactBurst_CellHeights[cell]) >> 1),
                width, height);
        }

        for (i = 0, step = work->particles; i != 30; i++, step++) {
            if (frame > i / 2 && step->variant > 0) {
                s32 size;
                s32 px;
                s32 py;

                step->variant--;
                EffectStep_AdvanceWithGravity2D(step, 60, 0);
                size = step->variant / 16 + 3;
                px = HI(step->x);
                py = HI(step->y);
                spark(canvas, (u8 *)sheet + ParticleStreams_CellOffsets[size - 1],
                    px - size / 2, py - size, size, size * 2);
            }
        }

        for (i = 0, step = gMapCellBuffer; i != 60; i++, step++) {
            if (frame > 35 && step->variant > 0) {
                s32 size;
                s32 px;
                s32 py;

                step->variant--;
                EffectStep_AdvanceWithGravity2D(step, 60, 0);
                size = step->variant / 16 + 1;
                px = HI(step->x);
                py = HI(step->y);
                spark(canvas, (u8 *)sheet + ParticleStreams_CellOffsets[size - 1],
                    px - size / 2, py - size, size, size * 2);
            }
        }

        for (i = 0; i != 3; i++) {
            struct EffectStep *shard = &work->particles[61 + i];

            if (frame >= 36 && frame <= 63) {
                s32 cell;
                s32 px;
                s32 py;

                EffectStep_AdvanceWithGravity2D(shard, 64, 0);
                cell = (frame - 36) / 7;
                px = HI(shard->x);
                py = HI(shard->y);
                flare(canvas, (u8 *)work + cell * 0x120 + 0x59d8,
                    px - 6, py - 12, 12, 24);
            }
        }

        if (frame <= 35)
            BattleFx_PlaceFormationObjects(formation, origin_x, y);
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    {
        /* FAKEMATCH: the routine's address is loaded before the size, as a
           call through a local makes it. */
        s32 (*clear)(void *, s32) = Iwram_ClearWords;

        clear(BG_CHAR_BLOCK(1), 0x4000);
    }
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
}

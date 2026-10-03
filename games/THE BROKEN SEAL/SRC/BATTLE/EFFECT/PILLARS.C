#include "BATTLE_PRESENTATION.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "CALL.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "MOTION_OBJECT.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"

/* Heap-allocation cache: gWorkSlot[kind] holds kind's block address. */
extern void *gWorkSlot[];

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_BeginCanvasLayer(s32 mode);
s32 BattleFx_EndCanvasLayer(void);
void *Resource_GetTableEntry(s32 id);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(void *a, void *b);
struct BattleObjectSlot *GetBattleObjectSlotFar(s32 member_id);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Audio_PlayCue(s32 cue);
void ObjectGroup_UpdateMembers(s32 actor, s32 object_mode, s32 group_mode,
    s32 slot, s32 delay);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);

extern u16 ParticleStreams_CellOffsets[];
/* Per variant: how many pillars rise, and how deep each one stands. */
extern u8 FirePillars_Counts[];
extern s8 FirePillars_Depths[][4];
/* The frame on which each pillar rises. */
extern u8 FirePillars_StartFrames[];

/* Battle effect: up to four pillars of fire rise in a row before the
   affected units, each from its own frame, throwing sparks that bounce on
   the ground; in variant 2 the camera swings round meanwhile. */
void BattleFx_RunFirePillars(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    s32 pillar;
    void *sheet;
    struct EffectPosition screen;
    DrawRectangle draw[2];
    struct MotionObject *object;
    struct BattleCamera *camera;
    s32 i;

    heap_cache = &gWorkSlot[39];
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    REG_BLDALPHA = 0x1010;
    Resource_LoadAndDecompress((s32)&ResourceId_FirePillarSheetA, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
    /* FAKEMATCH: forwarded through Call3, each palette copy loads its routine
       before it shifts the destination, as the reference does. */
    if (work->effect->variant == 0)
        Call3((void (*)())Iwram_CopyWords, (s32)BG_PLTT,
            (s32)Resource_GetTableEntry((s32)&ResourceId_RedPaletteB), 128);
    else if (work->effect->variant == 2)
        Call3((void (*)())Iwram_CopyWords, (s32)BG_PLTT,
            (s32)Resource_GetTableEntry((s32)&ResourceId_OrangePaletteB), 128);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw[0] = (DrawRectangle)gWorkSlot[46];
    BattleEffect_LoadWork(47, 7, 7, 7, 2);
    draw[1] = (DrawRectangle)gWorkSlot[47];

    for (i = 0; i != 1024; i++)
        ((struct EffectStep *)Ram_MapCellBuffer)[i].variant = 0;

    object = GetBattleObjectSlotFar(work->effect->actors[0])->object;
    for (i = 0; i != 4; i++) {
        struct EffectStep *base = &work->particles[i];
        s32 x;

        x = ((Random16() & 15) + 72) << 16;
        base->y = 0;
        base->x = x;
        base->z = FirePillars_Depths[work->effect->variant][i] << 16;
        if (object->x < 0)
            base->x = -x;
    }

    work->transfer_mode = 2;
    work->transfer_value = 50;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (frame = 0; frame != 96; frame++) {
        camera = gCameraWork;
        if (work->effect->variant == 2 && frame < 64) {
            if (work->effect->side == 0)
                camera->yaw = (s16)camera->yaw + 192;
            else
                camera->yaw = (s16)camera->yaw - 192;
        }
        if (frame == 16)
            BattleEventRuntime_BeginPhaseFar(134);
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(camera, camera->pos);

        if (frame < 64) {
            for (pillar = 0; pillar != FirePillars_Counts[work->effect->variant]; pillar++) {
                EffectPosition_ApplyBaseAndYOffset((s32 *)&work->particles[pillar], &screen);
                screen.x /= 2;
                screen.y -= 8;
                if (frame == FirePillars_StartFrames[pillar])
                    Audio_PlayCue(145);
                if (frame >= FirePillars_StartFrames[pillar] + 4) {
                    s32 scroll = (frame * 16 + pillar * 25) % 104;

                    draw[pillar & 1](canvas, work, screen.x - 17,
                        screen.y - scroll - 104, 34, 104);
                    draw[pillar & 1](canvas, work, screen.x - 17,
                        screen.y - scroll, 34, scroll);
                    if (frame & 1) {
                        draw[0](canvas, (u8 *)work + 0xdd0, screen.x - 20,
                            screen.y - 24, 20, 37);
                        draw[1](canvas, (u8 *)work + 0xdd0, screen.x,
                            screen.y - 24, 20, 37);
                    } else {
                        draw[0](canvas, (u8 *)work + 0x10b4, screen.x - 20,
                            screen.y - 24, 20, 37);
                        draw[1](canvas, (u8 *)work + 0x10b4, screen.x,
                            screen.y - 24, 20, 37);
                    }
                }
                if (frame == FirePillars_StartFrames[pillar] || frame >= FirePillars_StartFrames[pillar] + 16) {
                    s32 spawned = 0;

                    for (i = 0; i != 1024; i++) {
                        struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];

                        if (spark->variant == 0) {
                            s32 speed;
                            s32 angle;

                            speed = (Random16() & 0x3ff) + 32;
                            angle = (Random16() & 0x7fff) - 0x4000;
                            spark->x = screen.x << 8;
                            spark->y = (screen.y << 8) + 0x1000;
                            spark->z = (Trig_Sin(angle) * speed) >> 15;
                            spark->velocity_y = -(Trig_Cos(angle) * speed * 2) >> 15;
                            spawned++;
                            if (frame == FirePillars_StartFrames[pillar]) {
                                spark->variant = (Random16() & 7) + 48;
                                if (spawned == 200)
                                    break;
                            } else {
                                spark->variant = (Random16() & 7) + 24;
                                if (spawned == 4)
                                    break;
                            }
                        }
                    }
                }
                if (frame == FirePillars_StartFrames[pillar]) {
                    work->shake_frames = 2;
                    for (i = 0; i != work->effect->count; i++) {
                        ObjectGroup_UpdateMembers(work->effect->actors[i], 10, 5, i, 8);
                        BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 1);
                    }
                }
            }
        }

        for (i = 0; i != 1024; i++) {
            struct EffectStep *spark = &((struct EffectStep *)Ram_MapCellBuffer)[i];
            s32 life = spark->variant;

            if (life > 0) {
                s32 x;
                s32 y;
                s32 py;

                spark->variant = life - 1;
                x = spark->x += spark->z;
                y = spark->y += spark->velocity_y;
                spark->z = spark->z * 60 / 64;
                spark->velocity_y = spark->velocity_y * 60 / 64 - 16;
                py = y / 256;
                if (py > 120) {
                    spark->velocity_y = -spark->velocity_y / 2;
                } else if (x >= 0 && x >> 8 <= 126 && y >= 0) {
                    s32 px = x >> 8;
                    s32 size = (life - 17) / 8;

                    if (size <= 0)
                        size = 1;
                    draw[i & 1](canvas, (u8 *)sheet + ParticleStreams_CellOffsets[size - 1],
                        px - size / 2, py - size, size, size * 2);
                }
            }
        }
        Camera_ApplyShake(16, 16);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}

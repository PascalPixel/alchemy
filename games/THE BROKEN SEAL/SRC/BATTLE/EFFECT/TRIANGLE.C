#include "RUNTIME_MEM.H"
#include "HEAP_STATE.H"
#include "MOTION_OBJECT.H"
#include "CANVAS.H"
#include "RESOURCE.H"
#include "BATTLE_PRESENTATION.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFFECT_WORK.H"


struct Vector3 {
    s32 x;
    s32 y;
    s32 z;
};

extern u16 BattleFx6_FlareCells[];
/* One corner of the triangle before it is rolled into place. */
extern struct Vector3 SpinningTriangle_Vertex[];
extern struct Vector3 TriangleStrike_Vertex[];

void BattleFx_PrepareCanvasEffect(struct BattleEffectArgument *effect, s32 a, s32 b, s32 c, s32 *x, s32 *y);
void BattleMotion_ApproachTargetFar(s32 actor, s32 target, s32 a, s32 b);
void Graphics_UpdatePhasePalette(s32 frame, s32 red_phase, s32 green_phase, s32 blue_phase);
void BattleEventRuntime_BeginPhaseFar(s32 value);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 source, s32 destination);
void SceneTransform_ApplyRoll(s32 angle);
void SceneTransform_ApplyYaw(s32 angle);
void SceneTransform_ApplyScale(struct Vector3 *scale);
void AudioCommand_PlayFar(s32 value);

/*
 * A triangle of flare dots spins down onto each affected unit, shrinking for
 * 64 ticks, then gives way to the two halves of a 48 by 48 picture. With the
 * variant set, a 40 by 40 picture first circles the place the canvas effect
 * names and rises away. Units start eight frames apart.
 */
void BattleFx_RunSpinningTriangle(struct BattleEffectArgument *effect)
{
    struct EffectPosition anchor;
    struct Vector3 vector;
    struct EffectPosition screen;
    struct Vector3 scale;
    s32 origin_x;
    s32 origin_y;
    DrawRectangle draw[2];
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    s32 member;
    struct BattleCamera *camera;
    s32 shift;
    u8 *graphics;
    u8 *palette;
    struct MotionObject *object;
    s32 tick;
    struct EffectStep *point;
    s32 size;
    s32 j;
    s32 k;

    heap_cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    graphics = heap_cache[2];
    /* FAKEMATCH: the existing word-cell read keeps the cache base separate
       from the later blitter pointer reads; a pointer read coalesces them. */
    camera = (struct BattleCamera *)*(s32 *)(gWorkSlot + HEAP_SLOT_CAMERA * sizeof(void *));
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    if (work->effect->unknown_001c == 1)
        BattleFx_PrepareCanvasEffect(effect, 3, work->effect->side, 0, &origin_x, &origin_y);
    *(s16 *)0x04000020 = 0x100;
    palette = Resource_GetTableEntry((s32)&ResourceId_RuneSheet);
    Iwram_CopyWords((void *)0x05000000, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD), (u8 *)work + 0x1000);
    palette = Resource_GetTableEntry((s32)&ResourceId_JupiterDjinnSheet);
    palette += 128;
    Resource_DecodeType01(palette, (u8 *)work + 0x2000);
    work->transfer_mode = 3;
    work->transfer_value = 0x04040404;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &anchor);
    shift = 64 - anchor.x;
    *(s32 *)0x04000028 = shift << 8;
    AudioCommand_PlayFar(142);

    for (frame = 0; frame != work->effect->count * 20 + 72; frame++) {
        if (frame == 64)
            BattleEventRuntime_BeginPhaseFar(0);
        Graphics_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);
        if (work->effect->unknown_001c == 1) {
            s32 x;
            s32 y;

            x = ((Trig_Sin(frame << 11) * 20) >> 16) + origin_x + shift - 20;
            y = ((Trig_Cos(frame << 11) << 2) >> 16) + origin_y - 24;
            BattleFx_FetchRectangleBlitters(work->effect->side, draw);
            if (frame > 32)
                y = y - frame * 2 + 64;
            draw[0](canvas, (u8 *)work + 0x2000, x, y, 40, 40);
            if (frame <= 3)
                draw[1](canvas, (u8 *)work + 0x2000, x, y, 40, 40);
            Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
            Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        }
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
        draw[0] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
        draw[1] = *(DrawRectangle *)(gWorkSlot + HEAP_SLOT_BLITTER_ALTERNATE * sizeof(void *));
        if (frame > 16 && (frame & 15) == 0)
            work->transfer_value += 0x01010101;
        for (member = 0; member != 1; member++) {
            tick = frame - member * 8;
            object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
            if (tick >= 0 && tick < 96) {
                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
                vector.x = object->x;
                vector.y = object->y;
                vector.z = object->z;
                EffectPosition_ApplyBaseAndYOffset((s32 *)&vector, &screen);
                screen.x = anchor.x + shift;
                screen.y -= 24;
                if (tick <= 67) {
                    for (j = 0; j != 3; j++) {
                        point = &work->particles[member * 32 + j];
                        Render_ResetTransformState();
                        if (tick <= 63) {
                            scale.x = 0x2a000 - (tick * 3 << 9);
                            scale.y = 0x2a000 - (tick * 3 << 9);
                            scale.z = 0x2a000 - (tick * 3 << 9);
                            SceneTransform_ApplyScale(&scale);
                            SceneTransform_ApplyRoll((64 - tick) << 9);
                            SceneTransform_ApplyYaw((64 - tick) << 9);
                        }
                        SceneTransform_ApplyRoll(j * 0x5555);
                        EffectPosition_ApplyBaseAndYOffset((s32 *)SpinningTriangle_Vertex, (struct EffectPosition *)&vector);
                        point->velocity_x = vector.x + screen.x;
                        point->velocity_y = vector.y + screen.y + 16;
                    }
                    for (j = 0; j != 3; j++) {
                        struct EffectStep *from = &work->particles[j + member * 32];
                        struct EffectStep *to = &work->particles[(j + 1) % 3 + member * 32];

                        size = 5 - tick / 16;
                        for (k = 0; k != 24; k++) {
                            s32 x = from->velocity_x + k * (to->velocity_x - from->velocity_x) / 24;
                            s32 y = from->velocity_y + k * (to->velocity_y - from->velocity_y) / 24;

                            draw[0](canvas,
                                (u8 *)work + BattleFx6_FlareCells[size - 1] + 0x1000,
                                x - size, y - size, size * 2, size * 2);
                        }
                    }
                }
                if (tick > 63) {
                    draw[0](canvas, work, screen.x - 24, screen.y - 24, 24, 48);
                    draw[1](canvas, work, screen.x, screen.y - 24, 24, 48);
                }
            }
        }
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}

/*
 * The same triangle without the circling picture: the acting unit closes on
 * the first target at frame 46.
 */
void BattleFx_RunTriangleStrike(struct BattleEffectArgument *effect)
{
    struct EffectPosition anchor;
    struct Vector3 vector;
    struct EffectPosition screen;
    struct Vector3 scale;
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    DrawRectangle draw[2];
    s32 member;
    struct BattleCamera *camera;
    s32 shift;
    u8 *graphics;
    u8 *palette;
    struct MotionObject *object;
    s32 tick;
    struct EffectStep *point;
    s32 size;
    s32 j;
    s32 k;

    heap_cache = &((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BATTLE_EFFECT];
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    graphics = heap_cache[2];
    /* FAKEMATCH: the existing word-cell read keeps the cache base separate
       from the later blitter pointer reads; a pointer read coalesces them. */
    camera = (struct BattleCamera *)*(s32 *)(gWorkSlot + HEAP_SLOT_CAMERA * sizeof(void *));
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    *(s16 *)0x04000020 = 0x100;
    palette = Resource_GetTableEntry((s32)&ResourceId_RuneSheet);
    Iwram_CopyWords((void *)0x05000000, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
    palette = Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesD);
    Resource_DecodeType01(palette, (u8 *)work + 0x1000);
    work->transfer_mode = 3;
    work->transfer_value = 0x04040404;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &anchor);
    shift = 64 - anchor.x;
    *(s32 *)0x04000028 = shift << 8;
    AudioCommand_PlayFar(142);

    for (frame = 0; frame != work->effect->count * 20 + 72; frame++) {
        if (frame == 64)
            BattleEventRuntime_BeginPhaseFar(0);
        if (frame == 46)
            BattleMotion_ApproachTargetFar(work->effect->actor, work->effect->actors[0], 16, 0);
        Graphics_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
        draw[0] = (DrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
        BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
        draw[1] = *(DrawRectangle *)(gWorkSlot + HEAP_SLOT_BLITTER_ALTERNATE * sizeof(void *));
        if (frame > 16 && (frame & 15) == 0)
            work->transfer_value += 0x01010101;
        for (member = 0; member != 1; member++) {
            tick = frame - member * 8;
            object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
            if (tick >= 0 && tick < 96) {
                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
                vector.x = object->x;
                vector.y = object->y;
                vector.z = object->z;
                EffectPosition_ApplyBaseAndYOffset((s32 *)&vector, &screen);
                screen.x = anchor.x + shift;
                screen.y -= 24;
                if (tick <= 67) {
                    for (j = 0; j != 3; j++) {
                        point = &work->particles[member * 32 + j];
                        Render_ResetTransformState();
                        if (tick <= 63) {
                            scale.x = 0x2a000 - (tick * 3 << 9);
                            scale.y = 0x2a000 - (tick * 3 << 9);
                            scale.z = 0x2a000 - (tick * 3 << 9);
                            SceneTransform_ApplyScale(&scale);
                            SceneTransform_ApplyRoll((64 - tick) << 9);
                            SceneTransform_ApplyYaw((64 - tick) << 9);
                        }
                        SceneTransform_ApplyRoll(j * 0x5555);
                        EffectPosition_ApplyBaseAndYOffset((s32 *)TriangleStrike_Vertex, (struct EffectPosition *)&vector);
                        point->velocity_x = vector.x + screen.x;
                        point->velocity_y = vector.y + screen.y + 16;
                    }
                    for (j = 0; j != 3; j++) {
                        struct EffectStep *from = &work->particles[j + member * 32];
                        struct EffectStep *to = &work->particles[(j + 1) % 3 + member * 32];

                        size = 5 - tick / 16;
                        for (k = 0; k != 24; k++) {
                            s32 x = from->velocity_x + k * (to->velocity_x - from->velocity_x) / 24;
                            s32 y = from->velocity_y + k * (to->velocity_y - from->velocity_y) / 24;

                            draw[0](canvas,
                                (u8 *)work + BattleFx6_FlareCells[size - 1] + 0x1000,
                                x - size, y - size, size * 2, size * 2);
                        }
                    }
                }
                if (tick > 63) {
                    draw[0](canvas, work, screen.x - 24, screen.y - 24, 24, 48);
                    draw[1](canvas, work, screen.x, screen.y - 24, 24, 48);
                }
            }
        }
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
        Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}

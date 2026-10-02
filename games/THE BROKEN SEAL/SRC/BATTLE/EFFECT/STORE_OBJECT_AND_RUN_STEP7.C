#include "TYPES.H"
#include "DMA.H"
#include "IO_REG.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_EFX.H"
#include "BATTLE_PRESENTATION.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "IO_WRITE_QUEUE.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "MOTION_OBJECT.H"

extern u8 gBattleFxWork[];

void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_PrepareCanvasEffect(void *object, s32 a, s32 b, s32 c, s32 *out_a, s32 *out_b);
void BattleFx_FetchRectangleBlitters(s32 alternate, DrawRectangleFn *output);
void BattleFx_ArmBg2AffineHBlankDma(void);
void BattleFx_EndCanvasLayer(void);

/*
 * Battle effect: a rippling disc. A 128 x 128 map of distances from a point
 * just below the centre is drawn once with a 63-colour ramp; for 96 frames
 * a sine wave bends the scanlines while the ramp rotates one colour a frame.
 * The blend level fades in over the first nine frames and out over the last
 * seven.
 */
void BattleFx_RunRipplingDisc(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    struct BattleEffectWork *work;
    void *canvas;
    s32 screen_y;
    s32 screen_x;
    DrawRectangleFn draw[2];
    s32 x;
    s32 y;
    s32 dx;
    s32 dy;
    s32 cy;
    s32 d;
    s32 i;
    s32 t;
    s32 r;
    s32 g;
    s32 b;
    s32 frame;
    s32 wave;
    s32 level;
    u16 *pal;
    volatile u16 *ime;
    s32 angle;
    s32 *line;

    heap_cache = (void **)gBattleFxWork;
    work = *heap_cache++;
    canvas = *heap_cache;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0x2000);
    BattleFx_PrepareCanvasEffect(effect, 6, work->effect->side, 2, &screen_x, &screen_y);
    REG_BG2CNT = 0x2784;
    REG_BLDALPHA = 0x1000;
    REG_BG2PA = 0xaa;
    BattleFx_FetchRectangleBlitters(work->effect->side, draw);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (y = 0; y != 64; y++) {
        for (x = 0; x != 64; x++) {
            cy = y / 8 + 64;
            dy = y - cy;
            dx = x - 64;
            d = Iwram_Sqrt(dx * dx + dy * dy);
            d /= 2;
            if (d == 0)
                d = 1;
            if (d > 63)
                d = 63;
            ((u8 *)work)[y * 128 + x] = d;
            ((u8 *)work)[y * 128 + 127 - x] = d;
            ((u8 *)work)[(127 - y) * 128 + x] = d;
            ((u8 *)work)[(127 - y) * 128 + 127 - x] = d;
        }
    }

    pal = (u16 *)Ram_MapCellBuffer;
    for (i = 1; i != 64; i++) {
        if (i > 31)
            t = 64 - i;
        else
            t = i;
        r = t * 9;
        g = t * 7 - 42;
        b = t * 7 - 56;
        if (r < 0)
            r = 0;
        if (g < 0)
            g = 0;
        if (b < 0)
            b = 0;
        if (r > 255)
            r = 255;
        if (g > 255)
            g = 255;
        if (b > 250)
            b = 250;
        r >>= 3;
        g >>= 3;
        b >>= 3;
        pal[i] = ((u16 *)BG_PLTT)[i] = b << 10 | g << 5 | r;
    }

    draw[0](canvas, work, 0, 0, 128, 128);
    work->transfer_pending = 1;
    Scheduler_AddOrUpdateCallback((s32)BattleFx_ArmBg2AffineHBlankDma, 0x480);

    for (frame = 0; frame != 96; frame++) {
        if (frame <= 8) {
            level = frame * 2;
            wave = level;
            REG_BLDALPHA = level | 0x1000;
        } else {
            wave = frame * 2;
        }
        if (frame > 88)
            REG_BLDALPHA = (0xc0 - frame * 2) | 0x1000;

        line = work->bg2_x;
        for (i = 0, angle = -(wave << 9); i != 160; i++) {
            *line++ = ((i << 18) - (Trig_Sin(angle) << 7) + 0x40000) >> 10;
            angle += 0x200;
        }

        if (frame > 127) {
            work->transfer_pending = 1;
        } else {
            pal[1] = pal[63];
            Dma_Set(pal + 62, pal + 63, 0x80a0003e, REG_DMA3);
            {
                struct IoWriteQueue *q;
                u32 saved;
                s32 count;

                q = &gIoWriteQueue;
                /* FAKEMATCH: the one-pass block keeps the queue load first and the saved copy ahead of the IME store, as in the IO write queue. */
                do {
                    ime = &REG_IME;
                    saved = *ime;
                } while (0);
                *ime = (u16)ime;
                count = q->count;
                if (count <= 31) {
                    u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);
                    /* FAKEMATCH: the count is stored through a u16 pointer, as in the IO write queue, which places the store after the entry address. */
                    *(u16 *)&q->count = count + 1;
                    *destination++ = (u32)(pal + 1);
                    *destination++ = 0x05000002;
                    *destination = 0x8000003f;
                }
                REG_IME = saved;
            }
        }
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((s32)BattleFx_ArmBg2AffineHBlankDma);
    Scheduler_RemoveCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

extern u8 gBattleFxWork[];

void BattleFx_BeginCanvasLayer();
void BattleFx_EndCanvasLayer();
void BattleFx_PrepareCanvasEffect();

void ObjectGroup_StoreObjectAndRunStep7(s32 a0, s32 a1, s32 a2)
{
    u8 *p5;
    u8 slot12[4];
    u8 slot8[4];

    p5 = *(s32 *)((u32)gBattleFxWork);
    *(s32 *)(((s32)p5 + 0x7828)) = a0;
    BattleFx_BeginCanvasLayer(0);
    BattleFx_PrepareCanvasEffect(a0, 7, (*(s32 *)(*(s32 *)(((s32)p5 + 0x7828)) + 4) ^ 1), 0, slot12, slot8);
    BattleFx_EndCanvasLayer();
}




extern u8 gMapCellBuffer[];
/* The fire swirl's seven cells: size, place in the sheet and offset from
   the caster. */
extern u8 FireSwirl_CellWidths[];
extern u8 FireSwirl_CellHeights[];
extern u16 FireSwirl_CellSourceOffsets[];
extern s8 FireSwirl_CellBiasX[];
extern s8 FireSwirl_CellBiasY[];

struct MotionObject **GetBattleObjectSlotFar(s32 id);
void Object_ResetMotion(struct MotionObject *object);
void Object_SetMoveTargetFar(struct MotionObject *object, s32 x, s32 y, s32 z);
void Object_SetMode(struct MotionObject *object, s32 mode);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
void BattleEventRuntime_BeginPhaseFar(s32 value);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void BattleMotion_ApplyVariantMotionFar(s32 actor_id, s32 motion);
void EffectStep_AdvanceWithGravity2D(struct EffectStep *step, s32 damping, s32 gravity);
void Camera_ApplyShake(s32 random_mask, s32 shake_range);
void ObjectGroup_TickMemberTimers(void);

/*
 * Battle effect: the acting unit charges four fifths of the way to its
 * target while a fire swirl plays in front of it; on frame 18 the target is
 * hit and sixteen stars burst out, fall and bounce.
 */
void BattleFx_RunFireSwirlCharge(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    u8 *graphics;
    DrawRectangle draw[2];
    struct EffectPosition position;

    struct EffectStep *sparks;
    struct EffectStep *spark;
    s32 *life;
    s32 frame;
    s32 i;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    graphics = heap_cache[2];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    REG_BG2PA = 0x100;
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, graphics, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_FireSwirlSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_PinkStarSheet, (u8 *)work + 0x3e80, 1, 0);
    BattleFx_FetchRectangleBlitters(work->effect->side, draw);
    work->transfer_mode = 2;
    work->transfer_value = 50;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    life = &((struct EffectStep *)gMapCellBuffer)->variant;
    for (i = 0; i != 1024; i++) {
        *life = 0;
        life += 7;
    }
    sparks = (struct EffectStep *)gMapCellBuffer;

    {
        struct MotionObject *object = *GetBattleObjectSlotFar(work->effect->actor);
        struct MotionObject *target = *GetBattleObjectSlotFar(work->effect->actors[0]);
        s32 diff_x = target->x - object->x;
        s32 start_x = object->x;
        s32 delta_x = diff_x * 80 / 100;
        s32 diff_z = target->z - object->z;
        s32 start_z = object->z;
        s32 delta_z = diff_z * 80 / 100;
        s32 x = start_x + delta_x;
        s32 z = start_z + delta_z;
        s32 short_x = delta_x >> 8;
        s32 short_z = delta_z >> 8;
        s32 distance;

        distance = Iwram_Sqrt(short_x * short_x + short_z * short_z);
        distance = (distance << 8) / 20;
        object->acceleration = distance;
        object->speed_limit = distance;
        object->snap_to_target = 1;
        object->velocity_y = 0x70000;
        object->vertical_motion_strength = 0xdeb8;
        object->vertical_motion_phase = 0;
        object->auto_face_motion = 1;
        Object_ResetMotion(object);
        Object_SetMoveTargetFar(object, x, 0, z);
        Object_SetMode(object, 2);
    }

    for (frame = 0; frame != 70; frame++) {
        EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actor, &position);
        REG_BG2X = (80 - position.x) << 8;
        if ((u32)(frame - 8) <= 15) {
            s32 cell = (frame - 8) / 2;

            if (cell > 6)
                cell = 6;
            if (work->effect->side == 0) {
                draw[0](canvas, (u8 *)work + FireSwirl_CellSourceOffsets[cell],
                    FireSwirl_CellBiasX[cell] + 30,
                    FireSwirl_CellBiasY[cell] + position.y - 60,
                    FireSwirl_CellWidths[cell], FireSwirl_CellHeights[cell]);
            } else {
                draw[0](canvas, (u8 *)work + FireSwirl_CellSourceOffsets[cell],
                    -FireSwirl_CellBiasX[cell] - FireSwirl_CellWidths[cell] + 108,
                    FireSwirl_CellBiasY[cell] + position.y - 60,
                    FireSwirl_CellWidths[cell], FireSwirl_CellHeights[cell]);
            }
        }
        if (frame == 18) {
            BattleEventRuntime_BeginPhaseFar(134);
            ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
            BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 6);
            work->shake_frames = 4;
            for (i = 0; i != 16; i++) {
                s32 force = (Random16() & 63) + 256;
                s32 angle = Random16() & 0xffff;

                spark = &sparks[i];
                spark->x = 0x400000;
                spark->y = 0x500000;
                spark->velocity_x = (Trig_Sin(angle) * force) >> 7;
                spark->velocity_y = -(Trig_Cos(angle) * force) >> 6;
                spark->variant = (Random16() & 15) + 16;
            }
        }
        for (i = 0; i != 128; i++) {
            spark = &sparks[i];
            if (spark->variant > 0) {
                spark->variant--;
                EffectStep_AdvanceWithGravity2D(spark, 60, 0);
                if (spark->y > 0x680000) {
                    spark->velocity_y = -spark->velocity_y / 2;
                } else if ((u32)spark->x <= 0x7effff && spark->y >= 0) {
                    s32 sx = spark->x >> 16;
                    s32 sy = spark->y >> 16;

                    draw[0](canvas, (u8 *)work + 0x3e80 + (((frame + i) / 4 % 6) << 8),
                        sx - 8, sy - 8, 16, 16);
                }
            }
        }
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

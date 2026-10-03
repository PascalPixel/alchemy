#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "SCENE.H"
#include "RESOURCE.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "EFFECT_STEP.H"
#include "MOTION_OBJECT.H"
#include "RAM_BUFFER.H"

extern u8 gBattleFxWork[];
void BattleFx_ArmBg2AffineHBlankDma(void);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);

/*
 * Battle-presentation sub-effect: entry 34 of the effect callback table at
 * 0x080ee2b4.  The single argument is the effect state pointer, which the
 * owner republishes at work + 0x7828.
 */
#define FIELD_AT_OFFSET(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))
void BattleFx_BeginCanvasLayer(s32 mode);
u32 Resource_DecodeType01(const void *source, void *destination);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void **GetBattleObjectSlotFar(s32 member_id);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void BattleFx_EndCanvasLayer(void);

void BattleFx_RunMemberBurst(struct BattleEffectArgument *effect, s32 mode);

extern u16 ParticleStreams_CellOffsets[];

/* A chip of debris: it flies on its own speed, slows, falls and bounces. */
struct Chip {
    s32 x;
    s32 y;
    s32 velocity_x;
    s32 unused_0c;
    s32 velocity_y;
    s32 unused_14;
    s32 life;
};

#define gChips ((struct Chip *)Ram_MapCellBuffer)

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_BeginCanvasLayer(s32 mode);
void BattleMotion_ApplyVariantMotionFar(s32 actor, s32 variant);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void AudioCommand_PlayFar(s32 value);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 first, s32 last);
void Camera_ApplyShake(s32 x, s32 y);
void ObjectGroup_TickMemberTimers(void);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 id, struct EffectPosition *position);
u32 Resource_DecodeType01(const void *source, void *destination);
void Object_SetMode(struct MotionObject *object, s32 mode);
void Object_ResetMotion(struct MotionObject *object);
void Object_SetMoveTargetFar(struct MotionObject *object, s32 x, s32 y, s32 z);

/* Battle effect: the actor flares, leaps high across the field and comes
   down on the target, which bursts into chips. */
void BattleFx_RunLeapingStrike(struct BattleEffectArgument *effect)
{
    struct EffectPosition position;
    DrawRectangle draw[2];
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    u8 *sheet;
    struct MotionObject *target;
    s32 step;
    struct MotionObject *actor;
    u8 *palette;
    s32 frame;
    s32 i;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    sheet = heap_cache[2];
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), sheet);
    palette = Resource_GetTableEntry((s32)&ResourceId_FlashBurstSheet);
    Iwram_CopyWords((void *)0x05000000, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw[0] = heap_cache[7];
    BattleEffect_LoadWork(47, 7, 7, 7, 2);
    draw[1] = heap_cache[8];
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (i = 0; i != 1024; i++)
        gChips[i].life = 0;
    actor = *GetBattleObjectSlotFar(work->effect->actor);
    target = *GetBattleObjectSlotFar(work->effect->actors[0]);
    if (actor->x > 0)
        step = -0xf0000;
    else
        step = 0xf0000;

    for (frame = 0; frame != 88; frame++) {
        struct BattleCamera *camera = gCameraWork;

        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
        if (frame > 17 || frame == 0) {
            EffectPosition_ApplyAlternateStepAndYOffset(work->effect->actor, &position);
            position.x /= 2;
        }
        if ((u32)(frame - 2) <= 1)
            draw[0](canvas, work, position.x - 16, position.y - 64, 32, 64);
        if ((u32)(frame - 4) <= 11) {
            for (i = 0; i != 16; i++) {
                s32 x = position.x + (frame * Trig_Sin(i << 12) >> 16);
                s32 y = position.y + (frame * Trig_Cos(i << 12) >> 16) - frame;

                draw[0](canvas, work->sheet + ((frame - 4) / 2 << 11), x - 16, y - 64, 32, 64);
            }
        }
        if (frame == 4) {
            actor->velocity_y = 0x140000;
            actor->acceleration = 0x10000;
            actor->speed_limit = 0x30000;
            actor->vertical_motion_strength = 0xab85;
            actor->auto_face_motion = 0;
            actor->snap_to_target = 0;
            Object_SetMoveTargetFar(actor, actor->x * 3, 0, actor->z);
            Object_SetMode(actor, 2);
            work->shake_frames = frame;
            AudioCommand_PlayFar(136);
        }
        if (frame == 16) {
            palette = Resource_GetTableEntry((s32)&ResourceId_FlameballSheet);
            Iwram_CopyWords((void *)0x05000000, palette, 128);
            palette += 128;
            Resource_DecodeType01(palette, work);
            actor->vertical_motion_strength = 0;
            actor->velocity_x = 0;
            actor->velocity_y = 0;
            actor->z = target->z;
            Object_ResetMotion(actor);
        }
        if (frame > 17) {
            if (actor->y > 0) {
                actor->x += step;
                actor->y -= 0x80000;
                if (work->effect->side == 0) {
                    draw[0](canvas, work, position.x - 20, position.y - 52, 40, 64);
                    position.x -= 8;
                } else {
                    draw[1](canvas, work, position.x - 26, position.y - 52, 40, 64);
                    position.y += 8;
                }
            }
            if (actor->y < 0) {
                actor->y = 0;
                for (i = 0; i != 256; i++) {
                    struct Chip *chip = &gChips[i];
                    s32 speed;
                    s32 angle;

                    speed = 0x3ff;
                    speed &= Random16();
                    angle = Random16() & 0xffff;
                    chip->x = position.x << 16;
                    chip->y = (position.y - 24) << 16;
                    chip->velocity_x = Trig_Sin(angle) * (speed + 32) >> 6;
                    chip->velocity_y = -(Trig_Cos(angle) * (speed + 32) << 1) >> 6;
                    chip->life = (Random16() & 7) + 32;
                }
                work->shake_frames = 8;
                BattleEventRuntime_BeginPhaseFar(145);
                BattleMotion_ApplyVariantMotionFar(work->effect->actors[0], 4);
                ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
            }
        }
        for (i = 0; i != 256; i++) {
            struct Chip *chip = &gChips[i];
            s32 y;

            if (chip->life > 0) {
                chip->x += chip->velocity_x;
                y = chip->y + chip->velocity_y;
                chip->life--;
                chip->y = y;
                chip->velocity_x = chip->velocity_x * 56 / 64;
                chip->velocity_y = chip->velocity_y * 56 / 64 + 0x2000;
                if (chip->y > 0x700000) {
                    chip->velocity_y = -chip->velocity_y / 2;
                } else if ((u32)chip->x <= 0x7effff && chip->y >= 0) {
                    s32 size = chip->life / 8 + 1;

                    draw[i & 1](canvas, sheet + ParticleStreams_CellOffsets[size - 1],
                        (chip->x >> 16) - size / 2, (chip->y >> 16) - size, size, size * 2);
                }
            }
        }
        Camera_ApplyShake(16, 16);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

#define gSkulls ((struct EffectStep *)Ram_MapCellBuffer)

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_ArmBg2AffineHBlankDma(void);
void BattleFx_BeginCanvasLayer(s32 mode);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void AudioCommand_PlayFar(s32 value);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 first, s32 last);
void ObjectGroup_TickMemberTimers(void);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void SceneTransform_ApplyPosition(s32 *position);
void SceneTransform_ApplyPitch(s32 angle);
u32 Resource_DecodeType01(const void *source, void *destination);


typedef s32 (*WordCopy)(void *, const void *, s32);

static __inline__ void CopyPalette(WordCopy copy, void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: the two palette copies share the routine's address but
       build the palette address again at each call, which only an inlined
       constant argument compiles to; a direct call keeps it in a register. */
    copy(destination, source, size);
}

struct BlitterPair {
    DrawRectangle upper;
    DrawRectangle lower;
};

/* Battle effect: eight skulls for each target start scattered around it and
   close in, turning with the frame, while the background sways. */
void BattleFx_RunClosingSkulls(struct BattleEffectArgument *effect)
{
    struct EffectPosition position;
    s32 point[3];
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    struct BlitterPair draw;
    struct BattleCamera *camera;
    u8 *palette;
    s32 i;
    s32 k;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    palette = Resource_GetTableEntry((s32)&ResourceId_SkullSheet);
    CopyPalette(Iwram_CopyWords, (void *)0x05000000, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work);
    palette = Resource_GetTableEntry((s32)&ResourceId_PinkBurstSheet);
    CopyPalette(Iwram_CopyWords, (void *)0x05000000, palette, 128);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw.upper = heap_cache[7];
    BattleEffect_LoadWork(47, 7, 7, 3, 3);
    draw.lower = heap_cache[8];
    Scheduler_AddOrUpdateCallback((s32)BattleFx_ArmBg2AffineHBlankDma, 0x480);
    work->transfer_mode = 3;
    work->transfer_value = 0x04040404;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (i = 0; i != 512; i++) {
        gSkulls[i].x = ((Random16() & 255) - 127) << 15;
        gSkulls[i].y = ((Random16() & 255) - 127) << 15;
        gSkulls[i].z = ((Random16() & 255) - 127) << 15;
    }
    AudioCommand_PlayFar(142);

    for (frame = 0; frame != work->effect->count * 32 + 96; frame++) {
        s32 *row;

        camera = gCameraWork;
        if (frame == 96)
            BattleEventRuntime_BeginPhaseFar(0);
        row = work->bg2_x;
        if (work->effect->side == 0) {
            for (i = 0; i != 160; i++)
                *row++ = (0x60000 - Trig_Sin((frame + i) << 11) * 6) >> 10;
        } else {
            for (i = 0; i != 160; i++)
                *row++ = Trig_Sin((frame + i) << 11) * 6 >> 10;
        }
        for (k = 0; k != work->effect->count; k++) {
            struct MotionObject *target = *GetBattleObjectSlotFar(work->effect->actors[k]);

            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
            point[0] = target->x;
            point[1] = 0x140000;
            point[2] = target->z;
            SceneTransform_ApplyPosition(point);
            if (frame > k * 32) {
                SceneTransform_ApplyPitch(frame << 9);
                if (frame == k * 32 + 32)
                    ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, k, 32);
                for (i = 0; i != 8; i++) {
                    struct EffectStep *skull = &gSkulls[k * 64 + i];

                    if (frame > (k * 8 + i) * 4) {
                        s32 x = (skull->x >> 8) * (skull->x >> 8);
                        s32 y = (skull->y >> 8) * (skull->y >> 8);
                        s32 z = (skull->z >> 8) * (skull->z >> 8);
                        s32 distance = Iwram_Sqrt(x + y + z) >> 8;

                        if (distance != 0) {
                            EffectPosition_ApplyBaseAndYOffset((s32 *)skull, &position);
                            position.x >>= 1;
                            draw.upper(canvas, work->sheet + i % 3 * 576,
                                position.x - 12, position.y - 12, 24, 24);
                            skull->x -= skull->x / distance;
                            skull->y -= skull->y / distance;
                            skull->z -= skull->z / distance;
                            skull->variant++;
                        }
                    }
                }
            }
        }
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Scheduler_RemoveCallback((u32)BattleFx_ArmBg2AffineHBlankDma);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}


/* A small absolute link-time constant.  The resource id must be built from a
 * literal pool word, which an ordinary integer literal cannot produce. */

/*
 * Sets the BG2 affine scale, loads the palette and the 32x32 sprite frames
 * into the work block, prepares the two rectangle-blit routines, and runs
 * member_count * 16 + 48 frames of a sine-swept scanline table with four
 * sprites orbiting each member whose window is open.  Binding each callee's
 * result to `status` is load-bearing: it makes the call a set of r0 and so
 * fixes the order of the following argument setup.
 */
void BattleFx_RunMemberOrbit(void *object)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    void *palette;
    s32 status;
    void *rectangle[2];
    s32 record[3];
    s32 screen[3];
    s32 member;
    s32 y_offset;
    void **rectangle_slot;
    void *rect2;
    s32 *record_slot;
    s32 frame;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = object;
    BattleFx_BeginCanvasLayer(0);
    FIELD_AT_OFFSET((void *)0x04000020, s16 *, 0) = 0x100;
    palette = Resource_GetTableEntry((s32)&ResourceId_SpiralSheet);
    status = Iwram_CopyWords((void *)0x05000000, palette, 128);
    status = Resource_DecodeType01((u8 *)palette + 128, work);
    status = BattleEffect_LoadWork(46, 7, 7, 3, 2);
    rectangle[0] = heap_cache[7];
    status = BattleEffect_LoadWork(47, 7, 7, 15, 2);
    rect2 = heap_cache[8];
    rectangle_slot = rectangle;
    rectangle_slot[1] = rect2;
    Scheduler_AddOrUpdateCallback((void *)BattleFx_ArmBg2AffineHBlankDma, 0x480);
    work->transfer_mode = 2;
    work->transfer_value = 50;
    Scheduler_AddOrUpdateCallback((void *)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    if (work->effect->side == 1) {
        FIELD_AT_OFFSET((void *)0x04000028, s32 *, 0) = -0x6800;
        y_offset = -112;
    } else {
        y_offset = 0;
    }
    for (frame = 0;
            frame != (work->effect->count
                * 16) + 48;
            frame++) {
        s32 facing;
        s32 *scanline;
        s32 i;
        s32 id_ofs;

        facing = (s32)gCameraWork;
        scanline = work->bg2_x;
        if (work->effect->side == 0) {
            s32 angle;
            s32 ceiling;

            for (i = 0, ceiling = 0x80000, angle = frame << 10;
                    i != 160; i++) {
                *scanline++ = (ceiling - (Trig_Sin(angle) << 3)) >> 10;
                angle += 1024;
            }
        } else {
            s32 angle;

            for (i = 0, angle = frame << 10; i != 160; i++) {
                *scanline++ = ((Trig_Sin(angle) << 3) >> 10) - 0x7000;
                angle += 1024;
            }
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(facing, facing + 12);
        member = 0;
        if (work->effect->count != 0) {
            record_slot = record;
            id_ofs = 36;
            while (member
                != work->effect->count) {
                void *member_object;

                member_object = *GetBattleObjectSlotFar(
                    work->effect->actors[member]);
                if (frame > member * 16 && frame < (member * 16) + 60) {
                    s32 spin;

                    if (frame == (member * 16) + 32) {
                        ObjectGroup_UpdateMembers(
                            work->effect->actors[member],
                            0, 5, -1, 0);
                    }
                    record_slot[0] = FIELD_AT_OFFSET(member_object, s32 *, 8);
                    record_slot[1] = 0x280000;
                    record_slot[2] = FIELD_AT_OFFSET(member_object, s32 *, 16);
                    EffectPosition_ApplyBaseAndYOffset(record_slot, (struct EffectPosition *)screen);
                    for (i = 0; i != 4; i++) {
                        s32 x;
                        s32 y;
                        s32 slot;

                        spin = (frame << 9) + (i << 14);
                        x = (screen[0] + ((Trig_Sin(spin) << 4) >> 16))
                            + y_offset;
                        y = screen[1] + ((Trig_Cos(spin) << 4) >> 16);
                        slot = frame / 16;
                        ((DrawRectangleFn)rectangle_slot[slot & 1])(
                            canvas,
                            (u8 *)work + (((frame / 4) - (slot * 4)) << 10),
                            x - 16, y - 16, 32, 32);
                    }
                }
                id_ofs += 2;
                member++;
            }
        }
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((void *)BattlePresentation_ProcessPendingGraphicsTransfer);
    Scheduler_RemoveCallback((void *)BattleFx_ArmBg2AffineHBlankDma);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

void BattleFx_RunMemberBurstMode0(struct BattleEffectArgument *effect)
{
    BattleFx_RunMemberBurst(effect, 0);
}

void BattleFx_RunMemberBurstMode1(struct BattleEffectArgument *effect)
{
    BattleFx_RunMemberBurst(effect, 1);
}

void BattleFx_RunMemberBurstMode2(struct BattleEffectArgument *effect)
{
    BattleFx_RunMemberBurst(effect, 2);
}

/* For each mode: how many motes burst from each member and how many frames
   the effect runs after the last member's turn. */
extern u8 MemberBurst_Counts[];

#define gMotes ((struct EffectStep *)Ram_MapCellBuffer)

u32 Battle_GetObjectTableValueFar(s32 actor_id);
void BattleFx_FetchRectangleBlitters(s32 alternate, DrawRectangle *output);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void AudioCommand_PlayFar(s32 value);
void ObjectGroup_TickMemberTimers(void);

/* Battle effect: motes burst out of the actor, hang in the air and then
   home in on each target in turn. */
void BattleFx_RunMemberBurst(struct BattleEffectArgument *effect, s32 mode)
{
    struct EffectPosition position;
    DrawRectangle draw[2];
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    s32 k;
    struct BattleCamera *camera;
    s32 resource;
    s32 i;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    camera = *(struct BattleCamera **)((u8 *)heap_cache - 108);
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_MemberBurstImage), work);
    if (mode == 0)
        resource = (s32)&ResourceId_PinkBurstSheet;
    else if (mode == 1)
        resource = (s32)&ResourceId_MarsDjinnSheet;
    else
        resource = (s32)&ResourceId_MercuryDjinnSheet;
    Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry(resource), 128);
    BattleFx_FetchRectangleBlitters(work->effect->side, draw);

    for (i = 0; i != 1024; i++)
        gMotes[i].variant = -1;
    for (k = 0; k != work->effect->count; k++) {
        struct MotionObject *source = *GetBattleObjectSlotFar(work->effect->actor);
        s32 height = Battle_GetObjectTableValueFar(work->effect->actor);

        for (i = 0; i != 128; i++) {
            struct EffectStep *mote = &gMotes[k * 128 + i];

            mote->x = source->x;
            mote->y = height;
            mote->z = source->z;
            mote->velocity_x = ((Random16() & 255) - 128) << 10;
            mote->velocity_y = ((Random16() & 255) - 128) << 10;
            mote->velocity_z = ((Random16() & 255) - 128) << 10;
            mote->variant = 0;
        }
    }

    Scheduler_AddOrUpdateCallback((s32)BattleFx_ArmBg2AffineHBlankDma, 0x480);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    AudioCommand_PlayFar(146);

    for (frame = 0; frame != MemberBurst_Counts[mode * 2 + 1] + work->effect->count * 20; frame++) {
        s32 *row;

        if (frame == 80) {
            if (mode == 0)
                BattleEventRuntime_BeginPhaseFar(134);
            else
                BattleEventRuntime_BeginPhaseFar(133);
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
        row = work->bg2_x;
        for (i = 0; i != 160; i++)
            *row++ = (0x100000 - Trig_Sin((frame + i) << 10) * 16) >> 10;
        for (k = 0; k != work->effect->count; k++) {
            struct MotionObject *target = *GetBattleObjectSlotFar(work->effect->actors[k]);
            s32 half = (s32)Battle_GetObjectTableValueFar(work->effect->actors[k]) / 2;

            if (frame == k * 20 + 71) {
                if (mode == 0)
                    AudioCommand_PlayFar(134);
                else
                    AudioCommand_PlayFar(133);
            }
            if (frame == k * 20 + 70)
                ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, k, 26);
            if (frame > k * 20) {
                for (i = 0; i != MemberBurst_Counts[mode * 2]; i++) {
                    struct EffectStep *mote = &gMotes[k * 128 + i];

                    if (frame > (k * 10 + i) * 2 && mote->variant >= 0) {
                        EffectPosition_ApplyBaseAndYOffset((s32 *)mote, &position);
                        position.x >>= 1;
                        draw[0](canvas, work->sheet + i % 3 * 640,
                            position.x - 10, position.y - 16, 20, 32);
                        EffectStep_AdvanceWithGravity3D(mote, 62, 0);
                        if (frame > k * 20 + i + 30) {
                            s32 dx = (target->x - mote->x) >> 9;
                            s32 dy = (target->y + half - mote->y) >> 9;
                            s32 dz = (target->z - mote->z) >> 9;

                            mote->velocity_x += dx;
                            mote->velocity_y += dy;
                            mote->velocity_z += dz;
                            if ((u32)(dx + 0xfff) <= 0x1ffe && (u32)(dz + 0xfff) <= 0x1ffe)
                                mote->variant = -1;
                        }
                    }
                }
            }
        }
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    Scheduler_RemoveCallback((u32)BattleFx_ArmBg2AffineHBlankDma);
    BattleFx_EndCanvasLayer();
}

#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MEM.H"
#include "HEAP_STATE.H"
#include "MOTOBJ.H"
#include "CANVAS.H"
#include "RESOURCE.H"
#include "BTLDISP.H"
#include "TRANSFORM.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_STEP.H"
#include "BATTLE_EFFECT_WORK.H"
#include "MENU_LIST.H"
#include "FIELD_EVENT.H"
#include "PROJECT.H"
#include "B5_CONTEXT.H"
#include "BTLTYPE.H"
#include "IO_REG.H"
#include "RAM_BUFFER.H"

#define SceneTransform_ApplyPosition ((void (*)(void*))SceneTransform_ApplyPosition)
#define SceneTransform_ApplyScale ((void (*)(struct Vector3*))SceneTransform_ApplyScale)
#define Camera_ApplyShake ((void (*)(s32,s32))Camera_ApplyShake)
#define ObjectDispatch_ApplyValueToChildrenFar ((void (*)(struct MotionObject*,s32))ObjectDispatch_ApplyValueToChildrenFar)
#define Object_SetMoveTargetFar ((void (*)(struct MotionObject*,s32,s32,s32))Object_SetMoveTargetFar)
#define BattlePresentation_ConfigurePaletteFadeFar ((void (*)(s32,s32,s32))BattlePresentation_ConfigurePaletteFadeFar)

extern u8 gMapCellBuffer[];

void Object_ResetMotion(struct MotionObject *object);

void BattleFx_RunRingBolts(struct BattleEffectArgument *effect)
{
    struct EffectPosition pos;
    struct EffectPosition base;
    struct Vector3 scale;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    DrawRectangle draw[2];
    s32 i;
    u8 *graphics;
    struct BattleCamera *camera;
    struct MotionObject *caster;
    struct MotionObject *source;
    struct MotionObject *target;
    struct EffectStep *point;
    struct EffectStep *step;
    s32 start;
    s32 peak;
    s32 size;
    s32 j;
    s32 k;
    struct EffectStep *trails;

    work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
    canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
    graphics = *(gWorkSlot + HEAP_SLOT_BATTLE_SHEET);
    camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
    caster = GetBattleObjectSlotFar(effect->actor)->object;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
    Object_SetMode(caster, 2);
    ObjectDispatch_ApplyValueToChildrenFar(caster, 48);
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    draw[0] = (DrawRectangle)*(gWorkSlot + HEAP_SLOT_BLITTER);
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    source = GetBattleObjectSlotFar(work->effect->actor)->object;
    target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
    for (i = 0, point = work->particles; i != 3; i++) {
        point->x = source->x;
        point->y = source->y + 0x280000;
        point->z = source->z;
        if (i == 0)
            point->velocity_x = (target->x - point->x) / 12;
        else
            point->velocity_x = (target->x * 2 - point->x) / 12;
        point->velocity_y = (target->y - point->y + 0x280000) / 12;
        point->velocity_z = (target->z - point->z) / 12;
        point->variant = 0;
        point++;
    }

    trails = (struct EffectStep *)gMapCellBuffer;
    for (frame = 0; frame != 60; frame++) {
        if (frame <= 47) {
            struct BattleCamera *view = *(gWorkSlot + HEAP_SLOT_CAMERA);
            s32 speed;

            if (frame <= 39)
                speed = 128;
            else
                speed = 0x300 - frame * 16;
            if (work->effect->side == 0)
                view->yaw -= speed;
            else
                view->yaw += speed;
        }
        for (i = 0; i != 3; i++) {
            start = i * 12;
            if (frame >= start) {
                step = &work->particles[i];
                size = (frame - start) / 4 + 2;
                if (size > 10)
                    size = 10;
                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
                SceneTransform_ApplyPosition(step);
                peak = 0;
                for (j = 0; j != 10; j++) {
                    s32 depth;

                    Graphics_SaveTransferWorkOnce();
                    SceneTransform_ApplyRoll((frame - start) << 10);
                    SceneTransform_ApplyYaw(0x4000);
                    scale.x = ((frame - start) << 12) + 0x1000;
                    if (scale.x > 0x10000)
                        scale.x = 0x10000;
                    scale.y = scale.x;
                    scale.z = scale.x;
                    SceneTransform_ApplyScale(&scale);
                    SceneTransform_ApplyRoll(j * 0x199a);
                    depth = EffectPosition_ApplyBaseAndYOffset((s32 *)&RingBolts_Points[j & 1], &pos);
                    if (peak < depth)
                        peak = depth;
                    pos.x >>= 1;
                    trails[i * 10 + j].velocity_x = pos.x + base.x;
                    trails[i * 10 + j].velocity_y = pos.y + base.y;
                    trails[i * 10 + j].velocity_x = pos.x;
                    trails[i * 10 + j].velocity_y = pos.y;
                    Graphics_RestoreTransferWork();
                }
                if (peak <= 399999) {
                    for (j = 0; j != 10; j++) {
                        struct EffectStep *from = &((struct EffectStep *)gMapCellBuffer)[j + i * 10];
                        struct EffectStep *to = &((struct EffectStep *)gMapCellBuffer)[(j + 1) % 10 + i * 10];

                        for (k = 0; k != 16; k++) {
                            s32 x = from->velocity_x + (to->velocity_x - from->velocity_x) * k / 16;
                            s32 y = from->velocity_y + (to->velocity_y - from->velocity_y) * k / 16;

                            draw[0](canvas,
                                graphics + ParticleStreams_CellOffsets[size - 1],
                                x - size / 2, y - size, size, size * 2);
                        }
                    }
                }
                step->x += step->velocity_x;
                step->y += step->velocity_y;
                step->z += step->velocity_z;
                if (frame == start + i + 10) {
                    target->acceleration = 0x20000;
                    target->speed_limit = 0x80000;
                    target->velocity_y = 0x50000;
                    target->vertical_motion_strength = 0xab85;
                    target->auto_face_motion = 0;
                    Object_ResetMotion(target);
                    if (target->x < 0)
                        Object_SetMoveTargetFar(target, target->x - 0x280000, 0, target->z);
                    else
                        Object_SetMoveTargetFar(target, target->x + 0x280000, 0, target->z);
                    if (i == 2) {
                        BattleEventRuntime_BeginPhaseFar(134);
                    } else {
                        AudioCommand_PlayFar(134);
                        ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 8);
                    }
                    work->shake_frames = 4;
                }
            }
        }
        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    BattleFx_EndCanvasLayer();
}

void BattleFx_RunSpinningStars(struct BattleEffectArgument *effect)
{
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    DrawRectangle draw[2];
    s32 i;
    u8 *graphics;
    struct BattleCamera *camera;
    struct MotionObject *source;
    struct MotionObject *target;
    struct EffectStep *point;
    struct EffectStep *step;
    struct EffectPosition pos;
    s32 size;
    s32 j;
    s32 k;
    struct EffectStep *trails;

    work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
    canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
    graphics = *(gWorkSlot + HEAP_SLOT_BATTLE_SHEET);
    camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry((s32)&ResourceId_RuneSheet), 128);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);
    BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
    work->transfer_mode = 2;
    work->transfer_value = 50;
    draw[0] = (DrawRectangle)*(gWorkSlot + HEAP_SLOT_BLITTER);
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    source = GetBattleObjectSlotFar(work->effect->actor)->object;
    target = GetBattleObjectSlotFar(work->effect->actors[0])->object;
    for (i = 0, point = work->particles; i != 8; i++) {
        s32 delay = i * 8;

        point->x = source->x / 2;
        point->y = source->y + 0x780000;
        point->z = source->z;
        point->velocity_x = (target->x + ((s32)((Random16() & 0x7f) - 64) << 16) - point->x) / 12;
        point->velocity_y = (target->y - point->y + 0x140000) / 12;
        point->velocity_z = (target->z - point->z) / 12;
        point->variant = (Random16() & 15) + delay;
        point++;
    }

    size = 2;
    trails = (struct EffectStep *)gMapCellBuffer;
    for (frame = 0; frame != 128; frame++) {
        Graphics_UpdatePhasePalette(frame, 0xaaab, 0x5555, 0);
        if (frame == 96)
            BattleEventRuntime_BeginPhaseFar(134);
        for (i = 0; i != 8; i++) {
            step = &work->particles[i];
            if (frame >= step->variant) {
                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
                EffectPosition_ApplyBaseAndYOffset((s32 *)step, &pos);
                pos.x >>= 1;
                if (pos.x >= -8 && pos.x <= 127) {
                    if (pos.y <= 127) {
                        if (pos.y >= -8) {
                            for (j = 0; j != 10; j++) {
                                trails[i * 10 + j].velocity_x = pos.x
                                    + ((Trig_Sin(j * 0x199a - ((frame - step->variant) << 11))
                                        * SpinningStars_Radii[j & 1]) / 2 >> 16);
                                trails[i * 10 + j].velocity_y = pos.y
                                    - (Trig_Cos(j * 0x199a - ((frame - step->variant) << 11))
                                        * SpinningStars_Radii[j & 1] >> 16);
                            }
                            for (j = 0; j != 10; j++) {
                                struct EffectStep *from = &trails[j + i * 10];
                                struct EffectStep *to = &trails[i * 10 + (j + 1) % 10];

                                for (k = 0; k != 12; k++) {
                                    s32 x = from->velocity_x + (to->velocity_x - from->velocity_x) * k / 12;
                                    s32 y = from->velocity_y + (to->velocity_y - from->velocity_y) * k / 12;

                                    draw[0](canvas,
                                        graphics + ParticleStreams_CellOffsets[size - 1],
                                        x - size / 2, y - size, size, size * 2);
                                }
                            }
                        }
                    }
                }
                if (step->y <= 0x1dffff) {
                    step->velocity_y = -step->velocity_y;
                    step->velocity_x /= 2;
                    step->velocity_z /= 2;
                    work->shake_frames = 4;
                    AudioCommand_PlayFar(134);
                    for (j = 0; j != work->effect->count; j++)
                        ObjectGroup_UpdateMembers(work->effect->actors[j], 7, 5, j, 8);
                }
                step->x += step->velocity_x;
                step->y += step->velocity_y;
                step->z += step->velocity_z;
            }
        }
        Camera_ApplyShake(4, 4);
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
    Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
    BattleFx_EndCanvasLayer();
}

struct Placement {
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_0c;
};

struct DrawPair {
    DrawRectangle rect;
    DrawRectangle rect_alt;
};

struct ScalePair {
    s32 x;
    s32 y;
};
extern const struct ScalePair BattleFx_StarsUnitScale;
extern u8 Data_080ee15a[];
extern u8 Data_080ee163[];
extern u16 Data_080ee16c[];
extern u8 Data_080ee174[];
extern u8 Data_080ee177[];
extern u8 Data_080ee17a[];
extern u16 Data_080ee17e[];
s32 ArcTan2(s32 x, s32 z);
void Object_ApplyProjectedPlacementFar(void *handle, struct Placement *place, struct ScalePair *scale,
    s32 mode);

void BattleFx_RunWhirlwindComets(struct BattleEffectArgument *argument)
{
    void **cells;
    void *draw_destination;
    void *view;
    struct BattleEffectWork *runtime;
    struct DrawPair draw;
    s32 frame;
    u8 *graphics;
    struct ScalePair wave;
    struct ScalePair drift;
    struct MotionObject *object;
    struct BtlObj *context;
    struct EffectStep *point;
    struct EffectStep *spark;
    struct EffectStep *mark;
    u8 *tbl;
    u8 *src;
    s32 index;
    s32 slot;
    s32 size;
    s32 span;
    s32 dist;
    s32 alive;
    s32 width;
    s32 heading[8];
    s32 radius[8];
    s32 spin[8];
    struct ScalePair home[8];
    s32 base_angle[8];
    struct Vector3 work;
    struct EffectPosition screen;
    struct Vector3 shake;
    struct Placement place;
    struct ScalePair scale;

    cells = (void **)(gBattleFxWork + 4);
    draw_destination = cells[0];
    view = *(void **)((u8 *)cells - 112);
    runtime = cells[-1];
    graphics = (u8 *)cells[1];
    runtime->effect = argument;

    BattleFx_BeginCanvasLayer(0);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw.rect = (DrawRectangle)cells[6];
    BattleEffect_LoadWork(47, 7, 7, 3, 3);
    draw.rect_alt = (DrawRectangle)cells[7];

    tbl = Resource_GetTableEntry((s32)&ResourceId_WhirlwindSheet);
    Iwram_CopyWords((void *)0x05000000, tbl, 0x80);
    tbl += 0x80;
    Resource_DecodeType01(tbl, runtime);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_ParticleSpritesA), graphics);

    runtime->transfer_mode = 2;
    runtime->transfer_value = 50;
    Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    wave.x = 0x800000;
    wave.y = 0x280000;
    drift.x = 0;
    drift.y = 0;

    for (index = 0; index != runtime->effect->count; index++) {
        context = GetBattleObjectSlotFar(runtime->effect->actors[index]);
        object = (struct MotionObject *)context->object;
        home[index].x = object->x;
        home[index].y = object->z;
        base_angle[index] = object->angle;
        heading[index] = (u16)ArcTan2(object->x, object->z);
        radius[index] = Iwram_Sqrt(
            (object->x >> 8) * (object->x >> 8)
                + (object->z >> 8) * (object->z >> 8)) >> 7;
        spin[index] = 0;
        object->vertical_motion_strength = 0;
    }

    index = 0;
    do {
        point = &runtime->particles[index];
        point->z = 120;
        point->y = 0;
        Random16();
        point->velocity_y = 0;
        point->velocity_x = 0;
        point->variant = Random16() & 63;
        index++;
    } while (index != 64);

    width = 48;
    frame = 0;
    do {
        if ((gKeysRepeat & (KEY_A | KEY_B)) != 0) {
            if (frame <= 159)
                frame = 160;
            else if (frame <= 394)
                frame = 395;
        }

        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(view, (u8 *)view + 12);

        if (frame == 16)
            AudioCommand_PlayFar(141);
        if (frame == 256)
            AudioCommand_PlayFar(140);
        if (frame == 334)
            AudioCommand_PlayFar(212);
        if (frame == 347)
            AudioCommand_PlayFar(212);
        if (frame == 359)
            AudioCommand_PlayFar(212);
        if (frame == 372)
            AudioCommand_PlayFar(212);

        for (index = 0; index != runtime->effect->count; index++) {
            if (frame > index * 16) {
                context = GetBattleObjectSlotFar(
                    runtime->effect->actors[index]);
                object = (struct MotionObject *)context->object;
                object->x =
                    (radius[index] * Trig_Sin(heading[index])) >> 1;
                object->z =
                    (radius[index] * Trig_Cos(heading[index])) >> 1;

                if (frame <= 159) {
                    if (frame > index * 16 + 16)
                        spin[index] += 48;
                    if (radius[index] <= 31)
                        object->y += 0x60000;
                    else
                        object->y += 0x8000;
                    if (object->y > 0x7c0000)
                        object->y = 0x7c0000;
                }

                if (frame <= 490) {
                    heading[index] += spin[index];
                    if (heading[index] > 0x10000)
                        heading[index] -= 0x10000;
                    object->angle += spin[index] / 4;
                }

                if (frame == 395)
                    object->vertical_motion_strength = 0xab85;

                slot = 0;
                do {
                    if (frame == Data_080ee16c[slot]) {
                        object->velocity_y = 0;
                        ObjectGroup_UpdateMembers(
                            runtime->effect->actors[index],
                            7,
                            5,
                            index,
                            8);
                    }
                    slot++;
                } while (slot != 4);

                if (radius[index] > 16)
                    radius[index] -= 2;
            }
        }

        if ((u32)(frame - 16) <= 143) {
            size = frame * 2 - 32;
            if (size > 48)
                size = 48;
            draw.rect(
                draw_destination,
                &runtime->sheet[frame / 4 % 3 * 3072
                    + (width - size) * 48],
                32,
                112 - size,
                width,
                size);
        }

        if ((u32)(frame - 48) <= 111) {
            size = frame * 2 - 96;
            if (size > 64)
                size = 64;
            draw.rect(
                draw_destination,
                &runtime->sheet[frame / 4 % 3 * 3072
                    + (64 - size) * 48],
                32,
                64 - size,
                width,
                size);
        }

        if ((u32)(frame - 160) <= 239) {
            src = (u8 *)runtime + frame / 4 % 3 * 3072;
            draw.rect(draw_destination, src, 8, 0, width, 64);
            draw.rect(draw_destination, src, 8, 64, width, 64);
        }

        if (frame <= 159) {
            index = 0;
            point = runtime->particles;
            do {
                if (point[index].variant == 0) {
                    work.x = point[index].z * Trig_Sin(point[index].velocity_y);
                    work.y = point[index].y;
                    work.z = point[index].z * Trig_Cos(point[index].velocity_y);
                    EffectPosition_ApplyBaseAndYOffset(&work.x, &screen);
                    screen.x >>= 1;
                    if (point[index].y <= 0x3fffff) {
                        draw.rect(
                            draw_destination,
                            (u8 *)runtime + 0x2400,
                            screen.x - 12,
                            screen.y - 12,
                            16,
                            16);
                    }
                    if (point[index].z > 24)
                        point[index].z -= 4;
                    point[index].velocity_y += point[index].velocity_x * 2;
                    if (point[index].velocity_y > 0x10000)
                        point[index].velocity_y -= 0x10000;
                    point[index].velocity_x += 50;
                    if (point[index].velocity_x > 4096)
                        point[index].velocity_x = 4096;
                    point[index].velocity_y += 2048;
                    point[index].y += point[index].velocity_y * 2;
                    if (point[index].y > 0x300000) {
                        point[index].y = 0;
                        point[index].z = 100;
                        point[index].velocity_y = 0;
                        point[index].velocity_x = 0;
                    }
                } else {
                    point[index].variant--;
                }
                index++;
            } while (index != 24);
        }

        if (frame == 160) {
            s32 speed;
            s32 angle;

            BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
            BattlePresentation_ConfigurePaletteFadeFar(1, (s32)&ResourceId_VioletSkyBackdrop, 8);
            BattleFx_SelectLivingTargets(runtime->effect);
            BattleFx_SpawnObjects(9, 376, 2);

            tbl = Resource_GetTableEntry((s32)&ResourceId_VioletCometSheet);
            Iwram_CopyWords((void *)0x05000000, tbl, 0x80);
            tbl += 0x80;
            Resource_DecodeType01(tbl, (u8 *)runtime + 0x3600);

            index = 0;
            do {
                ((struct EffectStep *)Ram_MapCellBuffer)[index].variant = 0;
                index++;
            } while (index != 512);

            for (index = 0; index != runtime->effect->count; index++) {
                context = GetBattleObjectSlotFar(
                    runtime->effect->actors[index]);
                object = (struct MotionObject *)context->object;
                object->y = ((Random16() & 15) + 8) << 16;
            }

            gProjection.center_x = 72;

            index = 0;
            do {
                speed = Random16() & 127;
                angle = (Random16() & 0x7fff) + 0x9fff;
                runtime->particles[index].x = Trig_Sin(angle) * speed;
                runtime->particles[index].y = Trig_Cos(angle) * speed;
                runtime->particles[index].z =
                    ((u32)Random16() % 200 - 100) << 16;
                runtime->particles[index].variant = 0;
                index++;
            } while (index != 32);

            for (index = 0; index != runtime->effect->count; index++) {
                radius[index] = 16;
                spin[index] = 6000;
            }
        }

        if (frame > 159) {
            scale = BattleFx_StarsUnitScale;
            size = 16;
            if (frame - 160 <= 64)
                size = 32;

            wave.x -= (Trig_Sin((frame - 160) << 7) * 32) >> 6;
            wave.y += (Trig_Sin((frame - 160) << 9) * size) >> 6;
            wave.x += drift.x;
            wave.y += drift.y;
            drift.x /= 2;
            drift.y /= 2;

            scale.x = 0x10000;
            scale.y = 0x10000;
            place.unknown_0c = 0;
            index = 0;
            do {
                place.x = (Data_080ee15a[index] << 16) + wave.x + 0x280000;
                place.y = 0xff0000 + index * 0x400000;
                place.z =
                    ((Data_080ee163[index] + index * 64) << 16) + wave.y;
                Object_ApplyProjectedPlacementFar(runtime->objects[index], &place, &scale, 0);
                index++;
            } while (index != 9);

            if (frame > 255) {
                s32 threshold;

                shake.z = 0x1000000;
                alive = 0;
                shake.x = 0;
                shake.y = 0;
                Render_ResetTransformState();
                SceneTransform_ApplyPosition(&shake);

                if (frame <= 329) {
                    index = 0;
                    do {
                        s32 xx;
                        s32 yy;
                        s32 zz;

                        point = &runtime->particles[index];
                        xx = (point->x >> 8) * (point->x >> 8);
                        yy = (point->y >> 8) * (point->y >> 8);
                        zz = (point->z >> 8) * (point->z >> 8);
                        dist = Iwram_Sqrt(xx + yy + zz) >> 9;
                        if (dist != 0) {
                            EffectPosition_ApplyBaseAndYOffset(&point->x, &screen);
                            screen.x =
                                (screen.x >> 17) + (wave.x >> 17) + 32;
                            screen.y = (screen.y >> 16)
                                + (wave.y >> 16) - 4;
                            screen.depth >>= 16;
                            if (screen.depth <= 169)
                                screen.depth = 170;
                            if (screen.depth > 350)
                                screen.depth = 350;
                            span = 6 - (screen.depth - 170) / 36;
                            draw.rect_alt(
                                draw_destination,
                                graphics + ParticleStreams_CellOffsets[span - 1],
                                screen.x - span / 2,
                                screen.y - span,
                                span,
                                span * 2);
                            point->x -=
                                point->x / dist;
                            point->y -=
                                point->y / dist;
                            point->z -=
                                point->z / dist;
                        } else {
                            alive++;
                        }
                        index++;
                    } while (index != 32);

                    if (alive > 0) {
                        span = alive / 10 + 1;
                        draw.rect_alt(
                            draw_destination,
                            graphics + ParticleStreams_CellOffsets[span - 1],
                            (wave.x >> 17) - span / 2 + 32,
                            (wave.y >> 16) - span - 4,
                            span,
                            span * 2);
                    }
                }

                slot = 0;
                do {
                    if (frame == Data_080ee16c[slot]) {
                        s32 speed;
                        s32 angle;

                        index = 0;
                        do {
                            speed = (Random16() & 127) + 16;
                            angle = Random16() & 0xffff;
                            spark = &((struct EffectStep *)Ram_MapCellBuffer)[slot * 128 + index];
                            spark->velocity_x = (Trig_Sin(angle) * speed) >> 6;
                            spark->velocity_y = -(Trig_Cos(angle) * speed) >> 6;
                            spark->velocity_y = ((Random16() & 255) - 128) << 10;
                            spark->x = 0;
                            spark->y = 0;
                            spark->z = 0;
                            spark->variant = (Random16() & 15) + 64;
                            index++;
                        } while (index != 96);
                    }

                    if (frame >= Data_080ee16c[0]) {
                        spark = ((struct EffectStep *)Ram_MapCellBuffer);
                        index = 0;
                        do {
                            if (spark[index].variant > 0) {
                                EffectPosition_ApplyBaseAndYOffset(&spark[index].x, &screen);
                                screen.x = (screen.x >> 17) + 32;
                                screen.y = (screen.y >> 16) + 56;
                                screen.depth >>= 16;
                                if (screen.depth <= 169)
                                    screen.depth = 170;
                                if (screen.depth > 350)
                                    screen.depth = 350;
                                span =
                                    3 - (screen.depth - 170) / 90;
                                draw.rect(
                                    draw_destination,
                                    graphics + ParticleStreams_CellOffsets[span - 1],
                                    screen.x - span / 2,
                                    screen.y - span,
                                    span,
                                    span * 2);
                                spark[index].x += spark[index].velocity_x;
                                spark[index].y += spark[index].velocity_y;
                                spark[index].z += spark[index].velocity_y;
                                spark[index].variant--;
                            }
                            index++;
                        } while (index != 512);
                    }
                    slot++;
                } while (slot != 4);

                index = 0;
                do {
                    mark = &runtime->particles[index + 60];
                    threshold = index * 12 + 330;
                    if (frame == threshold) {
                        mark->x = (wave.x >> 17) + 32;
                        mark->y = (wave.y >> 16) - 4;
                        mark->velocity_x = (wave.x >> 17) + 32;
                        mark->velocity_y = (wave.y >> 16) - 4;

                        slot = 0;
                        do {
                            runtime->particles[slot + 32].variant = 4;
                            slot++;
                        } while (slot != 28);

                        drift.x = 0x80000;
                        drift.y = -0x20000;
                        runtime->shake_frames = 8;
                    }

                    if (frame >= threshold) {
                        s32 size;

                        size = (frame - threshold) / 2;
                        if (size > 2)
                            size = 2;
                        draw.rect_alt(
                            draw_destination,
                            (u8 *)runtime + Data_080ee17e[size],
                            mark->x,
                            mark->y - Data_080ee17a[size],
                            Data_080ee174[size],
                            Data_080ee177[size]);
                        mark->x -= 8;
                        mark->y += 2;

                        if (frame < threshold + 8) {
                            SceneTransform_ApplyRoll(-2048);
                            SceneTransform_ApplyYaw(-4096);

                            slot = 0;
                            spark = &runtime->particles[32];
                            do {
                                struct EffectStep *step;

                                step = &spark[slot];
                                work.x = 0;
                                work.y = step->variant
                                    * Trig_Cos(slot * 2340);
                                work.z = step->variant
                                    * Trig_Sin(slot * 2340);
                                step->variant += 2;
                                EffectPosition_ApplyBaseAndYOffset(&work.x, &screen);
                                screen.x =
                                    (screen.x >> 17) + runtime->particles[index + 60].velocity_x;
                                screen.y = (screen.y >> 16)
                                    + runtime->particles[index + 60].velocity_y;
                                screen.depth >>= 16;
                                if (screen.depth <= 169)
                                    screen.depth = 170;
                                if (screen.depth > 350)
                                    screen.depth = 350;
                                span = 3
                                    - (screen.depth - 170) / 90;
                                draw.rect(
                                    draw_destination,
                                    graphics + ParticleStreams_CellOffsets[span - 1],
                                    screen.x - span / 2,
                                    screen.y - span,
                                    span,
                                    span * 2);
                                slot++;
                            } while (slot != 28);
                        }
                    }
                    index++;
                } while (index != 4);
            }
        }

        ObjectGroup_TickMemberTimers();
        runtime->transfer_pending = 1;
        WaitFrames(1);
        frame++;
    } while (frame != 400);

    BattleEventRuntime_BeginPhaseFar(134);

    for (index = 0; index != runtime->effect->count; index++) {
        struct MotionObject *obj;

        context = GetBattleObjectSlotFar(runtime->effect->actors[index]);
        obj = (struct MotionObject *)context->object;
        obj->x = home[index].x;
        obj->z = home[index].y;
        obj->angle = base_angle[index];
    }

    gProjection.center_x = 120;

    BattleEffect_SetupBlendedDisplay();

    index = 0;
    do {
        ResourceObject_ReleaseFar((struct ResourceObjectWork *)runtime->objects[index]);
        index++;
    } while (index != 9);

    Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

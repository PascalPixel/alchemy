#include "BATTLE_PRESENTATION.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "B5_CONTEXT.H"
#include "MOTION_OBJECT.H"
#include "RESOURCE_IDS.H"

extern u8 gBattleFxWork[];

void BattleFx_BeginCanvasLayer(s32 mode);
s32 BattleFx_EndCanvasLayer(void);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
struct B5Context *GetBattleObjectSlotFar(s32 member_id);
s32 Battle_GetObjectTableValueFar(s32 member_id);
void Camera_ApplyShake(s32 x, s32 y);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void Audio_PlayCue(s32 cue);
void Object_ResetMotion(struct MotionObject *object);
void Object_SetPosition(struct MotionObject *object, s32 x, s32 y, s32 z);
void Object_SetMode(struct MotionObject *object, s32 mode);

/* Battle effect: the actor dashes 90% of the way to the first affected unit;
   both hop (frame 0), are raised by their object-table offsets and flipped
   at frame 11, and the actor springs back at frame 54 while the flash burst
   sheet is drawn over it; from frame 56 sixteen burst cells ring the actor
   on a widening circle for twelve frames, and at frame 64 the offsets and
   flips are undone. The screen shakes throughout. */
void BattleFx_RunFlippingBurst(struct BattleEffectArgument *effect)
{
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    /* FAKEMATCH: a blitter pair of which only draw[0] is used; the unused
       second slot is the spare word in the reference frame. */
    DrawRectangleFn draw[2]; /* FAKEMATCH: unused second slot, see above */
    struct B5Context *first;
    struct B5Context *second;
    struct MotionObject *object;
    struct MotionObject *target;
    s32 x;
    s32 z;
    s32 lift_actor;
    s32 lift_member;
    s32 record[3];
    struct EffectPosition screen;
    s32 frame;
    s32 scale;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work, 1, 1);
    BattleEffect_LoadWork(46, 7, 7, 3, 2);
    work->transfer_mode = 2;
    draw[0] = heap_cache[7];
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    first = GetBattleObjectSlotFar(work->effect->actor);
    second = GetBattleObjectSlotFar(work->effect->actors[0]);
    object = first->object;
    target = second->object;
    scale = 90;
    x = object->x + scale * (target->x - object->x) / 100;
    z = object->z + scale * (target->z - object->z) / 100;
    lift_actor = Battle_GetObjectTableValueFar(work->effect->actor);
    lift_member = Battle_GetObjectTableValueFar(work->effect->actors[0]);
    Object_ResetMotion(object);
    Object_SetPosition(object, x, 0, z);
    Object_SetMode(object, 2);
    object->snap_to_target = 1;
    object->auto_face_motion = 1;
    object->acceleration = 0x20000;
    object->speed_limit = 0x80000;
    WaitFrames(20);
    for (frame = 0; frame != 96; frame++) {
        struct BattleCamera *camera;

        camera = gCameraWork;
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
        if (frame == 0) {
            target->velocity_y = 0xf0000;
            target->vertical_motion_strength = 0x91eb;
            object->velocity_y = 0xf0000;
            object->vertical_motion_strength = 0x91eb;
        }
        if (frame == 11) {
            target->scale_y = -target->scale_y;
            object->scale_y = -object->scale_y;
            object->y += lift_actor;
            target->y += lift_member;
        }
        if (frame == 54) {
            ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 10);
            target->velocity_y = 0x80000;
            target->vertical_motion_strength = 0xab85;
            object->velocity_y = 0x50000;
            object->vertical_motion_strength = 0x7851;
            object->acceleration = 0x10000;
            object->speed_limit = 0x20000;
            object->auto_face_motion = 0;
            Object_ResetMotion(object);
            Object_SetPosition(object, 0, 0, object->z);
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
        record[0] = object->x;
        record[1] = object->y;
        record[2] = object->z;
        EffectPosition_ApplyBaseAndYOffset(record, &screen);
        screen.x >>= 1;
        if (frame == 54 || frame == 55)
            draw[0](canvas, work, screen.x - 16, screen.y - 16, 32, 64);
        if (frame >= 56 && frame <= 67) {
            s32 i;

            for (i = 0; i != 16; i++) {
                s32 angle;
                s32 rx;
                s32 ry;

                angle = i << 12;
                rx = screen.x + ((Trig_Sin(angle) * (frame - 46)) >> 16);
                ry = ((Trig_Cos(angle) * (frame - 46)) >> 16) - frame + 100;
                draw[0](canvas, (u8 *)work + (((frame - 56) / 2) << 11), rx - 16, ry, 32, 64);
            }
        }
        if (frame == 64) {
            target->scale_y = -target->scale_y;
            object->scale_y = -object->scale_y;
            object->y -= lift_actor;
            target->y -= lift_member;
            Object_SetMode(object, 0);
        }
        if (frame == 54)
            BattleEventRuntime_BeginPhaseFar(134);
        if (frame == 0) {
            Audio_PlayCue(136);
            work->shake_frames = 6;
        }
        if (frame == 53)
            work->shake_frames = 6;
        Camera_ApplyShake(16, 16);
        work->transfer_pending = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

void ObjectGroup_TickMemberTimers(void);

/* Per drift step 0..3: a blade cell's width and height and its offset in
   the sword slash sheet. */
extern const u8 BladeRain_CellWidths[];
extern const u8 BladeRain_CellHeights[];
extern const u16 BladeRain_CellSourceOffsets[];

/* Battle effect: a rain of blades. Sixty-four blades are seeded at random
   columns with a sideways drift set by the column; each of 120 frames draws
   sixteen of them, falling once their turn comes and then shrinking, while
   the screen shakes and the first target is hit every fourth frame from
   frame 23 to 87. A blade drifting left is drawn by the second blitter. */
void BattleFx_RunBladeRain(struct BattleEffectArgument *efx)
{
    struct EffectPosition position;
    DrawRectangle draw[2];
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    DrawRectangle *blit;
    struct EffectStep *blade;
    s32 frame;
    s32 i;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    work->effect = efx;
    BattleFx_BeginCanvasLayer(1);
    *(u16 *)0x04000020 = 0x100;
    *(u16 *)0x04000052 = 0x1000;
    BattleEffect_LoadWork(46, 7, 7, 3, 1);
    draw[0] = (DrawRectangle)heap_cache[46 - 39];
    BattleEffect_LoadWork(47, 7, 7, 7, 1);
    draw[1] = (DrawRectangle)heap_cache[47 - 39];
    blit = draw;
    Resource_LoadAndDecompress((s32)&ResourceId_SwordSlashSheet, work, 1, 1);
    work->transfer_mode = 1;
    work->transfer_value = 0;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &position);
    *(s32 *)0x04000028 = (64 - position.x) << 8;
    for (i = 0; i != 64; i++) {
        s32 column;

        blade = &work->particles[i];
        column = Random16() % 96 + 16;
        blade->x = column;
        blade->y = (24 - i / 4) << 16;
        if (column <= 43)
            blade->velocity_x = 3;
        else if (column <= 51)
            blade->velocity_x = 2;
        else if (column <= 59)
            blade->velocity_x = 1;
        else if (column <= 67)
            blade->velocity_x = 0;
        else if (column <= 75)
            blade->velocity_x = -1;
        else if (column <= 83)
            blade->velocity_x = -2;
        else
            blade->velocity_x = -3;
        blade->velocity_x <<= 17;
        blade->velocity_y = 0x80000;
        blade->x <<= 16;
    }
    Audio_PlayCue(212);
    for (frame = 0; frame != 120; frame++) {
        if (frame <= 16) {
            *(u16 *)0x04000052 = frame | 0x1000;
            if (frame == 16)
                *(u16 *)0x04000050 = 0;
        }
        if (frame > 103) {
            *(u16 *)0x04000052 = (0x78 - frame) | 0x1000;
            if (frame == 104)
                *(u16 *)0x04000050 = 0x3f44;
        }
        for (i = 15; i != -1; i--) {
            /* The blitter index, drift >> 31, is computed before the call's
               operands, so the allocator ranks the drift above the width and
               height and rotates their three registers. */
            /* FAKEMATCH: the drift is pinned to r6, as the reference keeps it. */
            register s32 drift __asm__("r6");
            s32 speed;
            s32 cell;
            u32 wide;
            u32 high;

            blade = &work->particles[i];
            drift = blade->velocity_x;
            speed = drift;
            if (drift < 0)
                speed = -drift;
            cell = speed >> 17;
            if (frame < i * 4 + 25) {
                blit[(u32)drift >> 31](canvas, (u8 *)work + BladeRain_CellSourceOffsets[cell],
                    (blade->x >> 16) - ((wide = BladeRain_CellWidths[cell]) >> 1),
                    (blade->y >> 16) - ((high = BladeRain_CellHeights[cell]) >> 1),
                    wide, high);
                if (frame >= i * 4 + 16) {
                    blade->x += blade->velocity_x;
                    blade->y += blade->velocity_y;
                }
            } else {
                blit[(u32)drift >> 31](canvas, (u8 *)work + BladeRain_CellSourceOffsets[cell],
                    (blade->x >> 16) - ((wide = BladeRain_CellWidths[cell]) >> 1),
                    (blade->y >> 16) - ((high = BladeRain_CellHeights[cell]) >> 1),
                    wide, high -= 4);
            }
        }
        if (frame >= 23 && frame <= 87 && (frame & 3) == 0) {
            ObjectGroup_UpdateMembers(work->effect->actors[0], 7, 5, 0, 2);
            work->shake_frames = 1;
            if ((frame & 7) == 0)
                Audio_PlayCue(133);
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

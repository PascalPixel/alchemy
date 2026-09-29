#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "B5_CONTEXT.H"
#include "MOTION_OBJECT.H"
#include "BATTLE_EFX.H"

/*
 * Draft for the battle-presentation sub-effect at 0x080cb1a4.
 *
 * 2026-09-26: callback pair gives 840/840 bytes, 101 differing halfwords
 * (96 aligned edits), down from 108/100 with one callback. The unused
 * callback slot accounts for four of the reference's twelve extra frame
 * bytes: ours is 76, reference 84. The reference also spills record_ptr
 * and screen_ptr, whereas ours rematerialises their stack addresses.
 * Volatile pointers produce the 84-byte frame but add twelve code bytes
 * and worsen the alignment to 151 edits; they are not kept. Byte views
 * of snap_to_target/auto_face_motion leave the frame unchanged and worsen
 * the original candidate to 836 bytes/131 aligned edits. Unlike the linked
 * overlay family, these plain assignments do not create a dead QImode
 * zero. The first register divergence remains heap_cache r6 versus r7;
 * the effect-state address takes r8 versus sl. These three bounded trials
 * did not explain that allocation difference; don't repeat them unchanged.
 *
 * Assigned from the member_orbit/run.c compiler-family cluster
 * (template-main-080ce85c, games/THE BROKEN SEAL/SRC/BATTLE/EFFECT/MEMBER_ORBIT.C),
 * but the real callee set and constants match the 0x03001eec "battle work"
 * subsystem documented there and in games/THE BROKEN SEAL/src/battle/effects/puff_arc/
 * run.c and recon/tbs/en/main/080e01e4.c: same BattleFx_BeginCanvasLayer(mode) /
 * Resource_LoadAndDecompress((s32)&Value_XXXXXXXX, work, f, f) / work-offsets
 * 0x7780/0x7784/0x7824/0x7828 shape, the same 96-pass outer loop as
 * 080e01e4.c, and the same 0x7780=2 / 0x7784=75 pair as puff_arc's
 * WORK_EFX->layers==2 branch.
 *
 * Unlike any of those siblings, this owner also drives a live
 * `struct MotionObject` pair through Object_ResetMotion/Object_SetPosition/
 * Object_SetMode (Object_ResetMotion/Object_SetPosition/Object_SetMode, aliased in
 * types.h) and FixedPoint_Ratio (Math_Div) -- the same low-level shape
 * games/THE BROKEN SEAL/src/battle/motion/set_approach_motion.c uses, but inlined here
 * with its own scale (90, not 80) and without that function's
 * snap_to_target/vertical_motion_strength/acceleration/speed_limit tail,
 * which this owner instead assigns directly on specific frames. The two
 * `struct B5Context` lookups key off the effect state's field 8 (the
 * source actor, named `actor` in puff_arc's Efx struct) and the first
 * entry of its member-id array at field 0x24 (36) -- the field this whole
 * family already uses for its per-member id list.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))



void BattleFx_BeginCanvasLayer(s32 mode);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 interval);
void Scheduler_RemoveCallback(void *callback);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
s32 Battle_GetObjectTableValueFar(s32 member_id);
void EffectPosition_ApplyBaseAndYOffset(void *source, void *screen);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void WaitFrames(s32 frames);
void Runtime_ReleaseHeapBlock(s32 id);
s32 BattleFx_EndCanvasLayer(void);
void Camera_ApplyShake(s32 a, s32 b);
void BattleEventRuntime_BeginPhaseFar(s32 id);
void Audio_PlayCue(s32 id);

void Unnamed_080cb1a4(void *object_param)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *canvas;
    s32 status;
    /* FAKEMATCH: callback pair preserves the reference's unused stack slot. */
    DrawRectangleFn draw[2];
    struct B5Context *first_context;
    struct B5Context *second_context;
    struct MotionObject *object;
    struct MotionObject *target;
    s32 x;
    s32 z;
    s32 y_offset_actor;
    s32 y_offset_member;
    s32 record[3];
    s32 screen[3];
    s32 *record_ptr;
    s32 *screen_ptr;
    s32 frame;
    s32 cel_phase;
    s32 radius;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    M2C_FIELD(work, void **, 0x7828) = object_param;
    BattleFx_BeginCanvasLayer(0);
    Resource_LoadAndDecompress((s32)&ResourceId_FlashBurstSheet, work, 1, 1);

    status = BattleEffect_LoadWork(46, 7, 7, 3, 2);
    M2C_FIELD(work, s32 *, 0x7780) = 2;
    draw[0] = (DrawRectangleFn)heap_cache[7];
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    Scheduler_AddOrUpdateCallback((void *)0x080CD261, 0x480);

    first_context = GetBattleObjectSlotFar(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 8));
    second_context = GetBattleObjectSlotFar(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36));
    object = first_context->object;
    target = second_context->object;

    x = object->x + FixedPoint_Ratio(90 * (target->x - object->x), 100);
    z = object->z + FixedPoint_Ratio(90 * (target->z - object->z), 100);

    y_offset_actor = Battle_GetObjectTableValueFar(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 8));
    y_offset_member = Battle_GetObjectTableValueFar(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36));

    Object_ResetMotion(object);
    Object_SetPosition(object, x, 0, z);
    Object_SetMode(object, 2);

    object->snap_to_target = 1;
    object->auto_face_motion = 1;
    object->acceleration = 0x20000;
    object->speed_limit = 0x80000;

    WaitFrames(20);
    frame = 0;

    record_ptr = record;
    screen_ptr = screen;

    for (cel_phase = -56, radius = -46; frame != 96;
            frame++, cel_phase++, radius++) {
        s32 facing;

        facing = *(s32 *)0x03001E80;
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(facing, facing + 12);

        if (frame == 0) {
            target->velocity_y = 0xF0000;
            target->vertical_motion_strength = 0xAB85;
            object->velocity_y = 0xF0000;
            object->vertical_motion_strength = 0xAB85;
        }
        if (frame == 11) {
            M2C_FIELD(target, s32 *, 0x1C) = -M2C_FIELD(target, s32 *, 0x1C);
            M2C_FIELD(object, s32 *, 0x1C) = -M2C_FIELD(object, s32 *, 0x1C);
            object->y += y_offset_actor;
            target->y += y_offset_member;
        }
        if (frame == 54) {
            ObjectGroup_UpdateMembers(
                M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36),
                7, 5, 0, 10);
            target->velocity_y = 0x80000;
            target->vertical_motion_strength = 0x91EB;
            object->velocity_y = 0x50000;
            object->vertical_motion_strength = 0x7851;
            object->acceleration = 0x10000;
            object->speed_limit = 0x20000;
            object->auto_face_motion = 0;
            Object_ResetMotion(object);
            Object_SetPosition(object, 0, 0, object->z);
        }

        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(facing, facing + 12);

        record_ptr[0] = object->x;
        record_ptr[1] = object->y;
        record_ptr[2] = object->z;
        EffectPosition_ApplyBaseAndYOffset(record_ptr, screen_ptr);
        screen_ptr[0] = screen_ptr[0] >> 1;

        if (frame == 54 || frame == 55) {
            draw[0](canvas, work, screen_ptr[0] - 16, screen_ptr[1] - 16,
                32, 64);
        }

        if (cel_phase >= 0 && cel_phase <= 11) {
            s32 i;
            s32 offset;

            offset = (cel_phase / 2) << 11;
            for (i = 0; i != 16; i++) {
                s32 angle;
                s32 rx;
                s32 ry;

                angle = i << 12;
                rx = (screen_ptr[0]
                    + ((radius * Trig_Sin(angle)) >> 16)) - 16;
                ry = (radius * Trig_Cos(angle) >> 16) - frame + 100;
                draw[0](canvas, (u8 *)work + offset, rx, ry, 32, 64);
            }
        }

        if (frame == 64) {
            M2C_FIELD(target, s32 *, 0x1C) = -M2C_FIELD(target, s32 *, 0x1C);
            M2C_FIELD(object, s32 *, 0x1C) = -M2C_FIELD(object, s32 *, 0x1C);
            object->y -= y_offset_actor;
            target->y -= y_offset_member;
            Object_SetMode(object, 0);
        }

        if (frame == 54) {
            BattleEventRuntime_BeginPhaseFar(134);
        }
        if (frame == 0) {
            Audio_PlayCue(136);
            M2C_FIELD(work, s32 *, 0x77A8) = 6;
        }
        if (frame == 53) {
            M2C_FIELD(work, s32 *, 0x77A8) = 6;
        }

        Camera_ApplyShake(16, 16);
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((void *)0x080CD261);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

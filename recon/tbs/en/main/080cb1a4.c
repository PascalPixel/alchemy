/* DRAFT: Independent S2 audit of the former RunFlippingBurst adoption.
 * Kept ordinary form: one saved blitter, no unread reservation; T10 setup.
 * Remaining score 45: five stack operands and two projection offsets. Complete
 * compiled extent remains 840 bytes, including its 44-byte literal pool.
 * The previous source compared exactly in all six TBS editions, but draw[2]
 * reserved one unused word solely to reproduce an 84-byte local frame.
 * All six editions now leave this complete 840-byte extent uncredited.
 * The existing neighbouring BladeRain source stays in FLIPPING_BURST.C.
 *
 * Own EN ROM at 080cb1a4: draw is stored at sp+48, canvas at sp+56,
 * screen at sp+60 (three words), and record at sp+72 (three words).
 * No instruction accesses sp+52 or passes its address. The only local
 * addresses passed out are the two three-word projection buffers.
 * EffectPosition_ApplyBaseAndYOffset -> Render_ProjectPoint -> resident
 * IwramTransformVector reads and writes three words; no fourth word exists
 * in that contract. The callback receives only a function and priority.
 * BattleEffect_LoadWork allocates its own heap block (slot 46 here), with
 * no local-frame pointer. Both drawing call sites use the saved heap[7]
 * routine. The shared work stores the input effect, not a local blitter pair.
 * BladeRain independently creates slots 46/47 and consumes both pointers;
 * its pair does not prove this function owns a second one.
 *
 * T0: compare the unchanged two-slot body with current verified compiler
 * assembly: score 1: raw assembler filled its pool alignment with a NOP,
 * where the owned ROM has two zero alignment bytes. The restored raw
 * listing uses explicit zero alignment; no executable instruction changed.
 * T1: scalar draw, declaration unchanged: score 245 (5 stack-only,
 * 6 operand, 2 reordered). Local frame shrinks 84 -> 80, canvas moves
 * 56 -> 52, screen 60 -> 56, record 72 -> 68; blitter stays at 48.
 * Initialization scheduling changes, but no instruction is added/removed.
 * T2: one-element array is identical to the scalar trial, score 245.
 * T3: scalar declaration after screen is score 255 (15 stack-only,
 * 6 operand, 2 reordered); restore the simpler T1 declaration order.
 * T0-T14 were scored against the verified module assembly, independently
 * checked against the own-ROM instructions. The retained draft now uses
 * the full raw listing restored alongside it; this is not an adoption.
 *
 * T4 canvas/blitter cache: 327; T5 actor/target lift pair: 315;
 * T6 input/output projection record: 1108; T7 destination x/z: 253.
 * All aggregates contained only consumed fields and kept an 80-byte frame.
 * T8 projection locals scoped to frame loop: 245.
 * T9 movement locals scoped to initialization: 253.
 * T10-T14 setup ordering (mode, blitter, transfer value):
 * 021:45; 102:205; 120:670; 201:455; 210:550. Retain T10: configure
 * both transfer fields, then cache the blitter. All scheduling now agrees;
 * only the seven stack/frame operands differ from the complete raw extent.
 *
 * Audit conclusion: no producer/consumer evidence establishes a genuine
 * shared two-slot object. The maintained unused word changes frame storage,
 * exceeding S2 instruction-order/register-choice steering. This scalar
 * draft awaits a natural exact source model; do not substitute a padded
 * struct, volatile spill or instruction that recreates the reservation.
 */
#include "CANVAS.H"
#include "RUNTIME_MEM.H"
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

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
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
    DrawRectangleFn draw;
    struct BattleObjectSlot *first;
    struct BattleObjectSlot *second;
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
    work->transfer_value = 75;
    draw = heap_cache[7];
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
            draw(canvas, work, screen.x - 16, screen.y - 16, 32, 64);
        if (frame >= 56 && frame <= 67) {
            s32 i;

            for (i = 0; i != 16; i++) {
                s32 angle;
                s32 rx;
                s32 ry;

                angle = i << 12;
                rx = screen.x + ((Trig_Sin(angle) * (frame - 46)) >> 16);
                ry = ((Trig_Cos(angle) * (frame - 46)) >> 16) - frame + 100;
                draw(canvas, (u8 *)work + (((frame - 56) / 2) << 11), rx - 16, ry, 32, 64);
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

/*
 * RunBattleEffect13 — unmatched draft; complete native extent is 588 bytes.
 * The earlier scorer improved 2590 to 1595 using loop-local step limits,
 * an inline scale helper, upward wait counters and a byte-zero temporary.
 * Its remaining differences were reload registers and origin/target pointer
 * copies. That scheduling search is closed. This ordinary owner/contract
 * baseline removes the helper, loop-local limits and zero temporary; the
 * upward wait loops remain ordinary bounded loops. No device is retained.
 * The native reset call supplies both X and Z as -0x90000, with the child's
 * current Y between them; the earlier three-argument declaration missed Z.
 * T0: one natural typed compile emitted 608 against native 588 bytes,
 * including six identical resolved pool words in the same order. Native
 * normalization reproduces all 588 own-ROM bytes. The candidate differs at
 * 553 positions in that span, first +0x0e, and has 20 extra bytes; this is
 * a positional comparison, not an aligned score. All 30 relocations resolve
 * (29 calls, one scene pointer); physical call targets and order agree.
 * Local frame 44 versus 40, saved area 32 both, total stack 76 versus 72;
 * origin/target/spawn move to sp+20/+8/+32 from native +16/+4/+28.
 * A used origin-pointer spill and scale accumulators replace native pointer
 * lifetimes and per-step constant multiplication. Instructions 261/253,
 * loads 43/41, stores 29/28, branches 10 both; natural padding 4/0 bytes.
 * No follow-up form, new device or adoption; reload/scheduling search stays
 * closed and the draft remains uncredited.
 */
#include "TYPES.H"
#include "FX_SCENE.H"
#include "MOTION_OBJECT.H"
#include "FIXED_POINT_POSITION.H"
#include "OBJECT_RUNTIME.H"
#include "OBJDISP.H"
#include "SYSTEM.H"

extern struct BattleFxScene *gEffectWork;

void Vector_AddPolarOffset(s32, s32, s32 *);
void Object_SetMode(struct ObjectRuntime *, s32);
void ObjectDispatch_ReleaseFar(struct DispatchObject *);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *, s32, s32, s32);
void Animation_ApplyChildValuesFar(struct DispatchObject *, u32);
struct ObjectRuntime *Object_Spawn(s32, s32, s32, s32);
void BattleEffect_InitializeSharedScene(void);
void BattleFx_PrepareBufferInterpolation(void);
void AudioCommand_PlayFar(s32);

void RunBattleEffect13(void)
{
    struct BattleFxScene *scene = gEffectWork;
    struct ObjectRuntime *secondary = scene->child;
    struct MotionObject *main_object = scene->main_object;
    struct MotionObject *object;
    struct FixedPointPosition spawn;
    struct FixedPointPosition origin;
    struct FixedPointPosition target;
    s32 step;
    s32 index;

    origin.x = main_object->x;
    origin.y = main_object->y + 0x100000;
    origin.z = main_object->z;
    if (scene->enabled) {
        target.x = main_object->x;
        target.y = main_object->y + 0x200000;
        target.z = main_object->z;
        Vector_AddPolarOffset(0x200000, scene->angle, &target.x);
    } else {
        target.x = scene->x;
        target.y = scene->y + 0x200000;
        target.z = scene->z;
    }
    spawn.x = scene->x;
    spawn.y = scene->y + 0x200000;
    spawn.z = scene->z;
    object = (struct MotionObject *)Object_Spawn(0xd7, spawn.x, spawn.y, spawn.z);
    if (object == 0)
        return;
    BattleEffect_InitializeSharedScene();
    AudioCommand_PlayFar(0x8a);
    object->angle = main_object->angle;
    object->speed_limit = 0x14ccc;
    object->motion_flags = 0;
    Object_SetMode((struct ObjectRuntime *)object, 5);
    Animation_ApplyChildValuesFar((struct DispatchObject *)object, 1);
    for (step = 0; step < 11; step++) {
        s32 scale;

        object->x = origin.x + (target.x - origin.x) * step / 10;
        object->y = origin.y + (target.y - origin.y) * step / 10;
        object->z = origin.z + (target.z - origin.z) * step / 10;
        scale = 0x4000 + (0x10000 - 0x4000) * step / 10;
        object->scale_x = scale;
        object->scale_y = scale;
        WaitFrames(1);
    }
    WaitFrames(10);
    Object_SetMode((struct ObjectRuntime *)object, 6);
    WaitFrames(15);
    for (index = 0; index < 10; index++) {
        object->y -= 0x20000;
        WaitFrames(1);
    }
    Object_SetMode((struct ObjectRuntime *)object, 5);
    AudioCommand_PlayFar(0x84);
    if (secondary != 0)
        Object_SetPositionAndResetMotionFar(secondary, -0x90000, secondary->y,
            -0x90000);
    WaitFrames(20);
    for (index = 0; index < 13; index++) {
        object->y += 0x18000;
        WaitFrames(1);
    }
    WaitFrames(10);
    AudioCommand_PlayFar(0x72);
    for (step = 0; step < 11; step++) {
        s32 scale;

        object->x = target.x + (origin.x - target.x) * step / 10;
        object->y = target.y + (origin.y - target.y) * step / 10;
        object->z = target.z + (origin.z - target.z) * step / 10;
        scale = 0x10000 + (0x4000 - 0x10000) * step / 10;
        object->scale_x = scale;
        object->scale_y = scale;
        WaitFrames(1);
    }
    ObjectDispatch_ReleaseFar((struct DispatchObject *)object);
    BattleFx_PrepareBufferInterpolation();
}

#include "types.h"
#include "fx_scene.h"
#include "motobj.h"
#include "fixed_point_position.h"
#include "system.h"

void RunBattleEffect13(void)
{
    struct BattleFxScene *scene = gEffectWork;
    struct MotionObject *main_object = scene->main_object;
    struct MotionObject *secondary = scene->child;
    struct MotionObject *object;
    struct FixedPointPosition spawn;
    struct FixedPointPosition origin;
    struct FixedPointPosition target;
    s32 step;
    s32 index;
    s32 steps = 11;

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
    object = Object_Spawn(0xd7, spawn.x, spawn.y, spawn.z);
    if (object == 0)
        return;
    BattleEffect_InitializeSharedScene();
    AudioCommand_PlayFar(0x8a);
    object->angle = main_object->angle;
    object->speed_limit = 0x14ccc;
    object->motion_flags = 0;
    Object_SetMode(object, 5);
    Animation_ApplyChildValuesFar(object, 1);
    step = 0;
    do {
        s32 scale;

        object->x = origin.x + (target.x - origin.x) * step / 10;
        object->y = origin.y + (target.y - origin.y) * step / 10;
        object->z = origin.z + (target.z - origin.z) * step / 10;
        scale = BattleFx_Interpolate(0x10000, 0x4000, step);
        object->scale_x = scale;
        object->scale_y = scale;
        WaitFrames(1);
        step++;
    } while (step < steps);
    WaitFrames(10);
    Object_SetMode(object, 6);
    WaitFrames(15);
    for (index = 0; index < 10; index++) {
        object->y -= 0x20000;
        WaitFrames(1);
    }
    Object_SetMode(object, 5);
    AudioCommand_PlayFar(0x84);
    if (secondary != 0)
        Object_SetPositionAndResetMotionFar(secondary, -0x90000, secondary->y, -0x90000);
    WaitFrames(20);
    for (index = 0; index < 13; index++) {
        object->y += 0x18000;
        WaitFrames(1);
    }
    WaitFrames(10);
    AudioCommand_PlayFar(0x72);
    step = 0;
    do {
        s32 scale;

        object->x = target.x + (origin.x - target.x) * step / 10;
        object->y = target.y + (origin.y - target.y) * step / 10;
        object->z = target.z + (origin.z - target.z) * step / 10;
        scale = BattleFx_Interpolate(0x4000, 0x10000, step);
        object->scale_x = scale;
        object->scale_y = scale;
        WaitFrames(1);
        step++;
    } while (step < steps);
    ObjectDispatch_ReleaseFar(object);
    BattleFx_PrepareBufferInterpolation();
}

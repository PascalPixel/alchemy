/* Near miss, differ 8 (four swapped pairs), on this game's structs with the scale
 * interpolation as an inline helper (called out of line or written in place, the
 * compiler strength-reduces step * 0xc000 and the frame grows; inline it matches).
 * Left: (1) in the else branch and at the spawn position the native starts the
 * 0x200000 constant, loads y, then shifts (movs, ldr, lsls); this draft loads first.
 * Both loads follow a store through a non-stack pointer; the two y loads that follow a
 * stack store match. (2) in both loops the native stores object->y before it copies the
 * position pointer for z; this draft copies first. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

struct FxScene13 {
    u16 angle;
    u8 unknown_02[2];
    s32 x;
    s32 y;
    s32 z;
    struct ObjectRuntime *main_object;
    struct ObjectRuntime *child;
    u8 unknown_18[8];
    s8 enabled;
};

struct FxPosition13 {
    s32 x;
    s32 y;
    s32 z;
};

void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void BattleEffect_InitializeSharedScene(void);
void Animation_ApplyChildValuesFar(struct ObjectRuntime *object, s32 value);
void WaitFrames(s32 frames);
void Object_SetPositionAndResetMotionFar(struct ObjectRuntime *object, s32 x, s32 y, s32 z);
void Object_Destroy(struct ObjectRuntime *object);
void BattleFx_PrepareBufferInterpolation(void);

static __inline__ s32 Interpolate(s32 from, s32 to, s32 step)
{
    return from + (to - from) * step / 10;
}

void Func_080e03cc(void)
{
    struct FxScene13 *scene = Ram_HeapSlots->effect_work;
    struct ObjectRuntime *main_object = scene->main_object;
    struct ObjectRuntime *secondary = scene->child;
    struct ObjectRuntime *object;
    struct FxPosition13 spawn;
    struct FxPosition13 origin;
    struct FxPosition13 target;
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
    object = Object_Spawn(278, spawn.x, spawn.y, spawn.z);
    if (object == 0)
        return;
    BattleEffect_InitializeSharedScene();
    Audio_PlayCue(138);
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
        scale = Interpolate(0x4000, 0x10000, step);
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
    Audio_PlayCue(132);
    if (secondary != 0)
        Object_SetPositionAndResetMotionFar(secondary, -0x90000, secondary->y, -0x90000);
    WaitFrames(20);
    for (index = 0; index < 13; index++) {
        object->y += 0x18000;
        WaitFrames(1);
    }
    WaitFrames(10);
    Audio_PlayCue(114);
    step = 0;
    do {
        s32 scale;

        object->x = target.x + (origin.x - target.x) * step / 10;
        object->y = target.y + (origin.y - target.y) * step / 10;
        object->z = target.z + (origin.z - target.z) * step / 10;
        scale = Interpolate(0x10000, 0x4000, step);
        object->scale_x = scale;
        object->scale_y = scale;
        WaitFrames(1);
        step++;
    } while (step < steps);
    Object_Destroy(object);
    BattleFx_PrepareBufferInterpolation();
}

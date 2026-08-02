#include "layout_guard.h"
#include "types.h"

typedef struct Position_0809ae64 {
    s32 x;
    s32 y;
    s32 z;
} Position_0809ae64;

typedef struct SceneObject_0809ae64 {
    u8 padding00[6];
    u16 facing;
    Position_0809ae64 position;
    u8 padding14[4];
    s32 scale_x;
    s32 scale_y;
    u8 padding20[0x10];
    s32 speed_limit;
    u8 padding34[0x21];
    u8 flags;
} SceneObject_0809ae64;

typedef struct SceneState_0809ae64 {
    s32 direction;
    Position_0809ae64 origin;
    SceneObject_0809ae64 *target;
    SceneObject_0809ae64 *secondary;
    u8 padding18[8];
    s8 approach_from_target;
} SceneState_0809ae64;

LAYOUT_OFFSET_GUARD(
    SceneObject0809ae64_Facing,
    SceneObject_0809ae64,
    facing,
    6);
LAYOUT_OFFSET_GUARD(
    SceneObject0809ae64_Position,
    SceneObject_0809ae64,
    position,
    8);
LAYOUT_OFFSET_GUARD(
    SceneObject0809ae64_ScaleX,
    SceneObject_0809ae64,
    scale_x,
    0x18);
LAYOUT_OFFSET_GUARD(
    SceneObject0809ae64_SpeedLimit,
    SceneObject_0809ae64,
    speed_limit,
    0x30);
LAYOUT_OFFSET_GUARD(
    SceneObject0809ae64_Flags,
    SceneObject_0809ae64,
    flags,
    0x55);
LAYOUT_OFFSET_GUARD(
    SceneState0809ae64_Target,
    SceneState_0809ae64,
    target,
    0x10);
LAYOUT_OFFSET_GUARD(
    SceneState0809ae64_ApproachFromTarget,
    SceneState_0809ae64,
    approach_from_target,
    0x20);

extern SceneState_0809ae64 *Data_03001f30;

s32 Func_080022ec(s32, s32);
void Func_080030f8(u32);
void Func_0800447c(s32, s32, Position_0809ae64 *);
void Func_08009080(SceneObject_0809ae64 *, s32);
void Func_080090d0(SceneObject_0809ae64 *);
void Func_080090f0(SceneObject_0809ae64 *, s32, s32, s32);
void Func_08009240(SceneObject_0809ae64 *, s32);
SceneObject_0809ae64 *Func_08096c80(s32, s32, s32, s32);
void Func_08097384(void);
void Func_0809748c(void);
void Func_080f9010(s32);

static s32 Interpolate_0809ae64(s32 start, s32 end, s32 step)
{
    return start + Func_080022ec(step * (end - start), 10);
}

static void MoveAndScale_0809ae64(
    SceneObject_0809ae64 *object,
    const Position_0809ae64 *start,
    const Position_0809ae64 *end,
    s32 step,
    s32 scale_start,
    s32 scale_delta)
{
    s32 scale;

    object->position.x = Interpolate_0809ae64(start->x, end->x, step);
    object->position.y = Interpolate_0809ae64(start->y, end->y, step);
    object->position.z = Interpolate_0809ae64(start->z, end->z, step);
    scale = scale_start + Func_080022ec(step * scale_delta, 10);
    object->scale_x = scale;
    object->scale_y = scale;
}

/* Run the approach, impact, displacement, and return presentation. */
void Func_0809ae64(void)
{
    SceneState_0809ae64 *scene = Data_03001f30;
    Position_0809ae64 target_position;
    Position_0809ae64 approach_position;
    Position_0809ae64 impact_position;
    SceneObject_0809ae64 *effect;
    s32 frame;

    target_position = scene->target->position;
    target_position.y += 0x100000;

    if (scene->approach_from_target != 0) {
        approach_position = scene->target->position;
        approach_position.y += 0x200000;
        Func_0800447c(
            0x200000,
            scene->direction,
            &approach_position);
    } else {
        approach_position = scene->origin;
        approach_position.y += 0x200000;
    }

    impact_position = scene->origin;
    impact_position.y += 0x200000;
    effect = Func_08096c80(
        0xd7,
        impact_position.x,
        impact_position.y,
        impact_position.z);
    if (effect == 0)
        return;

    Func_08097384();
    Func_080f9010(0x8a);
    effect->facing = scene->target->facing;
    effect->speed_limit = 0x14ccc;
    effect->flags = 0;
    Func_08009080(effect, 5);
    Func_08009240(effect, 1);

    for (frame = 0; frame < 11; frame++) {
        MoveAndScale_0809ae64(
            effect,
            &target_position,
            &approach_position,
            frame,
            0x4000,
            0xc000);
        Func_080030f8(1);
    }

    Func_080030f8(10);
    Func_08009080(effect, 6);
    Func_080030f8(15);
    for (frame = 0; frame < 10; frame++) {
        effect->position.y -= 0x20000;
        Func_080030f8(1);
    }

    Func_08009080(effect, 5);
    Func_080f9010(0x84);
    if (scene->secondary != 0) {
        Func_080090f0(
            scene->secondary,
            -0x90000,
            scene->secondary->position.y,
            -0x90000);
    }

    Func_080030f8(20);
    for (frame = 0; frame < 13; frame++) {
        effect->position.y += 0x18000;
        Func_080030f8(1);
    }

    Func_080030f8(10);
    Func_080f9010(0x72);
    for (frame = 0; frame < 11; frame++) {
        MoveAndScale_0809ae64(
            effect,
            &approach_position,
            &target_position,
            frame,
            0x10000,
            -0xc000);
        Func_080030f8(1);
    }

    Func_080090d0(effect);
    Func_0809748c();
}

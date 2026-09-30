/*
 * Near miss: ☀️'s BattleFx_UpdateObjectMotionScaleAndLinkedAngle
 * (FIELD/COMMON/EFFECT/MOVE.C) with ⚓️'s sprite rotation at +0x12. The same
 * code sits in 49 ⚓️ overlays (the first at resource_653 0x02008080).
 * Remaining difference: one reordered load. ⚓️ loads the sprite pointer
 * third, before the first store; with the build's flags, sched2 places it
 * after the scale_y loads. Compiled with -mtune=arm9tdmi (or strongarm,
 * arm8, arm920t) added, it matches exactly; no C rewrite tried moves it.
 */
#include "TYPES.H"

struct Sprite {
    u8 unknown_00[0x12];
    u16 rotation;
};

struct Effect {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[0x10];
    s32 scale_rate_x;
    s32 scale_rate_y;
    u8 unknown_38[0x0c];
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    struct Sprite *sprite;
    u8 unknown_54[0x10];
    u16 spin;
};

void BattleFx_UpdateObjectMotionScaleAndLinkedAngle(struct Effect *object)
{
    object->x += object->velocity_x;
    object->y += object->velocity_y;
    object->z += object->velocity_z;
    object->scale_x += object->scale_rate_x;
    object->scale_y += object->scale_rate_y;
    object->sprite->rotation += object->spin;
}

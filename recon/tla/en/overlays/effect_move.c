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
    struct Sprite *sprite = object->sprite;
    s32 x = object->x;
    s32 vx = object->velocity_x;

    object->x = x + vx;
    object->y += object->velocity_y;
    object->z += object->velocity_z;
    object->scale_x += object->scale_rate_x;
    object->scale_y += object->scale_rate_y;
    sprite->rotation += object->spin;
}

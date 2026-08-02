#include "layout_guard.h"
#include "types.h"

typedef struct Vec3_08098698 {
    s32 x;
    s32 y;
    s32 z;
} Vec3_08098698;

typedef struct SpriteRecord_08098698 {
    u8 padding00[4];
    u8 attr0_low;
    u8 attr0_high;
    u8 attr1_low;
    u8 attr1_high;
    u8 attr2_low;
    u8 attr2_high;
    u8 padding0a[2];
} SpriteRecord_08098698;

typedef struct SpritePair_08098698 {
    SpriteRecord_08098698 base;
    SpriteRecord_08098698 child;
} SpritePair_08098698;

typedef struct SceneObject_08098698 {
    u8 padding00[8];
    Vec3_08098698 position;
    u8 padding14[4];
    s32 scale_x;
    s32 scale_y;
    u8 padding20[0x10];
    s32 size_x;
    s32 size_y;
    u8 padding38[0x18];
    SpritePair_08098698 *sprites;
    u8 padding54;
    u8 kind;
} SceneObject_08098698;

typedef struct SceneState_08098698 {
    s32 direction;
    Vec3_08098698 launch_vector;
    SceneObject_08098698 *reference;
} SceneState_08098698;

LAYOUT_SIZE_GUARD(
    SpriteRecord08098698_Size,
    SpriteRecord_08098698,
    0x0c);
LAYOUT_OFFSET_GUARD(
    SceneObject08098698_Position,
    SceneObject_08098698,
    position,
    8);
LAYOUT_OFFSET_GUARD(
    SceneObject08098698_ScaleX,
    SceneObject_08098698,
    scale_x,
    0x18);
LAYOUT_OFFSET_GUARD(
    SceneObject08098698_SizeX,
    SceneObject_08098698,
    size_x,
    0x30);
LAYOUT_OFFSET_GUARD(
    SceneObject08098698_Sprites,
    SceneObject_08098698,
    sprites,
    0x50);
LAYOUT_OFFSET_GUARD(
    SceneObject08098698_Kind,
    SceneObject_08098698,
    kind,
    0x55);
LAYOUT_OFFSET_GUARD(
    SceneState08098698_Reference,
    SceneState_08098698,
    reference,
    0x10);

extern SceneState_08098698 *Data_03001f30;

void Func_080030f8(s32 frames);
u32 Func_08004458(void);
void Func_0800447c(
    s32 magnitude, s32 direction, Vec3_08098698 *vector);
void Func_08009080(SceneObject_08098698 *object, s32 mode);
void Func_08009098(SceneObject_08098698 *object, const void *program);
void Func_08009150(
    SceneObject_08098698 *object, s32 x, s32 y, s32 z);
void Func_080091e0(SceneObject_08098698 *object, s32 mode);
void Func_08009240(SceneObject_08098698 *object, s32 animation);
SceneObject_08098698 *Func_08096c80(
    s32 resource, s32 x, s32 y, s32 z);
void Func_08097384(void);
void Func_080f9010(s32 sound);

static void CopySpriteAppearance_08098698(
    SpriteRecord_08098698 *child,
    const SpriteRecord_08098698 *base)
{
    u16 *child_attr2 = (u16 *)&child->attr2_low;
    const u16 *base_attr2 = (const u16 *)&base->attr2_low;

    child->attr0_high =
        (child->attr0_high & (u8)~0x20) |
        (base->attr0_high & 0x20);
    child->attr0_high =
        (child->attr0_high & 0x3f) |
        (base->attr0_high & 0xc0);
    child->attr1_high =
        (child->attr1_high & 0x3f) |
        (base->attr1_high & 0xc0);
    *child_attr2 =
        (*child_attr2 & 0xfc00) |
        (*base_attr2 & 0x03ff);
    child->attr2_high =
        (child->attr2_high & 0x0f) |
        (base->attr2_high & 0xf0);
}

/*
 * Emit twenty-four copies of the scene projectile. Each copy inherits the
 * base sprite's render attributes, receives a randomized velocity, and is
 * launched from a direction-dependent point around the reference object.
 */
void Func_08098698(void)
{
    SceneState_08098698 *state = Data_03001f30;
    SceneObject_08098698 *reference = state->reference;
    s32 count;

    Func_08097384();
    for (count = 0; count < 24; count++) {
        Vec3_08098698 position = reference->position;
        SceneObject_08098698 *projectile;
        s32 direction = state->direction;

        if (direction == 0x4000) {
            position.y += 0xa0000;
        } else if (direction == 0xc000) {
            position.y += 0x180000;
        } else {
            position.y += 0xa0000;
            Func_0800447c(0xa0000, direction, &position);
        }

        projectile = Func_08096c80(
            0x11c, position.x, position.y, position.z);

        /* Func_08096c80 is expected to succeed in this scripted sequence. */
        CopySpriteAppearance_08098698(
            &projectile->sprites->child,
            &projectile->sprites->base);

        if (projectile != 0) {
            Vec3_08098698 velocity = state->launch_vector;
            s32 magnitude;

            projectile->scale_y = 0xb333;
            projectile->scale_x = 0xb333;
            projectile->size_y = 0x18000;
            projectile->size_x = 0x18000;
            projectile->kind = 0;
            Func_08009240(projectile, 0x0b);
            Func_08009080(projectile, 7);
            Func_08009098(projectile, (const void *)0x0809f0b4);
            Func_080091e0(projectile, 1);

            if (direction == 0xc000)
                Func_0800447c(0xe0000, direction, &velocity);

            magnitude = Func_08004458() * 6 + 0x40000;
            Func_0800447c(magnitude, Func_08004458(), &velocity);
            Func_08009150(
                projectile, velocity.x, velocity.y, velocity.z);
        }

        Func_080f9010(0x83);
        Func_080030f8(2);
    }

    Func_080030f8(8);
}

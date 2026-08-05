#include "effect_0809b11c.h"
#include "layout_guard.h"
#include "types.h"

struct Vector_0809abb4 {
    s32 x;
    s32 y;
    s32 z;
};

struct Object_0809abb4 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[4];
    s32 scale_x;
    s32 scale_y;
};

struct SceneState_0809abb4 {
    s32 owner;
    struct Vector_0809abb4 destination;
    struct Object_0809abb4 *anchor;
    u8 unknown_14[0x44];
    struct EffectSlot effects[16];
};

LAYOUT_OFFSET_GUARD(Object0809abb4_X, struct Object_0809abb4, x, 0x08);
LAYOUT_OFFSET_GUARD(
    Object0809abb4_ScaleX,
    struct Object_0809abb4,
    scale_x,
    0x18);
LAYOUT_OFFSET_GUARD(
    SceneState0809abb4_Anchor,
    struct SceneState_0809abb4,
    anchor,
    0x10);
LAYOUT_OFFSET_GUARD(
    SceneState0809abb4_Effects,
    struct SceneState_0809abb4,
    effects,
    0x58);

extern struct SceneState_0809abb4 *Data_03001f30;

s32 Func_080022ec(s32 value, s32 shift);
void Func_080030f8(s32 frames);
u32 Func_08004458(void);
void Func_0800447c(
    s32 magnitude,
    s32 angle,
    struct Vector_0809abb4 *vector);
void Func_08009080(struct Object_0809abb4 *object, s32 mode);
void Func_080090d0(struct Object_0809abb4 *object);
void Func_08009248(struct Object_0809abb4 *object, s32 mode);
struct Object_0809abb4 *Func_08096c80(s32, s32, s32, s32);
void Func_08097384(void);
void Func_0809748c(void);
void Func_080974d8(struct Vector_0809abb4 *vector);
void Func_0809aa98(struct EffectSlot *effect);
void Func_080f9010(s32 sound);

void Func_0809abb4(void)
{
    struct SceneState_0809abb4 *state = Data_03001f30;
    struct Object_0809abb4 *anchor = state->anchor;
    struct Object_0809abb4 *object;
    struct Vector_0809abb4 start;
    struct Vector_0809abb4 destination;
    struct Vector_0809abb4 position;
    u32 step;
    u32 i;

    state->destination.y = anchor->y;
    object = Func_08096c80(0xfa, 0, 0, 0);
    Func_08009080(object, 0);
    if (object == 0)
        return;

    Func_08097384();
    start.x = anchor->x;
    start.y = anchor->y + 0x100000;
    start.z = anchor->z;
    destination.x = state->destination.x;
    destination.y = state->destination.y + 0x80000;
    destination.z = state->destination.z;

    for (step = 0; step < 11; step++) {
        object->x =
            start.x + Func_080022ec(
                          step * (destination.x - start.x), 10);
        object->y =
            start.y + Func_080022ec(
                          step * (destination.y - start.y), 10);
        object->z =
            start.z + Func_080022ec(
                          step * (destination.z - start.z), 10);
        object->scale_x = Func_080022ec(step * 0xc000, 10) + 0x4000;
        object->scale_y = object->scale_x;
        Func_080030f8(1);
    }

    Func_080030f8(5);
    Func_08009080(object, 1);
    Func_080f9010(0x6c);
    Func_080030f8(10);
    Func_080f9010(0x6c);
    Func_080030f8(10);
    Func_080f9010(0x6c);
    Func_080030f8(10);
    Func_080f9010(0x6d);

    for (i = 0; i < 16; i++) {
        struct EffectSlot *effect = &state->effects[i];

        position.x = object->x;
        position.y = object->y + 0x80000;
        position.z = object->z;
        Func_080974d8(&position);
        Func_0800447c(0x40000, Func_08004458(), &position);
        Func_0809ba90(effect, 0x11d, position.x, position.z);
        Func_0809ba7c(effect, Func_0809aa98);
        Func_08009248(effect->object, 7);
    }

    position.x = object->x;
    position.y = object->y + 0x80000;
    position.z = object->z;
    Func_080030f8(8);
    Func_080090d0(object);
    Func_080030f8(4);
    Func_080030f8(30);
    Func_0809748c();
}

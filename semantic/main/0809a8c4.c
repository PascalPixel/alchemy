#include "types.h"

struct EffectObject_0809a8c4;
typedef void (*EffectCallback_0809a8c4)(struct EffectObject_0809a8c4 *);

struct EffectObject_0809a8c4 {
    u8 unknown_00[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[0x44];
    s16 counter_64;
    s16 counter_66;
    struct EffectObject_0809a8c4 *parent;
    EffectCallback_0809a8c4 callback;
};

struct PositionSource_0809a8c4 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct SceneState_0809a8c4 {
    u8 unknown_00[4];
    s32 x;
    s32 y;
    s32 z;
    struct PositionSource_0809a8c4 *source;
    struct EffectObject_0809a8c4 *secondary;
    u8 unknown_18[8];
    s8 suppress_children;
};

struct Vector_0809a8c4 {
    s32 x;
    s32 y;
    s32 z;
};

extern struct SceneState_0809a8c4 *Data_03001f30;

s32 Func_080022ec(s32 dividend, s32 divisor);
void Func_080030f8(s32 frames);
u32 Func_08004458(void);
void Func_0800447c(
    s32 magnitude,
    s32 angle,
    struct Vector_0809a8c4 *vector);
void Func_08009080(struct EffectObject_0809a8c4 *object, s32 mode);
void Func_080090d0(struct EffectObject_0809a8c4 *object);
struct EffectObject_0809a8c4 *Func_08096c80(
    s32 kind,
    s32 x,
    s32 y,
    s32 z);
void Func_08097384(void);
void Func_0809748c(void);
void Func_0809a7f4(struct EffectObject_0809a8c4 *object);
void Func_0809a890(struct EffectObject_0809a8c4 *object);
void Func_080f9010(s32 sound);

void Func_0809a8c4(void)
{
    struct SceneState_0809a8c4 *scene = Data_03001f30;
    struct PositionSource_0809a8c4 *source = scene->source;
    struct EffectObject_0809a8c4 *effect;
    struct Vector_0809a8c4 start;
    struct Vector_0809a8c4 end;
    s32 step;

    start.x = source->x;
    start.y = source->y;
    start.z = source->z;
    end.x = scene->x;
    end.y = scene->y - 0x40000;
    end.z = scene->z;

    effect = Func_08096c80(218, 0, 0, 0);
    if (effect == 0)
        return;

    Func_08097384();
    Func_08009080(effect, 2);

    for (step = 0; step < 11; step++) {
        s32 scale;

        effect->x =
            start.x + Func_080022ec(step * (end.x - start.x), 10);
        effect->y =
            start.y + Func_080022ec(step * (end.y - start.y), 10);
        effect->z =
            start.z + Func_080022ec(step * (end.z - start.z), 10);
        scale = Func_080022ec(step * 0x10ccc, 10) + 0x4000;
        effect->scale_x = scale;
        effect->scale_y = scale;
        Func_080030f8(1);
    }

    effect->scale_x = 0x1b333;
    effect->scale_y = 0x14ccc;
    Func_080f9010(163);
    Func_080030f8(20);

    if (scene->suppress_children == 0) {
        s32 index;

        if (scene->secondary != 0)
            scene->secondary->callback = Func_0809a890;

        for (index = 0; index <= 15; index++) {
            struct Vector_0809a8c4 position;
            struct EffectObject_0809a8c4 *child;
            s32 magnitude;

            position.x = effect->x;
            position.y =
                effect->y + index * 0xcccc + 0x40000;
            position.z = effect->z;
            magnitude = (s32)(Func_08004458() * 5) + 0x30000;
            Func_0800447c(magnitude, Func_08004458(), &position);
            child = Func_08096c80(
                249, position.x, position.y, position.z);
            if (child != 0) {
                child->callback = Func_0809a7f4;
                child->parent = effect;
                child->counter_64 = 0;
                child->counter_66 = 0;
                child->angle = (u16)Func_08004458();
            }
            Func_080030f8(6);
        }

        Func_080030f8(20);
        Func_080030f8(120);
    }

    Func_08009080(effect, 1);
    Func_080030f8(30);
    Func_080f9010(136);
    Func_080030f8(20);
    Func_080090d0(effect);
    Func_0809748c();
}

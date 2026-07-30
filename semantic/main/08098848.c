#include "layout_guard.h"
#include "types.h"

typedef struct EffectObject_08098848 {
    u8 padding_00[6];
    u16 angle_06;
    s32 x_08;
    s32 y_0c;
    s32 z_10;
    u8 padding_14[0x41];
    u8 mode_55;
    u8 padding_56[0x12];
    struct EffectObject_08098848 *linked_object_68;
    void (*callback_6c)(void *);
} EffectObject_08098848;

typedef struct EffectRuntime_08098848 {
    u8 padding_00[4];
    s32 x_04;
    s32 y_08;
    s32 z_0c;
    EffectObject_08098848 *camera_10;
    EffectObject_08098848 *actor_14;
} EffectRuntime_08098848;

LAYOUT_OFFSET_GUARD(
    EffectObject08098848_Angle,
    EffectObject_08098848,
    angle_06,
    6);
LAYOUT_OFFSET_GUARD(
    EffectObject08098848_Y,
    EffectObject_08098848,
    y_0c,
    0x0c);
LAYOUT_OFFSET_GUARD(
    EffectObject08098848_Mode,
    EffectObject_08098848,
    mode_55,
    0x55);
LAYOUT_OFFSET_GUARD(
    EffectObject08098848_LinkedObject,
    EffectObject_08098848,
    linked_object_68,
    0x68);
LAYOUT_OFFSET_GUARD(
    EffectObject08098848_Callback,
    EffectObject_08098848,
    callback_6c,
    0x6c);
LAYOUT_OFFSET_GUARD(
    EffectRuntime08098848_X,
    EffectRuntime_08098848,
    x_04,
    4);
LAYOUT_OFFSET_GUARD(
    EffectRuntime08098848_Y,
    EffectRuntime_08098848,
    y_08,
    8);
LAYOUT_OFFSET_GUARD(
    EffectRuntime08098848_Z,
    EffectRuntime_08098848,
    z_0c,
    0x0c);
LAYOUT_OFFSET_GUARD(
    EffectRuntime08098848_Camera,
    EffectRuntime_08098848,
    camera_10,
    0x10);
LAYOUT_OFFSET_GUARD(
    EffectRuntime08098848_Actor,
    EffectRuntime_08098848,
    actor_14,
    0x14);

extern EffectRuntime_08098848 *Data_03001f30;

void Func_080030f8(s32);
void Func_08009098(void *, const void *);
void Func_08009158(void *);
void Func_08096b88(void *);
void Func_08096bec(EffectObject_08098848 *, s32, u16);
void Func_08097384(void);
void Func_0809748c(void);
void Func_080981b0(EffectObject_08098848 *);
EffectObject_08098848 *Func_08098a84(s32, s32, s32, s32);
void Func_080f9010(s32);

/*
 * Attach the actor to the effect camera, create two side effects, and raise
 * all three objects together before handing each side effect to its finisher.
 */
void Func_08098848(void)
{
    EffectRuntime_08098848 *runtime = Data_03001f30;
    EffectObject_08098848 *actor = runtime->actor_14;
    EffectObject_08098848 *camera = runtime->camera_10;
    EffectObject_08098848 *effects[2];
    s32 x;
    s32 y;
    s32 z;
    s32 index;
    s32 initial_actor_y;

    if (actor == NULL)
        return;

    Func_08097384();
    camera->linked_object_68 = actor;
    Func_08009098(camera, (const void *)0x0809f0bc);

    x = runtime->x_04;
    y = (s32)((u32)runtime->y_08 + 0x100000);
    z = runtime->z_0c;
    effects[0] = Func_08098a84(
        (s32)((u32)x + 0x200000), y, z, 0x8000);
    effects[1] = Func_08098a84(
        (s32)((u32)x - 0x200000), y, z, 0);

    Func_080030f8(15);
    for (index = 0; index < 2; index++) {
        EffectObject_08098848 *effect = effects[index];

        if (effect != NULL)
            Func_08096bec(effect, 0x0e0000, effect->angle_06);
    }

    Func_08009158(effects[0]);
    actor->callback_6c = Func_08096b88;
    Func_080f9010(0x82);
    actor->mode_55 = 4;

    initial_actor_y = actor->y_0c;
    if (effects[0] != NULL && effects[1] != NULL &&
        initial_actor_y <=
            (s32)((u32)initial_actor_y + 0x200000)) {
        s32 limit = (s32)((u32)initial_actor_y + 0x200000);

        do {
            effects[0]->y_0c =
                (s32)((u32)effects[0]->y_0c + 0x4000);
            effects[1]->y_0c =
                (s32)((u32)effects[1]->y_0c + 0x4000);
            actor->y_0c = (s32)((u32)actor->y_0c + 0x4000);
            Func_080030f8(1);
        } while (actor->y_0c <= limit);
    }

    Func_080981b0(effects[0]);
    Func_080981b0(effects[1]);
    Func_0809748c();
}

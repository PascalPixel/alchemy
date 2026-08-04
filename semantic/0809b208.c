#include "effect_0809b11c.h"
#include "layout_guard.h"
#include "types.h"

typedef struct Vec3_0809b208 {
    s32 x;
    s32 y;
    s32 z;
} Vec3_0809b208;

typedef struct PrimaryObject_0809b208 PrimaryObject_0809b208;
typedef void (*PrimaryCallback_0809b208)(PrimaryObject_0809b208 *);

struct PrimaryObject_0809b208 {
    u8 padding00[8];
    Vec3_0809b208 position;
    u8 padding14[0x24];
    s32 destination;
    u8 padding3c[0x19];
    u8 kind;
    u8 padding56[0x0e];
    s16 phase;
    u8 padding66[6];
    PrimaryCallback_0809b208 callback;
};

typedef struct SceneState_0809b208 {
    u8 padding00[0x10];
    PrimaryObject_0809b208 *primary;
    u8 padding14[4];
    s16 source_id;
    u8 padding1a[0x3e];
    struct EffectSlot effects[24];
} SceneState_0809b208;

LAYOUT_OFFSET_GUARD(
    PrimaryObject0809b208_Position,
    PrimaryObject_0809b208,
    position,
    8);
LAYOUT_OFFSET_GUARD(
    PrimaryObject0809b208_Destination,
    PrimaryObject_0809b208,
    destination,
    0x38);
LAYOUT_OFFSET_GUARD(
    PrimaryObject0809b208_Kind,
    PrimaryObject_0809b208,
    kind,
    0x55);
LAYOUT_OFFSET_GUARD(
    PrimaryObject0809b208_Phase,
    PrimaryObject_0809b208,
    phase,
    0x64);
LAYOUT_OFFSET_GUARD(
    PrimaryObject0809b208_Callback,
    PrimaryObject_0809b208,
    callback,
    0x6c);
LAYOUT_OFFSET_GUARD(
    SceneState0809b208_SourceId,
    SceneState_0809b208,
    source_id,
    0x18);
LAYOUT_OFFSET_GUARD(
    SceneState0809b208_Effects,
    SceneState_0809b208,
    effects,
    0x58);

extern SceneState_0809b208 *Data_03001f30;

void Func_080030f8(s32 frames);
u32 Func_08004458(void);
void Func_08009080(PrimaryObject_0809b208 *object, s32 mode);
void Func_080091e0(PrimaryObject_0809b208 *object, s32 mode);
void Func_08009240(PrimaryObject_0809b208 *object, s32 animation);
void Func_08009248(void *object, s32 animation);
void Func_080916b0(void);
void Func_08092adc(s32 slot, s32 direction, s32 flags);
void Func_080933f8(s32 x, s32 y, s32 z, s32 mode);
void Func_08096b88(PrimaryObject_0809b208 *object);
void Func_08097384(void);
void Func_0809748c(void);
void Func_080974d8(Vec3_0809b208 *position);
void Func_0809b0b0(PrimaryObject_0809b208 *object);
void Func_0809b0dc(PrimaryObject_0809b208 *object);
void Func_0809b11c(struct EffectSlot *effect);
void Func_0809ba90(
    struct EffectSlot *effect, s32 resource, s32 x, s32 z);
void Func_080f9010(s32 sound);

/*
 * Run the source actor's transformation, then emit a ring of twenty-four
 * effects at its projected position. After their launch delay, every live
 * effect advances to state 2 so its callback begins the return phase.
 */
void Func_0809b208(void)
{
    SceneState_0809b208 *scene = Data_03001f30;
    PrimaryObject_0809b208 *primary = scene->primary;
    Vec3_0809b208 position;
    s32 index;

    Func_080916b0();
    Func_080933f8(-1, -1, -1, 0);
    Func_08097384();
    Func_080030f8(10);
    Func_08092adc(scene->source_id, 0x4000, 0);
    Func_080030f8(30);

    primary->callback = Func_08096b88;
    Func_080f9010(0x83);
    Func_08009080(primary, 0x1c);
    Func_080030f8(40);

    Func_080f9010(0xdc);
    Func_08009240(primary, 0);
    Func_08009080(primary, 3);
    primary->callback = Func_0809b0b0;
    primary->phase = 0;
    Func_080030f8(70);

    Func_080091e0(primary, 0);
    primary->kind = 0;
    primary->callback = Func_0809b0dc;
    primary->destination = EFFECT_NO_TARGET;

    position = primary->position;
    Func_080974d8(&position);

    for (index = 0; index < 24; index++) {
        struct EffectSlot *effect = &scene->effects[index];
        s32 scale;

        Func_0809ba90(effect, 0x11c, position.x, position.z);
        Func_0809ba7c(effect, Func_0809b11c);
        Func_0809ba70(effect, 7);
        Func_08009248(
            effect->object,
            (u32)(Func_08004458() * 7) >> 16);
        scale = (Func_08004458() >> 1) + 0x13333;
        effect->scale_y = scale;
        effect->scale_x = scale;
        Func_080030f8(1);
    }

    Func_080030f8(70);
    for (index = 0; index < 24; index++) {
        struct EffectSlot *effect = &scene->effects[index];

        if (effect->active != 0)
            effect->state = 2;
    }

    Func_080030f8(40);
    Func_0809748c();
    Func_080030f8(10);
}

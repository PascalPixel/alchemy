#include "layout_guard.h"
#include "types.h"

typedef struct Vec3_080999f0 {
    s32 x;
    s32 y;
    s32 z;
} Vec3_080999f0;

typedef struct EffectAsset_080999f0 {
    u8 padding00[9];
    u8 display_flags;
} EffectAsset_080999f0;

typedef struct EffectObject_080999f0 {
    u8 padding00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 base_y;
    s32 scale_x;
    s32 scale_y;
    u8 padding20[0x30];
    EffectAsset_080999f0 *asset;
    u8 padding54;
    s8 mode;
    u8 padding56[0x16];
    const void *callback;
} EffectObject_080999f0;

typedef struct ArrivalScene_080999f0 {
    s32 heading;
    Vec3_080999f0 destination;
    EffectObject_080999f0 *source;
    EffectObject_080999f0 *secondary;
    u8 padding18[8];
    s8 extended_burst;
    u8 padding21[0x13];
    s8 high_arrival;
    u8 padding35[0x10];
    s8 alternate_burst;
} ArrivalScene_080999f0;

LAYOUT_OFFSET_GUARD(
    EffectObject080999f0_Position,
    EffectObject_080999f0,
    x,
    8);
LAYOUT_OFFSET_GUARD(
    EffectObject080999f0_BaseY,
    EffectObject_080999f0,
    base_y,
    0x14);
LAYOUT_OFFSET_GUARD(
    EffectObject080999f0_ScaleX,
    EffectObject_080999f0,
    scale_x,
    0x18);
LAYOUT_OFFSET_GUARD(
    EffectObject080999f0_Asset,
    EffectObject_080999f0,
    asset,
    0x50);
LAYOUT_OFFSET_GUARD(
    EffectObject080999f0_Mode,
    EffectObject_080999f0,
    mode,
    0x55);
LAYOUT_OFFSET_GUARD(
    EffectObject080999f0_Callback,
    EffectObject_080999f0,
    callback,
    0x6c);
LAYOUT_OFFSET_GUARD(
    ArrivalScene080999f0_Source,
    ArrivalScene_080999f0,
    source,
    0x10);
LAYOUT_OFFSET_GUARD(
    ArrivalScene080999f0_ExtendedBurst,
    ArrivalScene_080999f0,
    extended_burst,
    0x20);
LAYOUT_OFFSET_GUARD(
    ArrivalScene080999f0_HighArrival,
    ArrivalScene_080999f0,
    high_arrival,
    0x34);
LAYOUT_OFFSET_GUARD(
    ArrivalScene080999f0_AlternateBurst,
    ArrivalScene_080999f0,
    alternate_burst,
    0x45);

extern ArrivalScene_080999f0 *Data_03001f30;

s32 Func_080022ec(s32, s32);
void Func_080030f8(s32);
u32 Func_08004458(void);
void Func_0800447c(s32, s32, Vec3_080999f0 *);
void Func_08009080(EffectObject_080999f0 *, s32);
void Func_080090d0(EffectObject_080999f0 *);
s32 Func_080091a8(s32, s32, s32);
void Func_08009240(EffectObject_080999f0 *, s32);
EffectObject_080999f0 *Func_08096c80(s32, s32, s32, s32);
void Func_08097384(void);
void Func_0809748c(void);
void Func_080f9010(s32);

static void InterpolateArrival_080999f0(
    EffectObject_080999f0 *effect,
    const Vec3_080999f0 *from,
    const Vec3_080999f0 *to)
{
    s32 step;

    for (step = 0; step < 11; step++) {
        s32 scale;

        effect->x =
            from->x + Func_080022ec(step * (to->x - from->x), 10);
        effect->y =
            from->y + Func_080022ec(step * (to->y - from->y), 10);
        effect->z =
            from->z + Func_080022ec(step * (to->z - from->z), 10);
        scale = Func_080022ec(step * 0xc000, 10) + 0x4000;
        effect->scale_x = scale;
        effect->scale_y = scale;
        Func_080030f8(1);
    }
}

static void InterpolateDeparture_080999f0(
    EffectObject_080999f0 *effect,
    const Vec3_080999f0 *from,
    const Vec3_080999f0 *to)
{
    s32 step;

    for (step = 0; step < 11; step++) {
        s32 scale;

        effect->x =
            from->x + Func_080022ec(step * (to->x - from->x), 10);
        effect->y =
            from->y + Func_080022ec(step * (to->y - from->y), 10);
        effect->z =
            from->z + Func_080022ec(step * (to->z - from->z), 10);
        scale = Func_080022ec(step * -0xc000, 10) + 0x10000;
        effect->scale_x = scale;
        effect->scale_y = scale;
        Func_080030f8(1);
    }
}

static void SpawnStandardBurst_080999f0(
    EffectObject_080999f0 *effect,
    s32 count)
{
    s32 index;

    for (index = 0; index < count; index++) {
        Vec3_080999f0 position;
        EffectObject_080999f0 *particle;

        position.x = effect->x;
        position.y = effect->y;
        position.z = effect->z;
        Func_0800447c(
            (s32)(Func_08004458() * 5) + 0x30000,
            (s32)Func_08004458(),
            &position);

        /* The final particle marks the pause at the undisturbed center. */
        if (index == count - 1) {
            Func_080030f8(0x19);
            position.x = effect->x;
            position.y = effect->y;
            position.z = effect->z;
        }

        particle = Func_08096c80(
            0xf0, position.x, position.y, position.z);
        if (particle != 0) {
            particle->base_y = position.y - 0x200000;
            particle->callback = (const void *)0x08099921;
            particle->mode = 2;
        }
        Func_080f9010(0x84);
        Func_080030f8(6);
    }

    Func_080030f8(10);
}

static void SpawnAlternateBurst_080999f0(
    EffectObject_080999f0 *effect,
    s32 count)
{
    s32 index;

    for (index = 0; index < count; index++) {
        Vec3_080999f0 position;
        EffectObject_080999f0 *particle;

        position.x = effect->x;
        position.y = effect->y;
        position.z = effect->z;
        Func_0800447c(
            (s32)(Func_08004458() * 5) + 0x30000,
            (s32)Func_08004458(),
            &position);
        particle = Func_08096c80(
            0x11c, position.x, position.y, position.z);
        if (particle != 0) {
            particle->callback = (const void *)0x080999a9;
            particle->mode = 0;
            particle->asset->display_flags =
                (particle->asset->display_flags & (u8)~0x0c) | 8;
            Func_08009080(particle, 8);
            Func_08009240(particle, 7);
        }
        Func_080030f8(6);
    }

    Func_080030f8(0x46);
}

/* Run the staged arrival, particle burst, and departure presentation. */
void Func_080999f0(void)
{
    ArrivalScene_080999f0 *scene = Data_03001f30;
    EffectObject_080999f0 *source = scene->source;
    EffectObject_080999f0 *effect;
    Vec3_080999f0 start;
    Vec3_080999f0 arrival;
    s32 burst_count;

    effect = Func_08096c80(0xef, 0, 0, 0);
    if (effect == 0)
        return;

    Func_08097384();
    Func_080f9010(0x8a);

    if (scene->secondary == 0) {
        scene->destination.x = source->x;
        scene->destination.z = source->z;
        Func_0800447c(
            0x100000, scene->heading, &scene->destination);
        scene->destination.y = Func_080091a8(
            0, scene->destination.x, scene->destination.z);
    }

    start.x = source->x;
    start.y = source->y + 0x100000;
    start.z = source->z;
    arrival.x = scene->destination.x;
    arrival.y = scene->destination.y + 0x200000;
    arrival.z = scene->destination.z;
    if (scene->high_arrival != 0)
        arrival.y = scene->destination.y + 0x500000;

    InterpolateArrival_080999f0(effect, &start, &arrival);
    Func_080030f8(10);

    burst_count = scene->extended_burst == 0 ? 0x18 : 10;
    if (scene->alternate_burst == 0)
        SpawnStandardBurst_080999f0(effect, burst_count);
    else {
        if (scene->extended_burst == 0)
            burst_count = 0x1e;
        SpawnAlternateBurst_080999f0(effect, burst_count);
    }

    InterpolateDeparture_080999f0(effect, &arrival, &start);
    Func_080090d0(effect);
    Func_0809748c();
}

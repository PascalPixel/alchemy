#include "effect_0809b11c.h"
#include "types.h"

struct PrimaryObject_08095a44;
typedef void (*PrimaryCallback_08095a44)(
    struct PrimaryObject_08095a44 *);

struct PrimaryObject_08095a44 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x58];
    PrimaryCallback_08095a44 callback;
};

struct SceneState_08095a44 {
    u8 unknown_00[0x58];
    struct EffectSlot particles[24];
};

struct Vector_08095a44 {
    s32 x;
    s32 y;
    s32 z;
};

extern struct SceneState_08095a44 *Data_03001f30;

u32 Func_080022f4(u32 dividend, u32 divisor);
void Func_080030f8(s32 frames);
u32 Func_08004458(void);
void Func_08009080(struct PrimaryObject_08095a44 *object, s32 mode);
void Func_080090d0(struct PrimaryObject_08095a44 *object);
void Func_08009248(void *object, s32 animation);
struct PrimaryObject_08095a44 *Func_08092054(s32 slot);
void Func_08092560(s32 slot, s32 speed, s32 event_id);
void Func_080925cc(s32 slot);
void Func_08092adc(s32 slot, s32 mode, s32 flags);
void Func_080958a8(void);
void Func_080958e4(void);
void Func_0809592c(struct PrimaryObject_08095a44 *object);
void Func_08095938(struct EffectSlot *effect);
void Func_080974d8(struct Vector_08095a44 *position);
void Func_0809ba90(
    struct EffectSlot *effect,
    s32 object_kind,
    s32 x,
    s32 z);
void Func_080b0048(s32 resources);
void Func_080b0050(void);
void Func_080f9010(s32 sound);

void Func_08095a44(s32 slot)
{
    struct PrimaryObject_08095a44 *primary = Func_08092054(slot);
    struct SceneState_08095a44 *scene;
    struct Vector_08095a44 position;
    s32 index;
    s32 current_slot;

    if (primary == 0)
        return;

    Func_080958a8();
    scene = Data_03001f30;
    Func_080b0048(0x20118c);

    Func_080f9010(173);
    Func_080925cc(slot);
    Func_080f9010(174);
    Func_080925cc(slot);
    Func_080f9010(175);
    Func_080925cc(slot);
    Func_080030f8(20);

    Func_080f9010(140);
    primary->callback = Func_0809592c;
    Func_080030f8(40);
    Func_080f9010(153);
    Func_08092560(slot, 12, 22);

    position.x = primary->x;
    position.y = primary->y;
    position.z = primary->z;
    Func_080974d8(&position);
    Func_080090d0(primary);
    Func_080f9010(164);

    for (index = 0; index < 24; index++) {
        struct EffectSlot *particle = &scene->particles[index];
        s32 scale;

        Func_0809ba90(particle, 0x11c, position.x, position.z);
        Func_0809ba7c(particle, Func_08095938);
        Func_0809ba70(particle, 7);
        Func_08009248(
            particle->object,
            (Func_08004458() * 7) >> 16);
        scale = (s32)Func_080022f4(Func_08004458(), 3) + 0x10000;
        particle->scale_y = scale;
        particle->scale_x = scale;
        Func_080030f8(1);
    }

    Func_080030f8(60);
    current_slot = *(s32 *)0x02000434;
    Func_08092adc(current_slot, 0x4000, 0);
    Func_080030f8(20);
    Func_08009080(Func_08092054(current_slot), 28);
    Func_080030f8(40);
    Func_080f9010(164);
    Func_080030f8(100);
    Func_080b0050();
    Func_080958e4();
}

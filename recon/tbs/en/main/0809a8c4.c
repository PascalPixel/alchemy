/* Draft, not exact (2026-09-26): 11 differing halfwords, 468 of 468 bytes.
   Whole scene and particle fields plus an inline interpolation helper
   preserve the complete body but leave the allocation residual unchanged.
   The first loop compares step with 11 (blt) only when the bound is a local;
   a literal folds to <= 10. Residual: reload picks r3 where the reference
   picks r2 (and the reverse) for the r9 origin copies, the 0x4000 add and
   the step increment. Expression order, cursor pointers and operand swaps
   leave it unchanged. */
#include "TYPES.H"

struct EffectVector { s32 x, y, z; };
struct EffectParticle {
    u8 reserved_00[6];
    s16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 reserved_20[0x64 - 0x20];
    s16 timer;
    s16 phase;
    struct EffectParticle *parent;
    void (*callback)(void);
};

struct EffectScene {
    s32 variant;
    s32 x;
    s32 y;
    s32 z;
    struct EffectParticle *main_object;
    struct EffectParticle *secondary_object;
    u8 reserved_18[8];
    s8 flags;
};

extern struct EffectScene *Data_03001f30;

void *Func_08096c80(s32, s32, s32, s32);
void Func_08097384(void);
void Func_08009080(void *, s32);
s32 Func_080022ec(s32, s32);
void Func_080030f8(s32);
void Func_080f9010(s32);
u32 Func_08004458(void);
void Func_0800447c(s32, s32, struct EffectVector *);
void Func_080090d0(void *);
void Func_0809748c(void);
void Func_0809a890(void);
void Func_0809a7f4(void);

#define RunBattleEffect14 Func_0809a8c4

static __inline__ s32 InterpolateCoordinate(s32 origin, s32 target, s32 step)
{
    return origin + Func_080022ec(step * (target - origin), 10);
}

void RunBattleEffect14(void)
{
    struct EffectScene *scene = Data_03001f30;
    struct EffectParticle *main_object = scene->main_object;
    struct EffectParticle *secondary_object = scene->secondary_object;
    struct EffectVector particle_position;
    struct EffectVector origin;
    struct EffectVector target;
    struct EffectVector *target_cursor;
    struct EffectParticle *object;
    void *spawned_object;
    s32 step;
    struct EffectParticle *particle;
    s32 count = 11;

    step = 0;
    origin.x = main_object->x;
    origin.y = main_object->y;
    origin.z = main_object->z;
    target.x = scene->x;
    target.y = scene->y - 0x40000;
    target.z = scene->z;
    object = Func_08096c80(0xda, 0, 0, 0);
    if (object == 0) {
        return;
    }
    Func_08097384();
    Func_08009080(object, 2);
    target_cursor = &target;
    do {
        s32 scale;
        object->x = InterpolateCoordinate(origin.x, target_cursor->x, step);
        object->y = InterpolateCoordinate(origin.y, target_cursor->y, step);
        object->z = InterpolateCoordinate(origin.z, target_cursor->z, step);
        scale = 0x4000 + Func_080022ec(step * 0x10ccc, 10);
        object->scale_x = scale;
        object->scale_y = scale;
        Func_080030f8(1);
        step++;
    } while (step < count);
    object->scale_x = 0x1b333;
    object->scale_y = 0x14ccc;
    Func_080f9010(0xa3);
    Func_080030f8(20);
    if (scene->flags == 0) {
        if ((particle = secondary_object) != 0) {
            particle->callback = Func_0809a890;
        }
        step = 0;
        do {
            s32 magnitude;

            particle_position.x = object->x;
            particle_position.y = object->y + step * 0xcccc + 0x40000;
            particle_position.z = object->z;
            magnitude = Func_08004458() * 5 + 0x30000;
            Func_0800447c(magnitude, Func_08004458(), &particle_position);
            particle = (spawned_object = Func_08096c80(
                0xf9,
                particle_position.x,
                particle_position.y,
                particle_position.z
            ));
            if (spawned_object != 0) {
                particle->callback = Func_0809a7f4;
                particle->parent = object;
                particle->timer = 0;
                particle->phase = 0;
                particle->angle = Func_08004458();
            }
            Func_080030f8(6);
            step++;
        } while (step <= 15);
        Func_080030f8(20);
        Func_080030f8(120);
    }
    Func_08009080(object, 1);
    Func_080030f8(30);
    Func_080f9010(0x88);
    Func_080030f8(20);
    Func_080090d0(object);
    Func_0809748c();
}

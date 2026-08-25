#include "types.h"

struct EffectVector { s32 x, y, z; };
struct EffectParticle {
    u8 reserved_00[100];
    s16 timer;
    s16 phase;
    void *parent;
    void (*callback)(void);
};

extern u8 *Data_03001f30;

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

void RunBattleEffect14(void)
{
    u8 *scene = Data_03001f30;
    u8 *main_object = *(u8 **)(scene + 16);
    struct EffectParticle *secondary_object = *(void **)(scene + 20);
    struct EffectVector particle_position;
    struct EffectVector origin;
    struct EffectVector target;
    struct EffectVector *target_cursor;
    u8 *object;
    void *spawned_object;
    s32 step;
    struct EffectParticle *particle;

    step = 0;
    origin.x = *(s32 *)(main_object + 8);
    origin.y = *(s32 *)(main_object + 12);
    origin.z = *(s32 *)(main_object + 16);
    target.x = *(s32 *)(scene + 4);
    target.y = *(s32 *)(scene + 8) - 0x40000;
    target.z = *(s32 *)(scene + 12);
    object = Func_08096c80(0xda, 0, 0, 0);
    if (object == 0) {
        return;
    }
    Func_08097384();
    Func_08009080(object, 2);
    target_cursor = &target;
    do {
        s32 base;
        s32 scale;
        base = origin.x;
        base += Func_080022ec(step * (target_cursor->x - base), 10);
        *(s32 *)(object + 8) = base;
        base = origin.y;
        base += Func_080022ec(step * (target_cursor->y - base), 10);
        *(s32 *)(object + 12) = base;
        base = origin.z;
        base += Func_080022ec(step * (target_cursor->z - base), 10);
        *(s32 *)(object + 16) = base;
        scale = 0x4000 + Func_080022ec(step * 0x10ccc, 10);
        *(s32 *)(object + 24) = scale;
        *(s32 *)(object + 28) = scale;
        Func_080030f8(1);
        step++;
    } while (step < 11);
    *(s32 *)(object + 24) = 0x1b333;
    *(s32 *)(object + 28) = 0x14ccc;
    Func_080f9010(0xa3);
    Func_080030f8(20);
    if (*(s8 *)(scene + 32) == 0) {
        if ((particle = secondary_object) != 0) {
            particle->callback = Func_0809a890;
        }
        step = 0;
        do {
            s32 magnitude;

            particle_position.x = *(s32 *)(object + 8);
            particle_position.y = *(s32 *)(object + 12) + step * 0xcccc + 0x40000;
            particle_position.z = *(s32 *)(object + 16);
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
                *(s16 *)((u8 *)particle + 6) = Func_08004458();
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

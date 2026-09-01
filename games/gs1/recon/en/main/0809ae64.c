#include "types.h"

struct EffectVector { s32 x, y, z; };

extern u8 *Data_03001f30;

s32 Func_080022ec(s32, s32);
void Func_080030f8(s32);
void Func_0800447c(s32, s32, struct EffectVector *);
void Func_08009080(void *, s32);
void Func_080090d0(void *);
void Func_080090f0(void *, s32, s32);
void Func_08009240(void *, s32);
void *Func_08096c80(s32, s32, s32, s32);
void Func_08097384(void);
void Func_0809748c(void);
void Func_080f9010(s32);

#define RunBattleEffect13 Func_0809ae64

void RunBattleEffect13(void)
{
    u8 *scene = Data_03001f30;
    u8 *main_object = *(u8 **)(scene + 16);
    void *secondary_object = *(void **)(scene + 20);
    struct EffectVector origin;
    struct EffectVector target;
    struct EffectVector scene_target;
    struct EffectVector *origin_cursor;
    struct EffectVector *target_cursor;
    u8 *object;
    s32 step;
    s32 zero;

    origin.x = *(s32 *)(main_object + 8);
    origin.y = *(s32 *)(main_object + 12) + 0x100000;
    origin.z = *(s32 *)(main_object + 16);
    origin_cursor = &origin;

    if (*(s8 *)(scene + 32) != 0) {
        target.x = *(s32 *)(main_object + 8);
        target.y = *(s32 *)(main_object + 12) + 0x200000;
        target.z = *(s32 *)(main_object + 16);
        Func_0800447c(0x200000, *(s32 *)scene, &target);
    } else {
        target.x = *(s32 *)(scene + 4);
        target.y = *(s32 *)(scene + 8) + 0x200000;
        target.z = *(s32 *)(scene + 12);
    }
    target_cursor = &target;

    scene_target.x = *(s32 *)(scene + 4);
    scene_target.y = *(s32 *)(scene + 8) + 0x200000;
    scene_target.z = *(s32 *)(scene + 12);

    object = Func_08096c80(
        0xd7, scene_target.x, scene_target.y, scene_target.z);
    if (object == 0)
        return;

    Func_08097384();
    Func_080f9010(0x8a);
    *(u16 *)(object + 6) = *(u16 *)(main_object + 6);
    *(s32 *)(object + 48) = 0x14ccc;
    zero = 0;
    *(u8 *)(object + 85) = zero;
    Func_08009080(object, 5);
    Func_08009240(object, 1);

    step = 0;
    do {
        s32 value;
        value = origin_cursor->x;
        value += Func_080022ec(step * (target_cursor->x - value), 10);
        *(s32 *)(object + 8) = value;
        value = origin_cursor->y;
        value += Func_080022ec(step * (target_cursor->y - value), 10);
        *(s32 *)(object + 12) = value;
        value = origin_cursor->z;
        value += Func_080022ec(step * (target_cursor->z - value), 10);
        *(s32 *)(object + 16) = value;
        value = 0x4000 + Func_080022ec(step * 0xc000, 10);
        *(s32 *)(object + 24) = value;
        *(s32 *)(object + 28) = value;
        step++;
        Func_080030f8(1);
    } while (step < 11);

    Func_080030f8(10);
    Func_08009080(object, 6);
    Func_080030f8(15);
    step = 9;
    do {
        *(s32 *)(object + 12) -= 0x20000;
        step--;
        Func_080030f8(1);
    } while (step >= 0);

    Func_08009080(object, 5);
    Func_080f9010(0x84);
    if (secondary_object != 0)
        Func_080090f0(secondary_object, -0x90000, *(s32 *)((u8 *)secondary_object + 12));
    Func_080030f8(20);

    step = 12;
    do {
        *(s32 *)(object + 12) += 0x18000;
        step--;
        Func_080030f8(1);
    } while (step >= 0);

    Func_080030f8(10);
    Func_080f9010(0x72);
    step = 0;
    do {
        s32 value;
        value = target_cursor->x;
        value += Func_080022ec(step * (origin_cursor->x - value), 10);
        *(s32 *)(object + 8) = value;
        value = target_cursor->y;
        value += Func_080022ec(step * (origin_cursor->y - value), 10);
        *(s32 *)(object + 12) = value;
        value = target_cursor->z;
        value += Func_080022ec(step * (origin_cursor->z - value), 10);
        *(s32 *)(object + 16) = value;
        value = 0x10000 + Func_080022ec(step * -0xc000, 10);
        *(s32 *)(object + 24) = value;
        *(s32 *)(object + 28) = value;
        step++;
        Func_080030f8(1);
    } while (step < 11);

    Func_080090d0(object);
    Func_0809748c();
}

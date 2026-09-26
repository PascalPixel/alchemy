/* Draft, not exact (2026-09-26): 588 bytes, 273 differing halfwords.
   One typed frame establishes the observed 40-byte layout: secondary
   object at +0, target at +4, origin at +16, scene target at +28.
   Remaining: direct sp-relative position stores instead of cursor stores,
   pointer/counter allocation, scale-loop strength reduction, and pools.
   The original raw-offset draft was 588 bytes / 282 halfwords.
 */
#include "TYPES.H"
extern u8 Value_00000000;
extern u8 Value_ffff4000;

struct EffectVector { s32 x, y, z; };

struct MotionObject {
    u8 unknown_00[6];
    u16 angle;
    struct EffectVector pos;
    u8 unknown_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[16];
    s32 speed;
    u8 unknown_34[0x21];
    u8 mode;
};

struct MotionScene {
    s32 angle;
    struct EffectVector pos;
    struct MotionObject *main_object;
    struct MotionObject *secondary_object;
    u8 unknown_18[8];
    s8 offset_target;
};

struct MotionFrame {
    struct MotionObject *secondary_object;
    struct EffectVector target;
    struct EffectVector origin;
    struct EffectVector scene_target;
};

extern struct MotionScene *Data_03001f30;

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

void RunBattleEffect13(void)
{
    struct MotionScene *scene = Data_03001f30;
    struct MotionObject *main_object = scene->main_object;
    struct MotionFrame frame;
    struct EffectVector *origin_cursor;
    struct EffectVector *target_cursor;
    struct MotionObject *object;
    s32 step;
    u16 zero;

    frame.secondary_object = scene->secondary_object;
    frame.origin.x = main_object->pos.x;
    frame.origin.y = main_object->pos.y + 0x100000;
    frame.origin.z = main_object->pos.z;
    origin_cursor = &frame.origin;

    if (scene->offset_target != 0) {
        frame.target.x = main_object->pos.x;
        frame.target.y = main_object->pos.y + 0x200000;
        frame.target.z = main_object->pos.z;
        Func_0800447c(0x200000, scene->angle, &frame.target);
    } else {
        frame.target.x = scene->pos.x;
        frame.target.y = scene->pos.y + 0x200000;
        frame.target.z = scene->pos.z;
    }
    target_cursor = &frame.target;

    frame.scene_target.x = scene->pos.x;
    frame.scene_target.y = scene->pos.y + 0x200000;
    frame.scene_target.z = scene->pos.z;

    object = Func_08096c80(
        0xd7, frame.scene_target.x, frame.scene_target.y, frame.scene_target.z);
    if (object == 0)
        return;

    Func_08097384();
    Func_080f9010(0x8a);
    object->angle = main_object->angle;
    object->speed = 0x14ccc;
    zero = (u16)(s32)&Value_00000000;
    object->mode = zero;
    Func_08009080(object, 5);
    Func_08009240(object, 1);

    step = 0;
    do {
        s32 value;
        value = origin_cursor->x;
        value += Func_080022ec(step * (target_cursor->x - value), 10);
        object->pos.x = value;
        value = origin_cursor->y;
        value += Func_080022ec(step * (target_cursor->y - value), 10);
        object->pos.y = value;
        value = origin_cursor->z;
        value += Func_080022ec(step * (target_cursor->z - value), 10);
        object->pos.z = value;
        value = 0x4000 + Func_080022ec(step * 0xc000, 10);
        object->scale_x = value;
        object->scale_y = value;
        step++;
        Func_080030f8(1);
    } while (step < 11);

    Func_080030f8(10);
    Func_08009080(object, 6);
    Func_080030f8(15);
    step = 9;
    do {
        object->pos.y -= 0x20000;
        step--;
        Func_080030f8(1);
    } while (step >= 0);

    Func_08009080(object, 5);
    Func_080f9010(0x84);
    if (frame.secondary_object != 0)
        Func_080090f0(frame.secondary_object, -0x90000, frame.secondary_object->pos.y);
    Func_080030f8(20);

    step = 12;
    do {
        object->pos.y += 0x18000;
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
        object->pos.x = value;
        value = target_cursor->y;
        value += Func_080022ec(step * (origin_cursor->y - value), 10);
        object->pos.y = value;
        value = target_cursor->z;
        value += Func_080022ec(step * (origin_cursor->z - value), 10);
        object->pos.z = value;
        value = 0x10000 + Func_080022ec(step * (s32)&Value_ffff4000, 10);
        object->scale_x = value;
        object->scale_y = value;
        step++;
        Func_080030f8(1);
    } while (step < 11);

    Func_080090d0(object);
    Func_0809748c();
}

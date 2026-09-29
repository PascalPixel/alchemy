/* 2026-09-29 alchemy permute: score 3735 to 2690 on the permuter's scorer
   (0 is exact); remaining 47 register-only, 27 operand, 10 reordered, 5
   inserted, 8 deleted. Kept rewrites: 15x swap commutative operands, 10x
   reorder independent statements, 6x reorder local declarations, 6x change
   loop form, 5x introduce a temporary, 5x pointer arithmetic or indexing,
   5x test truth or compare with zero, 4x toggle register, 3x split or join
   a compound assignment, 2x invert an if/else, 1x remove a temporary, 1x
   add a same-width cast, 1x drop a same-width cast, 1x move an assignment
   into or out of a condition. FAKEMATCH: the permuter's temporaries,
   register hints and swapped operand orders below only steer allocation
   and scheduling; no programmer would write them, so they stay tagged
   until a natural spelling replaces them. */
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

s32 Math_Div(s32, s32);
void WaitFrames(s32);
void Vector_AddPolarOffset(s32, s32, struct EffectVector *);
void Func_08009080(void *, s32);
void Func_080090d0(void *);
void Func_080090f0(void *, s32, s32);
void Animation_ApplyChildValuesFar(void *, s32);
void *Object_Spawn(s32, s32, s32, s32);
void BattleEffect_InitializeSharedScene(void);
void BattleFx_PrepareBufferInterpolation(void);
void Func_080f9010(s32);

void RunBattleEffect13(void)
{
    struct MotionScene *scene = Data_03001f30;
    struct MotionFrame frame;
    struct MotionObject *main_object = scene->main_object;
    register struct EffectVector *origin_cursor;
    struct MotionObject *object;
    struct EffectVector *target_cursor;
    s32 step;
    s32 tmp3;

    frame.secondary_object = scene->secondary_object;
    frame.origin.x = (*main_object).pos.x;
    frame.origin.y = 0x100000 + main_object->pos.y;
    frame.origin.z = main_object->pos.z;
    if (scene->offset_target) {
        struct EffectVector *tmp;
        frame.target.x = main_object->pos.x;
        frame.target.y = main_object->pos.y + 0x200000;
        frame.target.z = main_object->pos.z;
        tmp = &frame.target;
        Vector_AddPolarOffset(0x200000, scene->angle, tmp);
    } else {
        frame.target.x = scene->pos.x;
        frame.target.y = scene->pos.y + 0x200000;
        frame.target.z = scene->pos.z;
    }
    target_cursor = &frame.target;
    frame.scene_target.x = scene->pos.x;
    origin_cursor = &frame.origin;
    frame.scene_target.y = scene->pos.y + 0x200000;
    frame.scene_target.z = scene->pos.z;
    if (!(object = Object_Spawn(0xd7, frame.scene_target.x, frame.scene_target.y, frame.scene_target.z)))
        return;
    BattleEffect_InitializeSharedScene();
    Func_080f9010(0x8a);
    object->angle = main_object->angle;
    tmp3 = (s32)&Value_00000000;
    object->speed = 0x14ccc;
    object->mode = (u16)tmp3;
    step = 0;
    Func_08009080(object, 5);
    Animation_ApplyChildValuesFar(object, 1);
    do {
        s32 value;
        s32 tmp4;
        s32 tmp5;
        value = origin_cursor->x;
        value += Math_Div((target_cursor->x - value) * step, 10);
        object->pos.x = value;
        tmp5 = origin_cursor->y;
        value = tmp5;
        value = value + Math_Div((target_cursor->y - value) * step, 10);
        object->pos.y = value;
        value = origin_cursor->z;
        value += Math_Div((target_cursor->z - value) * step, 10);
        object->pos.z = value;
        tmp4 = Math_Div(step * 0xc000, 10) + 0x4000;
        value = tmp4;
        object->scale_x = value;
        object->scale_y = value;
        step++;
        WaitFrames(1);
    } while (!(step >= 11 != 0));
    WaitFrames(10);
    Func_08009080(object, 6);
    WaitFrames(15);
    step = 9;
    do {
        object->pos.y -= 0x20000;
        WaitFrames(1);
        step--;
    } while (step >= 0);
    Func_08009080(object, 5);
    Func_080f9010(0x84);
    if (frame.secondary_object != 0)
        Func_080090f0(frame.secondary_object, -0x90000, frame.secondary_object->pos.y);
    WaitFrames(20);
    step = 12;
    do {
        object->pos.y = object->pos.y + 0x18000;
        WaitFrames(1);
        step--;
    } while (step >= 0);
    WaitFrames(10);
    Func_080f9010(0x72);
    step = 0;
    do {
        register s32 value;
        s32 tmp2;
        value = target_cursor->x;
        value += Math_Div((origin_cursor->x - value) * step, 10);
        object->pos.x = value;
        value = target_cursor->y;
        value += Math_Div((origin_cursor->y - value) * step, 10);
        object->pos.y = value;
        value = target_cursor->z;
        value += Math_Div((origin_cursor->z - value) * step, 10);
        object->pos.z = value;
        tmp2 = Math_Div(step * (s32)&Value_ffff4000, 10) + 0x10000;
        value = tmp2;
        object->scale_x = value;
        object->scale_y = value;
        WaitFrames(1);
        ++step;
    } while (step < 11);
    Func_080090d0(object);
    BattleFx_PrepareBufferInterpolation();
}

/* Not exact: 1595 on the permuter scorer (was 2590), written the way the
   exact fallback transition in FALLBACK.C is: step counts declared in the
   loop bodies, the scale through a helper, the two waits counting up for
   the loop pass to reverse, and the mode cleared through a byte local,
   which pools its zero as the reference does. Remaining: every reload here
   takes r2 or r3 where the reference cycles r1, r2 and r0, so the origin
   and target pointers are copied from fp and r9 at each store instead of
   staying in the low register that built them. The reference's spill
   registers are r1 and r2 only, and it forms each address after the first
   load; a source whose busiest reload leaves r3 free should fix the rest. */
#include "TYPES.H"

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

extern struct MotionScene *gEffectWork;

void WaitFrames(s32);
void Vector_AddPolarOffset(s32, s32, struct EffectVector *);
void Object_SetMode(void *, s32);
void Object_Destroy(void *);
void Object_SetPositionAndResetMotionFar(void *, s32, s32);
void Animation_ApplyChildValuesFar(void *, s32);
void *Object_Spawn(s32, s32, s32, s32);
void BattleEffect_InitializeSharedScene(void);
void BattleFx_PrepareBufferInterpolation(void);
void Audio_PlayCue(s32);

static __inline__ s32 Interpolate(s32 to, s32 from, s32 step)
{
    return from + (to - from) * step / 10;
}

void RunBattleEffect13(void)
{
    struct MotionScene *scene = gEffectWork;
    struct MotionObject *secondary = scene->secondary_object;
    struct MotionObject *main_object = scene->main_object;
    struct MotionObject *object;
    struct EffectVector spawn;
    struct EffectVector origin;
    struct EffectVector target;
    s32 step;
    s32 index;
    u8 zero;

    origin.x = main_object->pos.x;
    origin.y = main_object->pos.y + 0x100000;
    origin.z = main_object->pos.z;
    if (scene->offset_target) {
        target.x = main_object->pos.x;
        target.y = main_object->pos.y + 0x200000;
        target.z = main_object->pos.z;
        Vector_AddPolarOffset(0x200000, scene->angle, &target);
    } else {
        target.x = scene->pos.x;
        target.y = scene->pos.y + 0x200000;
        target.z = scene->pos.z;
    }
    spawn.x = scene->pos.x;
    spawn.y = scene->pos.y + 0x200000;
    spawn.z = scene->pos.z;
    object = Object_Spawn(0xd7, spawn.x, spawn.y, spawn.z);
    if (object == 0)
        return;
    BattleEffect_InitializeSharedScene();
    Audio_PlayCue(0x8a);
    object->angle = main_object->angle;
    object->speed = 0x14ccc;
    zero = 0;
    object->mode = zero;
    Object_SetMode(object, 5);
    Animation_ApplyChildValuesFar(object, 1);
    step = 0;
    for (;;) {
        s32 steps = 11;
        s32 scale;

        object->pos.x = origin.x + (target.x - origin.x) * step / 10;
        object->pos.y = origin.y + (target.y - origin.y) * step / 10;
        object->pos.z = origin.z + (target.z - origin.z) * step / 10;
        scale = Interpolate(0x10000, 0x4000, step);
        object->scale_x = scale;
        object->scale_y = scale;
        WaitFrames(1);
        step++;
        if (step >= steps)
            break;
    }
    WaitFrames(10);
    Object_SetMode(object, 6);
    WaitFrames(15);
    for (index = 0; index < 10; index++) {
        object->pos.y -= 0x20000;
        WaitFrames(1);
    }
    Object_SetMode(object, 5);
    Audio_PlayCue(0x84);
    if (secondary != 0)
        Object_SetPositionAndResetMotionFar(secondary, -0x90000, secondary->pos.y);
    WaitFrames(20);
    for (index = 0; index < 13; index++) {
        object->pos.y += 0x18000;
        WaitFrames(1);
    }
    WaitFrames(10);
    Audio_PlayCue(0x72);
    step = 0;
    for (;;) {
        s32 steps = 11;
        s32 scale;

        object->pos.x = target.x + (origin.x - target.x) * step / 10;
        object->pos.y = target.y + (origin.y - target.y) * step / 10;
        object->pos.z = target.z + (origin.z - target.z) * step / 10;
        scale = Interpolate(0x4000, 0x10000, step);
        object->scale_x = scale;
        object->scale_y = scale;
        WaitFrames(1);
        step++;
        if (step >= steps)
            break;
    }
    Object_Destroy(object);
    BattleFx_PrepareBufferInterpolation();
}

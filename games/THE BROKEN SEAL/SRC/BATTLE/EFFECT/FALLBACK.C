/* Battle effects: the fallback transition. An object grows as it glides
   from above the scene's main object to above the scene position over
   eleven frames, chimes three times, then throws out sixteen radial camera
   particles from where it stands and is destroyed. */
#include "TYPES.H"
#include "EFFECT_0809B11C.H"

struct FallbackPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct FallbackObject {
    u8 reserved_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 reserved_14[4];
    s32 scale_x;
    s32 scale_y;
};

struct FallbackScene {
    s32 reserved_00;
    s32 x;
    s32 y;
    s32 z;
    struct FallbackObject *target;
    u8 reserved_14[0x44];
    struct EffectSlot records[16];
};

extern struct FallbackScene *gEffectWork;

struct FallbackObject *Object_Spawn(s32, s32, s32, s32);
void Object_SetMode(struct FallbackObject *, s32);
void BattleEffect_InitializeSharedScene(void);
void WaitFrames(s32);
void Audio_PlayCue(s32);
void Camera_WorldToScreen(s32 *);
u32 Random16(void);
void Vector_AddPolarOffset(s32, u32, s32 *);
void EffectSlot_Initialize(struct EffectSlot *, s32, s32, s32);
void ObjectGroup_SetChildValueUnlessFifteenFar(struct FallbackObject *, s32);
void Object_Destroy(struct FallbackObject *);
void BattleFx_UpdateRadialCamera(struct EffectSlot *);
void BattleFx_PrepareBufferInterpolation(void);

/* The value a tenth of the way per step from one end to the other. */
static __inline__ s32 Fallback_Interpolate(s32 to, s32 from, s32 step)
{
    return from + (to - from) * step / 10;
}

void BattleEffect_RunFallbackObjectTransition(void)
{
    struct FallbackScene *scene = gEffectWork;
    struct FallbackObject *target = scene->target;
    struct FallbackObject *object;
    struct EffectSlot *record;
    struct FallbackPosition position;
    struct FallbackPosition origin;
    struct FallbackPosition destination;
    s32 step;
    s32 index;

    scene->y = target->y;
    object = Object_Spawn(0xfa, 0, 0, 0);
    step = 0;
    Object_SetMode(object, 0);
    if (object == 0)
        return;

    BattleEffect_InitializeSharedScene();
    origin.x = target->x;
    origin.y = target->y + 0x100000;
    origin.z = target->z;
    destination.x = scene->x;
    destination.y = scene->y + 0x80000;
    destination.z = scene->z;
    for (;;) {
        s32 steps = 11;
        s32 scale;

        object->x = origin.x + (destination.x - origin.x) * step / 10;
        object->y = origin.y + (destination.y - origin.y) * step / 10;
        object->z = origin.z + (destination.z - origin.z) * step / 10;
        scale = Fallback_Interpolate(0x10000, 0x4000, step);
        object->scale_x = scale;
        object->scale_y = scale;
        WaitFrames(1);
        step++;
        if (step >= steps)
            break;
    }

    WaitFrames(5);
    Object_SetMode(object, 1);
    Audio_PlayCue(0x6c);
    WaitFrames(10);
    Audio_PlayCue(0x6c);
    WaitFrames(10);
    Audio_PlayCue(0x6c);
    WaitFrames(10);
    Audio_PlayCue(0x6d);

    record = &scene->records[0];
    for (index = 0; index < 16; index++) {
        position.x = object->x;
        position.y = object->y + 0x80000;
        position.z = object->z;
        Camera_WorldToScreen(&position.x);
        Vector_AddPolarOffset(0x40000, Random16(), &position.x);
        EffectSlot_Initialize(record, 0x11d, position.x, position.z);
        EffectSlot_SetCallback(record, BattleFx_UpdateRadialCamera);
        ObjectGroup_SetChildValueUnlessFifteenFar(record->object, 7);
        record++;
    }

    position.x = object->x;
    position.y = object->y + 0x80000;
    position.z = object->z;
    WaitFrames(8);
    Object_Destroy(object);
    WaitFrames(4);
    WaitFrames(30);
    BattleFx_PrepareBufferInterpolation();
}

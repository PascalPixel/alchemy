#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "MOTOBJ.H"
#include "FX_SCENE.H"
#include "EFFECT_SLOT.H"

#define Camera_WorldToScreen ((void (*)(s32*))Camera_WorldToScreen)
#define ObjectGroup_SetChildValueUnlessFifteenFar ((void (*)(struct MotionObject*,s32))ObjectGroup_SetChildValueUnlessFifteenFar)

struct FallbackPosition {
    s32 x;
    s32 y;
    s32 z;
};

extern struct BattleFxScene *gEffectWork;

struct MotionObject *Object_Spawn(s32, s32, s32, s32);
void Object_SetMode(struct MotionObject *, s32);
void WaitFrames(s32);
void Audio_PlayCue(s32);
u32 Random16(void);
void Vector_AddPolarOffset(s32, u32, s32 *);
void ObjectDispatch_ReleaseFar(struct MotionObject *);

void BattleEffect_RunFallbackObjectTransition(void)
{
    struct BattleFxScene *scene = gEffectWork;
    struct MotionObject *target = scene->main_object;
    struct MotionObject *object;
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
        scale = BattleFx_Interpolate(0x10000, 0x4000, step);
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

    record = &scene->slots[0];
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
    ObjectDispatch_ReleaseFar(object);
    WaitFrames(4);
    WaitFrames(30);
    BattleFx_PrepareBufferInterpolation();
}

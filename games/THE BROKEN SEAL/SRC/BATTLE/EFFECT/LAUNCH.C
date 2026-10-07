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
#include "SCENE.H"
#include "FIXED_MATH.H"
#include "EFFECT_SLOT.H"
#include "FX_SCENE.H"
#include "FIELDOBJ.H"
#include "OBJECT_LOOKUP.H"
#include "SYSTEM.H"

#define Camera_WorldToScreen ((void (*)(struct Output*))Camera_WorldToScreen)
#define EffectSlot_Initialize ((void (*)(void*,s32,s32,s32))EffectSlot_Initialize)
#define ObjectGroup_SetChildValueUnlessFifteenFar ((void (*)(void*,s32))ObjectGroup_SetChildValueUnlessFifteenFar)
#define ObjectGroup_ApplyRandomChildValues ((void (*)(void))ObjectGroup_ApplyRandomChildValues)

void BattleFx_UpdateSpinAngle(struct FieldActor *object)
{
    u32 step;

    step = (s16)object->timer * 0x50;
    object->facing = object->facing + step + 0x1000;
    if (step < 0x1000)
        object->timer = object->timer + 1;
}

void BattleFx_UpdateDescendingObject(struct FieldActor *object)
{
    u32 scale;

    object->scale_y -= 0x280;
    scale = object->scale_x - 0x280;
    object->facing += 0x2000;
    object->y.fixed += 0x10000;
    object->scale_x = scale;
    if ((s32)scale < 0x3000)
        object->active = 0;
}

struct Output {
    s32 x;
    s32 y;
    s32 z;
};

extern struct BattleFxScene *gEffectWork;
extern u32 gFrameCount;

void Vector_AddPolarOffset(s32, s32, struct Output *);
void Audio_PlayCue(s32);

void BattleFx_UpdateRadialLaunch(struct EffectSlot *effect)
{
    struct Output position;
    struct FieldActor *source;
    s32 state;
    u32 first_random;

    source = gEffectWork->main_object;
    state = effect->state;

    if (state == 0) {
        position.x = source->x.fixed;
        position.y = source->y.fixed;
        position.z = source->z.fixed;
        Camera_WorldToScreen(&position);

        effect->x = position.x;
        effect->z = position.z + 0x80000;
        effect->origin_z = effect->z;
        effect->origin_x = effect->x;
        position.z = effect->origin_z;
        position.x = effect->origin_x;

        first_random = Random16();
        Vector_AddPolarOffset(
            0x780000,
            ((first_random << 13) >> 16)
                - ((Random16() << 13) >> 16)
                + 0xc000,
            &position);

        effect->target_x = position.x;
        effect->target_z = position.z;
        effect->acceleration = 0x50000;
        effect->max_speed = 0x50000;
        effect->flag42 = state;
        effect->state++;

        if ((gFrameCount & 2) != 0)
            Audio_PlayCue(0xf6);
    } else if (state == 1) {
        if (BattleFx_HasReachedTarget(effect) == 0)
            effect->state--;
    } else if (state == 2) {
        if (BattleFx_HasReachedTarget(effect) == 0)
            BattleFx_ClearOwnedSlot(effect);
    }
}

void Battle_Reset(void);
void ObjectMotion_ArmCallback(s32, s32, s32);
void Audio_PlayCue(s32);
void Object_SetMode(void *, s32);
void Animation_ApplyChildValuesFar(void *, s32);
void ObjectDispatch_SetSingleChildFlagsFar(void *, s32);

void BattleFx_RunEffect15(void)
{
    struct BattleFxScene *scene = gEffectWork;
    struct FieldActor *main_object = scene->main_object;
    struct Output position;
    struct EffectSlot *slot;
    u8 stopped;
    u32 index;

    Battle_Reset();
    Motion_CamBounds(-1, -1, -1, 0);
    BattleEffect_InitializeSharedScene();
    WaitFrames(10);
    ObjectMotion_ArmCallback(scene->first_object_id, 0x4000, 0);
    WaitFrames(30);
    main_object->update = (void (*)(union FieldObject *))ObjectGroup_ApplyRandomChildValues;
    Audio_PlayCue(0x83);
    Object_SetMode(main_object, 28);
    WaitFrames(40);
    Audio_PlayCue(0xdc);
    Animation_ApplyChildValuesFar(main_object, 0);
    Object_SetMode(main_object, 3);
    main_object->update = (void (*)(union FieldObject *))BattleFx_UpdateSpinAngle;
    main_object->timer = 0;
    WaitFrames(70);
    ObjectDispatch_SetSingleChildFlagsFar(main_object, 0);
    main_object->motion_flags = 0;
    main_object->update = (void (*)(union FieldObject *))BattleFx_UpdateDescendingObject;
    main_object->target_x = ACTOR_NO_TARGET;
    position.x = main_object->x.fixed;
    position.y = main_object->y.fixed;
    position.z = main_object->z.fixed;
    Camera_WorldToScreen(&position);
    index = 0;
    slot = scene->slots;
    do {
        s32 speed;
        EffectSlot_Initialize(slot, 0x11c, position.x, position.z);
        EffectSlot_SetCallback(slot, BattleFx_UpdateRadialLaunch);
        EffectSlot_SetObjectMode(slot, 7);
        ObjectGroup_SetChildValueUnlessFifteenFar(slot->object, (Random16() * 7) >> 16);
        speed = (Random16() >> 1) + 0x13333;
        slot->scale_y = speed;
        slot->scale_x = speed;
        index++;
        WaitFrames(1);
        slot++;
    } while (index <= 23);
    WaitFrames(70);
    index = 0;
    stopped = 2;
    do {
        if (scene->slots[index].active != 0)
            scene->slots[index].state = stopped;
        index++;
    } while (index <= 23);
    WaitFrames(40);
    BattleFx_PrepareBufferInterpolation();
    WaitFrames(10);
}

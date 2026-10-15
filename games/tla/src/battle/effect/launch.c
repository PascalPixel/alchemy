#include "types.h"
#include "ram_buffer.h"
#include "effect_slot.h"

#define SLOT_MAX 24

struct SpinObject
{
    u8  unk00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 floor;
    s32 fall;
    u8  unk1c[0x1c];
    s32 targetX;
    u8  unk3c[0x19];
    u8  motionFlags;
    u8  unk56[0xe];
    s16 timer;
    u8  unk66[6];
    void (*update)(struct SpinObject *);
};

struct SpinScene
{
    u8                 unk00[0x10];
    struct SpinObject *mainObject;
    u8                 unk14[4];
    s16                firstObjectId;
    u8                 unk1a[0x36];
    struct EffectSlot  slots[SLOT_MAX];
};

struct SpinVector
{
    s32 x;
    s32 y;
    s32 z;
};

void EventRuntime_Begin(void);
void Motion_CamBounds(s32, s32, s32, s32);
void BattleEffect_InitializeSharedScene(void);
void WaitFrames(s32 frames);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);
void Audio_PlayCue(s32 cue);
void Object_SetMode(struct SpinObject *object, s32 mode);
void Animation_ApplyChildValuesFar(void *object, s32 value);
void ObjectDispatch_SetSingleChildFlagsFar(void *object, s32 flags);
void Camera_WorldToScreen(struct SpinVector *position);
void EffectSlot_Initialize(struct EffectSlot *slot, s32 kind, s32 x, s32 z);
void Animation_ApplyChildValuesToRecordFar(void *record, u32 value);
u32  Random16(void);
void BattleFx_PrepareBufferInterpolation(void);
void ObjectGroup_ApplyRandomChildValues(struct SpinObject *owner);
void BattleFx_AdvanceAngleWithRamp(struct SpinObject *object);
void Func_080e0cac(struct SpinObject *obj);
void Func_080e0cec(struct EffectSlot *slot);

void BattleFx_RunEffect15(void)
{
    struct SpinScene  *scene = Ram_HeapSlots->effectWork;
    struct SpinObject *obj = scene->mainObject;
    struct SpinVector  pos;
    struct EffectSlot *slot;
    struct EffectSlot *slots;
    u8                 last;
    u32                i;

    EventRuntime_Begin();
    Motion_CamBounds(-1, -1, -1, 0);
    BattleEffect_InitializeSharedScene();
    WaitFrames(10);
    ObjectMotion_ArmCallback(scene->firstObjectId, 0x4000, 0);
    WaitFrames(30);
    obj->update = ObjectGroup_ApplyRandomChildValues;
    Audio_PlayCue(131);
    Object_SetMode(obj, 28);
    WaitFrames(40);

    Audio_PlayCue(220);
    Animation_ApplyChildValuesFar(obj, 0);
    Object_SetMode(obj, 3);
    obj->update = BattleFx_AdvanceAngleWithRamp;
    obj->timer = 0;
    WaitFrames(70);

    ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
    obj->motionFlags = 0;
    obj->update = Func_080e0cac;
    obj->targetX = EFFECT_NO_TARGET;
    pos.x = obj->x;
    pos.y = obj->y;
    pos.z = obj->z;
    Camera_WorldToScreen(&pos);
    i = 0;
    slot = scene->slots;
    slots = slot;
    do
    {
        s32 scale;

        EffectSlot_Initialize(slot, 0x2a0, pos.x, pos.z);
        EffectSlot_SetCallback(slot, Func_080e0cec);
        EffectSlot_SetObjectMode(slot, 7);
        Animation_ApplyChildValuesToRecordFar(slot->object, (Random16() * 7) >> 16);
        scale = (Random16() >> 1) + 0x13333;
        slot->scaleY = scale;
        slot->scaleX = scale;
        WaitFrames(1);
        i++;
        slot++;
    } while (i < SLOT_MAX);
    WaitFrames(70);

    i = 0;
    last = 2;
    slot = slots;
    do
    {
        if (slot->active != 0)
            slot->state = last;
        i++;
        slot++;
    } while (i < SLOT_MAX);
    WaitFrames(40);
    BattleFx_PrepareBufferInterpolation();
    obj->fall = 0;
    obj->y = obj->floor;
    WaitFrames(10);
}

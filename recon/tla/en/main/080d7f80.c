/* differ 14: ported from ☀️ radpart.c; load order only */
#include "types.h"
#include "system.h"
#include "animspr.h"
#include "motobj.h"
#include "object_runtime.h"
#include "objdisp.h"
#include "party_state.h"
#include "effect_slot.h"
#include "ram_buffer.h"

struct EffectVector {
    s32 x;
    s32 y;
    s32 z;
};

struct BattleFxScene;

#define ObjectGroup_SetChildValueUnlessFifteenFar ((void (*)(s32,s32))Animation_ApplyChildValuesToRecordFar)
#define ObjectDispatch_ReleaseFar Object_Destroy
#define gBattleFx_PulseScales gData_080f0bfc
#define gEffectWork Ram_HeapSlots->effectWork

extern u32 gFrameTick;
extern u32 gData_080f0bfc[];
void Animation_ApplyChildValuesToRecordFar();
void Object_Destroy();

struct EffectOwner {
    u8 unk00[8];
    struct EffectVector position;
    u8 unk14[0x3c];
    struct AnimationObject *sprite;
};

struct ObjectRuntime *Object_GetById(u32 actor);
void BattleFx_ClearOwnedSlot(struct EffectSlot *effect);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct EffectVector *position);
void Audio_PlayCue(s32 cue);
void Motion_SetVarCbAndRefresh();
void Camera_WorldToScreen();
void BattleFx_InitializeSlots(void);
void Shop_RestoreSceneTilesFar(s32);
void Shop_InitEffectFar(void);
void EffectSlot_Initialize();
void BattleFx_AdvanceSpinAngle();
void Runtime_SetWorkTripleIfNonNegativeFar(s32, s32, s32);
void Motion_SetTargetPositionFromMagnitudeAngle();

struct PhasedRadialSequenceObject {
    u8 unk00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unk14[4];
    s32 scaleX;
    s32 scaleY;
    u8 unk20[68];
    u16 timer;
    u8 unk66[6];
    void *callback;
};

struct OrbitPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct DescendingOrbitObject {
    u8 unk00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unk14[80];
    u16 timer;
    s16 angle;
};

struct SpreadPosition {
    s32 x;
    s32 y;
    s32 z;
};

void Audio_PlayCue(s32);

extern const s32 gBattleFx_CommonParticleScript[];

struct CaptureObject {
    u8 unk00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 baseY;
    s32 scaleX;
    s32 scaleY;
    u8 unk20[8];
    s32 gravity;
    u8 unk2c[4];
    s32 speed;
    u8 unk34[33];
    u8 mode;
    u8 unk56[5];
    u8 visible;
    u8 unk5c[2];
    u16 lifetime;
    u8 unk60[4];
    u16 timer;
    u16 angle;
    u8 unk68[4];
    void *callback;
};

void ObjectMotion_Launch(s32 id, s32 height, s32 frames);
void Object_SetMode(struct ObjectRuntime *object, s32 mode);
void ObjectMotion_ArmCallback(s32 id, s32 value, s32 flags);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Animation_ApplyChildValuesFar(struct CaptureObject *object, s32 value);
void BattleFx_ClearActiveSlotsAndScheduleUpdates(void);

struct LinkedCaptureObject {
    u8 padding000[8];
    s32 x;
    s32 y;
    s32 z;
    u8 padding014[84];
    struct LinkedCaptureObject *target;
};

void BattleFx_SetObjectAlternatingWords(u8 *object);

void BattleFx_SpinUpAngle(void *obj);

void BattleFx_ShrinkObjectAndDestroySlow(void *obj);

void BattleEffect_UpdatePhasedRadialParticle(struct EffectSlot *effect);

void BattleEffect_RunPhasedRadialParticleSequence(s32 arg)
{
    s8 *object_pointer;
    struct {
        s32 x;
        s32 y;
        s32 z;
    } position;
    s32 value;
    s32 remaining;
    struct PhasedRadialSequenceObject *source_object;
    void *effect_slots;
    struct PhasedRadialSequenceObject *target_object;
    void *slot;
    s8 *slot_state;

    source_object = (struct PhasedRadialSequenceObject *)Object_GetById(arg);
    target_object = (struct PhasedRadialSequenceObject *)Object_GetById(gPartyState.currentOwner);
    if (source_object == NULL)
        return;

    BattleFx_InitializeSlots();
    effect_slots = gEffectWork;
    Shop_RestoreSceneTilesFar(0x201090);
    WaitFrames(30);
    ObjectMotion_ArmCallback(arg, 0x4000, 0);
    WaitFrames(20);
    Audio_PlayCue(173);
    Motion_SetVarCbAndRefresh(arg, 1);
    Audio_PlayCue(174);
    Motion_SetVarCbAndRefresh(arg, 1);
    Audio_PlayCue(175);
    Motion_SetVarCbAndRefresh(arg, 1);
    WaitFrames(20);
    Audio_PlayCue(140);
    source_object->callback = (void *)BattleFx_SpinUpAngle;
    source_object->timer = 0;
    WaitFrames(80);
    source_object->callback = (void *)BattleFx_ShrinkObjectAndDestroySlow;
    Object_SetMode((struct ObjectRuntime *)source_object, 3);

    position.x = source_object->x;
    position.y = source_object->y;
    position.z = source_object->z;
    Camera_WorldToScreen(&position);

    slot = (u8 *)effect_slots + 88;
    remaining = 23;
    do {
        EffectSlot_Initialize(slot, 284, position.x, position.z);
        EffectSlot_SetCallback(slot, (void *)BattleEffect_UpdatePhasedRadialParticle);
        EffectSlot_SetObjectMode(slot, 7);
        object_pointer = slot;
        value = *(s32 *)object_pointer;
        ObjectGroup_SetChildValueUnlessFifteenFar(value, 10);
        value = Random16() / 3 + 0x10000;
        ((struct EffectSlot *)slot)->scaleY = value;
        ((struct EffectSlot *)slot)->scaleX = value;
        remaining--;
        WaitFrames(1);
        slot = (u8 *)slot + 72;
    } while (remaining >= 0);

    WaitFrames(60);
    ObjectMotion_ArmCallback(gPartyState.currentOwner, 0x4000, 0);
    WaitFrames(20);
    Object_SetMode(Object_GetById(gPartyState.currentOwner), 28);
    {
        s32 next_state;

        WaitFrames(20);
        next_state = 2;
        slot_state = effect_slots;
        slot_state += 152;
        remaining = 23;
        do {
            if (slot_state[5] != 0)
                slot_state[0] = next_state;
            remaining--;
            slot_state += 72;
        } while (remaining >= 0);
    }

    WaitFrames(60);
    target_object->callback = (void *)BattleFx_SetObjectAlternatingWords;
    WaitFrames(100);

    {
        s32 next_state;

        slot_state = effect_slots;
        next_state = 5;
        slot_state += 152;
        remaining = 23;
        do {
            if (slot_state[5] != 0)
                slot_state[0] = next_state;
            remaining--;
            slot_state += 72;
        } while (remaining >= 0);
    }

    WaitFrames(10);
    slot = NULL;
    target_object->callback = slot;
    target_object->scaleX = 0x10000;
    target_object->scaleY = 0x10000;
    WaitFrames(30);
    Shop_InitEffectFar();
    BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

void BattleFx_ShrinkObjectAndDestroyFast(void *obj);

void BattleFx_UpdateDescendingOrbitObject(struct DescendingOrbitObject *arg);

void BattleFx_UpdateRadialSpread(struct EffectSlot *effect);

void BattleFx_RunVenusDjinnCapture(s32 arg);

void BattleFx_UpdateRandomTargetParticle(struct EffectSlot *effect);


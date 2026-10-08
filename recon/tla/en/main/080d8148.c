/* differ 4: ported from ☀️ radpart.c; native loads the scale constant first */
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
#define BattleFx_PulseScales Data_080f0bfc
#define gEffectWork Ram_HeapSlots->effect_work

extern u32 gFrameTick;
extern u32 Data_080f0bfc[];
void Animation_ApplyChildValuesToRecordFar();
void Object_Destroy();

struct EffectOwner {
    u8 unknown_00[8];
    struct EffectVector position;
    u8 unknown_14[0x3c];
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
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[4];
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[68];
    u16 timer;
    u8 unknown_66[6];
    void *callback;
};

struct OrbitPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct DescendingOrbitObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[80];
    u16 timer;
    s16 angle;
};

struct SpreadPosition {
    s32 x;
    s32 y;
    s32 z;
};

void Audio_PlayCue(s32);

extern const s32 BattleFx_CommonParticleScript[];

struct CaptureObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 base_y;
    s32 scale_x;
    s32 scale_y;
    u8 unknown_20[8];
    s32 gravity;
    u8 unknown_2c[4];
    s32 speed;
    u8 unknown_34[33];
    u8 mode;
    u8 unknown_56[5];
    u8 visible;
    u8 unknown_5c[2];
    u16 lifetime;
    u8 unknown_60[4];
    u16 timer;
    u16 angle;
    u8 unknown_68[4];
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

void BattleEffect_RunPhasedRadialParticleSequence(s32 arg);

void BattleFx_ShrinkObjectAndDestroyFast(void *obj)
{
    s32 scale;

    scale = ((struct MotionObject *)obj)->scale_x + 0xFFFFFC00;
    ((struct MotionObject *)obj)->scale_y = ((struct MotionObject *)obj)->scale_y + 0xFFFFFC00;
    ((struct MotionObject *)obj)->angle = ((struct MotionObject *)obj)->angle + 0x2000;
    ((struct MotionObject *)obj)->scale_x = scale;
    if (scale < 0x3000) {
        ObjectDispatch_ReleaseFar();
    }
}

void BattleFx_UpdateDescendingOrbitObject(struct DescendingOrbitObject *arg);

void BattleFx_UpdateRadialSpread(struct EffectSlot *effect);

void BattleFx_RunVenusDjinnCapture(s32 arg);

void BattleFx_UpdateRandomTargetParticle(struct EffectSlot *effect);


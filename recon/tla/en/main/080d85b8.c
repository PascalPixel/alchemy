/* differ 4: ported from ☀️ radpart.c; native starts the acceleration constant before it loads z */
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

void BattleFx_ShrinkObjectAndDestroyFast(void *obj);

void BattleFx_UpdateDescendingOrbitObject(struct DescendingOrbitObject *arg);

void BattleFx_UpdateRadialSpread(struct EffectSlot *effect);

void BattleFx_RunVenusDjinnCapture(s32 arg);

void BattleFx_UpdateRandomTargetParticle(struct EffectSlot *effect)
{
    struct ObjectRuntime *source;
    struct EffectVector position;
    s8 *state_pointer;
    s32 state;

    source = Object_GetById(gPartyState.current_owner);
    state_pointer = &effect->state;
    state = *state_pointer;

    if (state == 0) {
        position.x = source->x;
        position.y = source->y + Random16() * 5 + 0xf0000;
        {
            struct EffectVector *p = &position;

            asm volatile("" : "+l"(p)); /* FAKEMATCH: ⚓️ passes the position before it loads z */
            position.z = source->z;
            Camera_WorldToScreen(p);
        }
        Vector_AddPolarOffset(
            Random16() * 6 + 0x20000,
            Random16(),
            &position);

        effect->target_x = position.x;
        effect->target_z = position.z;
        effect->x = position.x;
        effect->z = position.z - 0x640000;
        effect->acceleration = 0x30000;
        effect->max_speed = Random16() * 3 + 0x30000;
        effect->scale_x = 0x10000;
        effect->scale_y = 0x10000;
        effect->flag42 = state;
        effect->stop_at_target = 1;
        (*state_pointer)++;
    } else if ((u8)(effect->state - 1) <= 1) {
        if (BattleFx_HasReachedTarget(effect) == 0) {
            position.x = effect->x;
            position.z = effect->z;
            Vector_AddPolarOffset(0xc0000, Random16(), &position);
            effect->target_x = position.x;
            effect->target_z = position.z;
            effect->stop_at_target = 0;
            effect->speed = 0x10000;
            effect->acceleration = 0;
            effect->max_speed = Random16() + 0x23333;
            effect->scale_x = 0x8000;
            effect->scale_y = 0x8000;
            Audio_PlayCue(143);

            if (*state_pointer == 1)
                (*state_pointer)--;
            else
                (*state_pointer)++;
            effect->callback_delay = 6;
        }
    } else if (state == 3) {
        BattleFx_ClearOwnedSlot(effect);
    }
}


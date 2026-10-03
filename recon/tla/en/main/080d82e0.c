/* 2026-10-03 child-call ownership correction: the native call at 080d83fe
   reads the slot's first-word child pointer and passes value 11 to
   Animation_ApplyChildValuesToRecordFar. Removed the stale unresolved
   ObjectGroup_SetChildValueUnlessFifteenFar declaration and use the
   maintained DispatchChild/u32 contract. EN score improves 1731/42 to
   1711/41 solely by resolving that target name. All six candidate .text
   sections remain identical at 728 bytes, and instruction/relocation
   comparisons differ only in that target name; whole objects differ in
   symbol metadata. Other unresolved names and matching gaps remain. */
/* Earlier 2026-10-03 trial: ApplyChildValuesFar now uses OBJECT_DISPATCH.H's maintained
   void(DispatchObject *, u32) declaration and an explicit object view.
   Its imported SYSTEM.H also replaces the competing local Random16 return
   declaration. The complete EN object and score 1731/42 are unchanged;
   the same unresolved effect/slot/object names and globals remain. */
/*
 * Canonical draft API context; no match or adoption is claimed.
 * API context measured 2026-10-02 with ordinary target flags:
 * all six prior successful objects retain every allocated byte and
 * normalized call/pool operand; no new match or adoption is claimed.
 * The shared void DispatchObject/u32 contract uses ordinary data casts;
 * every original matching-body and trial annotation is retained.
 */
#include "TYPES.H"
#include "OBJECT_DISPATCH.H"
#include "OBJDISP.H"

extern u8 gEffectWork[];

void BattleFx_AdvanceSpinAngle(void);
void BattleFx_ShrinkObjectAndDestroyFast(void);
void BattleFx_UpdateRadialSpread(void);
void BattleFx_UpdateDescendingOrbitObject(void);
extern const u8 BattleFx_CommonParticleScript[];

/* The Venus Djinni capture: the Djinni hops twice and leaps, a ring of
   particles rises from it and falls back, then a burst above the leader
   and eight sparks scattered around it close the scene. */

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

extern s32 gGameState[];

struct CaptureObject *Object_GetById(s32 id);
void BattleFx_InitializeSlots(void);
void Unnamed_080b0840Far(s32 value);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void ObjectMotion_Launch(s32 id, s32 height, s32 frames);
void Func_080091f0(s32 x, s32 y, s32 z);
void Object_SetMode(struct CaptureObject *object, s32 mode);
void ObjectMotion_ArmCallback(s32 id, s32 value, s32 flags);
void Camera_WorldToScreen(s32 *position);
void EffectSlot_Initialize(void *slot, s32 kind, s32 x, s32 y);
void EffectSlot_SetCallback(void *slot, void *callback);
void EffectSlot_SetObjectMode(void *slot, s32 mode);
s32 __divsi3(s32 numerator, s32 denominator);
s32 __udivsi3(s32 numerator, s32 denominator);
struct CaptureObject *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Motion_SetTargetPositionFromMagnitudeAngle(struct CaptureObject *object, s32 magnitude, s32 angle);
void Shop_InitEffectFar(void);
void BattleFx_ClearActiveSlotsAndScheduleUpdates(void);

void BattleFx_RunVenusDjinnCapture(s32 arg)
{
    struct CaptureObject *leader;
    struct CaptureObject *djinni;
    void *effect_slots;
    u8 *slot;
    s8 *slot_state;
    s32 remaining;
    s32 scale;
    s32 position[3];

    leader = Object_GetById(gGameState[125]);
    djinni = Object_GetById(arg);
    if (djinni == NULL)
        return;

    BattleFx_InitializeSlots();
    effect_slots = *(void **)gEffectWork;
    Unnamed_080b0840Far(0x201204);
    WaitFrames(30);
    djinni->visible = 0;
    Audio_PlayCue(152);
    ObjectMotion_Launch(arg, 4, 15);
    Audio_PlayCue(152);
    ObjectMotion_Launch(arg, 4, 15);
    WaitFrames(30);
    djinni->callback = (void *)BattleFx_AdvanceSpinAngle;
    Audio_PlayCue(153);
    ObjectMotion_Launch(arg, 8, 22);
    Audio_PlayCue(140);
    Func_080091f0(0x14ccc, 0x14ccc, 0x10000);
    djinni->callback = (void *)BattleFx_ShrinkObjectAndDestroyFast;
    Object_SetMode(djinni, 3);
    WaitFrames(90);
    ObjectMotion_ArmCallback(gGameState[125], 0x4000, 0);
    WaitFrames(20);
    Object_SetMode(Object_GetById(gGameState[125]), 28);
    WaitFrames(30);
    Func_080091f0(0x19999, 0x19999, 0x10000);

    position[0] = djinni->x;
    position[1] = djinni->y;
    position[2] = djinni->z;
    Camera_WorldToScreen(position);

    slot = (u8 *)effect_slots + 88;
    remaining = 23;
    do {
        EffectSlot_Initialize(slot, 284, position[0], position[2]);
        EffectSlot_SetCallback(slot, (void *)BattleFx_UpdateRadialSpread);
        EffectSlot_SetObjectMode(slot, 7);
        Animation_ApplyChildValuesToRecordFar(*(struct DispatchChild **)slot, 11);
        *(s32 *)(slot + 40) = 0x8000;
        *(s32 *)(slot + 44) = Random16() + 0x18000;
        remaining--;
        WaitFrames(1);
        slot += 72;
    } while (remaining >= 0);

    WaitFrames(140);
    {
        s32 next_state;

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

    WaitFrames(20);
    Func_080091f0(1, 1, 1);
    WaitFrames(30);

    for (remaining = 0; remaining <= 23; remaining++) {
        position[0] = leader->x;
        position[1] = leader->y + 0x780000;
        position[2] = leader->z;
        djinni = Object_Spawn(284, position[0], position[1], position[2]);
        if (djinni != NULL) {
            scale = __udivsi3(Random16(), 3) + 0x10000;
            djinni->scale_y = scale;
            djinni->scale_x = scale;
            djinni->timer = 100;
            djinni->angle = __divsi3(remaining << 16, 24);
            djinni->callback = (void *)BattleFx_UpdateDescendingOrbitObject;
            djinni->mode = 0;
            Object_SetMode(djinni, 7);
            Animation_ApplyChildValuesFar((struct DispatchObject *)djinni, 11);
        }
    }

    WaitFrames(100);
    Audio_PlayCue(288);
    WaitFrames(1);
    Audio_PlayCue(151);
    position[0] = leader->x;
    position[1] = leader->y + 0x120000;
    position[2] = leader->z;
    for (remaining = 0; remaining <= 7; remaining++) {
        djinni = Object_Spawn(284, position[0], position[1], position[2]);
        if (djinni == NULL)
            break;
        djinni->scale_y = 0x9999;
        djinni->scale_x = 0x9999;
        djinni->mode = 2;
        djinni->gravity = 0x50000;
        djinni->base_y = djinni->y;
        djinni->speed = Random16() + 0x16666;
        Motion_SetTargetPositionFromMagnitudeAngle(djinni, 0x200000, Random16());
        Animation_ApplyChildValuesFar((struct DispatchObject *)djinni, 11);
        djinni->lifetime = 8;
        ObjectDispatch_InitializeFar((struct DispatchObject *)djinni, (u32)BattleFx_CommonParticleScript);
    }

    WaitFrames(15);
    Shop_InitEffectFar();
    BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

#include "RESOURCE.H"
/* Declarations shared by the Vale house scene's sources (resource_374). The
 * calls reach this overlay's import veneers by the names IMPORT.S gives
 * them, and its tables by the labels on the listing's data rows. */
#ifndef HAIDIA_H
#define HAIDIA_H

#include "TYPES.H"
#include "VRAM_BLOCK.H"
#include "CALLBACK_SCHEDULER.H"

#define NULL ((void *)0)

#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FACING_OBJECT.H"
#include "FIELD_EFFECT.H"

struct Obj {
    u8 pad00[6];
    u16 f06;
    u8 pad08[0x30];
    s32 f38;
    s32 f3c;
    s32 f40;
};

struct VerticalEffectAnchor {
    u8 pad00[8];
    s32 x;
    u8 pad0c[4];
    s32 z;
};

struct SceneVerticalEffect {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[4];
    s32 amplitude_x;
    s32 amplitude_y;
    u8 pad20[0x44];
    u16 frame;
    u8 pad66[2];
    struct VerticalEffectAnchor *anchor;
};

/*
 * Exact 2026-09-23 (832 bytes), a tagged fake match. The callback passed
 * to the target-and-callback call (main Object_SetTargetAndCallback) is the
 * data symbol Data_0200ac00, as in HAIDIA_IE/ELDER_AID_EVENT.C; as a plain
 * integer its pool load was scheduled before the zero actor argument.
 * The do/while around the 0x22b area-state store holds the linked scene
 * load after the store, as in the reference.
 */
struct SceneWork {
    u8 pad000[0x22b];
    u8 area_state;
};

struct PairDetail {
    u8 unknown_00[22];
    u8 field_16;
};

struct PairSprite {
    struct FieldSprite sprite;
    struct PairDetail *detail;
};

union PairObject {
    union FieldObject object;
    s32 words[28];
    struct {
        u8 unknown_00[0x68];
        union PairObject *parent;
    } link;
};

void HaidiaIe_SpawnEffectPair(union PairObject *parent);
void FieldScene_RunGroupChoreography(void);
void FieldScene_RunStep15(void);
void FieldScene_RunStep17(void);
void FieldScene_RunStep20(void);
void SceneState_ApplyPair140And0(void);
void FieldScene_Forward4dac(void);
void OverlayObject_UpdateOnFrameBit1(s32 obj);
struct SceneVerticalEffect;
void SceneEffect_AnimateVerticalPositive(struct SceneVerticalEffect *effect);
void SceneEffect_AnimateVerticalNegative(struct SceneVerticalEffect *effect);

struct PairWork {
    u8 unknown_00[70];
    u16 vram_block;
};

/* The OAM view with attribute 1 ending in the two-bit size field. */
struct WorldMapOam {
    u8 unknown_00[4];
    u16 attr0;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
};

extern u8 Data_0200b144[];
extern u8 Data_0200b108[];
extern u8 Data_0200b380[];
extern u8 Data_0200b560[];
extern u8 Data_0200b7d0[];
extern u8 Data_0200b170[];
extern u8 Data_0200bcec[];
extern u8 Data_0200bb3c[];
extern u8 Data_0200bb30[];
extern u8 Data_0200ba64[];
extern u8 Data_0200b938[];
extern u8 Value_0200beb4;
extern u8 Data_0200ae34[];
extern u8 Data_0200aef0[];
extern u8 Data_0200af50[];
extern u8 Data_0200af78[];
extern u8 Data_0200a874[];
extern u8 Data_0200aa48[];
extern u8 Data_0200ab2c[];
extern u8 Data_0200ac14[];
extern u8 Data_0200acf8[];
extern u8 Data_0200ad74[];
extern u8 Data_0200afa0[];
extern const u8 Data_0200ac90[];
extern const u8 Data_0200adf0[];
extern u8 Data_03001ebc[];
extern struct PairWork *Data_03001f30;

void BattleEffect_CleanupSceneObjects(void);
void BattleFx_SetBlock30ValuesMaxZero(void);
s32 Math_RemainderUnsigned();
void UiWork_FinalizePendingCore(void);
void BattleParty_ApplyDrain(s32, s32);
s32 ArcTan2(s32, s32);
void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c);
s32 Engine_GameFlagIsSet();
void Engine_EventRequestExit();
void Engine_MapCopyCellsTo();
void Engine_MapRedraw();
void Engine_TaskWait();
void BattleFx_StartTwelveFrameBlend();
void Unnamed_08094ac8();
s32 Scene_CopyCellAttributes();
void Object_SetTargetAndCallback();
void Engine_ActorSetSpriteFlags();
void Engine_ActorSetAnimation();
void HaidiaIe_RunScriptScene();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void Event_SetPair1d4(const void *, s32);
void BattleFx_SetWeightedResult();
s32 Party_SetFields1ceAnd1d0(const void *, s32);
struct FieldActor *Battle_GetWorkObject1e0(void);
void Event_PrepareObjectAndApplyValue();
void Object_RefreshSelectorById();
void Object_SetActionCallbackAndRefreshById();
void BattleFx_CommitObjectPositionAndWait();
void BattleFx_SetBlock30Values128One();
void Event_SayThenWait(s32 speaker, s32 frames);
void Engine_AudioPlayCue();
void Engine_EventBegin();
void Engine_CameraMoveTo();
void Engine_ActorFaceDirection();
void Engine_EventWait();
void Engine_ActorSetSpritePriority();
void Engine_ActorSetSpeed();
void Engine_ActorEnableActionCallback();
void BattleFx_SetBlock30Values12Zero();
void Engine_EventCloseScreen();
void Engine_GameFlagClear();
void Engine_EventEnd();
s32 AnimationObjects_SelectAnimation(struct FieldSprite *sprite, s32 animation);
void Object_LinkObjectAndSetCallback();
void ObjectMotion_Launch();
void Owner_RefreshActiveRatios();

/* Exact complete owner [020017c8,020022c8): 2816 bytes, pools included.
 * 2026-09-27 Sol H1: FieldActor lookup results own x/z, priority_flags and
 * motion_flags. The nullable lookup has a separate actor lifetime from the
 * earlier child records. This typed record model resolves all 639 differing
 * halfwords / 185 edits, preserving callback target reloads and table r5.
 * Complete normalized diff and literal pools read; candidate/reference cmp
 * is exact. This is not a constant spelling or register-allocation sweep.
 * Reuse sun-west's canonical s32 Party_GiveItem interface for both rewards;
 * that independently proven ABI correction is not the matching lever.
 * Historical baseline and closed axes:
 * Typed callback inline scopes reproduce the prior baseline bytes exactly;
 * they restore target-ID reloads and the r5 table but lose the direct-call
 * trial's shared-constant lifetimes (16 wrong instructions in 671ed82f4).
 * The compiler's cross-call target pseudo, not pointer qualifiers alone,
 * changes the saved-register interference. Inline scopes are ruled out. */

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

LAYOUT_OFFSET_GUARD(PairSprite_Detail, struct PairSprite, detail, 0x28);

LAYOUT_OFFSET_GUARD(PairObject_Parent, union PairObject, link.parent, 0x68);

#endif

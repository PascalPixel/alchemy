#include "GLOBAL_CELLS.H"
#ifndef IMIRU_H
#define IMIRU_H

#include "TYPES.H"
#include "text/MSG_IDS.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FIELD_EFFECT.H"
#include "FACING_OBJECT.H"

#include "BATTLE_EFFECT_RUNTIME.H"
#include "OBJECT_LOOKUP.H"
#define ObjectTable_Get ((struct Subject_02001fa4 * (*)())ObjectTable_Get)
extern u8 gEffectWork[];
extern u8 gKeysHeld[];


#define NULL ((void *)0)
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define ACTOR_FLAGS_OFFSET 89
#define ACTOR_FLAGS_OFFSET_020019e8 90
#define ACTOR_DONE_OFFSET 100

struct Object {
    u8 filler00[24];
    s32 x;
    s32 z;
    u8 filler20[68];
    s16 counter;
    s16 mode;
};

struct SceneService_02001990 {
    u16 unknown00[3];
    u16 value06;
};

typedef struct {
    u8 filler0[6];
    u16 unk6;
    u8 filler8[82];
    u8 unk5A;
    u8 filler5B[9];
    u16 unk64;
} T;

struct MuraSceneRecord {
    u8 pad[100];
    u16 field_0x64;
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 filler14[80];
    u16 unk64;
    s16 unk66;
} T_0200154c;

struct Rec_399 {
    u8 pad00[9];
    u8 lo : 2;
    u8 mode : 2;
    u8 hi : 4;
};

struct Work_399 {
    u8 pad00[24];
    s32 f24;
    s32 f28;
    u8 pad20[3];
    u8 f35;
    u8 pad24[44];
    struct Rec_399 *f80;
    u8 pad54[1];
    u8 f85;
    u8 pad56[14];
    u16 f100;
};

typedef struct {
    u8 filler0[8];
    s32 unk8;
    s32 unkC;
    s32 unk10;
} T_020018c4;

typedef struct {
    u8 filler0[23];
    u8 unk17;
} State;

struct SharedData_02000240 {
    u8 pad_000[500];
    s32 selected_subject;
};

struct Subject_02001fa4 {
    u8 pad_000[6];
    u16 heading;
    s32 x;
    s32 y;
    s32 z;
    u8 pad_020[14];
    u8 id;
    u8 pad_035[13];
    s32 state_048;
    s32 state_052;
    u8 pad_056[34];
    u8 flags_090;
    u8 pad_091[9];
    u16 state_100;
    u8 pad_102[6];
    void *callback;
};

extern s16 ImiruMura_KeyHeadings[];
extern u8 Data_0200a8a0[];
extern u8 Data_0200a798[];
extern u8 Data_0200a9e0[];
extern u8 Data_0200aa58[];
extern u8 Data_0200aad0[];
extern u8 Data_0200adb8[];
extern u8 Data_0200ac80[];
extern u8 ImiruMura_CellStepsB[];
extern u8 ImiruMura_PrimaryScript[];
extern u8 ImiruMura_PrimaryScript2[];
extern s32 ImiruMura_ArcOrigin[];
extern u8 ImiruMura_SceneTable[];
extern u8 ImiruMura_CellStepsA[];
extern u8 ImiruMura_ActorScriptA[];
extern u8 ImiruMura_ActorScriptB[];
extern u8 ImiruMura_ActorScriptC[];
extern u8 ImiruMura_MiaScriptA[];
extern u8 ImiruMura_MiaScriptB[];
extern s16 ImiruMura_SwayHeadings[];
s32 ArcTan2(s32, s32);
void ObjectDispatch_ApplyValueToChildren(T *, s32);
void Object_SetActionCallbackAndRefreshById();
s32 Battle_GetWorkObject1e0();
void Engine_ColorBufferApplySource();
void Engine_ColorBufferInterpolate();
void Engine_ObjectSetTargetAndCallback();
void Engine_ObjectSetTargetAndCallback();
void Engine_ObjectSetTargetAndCallback();
void Object_RefreshSelectorById();
void Object_RefreshSelectorById();
void Object_SetActionCallbackAndRefreshById();
void Vector_AddPolarOffset(s32, s32, s32 *);
void BattleEffect_CleanupSceneObjects(void);
void SceneActor_TurnTowardTableAngle();
s32 GetMapCellCollision();
void Vector_AddPolarOffset();
s32 GetMapCellCollision();
s32 Map_GetTerrainHeight();
void Engine_ObjectSetPosition();
void ObjectDispatch_ApplyValueToChildren();
void Engine_ObjectCommitPosition();
s32 Map_GetTerrainHeight();
void Engine_ObjectSetPosition();
void Engine_ObjectCommitPosition();
void Vector_AddPolarOffset();
s32 GetMapCellCollision();
void Engine_ObjectSetPosition();
void Engine_ObjectCommitPosition();

static __inline__ void ImiruMura_AdvanceProbe(s32 heading, s32 *probe)
{
    /* FAKEMATCH: a direct call reuses r5 after the shift; this boundary rebuilds sp+8 before completing 0x100000. */
    Vector_AddPolarOffset((s32)0x100000, heading, probe);
}

s32 OverlayObject_UpdateWobbleByCounter(struct Object *obj);
s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj);
s32 SceneData_ReturnZero(void);
u8 *ImiruMura_GetExits(void);
void ImiruMura_RunHeadingProbe(void);
void SceneDialogue_RunActorEightFlagGatedDialogue(void);
void SceneDialogue_ShowLine1571Or152F(void);
void SceneDialogue_RunActor9Line(void);
void SceneDialogue_RunActorTenFlag881Dialogue(void);
void FieldScene_RunSupplementalSequenceOne(void);
void SceneDialogue_RunActor12Line(void);
void SceneDialogue_RunActor18Line(void);
void SceneDialogue_RunActor20BranchScene(void);
void SceneDialogue_RunActor20FlaggedLine(void);
void ImiruMura_TalkAboutLighthouse(void);
void SceneDialogue_RunActorEightBranchedDialogue(void);
void FieldScene_RunSingleStep(void);
void SceneDialogue_ShowLine156E(void);
void SceneDialogue_ShowLine1573Or155A(void);
void ImiruMura_OpenInn(void);
void ImiruMura_OpenSanctum(void);
void FieldScene_RunPrimaryScriptChoreography(void);
void SceneActor_UpdateCountdownArcPosition(T_0200154c *o);
void ImiruMura_ShowActorNineTurnMessage(void);
void ImiruMura_CastPsynergyAtActorNine(void);
void SceneActor_ResetActorAndCenterOffsets(struct Work_399 *work);
void FieldScene_RunSupplementalSequenceTwo(union FieldObject *object);
void FieldScene_RunScene399SequenceA(void);
void SceneState_UpdateZoneFlagsFromActorZero(void);
void SceneState_UpdateActor11WithFlag203(void);
void SceneActor_SetActorZeroFacingC000AndRun(void);
void RunEventScript02(void);
void FieldScene_RunThreeActorChoreography(void);
void SceneActor_TurnTowardTableAngle(s32 z);
void StagedActor_RunHeadingProbeStep(void);

#endif

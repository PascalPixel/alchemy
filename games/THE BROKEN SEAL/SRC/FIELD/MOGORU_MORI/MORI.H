/* Declarations shared by Mogall Forest's waypoint sources (resource_39f). */
#ifndef MORI_H
#define MORI_H

#include "MAPCOPY.H"

#define FIELD_STAGED_ACTOR_IMPORTS
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

#include "FIELD_EFFECT.H"
#include "STAGED_ACTOR.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

enum {
    /* Message 0x182 + 181. */
    ITEM_NUT = 181
};

typedef struct OrbitingSceneObjectSprite {
    u8 padding_00[5];
    u8 flags_05_low : 5;
    u8 flags_05_bit_5 : 1;
    u8 flags_05_high : 2;
    u8 padding_06[3];
    u8 flags_09_low : 2;
    u8 flags_09_mode : 2;
    u8 flags_09_high : 4;
    u8 padding_0a[18];
    u8 palette;
    u8 padding_1d[10];
    u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject {
    u8 padding_00[8];
    s32 x;
    s32 y;
    u8 padding_10[19];
    u8 flags_23;
    u8 padding_24[12];
    s32 orbit_angle;
    u8 padding_34[4];
    s32 orbit_center_x;
    s32 orbit_center_y;
    u8 padding_40[16];
    OrbitingSceneObjectSprite *sprite;
    u8 padding_54;
    u8 mode;
    u8 state;
    u8 padding_57[5];
    u8 active;
    u8 padding_5d[4];
    u8 visible;
    u8 padding_62[10];
    u32 callback;
} OrbitingSceneObject;

void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_OffsetPositionAndResetMotion(s32 actor, s32 dx, s32 dz);
void Object_SetModeById(s32 actor, s32 mode);
s32 Object_CheckMovementCollision(struct FieldActor *actor, s16 *position);
void BattleFx_SetPhaseRequest(s32 a, s32 b);
void BattleFx_SetWeightedResult();
u8 *AllocateEffectTransfer(s32, s32);
void MogoruMori_SpawnPuffRing(s32 id);

/* The scene table in the overlay's data. */
extern u8 MogoruMori_SceneTable[];

/* Copy the low two mode bits into the object's owner record. */

/* Returns a slot record.  Its fields are named by position and not verified:
 * +8/+12/+16 read as the position and +10/+18 as signed halfwords. */

/* The sources call one another across files. */
void OverlayObject_SetOwnerMode(u8 *object, s32 mode);
u8 *OverlayObject_PrepareSpawnedObject(s32 x, s32 y, s32 z, s32 kind);
u8 *OverlayObject_SpawnConfiguredWithMode15(s32 x, s32 y, s32 z, s32 kind);
void SceneState_SetValue18Mode2(void);
s32 SceneActor_TryRunSlotZeroMoveStep(s16 *arg);
s32 OverlayObject_ApplyField100(s32 a);
s32 OverlayObject_ApplyZero(s32 a);
void FieldScene_RunScene39f_02000d90(s32 a0, s32 a1, s32 a2, s32 a3);
void FieldScene_RunSixCallSetupSequence(s32 no, s32 val);

s32 SceneData_ReturnZero(void);
u8 *SceneData_GetTableB5bc(void);

void SceneActor_PassOffsetPointOfActorZero(void);
void SceneActor_BobActorZeroWhenAheadClear(void);
void FieldScene_RunScriptedSteps0And17E6(void);
void FieldScene_RunActor10WaypointSequence(void);
void FieldScene_RunActorElevenPresentationBeat(void);
void SceneActor_RunActorTwelveThreeWaypointMotion(void);
void FieldScene_RunStepFD4WithActor181(s32 a);
void FieldScene_RunProbedActorEightOrTenScene(void);
void SceneState_ApplyCrossRectsAroundActor11(void);
void FieldScene_RunActorThirteenPresentationBeat(void);
void FieldScene_RunScene39f_02001818(void);
void SceneActor_RunActorFourteenFourWaypointMotion(void);
void SceneActor_BobActorZeroWhenTargetClear(void);
void SceneActor_MarkActorThirteenTileAndPark(void);
void FieldScene_RunSupplementalSequenceOne(void);
void FieldScene_RunScene39fSequenceA(void);
void FieldScene_RunSlot16WaypointSequence(void);
void FieldScene_RunActor17CameraSequence(void);
void FieldScene_RunScene39f_02002004(void);
void FieldScene_RunActorEighteenEffectSequence(void);
void FieldScene_RunScene39f_020021b0(void);

s32 UpdateOrbitingSceneObject(s32 *p);
void InitializeOrbitingEffect(s32 id);

#endif

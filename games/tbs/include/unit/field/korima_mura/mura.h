#ifndef ALCHEMY_UNIT_FIELD_KORIMA_MURA_MURA_H
#define ALCHEMY_UNIT_FIELD_KORIMA_MURA_MURA_H

#include "types.h"
#include "field_scene.h"

struct Ent;
struct FieldActor;
struct Obj;
struct Obj_020025d8;
struct SceneEntrance;
struct SceneEvent;
struct ScenePlacement;
struct SceneRegion;

s32 KorimaMura_ApplyPendingPose(s32 arg0);
s32 SceneEffect_AdvanceCounterAndSwitchMode(struct Obj *p);
void SceneData_InitRecordTable(u8 *o);
s32 SceneEffect_UpdateFallingObject(struct Obj *p);
s32 SceneActor_TurnTowardTarget(struct Ent *p);
const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneRegion *Scene_GetRegions(void);
const u32 *Scene_GetExits(void);
const struct ScenePlacement *Scene_GetPlacements(void);
void FieldScene_RunActor16MessageBranch(void);
void FieldScene_RunActor27Step(void);
const struct SceneEvent *Scene_GetEvents(void);
void FieldScene_ConfigureActor0ThenRun(s32 a0);
void KorimaMura_AnimateSwitchOrExit(void);
void KorimaMura_RunObjectSpreadScene(void);
s32 Scene_Initialize(void);
void SceneActor_SetActors19To22HeightByFrameParity(void);
void FieldScene_StartEffect141Sequence(s32 arg0, s32 arg1);
void FieldScene_RunSequenceA(void);
void FieldScene_RunExtendedActorSequence(void);
void FieldScene_RunPairedStepA(s32 arg0, s32 arg1);
void FieldScene_RunPairedStepB(s32 arg0, s32 arg1, s32 arg2);
s32 SceneEffect_AdvanceAngleUntilIdle(struct Obj_020025d8 *p);
void SceneEffect_SpawnObject26EveryEightFrames(void);
s32 SceneEffect_SetModeByFrameBit1(s32 arg0);
s32 KorimaMura_TriggerRegionScript(struct FieldActor *actor);
void FieldScene_RunFlag845And847Branches(void);
extern s32 KorimaMura_ActionTable1[];
extern s32 KorimaMura_ActionTable2[];
extern s32 KorimaMura_ActionTable3[];
extern s32 KorimaMura_ActionTable4[];
extern s32 KorimaMura_ActionTable5[];
extern s32 KorimaMura_ActionTable6[];
extern s32 KorimaMura_ActionTable7[];
extern s32 KorimaMura_EntryActions[];
extern s32 KorimaMura_DebrisScript[];
extern s32 KorimaMura_Actor8Path[];
extern u16 KorimaMura_SwitchCells[];
extern struct SceneEntrance gKorimaMuraEntrances[];
extern struct SceneEntrance gKorimaMuraEntrances3[];
extern struct SceneEntrance gKorimaMuraEntrances2[];
extern s16 gKorimaMuraRegions2[];
extern u32 gKorimaMuraExits[];
extern struct ScenePlacement gKorimaMuraPlacements[];
extern struct ScenePlacement gKorimaMuraPlacements1[];
extern struct ScenePlacement KorimaMura_EntryScript[];
extern struct ScenePlacement gKorimaMuraPlacements3[];
extern struct SceneEvent gKorimaMuraEvents[];
extern struct SceneEvent gKorimaMuraEvents3[];
extern s32 KorimaMura_Object26Script[];
extern s32 KorimaMura_RegionScript[];
extern s32 gFallingEffectScript[];

#endif

#ifndef RUNPA_JO_FORTRESS_H
#define RUNPA_JO_FORTRESS_H

#include "types.h"
#include "jo.h"
#include "field_scene.h"
#include "item_ids.h"

#define PRIMARY_ID 24
#define DERIVED_ID 25

#include "staged_actor.h"
#include "object_runtime.h"

typedef struct SceneActor
{
	u8	pad0[8];
	s32 x;
	s32 y;
	s32 z;
} SceneActor;

typedef struct DirectionalSceneActor
{
	u8	pad0[6];
	u16 dir;
	s32 x;
	s32 y;
	s32 z;
} DirectionalSceneActor;
extern s32 gRunpaJoRandomPick;
extern u32 gRunpaJoPairTableA[];
extern u32 gRunpaJoPairTableB[];
extern u32 gRunpaJoPairTableC[];

s32 TryStartActorInteraction(s32 actor_id, s32 interaction_id);

s32 IsActorInteractionAvailable(s32 actor_id);

static __inline__ void SetSceneValue(s16 *field, s32 value)
{

	*field = value;
}

s32 GetEmptySceneData(void);

void ConfigureSceneActor12(void);

void EV70_INIT(void);

void EV50_START(void);

void EV12_MOVE(void);

void PlaceActorTwelveAndFinishScene(void);

void PlaceSceneObjectPairFromTableA(s32 table_index);

void EV40_A(void);

void EV40_B(void);

void PlaceSceneObjectPairFromTableB(s32 table_index);

void EV40_D(void);

void PlaceSceneObjectPairFromTableC(s32 table_index);

void EV40_C(void);

void EV99_ROUYA(void);

void EV98_TOBIRA(void);

void Actor8_Interact(void);

void Actor9_Interact(void);

void Actor10_Interact(void);

void Actor11_Interact(void);

s32 TryStartActorInteraction(s32 actor_id, s32 interaction_id);

void NoOpSceneCallbackA(void);

void NoOpSceneCallbackB(void);

void NoOpSceneCallbackC(void);

void NoOpSceneCallbackD(void);

void EV08_KAGI(void);

s32 IsPlayerInAccidentTriggerArea(void);

void FieldScene_UpdateActorPairInteraction(void);

void EV91_SET(void);

s32 AreSceneActorsInPassingLane(void);

void FieldScene_UpdateActorSeventeenInteraction(void);

void EV92_ON(void);

s32 IsPlayerInSecondaryTriggerArea(void);

void FieldScene_UpdateActorEighteenInteraction(void);

void EV93_ON(void);

s32 IsPlayerOutsideSceneRectangle(void);

void FieldScene_RunScene3bfSequenceA(void);

void EV94_MAIN(void);

void TriggerSceneStage95FromActor12(void);

void FieldScene_RunScene3bfSequenceB(void);

void FieldScene_RunScene3bfSequenceC(void);

s32 IsSceneActorVerticallyNearPlayer(s32 actor_id);

s32 IsSceneActorHorizontallyNearPlayer(s32 actor_id);

s32 IsActorInteractionAvailable(s32 actor_id);

s32 IsSceneActorWithinFourSteps(s32 actor_id);

s32 IsSceneActorWithinTriggerBox(s32 actor_id);

void TriggerScene41AtVillagePath(void);

void TriggerScene40AtVillagePath(void);

void RunActor9ScriptedSequence(void);

void RunActorScriptedSequenceA(s32 actor_id);

void TurnActorToSceneDirection(s32 actor_id);

void RunActorScriptedSequenceB(s32 handle);

void RunActorScriptedSequenceC(s32 actor_id);

void RunpaJo_ActOutGuardShiftDialogue(s32 a0);

void RunActorScriptedSequenceD(s32 actor_id);

void EV33_SHIRABE(void);

void EV30_TAKARABAKO(void);

void EV31_BANPEI(void);

void FieldScene_RunSequenceTail(void);

void EV32_KARA(void);

void EV50_MAIN(void);

void EV51_MAIN(void);

void ConfigureInteractionRegionA(void);

void ConfigureInteractionRegionB(void);

void ConfigureInteractionRegionC(void);

void EV52_IDO(void);

void EV51_SUB(void);

void EV00_HANTEI1(void);

void EV00_HANTEI2(void);

void EV08_TANSU(void);

void EV09_MAKI(void);

void EV10_HON(void);

void NoOpInteractionCallback(void);

void RunpaJo_GatherPartyAroundActorTwelve(void);

void RunpaJo_ActivateLeverTrap(void);

void RunpaJo_RevealMonster(void);

void RunpaJo_PositionPartyForMonster(void);

void EV20_DEMO(void);

void FieldScene_RunMainScriptSequence(void);

void EV21_MES(void);

void EV21_MAIN(void);

void EV25_DONPA(void);

void EV25_SEL(void);

void EV24_SEL(void);

void EV24_MAIN(void);

void EV26_SET(void);

void EV14_SET(void);

void EV13_SET(void);

void EV12_SET(void);

void EV18_SET(void);

void EV20_MAIN(void);

void EV20_END(void);

void NoOpActorCallback(void);

void EV17_TALK(void);

void EV17_LOAD(void);

#include "object_runtime.h"

s32 FieldScene_DispatchActorUpdate(void);

void FieldScene_InstallSceneTasks(void);

void FieldScene_SetupActorsForScene(void);

void FieldScene_RestoreActorsFromFlags(void);

void FieldScene_ActivateThreeActorGroup(void);

void FieldScene_ActivateTwoActorGroup(void);

void FieldScene_ActivateAlternateActorGroup(void);

void ActivateFiveActorGroupFromFlags(void);

void FieldScene_SetPositionPairs(s32 idx);

#endif

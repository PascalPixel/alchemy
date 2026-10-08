#include "types.h"
#include "ability_ids.h"
#include "field_event.h"
#include "field_scene.h"
#include "objcmd.h"
#include "text/msg_ids.h"

void BiribinoNiwa_LeaveThroughGate(void);
void BiribinoNiwa_RunGardenEvent(void);
void FieldScene_RunStepWithValue29de(void);
void FieldScene_RunStepWithValueFd2(void);
void SceneActor_UpdateFacingTowardTarget(void);
void SceneDialogue_AskAboutBarricade(void);
void SceneDialogue_AskIfFineWarrior(void);
void SceneDialogue_AskIfResponsible(void);

u16 Niwa_GateCells[] = {
	MAP_CELL_STEP(34, 25, 1, 3, 5), MAP_CELL_STEP(35, 25, 1, 3, 5), MAP_CELL_STEP(36, 25, 1, 3, 5), MAP_CELL_STEPS_END,
};

s32 BiribinoNiwa_GuardScript[] = {
	SC_CALL(SceneActor_UpdateFacingTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance Placement_Scripts[] = {
	{ 1, CONDITION_ALWAYS, 320, 0, 328, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 328, 0, 272, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 Placement_Messages[] = {
	SCENE_EXITS(34),
	SCENE_EXIT(1, 30, 2),
	SCENE_EXIT(2, 33, 1),
	SCENE_EXIT(10, 33, 29),
	SCENE_EXITS_END,
};

struct ScenePlacement gBiribinoNiwaPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gBiribinoNiwaPlacements[] = {
	{ 0x87, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(103), 0, PIXELS(280), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x87, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(135), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x45, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(128), 0, PIXELS(327), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x96, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(320), 0, PIXELS(263), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x96, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(334), 0, PIXELS(263), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x16, 0xfd2, 7,
		PIXELS(216), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
	{ 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(388), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#endif
	{ SCENE_TABLE_END },
};

struct SceneEvent gBiribinoNiwaEventsOther[] = {
	{ SCENE_EVENTS_END },
};

struct SceneEvent gBiribinoNiwaEvents[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(BiribinoNiwa_LeaveThroughGate) },
	{ EVENT_TALK, 8, 0x845, MSGNO(MsgBilibinTheLumberIOrderedFromKolima) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgBilibinNowThatTheCurseIsBroken) },
	{ EVENT_TALK, 9, 0x845, EVENT_SCRIPT(SceneDialogue_AskAboutBarricade) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgBilibinItsOverOverMilordDecidedNot) },
	{ EVENT_TALK, 10, 0x845, EVENT_SCRIPT(SceneDialogue_AskIfResponsible) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgBilibinHmphMyPalaceIsNotFinished) },
	{ EVENT_TALK, 11, 0x84f, MSGNO(MsgBilibinLordMccoyIsSoUpsetAbout) },
	{ EVENT_TALK, 11, 0x84e, MSGNO(MsgBilibinICantTellYouAnythingElse) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_AskIfFineWarrior) },
	{ EVENT_TALK, 12, 0x84f, EVENT_SCRIPT(BiribinoNiwa_RunGardenEvent) },
	{ EVENT_TALK, 12, 0x84e, MSGNO(MsgBilibinWhatAShameHuhIGuess) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBilibinICouldNeverHaveGoneTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x845, MSGNO(MsgBilibinIfThingsLikeThisAreGoing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBilibinWedStillBeWorkingIfWe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x845, MSGNO(MsgBilibinIAmGladIDidntKnow) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBilibinThatWasCloseBuildingACursed) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x845, MSGNO(MsgBilibinIfAllThisTroubleWasCaused) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgBilibinHeSaidHeStoppedConstructionSo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x84a, MSGNO(MsgBilibinNoOneBelievedTheyWouldTurn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x84e, MSGNO(MsgBilibinThatBarricadeIsPrettyShabbyIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBilibinIWillBeAReliableWarrior) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x84a, MSGNO(MsgBilibinIsThereNoOneWhoCan) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x84f, MSGNO(MsgBilibinLordMccoyWouldNeverHireAnyone) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x84e, MSGNO(MsgBilibinIDidntThinkTheydLetYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBilibinDontLeaveRobinBeMyMentor) },
	{ SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStepWithValue29de) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 13, 0xfd2, EVENT_SCRIPT(FieldScene_RunStepWithValueFd2) },
	{ SCENE_EVENTS_END },
};

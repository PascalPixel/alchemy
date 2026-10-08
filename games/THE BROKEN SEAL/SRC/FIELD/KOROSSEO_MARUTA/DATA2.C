#include "TYPES.H"
#include "ABILITY_IDS.H"
#include "EDITION.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "ITEM_IDS.H"
#include "OBJCMD.H"
#include "text/MSG_IDS.H"

void ColossoLogRollingStage_ActivateClearObstacleActors(void);
void ColossoLogRollingStage_AdvanceParticleMotion(void);
void ColossoLogRollingStage_ClampAndOffsetActiveActor(void);
void ColossoLogRollingStage_ConfigureActorThirteen(void);
void ColossoLogRollingStage_ConfigureGridRegion(void);
void ColossoLogRollingStage_ConfigurePrimaryObjects(void);
void ColossoLogRollingStage_ConfigureSceneEventEffect(void);
void ColossoLogRollingStage_ConfigureSecondaryObjects(void);
void ColossoLogRollingStage_FinishOrContinueSceneEvent(void);
void ColossoLogRollingStage_NoopSetupHook(void);
void ColossoLogRollingStage_NudgeActorsLeft(void);
void ColossoLogRollingStage_OffsetActiveActor(void);
void ColossoLogRollingStage_RaiseLinkedSceneEffect(void);
void ColossoLogRollingStage_RunLogRollingInteraction(void);
void ColossoLogRollingStage_RunSceneEventIfReady(void);
void ColossoLogRollingStage_RunSetupCompletionHooks(void);
void ColossoLogRollingStage_RunSetupHook(void);
void ColossoLogRollingStage_SelectNearestObstacle(void);
void ColossoLogRollingStage_SpawnRandomSceneEffect(void);
void ColossoLogRollingStage_WaitForSceneEventTask(void);
void FieldScene_RunClosingAuxiliarySequence(void);
void FieldScene_RunDualArrivalSequence(void);
void FieldScene_RunEarlySequence(void);
void FieldScene_RunFinalAuxiliarySequence(void);
void FieldScene_RunFourStepActorMotion(void);
void FieldScene_RunOpeningAuxiliarySequence(void);
void FieldScene_RunSecondArrivalSequence(void);
void KorosseoMaruta_RunStageIntro(void);
void KorosseoMaruta_RunStageStart(void);
void RunPartyCountInteractionCopyB(void);

char gColossoHexChars[20] = "0123456789ABCDEF";

s32 gColossoRandomEffect[] = {
	SC_ADD(SW_Y, -0x4000),
	SC_WAIT(1),
	SC_LOOP(60, SC_TOP),
	SC_FREE,
};

s32 gColossoLinkedEffect[] = {
	SC_SET(SW_HOOK, (s32)ColossoLogRollingStage_SpawnRandomSceneEffect),
	SC_LABEL(1),
	SC_WAIT(10),
	SC_SET(SW_SCALE_X, 0x16000),
	SC_SET(SW_SCALE_Y, 0x16000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x18000),
	SC_SET(SW_SCALE_Y, 0x18000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x12000),
	SC_SET(SW_SCALE_Y, 0x12000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x16000),
	SC_SET(SW_SCALE_Y, 0x16000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x18000),
	SC_SET(SW_SCALE_Y, 0x18000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x12000),
	SC_SET(SW_SCALE_Y, 0x12000),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_WAIT(2),
	SC_WAIT(50),
	SC_LOOP(2, SC_TO(1)),
	SC_POS(0, 0, 0),
	SC_SET(SW_HOOK, 0),
	SC_END,
};

s16 StagedActor_DirectionSteps[] = {
	0, 16,
	0, 16,
	0, 16,
	16, 0,
	16, 0,
	16, 0,
	16, 0,
	0, -16,
	0, -16,
	0, -16,
	0, -16,
	-16, 0,
	-16, 0,
	-16, 0,
	-16, 0,
	0, 16,
};

struct SceneEntrance gKorosseoMarutaEntrances[] = {
	{ 0, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 136, 0, 392, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 gKorosseoMarutaExits[] = {
	SCENE_EXITS(145),
	SCENE_EXIT(10, 145, 1),
	SCENE_EXIT(11, 140, 66),
	SCENE_EXIT(4, 138, 99),
	SCENE_EXIT(5, 138, 99),
	SCENE_EXITS_END,
};

struct ScenePlacement gKorosseoMarutaPlacements[] = {
	{ 0x0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(88), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ ACTOR_GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ ACTOR_IVAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ ACTOR_MIA, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x122, CONDITION_ALWAYS, 7,
		PIXELS(152), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x122, CONDITION_ALWAYS, 7,
		PIXELS(216), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(88), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(312), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(712), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1240), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1272), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1304), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x123, CONDITION_ALWAYS, 7,
		PIXELS(536), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x123, CONDITION_ALWAYS, 7,
		PIXELS(568), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x123, CONDITION_ALWAYS, 7,
		PIXELS(600), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x123, CONDITION_ALWAYS, 7,
		PIXELS(632), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x123, CONDITION_ALWAYS, 7,
		PIXELS(664), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x123, CONDITION_ALWAYS, 7,
		PIXELS(536), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x123, CONDITION_ALWAYS, 7,
		PIXELS(568), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x123, CONDITION_ALWAYS, 7,
		PIXELS(600), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x123, CONDITION_ALWAYS, 7,
		PIXELS(632), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x123, CONDITION_ALWAYS, 7,
		PIXELS(664), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(720), 0, PIXELS(216), FACING_WEST, TALK_KEEP_FACING, 0 },
	{ 0xee, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(792), 0, PIXELS(194), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x126, CONDITION_ALWAYS, 7,
		PIXELS(1080), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(1208), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(136), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(376), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(600), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(856), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1080), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1304), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1512), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1512), 0, PIXELS(208), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x118, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x118, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x118, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ SCENE_TABLE_END },
};

s32 gColossoSceneTaskStatus = 0;

s32 gColossoSceneTaskState = 0;

struct SceneEvent gKorosseoMarutaEvents[] = {
	{ 0x202, 90, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoLogRollingStage_RunSceneEventIfReady) },
	{ 0x202, 91, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoLogRollingStage_FinishOrContinueSceneEvent) },
	{ EVENT_TOUCH, 11, 0x363, EVENT_SCRIPT(ColossoLogRollingStage_NudgeActorsLeft) },
	{ 0x202, 20, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoLogRollingStage_RunSetupCompletionHooks) },
	{ PSYNERGY_ON_ACTOR(0x8c), 12, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunFinalAuxiliarySequence) },
	{ PSYNERGY_ON_ACTOR(0x8c), 13, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoLogRollingStage_ConfigureActorThirteen) },
	{ 0x202, 22, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoLogRollingStage_RunSetupHook) },
	{ PSYNERGY_ON_ACTOR(0x8c), 14, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoLogRollingStage_NoopSetupHook) },
	{ 0x202, 23, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunEarlySequence) },
	{ PSYNERGY_ON_ACTOR(0x8c), 15, 0x214, EVENT_SCRIPT(ColossoLogRollingStage_ActivateClearObstacleActors) },
	{ PSYNERGY_ON_ACTOR(0x8c), 16, 0x215, EVENT_SCRIPT(ColossoLogRollingStage_ActivateClearObstacleActors) },
	{ PSYNERGY_ON_ACTOR(0x8c), 17, 0x216, EVENT_SCRIPT(ColossoLogRollingStage_ActivateClearObstacleActors) },
	{ EVENT_TOUCH, 15, 0x367, EVENT_SCRIPT(FieldScene_RunClosingAuxiliarySequence) },
	{ EVENT_TOUCH, 16, IF_FLAG_SET(0x367), EVENT_SCRIPT(ColossoLogRollingStage_ConfigureSecondaryObjects) },
	{ EVENT_TOUCH, 50, 0x210, EVENT_SCRIPT(KorosseoMaruta_RunStageStart) },
	{ SEARCH(SEARCH_CHEST), 101, 0x36a, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ SEARCH(SEARCH_CHEST), 102, 0x36b, FIND_ITEM(ITEM_NUT) },
	{ 0x8413, 103, 0x36c, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ 0x50001815, 9, 0x362, EVENT_SCRIPT(ColossoLogRollingStage_ConfigurePrimaryObjects) },
	{ PSYNERGY_ON_ACTOR(0xc), 29, 0x360, EVENT_SCRIPT(ColossoLogRollingStage_ConfigureGridRegion) },
	{ PSYNERGY_ON_ACTOR(0x91), 28, 0x361, EVENT_SCRIPT(ColossoLogRollingStage_WaitForSceneEventTask) },
	{ 0x50008e15, 30, 0x363, EVENT_SCRIPT(ColossoLogRollingStage_ConfigureSceneEventEffect) },
	{ EVENT_RAISED, 4, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoLogRollingStage_OffsetActiveActor) },
	{ EVENT_RAISED, 5, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoLogRollingStage_ClampAndOffsetActiveActor) },
	{ EVENT_TALK, 32, CONDITION_ALWAYS, EVENT_SCRIPT(RunPartyCountInteractionCopyB) },
	{ EVENT_TALK, 33, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunDualArrivalSequence) },
	{ EVENT_TALK, 34, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunSecondArrivalSequence) },
	{ EVENT_TALK, 35, CONDITION_ALWAYS, EVENT_SCRIPT(KorosseoMaruta_RunStageIntro) },
	{ EVENT_TALK, 36, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunFourStepActorMotion) },
	{ EVENT_TALK, 37, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunOpeningAuxiliarySequence) },
	{ EVENT_TALK, 38, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoLogRollingStage_RunLogRollingInteraction) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 32, CONDITION_ALWAYS, MSGNO(MsgKorosseoKawaThisYearsFinalsAreIncredibleThis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 33, CONDITION_ALWAYS, MSGNO(MsgKorosseoMarutaTheStreetVendorsCornOnThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 34, CONDITION_ALWAYS, MSGNO(MsgKorosseoMarutaICantSeeAnythingWhenIm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 35, CONDITION_ALWAYS, MSGNO(MsgKorosseoMarutaItsNotVeryExcitingWorkBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 36, CONDITION_ALWAYS, MSGNO(MsgKorosseoMarutaStandingForLongPeriodsOfTime) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 37, CONDITION_ALWAYS, MSGNO(MsgKorosseoMarutaIWasUpAllNightTrying) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 38, CONDITION_ALWAYS, MSGNO(MsgKorosseoMarutaSomeTouristToldMeILook) },
	{ EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoLogRollingStage_SelectNearestObstacle) },
	{ SCENE_EVENTS_END },
};

s32 KorosseoMaruta_Actor8Action[] = {
	SC_ADD(SW_SCALE_Y, 512),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 512),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 512),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -512),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -512),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -512),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(10),
	SC_LOOP(SC_FOREVER, SC_TOP),
	SC_END,
};

s32 KorosseoMaruta_LeaderActionA[] = {
	SC_ADD(SW_SCALE_Y, 512),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 512),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 512),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(6),
	SC_ADD(SW_SCALE_Y, -512),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -512),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -512),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(12),
	SC_LOOP(SC_FOREVER, SC_TOP),
	SC_END,
};

s32 KorosseoMaruta_LeaderActionB[] = {
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_Y, 3072),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 3072),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 3072),
	SC_WAIT(16),
	SC_ADD(SW_SCALE_Y, -3072),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -3072),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -3072),
	SC_WAIT(32),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

s32 KorosseoMaruta_LeaderActionC[] = {
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_Y, 3072),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 3072),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 3072),
	SC_WAIT(26),
	SC_ADD(SW_SCALE_Y, -3072),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -3072),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -3072),
	SC_WAIT(52),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

s32 gColossoMultiPhaseData[] = {
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_END,
};

s32 gColossoParticleKinds[] = {
	SC_CALL(ColossoLogRollingStage_AdvanceParticleMotion),
	SC_WAIT(1),
	SC_LOOP(60, SC_TOP),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

u16 Korosseo_PortraitSlot[] = {
	MAP_CELL_STEPS_END,
};

s16 gColossoModeScriptDefault[] = {
#if !EDITION_INTERNATIONAL
	0x4000, -188, 2048, 0x1000, 256, 1, 0x2000, 16,
#else
	0x4000, -188, 2048, 0x1000, 384, 1, 0x2000, 16,
#endif
	1, 0x7fff, 14, 0x3000, 0, 20, 0x7fff, 60,
	0x3000, 2048, 20, 0x7fff, 60, -1,
};

s16 gColossoModeScript4[] = {
#if defined(TBS_EDITION_FR)
	0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 384,
	6, 0x2000, 16, 6, 0x7fff, 6, 0x1000, 512,
	6, 0x7fff, 6, 0x1000, 384, 6, 0x7fff, 6,
	0x1000, 512, 6, 0x7fff, 6, 0x1000, 384, 6,
	0x7fff, 6, 0x2000, 0, 30, 0x1000, 512, 6,
	0x7fff, 6, 0x1000, 384, 6, 0x7fff, 6, 0x1000,
	512, 6, 0x7fff, 6, 0x1000, 384, 6, 0x7fff,
	6, 0x1000, 512, 6, 0x7fff, 6, -1,
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
	0x1000, 384, 1, 0x2000, 0, 1, 0x1000, 256,
	6, 0x2000, 16, 6, 0x7fff, 6, 0x1000, 384,
	6, 0x7fff, 6, 0x1000, 256, 6, 0x7fff, 6,
	0x1000, 384, 6, 0x7fff, 6, 0x1000, 256, 6,
	0x7fff, 6, 0x2000, 0, 30, 0x1000, 384, 6,
	0x7fff, 6, 0x1000, 256, 6, 0x7fff, 6, 0x1000,
	384, 6, 0x7fff, 6, 0x1000, 256, 6, 0x7fff,
	6, 0x1000, 384, 6, 0x7fff, 6, -1,
#else
	0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 256,
	6, 0x2000, 16, 6, 0x7fff, 6, 0x1000, 384,
	6, 0x7fff, 6, 0x1000, 256, 6, 0x7fff, 6,
	0x1000, 384, 6, 0x7fff, 6, 0x1000, 256, 6,
	0x7fff, 6, 0x2000, 0, 30, 0x1000, 384, 6,
	0x7fff, 6, 0x1000, 256, 6, 0x7fff, 6, 0x1000,
	384, 6, 0x7fff, 6, 0x1000, 256, 6, 0x7fff,
	6, 0x1000, 384, 6, 0x7fff, 6, -1,
#endif
};

s16 gColossoModeScript3A[] = {
	0x1000, 128, 1, 0x2000, 0, 1, 0x1000, 256,
	6, 0x2000, 16, 6, 0x7fff, 60, 0x1000, 384,
	6, 0x7fff, 6, 0x1000, 272, 6, 0x7fff, 6,
	0x1000, 400, 6, 0x7fff, 6, 0x1000, 288, 6,
	0x7fff, 6, 0x2000, 0, 30, 0x1000, 416, 6,
	0x7fff, 6, 0x1000, 304, 6, 0x7fff, 6, 0x1000,
	432, 6, 0x7fff, 6, 0x1000, 320, 6, 0x7fff,
	6, 0x1000, 448, 6, 0x7fff, 6, -1,
};

u16 Korosseo_MarkerSlot[] = {
	MAP_CELL_STEPS_END,
};

s32 Korosseo_RivalFinishScript[] = {
	SC_ARG(2),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(6),
	SC_CALL(ColossoLogRollingStage_RaiseLinkedSceneEffect),
	SC_WAIT(3),
	SC_ARG(10),
	SC_WAIT(150),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_ARG(1),
	SC_END,
};

s32 KorosseoMaruta_PlaceScript[] = {
	SC_ARG(2),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(6),
	SC_CALL(ColossoLogRollingStage_RaiseLinkedSceneEffect),
	SC_WAIT(3),
	SC_ARG(28),
	SC_WAIT(150),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_ARG(1),
	SC_END,
};

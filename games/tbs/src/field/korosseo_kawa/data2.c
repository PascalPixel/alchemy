#include "types.h"
#include "ability_ids.h"
#include "edition.h"
#include "field_event.h"
#include "field_scene.h"
#include "item_ids.h"
#include "objcmd.h"
#include "text/msg_ids.h"

void FieldScene_RunBranchedStep(void);
void FieldScene_RunNearestActor165Scene(void);
void FieldScene_RunTwoCallSequence(void);
void KorosseoKawa_RaisePipes(void);
void KorosseoKawa_RaisePlatformPair(void);
void KorosseoKawa_RunStageIntro(void);
void KorosseoKawa_RunStageStart(void);
void Korosseo_RunBrokenBridgeIntro(void);
void Korosseo_RunLumberWaterIntro(void);
void Korosseo_RunPipeworksIntro(void);
void RunPartyCountInteraction(void);
void SceneActor_PlaceLinkedActorAbove(void);
void SceneActor_ShiftActorEighteenByInputAndLeaderColumn(void);
void SceneActor_ShiftActorSeventeenByLeaderRow(void);
void SceneEffect_SpawnKind285AtRandomChance(void);
void SceneState_ApplyRectAndSend303(void);
void SceneState_ApplyRectsForActorsNineAndTen(void);

char HexDigits[20] = "0123456789ABCDEF";

s32 KorosseoKawa_ScriptA[] = {
	SC_ADD(SW_Y, -0x4000),
	SC_WAIT(1),
	SC_LOOP(60, SC_TOP),
	SC_FREE,
};

s32 KorosseoKawa_ScriptB[] = {
	SC_SET(SW_HOOK, (s32)SceneEffect_SpawnKind285AtRandomChance),
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

// 向きごとの一歩: 上位16ビットがX、下位16ビットがZ (ピクセル)
#define DIRECTION_STEP(x, z) ((u32)(x) << 16 | (u16)(z))

u32 KorosseoKawa_DirectionSteps[] = {
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(16, 0),
};

struct SceneEntrance KorosseoKawa_SceneTableA[] = {
	{ 0, CONDITION_ALWAYS, 920, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 312, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 KorosseoKawa_SceneTableB[] = {
	SCENE_EXITS(143),
	SCENE_EXIT(10, 143, 1),
	SCENE_EXIT(11, 140, 12),
	SCENE_EXIT(4, 140, 21),
	SCENE_EXIT(5, 138, 98),
	SCENE_EXITS_END,
};

struct ScenePlacement KorosseoKawa_SceneTableC[] = {
	{ 0x0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(312), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ MEARI, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(312), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(408), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(456), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe5, CONDITION_ALWAYS, 7,
		PIXELS(601), 0, PIXELS(146), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x122, CONDITION_ALWAYS, 7,
		PIXELS(552), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x122, CONDITION_ALWAYS, 7,
		PIXELS(664), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xee, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(760), 0, PIXELS(194), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(944), 0, PIXELS(216), FACING_WEST, TALK_KEEP_FACING, 0 },
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(1048), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(1016), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(360), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(376), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(600), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(824), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1080), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1304), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1304), 0, PIXELS(208), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

s32 KorosseoKawa_Countdown = 0;

struct SceneEvent KorosseoKawa_SceneTableD[] = {
	{ 0x202, 90, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunBranchedStep) },
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunTwoCallSequence) },
	{ 0x202, 11, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunTwoCallSequence) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 12, 0x302, EVENT_SCRIPT(KorosseoKawa_RaisePipes) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 20, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_ShiftActorSeventeenByLeaderRow) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 20, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_ShiftActorSeventeenByLeaderRow) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_ShiftActorEighteenByInputAndLeaderColumn) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_ShiftActorEighteenByInputAndLeaderColumn) },
	{ EVENT_TOUCH, 50, 0x210, EVENT_SCRIPT(KorosseoKawa_RunStageStart) },
	{ SEARCH(SEARCH_CHEST), 100, 0x308, FIND_ITEM(ITEM_NUT) },
	{ SEARCH(SEARCH_CHEST), 101, 0x309, FIND_ITEM(0xee) },
	{ 0x8413, 103, 0x30a, FIND_ITEM(ITEM_NUT) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyRectsForActorsNineAndTen) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyRectsForActorsNineAndTen) },
	{ PSYNERGY_ON_ACTOR(0x8c), 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyRectsForActorsNineAndTen) },
	{ PSYNERGY_ON_ACTOR(0xc), 15, 0x303, EVENT_SCRIPT(SceneState_ApplyRectAndSend303) },
	{ PSYNERGY_ON_ACTOR(0x21), 13, 0x301, EVENT_SCRIPT(KorosseoKawa_RaisePlatformPair) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(RunPartyCountInteraction) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(KorosseoKawa_RunStageIntro) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Korosseo_RunPipeworksIntro) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(Korosseo_RunBrokenBridgeIntro) },
	{ EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(Korosseo_RunLumberWaterIntro) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgKorosseoKawaThisYearsFinalsAreIncredibleThis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgKorosseoKawaMovingSuchAHugeRockMust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgKorosseoKawaTheresOnlyOneFinalAndThis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgKorosseoKawaDidYouSeeHisMovesOut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgKorosseoKawaHowManyWarriorsHaveSwallowedTheir) },
	{ EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunNearestActor165Scene) },
	{ SCENE_EVENTS_END },
};

s16 Korosseo_PortraitSlot = -1;

s16 KorosseoKawa_RoundSpans[] = {
#if !EDITION_INTERNATIONAL
	0x4000, -188, 2048, 0x1000, 256, 1, 0x2000, 16,
#else
	0x4000, -188, 2048, 0x1000, 384, 1, 0x2000, 16,
#endif
	1, 0x7fff, 14, 0x3000, 0, 20, 0x7fff, 60,
	0x3000, 2048, 20, 0x7fff, 60, -1,
};

s16 KorosseoKawa_ModeRecordFour[] = {
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

s16 KorosseoKawa_SpanA[] = {
	0x1000, 128, 1, 0x2000, 0, 1, 0x1000, 256,
	6, 0x2000, 16, 6, 0x7fff, 60, 0x1000, 384,
	6, 0x7fff, 6, 0x1000, 272, 6, 0x7fff, 6,
	0x1000, 400, 6, 0x7fff, 6, 0x1000, 288, 6,
	0x7fff, 6, 0x2000, 0, 30, 0x1000, 416, 6,
	0x7fff, 6, 0x1000, 304, 6, 0x7fff, 6, 0x1000,
	432, 6, 0x7fff, 6, 0x1000, 320, 6, 0x7fff,
	6, 0x1000, 448, 6, 0x7fff, 6, -1,
};

s16 Korosseo_MarkerSlot = -1;

s32 Korosseo_RivalFinishScript[] = {
	SC_ARG(2),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(6),
	SC_CALL(SceneActor_PlaceLinkedActorAbove),
	SC_WAIT(3),
	SC_ARG(10),
	SC_WAIT(150),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_ARG(1),
	SC_END,
};

s32 KorosseoKawa_ApproachScript[] = {
	SC_ARG(2),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(6),
	SC_CALL(SceneActor_PlaceLinkedActorAbove),
	SC_WAIT(3),
	SC_ARG(28),
	SC_WAIT(150),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_ARG(1),
	SC_END,
};

#include "types.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "objcmd.h"

void BabiFune_LoadTablesAndStopDma(void);
void BabiFune_RunDeparture(void);
void BabiFune_StoreActorOneHeight(void);
void BabiFune_StoreActorThreeHeight(void);
void BabiFune_StoreActorTwoHeight(void);
void BabiFune_StoreLeaderHeight(void);
void FieldScene_ConfigureFixedPointValues(void);
void PlayWorkspaceCueAndClearPaletteZero(void);

struct SceneEvent BabiFune_PaletteFrames[] = {
	{ 0x69c05860, 0x7f20, 0x69c0, 0x48005860 },
	{ 0x69c05860, 0x7f20, 0x69c0, 0x5860 },
};

s32 BabiFune_DriftScript[] = {
	SC_FREE,
};

u32 BabiFune_WaveSine[] = {
	0x640000, 0x12d00c8, 0x1f50191, 0x2bc0259, 0x381031f, 0x44403e3, 0x50404a5, 0x5c20563,
	0x67b061f, 0x73106d7, 0x7e2078a, 0x88f0839, 0x93608e3, 0x9d70987, 0xa730a26, 0xb080abe,
	0xb960b50, 0xc1d0bda, 0xc9d0c5e, 0xd140cd9, 0xd840d4d, 0xdeb0db9, 0xe4a0e1c, 0xea00e76,
	0xeed0ec8, 0xf310f10, 0xf6b0f4f, 0xf9c0f85, 0xfc30fb1, 0xfe10fd3, 0xff40fec, 0xffe0ffb,
	0xffe1000, 0xff40ffb, 0xfe10fec, 0xfc30fd3, 0xf9c0fb1, 0xf6b0f85, 0xf310f4f, 0xeed0f10,
	0xea00ec8, 0xe4a0e76, 0xdeb0e1c, 0xd840db9, 0xd140d4d, 0xc9d0cd9, 0xc1d0c5e, 0xb960bda,
	0xb080b50, 0xa730abe, 0x9d70a26, 0x9360987, 0x88f08e3, 0x7e20839, 0x731078a, 0x67b06d7,
	0x5c2061f, 0x5040563, 0x44404a5, 0x38103e3, 0x2bc031f, 0x1f50259, 0x12d0191, 0x6400c8,
	0xff9c0000, 0xfed3ff38, 0xfe0bfe6f, 0xfd44fda7, 0xfc7ffce1, 0xfbbcfc1d, 0xfafcfb5b, 0xfa3efa9d,
	0xf985f9e1, 0xf8cff929, 0xf81ef876, 0xf771f7c7, 0xf6caf71d, 0xf629f679, 0xf58df5da, 0xf4f8f542,
	0xf46af4b0, 0xf3e3f426, 0xf363f3a2, 0xf2ecf327, 0xf27cf2b3, 0xf215f247, 0xf1b6f1e4, 0xf160f18a,
	0xf113f138, 0xf0cff0f0, 0xf095f0b1, 0xf064f07b, 0xf03df04f, 0xf01ff02d, 0xf00cf014, 0xf002f005,
	0xf002f000, 0xf00cf005, 0xf01ff014, 0xf03df02d, 0xf064f04f, 0xf095f07b, 0xf0cff0b1, 0xf113f0f0,
	0xf160f138, 0xf1b6f18a, 0xf215f1e4, 0xf27cf247, 0xf2ecf2b3, 0xf363f327, 0xf3e3f3a2, 0xf46af426,
	0xf4f8f4b0, 0xf58df542, 0xf629f5da, 0xf6caf679, 0xf771f71d, 0xf81ef7c7, 0xf8cff876, 0xf985f929,
	0xfa3ef9e1, 0xfafcfa9d, 0xfbbcfb5b, 0xfc7ffc1d, 0xfd44fce1, 0xfe0bfda7, 0xfed3fe6f, 0xff9cff38,
};

struct SceneEntrance BabiFune_SceneTableA[] = {
	{ 0, CONDITION_ALWAYS, 408, 0, 200, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 312, 0, 296, FACING_NORTH, 0, -1, -1, -1, 328, 0 },
	{ SCENE_TABLE_END },
};

u32 BabiFune_SceneTableB[] = {
	SCENE_EXITS(188),
	SCENE_EXIT(1, 180, 31),
	SCENE_EXITS_END,
};

struct ScenePlacement BabiFune_SceneTableC[] = {
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ MEARI, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x1f6, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(312), 0, PIXELS(248), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x1f6, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(312), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent BabiFune_SceneTableD[] = {
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(PlayWorkspaceCueAndClearPaletteZero) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(BabiFune_LoadTablesAndStopDma) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_ConfigureFixedPointValues) },
	{ 0xf204, 10, CONDITION_ALWAYS, EVENT_SCRIPT(BabiFune_RunDeparture) },
	{ SCENE_EVENTS_END },
};

s32 BabiFune_ShimmerActive = 0;

s32 BabiFune_ShimmerPhase = 0;

s32 BabiFune_Count = 16;

s32 BabiFune_CountTicks = 0;

s32 BabiFune_DriftActive = 0;

s32 BabiFune_SwellActive = 0;

s32 BabiFune_SwellPhase = 0;

s32 BabiFune_SwellLayerSixY = 0;

s32 BabiFune_SwellLayerSevenY = 0;

s32 BabiFune_StoredSlot0 = -0x10000;

s32 BabiFune_StoredRecord1 = -0x10000;

s32 BabiFune_StoredSlot3 = -0x10000;

s32 BabiFune_StoredRecord2 = -0x10000;

u32 BabiFune_PaletteStep[] = {
	0,
};

s32 BabiFune_ActionScriptA[] = {
	SC_ARG(2),
	SC_MOVE_BY(PIXELS(-8), 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, PIXELS(-16)),
	SC_SYNC,
	SC_SET(SW_VELOCITY_Y, 0x80000),
	SC_SE(153),
	SC_MOVE_BY(0, 0, PIXELS(-24)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(20),
	SC_ARG(2),
	SC_MOVE_BY(PIXELS(-8), 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, PIXELS(-56)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(1),
	SC_CALL(BabiFune_StoreLeaderHeight),
	SC_END,
};

s32 BabiFune_ActionScriptB[] = {
	SC_ARG(2),
	SC_MOVE_BY(0, 0, PIXELS(-32)),
	SC_SYNC,
	SC_SET(SW_VELOCITY_Y, 0x80000),
	SC_SE(153),
	SC_MOVE_BY(0, 0, PIXELS(-24)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(20),
	SC_ARG(2),
	SC_MOVE_BY(PIXELS(-8), 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, PIXELS(-40)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(1),
	SC_CALL(BabiFune_StoreActorOneHeight),
	SC_END,
};

s32 BabiFune_ActionScriptC[] = {
	SC_ARG(2),
	SC_MOVE_BY(PIXELS(-20), 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, PIXELS(-32)),
	SC_SYNC,
	SC_SET(SW_VELOCITY_Y, 0x80000),
	SC_SE(153),
	SC_MOVE_BY(0, 0, PIXELS(-24)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(20),
	SC_ARG(2),
	SC_MOVE_BY(PIXELS(-8), 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, PIXELS(-24)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(1),
	SC_CALL(BabiFune_StoreActorTwoHeight),
	SC_END,
};

s32 BabiFune_ActionScriptD[] = {
	SC_ARG(2),
	SC_MOVE_BY(PIXELS(-8), 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, PIXELS(-8)),
	SC_SYNC,
	SC_SET(SW_VELOCITY_Y, 0x80000),
	SC_SE(153),
	SC_MOVE_BY(0, 0, PIXELS(-24)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(20),
	SC_ARG(2),
	SC_MOVE_BY(PIXELS(-8), 0, 0),
	SC_SYNC,
	SC_MOVE_BY(0, 0, PIXELS(-8)),
	SC_SYNC,
	SC_ARG(1),
	SC_CALL(BabiFune_StoreActorThreeHeight),
	SC_END,
};

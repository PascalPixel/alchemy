#include "types.h"
#include "field_scene.h"
#include "text/msg_ids.h"

void Func_02000054(void);
void Func_02000058(void);
void Func_0200008c(void);
void Func_02000128(void);
void Func_0200013c(void);
void Func_020004f4(void);

u32 gData_020032b0[] = {
	39, 2, 5, 0x200000, 0, 0,
	1, 5, 0, 0, 0x1c0000, 1,
	5, 0x300000, 0, 0, 1, 5,
	0xc0000, 0, 0xc0000, 1, 4, 0x1c80000,
	0, 0x1b80000, 1, 39, 1, 22,
	3, 0xc000, 0, 1, 17,
};

s32 gData_0200333c[] = {
	39, 2, 5, 0x200000, 0, 0,
	1, 5, 0xc0000, 0, -0xc0000, 1,
	5, 0x340000, 0, 0, 1, 5,
	0xc0000, 0, 0xc0000, 1, 4, 0x1d80000,
	0, 0x1b80000, 1, 39, 1, 22,
	3, 0xb000, 0, 1, 17,
};

u32 gData_020033c8[] = {
	39, 2, 4, 0x1d80000, 0, 0x1080000,
	1, 4, 0x1b80000, 0, 0x1080000, 1,
	38,
};

struct SceneEntrance gSceneEntrances[] = {
	{0, CONDITION_ALWAYS, 456, 0, 392, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 gSceneExits[] = {
	SCENE_EXITS(99),
	SCENE_EXIT_IF(1, 98, 2, 0xffffffff),
	SCENE_EXIT_IF(2, 99, 3, 0xffffffff),
	SCENE_EXIT_IF(3, 99, 2, 0xffffffff),
	SCENE_EXIT_IF(4, 99, 5, 0xffffffff),
	SCENE_EXIT_IF(5, 99, 4, 0xffffffff),
	SCENE_EXITS_END,
};

s16 gData_0200345c[] = {
	16, -1, 1, 0, 0, 360, 0, 0,
	0, 396, 0x4000, 1, 39, -1, 1, 0,
	0, 360, 0, 0, 0, 440, 0xc000, 1,
	37, -1, 1, 0, 0, 376, 0, 0,
	0, 88, 0xa000, 1, 38, -1, 1, 0,
	0, 392, 0, 0, 0, 136, 0x8000, 2,
	466, -1, 1, 0, 0, 376, 0, 0,
	0, 128, 0x4000, 1, 287, -1, 1, 0,
	0, 376, 0, 0, 0, 648, 0x4000, 2,
	288, -1, 1, 0, 0, 149, 0, 0,
	0, 475, 0x4000, 2, 489, -1, 1, 0,
	0, 149, 0, 0, 0, 472, 0x4000, 2,
	24, -1, 1, 0, 0, -56, 0, 0,
	0, -56, 0x4000, 1, 20, -1, 1, 0,
	0, -56, 0, 0, 0, -56, 0x4000, 1,
	19, -1, 1, 0, 0, -56, 0, 0,
	0, -56, 0x4000, 1, 88, -1, 1, 0,
	0, -56, 0, 0, 0, -56, 0x4000, 1,
	231, -1, 1, 0, 0, -56, 0, 0,
	0, -56, 0x4000, 1, 231, -1, 1, 0,
	0, -56, 0, 0, 0, -56, 0x4000, 1,
	39, -1, 1, 0, 0, -56, 0, 0,
	0, -56, 0x4000, 1, 39, -1, 1, 0,
	0, -56, 0, 0, 0, -56, 0x4000, 1,
	39, -1, 1, 0, 0, -56, 0, 0,
	0, -56, 0x4000, 1, 56, -1, 1, 0,
	0, -56, 0, 0, 0, -56, 0x4000, 2,
	5, -1, 1, 0, 0, -56, 0, 0,
	0, -56, 0x4000, 2, 6, -1, 1, 0,
	0, -56, 0, 0, 0, -56, 0x4000, 2,
	-1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0,
};

struct SceneEvent gSceneEvents[] = {
	{1, 1, CONDITION_ALWAYS, 1},
	{1, 2, CONDITION_ALWAYS, 2},
	{1, 3, CONDITION_ALWAYS, 3},
	{1, 4, CONDITION_ALWAYS, 4},
	{1, 5, CONDITION_ALWAYS, 5},
	{2, 20, 0x8aa, EVENT_SCRIPT(Func_020004f4)},
	{0, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Func_0200013c)},
	{0, 10, CONDITION_ALWAYS, MSGNO(MsgEoleoAndI)},
	{0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgIMGonna)},
	{0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgIfEoleoGets)},
	{0, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000128)},
	{0x10008c15, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000054)},
	{0x8c15, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000058)},
	{8, 30, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000054)},
	{9, 30, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000058)},
	{0x8715, 14, 0x901, EVENT_SCRIPT(Func_0200008c)},
	{SCENE_EVENTS_END},
};

u32 gData_02003720[] = {
	38,
};

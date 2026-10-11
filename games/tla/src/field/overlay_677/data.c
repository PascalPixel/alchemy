#include "types.h"
#include "field_scene.h"

u32 Data_02000a98[] = {
	22, 30, 131, 22, 16, 0x1999,
	22, 15, 0x4ccc, 0x80010000, 39, 2,
	4, 0x780000, 0, 0x680000, 1, 4,
	0x800000, 0, 0x5c0000, 1, 0, 20,
	39, 1, 0, 20, 39, 2,
	4, 0x780000, 0, 0x680000, 1, 4,
	0x580000, 0, 0x680000, 1, 0, 20,
	39, 1, 0, 20, 13, 0xffff,
	0xc0010000, 17, 22, 30, 129, 22,
	16, 0x1999, 22, 15, 0x4ccc, 0x80010000,
	39, 2, 4, 0x1b50000, 0, 0x7f0000,
	1, 0, 20, 39, 1, 22,
	3, 0x2000, 0, 60, 39, 2,
	4, 0x1c50000, 0, 0x720000, 1, 0,
	20, 39, 1, 22, 3, 0x2000,
	0, 60, 13, 0xffff, 0xc0010000, 17,
};

struct SceneEntrance gSceneEntrances[] = {
	{0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 gSceneExits[] = {
	SCENE_EXITS(141),
	SCENE_EXIT_IF(1, 139, 1, 0xffffffff),
	SCENE_EXIT_IF(2, 139, 2, 0xffffffff),
	SCENE_EXIT_IF(3, 139, 7, 0xffffffff),
	SCENE_EXIT_IF(4, 139, 4, 0xffffffff),
	SCENE_EXITS(142),
	SCENE_EXIT_IF(1, 139, 3, 0xffffffff),
	SCENE_EXIT_IF(2, 139, 6, 0xffffffff),
	SCENE_EXIT_IF(3, 139, 5, 0xffffffff),
	SCENE_EXITS_END,
};

u32 Data_02000c8c[] = {
	0xffff, 0, 0, 0, 0, 0,
};

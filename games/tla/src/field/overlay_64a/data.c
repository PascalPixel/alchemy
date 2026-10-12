#include "types.h"
#include "field_scene.h"

u32 Data_02002410[] = {
	0x40004, 4,
};

u16 Data_02002418[] = {
	64, 128, 64, 128, 32, 16, 32, 16,
	64, 16, 128, 32, 64, 4, 0, 0,
};

u16 Data_02002438[] = {
	256, 770, 515, 1,
};

u32 Data_02002440[] = {
	0x44434241, 0x47462045, 0x204b4a48, 0x504e4d4c, 0x522051,
};

u32 Data_02002454[] = {
	0x56555453, 0x59582057, 0x2033325a, 0x37363534, 0x392038,
};

u32 Data_02002468[] = {
	0x64636261, 0x67662065, 0x206a6968, 0x706e6d6b, 0x722071,
};

u32 Data_0200247c[] = {
	0x76757473, 0x79782077, 0x203f217a, 0x25242623, 0x3d202b,
};

u32 Data_02002490[] = {
	0x9510941, 0x8d108b3, 0x868081e,
};

struct SceneEntrance gSceneEntrances[] = {
	{0, CONDITION_ALWAYS, 0, 0, 0, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 gSceneExits[] = {
	SCENE_EXITS_END,
};

struct ScenePlacement gScenePlacements[] = {
	{SCENE_TABLE_END},
};

struct SceneEvent gSceneEvents[] = {
	{SCENE_EVENTS_END},
};

u16 Data_020024f4[] = {
	0,
};

u16 Data_020024f6[] = {
	0,
};

u16 Data_020024f8[] = {
	0,
};

u16 Data_020024fa[] = {
	0,
};

u16 Data_020024fc[] = {
	0,
};

u16 Data_020024fe[] = {
	0,
};

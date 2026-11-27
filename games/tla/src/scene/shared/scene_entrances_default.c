// The entrance table of most overlays, which holds only the default entrance: at 172, 149, facing
// south, with no room bounds. The ROM keeps a copy in each of them.
//
// An includer whose overlay places the table after its pictures defines SCENE_ENTRANCES_SECTION as
// LATE_DATA first.

#ifndef SCENE_ENTRANCES_SECTION
#	define SCENE_ENTRANCES_SECTION
#endif

// The default entrance alone.
SCENE_ENTRANCES_SECTION SCENE_ENTRANCE SceneEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

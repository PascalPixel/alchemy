/* The entrance table of the screens with no map, which the title, the clear */
/* screen and the epilogue keep a copy of: the default entrance alone, at the */
/* origin, facing south. */
SCENE_ENTRANCE SceneEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 0, 0, 0, FACING_SOUTH, 0, -1, -1, -1,
	 -1, 0},
	{SCENE_TABLE_END},
};

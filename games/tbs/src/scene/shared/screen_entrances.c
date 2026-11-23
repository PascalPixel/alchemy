/// @file
/// The title and the File Screen, scenes that are no place (empty_map): one
/// entrance at the map's corner, which nothing walks from.

struct SceneEntrance gScreen_Entrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 0, 0, 0, FACING_SOUTH, 0, -1, -1, -1,
     -1, 0},
    {SCENE_TABLE_END},
};

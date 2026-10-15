#include "types.h"
#include "field_scene.h"

struct SceneEntrance gSceneEntrances[] = {
    { 0, CONDITION_ALWAYS, 0, 0, 0, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gSceneExits[] = {
    SCENE_EXITS_END,
};

struct ScenePlacement gScenePlacements[] = {
    { SCENE_TABLE_END },
};

struct SceneEvent gSceneEvents[] = {
    { SCENE_EVENTS_END },
};

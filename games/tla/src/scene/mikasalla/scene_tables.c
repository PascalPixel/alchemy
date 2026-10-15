#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_scene.h"

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gSceneEntrances;
}

#include "party_state.h"
#include "scene_ids.h"

extern s16 gData_02000794[];

s16 *Func_02000040(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov65dRoom2)
        return gData_02000794;
    return 0;
}

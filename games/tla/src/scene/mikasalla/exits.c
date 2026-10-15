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

const u32 *Scene_GetExits(void)
{
    return gSceneExits;
}

#include "party_state.h"
#include "scene_ids.h"

extern s32 gData_02000fa0[];
extern u32 gData_020010c0[];

void *Func_02000070(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov65dRoom1)
        return gData_02000fa0;
    return gData_020010c0;
}

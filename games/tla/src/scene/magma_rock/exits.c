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

#include "scene_ids.h"

extern const u8 Data_0200403c[];
extern const u8 Data_020041a4[];
extern const u8 Data_0200427c[];
extern const u8 Data_020043e4[];
extern const u8 Data_020044a4[];
extern const u8 Data_020045c4[];

const struct SceneEvent *Func_020008d4(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov691Room2) {
        if (gPartyState.entrance <= 3)
            return (const struct SceneEvent *)Data_020041a4;
        if (gPartyState.entrance <= 8)
            return (const struct SceneEvent *)Data_0200427c;
        if (gPartyState.entrance <= 12)
            return (const struct SceneEvent *)Data_020043e4;
        return (const struct SceneEvent *)Data_020044a4;
    }
    if (gPartyState.scene == (s32)&SceneId_Ov691Room3)
        return (const struct SceneEvent *)Data_020045c4;
    return (const struct SceneEvent *)Data_0200403c;
}

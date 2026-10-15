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
#include "scene_ids.h"

extern const u8 Data_02003f08[];
extern const u8 Data_02003f20[];
extern const u8 Data_02003fe0[];
extern const u8 Data_02004058[];
extern const u8 Data_02004130[];
extern const u8 Data_020042c8[];
extern const u8 Data_02004388[];
extern const u8 Data_02004400[];
extern const u8 Data_020045f8[];
extern const u8 Data_02004658[];
extern const u8 Data_02004664[];
extern const u8 Data_020046c4[];
extern const u8 Data_020046f4[];
extern const u8 Data_02004790[];
extern const u8 Data_020047cc[];
extern const u8 Data_020048ec[];
extern const u8 Data_02004928[];
extern const u8 Data_020049a0[];
extern const u8 Data_020049f4[];


const u32 *Scene_GetExits(void)
{
    return gSceneExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_ShrineOfTheSeaGod1)
        return (const struct ScenePlacement *)Data_02003f20;
    if (scene == (s32)&SceneId_ShrineOfTheSeaGod2)
        return (const struct ScenePlacement *)Data_02003fe0;
    if (scene == (s32)&SceneId_ShrineOfTheSeaGod3)
        return (const struct ScenePlacement *)Data_02004058;
    if (scene == (s32)&SceneId_ShrineOfTheSeaGod4)
        return (const struct ScenePlacement *)Data_02004130;
    if (scene == (s32)&SceneId_ShrineOfTheSeaGod5)
        return (const struct ScenePlacement *)Data_020042c8;
    if (scene == (s32)&SceneId_ShrineOfTheSeaGod6)
        return (const struct ScenePlacement *)Data_02004388;
    if (scene == (s32)&SceneId_ShrineOfTheSeaGod7)
        return (const struct ScenePlacement *)Data_02004400;
    if (scene == (s32)&SceneId_ShrineOfTheSeaGod8)
        return (const struct ScenePlacement *)Data_020045f8;
    return (const struct ScenePlacement *)Data_02003f08;
}

const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_ShrineOfTheSeaGod1)
        return (const struct SceneEvent *)Data_02004664;
    if (scene == (s32)&SceneId_ShrineOfTheSeaGod2)
        return (const struct SceneEvent *)Data_020046c4;
    if (scene == (s32)&SceneId_ShrineOfTheSeaGod3)
        return (const struct SceneEvent *)Data_020046f4;
    if (scene == (s32)&SceneId_ShrineOfTheSeaGod4)
        return (const struct SceneEvent *)Data_02004790;
    if (scene == (s32)&SceneId_ShrineOfTheSeaGod5)
        return (const struct SceneEvent *)Data_020047cc;
    if (scene == (s32)&SceneId_ShrineOfTheSeaGod6)
        return (const struct SceneEvent *)Data_020048ec;
    if (scene == (s32)&SceneId_ShrineOfTheSeaGod7)
        return (const struct SceneEvent *)Data_02004928;
    if (scene == (s32)&SceneId_ShrineOfTheSeaGod8)
        return (const struct SceneEvent *)Data_020049a0;
    if (scene == (s32)&SceneId_ShrineOfTheSeaGod9)
        return (const struct SceneEvent *)Data_020049f4;
    return (const struct SceneEvent *)Data_02004658;
}

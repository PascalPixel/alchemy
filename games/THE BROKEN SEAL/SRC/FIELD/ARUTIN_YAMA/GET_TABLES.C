#include "YAMA.H"

extern const struct SceneEntrance gArutinYamaEntrancesOther[];
extern const struct SceneEntrance gArutinYamaEntrances1[];
extern const struct SceneEntrance gArutinYamaEntrances2[];
extern const struct SceneEntrance gArutinYamaEntrances3[];
extern const struct SceneEntrance gArutinYamaEntrances4[];
extern const struct SceneEntrance gArutinYamaEntrances5[];
extern const struct SceneEntrance gArutinYamaEntrances6[];
extern const struct SceneEntrance gArutinYamaEntrances7[];
extern const struct SceneEntrance gArutinYamaEntrances8[];
extern const struct SceneEntrance gArutinYamaEntrances9[];
extern const struct SceneEntrance gArutinYamaEntrances10[];
extern const struct SceneEntrance gArutinYamaEntrances11[];
extern const struct SceneRegion gArutinYamaRegions9[];
extern const struct SceneRegion gArutinYamaRegions10[];

/* Where the party appears in each of Altin Peak's eleven areas. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_ArutinYama1) {
        return gArutinYamaEntrances1;
    }
    if (scene == (s32)&SceneId_ArutinYama2) {
        return gArutinYamaEntrances2;
    }
    if (scene == (s32)&SceneId_ArutinYama3) {
        return gArutinYamaEntrances3;
    }
    if (scene == (s32)&SceneId_ArutinYama4) {
        return gArutinYamaEntrances4;
    }
    if (scene == (s32)&SceneId_ArutinYama5) {
        return gArutinYamaEntrances5;
    }
    if (scene == (s32)&SceneId_ArutinYama6) {
        return gArutinYamaEntrances6;
    }
    if (scene == (s32)&SceneId_ArutinYama7) {
        return gArutinYamaEntrances7;
    }
    if (scene == (s32)&SceneId_ArutinYama8) {
        return gArutinYamaEntrances8;
    }
    if (scene == (s32)&SceneId_ArutinYama9) {
        return gArutinYamaEntrances9;
    }
    if (scene == (s32)&SceneId_ArutinYama10) {
        return gArutinYamaEntrances10;
    }
    if (scene == (s32)&SceneId_ArutinYama11) {
        return gArutinYamaEntrances11;
    }
    return gArutinYamaEntrancesOther;
}

/* Only the ninth and tenth areas have map regions. */
const struct SceneRegion *Scene_GetRegions(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_ArutinYama9) {
        return gArutinYamaRegions9;
    }
    if (scene == (s32)&SceneId_ArutinYama10) {
        return gArutinYamaRegions10;
    }
    return 0;
}

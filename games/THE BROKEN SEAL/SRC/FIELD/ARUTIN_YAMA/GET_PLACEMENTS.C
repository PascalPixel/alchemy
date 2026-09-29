#include "YAMA.H"

extern const struct ScenePlacement gArutinYamaPlacementsOther[];
extern const struct ScenePlacement gArutinYamaPlacements1[];
extern const struct ScenePlacement gArutinYamaPlacements3[];
extern const struct ScenePlacement gArutinYamaPlacements5[];
extern const struct ScenePlacement gArutinYamaPlacements6[];
extern const struct ScenePlacement gArutinYamaPlacements7[];
extern const struct ScenePlacement gArutinYamaPlacements8[];
extern const struct ScenePlacement gArutinYamaPlacements9[];
extern const struct ScenePlacement gArutinYamaPlacements10[];
extern const struct ScenePlacement gArutinYamaPlacements11[];

/* The actors placed in Altin Peak's areas; the second and fourth areas
   take the table the other scenes take. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_ArutinYama1) {
        return gArutinYamaPlacements1;
    }
    if (scene == (s32)&SceneId_ArutinYama3) {
        return gArutinYamaPlacements3;
    }
    if (scene == (s32)&SceneId_ArutinYama5) {
        return gArutinYamaPlacements5;
    }
    if (scene == (s32)&SceneId_ArutinYama6) {
        return gArutinYamaPlacements6;
    }
    if (scene == (s32)&SceneId_ArutinYama7) {
        return gArutinYamaPlacements7;
    }
    if (scene == (s32)&SceneId_ArutinYama8) {
        return gArutinYamaPlacements8;
    }
    if (scene == (s32)&SceneId_ArutinYama9) {
        return gArutinYamaPlacements9;
    }
    if (scene == (s32)&SceneId_ArutinYama10) {
        return gArutinYamaPlacements10;
    }
    if (scene == (s32)&SceneId_ArutinYama11) {
        return gArutinYamaPlacements11;
    }
    return gArutinYamaPlacementsOther;
}

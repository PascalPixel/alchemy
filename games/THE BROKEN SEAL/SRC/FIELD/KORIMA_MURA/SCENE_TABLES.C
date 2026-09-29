#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KORIMA_MURA.H"

extern const struct SceneEntrance gKorimaMuraEntrances[];
extern const struct SceneEntrance gKorimaMuraEntrances2[];
extern const struct SceneEntrance gKorimaMuraEntrances3[];
extern const struct SceneRegion gKorimaMuraRegions2[];
extern const u32 gKorimaMuraExits[];
extern const struct ScenePlacement gKorimaMuraPlacements[];
extern const struct ScenePlacement gKorimaMuraPlacements1[];
extern const struct ScenePlacement gKorimaMuraPlacements3[];

void SceneData_InitRecordTable(const struct ScenePlacement *placements);

/* Where the party appears in each of Kori's scenes. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KorimaMura3) {
        return gKorimaMuraEntrances3;
    }
    if (scene == (s32)&SceneId_KorimaMura2) {
        return gKorimaMuraEntrances2;
    }
    return gKorimaMuraEntrances;
}

/* Only the second scene has map regions. */
const struct SceneRegion *Scene_GetRegions(void)
{
    if (gGameState.scene == (s32)&SceneId_KorimaMura2) {
        return gKorimaMuraRegions2;
    }
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gKorimaMuraExits;
}

/* The actors placed in each scene; the first scene's table is prepared
   while flag 0x845 is clear. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s32 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KorimaMura1) {
        if (GameFlag_IsSet(0x845) == 0) {
            SceneData_InitRecordTable(gKorimaMuraPlacements1);
        }
        return gKorimaMuraPlacements1;
    }
    if (scene == (s32)&SceneId_KorimaMura3) {
        return gKorimaMuraPlacements3;
    }
    return gKorimaMuraPlacements;
}

#include "SORU.H"

extern const struct ScenePlacement gSoruIriguchiPlacementsOther[];
extern const struct ScenePlacement gSoruIriguchiPlacements1[];
extern const struct ScenePlacement gSoruIriguchiPlacements1Entrances11To13[];
extern const struct ScenePlacement gSoruIriguchiPlacements1Entrances14To16[];
extern const struct ScenePlacement gSoruIriguchiPlacements2[];
extern const struct SceneEvent gSoruIriguchiEventsOther[];
extern const struct SceneEvent gSoruIriguchiEvents1[];
extern const struct SceneEvent gSoruIriguchiEvents1Entrances11To13[];
extern const struct SceneEvent gSoruIriguchiEvents1Entrances14To16[];
extern const struct SceneEvent gSoruIriguchiEvents2[];

void FieldScene_PrepareActors(s32 placements);

/* The actors placed at the sanctum's entrance. In the first scene the
   entrances 11 to 13 and 14 to 16 have their own tables; any other entrance
   takes a table that is prepared first. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s32 table;
    s32 lo = 11;

    if (gGameState.scene == (s32)&SceneId_SoruIriguchi1) {
        if (gGameState.entrance >= lo) {
            if (gGameState.entrance > 13) {
                if (gGameState.entrance > 16) {
                    goto other_entrance;
                }
                return gSoruIriguchiPlacements1Entrances14To16;
            }
            return gSoruIriguchiPlacements1Entrances11To13;
        }
    other_entrance:;
        table = (s32)gSoruIriguchiPlacements1;
        FieldScene_PrepareActors(table);
        return (const struct ScenePlacement *)table;
    } else {
        if (gGameState.scene == (s32)&SceneId_SoruIriguchi2) {
            return gSoruIriguchiPlacements2;
        }
    }
    return gSoruIriguchiPlacementsOther;
}

/* What the sanctum's entrance answers, chosen as its placements are. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s32 lo = 11;

    if (gGameState.scene == (s32)&SceneId_SoruIriguchi2) {
        return gSoruIriguchiEvents2;
    } else {
        if (gGameState.scene == (s32)&SceneId_SoruIriguchi1) {
            if (gGameState.entrance >= lo) {
                if (gGameState.entrance > 13) {
                    if (gGameState.entrance > 16) {
                        goto other_entrance;
                    }
                    return gSoruIriguchiEvents1Entrances14To16;
                }
                return gSoruIriguchiEvents1Entrances11To13;
            }
        other_entrance:;
            return gSoruIriguchiEvents1;
        } else {
        }
    }
    return gSoruIriguchiEventsOther;
}

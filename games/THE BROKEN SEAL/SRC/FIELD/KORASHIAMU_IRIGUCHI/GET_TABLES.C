#include "STATUS.H"

extern const struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance5[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance7[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance8[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance12[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements1Entrance66[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements1[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements3Flag950[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements3Flag962[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacements3[];
extern const struct ScenePlacement gKorashiamuIriguchiPlacementsOther[];
extern const struct SceneEvent gKorashiamuIriguchiEvents2[];
extern const struct SceneEvent gKorashiamuIriguchiEvents1Entrance12[];
extern const struct SceneEvent gKorashiamuIriguchiEvents1[];
extern const struct SceneEvent gKorashiamuIriguchiEvents3[];
extern const struct SceneEvent gKorashiamuIriguchiEventsOther[];

/* The actors placed at the Colosso entrance. In the first scene the
   entrance the party came by picks the table; in the third, flags 0x950
   and 0x962. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    const struct ScenePlacement *table;

    if (gGameState.scene == (s32)&SceneId_KorashiamuIriguchi1) {
        switch (gGameState.entrance) {
        case 5:
        case 69:
            table = gKorashiamuIriguchiPlacements1Entrance5;
            break;
        case 7:
        case 70:
            table = gKorashiamuIriguchiPlacements1Entrance7;
            break;
        case 8:
        case 21:
        case 31:
        case 64:
        case 65:
        case 67:
            table = gKorashiamuIriguchiPlacements1Entrance8;
            break;
        case 12:
            table = gKorashiamuIriguchiPlacements1Entrance12;
            break;
        case 66:
        case 68:
            table = gKorashiamuIriguchiPlacements1Entrance66;
            break;
        default:
            table = gKorashiamuIriguchiPlacements1;
            break;
        }
    } else if (gGameState.scene == (s32)&SceneId_KorashiamuIriguchi3) {
        if (GameFlag_IsSet(0x950) != 0) {
            table = gKorashiamuIriguchiPlacements3Flag950;
        } else if (GameFlag_IsSet(0x962) != 0) {
            table = gKorashiamuIriguchiPlacements3Flag962;
        } else {
            table = gKorashiamuIriguchiPlacements3;
        }
    } else {
        table = gKorashiamuIriguchiPlacementsOther;
    }
    return table;
}

/* What the Colosso entrance answers; entrance 12 of the first scene has its
   own events. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KorashiamuIriguchi2) {
        return gKorashiamuIriguchiEvents2;
    }
    if (scene == (s32)&SceneId_KorashiamuIriguchi1) {
        if (gGameState.entrance == 12) {
            return gKorashiamuIriguchiEvents1Entrance12;
        }
        return gKorashiamuIriguchiEvents1;
    }
    if (scene == (s32)&SceneId_KorashiamuIriguchi3) {
        return gKorashiamuIriguchiEvents3;
    }
    return gKorashiamuIriguchiEventsOther;
}

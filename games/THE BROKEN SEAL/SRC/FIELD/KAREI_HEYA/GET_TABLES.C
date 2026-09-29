#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement gKareiHeyaPlacements1Entrance9[];
extern const struct ScenePlacement gKareiHeyaPlacements1[];
extern const struct ScenePlacement gKareiHeyaPlacements2[];
extern const struct ScenePlacement gKareiHeyaPlacementsOther[];
extern const struct SceneEvent gKareiHeyaEvents1Entrance9[];
extern const struct SceneEvent gKareiHeyaEvents1[];
extern const struct SceneEvent gKareiHeyaEvents2[];
extern const struct SceneEvent gKareiHeyaEventsOther[];

void FieldScene_PrepareActors(const struct ScenePlacement *placements);

/* The actors placed in the Kalay houses. In the first scene the entrances
   9 to 15 and 17 have their own table; the first scene's table is prepared
   before it is returned. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s32 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KareiHeya1) {
        s32 entrance = gGameState.entrance;
        const struct ScenePlacement *table;

        switch (entrance) {
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 17:
            table = gKareiHeyaPlacements1Entrance9;
            break;
        default:
            table = gKareiHeyaPlacements1;
            break;
        }
        FieldScene_PrepareActors(table);
        return table;
    }
    if (scene == (s32)&SceneId_KareiHeya2) {
        return gKareiHeyaPlacements2;
    }
    return gKareiHeyaPlacementsOther;
}

/* What the Kalay houses answer, chosen as their placements are. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s32 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KareiHeya1) {
        s32 entrance = gGameState.entrance;

        switch (entrance) {
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 17:
            return gKareiHeyaEvents1Entrance9;
        default:
            return gKareiHeyaEvents1;
        }
    }
    if (scene == (s32)&SceneId_KareiHeya2) {
        return gKareiHeyaEvents2;
    }
    return gKareiHeyaEventsOther;
}

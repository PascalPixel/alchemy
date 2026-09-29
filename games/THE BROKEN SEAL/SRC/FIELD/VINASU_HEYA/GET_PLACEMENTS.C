#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 gVinasuHeyaPlacements1[];
extern u8 gVinasuHeyaPlacements2[];
extern u8 gVinasuHeyaPlacements3[];
extern u8 gVinasuHeyaPlacements4[];
extern u8 gVinasuHeyaPlacements5[];
extern u8 gVinasuHeyaPlacements6[];
extern u8 gVinasuHeyaPlacementsOther[];

void FieldScene_PrepareActors(u8 *placements);

/* The actors placed in each room. Every room but the first has its table
   prepared before it is returned. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    u8 *ret;
    s16 v;

    v = gGameState.scene;
    if (v == (s32)&SceneId_VinasuHeya1) {
        return (const struct ScenePlacement *)gVinasuHeyaPlacements1;
    }
    if (v == (s32)&SceneId_VinasuHeya2) {
        ret = gVinasuHeyaPlacements2;
    } else if (v == (s32)&SceneId_VinasuHeya3) {
        ret = gVinasuHeyaPlacements3;
    } else if (v == (s32)&SceneId_VinasuHeya4) {
        ret = gVinasuHeyaPlacements4;
    } else if (v == (s32)&SceneId_VinasuHeya5) {
        ret = gVinasuHeyaPlacements5;
    } else if (v == (s32)&SceneId_VinasuHeya6) {
        ret = gVinasuHeyaPlacements6;
    } else {
        goto no_match;
    }
    FieldScene_PrepareActors(ret);
    return (const struct ScenePlacement *)ret;

no_match:
    return (const struct ScenePlacement *)gVinasuHeyaPlacementsOther;
}

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

void FieldScene_PrepareActors(struct ScenePlacement *placements);

extern const struct ScenePlacement gShianJiinPlacements1[];
extern const struct ScenePlacement gShianJiinPlacementsEntrance3[];
extern struct ScenePlacement gShianJiinPlacements[];

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* The actors placed in the temple: the first scene and the third entrance
   have their own tables. Once flag 0x895 is set, the gated actors of the
   other table take that flag and one of them moves; that table is prepared
   before it is returned. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_ShianJiin1) {
        return gShianJiinPlacements1;
    }
    if (gGameState.entrance == 3) {
        return gShianJiinPlacementsEntrance3;
    }
    if (Value1(Engine_GameFlagIsSet, 0x895)) {
        gShianJiinPlacements[5].condition = 0x895;
        gShianJiinPlacements[7].condition = 0x895;
        gShianJiinPlacements[8].x = 0x1200000;
        gShianJiinPlacements[8].z = 0xf80000;
        gShianJiinPlacements[11].condition = 0x895;
        gShianJiinPlacements[12].condition = 0x895;
    }
    FieldScene_PrepareActors(gShianJiinPlacements);
    return gShianJiinPlacements;
}

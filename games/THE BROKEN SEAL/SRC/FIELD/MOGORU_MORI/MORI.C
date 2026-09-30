#include "MORI.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gMogoruMoriEntrances1[];
extern const struct SceneEntrance gMogoruMoriEntrances2[];
extern const struct SceneEntrance gMogoruMoriEntrances3[];
extern const struct SceneEntrance gMogoruMoriEntrancesOther[];

extern const struct ScenePlacement gMogoruMoriPlacements1[];
extern const struct ScenePlacement gMogoruMoriPlacements2[];
extern const struct ScenePlacement gMogoruMoriPlacements3[];
extern const struct ScenePlacement gMogoruMoriPlacementsOther[];

void FieldScene_RunSixCallSetupSequence(s32 no, s32 val)
{
    s32 v0 = 0x20000;
    s32 v1 = 0x4000;

    Camera_SetSpeed(v0, v1);
    Camera_MoveToActor(no, 1);
    Camera_WaitForMove();
    Battle_WaitMode0(30);
    MogoruMori_SpawnPuffRing(no);
    Actor_SetChildValue(no, val);
}

/* Where the party appears in each of the forest's three areas. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_MogoruMori1) {
        return gMogoruMoriEntrances1;
    }
    if (selector == (s32)&SceneId_MogoruMori2) {
        return gMogoruMoriEntrances2;
    }
    if (selector == (s32)&SceneId_MogoruMori3) {
        return gMogoruMoriEntrances3;
    }
    return gMogoruMoriEntrancesOther;
}

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* Complete eight-byte literal-address getter, including its sole pool word. */
u8 *SceneData_GetTableB5bc(void)
{
    return MogoruMori_SceneTable;
}

/* The actors placed in each area. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_MogoruMori1) {
        return gMogoruMoriPlacements1;
    }
    if (selector == (s32)&SceneId_MogoruMori2) {
        return gMogoruMoriPlacements2;
    }
    if (selector == (s32)&SceneId_MogoruMori3) {
        return gMogoruMoriPlacements3;
    }
    return gMogoruMoriPlacementsOther;
}

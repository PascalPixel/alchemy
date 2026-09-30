#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "GOMA.H"
#include "CALL.H"
#include "SCENE_IDS.H"

extern const struct SceneEntrance gGomaSuiroEntrances2[];
extern const struct SceneEntrance gGomaSuiroEntrancesOther[];

extern const u32 GomaSuiro_Exits[];

extern const struct ScenePlacement gGomaSuiroPlacements2[];
extern const struct ScenePlacement gGomaSuiroPlacementsOther[];

void Engine_ActorSetSpriteFlags();
void Map_CopyCellAttributeRect();
void GameFlag_SetBit();
void SceneEffect_RunActorBurst(s32 no);

static __inline__ void SetFlagBits(u8 *flags, u8 bits)
{
    *flags |= bits;
}

extern const struct SceneEvent gGomaSuiroEvents2[];
extern const struct SceneEvent gGomaSuiroEventsOther[];

void Object_SetModeById();

/* Where the party appears; the second area has its own entrances. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    if (gGameState.scene == (s32)&SceneId_GomaSuiro2) {
        return gGomaSuiroEntrances2;
    }
    return gGomaSuiroEntrancesOther;
}

/* The waterway's regions and exits, between its scene-dependent getters. */
const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return GomaSuiro_Exits;
}

/* The actors placed; the second area has its own. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_GomaSuiro2) {
        return gGomaSuiroPlacements2;
    }
    return gGomaSuiroPlacementsOther;
}

void FieldScene_RunActor8AtCell24Sequence(void)
{
    s32 *record;
    u8 *target;
    s32 value;

    record = (s32 *)Object_GetById(8);
    value = record[2] / 0x100000;
    if (value == 24) {
        SceneEffect_RunActorBurst(8);
        SetFlagBits((u8 *)Object_GetById(8) + 35, 2);
        Call6(Map_CopyCellAttributeRect, 19, 74, 9, 3, 19, 17);
        target = (u8 *)Object_GetById(8);
        Engine_ActorSetSpriteFlags((s32)target, 0);
        GameFlag_SetBit(0x864);
    }
}

/* What the waterway answers; the second area has its own events. */
const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_GomaSuiro2) {
        return gGomaSuiroEvents2;
    }
    return gGomaSuiroEventsOther;
}

/* The waterway's scene start: open with the window transition; in the
   first area, entering by the fifth entrance clears flag 0x12f, and
   otherwise actor 8 is set up, placed where flag 0x864 records it. */
s32 FieldScene_PlaceActor8OnEntry(void)
{
    struct FieldActor *record;

    gEventWork->start_transition = 0x204;
    if (gGameState.scene == (s32)&SceneId_GomaSuiro1) {
        if (gGameState.entrance == 5) {
            Engine_GameFlagClear(0x12f);
        } else {
            SetFlagBits((u8 *)Object_GetById(8) + 89, 16);
            if (Engine_GameFlagIsSet(0x864) != 0) {
                Engine_ActorSetPosition(8, 0x15a0000, 0x1240000);
                record = Object_GetById(8);
                Engine_ActorSetSpriteFlags(record, 0);
                *((u8 *)Object_GetById(8) + 35) |= 2;
                Object_SetModeById(8, 2);
                Call6(Map_CopyCellAttributeRect, 19, 74, 9, 3, 19, 17);
            }
        }
    }
    return 0;
}

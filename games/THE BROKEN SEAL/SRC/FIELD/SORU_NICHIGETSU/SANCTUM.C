#include "SANCTUM.H"

/* The scene's tables, which the entry veneers publish to the map engine. */
extern const struct SceneEntrance gSoruNichigetsuEntrances[];
extern const u32 gSoruNichigetsuExits[];
extern const struct ScenePlacement gSoruNichigetsuPlacements[];
extern const struct SceneEvent gSoruNichigetsuEvents[];

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gSoruNichigetsuEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gSoruNichigetsuExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    return gSoruNichigetsuPlacements;
}

const struct SceneEvent *Scene_GetEvents(void)
{
    return gSoruNichigetsuEvents;
}
void FieldScene_RunScene37aSequenceA(void)
{
    u32 i;
    s32 record;

    if (Value0(CheckAllStatueLights)!= 0) {
        record = GameFlag_IsSet(0x201);
        if (record != 0) {
            goto L_020000f0;
        }
        Event_Begin();
        ColorBuffer_ApplyTarget(0x2051cc, 1);
        ColorBuffer_Interpolate(20);
        GameFlag_Set(0x201);
        GameFlag_Clear(0x200);
        GameFlag_Clear(0x202);
        if (GameFlag_IsSet(FLAG_INNER_SANCTUM_ENTERED) == 0) {
            Scene_EnterInnerSanctum();
        }
        if (Value0(CheckAllStatueLights)!= 0) {
            if (GameFlag_IsSet(0x811) == 0) {
                FieldScene_RunActorPositionTransition();
            }
        }
        Event_End();
    } else {
        if (GameFlag_IsSet(0x200) == 0) {
            Event_Begin();
            ColorBuffer_ApplyTarget(0x10000, 1);
            ColorBuffer_Interpolate(20);
            GameFlag_Set(0x200);
            GameFlag_Clear(0x201);
            GameFlag_Clear(0x202);
            Event_End();
        }
    }
    L_020000f0:;
}

void FieldScene_RunScene37aSequenceB(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x200) == 0) {
        Event_Begin();
        ColorBuffer_ApplyTarget(0x10000, 1);
        ColorBuffer_Interpolate(20);
        GameFlag_Set(0x200);
        GameFlag_Clear(0x201);
        GameFlag_Clear(0x202);
        Event_End();
    }
}

void FieldScene_RunScene37aSequenceC(void)
{
    u32 i;
    s32 record;

    if (Value0(CheckAllStatueLights)!= 0) {
        record = GameFlag_IsSet(0x200);
        if (record != 0) {
            goto L_020001d6;
        }
        Event_Begin();
        ColorBuffer_ApplyTarget(0x10000, 1);
        ColorBuffer_Interpolate(20);
        GameFlag_Set(0x200);
        GameFlag_Clear(0x201);
        GameFlag_Clear(0x202);
        Event_End();
    } else {
        if (GameFlag_IsSet(0x201) == 0) {
            Event_Begin();
            ColorBuffer_ApplyTarget(0x2051cc, 1);
            ColorBuffer_Interpolate(20);
            GameFlag_Set(0x201);
            GameFlag_Clear(0x200);
            GameFlag_Clear(0x202);
            if (GameFlag_IsSet(FLAG_INNER_SANCTUM_ENTERED) == 0) {
                Scene_EnterInnerSanctum();
            }
            Event_End();
        }
    }
    L_020001d6:;
}

void FieldScene_RunScene37aSequenceD(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x202) == 0) {
        ColorBuffer_ApplyTarget(0x202db1, 1);
        ColorBuffer_Interpolate(20);
        GameFlag_Set(0x202);
        GameFlag_Clear(0x200);
        GameFlag_Clear(0x201);
    }
}

void ClearSolShindenBackdrop(void)
{
    s32 black = 0;
    u16 *backdrop_color = (u16 *)0x5000000;
    *backdrop_color = black;
}

void SetStatueLightGroup1(void)
{
    if (Engine_GameFlagIsSet(FLAG_STATUE_LIGHT_1) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 28, 0x22, 10, a, b);
    }
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_2) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 28, 0x24, 10, a, b);
    }
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_3) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 29, 0x22, 11, a, b);
    }
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_4) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 29, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup2(void)
{
    if (GameFlag_IsSet(0x826) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 28, 0x22, 10, a, b);
    }
    if (GameFlag_IsSet(0x827) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 28, 0x24, 10, a, b);
    }
    if (GameFlag_IsSet(0x828) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 29, 0x22, 11, a, b);
    }
    if (GameFlag_IsSet(0x829) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 29, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup3(void)
{
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_1) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 30, 0x22, 10, a, b);
    }
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_2) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 30, 0x24, 10, a, b);
    }
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_3) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 31, 0x22, 11, a, b);
    }
    if (GameFlag_IsSet(FLAG_STATUE_LIGHT_4) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 31, 0x24, 11, a, b);
    }
}

void SetStatueLightGroup4(void)
{
    if (GameFlag_IsSet(0x826) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 30, 0x22, 10, a, b);
    }
    if (GameFlag_IsSet(0x827) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 30, 0x24, 10, a, b);
    }
    if (GameFlag_IsSet(0x828) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2d, 31, 0x22, 11, a, b);
    }
    if (GameFlag_IsSet(0x829) != 0) {
        s32 a = 2;
        s32 b = 1;
        Map_CopyCellsTo(0x2f, 31, 0x24, 11, a, b);
    }
}

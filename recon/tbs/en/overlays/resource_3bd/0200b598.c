/* NONMATCHING: resource_3bd 0x0200b598, SceneData_SelectTableBySceneId, from
 * FIELD/ARUTAMIRA_DOU/EXTENDED_PRESENTATION.C (2026-09-28).
 * Compares the scene with ids 0x94 and 0x96 loaded from literals (link-time
 * values). Remaining: the scene ids. */
#include "ARUTAMIRA.H"

s32 SceneData_SelectTableBySceneId(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&PrimaryRuntimeSelector) {
        return (s32)Data_0200c688;
    }
    if (v == (s32)&Value_00000094) {
        return (s32)Data_0200c724;
    }
    if (v == (s32)&SecondaryRuntimeSelector) {
        return (s32)Data_0200c76c;
    }
    if (v == (s32)&Value_00000096) {
        return (s32)Data_0200c808;
    }
    if (v == (s32)&TertiaryRuntimeSelector) {
        return (s32)Data_0200c850;
    }
    return (s32)Data_0200c5e0;
}

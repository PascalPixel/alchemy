/* NONMATCHING: resource_3bd 0x02008c2c, SceneState_RunFlag200SetupAndPlaceActors16To20,
 * from FIELD/ARUTAMIRA_DOU/EXTENDED_PRESENTATION.C (2026-09-28).
 * Compares the scene with id 0x97 loaded from a literal (a link-time value).
 * Remaining: the scene id. */
#include "ARUTAMIRA.H"

#include "ARUTAMIRA.H"

void SceneState_RunFlag200SetupAndPlaceActors16To20(void)
{

    u8 *work = *(u8 **)0x03001f30;
    s16 *tbl;

    if (GameFlag_IsSet(0x200) != 0) {
        SceneEffect_SetupBlendByFlag201();
        work[0x34] = 1;
    }
    tbl = RuntimeSelectorTable;
    if (tbl[0xe0] == (s32)&TertiaryRuntimeSelector) {
        Actor_SetChildValue(16, 6);
        Actor_SetChildValue(17, 6);
        Actor_SetChildValue(18, 6);
        Actor_SetChildValue(19, 6);
        Actor_SetChildValue(20, 6);
    }
}

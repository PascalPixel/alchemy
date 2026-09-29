#include "ARUTAMIRA.H"

extern u8 *gEffectWork;

void SceneState_RunFlag200SetupAndPlaceActors16To20(void)
{

    u8 *work = gEffectWork;
    s16 *tbl;

    if (GameFlag_IsSet(0x200) != 0) {
        SceneEffect_SetupBlendByFlag201();
        work[0x34] = 1;
    }
    tbl = (s16 *)&gGameState;
    if (tbl[0xe0] == (s32)&SceneId_ArutamiraDou6) {
        Actor_SetChildValue(16, 6);
        Actor_SetChildValue(17, 6);
        Actor_SetChildValue(18, 6);
        Actor_SetChildValue(19, 6);
        Actor_SetChildValue(20, 6);
    }
}

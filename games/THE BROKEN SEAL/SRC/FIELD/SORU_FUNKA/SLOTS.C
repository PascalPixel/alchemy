/* The scene's state words and slots. */
#include "FUNKA.H"


void SceneState_InitStateWordsAndSlots(void)
{
    s32 *p;
    u32 i;

    gEmberMask = 63;
    gEmberTimer = 0;
    gEmberLevel = 0;
    gEmberLevelTimer = 120;
    p = gEmberState;
    for (i = 0; i < 16; i++) {
        *p++ = 0;
    }
}

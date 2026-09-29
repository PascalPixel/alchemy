#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The river stage marks its start in the scene state's first halfword: 9
   once the stage is ready. */
void SceneState_SetHalfword1000To9(void)
{
    s16 *ready = (s16 *)gSceneState;
    /* FAKEMATCH: a word temporary; a halfword constant stored into the byte
       buffer is loaded from the literal pool instead. */
    s32 nine = 9;

    *ready = nine;
}

/* Wait a frame at a time until the stage is ready. */
void SceneState_WaitUntilWord1000IsNine(void)
{
    s16 *ready = (s16 *)gSceneState;

    while (*ready != 9) {
        Engine_TaskWait(1);
    }
}

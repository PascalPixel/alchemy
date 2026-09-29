#include "TASK.H"

/* Mark the stage's control record finished. */
void SceneState_SetHalfword1000To9(void)
{
    ((struct StageControl *)gSceneState)->status = 9;
}

/* Wait until the stage's control record is marked finished. */
void SceneState_WaitUntilStatusNine(void)
{
    s16 *status = &((struct StageControl *)gSceneState)->status;

    while (*status != 9) {
        Task_Wait(1);
    }
}

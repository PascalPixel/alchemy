#include "ENTRANCE.H"

void SceneState_ClearCurrentRecordAndReleaseTarget(void)
{
    s32 *record = **(s32 ***)&((struct WorkPointers *)&gEventWork)->scene;
    s32 *target;

    if (record[0] == 0) {
        return;
    }

    record[0] = 0;
    Engine_GameFlagClear(0x161);

    target = (s32 *)record[5];
    if (target != 0) {
        *(short *)((u8 *)target + 0x64) = 0;
        Engine_ObjectSetScript(target, (s32)Makyuri_ScaleCounterScript);
        Object_SetMode(target, 7);
        record[5] = 0;
    }
}

#include "PROBE.H"

extern const s32 Makyuri_StartMoveScript[];

void SceneState_ClearCurrentRecordAndReleaseTarget(void)
{
    s32 *rec = **(s32 ***)&gWork.scene;
    s32 *target;

    if (rec[0] == 0) {
        return;
    }

    rec[0] = 0;
    GameFlag_Clear(0x161);

    target = (s32 *)rec[5];
    if (target != 0) {
        *(short *)((u8 *)target + 0x64) = 0;
        Engine_ObjectSetScript(target, (s32)Makyuri_StartMoveScript);
        Object_SetAnimation(target, 7);
        rec[5] = 0;
    }
}

#include "TASK.H"

/* Decode the stage's resource into its descriptor, open the control record
 * unless the party's position is kept, and start the rival on its path. */
void SceneState_InitControlRecordAndStartTask(s32 resource)
{
    u8 *state = *(u8 **)(gWorkSlot + WORK_SLOT_STAGE * 4);
    struct StageControl *m = (struct StageControl *)gSceneState;

    Engine_ResourceDecodeType01((const u8 *)Resource_GetTableEntry(resource), state + 240);
    if (GameFlag_IsSet(0x109) == 0) {
        m->status = 1;
        m->f2 = 1;
        m->f4 = *(u16 *)(state + 224);
        m->f8 = 0;
        m->f6 = 0;
    }
    Engine_TaskAddCallback(Korosseo_UpdatePathRival, 0xc85);
}

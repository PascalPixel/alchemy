#include "TOPIC.H"

/*
 * The 148-byte owner includes its eight-word literal pool: those words lie
 * past the return and are read only by the pc-relative loads.
 * Field names are descriptive only: the 24-byte record stride and the cleared
 * halfwords at +14..+20 are read off the stores alone, and the second heading
 * is 0x0001 rather than a multiple of 0x4000 -- the byte is certain, its
 * meaning is not.
 */
void SceneState_InitFourActorRecordsAndInstallTask(void)
{
    u8 *work = TorebiIzumi_Ride;
    s32 i = 0;
    u8 *xtbl;
    u16 *htbl;
    u8 *rec;
    u8 *ztbl;

    xtbl = (u8 *)TorebiIzumi_ActorTileX;
    rec = TorebiIzumi_ActorRecords;
    htbl = (u16 *)TorebiIzumi_ActorHeadings;
    ztbl = (u8 *)TorebiIzumi_ActorTileZ;

    do {
        *(s32 *)(rec + 0) = (s32)*xtbl << 16;
        *(s32 *)(rec + 8) = (s32)*ztbl << 16;
        *(s32 *)(rec + 4) = 0;
        *(u16 *)(rec + 12) = *htbl;
        *(u16 *)(rec + 14) = 0;
        *(u16 *)(rec + 16) = 0;
        *(u16 *)(rec + 18) = 0;
        *(u16 *)(rec + 20) = 0;

        i++;
        xtbl++;
        ztbl++;
        htbl++;
        rec += 24;
    } while (i != 4);

    *(s32 *)(work + 4) = (s32)0xffe20000;      /* -30.0 in 16.16 */
    *(s32 *)(work + 8) = 0;
    *(s32 *)(work + 12) = 0x640000;            /* 200 << 15, i.e. 100.0 */
    *(s32 *)(work + 64) = 0;
    *(s32 *)(work + 68) = 0;
    *(s32 *)(work + 72) = 0;
    *(s32 *)(work + 76) = 0;

    /* r0 carries each lookup's result straight into the retag call. */
    Object_SetAnimation(Engine_ActorGet(20), 2);
    Object_SetAnimation(Engine_ActorGet(21), 2);

    /* The locals keep the task and its rate built rather than folded. */
    {
        s32 budget = 0xc83;
        void (*task)(void) = FieldScene_RunSecondaryScript;

        Engine_TaskAddCallback(task, budget);
    }
}

/* resource_3bb 0x0200bae4..0x0200bb38: compiles exactly with GCC 2.96 once
 * its control record has a name, written against the area's
 * FIELD/KOROSSEO_KABE/TASK.H.
 * Remaining difference: the control record at 0x02001000 is EWRAM scratch
 * the main image defines no symbol for, so the C has no name to reach it by;
 * the rows stay in the listing until the main image names that buffer. */
#include "TASK.H"

typedef struct Ctl {
    s16 f0;
    s16 f2;
    s16 f4;
    s16 f6;
    s16 f8;
} Ctl;

extern Ctl StageControlRecord; /* the buffer at 0x02001000, unnamed in main */

void SceneState_InitControlRecordAndStartTask(void)
{
    u8 *state = *(u8 **)(gWorkSlot + WORK_SLOT_STAGE * 4);
    Ctl *m = &StageControlRecord;

    Engine_ResourceDecodeType01(Resource_GetTableEntry(), (s32)(state + 240));
    if (GameFlag_IsSet(0x109) == 0) {
        m->f0 = 1;
        m->f2 = 1;
        m->f4 = *(u16 *)(state + 224);
        m->f8 = 0;
        m->f6 = 0;
    }
    {
        s32 e = 0xc85;

        Engine_TaskAddCallback(Korosseo_UpdatePathRival, e);
    }
}

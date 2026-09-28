/* resource_3bb 0x0200b22c..0x0200b25c: two owners that compile exactly with
 * GCC 2.96 once their control record has a name, written against the area's
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

/* Complete eight-byte state setter plus its sole four-byte pool word. */
void SceneState_SetHalfword1000To9(void)
{
    StageControlRecord.f0 = 9;
}

void SceneState_WaitUntilStatusNine(void)
{
    s16 *status = &StageControlRecord.f0;

    while (*status != 9) {
        Task_Wait(1);
    }
}

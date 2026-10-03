#include "SELECT.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
extern struct SelectionScreen *gResQueueWork;

/* resource/transfer/find_free_entry.c */
struct SelectionNode *Resource_FindFreeTransferEntry(s32 kind)
{
    /* FAKEMATCH: retain the existing two scalar scans and their separate
       running addresses. A shared typed for-loop produces 56 rather than
       88 bytes in all six editions (2026-10-03 attempt aadcc37dd). */
    s32 state;
    s32 off;
    s32 i;
    s32 j;
    u32 ret;
    u16 *q;
    u16 *p;
    u32 v;

    state = (s32)gResQueueWork;
    if (kind != 0) {
        i = 0;
        p = (u16 *)(state + 0x1DE);
        off = 0;
loop_2:
        if (*p == 0) {
            return (struct SelectionNode *)(state + off + 0x1d4);
        }
        i = i + 1;
        p += 0x1A;
        off = off + 0x34;
        if (i == 5) {
            goto block_10;
        }
        goto loop_2;
    }
    j = 0;
    ret = state + 0x68;
    q = (u16 *)(state + 0x72);
loop_7:
    v = *q;
    q += 0x1A;
    if (v == 0) {
        return (struct SelectionNode *)ret;
    }
    ret += 0x34;
    j += 1;
    if (j == 7) {
block_10:
        return 0;
    }
    goto loop_7;
}

/* resource/Resource_ScheduleOwnerResetDelayed.c */
void MenuSelection_DrawFrame(void);

void Resource_ScheduleOwnerResetDelayed(void)
{
    Scheduler_AddOrUpdateCallback((s32)MenuSelection_DrawFrame, 0xC80);
}

/* resource/Resource_ScheduleOwnerReset.c */

void Resource_ScheduleOwnerReset(void)
{
    Scheduler_RemoveCallback((u32)((s32)MenuSelection_DrawFrame));
}

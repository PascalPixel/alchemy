#include "SELECT.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
extern struct SelectionScreen *gResQueueWork;

/* resource/transfer/find_free_entry.c */
struct SelectionNode *Resource_FindFreeTransferEntry(s32 kind)
{
    struct SelectionScreen *screen = gResQueueWork;
    struct SelectionNode *node;
    s32 i;
    s32 count;

    if (kind != 0) {
        node = &screen->records[9];
        count = 5;
    } else {
        node = &screen->records[2];
        count = 7;
    }
    for (i = 0; i < count; i++, node++) {
        if (node->kind == 0)
            return node;
    }
    return NULL;
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

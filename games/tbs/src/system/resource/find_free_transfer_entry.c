#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "select.h"
#include "types.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "global_cells.h"

struct SelectionNode *Resource_FindFreeTransferEntry(s32 kind)
{
    /* FAKEMATCH: the separate scalar scans retain their running addresses and 88 bytes; a shared typed loop produces 56. */
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
        p = &((struct SelectionScreen *)state)->records[9].kind;
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
    q = &((struct SelectionScreen *)state)->records[2].kind;
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


void Resource_ScheduleOwnerResetDelayed(void)
{
    Scheduler_AddOrUpdateCallback(MenuSelection_DrawFrame, 0xC80);
}

void Resource_ScheduleOwnerReset(void)
{
    Scheduler_RemoveCallback(MenuSelection_DrawFrame);
}

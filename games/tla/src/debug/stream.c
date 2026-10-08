#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "ram_buffer.h"
#include "eventwrk.h"


u32 Stream_ReadU16(u8 **cursor)
{
    u32 low = *(*cursor)++;
    u32 high = *(*cursor)++;

    return low | (high << 8);
}

s16 Stream_ReadS16(u8 **cursor)
{
    u32 value;

    value = *(*cursor)++;
    value |= *(*cursor)++ << 8;
    return value;
}

void BattleMap_ApplySceneEntranceView(void)
{
    struct EventWork *event = Ram_HeapSlots->event_work;
    s32 done;

    /* FAKEMATCH: the native issues the event work load before the argument. */
    __asm__("");
    done = Func_080cc67c(1);
    if (event->control_mode == 2 && done == 0)
        done = Func_080cc7c4();
    if (done == 0)
        Func_080cc67c(0);
}

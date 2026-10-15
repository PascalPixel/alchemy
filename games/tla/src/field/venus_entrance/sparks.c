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
#include "resource.h"
#include "mem.h"

struct SparkWork {
    u8 unknown_0000[0x2102];
    u16 tile_base;
    u8 unknown_2104[0x80];
    s16 resource;
};

void Scheduler_RemoveCallbackFar(void (*callback)(void));

void VenusEntrance_StopSparks(void)
{
    struct SparkWork *work = Ram_HeapSlots->menu_runtime;

    Scheduler_RemoveCallbackFar(Func_0200258c);
    Resource_ResetEntry(work->resource);
    Runtime_ReleaseHeapBlock(220);
}

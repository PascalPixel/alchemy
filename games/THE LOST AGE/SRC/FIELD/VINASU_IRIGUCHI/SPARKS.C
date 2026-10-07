#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"

struct SparkWork {
    u8 unknown_0000[0x2102];
    u16 tile_base;
    u8 unknown_2104[128];
    s16 resource;
};

void Scheduler_RemoveCallbackFar(void (*callback)(void));

void VinasuIriguchi_StopSparks(void)
{
    struct SparkWork *work = Ram_HeapSlots->menu_runtime;

    Scheduler_RemoveCallbackFar(Func_0200258c);
    Resource_ResetEntry(work->resource);
    Runtime_ReleaseHeapBlock(220);
}

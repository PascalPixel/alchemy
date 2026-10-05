#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "RESOURCE.H"
#include "RUNTIME_MEM.H"

/* The scene's spark work, which heap slot 0xdc holds: 96 sparks of 28
   bytes from 0x800, their 40-byte sprites from 0x1280, then the tile base
   and the resource entry they draw from. */
struct SparkWork {
    u8 unknown_0000[0x2102];
    u16 tile_base;
    u8 unknown_2104[0x80];
    s16 resource;
};

void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Func_0200258c(void);

/* Stops the spark updater, frees the sparks' resource entry and releases
   their work block. */
void Func_020027b0(void)
{
    struct SparkWork *work = Ram_HeapSlots->menu_runtime;

    Scheduler_RemoveCallbackFar(Func_0200258c);
    Resource_ResetEntry(work->resource);
    Runtime_ReleaseHeapBlock(220);
}

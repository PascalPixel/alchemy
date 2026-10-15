#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "resource.h"
#include "system.h"
#include "callback_scheduler.h"

extern char ResourceId_DefaultMapCells;
extern void (*gData_030011f8)(void);
void MapAnimation_Update(void);
void Map_ShowBg1FromBuffer(void);
void Map_UpdateCurrentTileBlock(void);

void Map_LoadDefaultCellsAndUpdateBlock(void)
{
    struct MapViewWork *work = (struct MapViewWork *)Ram_HeapSlots->mapWork;

    gData_030011f8 = Map_ShowBg1FromBuffer;
    work->windowTop = 0;
    work->windowBottom = 0x9f;
    WaitFrames(1);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), Ram_MapCellBuffer);
    Map_UpdateCurrentTileBlock();
    Scheduler_DisableCallbacks(MapAnimation_Update);
    WaitFrames(1);
}

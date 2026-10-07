#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "MAP.H"
#include "RESOURCE.H"
#include "SYSTEM.H"
#include "CALLBACK_SCHEDULER.H"

extern char ResourceId_DefaultMapCells;
extern void (*Data_030011f8)(void);
void MapAnimation_Update(void);
void Map_ShowBg1FromBuffer(void);
void Map_UpdateCurrentTileBlock(void);

void Map_LoadDefaultCellsAndUpdateBlock(void)
{
    struct MapViewWork *work = (struct MapViewWork *)Ram_HeapSlots->map_work;

    Data_030011f8 = Map_ShowBg1FromBuffer;
    work->window_top = 0;
    work->window_bottom = 0x9f;
    WaitFrames(1);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), Ram_MapCellBuffer);
    Map_UpdateCurrentTileBlock();
    Scheduler_DisableCallbacks(MapAnimation_Update);
    WaitFrames(1);
}

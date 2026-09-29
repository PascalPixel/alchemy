#include "TYPES.H"
#include "SYSTEM.H"
extern u8 gMapCellBuffer[];
extern u8 Data_03001e70[];
extern u8 Data_03001cfc[];

/* map/shared/load_default_cells_and_update_block.c */
extern u8 Value_000000d5[];
void Map_ShowBg1FromBuffer(void);
void Resource_DecodeType01(s32, s32);
void *Resource_GetTableEntry(s32);
void Map_UpdateCurrentTileBlock(void);
void Scheduler_DisableCallbacks(u32 value);
void MapAnimation_Update(void);

struct MapInitWork {
    u8 unknown_000[0x100];
    s16 first;
    s16 second;
};

void Map_LoadDefaultCellsAndUpdateBlock(void)
{
    struct MapInitWork *work = *(struct MapInitWork **)((u32)&Data_03001e70);
    *(s32 *)((u32)&Data_03001cfc) = (s32)Map_ShowBg1FromBuffer;
    work->first = 0;
    work->second = 0x9f;
    WaitFrames(1U);
    Resource_DecodeType01((s32)Resource_GetTableEntry((s32)Value_000000d5), (u32)gMapCellBuffer);
    Map_UpdateCurrentTileBlock();
    Scheduler_DisableCallbacks((u32)MapAnimation_Update);
    WaitFrames(1U);
}

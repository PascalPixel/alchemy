#include "TYPES.H"
#include "SYSTEM.H"

/* map/shared/load_default_cells_and_update_block.c */
extern u8 gVal[];
void FunctionHead_0801161c(void);
void Resource_DecodeType01(s32, s32);
void *GetResource(s32);
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
    struct MapInitWork *work = *(struct MapInitWork **)ADDR_03001E70;
    *(s32 *)ADDR_03001CFC = (s32)FunctionHead_0801161c;
    work->first = 0;
    work->second = 0x9f;
    WaitFrames(1U);
    Resource_DecodeType01((s32)GetResource((s32)gVal), 0x02010000);
    Map_UpdateCurrentTileBlock();
    Scheduler_DisableCallbacks((u32)MapAnimation_Update);
    WaitFrames(1U);
}

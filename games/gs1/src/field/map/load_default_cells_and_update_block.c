#include "types.h"

/* map/shared/load_default_cells_and_update_block.c */
extern u8 gVal[];
void WaitFrames(s32);
void FunctionHead_0801161c(void);
void Map_Apply(s32, s32);
void *GetResource(s32);
void Map_UpdateCurrentTileBlock(void);
void FunctionHead_0800439c(u32 value);
void Map_unk3_4(void);

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
    Map_Apply((s32)GetResource((s32)gVal), 0x02010000);
    Map_UpdateCurrentTileBlock();
    FunctionHead_0800439c((u32)Map_unk3_4);
    WaitFrames(1U);
}

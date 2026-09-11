#include "types.h"
#include "scene.h"
#include "abi/map/shared/load_default_cells_and_update_block.h"
#include "global_cells.h"
#include "resource.h"

extern u8 gVal[];
void WaitFrames(u32);

struct MapInitWork {
    u8 unknown_000[0x100];
    s16 first;
    s16 second;
};

void Map_LoadDefaultCellsAndUpdateBlock(void)
{
    struct MapInitWork *work = *(struct MapInitWork **)ADDR_03001E70;
    *(s32 *)ADDR_03001CFC = (s32)Map_Run;
    work->first = 0;
    work->second = 0x9f;
    WaitFrames(1U);
    Map_Apply((s32)GetResource((s32)gVal), 0x02010000);
    Map_Run2();
    Map_Check((u32)Map_Run3);
    WaitFrames(1U);
}

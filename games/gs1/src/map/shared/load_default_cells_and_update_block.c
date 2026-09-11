#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "resource.h"
#include "map_render_work.h"

/* map/shared/load_default_cells_and_update_block.c */
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
    Map_UpdateCurrentTileBlock();
    Map_Check((u32)Map_unk3_4);
    WaitFrames(1U);
}

/* map/shared/clear_layer_entry_flag.c */
extern u8 *volatile gCam;

void Map_ClearLayerEntryFlag(u32 no)
{
    u8 *base = gCam;
    u8 *entry = base + no * 12;
    u32 value = 0;
    *(u16 *)(entry + 0x22) = value;
}

void Map_Run(u32 no)
{
    u8 *base = gCam;
    u8 *entry = base + no * 12;
    u32 value = 1;
    *(u16 *)(entry + 0x22) = value;
}

/* map/shared/enable_update_callback.c */
void Map_EnableUpdateCallback(void)
{
    if (gCam->active == 0)
        Map_Check((u32)Map_Run);
}

/* map/shared/disable_update_callback.c */
void Map_DisableUpdateCallback(void)
{
    if (gCam->active == 0)
        Map_Check((u32)Map_Run);
}

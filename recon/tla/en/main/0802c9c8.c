#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "resource.h"
#include "system.h"
#include "dma.h"
#include "io_reg.h"
#include "callback_scheduler.h"

extern void (*Data_030011f8)(void);
extern char ResourceId_DefaultMapCells;
extern u8 Data_02038000[];
extern u8 Data_0203a000[];
extern u8 Data_0203c000[];
extern u8 Data_0203e000[];
extern u8 Data_02028000[];
extern u8 gWorldMapWork[];
s32 Resource_DecodeByteLzInRam(const void *source, void *destination);
void MapAnimation_Update(void);
void Map_ShowBg1FromBuffer(void);
void Map_UpdateCurrentTileBlock(void);
void Func_0802d088(s32 area, s32 mode);

s32 Map_LoadAreaGraphics(void)
{
    struct MapViewWork *state;
    u32 *resources;
    u8 *buffer;
    s16 value;

    state = (struct MapViewWork *)Ram_HeapSlots->mapWork;
    buffer = Ram_MapCellBuffer;
    resources = state->resources;
    value = *(s16 *)0x05000000;
    Resource_DecodeByteLzInRam(Resource_GetTableEntry(resources[0]), buffer);
    *(s16 *)buffer = value;
    Dma_Set(buffer, (void *)0x05000000, 0x84000070, REG_DMA3);
    Resource_DecodeType01(Resource_GetTableEntry(resources[3]), Data_02038000);
    Resource_DecodeType01(Resource_GetTableEntry(resources[4]), Data_0203a000);
    Resource_DecodeType01(Resource_GetTableEntry(resources[5]), Data_0203c000);
    Resource_DecodeType01(Resource_GetTableEntry(resources[6]), Data_0203e000);
    Resource_DecodeType01(Resource_GetTableEntry(resources[7]), Data_02028000);
    Resource_DecodeType01(Resource_GetTableEntry(resources[8]), gWorldMapWork);
    Data_030011f8 = Map_ShowBg1FromBuffer;
    state->windowTop = 0;
    state->windowBottom = 159;
    WaitFrames(1);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DefaultMapCells), buffer);
    /* FAKEMATCH: the halfword local keeps zero in r5 and the short-range pool before the epilogue. */
    value = 0;
    Map_UpdateCurrentTileBlock();
    state->animationActive = value;
    Scheduler_DisableCallbacks(MapAnimation_Update);
    state->framePending = value;
    state->frameShown = value;
    Func_0802d088(state->area, 0);
    WaitFrames(1);
}

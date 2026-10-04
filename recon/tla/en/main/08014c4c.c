/*
 * Draft: PaletteDma_LoadBlock, ☀️'s MEMORY.C body: matches as it stands;
 * waits for Runtime_InitializeHeap (08014c6c) to join ⚓️'s MEMORY.C ahead of
 * the heap counts.
 */
#include "TYPES.H"
#include "DMA.H"
#include "RAM_BUFFER.H"
#include "IWRAM_CALL.H"
#include "RUNTIME_MEM.H"

extern u16 *Data_030011c4;
extern const u8 Data_08017b10[];
extern const u16 Data_08017af0[];
extern u8 Data_03001300[];
extern u8 Data_02030000[];
extern u8 IwramSoundMixWorkspace[];
extern char ResourceId_WindowTiles;
void *Resource_GetTableEntry(s32);

void PaletteDma_LoadBlock(void)
{
    Dma_Set((const void *)Data_08017b10, (void *)0x05000200, 0x800000e0, (volatile u32 *)0x040000d4);
}

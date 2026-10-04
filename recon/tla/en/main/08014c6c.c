/*
 * Draft: Runtime_InitializeHeap over ⚓️'s heap cursors. Score 4: the listing
 * issues add sp, #4 straight after the IWRAM cursor's pool load, filling its
 * delay. The rest is load scheduling: the native issues loads (and fills
 * their delay) earlier than agscc at -mcpu=arm7tdmi; with -mtune=arm9tdmi
 * added it scores 0. Data_03001300 and Data_02030000 are ☀️'s gIwramHeap and
 * gEwramHeap.
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

void Runtime_InitializeHeap(void)
{
    struct HeapSlots *work = Ram_HeapSlots;
    volatile u32 zero = 0;
    Dma_Set(&zero, work, 0x85000040, (volatile u32 *)0x040000d4);
    Ram_HeapSlots->next_iwram = Data_03001300;
    Ram_HeapSlots->next_ewram = Data_02030000;
}

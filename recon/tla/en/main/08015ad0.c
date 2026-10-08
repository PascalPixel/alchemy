#include "types.h"
#include "ram_buffer.h"
#include "save_state.h"
#include "dma.h"
#include "flash.h"

struct DmaRegisters {
    u32 source;
    u32 destination;
    u32 control;
};

#define DMA3 ((volatile struct DmaRegisters *)0x040000d4)
#define DMA3_BUSY 0x80000000

#define START_DMA(source_, destination_, control_) \
    Dma_Set((const void *)(source_), (void *)(destination_), (control_), (volatile u32 *)0x040000d4)

static __inline__ void WaitDma(struct DmaRegisters volatile *channel)
{
    while ((channel->control & DMA3_BUSY) != 0) {}
}

#define WAIT_DMA() WaitDma(DMA3)

extern u16 (*Flash_Handler0)(u16 sector, u8 *source);
extern const u8 Data_08017d08[];
extern const u8 Data_08017d10[];

u32 Random16(void);
s32 SaveState_CompareBytes(u8 *left, u8 *right, s32 count);
s32 SaveState_ChecksumWorkspace(void);
s32 SaveState_ReadSlotAndCheckChecksum(s32 index);
u32 SaveState_WriteWorkspaceSlot(s32 sector);
s32 SaveState_InvalidateSlot(s32 index);

u32 SaveState_SelectWriteSlot(s32 record_id)
{
    u32 empty[15];
    s32 count;
    u32 index;
    u8 *slot;
    u8 value;

    slot = ((struct SaveWorkspace *)Ram_HeapSlots->serial_work)->occupied;
    count = 0;
    index = 0;
    do {
        value = *slot;
        slot += 3;
        if (value == 0)
            empty[count++] = index;
        index += 3;
    } while (index <= 14);
    index = 15;
    if (count != 0) {
        if (count == 1) {
            index = empty[0];
            if (SaveState_FindLatestSlot(record_id) == 15)
                index = 15;
        } else {
            index = Random16() % count;
            index = empty[index];
        }
    }
    return index;
}

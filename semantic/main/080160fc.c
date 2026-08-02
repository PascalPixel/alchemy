#include "layout_guard.h"
#include "types.h"

typedef struct DmaChannel_080160fc {
    const void *source;
    void *destination;
    u32 control;
} DmaChannel_080160fc;

typedef struct TextRuntime_080160fc {
    u8 tile_data[0xea3];
    u8 dirty_blocks;
    u8 paddingea4[2];
    u8 upload_busy;
} TextRuntime_080160fc;

LAYOUT_OFFSET_GUARD(
    TextRuntime080160fc_DirtyBlocks,
    TextRuntime_080160fc,
    dirty_blocks,
    0xea3);
LAYOUT_OFFSET_GUARD(
    TextRuntime080160fc_UploadBusy,
    TextRuntime_080160fc,
    upload_busy,
    0xea6);

extern TextRuntime_080160fc *Data_03001e8c;

/* Upload the dirty 0x100-byte text-tile blocks to character memory. */
void Func_080160fc(void)
{
    volatile DmaChannel_080160fc *dma =
        (volatile DmaChannel_080160fc *)0x040000d4;
    TextRuntime_080160fc *runtime = Data_03001e8c;
    const u8 *source = runtime->tile_data;
    u8 *destination = (u8 *)0x06002000;
    u32 pending = runtime->dirty_blocks;

    if (runtime->upload_busy != 0 || pending == 0)
        return;

    /* Bit 0 requests all five blocks; bits 1..5 select them individually. */
    if ((pending & 1) != 0)
        pending = 0x3f;
    pending = (pending & 0x3f) >> 1;

    do {
        if ((pending & 1) != 0) {
            dma->source = source;
            dma->destination = destination;
            dma->control = 0x84000040;
        }

        pending >>= 1;
        source += 0x100;
        destination += 0x100;
    } while (pending != 0);

    runtime->dirty_blocks = 0;
}

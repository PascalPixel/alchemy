/* Load character set `resource` into the background (or, when alternate, the
   second) character block and its palette into the matching bank; resource
   0 clears them instead. The packed decoder is copied from ROM into heap
   block 49 and run there; the palette load is queued for the next frame.

   FAKEMATCH: the queued palette load is QueueIoWriteDelay-style inline code
   with the odd constructs of SYSTEM/IO_WRITE_QUEUE.C (one-pass loops around
   the IME read and around everything after the queue pointer, and the count
   stored through an explicit u16 pointer). */
#include "TYPES.H"
#include "DMA.H"
#include "IO_WRITE_QUEUE.H"

extern volatile u16 Data_04000208;
#define REG_IME Data_04000208

extern void *Data_03001e50[];
extern u8 Value_00000230[];

void Graphics_ClearCharacterBlockAndPalette(s32 alternate);
u8 *Resource_GetTableEntry(u32 resource);
void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);

/* The decoder returns a value this caller ignores. */
typedef s32 (*PackedDecoder)(u8 *source, u32 destination, u32 fill);

void Graphics_LoadCharacterBlockAndPalette(u32 resource, s32 alternate)
{
    u8 *data;
    u32 palette;
    u32 fill;
    u32 vram;
    void *decoder;
    u32 size;

    if (resource == 0) {
        Graphics_ClearCharacterBlockAndPalette(alternate);
        return;
    }
    data = Resource_GetTableEntry(resource);
    if (alternate == 0) {
        fill = 0;
        vram = 0x06000000;
        palette = 0x05000000;
    } else {
        fill = 0x80808080;
        vram = 0x06008000;
        palette = 0x05000100;
    }
    size = (u32)Value_00000230;
    decoder = Runtime_AllocateHeapBlock(49, size);
    Dma_Set((void *)0x080f0024, decoder, 0x84000000 | (size >> 2), (volatile u32 *)0x040000d4);
    ((PackedDecoder)Data_03001e50[49])(data + 256, vram, fill);
    Runtime_ReleaseHeapBlock(49);

    {
        volatile u16 *ime;
        struct IoWriteQueue *q;
        u32 saved;
        s32 count;

        q = &gIoWriteQueue;
        do {
            do {
                ime = &REG_IME;
                saved = *ime;
            } while (0);
            *ime = (u16)ime;
            count = q->count;
            if (count <= 31) {
                u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);
                *(u16 *)&q->count = count + 1;
                *destination++ = (u32)data;
                *destination++ = palette;
                *destination = 0x84000040;
            }
            *ime = saved;
        } while (0);
    }
}

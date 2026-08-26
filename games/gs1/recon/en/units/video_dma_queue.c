#include "../main/io_write_queue_family.h"
#include "../../../src/input/init_key_irq.c"

#undef DEFINE_QUEUE_IO_WRITE
#define DEFINE_QUEUE_IO_WRITE(name, delay) \
void name(u32 address, u32 value) \
{ \
    volatile u16 *ime = &REG_IME; \
    struct IoWriteQueue *q = &gIoWriteQueue; \
    do { \
        u32 saved; \
        \
        saved = *ime; \
        *ime = (u16)ime; \
        do { \
            s32 cnt; \
            \
            cnt = q->count; \
            if (cnt <= 31) { \
                u32 *dst = (u32 *)((u8 *)q + cnt * 12 + 4); \
                *dst++ = value; \
                q->count = cnt + 1; \
                *dst++ = address; \
                *dst = (delay); \
            } \
        } while (0); \
        *ime = saved; \
    } while (0); \
}

#define QueueIoWriteDelay1 Func_0800383c
#define QueueIoWriteDelay2 Func_0800387c
#define QueueIoWriteDelay3 Func_080038bc
#define QueueIoWriteDelay5 Func_080038fc
#define QueueIoWriteDelay6 Func_0800393c
#define QueueIoWriteDelay7 Func_0800397c
#define QueueIoWriteDelay9 Func_080039bc
#define QueueIoWriteDelay10 Func_080039fc
#define QueueIoWriteDelay11 Func_08003a3c
#define IoWriteQueue_FlushPending Func_08003a7c
#define Runtime_CopyAndCallRoutine Func_08003e10
#define ResourceTable_AllocateBlocks Func_08003e58

DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay1, 0x10000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay2, 0x20000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay3, 0x30000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay5, 0x50000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay6, 0x60000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay7, 0x70000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay9, 0x90000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay10, 0xa0000)
DEFINE_QUEUE_IO_WRITE(QueueIoWriteDelay11, 0xb0000)

struct DmaChannel {
    const void *source;
    void *destination;
    u32 control;
};

static __inline__ void StartDmaTransfer(
    const void *source,
    void *destination,
    u32 control)
{
    volatile struct DmaChannel *dma =
        (volatile struct DmaChannel *)0x040000d4;

    dma->source = source;
    dma->destination = destination;
    dma->control = control;
}

typedef void (*QueueFlushRoutine)(struct IoWriteQueue *queue, u32 count);

/* Linker-resolved absolute size: 0x68. */
extern u8 TransferCommandExecutor_Size[];

void IoWriteQueue_FlushPending(void)
{
    struct IoWriteQueue *queue;
    u32 count;

    queue = &gIoWriteQueue;
    count = queue->count;
    if (count != 0) {
        const void *dma_source;
        void *dma_destination;
        u32 dma_control;
        QueueFlushRoutine routine;
        u32 size = (u32)TransferCommandExecutor_Size;
        u32 dma_word_count = size >> 2;
        u32 zero = 0;
        u32 dma_control_base = 0x84;

        dma_control_base <<= 24;

        {
            u8 routine_bytes[dma_word_count << 2];

            routine = (QueueFlushRoutine)routine_bytes;
            dma_source = (const void *)0x08002cf4;
            dma_destination = (void *)routine;
            dma_control = dma_word_count | dma_control_base;
            StartDmaTransfer(dma_source, dma_destination, dma_control);
            routine(queue, count);
            queue->count = zero;
            /*
             * Extent probe: the scalar DMA spelling is four bytes shorter
             * than the retained movmem-shaped owner. Keep the complete queue
             * header access explicit until that store shape is recovered.
             */
            *(volatile u16 *)&queue->pad = *(volatile u16 *)&queue->pad;
        }
    }
}

#include "../../../src/display/blend/update_transition.c"

#include "../../../src/display/blend/set_darken_target_16.c"
#include "../../../src/display/blend/set_darken_target_0.c"
#include "../../../src/display/blend/set_brighten_target_16.c"
#include "../../../src/display/blend/set_brighten_target_0.c"
#include "../../../src/display/blend/configure_transition.c"

#include "../../../src/display/blend/wait_for_transition.c"
#include "../../../src/graphics/reset_frame_state.c"
#include "../../../src/graphics/affine/build_matrix.c"
#include "../../../src/runtime/slot/push_entry.c"

u32 Func_08004938(s32 size);
void Func_08002df0(void *allocation);

typedef void (*LoadedRoutine)(void *argument);

/* Linker-resolved absolute size: 0xe0. */
extern u8 LoadedRuntime_Size[];

void Runtime_CopyAndCallRoutine(void *argument)
{
    struct DmaChannel *dma;
    u32 size = (u32)LoadedRuntime_Size;
    LoadedRoutine routine = (LoadedRoutine)Func_08004938(size);

    dma = (struct DmaChannel *)0x040000d4;
    dma->source = (const void *)0x08001dc8;
    dma->destination = (void *)routine;
    dma->control = (size >> 2) | 0x84000000;
    routine(argument);
    Func_08002df0((void *)routine);
}

struct ResourceTableEntry {
    u16 size;
    u16 block;
};

extern u8 Data_03001810[512];
extern struct ResourceTableEntry Data_03001b10[96];

s32 ResourceTable_AllocateBlocks(u32 id, u32 size)
{
    u32 blocks;
    s32 result;

    blocks = size >> 6;
    if (id > 95) {
        return -1;
    }
    {
        u8 *map = Data_03001810;
        struct ResourceTableEntry *tbl = Data_03001b10;
        s32 pos = 0;
        u8 *scan_map = map;

next_run:
        result = -1;
        if (pos >= 512) {
            goto done;
        }
        if (scan_map[pos] == 0xff) {
            u32 end;
            u32 i;
            u8 *scan;

            result = pos;
            end = result + blocks;
            scan = scan_map + result;
            while (pos < end) {
                if (*scan++ != 0xff) {
                    goto occupied;
                }
                pos++;
            }
            for (i = 0; i < blocks; i++) {
                ResourceBlockOwners[result + i] = id;
            }
            result <<= 6;
            goto done;
        }
occupied:
        pos += tbl[map[pos]].size >> 6;
        goto next_run;
done:
        return result;
    }
}

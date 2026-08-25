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

void Func_08003a7c(void)
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

void Func_08003b70(s32 duration)
{
    Data_03001cd4 = 0;
    Data_03001cf8 = 0x3e;
    Data_03001aec = Data_03001ca8;
    Data_03001ca8 = 0x10;
    Data_03001c98 = duration;
    Data_03001ac0 = Data_03001c98;
}

void Func_08003bb4(s32 duration)
{
    Data_03001cd4 = 0;
    Data_03001cf8 = 0x3e;
    Data_03001aec = Data_03001ca8;
    Data_03001ca8 = 0;
    Data_03001c98 = duration;
    Data_03001ac0 = Data_03001c98;
}

void Func_08003bf8(s32 duration)
{
    Data_03001cd4 = 1;
    Data_03001cf8 = 0x3e;
    Data_03001aec = Data_03001ca8;
    Data_03001ca8 = 0x10;
    Data_03001c98 = duration;
    Data_03001ac0 = Data_03001c98;
}

void Func_08003c3c(s32 duration)
{
    Data_03001cd4 = 1;
    Data_03001cf8 = 0x3e;
    Data_03001aec = Data_03001ca8;
    Data_03001ca8 = 0;
    Data_03001c98 = duration;
    Data_03001ac0 = Data_03001c98;
}

void Func_08003c80(s8 mode, s32 coefficient, u32 start, s32 target, s32 duration)
{
    Data_03001cd4 = mode;
    Data_03001cf8 = coefficient & 0x3f;
    if (start > 0x10U) {
        Data_03001aec = Data_03001ca8;
    } else {
        Data_03001aec = start;
    }
    Data_03001ca8 = target;
    Data_03001ac0 = (Data_03001c98 = duration);
}

s32 WaitFrames(s32 frames);

void Func_08003ce0(void)
{
    if (*(u8 *)ADDR_03001C98 != 0) {
        do {
            WaitFrames(1);
        } while (*(u8 *)ADDR_03001C98 != 0);
    }
}

s32 Func_080072f0(s32, s32, s32, s32);

void Func_08003d04(void)
{
    *(s8 *)ADDR_03001D00 = 0;
    Func_080072f0(ADDR_03001400, 0x400, ADDR_03001D00, 0x03000164);
}

typedef s32 (*SignedDivide)(s32 numerator, s32 denominator);

struct Effect {
    unsigned x : 16;
    unsigned y : 16;
    unsigned angle : 16;
    unsigned unused : 16;
};

union AffineMatrix {
    s16 coefficients[4];
    u32 rows[2];
};

s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
s32 FixedPoint_Ratio(s32 numerator, s32 denominator);

extern u8 Data_03001d00;
extern union AffineMatrix Data_03001d40[];

s32 AffineMatrix_BuildForEffect(struct Effect *source)
{
    union AffineMatrix *matrix;
    s16 *coefficient;
    s32 x_scale;
    s32 y_scale;
    s32 angle;
    u8 index;

    index = Data_03001d00;
    x_scale = (s16)source->x;
    y_scale = (s16)source->y;
    angle = source->angle;
    if (index > 31)
        return 0;

    matrix = &Data_03001d40[index];
    coefficient = matrix->coefficients;
    if ((x_scale == y_scale || -x_scale == y_scale) && angle == 0) {
        SignedDivide divide;
        s32 reciprocal;
        s32 x_reciprocal;

        divide = (SignedDivide)0x03000380;
        reciprocal = divide(0x10000, y_scale);
        x_reciprocal = reciprocal;
        if (-x_scale == y_scale)
            x_reciprocal = -reciprocal;

        matrix->rows[0] = (u16)x_reciprocal;
        matrix->rows[1] = (u32)reciprocal << 16;
    } else {
        s32 sine;
        s32 cosine;

        sine = Func_08002322(angle);
        cosine = Func_0800231c(angle);
        *coefficient = FixedPoint_Ratio(cosine, x_scale);
        coefficient++;
        *coefficient = FixedPoint_Ratio(sine, x_scale);
        coefficient++;
        *coefficient = FixedPoint_Ratio(-sine, y_scale);
        coefficient++;
        *coefficient = FixedPoint_Ratio(cosine, y_scale);
    }

    Data_03001d00 = index + 1;
    return index;
}

#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void Runtime_PushSlotEntry(s32 *slot_entry, s32 slot)
{
    s32 *previous_head;
    s32 slot_offset;
    s32 clamped_slot;

    clamped_slot = slot;
    if (clamped_slot > 0xFF) {
        clamped_slot = 0xFF;
    }
    if (clamped_slot < 0) {
        clamped_slot = 0;
    }
    slot_offset = clamped_slot * 4;
    previous_head = M2C_FIELD(slot_offset, s32 **, ADDR_03001400);
    M2C_FIELD(slot_offset, s32 **, ADDR_03001400) = slot_entry;
    *slot_entry = previous_head;
}

#undef M2C_FIELD

u32 Func_08004938(s32 size);
void Func_08002df0(void *allocation);

typedef void (*LoadedRoutine)(void *argument);

/* Linker-resolved absolute size: 0xe0. */
extern u8 LoadedRuntime_Size[];

void Func_08003e10(void *argument)
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

s32 Func_08003e58(u32 id, u32 size)
{
    u32 blocks;
    u32 start;
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

            start = pos;
            end = start + blocks;
            scan = scan_map + start;
            while (pos < end) {
                if (*scan++ != 0xff) {
                    goto occupied;
                }
                pos++;
            }
            for (i = 0; i < blocks; i++) {
                ResourceBlockOwners[start + i] = id;
            }
            result = start << 6;
            goto done;
        }
occupied:
        pos += tbl[map[pos]].size >> 6;
        goto next_run;
done:
        return result;
    }
}

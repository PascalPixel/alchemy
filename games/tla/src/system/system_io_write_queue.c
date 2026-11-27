// The register write queue that VBlank carries out after each finished tick. Part of system.c.

// The queued register writers: each adds a write of `value` to `addr` that the VBlank handler
// carries out once the tick loop has finished the tick.
#define IO_QUEUE_DEFINE_WRITE(name, control)                                                       \
    void name(Uint32 addr, Uint32 value)                                                           \
    {                                                                                              \
        IO_WRITE_QUEUE_PUSH(addr, value, control);                                                 \
    }

// Queues a byte store.
IO_QUEUE_DEFINE_WRITE(syIOWriteQueueStore8, IO_WRITE_BYTE)
// Queues a halfword store.
IO_QUEUE_DEFINE_WRITE(syIOWriteQueueStore16, IO_WRITE_HALFWORD)
// Queues a word store.
IO_QUEUE_DEFINE_WRITE(syIOWriteQueueStore32, IO_WRITE_WORD)
// Queues setting a byte register's bits.
IO_QUEUE_DEFINE_WRITE(syIOWriteQueueSetBits8, IO_WRITE_SET | IO_WRITE_BYTE)
// Queues setting a halfword register's bits.
IO_QUEUE_DEFINE_WRITE(syIOWriteQueueSetBits16, IO_WRITE_SET | IO_WRITE_HALFWORD)
// Queues setting a word register's bits.
IO_QUEUE_DEFINE_WRITE(syIOWriteQueueSetBits32, IO_WRITE_SET | IO_WRITE_WORD)
// Queues clearing a byte register's bits.
IO_QUEUE_DEFINE_WRITE(syIOWriteQueueClearBits8, IO_WRITE_CLEAR | IO_WRITE_BYTE)
// Queues clearing a halfword register's bits.
IO_QUEUE_DEFINE_WRITE(syIOWriteQueueClearBits16, IO_WRITE_CLEAR | IO_WRITE_HALFWORD)
// Queues clearing a word register's bits.
IO_QUEUE_DEFINE_WRITE(syIOWriteQueueClearBits32, IO_WRITE_CLEAR | IO_WRITE_WORD)

// Runs the queued register writes and empties the queue, with a copy of the executor function on
// the stack.
void syIOWriteQueueFlushPending(void)
{
    IO_WRITE_QUEUE *queue;
    Uint32          count;

    queue = &IoWriteQueueWork;
    count = queue->count;
    if (count != 0)
    {
        QUEUE_FLUSH_ROUTINE routine;

        {
            Uint8 routineBytes[(Uint32)ResourceExecuteTransferSize];

            // The function runs from its copy on the stack, which lasts until it ends.
            routine = (QUEUE_FLUSH_ROUTINE)routineBytes;
            syDMAXCopy(ResourceExecuteTransferCode,
                       (void *)routine,
                       DMA_COPY_WORDS(sizeof(routineBytes) / 4),
                       REG_DMA3);
            routine(queue, count);
            queue->count = 0;
        }
    }
}

#undef IO_QUEUE_DEFINE_WRITE

/* @file */
/* The register write queue that VBlank carries out after each finished frame.
 */
/* Part of system.c. */

typedef void (*QUEUE_FLUSH_ROUTINE)(IO_WRITE_QUEUE *queue, u32 count);

/* The queued register writers: each adds a write of `value` to `address` that
 */
/* the VBlank handler carries out once the frame loop has finished the frame. */
#define IO_QUEUE_DEFINE_WRITE(name, control)                                   \
	void name(u32 address, u32 value) {                                        \
		IO_WRITE_QUEUE_PUSH(address, value, control);                          \
	}

/* Queues a byte store. */
IO_QUEUE_DEFINE_WRITE(syIoWriteQueueStore8, IO_WRITE_BYTE)
/* Queues a halfword store. */
IO_QUEUE_DEFINE_WRITE(syIoWriteQueueStore16, IO_WRITE_HALFWORD)
/* Queues a word store. */
IO_QUEUE_DEFINE_WRITE(syIoWriteQueueStore32, IO_WRITE_WORD)
/* Queues setting a byte register's bits. */
IO_QUEUE_DEFINE_WRITE(syIoWriteQueueSetBits8, IO_WRITE_SET | IO_WRITE_BYTE)
/* Queues setting a halfword register's bits. */
IO_QUEUE_DEFINE_WRITE(syIoWriteQueueSetBits16, IO_WRITE_SET | IO_WRITE_HALFWORD)
/* Queues setting a word register's bits. */
IO_QUEUE_DEFINE_WRITE(syIoWriteQueueSetBits32, IO_WRITE_SET | IO_WRITE_WORD)
/* Queues clearing a byte register's bits. */
IO_QUEUE_DEFINE_WRITE(syIoWriteQueueClearBits8, IO_WRITE_CLEAR | IO_WRITE_BYTE)
/* Queues clearing a halfword register's bits. */
IO_QUEUE_DEFINE_WRITE(syIoWriteQueueClearBits16,
					  IO_WRITE_CLEAR | IO_WRITE_HALFWORD)
/* Queues clearing a word register's bits. */
IO_QUEUE_DEFINE_WRITE(syIoWriteQueueClearBits32, IO_WRITE_CLEAR | IO_WRITE_WORD)

/* Runs the queued register writes and empties the queue, with a copy of the */
/* executor function on the stack. */
void syIoWriteQueueFlushPending(void) {
	IO_WRITE_QUEUE *queue;
	u32 count;

	queue = &IoWriteQueueWork;
	count = queue->cnt;
	if(count != 0) {
		u32 wordCount = (u32)syResourceExecuteTransferCodeSize / 4;
		u8 routineBytes[wordCount * 4];
		QUEUE_FLUSH_ROUTINE routine = (QUEUE_FLUSH_ROUTINE)routineBytes;

		/* The function runs from its copy on the stack, which lasts until it
		 * ends. */
		syDMAXCopy(syResourceExecuteTransfer, routine,
				   DMA_COPY_WORDS(wordCount), REG_DMA3);
		routine(queue, count);
		queue->cnt = 0;
	}
}

#undef IO_QUEUE_DEFINE_WRITE

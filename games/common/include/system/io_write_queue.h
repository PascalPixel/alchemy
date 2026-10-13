#ifndef ALCHEMY_IO_WRITE_QUEUE_H
#define ALCHEMY_IO_WRITE_QUEUE_H

#include "edition.h"
#include "irqlock.h"
#include "types.h"

struct IoWriteQueue
{
	u16 count;
	u16 pad;
	u32 entries[32][3];
};

extern struct IoWriteQueue gIoWriteQueue;

void QueueIoWriteDelay1(u32 address, u32 value);
void QueueIoWriteDelay2(u32 address, u32 value);
void QueueIoWriteDelay3(u32 address, u32 value);
void QueueIoWriteDelay5(u32 address, u32 value);
void QueueIoWriteDelay6(u32 address, u32 value);
void QueueIoWriteDelay7(u32 address, u32 value);
void QueueIoWriteDelay9(u32 address, u32 value);
void QueueIoWriteDelay10(u32 address, u32 value);
void QueueIoWriteDelay11(u32 address, u32 value);
#if GAME_TLA
void IoWriteQueue_FlushPending(void);
#endif

// 割り込みを止めて I/O 書き込みキューに 1 件積む。満杯（32 件）なら何もしない
// address : 書き込み先、value : 値、control : 遅延フレーム << 16、または DMA の制御語
#define IO_WRITE_QUEUE_PUSH(address, value, control) \
	{                                                \
		struct IoWriteQueue *q = &gIoWriteQueue;     \
		u32					 saved;                  \
		s32					 count;                  \
		u16					*count_ = &q->count;     \
                                                     \
		IRQ_LOCK(saved);                             \
		count = *count_;                             \
		if (count <= 31)                             \
		{                                            \
			u32 *entry = q->entries[count];          \
                                                     \
			*count_ = count + 1;                     \
			*entry++ = (value);                      \
			*entry++ = (address);                    \
			*entry = (control);                      \
		}                                            \
		IRQ_UNLOCK(saved);                           \
	}

// IO_WRITE_QUEUE_PUSH と同じだが、IME のアドレスを呼び出し側の ime に持ったまま止める・戻す
#define IO_WRITE_QUEUE_PUSH_IME(ime, address, value, control) \
	{                                                         \
		struct IoWriteQueue *q;                               \
		u32					 saved;                           \
		s32					 count;                           \
		u16					*count_;                          \
                                                              \
		q = &gIoWriteQueue;                                   \
		count_ = &q->count;                                   \
		IME_SAVE(ime, saved);                                 \
		IME_DISABLE(ime);                                     \
		count = *count_;                                      \
		if (count <= 31)                                      \
		{                                                     \
			u32 *entry = q->entries[count];                   \
                                                              \
			*count_ = count + 1;                              \
			*entry++ = (value);                               \
			*entry++ = (address);                             \
			*entry = (control);                               \
		}                                                     \
		IME_RESTORE(ime, saved);                              \
	}

#endif

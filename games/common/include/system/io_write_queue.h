#ifndef ALCHEMY_IO_WRITE_QUEUE_H
#define ALCHEMY_IO_WRITE_QUEUE_H

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
void IoWriteQueue_FlushPending(void);

// 割り込みを止めて I/O 書き込みキューに 1 件積む。満杯（32 件）なら何もしない
// address : 書き込み先、value : 値、control : 遅延フレーム << 16、または DMA の制御語
/* FAKEMATCH: the count is stored through a u16 pointer; stored as q->count, 21 objects differ. One text for the 9 QueueIoWriteDelay writers and 67 inline pushes; with braces alone, since an outer one-pass block changes 9 objects. Not covered: the four pushes of BattleEffect_RunShockwaveScene and the first of BattleFx_PlayUnitElementEffect, where the ROM keeps the IME address in one register across the frame loop. */
#define IO_WRITE_QUEUE_PUSH(address, value, control) \
	{                                                \
		struct IoWriteQueue *q = &gIoWriteQueue;     \
		u32					 saved;                  \
		s32					 count;                  \
                                                     \
		IRQ_LOCK(saved);                             \
		count = q->count;                            \
		if (count <= 31)                             \
		{                                            \
			u32 *entry = q->entries[count];          \
                                                     \
			*(u16 *)&q->count = count + 1;           \
			*entry++ = (value);                      \
			*entry++ = (address);                    \
			*entry = (control);                      \
		}                                            \
		IRQ_UNLOCK(saved);                           \
	}

#endif

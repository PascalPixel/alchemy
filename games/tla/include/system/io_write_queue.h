#ifndef ALCHEMY_IO_WRITE_QUEUE_H
#define ALCHEMY_IO_WRITE_QUEUE_H

#include "edition.h"
#include "irq_lock.h"
#include "types.h"

struct IoWriteQueue {
  u16 count;
  u16 pad;
  u32 entries[32][3];
};

extern struct IoWriteQueue gIoWriteQueue;

// A control word's bits 16 and 17 give a register write's width and bits 18
// and 19 its operation, as Resource_ExecuteTransfer reads them
// (system/resource/execute_transfer.s); a word with no width is a DMA.

/// Writes a byte.
#define IO_WRITE_8 (1 << 16)
/// Writes a halfword.
#define IO_WRITE_16 (2 << 16)
/// Writes a word.
#define IO_WRITE_32 (3 << 16)
/// ORs the value into the register instead of storing it.
#define IO_WRITE_SET (4 << 16)
/// Clears the value's bits in the register instead of storing it.
#define IO_WRITE_CLEAR (8 << 16)

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

/// Disables interrupts to enqueue one I/O write; a full 32-entry queue is
/// ignored. address: destination, value: value to write, control: delay frames
/// << 16 or a DMA control word.
// The body is braces alone: as do { } while (0), 15 objects of tla-en differ,
// the nine writers' among them (2 of 25 instructions in QueueIoWriteDelay1).
// The count is read and written as *&q->count, a pointer to the member: as
// q->count, the struct store of the new count is scheduled before the first
// entry word (4 of 25 instructions in QueueIoWriteDelay1 in tla-en).
#define IO_WRITE_QUEUE_PUSH(address, value, control)                           \
  {                                                                            \
    struct IoWriteQueue *q = &gIoWriteQueue;                                   \
    u32 saved;                                                                 \
    s32 count;                                                                 \
                                                                               \
    IRQ_LOCK(saved);                                                           \
    count = *&q->count;                                                        \
    if (count <= 31) {                                                         \
      u32 *entry = q->entries[count];                                          \
                                                                               \
      *&q->count = count + 1;                                                  \
      *entry++ = (value);                                                      \
      *entry++ = (address);                                                    \
      *entry = (control);                                                      \
    }                                                                          \
    IRQ_UNLOCK(saved);                                                         \
  }

/// Like IO_WRITE_QUEUE_PUSH, but keeps IME's address in the caller's ime
/// variable while disabling and restoring interrupts.
// The count is read and written as in IO_WRITE_QUEUE_PUSH.
#define IO_WRITE_QUEUE_PUSH_IME(ime, address, value, control)                  \
  {                                                                            \
    struct IoWriteQueue *q;                                                    \
    u32 saved;                                                                 \
    s32 count;                                                                 \
                                                                               \
    q = &gIoWriteQueue;                                                        \
    IME_SAVE(ime, saved);                                                      \
    IME_DISABLE(ime);                                                          \
    count = *&q->count;                                                        \
    if (count <= 31) {                                                         \
      u32 *entry = q->entries[count];                                          \
                                                                               \
      *&q->count = count + 1;                                                  \
      *entry++ = (value);                                                      \
      *entry++ = (address);                                                    \
      *entry = (control);                                                      \
    }                                                                          \
    IME_RESTORE(ime, saved);                                                   \
  }

#endif

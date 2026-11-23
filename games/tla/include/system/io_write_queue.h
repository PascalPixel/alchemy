#ifndef ALCHEMY_IO_WRITE_QUEUE_H
#define ALCHEMY_IO_WRITE_QUEUE_H

/// @file
/// The I/O write queue: register writes held until VBlank.

#include "edition.h"
#include "irq_lock.h"
#include "types.h"

/// The writes the queue holds.
#define IO_WRITE_QUEUE_SIZE 32

/// The register writes VBlank carries out once the frame loop has finished a
/// frame, each its value, address and control word; IoWriteQueue_FlushPending
/// empties it.
struct IoWriteQueue {
  u16 count;
  u16 padding;
  u32 entries[IO_WRITE_QUEUE_SIZE][3];
};

/// The queue IO_WRITE_QUEUE_PUSH and the IoWriteQueue_Store writers fill, and
/// IoWriteQueue_FlushPending empties at VBlank.
extern struct IoWriteQueue gIoWriteQueue;

// A control word's bits 16 and 17 give a register write's width and bits 18
// and 19 its operation, as Resource_ExecuteTransfer reads them
// (system/decompress_tagged_lz.s); a word with no width is a DMA.

/// Writes a byte.
#define IO_WRITE_BYTE (1 << 16)
/// Writes a halfword.
#define IO_WRITE_HALFWORD (2 << 16)
/// Writes a word.
#define IO_WRITE_WORD (3 << 16)
/// ORs the value into the register instead of storing it.
#define IO_WRITE_SET (4 << 16)
/// Clears the value's bits in the register instead of storing it.
#define IO_WRITE_CLEAR (8 << 16)

void IoWriteQueue_Store8(u32 address, u32 value);
void IoWriteQueue_Store16(u32 address, u32 value);
void IoWriteQueue_Store32(u32 address, u32 value);
void IoWriteQueue_SetBits8(u32 address, u32 value);
void IoWriteQueue_SetBits16(u32 address, u32 value);
void IoWriteQueue_SetBits32(u32 address, u32 value);
void IoWriteQueue_ClearBits8(u32 address, u32 value);
void IoWriteQueue_ClearBits16(u32 address, u32 value);
void IoWriteQueue_ClearBits32(u32 address, u32 value);
void IoWriteQueue_FlushPending(void);

/// Queues one register write with interrupts off; a write to a full queue is
/// dropped. `control` is an IO_WRITE_ width, with IO_WRITE_SET or
/// IO_WRITE_CLEAR, to write `value` to `address`; or a DMA control word, to
/// copy by DMA 3 from `value` to `address`.
#define IO_WRITE_QUEUE_PUSH(address, value, control)                           \
  {                                                                            \
    struct IoWriteQueue *q = &gIoWriteQueue;                                   \
    u32 saved;                                                                 \
    s32 count;                                                                 \
                                                                               \
    IRQ_LOCK(saved);                                                           \
    count = *&q->count;                                                        \
    if (count < IO_WRITE_QUEUE_SIZE) {                                         \
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
    if (count < IO_WRITE_QUEUE_SIZE) {                                         \
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

#ifndef ALCHEMY_IO_WRITE_QUEUE_H
#define ALCHEMY_IO_WRITE_QUEUE_H

/// @file
/// The register write queue that VBlank carries out after each finished frame.

#include "edition.h"
#include "irq_lock.h"
#include "types.h"

/// The register writes VBlank carries out once the frame loop has finished a
/// frame; IoWriteQueue_FlushPending empties it.
struct IoWriteQueue {
  u16 count; ///< Entries waiting, at most 32.
  u16 pad;   ///< not named by TBS code
  /// Each entry is the value, the address and the control word, in that order.
  /// A control word with a width (IO_WRITE_8 to IO_WRITE_32) is a register
  /// write; one without is a DMA, whose three words go to DMA3's source,
  /// destination and control registers.
  u32 entries[32][3];
};

/// The queue IO_WRITE_QUEUE_PUSH and the IoWriteQueue_ writers fill, and
/// IoWriteQueue_FlushPending empties at VBlank.
extern struct IoWriteQueue gIoWriteQueue;

// A control word's bits 16 and 17 give a register write's width and bits 18
// and 19 its operation; Resource_ExecuteTransfer reads them.

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

void IoWriteQueue_Store8(u32 address, u32 value);
void IoWriteQueue_Store16(u32 address, u32 value);
void IoWriteQueue_Store32(u32 address, u32 value);
void IoWriteQueue_SetBits8(u32 address, u32 value);
void IoWriteQueue_SetBits16(u32 address, u32 value);
void IoWriteQueue_SetBits32(u32 address, u32 value);
void IoWriteQueue_ClearBits8(u32 address, u32 value);
void IoWriteQueue_ClearBits16(u32 address, u32 value);
void IoWriteQueue_ClearBits32(u32 address, u32 value);

/// Disables interrupts to add one register write or DMA to the queue; a write
/// to a full queue is dropped.
/// @param control a width (IO_WRITE_8 to IO_WRITE_32), with IO_WRITE_SET or
/// IO_WRITE_CLEAR to change bits; or a DMA control word.
// The body is braces alone: as do { } while (0), 8 linked sections of tbs-en
// differ (the nine writers, six objects with inline pushes and the overlays).
// The count is read and written as *&q->count, a pointer to the member: as
// q->count, the struct store of the new count is scheduled before the first
// entry word (2 of 27 instructions in IoWriteQueue_Store8 in tbs-en).
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

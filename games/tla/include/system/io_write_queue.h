#ifndef ALCHEMY_IO_WRITE_QUEUE_H
#define ALCHEMY_IO_WRITE_QUEUE_H

#include "edition.h"
#include "irqlock.h"
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
/* FAKEMATCH: the count is read and stored through count_, a u16 pointer to
 * q->count held in a local; written as q->count, 15 linked sections of tbs-en
 * differ (measured 2026-10-13: the nine QueueIoWriteDelay writers, 18 functions
 * with inline pushes in 12 objects, and the overlays). It replaces the earlier
 * tagged cast store *(u16 *)&q->count, so it is the same device in another
 * form. The body is braces alone, not do { } while (0): an outer one-pass block
 * changes 8 linked sections of tbs-en (the QueueIoWriteDelay writers, six
 * objects with inline pushes and the overlays; ⚓️ unchanged; measured
 * 2026-10-13). */
#define IO_WRITE_QUEUE_PUSH(address, value, control)                           \
  {                                                                            \
    struct IoWriteQueue *q = &gIoWriteQueue;                                   \
    u32 saved;                                                                 \
    s32 count;                                                                 \
    u16 *count_ = &q->count;                                                   \
                                                                               \
    IRQ_LOCK(saved);                                                           \
    count = *count_;                                                           \
    if (count <= 31) {                                                         \
      u32 *entry = q->entries[count];                                          \
                                                                               \
      *count_ = count + 1;                                                     \
      *entry++ = (value);                                                      \
      *entry++ = (address);                                                    \
      *entry = (control);                                                      \
    }                                                                          \
    IRQ_UNLOCK(saved);                                                         \
  }

/// Like IO_WRITE_QUEUE_PUSH, but keeps IME's address in the caller's ime
/// variable while disabling and restoring interrupts.
/* FAKEMATCH: the count goes through count_ as in IO_WRITE_QUEUE_PUSH (see its
 * tag): written as q->count, ElementGlow_Play and
 * BattleEffect_RunJudgment, its users, are among the sections that
 * differ. */
#define IO_WRITE_QUEUE_PUSH_IME(ime, address, value, control)                  \
  {                                                                            \
    struct IoWriteQueue *q;                                                    \
    u32 saved;                                                                 \
    s32 count;                                                                 \
    u16 *count_;                                                               \
                                                                               \
    q = &gIoWriteQueue;                                                        \
    count_ = &q->count;                                                        \
    IME_SAVE(ime, saved);                                                      \
    IME_DISABLE(ime);                                                          \
    count = *count_;                                                           \
    if (count <= 31) {                                                         \
      u32 *entry = q->entries[count];                                          \
                                                                               \
      *count_ = count + 1;                                                     \
      *entry++ = (value);                                                      \
      *entry++ = (address);                                                    \
      *entry = (control);                                                      \
    }                                                                          \
    IME_RESTORE(ime, saved);                                                   \
  }

#endif

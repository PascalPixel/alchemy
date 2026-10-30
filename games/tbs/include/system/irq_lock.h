#ifndef ALCHEMY_IRQ_LOCK_H
#define ALCHEMY_IRQ_LOCK_H

/// @file
/// Saving, disabling and restoring the interrupt master enable.

#include "io_reg.h"
#include "types.h"

// The three steps take ime, a pointer to REG_IME. Disabling writes the
// pointer's low halfword (0x0208, bit 0 clear) back through it. Each step is a
// do { } while (0) statement macro, the shape every lock and unlock site
// matches through. Serial_Initialize writes IRQ_LOCK out (see its comment).

/// Saves the original IME value in saved.
#define IME_SAVE(ime, saved)                                                   \
  do {                                                                         \
    (saved) = *(ime);                                                          \
  } while (0)

/// Disables interrupts.
#define IME_DISABLE(ime)                                                       \
  do {                                                                         \
    *(ime) = (u16)(u32)(ime);                                                  \
  } while (0)

/// Restores IME from saved.
#define IME_RESTORE(ime, saved)                                                \
  do {                                                                         \
    *(ime) = (saved);                                                          \
  } while (0)

/// Disables interrupts and saves IME in saved; IRQ_UNLOCK restores it.
#define IRQ_LOCK(saved)                                                        \
  do {                                                                         \
    volatile u16 *ime_ = &REG_IME;                                             \
                                                                               \
    IME_SAVE(ime_, saved);                                                     \
    IME_DISABLE(ime_);                                                         \
  } while (0)

/// Restores the IME value saved by IRQ_LOCK.
#define IRQ_UNLOCK(saved)                                                      \
  do {                                                                         \
    REG_IME = (saved);                                                         \
  } while (0)

#endif

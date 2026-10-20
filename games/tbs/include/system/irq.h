#ifndef ALCHEMY_IRQ_H
#define ALCHEMY_IRQ_H

/// @file
/// The hardware's interrupt sources and the interrupt handler table.

#include "edition.h"
#include "types.h"

/// The hardware's interrupt sources: each one's bit number in IE and IF, and
/// its slot in gIrqHandlers.
enum {
  IRQ_VBLANK,
  IRQ_HBLANK,
  IRQ_VCOUNT,
  IRQ_TIMER0,
  IRQ_TIMER1,
  IRQ_TIMER2,
  IRQ_TIMER3,
  IRQ_SERIAL,
  IRQ_DMA0,
  IRQ_DMA1,
  IRQ_DMA2,
  IRQ_DMA3,
  IRQ_KEYPAD,
  IRQ_GAMEPAK,
  IRQ_COUNT
};

#define INTR_FLAG_VBLANK (1 << IRQ_VBLANK)
#define INTR_FLAG_TIMER3 (1 << IRQ_TIMER3)
#define INTR_FLAG_SERIAL (1 << IRQ_SERIAL)
#define INTR_FLAG_KEYPAD (1 << IRQ_KEYPAD)
#define INTR_FLAG_GAMEPAK (1 << IRQ_GAMEPAK)

/// An interrupt source's handler, called by the IWRAM dispatcher.
typedef void (*InterruptHandler)(void);

/// The handler the IWRAM dispatcher calls for each interrupt source.
extern InterruptHandler gIrqHandlers[IRQ_COUNT];

/// The handlers installed at power-on, copied into gIrqHandlers.
extern const u32 gRuntime_IrqHandlers[IRQ_COUNT];

/// The interrupt dispatcher at the start of the IWRAM-resident code, whose
/// other routines are reached at fixed offsets from it (iwram_call.h).
extern u8 IwramIrqMain[];
/// An instruction inside the dispatcher (`orr r3, r3, #0x92`); see
/// Dma_StopAllThenRunHook's XXX.
extern const u32 IwramIrqModeWord;
/// Where the IWRAM-resident code lies in ROM (main.ld), 0x1400 bytes copied to
/// IwramIrqMain at power-on.
extern const u8 IwramRuntime_Rom[];
/// The BIOS's interrupt vector, pointed at IwramIrqMain at power-on.
extern void *gIrqVector;

#endif

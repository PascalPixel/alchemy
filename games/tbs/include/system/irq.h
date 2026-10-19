#ifndef ALCHEMY_IRQ_H
#define ALCHEMY_IRQ_H

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

typedef void (*InterruptHandler)(void);

/// The handler the IWRAM dispatcher calls for each interrupt source.
extern InterruptHandler gIrqHandlers[IRQ_COUNT];

/// The handlers installed at power-on, copied into gIrqHandlers.
extern const u32 gRuntime_IrqHandlers[IRQ_COUNT];

extern u8 IwramIrqMain[];
extern const u32 IwramIrqModeWord;
extern const u8 IwramRuntime_Rom[];
extern void *gIrqVector;

#endif

#ifndef ALCHEMY_IRQ_H
#define ALCHEMY_IRQ_H

#include "TYPES.H"

typedef void (*InterruptHandler)(void);

/* Writable dispatch slots, one per interrupt bit. */
extern InterruptHandler gIrqHandlers[14];

/* Initial dispatch image copied as fourteen words. The TBS European image
   also carries resident ARM instructions, so it is not a callback array. */
extern const u32 Runtime_IrqHandlers[14];

/* Resident code bank, its ROM copy source, and the BIOS interrupt vector. */
extern u8 IwramIrqMain[];
extern const u8 IwramRuntime_Rom[];
extern void *Data_03007ffc;

void Runtime_SetIrqHandler(u32 irq, s32 vcount, InterruptHandler handler);
void Runtime_InstallIwramAndIrqs(void);
void Runtime_IgnoreInterrupt(void);

#endif

#ifndef ALCHEMY_IRQ_H
#define ALCHEMY_IRQ_H

#include "types.h"

typedef void (*InterruptHandler)(void);

extern InterruptHandler gIrqHandlers[14];

extern const u32 Runtime_IrqHandlers[14];

extern u8 IwramIrqMain[];
extern const u32 IwramIrqModeWord;
extern const u8 IwramRuntime_Rom[];
extern void *gIrqVector;

void Runtime_SetIrqHandler(u32 irq, s32 vcount, InterruptHandler handler);
void Runtime_InstallIwramAndIrqs(void);
void Runtime_IgnoreInterrupt(void);

#endif

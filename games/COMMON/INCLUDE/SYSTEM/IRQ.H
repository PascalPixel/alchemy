#ifndef ALCHEMY_IRQ_H
#define ALCHEMY_IRQ_H

#include "TYPES.H"

typedef void (*InterruptHandler)(void);

void Runtime_SetIrqHandler(u32 irq, s32 vcount, InterruptHandler handler);
void Runtime_InstallIwramAndIrqs(void);
void RuntimeDispatch_ReservedNoOp03008(void);

#endif

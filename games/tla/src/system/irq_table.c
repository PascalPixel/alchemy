// The interrupt handlers: the table the IWRAM dispatcher calls through, and the handlers start-up
// copies into it.

#include "system.h"

// The handler the IWRAM dispatcher calls for each interrupt source; empty until start-up copies
// DefaultIrqHandlers in.
INTERRUPT_HANDLER       IrqHandlers[IRQ_COUNT]        = {0};

// The handlers start-up copies into IrqHandlers: only the keypad interrupt has one of its own.
const INTERRUPT_HANDLER DefaultIrqHandlers[IRQ_COUNT] = {
    [IRQ_VBLANK]  = syIRQIgnore,
    [IRQ_HBLANK]  = syIRQIgnore,
    [IRQ_VCOUNT]  = syIRQIgnore,
    [IRQ_TIMER0]  = syIRQIgnore,
    [IRQ_TIMER1]  = syIRQIgnore,
    [IRQ_TIMER2]  = syIRQIgnore,
    [IRQ_TIMER3]  = syIRQIgnore,
    [IRQ_SERIAL]  = syIRQIgnore,
    [IRQ_DMA0]    = syIRQIgnore,
    [IRQ_DMA1]    = syIRQIgnore,
    [IRQ_DMA2]    = syIRQIgnore,
    [IRQ_DMA3]    = syIRQIgnore,
    [IRQ_KEYPAD]  = syInputHandleKeyInterrupt,
    [IRQ_GAMEPAK] = syIRQIgnore,
};

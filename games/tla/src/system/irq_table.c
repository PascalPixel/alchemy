/// @file
/// The interrupt handlers: the table the IWRAM dispatcher calls through, and
/// the handlers start-up copies into it.

#include "types.h"
#include "system/irq.h"
#include "system/input.h"

/// The handler the IWRAM dispatcher calls for each interrupt source; empty
/// until start-up copies gDefaultIrqHandlers in.
InterruptHandler gIrqHandlers[IRQ_COUNT] = {0};

/// The handlers start-up copies into gIrqHandlers: only the keypad interrupt
/// has one of its own.
const InterruptHandler gDefaultIrqHandlers[IRQ_COUNT] = {
    [IRQ_VBLANK] = Irq_Ignore,
    [IRQ_HBLANK] = Irq_Ignore,
    [IRQ_VCOUNT] = Irq_Ignore,
    [IRQ_TIMER0] = Irq_Ignore,
    [IRQ_TIMER1] = Irq_Ignore,
    [IRQ_TIMER2] = Irq_Ignore,
    [IRQ_TIMER3] = Irq_Ignore,
    [IRQ_SERIAL] = Irq_Ignore,
    [IRQ_DMA0] = Irq_Ignore,
    [IRQ_DMA1] = Irq_Ignore,
    [IRQ_DMA2] = Irq_Ignore,
    [IRQ_DMA3] = Irq_Ignore,
    [IRQ_KEYPAD] = Input_HandleKeyInterrupt,
    [IRQ_GAMEPAK] = Irq_Ignore,
};

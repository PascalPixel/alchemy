/* @file */
/* The interrupt handlers: the table the IWRAM dispatcher calls through, and */
/* the handlers start-up copies into it. */

#include "system.h"

/* The handler the IWRAM dispatcher calls for each interrupt source; empty */
/* until start-up copies DefaultIrqHandlers in. */
INTERRUPT_HANDLER IrqHandlers[IRQ_COUNT] = {0};

/* The handlers start-up copies into IrqHandlers: only the keypad interrupt */
/* has one of its own. */
const INTERRUPT_HANDLER DefaultIrqHandlers[IRQ_COUNT] = {
	[IRQ_VBLANK] = syIrqIgnore,
	[IRQ_HBLANK] = syIrqIgnore,
	[IRQ_VCOUNT] = syIrqIgnore,
	[IRQ_TIMER0] = syIrqIgnore,
	[IRQ_TIMER1] = syIrqIgnore,
	[IRQ_TIMER2] = syIrqIgnore,
	[IRQ_TIMER3] = syIrqIgnore,
	[IRQ_SERIAL] = syIrqIgnore,
	[IRQ_DMA0] = syIrqIgnore,
	[IRQ_DMA1] = syIrqIgnore,
	[IRQ_DMA2] = syIrqIgnore,
	[IRQ_DMA3] = syIrqIgnore,
	[IRQ_KEYPAD] = syInputHandleKeyInterrupt,
	[IRQ_GAMEPAK] = syIrqIgnore,
};

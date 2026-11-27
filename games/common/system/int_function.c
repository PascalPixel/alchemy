/* syIntFunction, the same in both games. Both games' system/system_frame.c
   include it where the function stands. */

/* Installs `handler` for interrupt `irq` and enables that interrupt, or with a
   null handler disables it and installs the do-nothing handler. */
void syIntFunction(Uint32 irq, Sint32 vcount, INTERRUPT_HANDLER handler)
{
	if(irq <= IRQ_GAMEPAK) {
		Uint32 ime;
		Uint32 bit;
		Uint32 ie;

		IRQ_LOCK(ime);
		bit = 1 << irq;
		ie = REG_IE & ~bit;
		if(handler != 0)
			ie |= bit;
		REG_IE = ie;
		if(irq <= IRQ_VCOUNT) {
			/* The display interrupts also need their DISPSTAT enable */
			Uint32 enable = DISPSTAT_VBLANK_INTR << irq;
			Uint32 keep = ~enable;
			Uint32 stat;

			if(irq == IRQ_VCOUNT) {
				enable |= DISPSTAT_VCOUNT_LINE(vcount);
				/* The old line goes too. */
				keep &= DISPSTAT_FLAGS;
			}
			stat = REG_DISPSTAT & keep;
			if(handler != 0)
				stat |= enable;
			REG_DISPSTAT = stat;
		}
		if(handler != 0)
			IrqHandlers[irq] = handler;
		else
			IrqHandlers[irq] = syIRQIgnore;
		IRQ_UNLOCK(ime);
	}
}

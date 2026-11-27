/* sySerialRemoveIRQHandlers and sySerialWaitForStatMask, the same in both
   games. Both games' system/system_serial.c include it where the functions
   stand. */

/* Clears SerialExchangeActive, which stops the per-tick exchange, and removes
   the link interrupt handlers. */
void sySerialRemoveIRQHandlers(void)
{
	SerialExchangeActive = 0;
	syIntFunction(IRQ_SERIAL, 0, 0);
	syIntFunction(IRQ_TIMER3, 0, 0);
}

/* Waits a tick at a time until every status bit in `mask` is set in
   LinkStatusWork, then returns this console's multiplayer id. */
Uint32 sySerialWaitForStatMask(Sint32 mask)
{
	while((mask & LinkStatusWork) != mask) {
		sySynch(1);
	}
	return SIO_ID_OF(REG_SIOCNT);
}

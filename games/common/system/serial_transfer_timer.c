/* sySerialEnableTransferTimer and sySerialDisableTransfer, the same in both
   games. Both games' system/system_serial.c include it where the functions
   stand. */

/* Lets the timer start transfers again once a link mode is set, by setting
   SerialStateWork.transferEnabled. */
void sySerialEnableTransferTimer(void)
{
	SERIAL_STATE *state = &SerialStateWork;

	if(state->mode != 0)
		state->transferEnabled = 1;
}

/* Unused: disables the serial and timer 3 interrupts, stops the transfer timer
   and clears SerialStateWork.transferEnabled. */
void sySerialDisableTransfer(void)
{
	SERIAL_STATE *state;

	REG_IME = 0;
	REG_IE &= (Uint16)~(INTR_FLAG_SERIAL | INTR_FLAG_TIMER3);
	REG_IME = 1;
	REG_SIOCNT16 = SIO_MULTI_MODE | SIO_115200_BPS;
	/* Timer 3 stopped, with its reload set */
	REG_TM3CNT = SERIAL_TIMER_RELOAD;
	REG_IF = INTR_FLAG_SERIAL | INTR_FLAG_TIMER3;
	state = &SerialStateWork;
	state->transferEnabled = 0;
}

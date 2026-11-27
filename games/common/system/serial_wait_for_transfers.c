/* sySerialWaitForTransfers and sySerialGetActiveTransfers, the same in both
   games. Both games' system/system_serial.c include it where the functions
   stand. */

/* Waits until both the send and the receive have finished, giving up after
   SERIAL_TRANSFER_TIMEOUT_TICKS ticks. Reached through the stub table. */
void sySerialWaitForTransfers(void)
{
	Uint32 cnt;

	cnt = 0;
	while(SerialSendSource != 0 || SerialReceiveDestination != 0) {
		sySynch(1);
		cnt++;
		if(cnt >= SERIAL_TRANSFER_TIMEOUT_TICKS)
			return;
	}
}

/* Returns bit 0 while a send runs and bit 1 while a receive runs. The link
   battle and the Battle Arena poll it between ticks. */
Sint32 sySerialGetActiveTransfers(void)
{
	Sint32 flg;

	flg = 0;
	if(SerialSendSource != 0) {
		flg = 1;
	}
	if(SerialReceiveDestination != 0) {
		flg |= 2;
	}
	return flg;
}

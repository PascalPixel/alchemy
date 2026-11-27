/* sySerialExchangePayloads, the same in both games. Both games'
   system/system_serial.c include it where the function stands. */

/* Once connected (SERIAL_PHASE_CONNECTED), gathers the received payloads into
   `receive` and builds the next packet from `send`. Called by the VBlank
   handler while SerialExchangeActive is set, which stores the result in
   LinkStatusWork. Returns the link status: in bits 0 and 1 the consoles heard
   from this tick (both: LINK_STATUS_HEARD_BOTH, which the block transfer and
   sySerialWaitForStatMask wait for), in bits 8 and 9 those heard from since the
   link began, and LINK_STATUS_CLOCKING, LINK_STATUS_ERROR and
   LINK_STATUS_EXTRA_CONSOLE. */
Sint32 sySerialExchangePayloads(void *send, void *receive)
{
	volatile Uint32 *siocnt;
	Uint32			 control;

	siocnt = &REG_SIOCNT;
	control = *siocnt;
	if(SerialStateWork.phase == SERIAL_PHASE_CONNECTED) {
		sySerialCollectReceivedPayloads(receive);
		sySerialPrepareSendPacket(send);
		SerialStateWork.packetNum++;
	}
	return sySerialGetLinkStat(control);
}

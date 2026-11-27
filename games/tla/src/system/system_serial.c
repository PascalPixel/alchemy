/* The link cable: payload exchange, block transfers and the link test. Part of
   system.c. */

#include "field/actor.h"

enum {
	/* A packet: its number, missing mask, checksum and the 24-byte payload. */
	SERIAL_PACKET_HALFWORDS = 14,
	SERIAL_PAYLOAD_BYTES = 24, /* one console's payload each tick */
	SERIAL_BLOCK_BYTES = 20,   /* bulk data carried in each payload */
	/* About 10,000 seconds, nearly three hours. */
	SERIAL_TRANSFER_TIMEOUT_TICKS = 600000,
	SERIAL_IDLE_HALFWORD = 0xfefe, /* sent and seen between packets */
	/* Timer 3's reload: it overflows 13981 cycles on, from one halfword
	   transfer to the next, about 20 a tick. */
	SERIAL_TIMER_RELOAD = 65536 - 13981
};

/* Sets the function the save calls between sector writes (0 for none) and
   starts its count again at 0 of BUP_SECTORS_PER_PART; does nothing while no
   save workspace (heap slot HEAP_SLOT_BUP) is allocated. */
void syBupSetProgressCallback(void (*progress)(Sint32 step))
{
	BUP_WORKSPACE *state = Ram_HeapSlots->bupState;

	if(state == 0)
		return;
	state->progress = progress;
	state->sectorCnt = 0;
	state->sectorTotal = BUP_SECTORS_PER_PART;
}

/* Resets the serial port into multiplayer mode, installs the serial and timer 3
   interrupt handlers, clears SerialStateWork and points its packet buffers into
   its storage, clears the bulk transfer state and sets SerialExchangeActive,
   which starts the per-tick exchange. */
void sySerialInit(void)
{
	Uint32		  savedIme;
	Sint32		  idx;
	SERIAL_STATE *state;

	IRQ_LOCK(savedIme);
	/* Replace both interrupt handlers before configuring multiplayer
	   transfers. */
	syIntFunction(IRQ_SERIAL, 0, sySerialHandleTransferInterrupt);
	syIntFunction(IRQ_TIMER3, 0, sySerialHandleTransferInterrupt);
	REG_IME = 0;
	REG_IE &= (Uint16)~(INTR_FLAG_SERIAL | INTR_FLAG_TIMER3);
	if((REG_IF & INTR_FLAG_SERIAL) != 0)
		REG_IF = INTR_FLAG_SERIAL;
	if((REG_IF & INTR_FLAG_TIMER3) != 0)
		REG_IF = INTR_FLAG_TIMER3;
	REG_RCNT = RCNT_GPIO_MODE;
	REG_RCNT = RCNT_SIO_MODE;
	REG_SIOCNT = SIO_32BIT_MODE;
	REG_RCNT = RCNT_SIO_MODE;
	REG_SIOCNT = SIO_MULTI_MODE;
	REG_SIOCNT16 = REG_SIOCNT16 | SIO_INTR_ENABLE | SIO_115200_BPS;
	state = &SerialStateWork;
	REG_IME = 1;
	DMA_FILL32(0, state, sizeof(*state));
	state->sendIdx = -1;
	state->sendBuf[0] = (Uint16 *)(state->storage + 0);
	state->sendBuf[1] = (Uint16 *)(state->storage + SERIAL_PACKET_BYTES);
	/* Each console has separate incoming, completed and pending packet
	   buffers. */
	for(idx = 0; idx <= 1; idx++) {
		state->incomingBuf[idx] = (Uint16 *)(state->storage +
			SERIAL_PACKET_BYTES * 2 + idx * SERIAL_PACKET_BYTES * 3);
		state->readyBuf[idx] = (Uint16 *)(state->storage +
			SERIAL_PACKET_BYTES * 3 + idx * SERIAL_PACKET_BYTES * 3);
		state->pendingBuf[idx] = (Uint16 *)(state->storage +
			SERIAL_PACKET_BYTES * 4 + idx * SERIAL_PACKET_BYTES * 3);
	}
	REG_IME = 0;
	REG_IE |= INTR_FLAG_SERIAL;
	REG_IME = 1;
	SerialExchangeActive = 1;
	LinkExchangeState = 0;
	SerialSendSource = 0;
	SerialSendSize = 0;
	SerialReceiveDestination = 0;
	SerialReceivedSize = 0;
	syLinkBattleResetTransferState();
	IRQ_UNLOCK(savedIme);
}

/* Lets the timer start transfers again once a link mode is set, by setting
   SerialStateWork.transferEnabled. */
void sySerialEnableTransferTimer(void)
{
	SERIAL_STATE *state = &SerialStateWork;

	if(state->mode != 0)
		state->transferEnabled = 1;
}

/* Disables the serial and timer 3 interrupts, stops the transfer timer and
   clears SerialStateWork.transferEnabled. Nothing in the game calls it. */
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

/* The parent's connection: timer 3, not the serial interrupt, now clocks the
   transfers, in `mode` (SERIAL_MODE_CLOCKING). */
static __inline__ void sySerialStartClocking(
	volatile Uint32 *siocnt, Uint8 mode)
{
	syIOWrite16(0, &REG_IME);
	{
		Uint32 ie = REG_IE;

		syIOWrite16((ie & ~INTR_FLAG_SERIAL) | INTR_FLAG_TIMER3, &REG_IE);
		syIOWrite16(1, &REG_IME);
		/* SIOCNT's high byte: clear SIO_INTR_ENABLE. */
		*((volatile Sint8 *)siocnt + 1) &= ~(SIO_INTR_ENABLE >> 8);
		REG_IF = INTR_FLAG_SERIAL | INTR_FLAG_TIMER3;
		REG_TM3CNT = SERIAL_TIMER_RELOAD;
		SerialStateWork.mode = mode;
	}
}

/* The link status the polls return, from SIOCNT's `control`: the consoles heard
   from, LINK_STATUS_CLOCKING, LINK_STATUS_ERROR and LINK_STATUS_EXTRA_CONSOLE
   when the multiplayer id says a third is there. */
static __inline__ Sint32 sySerialGetLinkStat(Uint32 control)
{
	Sint32 stat;
	Sint32 result;

	stat = SerialStateWork.curMask | (SerialStateWork.receivedMask << 8);
	if(SerialStateWork.mode == SERIAL_MODE_CLOCKING)
		stat |= LINK_STATUS_CLOCKING;
	result = sySerialAddErrorFlag(stat);
	if(SIO_ID_OF(control) > 1)
		result |= LINK_STATUS_EXTRA_CONSOLE;
	return result;
}

/* Looks for the link partner and, once connected, does one exchange (as
   sySerialExchangePayloads). */
Sint32 sySerialPollAndTransfer(void *send, void *receive)
{
	volatile Uint32 *siocnt;
	Uint32			 control;

	siocnt = &REG_SIOCNT;
	control = *siocnt;
	switch(SerialStateWork.phase) {
	case SERIAL_PHASE_SEARCHING: {
		Uint8 multiplayerId = control & SIO_ID;
		Uint8 mode;

		/* The parent waits here until SIO_MULTI_SD is set with SIO_START clear;
		   a child connects at once. */
		if(multiplayerId == 0 &&
			(mode = control & (SIO_START | SIO_MULTI_SD)) ==
				SERIAL_MODE_CLOCKING) {
			Uint8 serialError = control & SIO_MULTI_SI;

			if(serialError == 0 && SerialStateWork.sendIdx == -1)
				sySerialStartClocking(siocnt, mode);
		} else if(multiplayerId == 0) {
			break;
		}
		SerialStateWork.phase = SERIAL_PHASE_CONNECTED;
	}
		/* Fall through. */
	case SERIAL_PHASE_CONNECTED:
		sySerialCollectReceivedPayloads(receive);
		sySerialPrepareSendPacket(send);
		break;
	}

	SerialStateWork.packetNum++;
	return sySerialGetLinkStat(control);
}

/* As sySerialPollAndTransfer, without exchanging data: sySynch() calls it once
   a tick while SerialExchangeActive is set, and the VBlank handler does the
   exchange. */
Sint32 sySerialPollStat(void)
{
	volatile Uint32 *siocnt;
	Uint32			 control;

	siocnt = &REG_SIOCNT;
	control = *siocnt;
	if(SerialStateWork.phase == SERIAL_PHASE_SEARCHING) {
		Uint8 mode = control & (SIO_START | SIO_MULTI_SD);

		if(mode == SERIAL_MODE_CLOCKING) {
			Uint8 serialError = control & SIO_MULTI_SI;

			if(serialError == 0 && SerialStateWork.sendIdx == -1)
				sySerialStartClocking(siocnt, mode);
			SerialStateWork.phase = SERIAL_PHASE_CONNECTED;
		}
		SerialStateWork.packetNum++;
	}

	return sySerialGetLinkStat(control);
}

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

/* Builds the packet to send in sendBuf[0] (SERIAL_PACKET): the packet number,
   the consoles not yet heard from, the checksum and the 24-byte payload. */
void sySerialPrepareSendPacket(void *payload)
{
	volatile SERIAL_STATE *state;
	Uint32				   i;
	Uint32				   sum;

	state = SERIAL_RUNTIME;
	{
		/* volatile: the serial interrupt sends from this buffer. The bytes of
		   SERIAL_PACKET, written through a byte pointer: as the struct's
		   volatile members, each store would read its word first. */
		volatile Uint8 *packet;
		Uint8			curMask;
		Uint8			receivedMask;

		packet = (volatile Uint8 *)state->sendBuf[0];
		packet[offsetof(SERIAL_PACKET, packetNum)] = state->packetNum;
		curMask = state->curMask;
		receivedMask = state->receivedMask;
		sum = 0;
		*(volatile Uint16 *)(packet + offsetof(SERIAL_PACKET, checksum)) = sum;
		packet[offsetof(SERIAL_PACKET, missingMask)] = receivedMask ^ curMask;
		syDMAXCopy(payload, (void *)(packet + offsetof(SERIAL_PACKET, payload)),
			DMA_COPY_WORDS(SERIAL_PAYLOAD_BYTES / 4), REG_DMA3);
	}
	{
		Uint16 *halfword;
		Uint32	value;

		halfword = state->sendBuf[0];
		for(i = 0; i < SERIAL_PACKET_HALFWORDS; i++) {
			value = *halfword;
			halfword++;
			sum += value;
		}
	}
	{
		Uint32		   checksum;
		SERIAL_PACKET *packet;

		checksum = ~sum;
		packet = (SERIAL_PACKET *)state->sendBuf[0];
		packet->checksum = checksum;
	}

	/* Stop timer 3, rewind the send position and start the timer again */
	if(state->mode != 0)
		REG_TM3CNT_H = 0;
	state->sendIdx = -1;
	if(state->mode != 0 && state->transferEnabled != 0)
		REG_TM3CNT_H = TIMER_INTR_ENABLE | TIMER_ENABLE;
}

/* Swaps in the packets the interrupt handler finished, then copies the payload
   of each one whose checksum is good to `payload` (24 bytes per console), and
   records the consoles heard from in currentMask and receivedMask. */
Uint8 sySerialCollectReceivedPayloads(void *payload)
{
	Uint8	flg[8];
	Sint32	i;
	Uint32	k;
	Uint16 *packet;
	Uint16 *swap;
	Sint32	sum;

	*(Uint32 *)&flg[4] = 0;
	/* Swap completed buffers with interrupts masked, then validate outside
	   it. */
	syIOWrite16(0, &REG_IME);
	for(i = 0; i <= 3; i++) {
		swap = SerialStateWork.pendingBuf[i];
		SerialStateWork.pendingBuf[i] = SerialStateWork.readyBuf[i];
		SerialStateWork.readyBuf[i] = swap;
	}
	*(Uint32 *)flg = *(Uint32 *)SerialStateWork.channelFlg;
	*(Uint32 *)SerialStateWork.channelFlg = 0;
	syIOWrite16(1, &REG_IME);
	SerialStateWork.curMask = 0;
	for(i = 0; i <= 1; i++) {
		packet = SerialStateWork.pendingBuf[i];
		sum = 0;
		for(k = 0; k < SERIAL_PACKET_HALFWORDS; k++)
			sum += packet[k];
		/* A good packet's halfwords, its complemented checksum included, sum to
		   0xffff. */
		if(flg[i] == SERIAL_CHANNEL_PACKET_COMPLETE && (Sint16)sum == -1) {
			syDMAXCopy(SerialStateWork.pendingBuf[i] + 2,
				(Uint8 *)payload + i * SERIAL_PAYLOAD_BYTES,
				DMA_COPY_WORDS(SERIAL_PAYLOAD_BYTES / 4), REG_DMA3);
			SerialStateWork.curMask |= 1 << i;
		}
		/* Complementing a halfword spoils the sum, so a buffer that comes round
		   again unchanged is never taken twice */
		if((Sint16)sum == -1)
			SerialStateWork.pendingBuf[i][1] =
				~SerialStateWork.pendingBuf[i][1];
	}
	SerialStateWork.receivedMask |= SerialStateWork.curMask;
	return SerialStateWork.curMask;
}

/* The serial and timer 3 interrupt, installed by sySerialInit: each one moves a
   halfword. */
void sySerialHandleTransferInterrupt(void)
{
	Uint16			 recv[4];
	volatile Uint32 *sioControl;
	Sint32			 channel;

	/* The four consoles' halfwords, SIOMULTI0 to 3. */
	/* UB: the Uint16 array is written through one doubleword store */
#if FIX_TYPE_PUNNING
	{
		volatile Uint16 *multi = (volatile Uint16 *)REG_SIODATA32;

		recv[0] = multi[0];
		recv[1] = multi[1];
		recv[2] = multi[2];
		recv[3] = multi[3];
	}
#else
	*(Uint64 *)recv = *(volatile Uint64 *)REG_SIODATA32;
#endif
	sioControl = &REG_SIOCNT;
	/* SIOCNT's error bit (SIO_ERROR): a packet heard while it is set is refused
	   (SERIAL_CHANNEL_ERROR, below) */
	SerialStateWork.transferError = SIO_ERROR_OF(*sioControl);

	if(SerialStateWork.sendIdx == -1) {
		Uint32	idleWord = SERIAL_IDLE_HALFWORD;
		Uint16 *swap;

		((volatile Uint16 *)sioControl)[1] = idleWord;
		swap = SerialStateWork.sendBuf[1];
		SerialStateWork.sendBuf[1] = SerialStateWork.sendBuf[0];
		SerialStateWork.sendBuf[0] = swap;
	} else if(SerialStateWork.sendIdx >= 0) {
		((volatile Uint16 *)sioControl)[1] =
			SerialStateWork.sendBuf[1][SerialStateWork.sendIdx];
	}
	if(SerialStateWork.sendIdx <= SERIAL_PACKET_HALFWORDS)
		SerialStateWork.sendIdx++;

	for(channel = 0; channel <= 1; channel++) {
		Sint32	receiveIdx;
		Uint16 *incoming;

		/* Hand complete packets to the tick code without overwriting its
		   buffers. */
		if(recv[channel] == SERIAL_IDLE_HALFWORD &&
			SerialStateWork.receiveIdx[channel] >= SERIAL_PACKET_HALFWORDS) {
			SerialStateWork.receiveIdx[channel] = -1;
		} else {
			receiveIdx = SerialStateWork.receiveIdx[channel];
			incoming = SerialStateWork.incomingBuf[channel];
			incoming[receiveIdx] = recv[channel];
			if(receiveIdx == SERIAL_PACKET_HALFWORDS - 1) {
				Uint16 *swap = SerialStateWork.readyBuf[channel];

				SerialStateWork.readyBuf[channel] = incoming;
				SerialStateWork.incomingBuf[channel] = swap;
				SerialStateWork.channelFlg[channel] |=
					SERIAL_CHANNEL_PACKET_COMPLETE;
			}
		}
		if(SerialStateWork.transferError != 0)
			SerialStateWork.channelFlg[channel] |= SERIAL_CHANNEL_ERROR;
		if(SerialStateWork.receiveIdx[channel] <= SERIAL_PACKET_HALFWORDS)
			SerialStateWork.receiveIdx[channel]++;
	}

	/* The console that clocks the transfers starts the next one, and restarts
	   timer 3 so its count reloads */
	if(SerialStateWork.mode == SERIAL_MODE_CLOCKING) {
		REG_TM3CNT_H = 0;
		REG_SIOCNT16 |= SIO_START;
		REG_TM3CNT_H = TIMER_ENABLE | TIMER_INTR_ENABLE;
	}
}

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

/* Starts sending `size` bytes from `src` to the partner in blocks of
   SERIAL_BLOCK_BYTES: sets the send source, size and block number that
   sySerialStepBlockTransfer works from. */
Sint32 sySerialStartSend(void *src, Sint32 size)
{
	SERIAL_TRANSFER_STATE *state = &SerialTransfer;
	Uint32				   savedIme;

	if(SerialSendSource != 0)
		return -1;
	IRQ_LOCK(savedIme);
	state->stat = SERIAL_STATUS_RUNNING;
	SerialSendSource = (Sint32)src;
	SerialSendSize = size;
	SerialBlockNumber = 0;
	state->active = 1;
	IRQ_UNLOCK(savedIme);
	return 0;
}

/* Starts receiving the partner's blocks into `dst`: sets the receive
   destination and clears the received size. */
Sint32 sySerialStartReceive(void *dst)
{
	SERIAL_TRANSFER_STATE *state = &SerialTransfer;
	Uint32				   savedIme;

	if(SerialReceiveDestination != 0)
		return -1;
	IRQ_LOCK(savedIme);
	state->stat = SERIAL_STATUS_RUNNING | 1;
	SerialReceiveDestination = (Sint32)dst;
	SerialReceivedSize = 0;
	state->active = 1;
	SerialBlockNumber = 0;
	IRQ_UNLOCK(savedIme);
	return 0;
}

/* Waits a tick at a time until the queued send is done, giving up after
   SERIAL_TRANSFER_TIMEOUT_TICKS ticks. */
void sySerialWaitForSend(void)
{
	Uint32 cnt = 0;

	if(SerialSendSource != 0) {
		for(; cnt < SERIAL_TRANSFER_TIMEOUT_TICKS && SerialSendSource != 0;
			cnt++) {
			sySynch(1);
		}
	}
}

/* Waits a tick at a time until the queued receive is done, giving up after
   SERIAL_TRANSFER_TIMEOUT_TICKS ticks. */
void sySerialWaitForReceive(void)
{
	Uint32 cnt = 0;

	if(SerialReceiveDestination != 0) {
		do {
			sySynch(1);
			cnt++;
		} while(cnt < SERIAL_TRANSFER_TIMEOUT_TICKS &&
			SerialReceiveDestination != 0);
	}
}

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

/* Clears the serial transfer for a link battle with interrupts off: nothing to
   send or receive, no flags, and status SERIAL_STATUS_RUNNING. */
void syLinkBattleResetTransferState(void)
{
	SERIAL_TRANSFER_STATE *state;
	Uint32				   saved;

	state = &SerialTransfer;
	IRQ_LOCK(saved);
	state->stat = SERIAL_STATUS_RUNNING;
	state->peerFlg = 0;
	SerialSendSource = 0;
	state->flg = 0;
	SerialSendSize = 0;
	SerialReceiveDestination = 0;
	SerialReceivedSize = 0;
	IRQ_UNLOCK(saved);
}

/* Runs each VBlank after the exchange, called by syVBlankHandler: moves the
   bulk transfer on by one SERIAL_BLOCK_BYTES block each way. */
void sySerialStepBlockTransfer(void)
{
	SERIAL_TRANSFER_STATE *peer;
	Uint32				   id;
	SERIAL_TRANSFER_STATE *own;
	Uint32				   gap;
	/* FAKEMATCH(temporary): the 1 kept in a halfword record. */
	SERIAL_HALFWORD one;

	/* The partner's payload: of multiplayer ids 0 and 1, the one not ours. */
	id = ~(SIO_ID_OF(REG_SIOCNT)) & 1;
	one.value = 1;
	own = &SerialTransfer;
	peer = &SerialPeerPayloads[id];
	/* Both consoles must have been heard from this tick */
	if((LinkStatusWork & LINK_STATUS_HEARD_BOTH) != LINK_STATUS_HEARD_BOTH)
		return;
	/* Receiving advances only when the peer presents the expected block number.
	   peerFlg is the sender's block kind, an SERIAL_SEND_STATE. */
	if(SerialReceiveDestination != 0) {
		if(own->flg == SERIAL_RECEIVE_ACTIVE &&
			(Uint8)(peer->peerFlg - SERIAL_SEND_MORE) <=
				SERIAL_SEND_LAST - SERIAL_SEND_MORE) {
			if(peer->active == (SerialBlockNumber & SERIAL_BLOCK_NUMBER_MASK)) {
				own->active = 0;
				switch(peer->peerFlg) {
				case SERIAL_SEND_MORE:
					/* A block with more to come: store it and move on */
					syDMAXCopy(peer->payload, (void *)SerialReceiveDestination,
						DMA_COPY_WORDS(SERIAL_BLOCK_BYTES / 4), REG_DMA3);
					SerialReceiveDestination += SERIAL_BLOCK_BYTES;
					SerialReceivedSize += SERIAL_BLOCK_BYTES;
					own->stat = (own->stat + 1) | SERIAL_STATUS_RUNNING;
					break;
				case SERIAL_SEND_LAST:
					/* The last block: store it and report the receive
					   finished */
					syDMAXCopy(peer->payload, (void *)SerialReceiveDestination,
						DMA_COPY_WORDS(SERIAL_BLOCK_BYTES / 4), REG_DMA3);
					SerialReceivedSize += SERIAL_BLOCK_BYTES;
					own->flg = SERIAL_RECEIVE_DONE;
					own->stat = 0;
					own->active = one.value;
					break;
				}
				SerialBlockNumber =
					(SerialBlockNumber + 1) & SERIAL_BLOCK_NUMBER_MASK;
			} else if(SerialBlockNumber & SERIAL_BLOCK_REWIND) {
				/* Out of step and already asked: bit 7 of active is the request
				   to rewind to the block number in its low bits */
				if(own->active & SERIAL_BLOCK_REWIND) {
					own->active = one.value;
				} else if(own->active == 1) {
					own->active = 0;
					SerialBlockNumber &= SERIAL_BLOCK_NUMBER_MASK;
				}
			} else {
				own->active = SerialBlockNumber | SERIAL_BLOCK_REWIND;
				SerialBlockNumber |= SERIAL_BLOCK_REWIND;
			}
		} else {
			own->active = 0;
		}
	}
	/* A peer behind our block number requests a rewind before the next
	   block. */
	if(SerialSendSource != 0) {
		if(peer->flg == SERIAL_RECEIVE_ACTIVE) {
			if(peer->active & SERIAL_BLOCK_REWIND) {
				gap = (SerialBlockNumber - peer->active) &
					SERIAL_BLOCK_NUMBER_MASK;
				SerialSendSource -= gap * SERIAL_BLOCK_BYTES;
				SerialSendSize += gap * SERIAL_BLOCK_BYTES;
				SerialBlockNumber -= gap;
				SerialBlockNumber &= SERIAL_BLOCK_NUMBER_MASK;
			}
			/* Send the next block, marked as the last when it empties the
			   size */
			if(SerialSendSize != 0) {
				syDMAXCopy((void *)SerialSendSource, own->payload,
					DMA_COPY_WORDS(SERIAL_BLOCK_BYTES / 4), REG_DMA3);
				SerialSendSize -= SERIAL_BLOCK_BYTES;
				if(SerialSendSize != 0) {
					own->peerFlg = SERIAL_SEND_MORE;
					own->active = SerialBlockNumber & SERIAL_BLOCK_NUMBER_MASK;
					SerialSendSource += SERIAL_BLOCK_BYTES;
				} else {
					own->peerFlg = SERIAL_SEND_LAST;
					own->active = SerialBlockNumber & SERIAL_BLOCK_NUMBER_MASK;
					SerialSendSource += SERIAL_BLOCK_BYTES;
				}
				SerialBlockNumber =
					(SerialBlockNumber + 1) & SERIAL_BLOCK_NUMBER_MASK;
			}
		}
		/* The partner has stored the last block: the send is finished */
		if(own->peerFlg == SERIAL_SEND_LAST &&
			peer->flg == SERIAL_RECEIVE_DONE) {
			SerialSendSource = 0;
			own->peerFlg = SERIAL_SEND_IDLE;
			own->active = 1;
		}
	}
	/* flags tells the partner this side is receiving or has received the last
	   block; the receive ends once the partner has seen that */
	if(own->flg == SERIAL_RECEIVE_DONE) {
		if(peer->peerFlg != SERIAL_SEND_LAST) {
			SerialReceiveDestination = 0;
			own->flg = SERIAL_RECEIVE_IDLE;
		}
	} else {
		own->flg = SERIAL_RECEIVE_IDLE;
		if(SerialReceiveDestination != 0)
			own->flg = SERIAL_RECEIVE_ACTIVE;
	}
}

extern Uint8 MapGridMetatileBuffer[];

/* An empty function the link test calls to waste time. */
void sySerialTestDelay(void)
{
}

/* The link test's block: LINK_TEST_TILES tiles' worth of bytes, shown from tile
   LINK_TEST_TILE of character block 0. */
#define LINK_TEST_BYTES			640
#define LINK_TEST_TILES			(LINK_TEST_BYTES / TILE_SIZE_4BPP)
#define LINK_TEST_TILE			(2 * BG_SCREEN_SIZE / TILE_SIZE_4BPP)
/* What B sends: the cartridge's bytes from 4 KB in, where A sends its first. */
#define LINK_TEST_SECOND_SOURCE (Rom_Start + 4096)

/* A link cable test left in the game: it shows a row of tiles, receives blocks
   into the map grid metatile buffer and shows them as tiles, and sends a part
   of the ROM when A or B is held. */
void sySerialTestRun(void)
{
	Uint16 *tile;
	Sint32	column;
	Sint32	delay;

	sySndPlayStub(MUS_CHAMPA);
	sySerialInit();
	/* Row 16, columns 0 to 19: the received tiles in WINDOW_PALETTE. */
	tile = (Uint16 *)BG_SCREEN_BLOCK(DEBUG_TEXT_SCREEN_BLOCK) +
		16 * BG_SCREEN_WIDTH;
	for(column = 0; column < LINK_TEST_TILES; column++)
		tile[column] =
			BG_TILE_PALETTE(WINDOW_PALETTE) + LINK_TEST_TILE + column;
	CPU_FILL32(0, MapGridMetatileBuffer, 1024);
	sySerialWaitForStatMask(LINK_STATUS_HEARD_BOTH);
restart:
	sySerialStartReceive(MapGridMetatileBuffer);
	for(;;) {
		if(PerWork.data & KEY_A)
			sySerialStartSend((void *)Cart_Start, LINK_TEST_BYTES);
		if(PerWork.data & KEY_B)
			sySerialStartSend((void *)LINK_TEST_SECOND_SOURCE, LINK_TEST_BYTES);
		if(PerWork.data & KEY_START) {
			for(delay = 0; delay < 10000; delay++)
				sySerialTestDelay();
		}
		if(SerialReceiveDestination == 0) {
			syDMAXCopy(MapGridMetatileBuffer,
				BG_CHAR_BLOCK(0) + LINK_TEST_TILE * TILE_SIZE_4BPP,
				DMA_COPY_WORDS(LINK_TEST_BYTES / 4), REG_DMA3);
			goto restart;
		}
		sySynch(1);
	}
}

#undef LINK_TEST_BYTES
#undef LINK_TEST_TILES
#undef LINK_TEST_TILE
#undef LINK_TEST_SECOND_SOURCE

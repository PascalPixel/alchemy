@ Copies of the two command-byte setters in the leftover block of
@ LEFTOVER_HANDLE_NOTE.S; no bl in code reaches them (the halfword pair at
@ 0x081cb41a that decodes as a bl to the second lies in a byte table) and no
@ literal pool names them.
@ They keep their return address in ip (mov ip, lr ... bx ip), take the byte
@ from the reader's r3 result (LEFTOVER_COMMAND_BYTE_READER.S, 0x081c03f4) and
@ call the copy at 0x081c03d8 of the TBS routine at 0x080fa1ac when it is
@ zero. These 40 bytes equal TBS SOUND/DRIVER/COMMAND_BYTE_SETTERS.S
@ (0x080fa1d4) byte for byte. The zero halfword after each setter is
@ alignment and carries no credit.
@ credit: library — TBS/TLA common sound command-byte reader and setters (setters)
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x081c0400
	.global MusicTrack_SetLfoSpeedFromCommandLeftover
	.thumb_func
MusicTrack_SetLfoSpeedFromCommandLeftover:
	mov	ip, lr
	bl	Func_081c03f4
	strb	r3, [r1, #25]
	cmp	r3, #0
	bne.n	.L_081c0410
	bl	Func_081c03d8
.L_081c0410:
	bx	ip
AlchemyUncredited_081c0412:
	.2byte	0x0000
	.global MusicTrack_SetModulationFromCommandLeftover
	.thumb_func
MusicTrack_SetModulationFromCommandLeftover:
AlchemyUncreditedEnd_081c0412:
	mov	ip, lr
	bl	Func_081c03f4
	strb	r3, [r1, #23]
	cmp	r3, #0
	bne.n	.L_081c0424
	bl	Func_081c03d8
.L_081c0424:
	bx	ip
AlchemyUncredited_081c0426:
	.2byte	0x0000
AlchemyUncreditedEnd_081c0426:

@ Command-byte setters that save the return address in ip (mov ip, lr ...
@ bx ip) and store the reader's r3 result, a return register no C call has.
@ These 40 bytes equal TBS SOUND/DRIVER/COMMAND_BYTE_SETTERS.S (0x080fa1d4)
@ byte for byte.
@ The zero halfwords of alignment at 0x081c1e82 and 0x081c1e96 never run and
@ carry no credit.
@ credit: library — TBS/TLA common sound command-byte reader and setters (setters)
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x081c1e70
	.global MusicTrack_SetLfoSpeedFromCommand
	.thumb_func
MusicTrack_SetLfoSpeedFromCommand:
	mov	ip, lr
	bl	Func_081c1e64
	strb	r3, [r1, #25]
	cmp	r3, #0
	bne.n	.L_081c1e80
	bl	Func_081c1e48
.L_081c1e80:
	bx	ip
AlchemyUncredited_081c1e82:
	.2byte	0x0000
	.global MusicTrack_SetModulationFromCommand
	.thumb_func
MusicTrack_SetModulationFromCommand:
AlchemyUncreditedEnd_081c1e82:
	mov	ip, lr
	bl	Func_081c1e64
	strb	r3, [r1, #23]
	cmp	r3, #0
	bne.n	.L_081c1e94
	bl	Func_081c1e48
.L_081c1e94:
	bx	ip
AlchemyUncredited_081c1e96:
	.2byte	0x0000
AlchemyUncreditedEnd_081c1e96:

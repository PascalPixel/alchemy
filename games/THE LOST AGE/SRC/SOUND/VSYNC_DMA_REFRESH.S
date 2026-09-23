@ Sound driver VSync step that re-arms the DMA channels feeding the direct
@ sound FIFOs. It loads the SoundInfo pointer holder and the 'Smsh' ID from
@ the literal block of the frame entry that follows (0x0800064c), a literal
@ shared across functions that per-function compiler pools never produce,
@ and branches on the flags of subs (across a strb) and of lsls with no
@ compare. TLA revision of the first routine of TBS SOUND/DRIVER/UPDATE.S
@ (0x080f9c44), which TLA's UPDATE.S still carries unchanged.
@ credit: handwritten — sound driver VSync DMA refresh (TLA revision)
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x080005e8
	.global SoundDriver_VSyncRefresh
	.thumb_func
SoundDriver_VSyncRefresh:
	ldr	r0, [pc, #100]
	ldr	r0, [r0, #0]
	ldr	r2, [pc, #92]
	ldr	r3, [r0, #0]
	subs	r3, r3, r2
	cmp	r3, #1
	bhi.n	.L_08000626
	ldrb	r1, [r0, #4]
	subs	r1, #1
	strb	r1, [r0, #4]
	bgt.n	.L_08000626
	ldrb	r1, [r0, #11]
	strb	r1, [r0, #4]
	ldr	r2, [pc, #36]
	ldr	r1, [r2, #8]
	ldr	r0, [pc, #36]
	lsls	r1, r1, #7
	bcc.n	.L_0800060e
	str	r0, [r2, #8]
.L_0800060e:
	ldr	r1, [r2, #20]
	lsls	r1, r1, #7
	bcc.n	.L_08000616
	str	r0, [r2, #20]
.L_08000616:
	movs	r1, #4
	lsls	r1, r1, #8
	strh	r1, [r2, #10]
	strh	r1, [r2, #22]
	movs	r1, #182
	lsls	r1, r1, #8
	strh	r1, [r2, #10]
	strh	r1, [r2, #22]
.L_08000626:
	bx	lr
	.4byte	0x040000bc
	.4byte	0x84400004

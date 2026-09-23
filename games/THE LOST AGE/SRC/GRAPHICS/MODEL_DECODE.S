@ Model decoder, the first member of a 564-byte unit (size and source {0x234,
@ 0x08118160} in a literal pool at 0x08126520) that is copied and run as a
@ whole. Behind a 4-byte Thumb entry (bx pc) the ARM decoder rebuilds the
@ dispatcher's jump table for the address it runs at from its own position
@ (add r3, pc, #240; add r4, pc, #200) and calls the dispatcher with bl,
@ keeping the decoder state in r2, r3, r5 and fp across those calls. Neither
@ approved compiler relocates code at run time or passes state that way.
@ After that entry these 204 bytes equal TBS GRAPHICS/TILE/DECODE.S
@ (0x080b5138) byte for byte. The unit's data words (a 0x60606060 bias and
@ the table's link address at 0x08118230, the runtime table and its eight
@ case addresses at 0x08118244) stay outside the credited modules.
@ Its 4-byte Thumb entry, which is TLA's own, carries no credit.
@ credit: library — TBS/TLA common ARM graphics and runtime kernel bank, model decoder
.syntax unified
	.set Model_DispatchBitCommand, 0x08118238
	.thumb
	.set AlchemyLoadAddress, 0x08118160
	.global Model_Decode
	.thumb_func
Model_Decode:
AlchemyUncredited_08118160:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08118160:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	ldr	r2, [pc, #196]
	add	r3, pc, #240
	sub	r2, r3, r2
	add	r4, pc, #200
	mov	r5, #8
.L_0811817c:
	ldr	r6, [r3], #4
	add	r6, r6, r2
	str	r6, [r4], #4
	subs	r5, r5, #1
	bne	.L_0811817c
	mov	fp, #7
	ldrh	r3, [r0], #2
	mov	r2, #0
	mov	r5, #0
	mov	r9, #15
	ldr	sl, [pc, #132]
.L_081181a8:
	mov	r8, #8
.L_081181ac:
	mov	r7, #32
.L_081181b0:
	bl	Model_DispatchBitCommand
	lsr	r6, r5, #25
	bl	Model_DispatchBitCommand
	orr	r6, r6, r5, lsr #17
	bl	Model_DispatchBitCommand
	orr	r6, r6, r5, lsr #9
	bl	Model_DispatchBitCommand
	orr	r6, r6, r5, lsr #1
	add	r6, r6, sl
	str	r6, [r1], #4
	bl	Model_DispatchBitCommand
	lsr	r6, r5, #25
	bl	Model_DispatchBitCommand
	orr	r6, r6, r5, lsr #17
	bl	Model_DispatchBitCommand
	orr	r6, r6, r5, lsr #9
	bl	Model_DispatchBitCommand
	orr	r6, r6, r5, lsr #1
	add	r6, r6, sl
	str	r6, [r1], #60
	subs	r7, r7, #1
	bne	.L_081181b0
	sub	r1, r1, #0x800
	add	r1, r1, #8
	subs	r8, r8, #1
	bne	.L_081181ac
	sub	r1, r1, #64
	add	r1, r1, #0x800
	subs	r9, r9, #1
	bne	.L_081181a8
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr

.syntax unified
	.thumb
	.global Model_DecodeCode
Model_DecodeCode:
	.global Model_Decode
	.thumb_func
Model_Decode:
	bx	pc
AlchemyUncredited_08118162:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08118162:
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
	.4byte 0x60606060, Model_BitCommandTable

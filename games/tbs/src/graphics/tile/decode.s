@ C2: r11/fp is the mask 7 passed through the command linkage; the opening
@ str loop rewrites the command table used by ldr pc in its dispatcher.
.syntax unified
.text
	.arm
	.global BitDecoder_DecodeImage
BitDecoder_DecodeImage:
	push	{r5, r6, r7, r8, r9, sl, fp, lr}
	ldr	r2, [pc, #196]
	add	r3, pc, #240
	sub	r2, r3, r2
	add	r4, pc, #200
	mov	r5, #8
.L_080b5150:
	ldr	r6, [r3], #4
	add	r6, r6, r2
	str	r6, [r4], #4
	subs	r5, r5, #1
	bne .L_080b5150
	mov	fp, #7
	ldrh	r3, [r0], #2
	mov	r2, #0
	mov	r5, #0
	mov	r9, #15
	ldr	sl, [pc, #132]
.L_080b517c:
	mov	r8, #8
.L_080b5180:
	mov	r7, #32
.L_080b5184:
	bl Model_DispatchBitCommand
	lsr	r6, r5, #25
	bl Model_DispatchBitCommand
	orr	r6, r6, r5, lsr #17
	bl Model_DispatchBitCommand
	orr	r6, r6, r5, lsr #9
	bl Model_DispatchBitCommand
	orr	r6, r6, r5, lsr #1
	add	r6, r6, sl
	str	r6, [r1], #4
	bl Model_DispatchBitCommand
	lsr	r6, r5, #25
	bl Model_DispatchBitCommand
	orr	r6, r6, r5, lsr #17
	bl Model_DispatchBitCommand
	orr	r6, r6, r5, lsr #9
	bl Model_DispatchBitCommand
	orr	r6, r6, r5, lsr #1
	add	r6, r6, sl
	str	r6, [r1], #60
	subs	r7, r7, #1
	bne .L_080b5184
	sub	r1, r1, #2048
	add	r1, r1, #8
	subs	r8, r8, #1
	bne .L_080b5180
	sub	r1, r1, #64
	add	r1, r1, #2048
	subs	r9, r9, #1
	bne .L_080b517c
	pop	{r5, r6, r7, r8, r9, sl, fp, lr}
	bx	lr

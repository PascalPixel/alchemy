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
@ A table of the handwritten routine beside it: it lives in the routine's text.
	.text
	.balign 4
@ BitDecoder_DecodeImage の定数
	.4byte 0x60606060, Model_DispatchHandlers
@ C2: Dispatcher enters Model_BitCommands' shared r0/r2/r3 decoder linkage;
@ bit_commands.s documents its unsaved r5 and ordinary fp mask.
@ In this file itself: it is a two-instruction fragment of that decoder, with
@ no prologue and no return of its own, and its tell is the computed pc into
@ code. and ip, r3, #7 / ldr pc, [pc, ip, lsl #2] is unconditional and has
@ no bounds test, where the compiler's casesi always emits cmp, ldrls pc and a
@ default branch; the table it indexes starts after the nop, in the next
@ object.
.syntax unified
.text
	.arm
	.global Model_DispatchBitCommand
Model_DispatchBitCommand:
	and	ip, r3, #7
	ldr	pc, [pc, ip, lsl #2]
AlchemyUncredited_080b5214:
	nop
AlchemyUncreditedEnd_080b5214:
@ A table of the handwritten routine beside it: it lives in the routine's text.
	.text
	.balign 4
@ IWRAM の写しで実行時に埋まる分岐先
	.space 32, 0
	.global Model_DispatchHandlers
Model_DispatchHandlers:
	.4byte Model_BitCommands, Model_AddShortDelta, Model_AddSignedDelta, Model_AddLongDelta
	.4byte Model_BitCommands, Model_SubtractShortDelta, Model_SetBitValue, Model_SubtractLongDelta
@ C2: entries share live r0/r2/r3 decoder state with their caller; delta
@ entries update unsaved r5 and use r11/fp as a data mask.
.syntax unified
.text
	.arm
	.global Model_BitCommands
	.global Model_AddShortDelta
	.global Model_SubtractShortDelta
	.global Model_AddLongDelta
	.global Model_SubtractLongDelta
	.global Model_AddSignedDelta
	.global Model_SetBitValue
Model_BitCommands:
	lsr	r3, r3, #2
	subs	r2, r2, #2
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
Model_AddShortDelta:
	and	ip, r3, #8
	lsrs	r3, r3, #4
	add	ip, ip, #8
	add	r5, r5, ip, lsl #22
	subs	r2, r2, #4
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
Model_SubtractShortDelta:
	and	ip, r3, #8
	lsrs	r3, r3, #4
	add	ip, ip, #8
	sub	r5, r5, ip, lsl #22
	subs	r2, r2, #4
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
Model_AddLongDelta:
	and	ip, fp, r3, lsr #3
	lsr	r3, r3, #6
	add	ip, ip, #3
	add	r5, r5, ip, lsl #25
	subs	r2, r2, #6
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
Model_SubtractLongDelta:
	and	ip, fp, r3, lsr #3
	lsr	r3, r3, #6
	add	ip, ip, #3
	sub	r5, r5, ip, lsl #25
	subs	r2, r2, #6
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
Model_AddSignedDelta:
	mov	ip, #15
	ands	ip, ip, r3, lsr #4
	add	ip, ip, #11
	rsbcs	ip, ip, #0
	add	r5, r5, ip, lsl #25
	lsr	r3, r3, #8
	subs	r2, r2, #8
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr
Model_SetBitValue:
	ror	r5, r3, #10
	and	r5, r5, #-33554432
	lsr	r3, r3, #10
	subs	r2, r2, #10
	movpl	pc, lr
	ldrh	ip, [r0], #2
	add	r2, r2, #16
	orr	r3, r3, ip, lsl r2
	mov	pc, lr

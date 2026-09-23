@ Compression format 0/1 decoder. Its loader (0x0801587c) allocates a
@ 0x2c8-byte heap block, has DMA3 copy exactly this module there (size and
@ source in its literal pool at 0x080158c4), calls the copy and frees it.
@ Behind a 4-byte Thumb entry (bx pc) the ARM body tests each code bit with
@ lsrs rN, rN, #1 and bcs or bcc, and reaches its refill subroutine through
@ a second entry that points lr back into its loop by hand
@ (sub lr, pc, #284). Branching on the carry out of a shift and rewriting lr
@ are beyond both approved compilers. After that entry its 708-byte body
@ equals TBS SYSTEM/RESOURCE/DECODE_01.S (0x08002544) byte for byte.
@ Its 4-byte Thumb entry, which is TLA's own, carries no credit.
@ credit: library — TBS/TLA common ARM graphics and runtime kernel bank, compression format 0/1 decoder
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x080128a8
	.global Resource_Decode01
	.thumb_func
Resource_Decode01:
AlchemyUncredited_080128a8:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_080128a8:
	push	{r5, r6, r7, r9, lr}
	ldrb	r2, [r0], #1
	cmp	r2, #0
	bne	.L_08012af0
	sub	r1, r1, #1
	add	r9, r1, #33
	ands	r2, r0, #1
	lsl	r4, r2, #3
	ldrbne	r2, [r0], #1
	subs	r4, r4, #16
	blmi	.L_08012ad0
	b	.L_080128f4
.L_080128dc:
	strb	r2, [r1, #1]!
	lsr	r2, r2, #8
	subs	r4, r4, #9
	ldrhmi	r3, [r0], #2
	addmi	r4, r4, #16
	orrmi	r2, r2, r3, lsl r4
.L_080128f4:
	lsrs	r2, r2, #1
	bcs	.L_080128dc
	lsrs	r2, r2, #1
	bcc	.L_080129b0
	lsrs	r2, r2, #1
	bcc	.L_08012974
	lsrs	r2, r2, #1
	bcc	.L_08012980
	lsrs	r2, r2, #1
	bcc	.L_08012990
	lsrs	r2, r2, #1
	bcs	.L_0801293c
	lsrs	r2, r2, #1
	bcc	.L_080129a0
	subs	r4, r4, #7
	mov	r5, #7
	bpl	.L_080129b8
	blmi	.L_08012acc
.L_0801293c:
	ands	r5, r2, #3
	lsr	r2, r2, #2
	beq	.L_08012958
	add	r5, r5, #7
	subs	r4, r4, #8
	bpl	.L_080129b8
	blmi	.L_08012acc
.L_08012958:
	ands	r5, r2, #127
	beq	.L_08012ae0
	add	r5, r5, #10
	lsr	r2, r2, #7
	subs	r4, r4, #15
	bpl	.L_080129b8
	blmi	.L_08012acc
.L_08012974:
	subs	r4, r4, #3
	mov	r5, #3
	b	.L_080129b8
.L_08012980:
	subs	r4, r4, #4
	mov	r5, #4
	bpl	.L_080129b8
	blmi	.L_08012acc
.L_08012990:
	subs	r4, r4, #5
	mov	r5, #5
	bpl	.L_080129b8
	blmi	.L_08012acc
.L_080129a0:
	subs	r4, r4, #7
	mov	r5, #6
	bpl	.L_080129b8
	blmi	.L_08012acc
.L_080129b0:
	subs	r4, r4, #2
	mov	r5, #2
.L_080129b8:
	lsrs	r2, r2, #1
	bcc	.L_08012a00
	and	r6, r2, #31
	subs	r4, r4, #6
	lsr	r2, r2, #5
.L_080129cc:
	ldrhmi	r3, [r0], #2
	addmi	r4, r4, #16
	orrmi	r2, r2, r3, lsl r4
	lsrs	r5, r5, #1
	ldrbcs	r7, [r1, -r6]
	strbcs	r7, [r1, #1]!
.L_080129e4:
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
	ldrb	r7, [r1, -r6]
	strb	r7, [r1, #1]!
	subs	r5, r5, #1
	bne	.L_080129e4
	b	.L_080128f4
.L_08012a00:
	sub	r6, r1, r9
	cmp	r6, #0x800
	bcc	.L_08012a24
	ror	r6, r2, #12
	lsr	r2, r2, #12
	lsr	r6, r6, #20
	add	r6, r6, #32
	subs	r4, r4, #13
	b	.L_080129cc
.L_08012a24:
	mov	r7, #11
	lsls	r6, r6, #22
	bcs	.L_08012aac
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012aac
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012aac
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012aac
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012aac
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012aac
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012aac
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012aac
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012aac
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012aac
	sub	r7, r7, #1
	lsls	r6, r6, #1
	bcs	.L_08012aac
	sub	r7, r7, #1
.L_08012aac:
	mov	r6, #1
	rsb	r6, r6, r6, lsl r7
	and	r6, r2, r6
	add	r6, r6, #32
	lsr	r2, r2, r7
	sub	r4, r4, #1
	subs	r4, r4, r7
	b	.L_080129cc
.L_08012acc:
	sub	lr, pc, #0x11c
.L_08012ad0:
	ldrh	r3, [r0], #2
	add	r4, r4, #16
	orr	r2, r2, r3, lsl r4
	bx	lr
.L_08012ae0:
	sub	r9, r9, #33
.L_08012ae4:
	sub	r0, r1, r9
	pop	{r5, r6, r7, r9, lr}
	bx	lr
.L_08012af0:
	cmp	r2, #1
	beq	.L_08012b00
	sub	r9, r1, #4
	b	.L_08012ae4
.L_08012b00:
	mov	r9, r1
.L_08012b04:
	ldrb	lr, [r0], #1
	mov	r3, #0x1000000
	orrs	lr, r3, lr, lsl #25
	bcs	.L_08012b28
.L_08012b14:
	ldrb	r3, [r0], #1
	strb	r3, [r1], #1
.L_08012b1c:
	lsls	lr, lr, #1
	bcc	.L_08012b14
	beq	.L_08012b04
.L_08012b28:
	ldrb	r3, [r0], #1
	ldrb	r4, [r0], #1
	and	r2, r3, #240
	orr	r2, r4, r2, lsl #4
	ands	r3, r3, #15
	beq	.L_08012b5c
.L_08012b40:
	ldrb	r4, [r1, -r2]
	strb	r4, [r1], #1
	subs	r3, r3, #1
	bne	.L_08012b40
	ldrb	r4, [r1, -r2]
	strb	r4, [r1], #1
	b	.L_08012b1c
.L_08012b5c:
	movs	r2, r2
	beq	.L_08012ae4
	ldrb	r3, [r0], #1
	add	r3, r3, #16
	b	.L_08012b40

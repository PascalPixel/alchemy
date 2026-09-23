@ Byte LZ decoder that the resource loader at 0x080053e8 copies into a heap
@ block, exactly these 600 bytes, calls there and then frees. Its flag byte
@ rides in lr with a sentinel bit, back-references jump into unrolled copy
@ runs with add pc, pc, ip, lsl #3, and it returns with pop {r0, r5, r6, r7,
@ ip}; bx ip, reloading its saved second argument as the result. GCC never
@ jumps into unrolled code, and with -mthumb-interwork it returns through lr.
@ Every TBS and TLA edition has these bytes.
@ The nops after its jumps (0x08001c70 and 0x08001d20), which the jumps
@ always skip, carry no credit.
@ credit: library — TBS/TLA common ARM byte-LZ decoder copied to the heap
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08001b70
	.global Resource_DecodeByteLz
Resource_DecodeByteLz:
	push	{r1, r5, r6, r7, lr}
.L_08001b74:
	ldrb	lr, [r0], #1
	lsls	lr, lr, #24
	bne	.L_08001c2c
	ands	r2, r0, #3
	ldmeq	r0!, {r4, r5}
	beq	.L_08001bac
	ldm	r0, {r4, r5, r6}
	lsl	r2, r2, #3
	rsb	r7, r2, #32
	lsr	r4, r4, r2
	orr	r4, r4, r5, lsl r7
	lsr	r5, r5, r2
	orr	r5, r5, r6, lsl r7
	add	r0, r0, #8
.L_08001bac:
	strb	r4, [r1], #1
	lsr	r4, r4, #8
	strb	r4, [r1], #1
	lsr	r4, r4, #8
	strb	r4, [r1], #1
	lsr	r4, r4, #8
	strb	r4, [r1], #1
	strb	r5, [r1], #1
	lsr	r5, r5, #8
	strb	r5, [r1], #1
	lsr	r5, r5, #8
	strb	r5, [r1], #1
	lsr	r5, r5, #8
	strb	r5, [r1], #1
	b	.L_08001b74
	ldrb	r3, [r0], #1
	strb	r3, [r1], #1
	ldrb	r2, [r0], #1
	strb	r2, [r1], #1
	ldrb	r3, [r0], #1
	strb	r3, [r1], #1
	ldrb	r2, [r0], #1
	strb	r2, [r1], #1
	ldrb	r3, [r0], #1
	strb	r3, [r1], #1
	ldrb	r2, [r0], #1
	strb	r2, [r1], #1
	ldrb	r3, [r0], #1
	strb	r3, [r1], #1
	ldrb	r2, [r0], #1
	strb	r2, [r1], #1
	b	.L_08001b74
.L_08001c2c:
	orr	lr, lr, #0x800000
	lsls	lr, lr, #1
	bcs	.L_08001c4c
.L_08001c38:
	ldrb	r3, [r0], #1
	strb	r3, [r1], #1
	lsls	lr, lr, #1
	bcc	.L_08001c38
	beq	.L_08001b74
.L_08001c4c:
	ldrb	ip, [r0], #1
	ldrb	r3, [r0], #1
	and	r2, ip, #240
	rsb	r3, r3, r1
	sub	r4, r3, r2, lsl #4
	ands	ip, ip, #15
	beq	.L_08001d04
	rsb	ip, ip, #15
	add	pc, pc, ip, lsl #3
AlchemyUncredited_08001c70:
	nop
AlchemyUncreditedEnd_08001c70:
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	ldrb	r3, [r4], #1
	strb	r3, [r1], #1
	lsls	lr, lr, #1
	bcc	.L_08001c38
	beq	.L_08001b74
	b	.L_08001c4c
.L_08001d04:
	cmp	r4, r1
	beq	.L_08001dbc
	ldrb	r3, [r0], #1
	add	r3, r3, #16
	and	ip, r3, #15
	rsb	ip, ip, #15
	add	pc, pc, ip, lsl #3
AlchemyUncredited_08001d20:
	nop
.L_08001d24:
AlchemyUncreditedEnd_08001d20:
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	ldrb	r2, [r4], #1
	strb	r2, [r1], #1
	subs	r3, r3, #16
	bge	.L_08001d24
	lsls	lr, lr, #1
	bcc	.L_08001c38
	beq	.L_08001b74
	b	.L_08001c4c
.L_08001dbc:
	pop	{r0, r5, r6, r7, ip}
	sub	r0, r1, r0
	bx	ip

@ Byte LZ decoder. Its resource loader (0x0801591c) allocates a 0x25c-byte
@ heap block, has DMA3 copy exactly these 604 bytes there, calls the copy and
@ frees it. Behind a 4-byte Thumb entry (bx pc) the ARM body keeps its flag
@ byte in lr with a sentinel bit, tests it with lsls/bcs/bcc, jumps into
@ unrolled copy runs with add pc, pc, ip, lsl #3 and returns with
@ pop {r0, r5, r6, r7, ip}; bx ip. GCC never jumps into unrolled code, and
@ with -mthumb-interwork it returns through lr. After that entry its
@ 600-byte body equals TBS SYSTEM/RESOURCE/DECODE_BYTE_LZ.S (0x08001b70) byte
@ for byte, as in every TBS and TLA edition.
@ Its 4-byte Thumb entry, which is TLA's own, carries no credit. The nops
@ after its jumps (0x0800175c and 0x0800180c), which the jumps always skip,
@ carry no credit.
@ credit: library — TBS/TLA common ARM byte-LZ decoder copied to the heap
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08001658
	.global Resource_DecodeByteLz
	.thumb_func
Resource_DecodeByteLz:
AlchemyUncredited_08001658:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08001658:
	push	{r1, r5, r6, r7, lr}
.L_08001660:
	ldrb	lr, [r0], #1
	lsls	lr, lr, #24
	bne	.L_08001718
	ands	r2, r0, #3
	ldmeq	r0!, {r4, r5}
	beq	.L_08001698
	ldm	r0, {r4, r5, r6}
	lsl	r2, r2, #3
	rsb	r7, r2, #32
	lsr	r4, r4, r2
	orr	r4, r4, r5, lsl r7
	lsr	r5, r5, r2
	orr	r5, r5, r6, lsl r7
	add	r0, r0, #8
.L_08001698:
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
	b	.L_08001660
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
	b	.L_08001660
.L_08001718:
	orr	lr, lr, #0x800000
	lsls	lr, lr, #1
	bcs	.L_08001738
.L_08001724:
	ldrb	r3, [r0], #1
	strb	r3, [r1], #1
	lsls	lr, lr, #1
	bcc	.L_08001724
	beq	.L_08001660
.L_08001738:
	ldrb	ip, [r0], #1
	ldrb	r3, [r0], #1
	and	r2, ip, #240
	rsb	r3, r3, r1
	sub	r4, r3, r2, lsl #4
	ands	ip, ip, #15
	beq	.L_080017f0
	rsb	ip, ip, #15
	add	pc, pc, ip, lsl #3
AlchemyUncredited_0800175c:
	nop
AlchemyUncreditedEnd_0800175c:
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
	bcc	.L_08001724
	beq	.L_08001660
	b	.L_08001738
.L_080017f0:
	cmp	r4, r1
	beq	.L_080018a8
	ldrb	r3, [r0], #1
	add	r3, r3, #16
	and	ip, r3, #15
	rsb	ip, ip, #15
	add	pc, pc, ip, lsl #3
AlchemyUncredited_0800180c:
	nop
.L_08001810:
AlchemyUncreditedEnd_0800180c:
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
	bge	.L_08001810
	lsls	lr, lr, #1
	bcc	.L_08001724
	beq	.L_08001660
	b	.L_08001738
.L_080018a8:
	pop	{r0, r5, r6, r7, ip}
	sub	r0, r1, r0
	bx	ip

@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800be70
	.thumb_func
Func_0800be70:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldrb	r3, [r0, #28]
	ldr	r2, [pc, #116]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	ldr	r2, [pc, #112]
	adds	r5, r3, r2
	adds	r3, r0, #0
	adds	r3, #32
	adds	r0, #33
	ldrb	r2, [r3, #0]
	ldrb	r3, [r0, #0]
	muls	r3, r2
	cmp	r3, #0
	bge.n	.L0
	adds	r3, #63
.L0:
	asrs	r6, r3, #6
	movs	r4, #0
	cmp	r4, r6
	bcs.n	.L1
	ldr	r3, [pc, #88]
	movs	r0, #255
	lsls	r0, r0, #8
	movs	r2, #63
	mov	r8, r3
	mov	lr, r0
	mov	ip, r2
	movs	r7, #62
.L5:
	adds	r3, r1, #0
	subs	r3, #64
	cmp	r3, #63
	bhi.n	.L2
	lsls	r3, r4, #4
	mov	r0, ip
	adds	r3, r1, r3
	ands	r3, r0
	mov	r0, r8
	ldrb	r2, [r0, r3]
	adds	r3, r2, #0
	ands	r3, r7
	adds	r0, r5, r3
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	ldrb	r3, [r0, #0]
	b.n	.L4
.L3:
	ldrh	r2, [r0, #0]
	mov	r3, lr
	ands	r3, r2
.L4:
	strh	r3, [r0, #0]
.L2:
	adds	r4, #1
	adds	r5, #64
	adds	r1, #1
	cmp	r4, r6
	bcc.n	.L5
.L1:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001b10
	.4byte 0x06010000
	.4byte 0x0801314c

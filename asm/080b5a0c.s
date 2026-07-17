@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b5a0c
	.thumb_func
Func_080b5a0c:
	push	{r5, r6, r7, lr}
	sub	sp, #28
	ldr	r3, [pc, #92]
	mov	r7, sp
	adds	r0, r7, #0
	ldr	r5, [r3, #0]
	bl	Func_080b6a60
	adds	r6, r0, #0
	cmp	r6, #0
	ble.n	.L0
	adds	r2, r5, #0
	adds	r0, r7, #0
	adds	r2, #88
	movs	r4, #0
	adds	r1, r6, #0
.L1:
	ldrh	r3, [r4, r0]
	subs	r1, #1
	strh	r3, [r2, #0]
	adds	r4, #2
	adds	r2, #2
	cmp	r1, #0
	bne.n	.L1
.L0:
	lsls	r3, r6, #1
	ldr	r2, [pc, #44]
	adds	r3, #88
	strh	r2, [r5, r3]
	adds	r0, r7, #0
	bl	Func_080b6ae0
	adds	r3, r5, #0
	adds	r3, #66
	ldrb	r3, [r3, #0]
	adds	r6, r0, #0
	cmp	r3, #0
	blt.n	.L2
	cmp	r3, #1
	bgt.n	.L2
	movs	r1, #0
	cmp	r1, r6
	bge.n	.L3
	adds	r3, r5, #2
	adds	r2, r5, #0
	mov	ip, r3
	adds	r0, r7, #0
	adds	r2, #102
	movs	r4, #0
	b.n	.L4
	.4byte 0x000000ff
	.4byte 0x03001e74
.L4:
	ldrh	r3, [r4, r0]
	adds	r1, #1
	strh	r3, [r2, #0]
	adds	r4, #2
	adds	r2, #2
	cmp	r1, r6
	blt.n	.L4
	b.n	.L5
.L2:
	cmp	r6, #0
	ble.n	.L3
	lsrs	r3, r6, #31
	adds	r3, r6, r3
	adds	r5, #2
	asrs	r3, r3, #1
	ldr	r4, [pc, #60]
	mov	ip, r5
	mov	lr, r3
	adds	r0, r7, #0
	adds	r1, r6, #0
.L6:
	ldrb	r3, [r4, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	add	r3, lr
	ldrh	r2, [r0, #0]
	lsls	r3, r3, #1
	adds	r3, #100
	subs	r1, #1
	adds	r4, #1
	adds	r0, #2
	strh	r2, [r5, r3]
	cmp	r1, #0
	bne.n	.L6
	b.n	.L5
.L3:
	adds	r5, #2
	mov	ip, r5
.L5:
	lsls	r3, r6, #1
	ldr	r2, [pc, #12]
	adds	r3, #100
	mov	r1, ip
	strh	r2, [r1, r3]
	add	sp, #28
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x000000ff
	.4byte 0x080c2a10

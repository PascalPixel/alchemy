@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08005d10
	.thumb_func
Func_08005d10:
	push	{r5, r6, r7, lr}
	ldr	r6, [pc, #68]
	ldrh	r3, [r6, #0]
	sub	sp, #4
	adds	r7, r3, #0
	strh	r6, [r6, #0]
	ldr	r5, [pc, #60]
	movs	r0, #7
	movs	r1, #0
	adds	r2, r5, #0
	bl	Func_0800307c
	movs	r0, #6
	movs	r1, #0
	adds	r2, r5, #0
	bl	Func_0800307c
	ldr	r4, [pc, #32]
	adds	r3, r6, #0
	strh	r4, [r3, #0]
	ldr	r1, [pc, #36]
	ldr	r3, [pc, #40]
	ldrh	r2, [r1, #0]
	ands	r3, r2
	strh	r3, [r1, #0]
	adds	r1, #2
	ldrh	r2, [r1, #0]
	movs	r0, #128
	adds	r3, r0, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	strh	r0, [r1, #0]
	b.n	.L0
	.4byte 0x00000000
	.4byte 0x04000208
	.4byte 0x08006241
	.4byte 0x04000200
	.4byte 0x0000ff3f
.L0:
	ldrh	r2, [r1, #0]
	movs	r0, #64
	adds	r3, r0, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	strh	r0, [r1, #0]
.L1:
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #48]
	ldr	r1, [pc, #64]
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #5
	strh	r4, [r2, #0]
	str	r3, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #6
	strh	r4, [r2, #0]
	str	r3, [r1, #0]
	ldr	r2, [pc, #32]
	ldrh	r3, [r1, #0]
	orrs	r3, r2
	strh	r3, [r1, #0]
	ldr	r2, [pc, #40]
	ldr	r3, [pc, #24]
	mov	ip, r2
	strh	r3, [r6, #0]
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	mov	r1, ip
	ldr	r3, [pc, #28]
	ldr	r2, [pc, #28]
	b.n	.L2
	.4byte 0x00008000
	.4byte 0x00004003
	.4byte 0x00000001
	.4byte 0x04000134
	.4byte 0x04000128
	.4byte 0x02002240
	.4byte 0x040000d4
	.4byte 0x85000058
.L2:
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #1
	mov	r2, ip
	negs	r3, r3
	str	r3, [r2, #20]
	mov	r3, ip
	adds	r3, #96
	str	r3, [r2, #40]
	mov	r4, ip
	adds	r3, #32
	str	r3, [r2, #44]
	adds	r4, #224
	adds	r3, #64
	adds	r2, #48
	adds	r1, #160
	movs	r0, #1
.L3:
	subs	r0, #1
	str	r1, [r2, #0]
	str	r3, [r2, #16]
	str	r4, [r2, #32]
	adds	r3, #96
	adds	r4, #96
	adds	r2, #4
	adds	r1, #96
	cmp	r0, #0
	bge.n	.L3
	ldr	r5, [pc, #68]
	movs	r0, #0
	strh	r0, [r5, #0]
	ldr	r1, [pc, #64]
	ldr	r2, [pc, #48]
	ldrh	r3, [r1, #0]
	orrs	r3, r2
	strh	r3, [r1, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #56]
	strh	r2, [r5, #0]
	ldr	r4, [pc, #40]
	strh	r2, [r3, #0]
	ldr	r3, [pc, #52]
	strb	r4, [r3, #0]
	ldr	r3, [pc, #52]
	str	r0, [r3, #0]
	ldr	r3, [pc, #52]
	strh	r0, [r3, #0]
	ldr	r3, [pc, #52]
	str	r0, [r3, #0]
	ldr	r3, [pc, #52]
	strh	r0, [r3, #0]
	bl	Func_0800651c
	strh	r7, [r5, #0]
	add	sp, #4
	b.n	.L4
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x04000208
	.4byte 0x04000200
	.4byte 0x03001cb0
	.4byte 0x020023a0
	.4byte 0x02002080
	.4byte 0x02002008
	.4byte 0x020023ac
	.4byte 0x02002238
.L4:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0

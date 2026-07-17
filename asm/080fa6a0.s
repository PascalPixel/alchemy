@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080fa6a0
	.thumb_func
Func_080fa6a0:
	push	{r4, r5, lr}
	sub	sp, #4
	adds	r5, r0, #0
	movs	r3, #0
	str	r3, [r5, #0]
	ldr	r1, [pc, #172]
	ldr	r0, [r1, #0]
	movs	r2, #128
	lsls	r2, r2, #18
	ands	r0, r2
	cmp	r0, #0
	beq.n	.L0
	ldr	r0, [pc, #160]
	str	r0, [r1, #0]
.L0:
	ldr	r1, [pc, #160]
	ldr	r0, [r1, #0]
	ands	r0, r2
	cmp	r0, #0
	beq.n	.L1
	ldr	r0, [pc, #148]
	str	r0, [r1, #0]
.L1:
	ldr	r0, [pc, #152]
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r1, r2, #0
	strh	r1, [r0, #0]
	adds	r0, #12
	strh	r1, [r0, #0]
	ldr	r1, [pc, #140]
	movs	r0, #143
	strh	r0, [r1, #0]
	subs	r1, #2
	ldr	r2, [pc, #136]
	adds	r0, r2, #0
	strh	r0, [r1, #0]
	ldr	r2, [pc, #136]
	ldrb	r1, [r2, #0]
	movs	r0, #63
	ands	r0, r1
	movs	r1, #64
	orrs	r0, r1
	strb	r0, [r2, #0]
	ldr	r1, [pc, #124]
	movs	r2, #212
	lsls	r2, r2, #2
	adds	r0, r5, r2
	str	r0, [r1, #0]
	adds	r1, #4
	ldr	r0, [pc, #116]
	str	r0, [r1, #0]
	adds	r1, #8
	movs	r2, #152
	lsls	r2, r2, #4
	adds	r0, r5, r2
	str	r0, [r1, #0]
	adds	r1, #4
	ldr	r0, [pc, #104]
	str	r0, [r1, #0]
	ldr	r0, [pc, #104]
	str	r5, [r0, #0]
	str	r3, [sp, #0]
	ldr	r2, [pc, #104]
	mov	r0, sp
	adds	r1, r5, #0
	bl	Func_08006864
	movs	r0, #8
	strb	r0, [r5, #6]
	movs	r0, #15
	strb	r0, [r5, #7]
	ldr	r0, [pc, #88]
	str	r0, [r5, #56]
	ldr	r0, [pc, #88]
	str	r0, [r5, #40]
	str	r0, [r5, #44]
	str	r0, [r5, #48]
	str	r0, [r5, #60]
	ldr	r4, [pc, #84]
	adds	r0, r4, #0
	bl	Func_080f9a80
	str	r4, [r5, #52]
	movs	r0, #128
	lsls	r0, r0, #11
	bl	Func_080fa798
	ldr	r0, [pc, #68]
	str	r0, [r5, #0]
	add	sp, #4
	pop	{r4, r5}
	pop	{r0}
	bx	r0
	.4byte 0x040000c4
	.4byte 0x84400004
	.4byte 0x040000d0
	.4byte 0x040000c6
	.4byte 0x04000084
	.4byte 0x0000a90e
	.4byte 0x04000089
	.4byte 0x040000bc
	.4byte 0x040000a0
	.4byte 0x040000a4
	.4byte 0x03007ff0
	.4byte 0x050003ec
	.4byte 0x080f9f6d
	.4byte 0x080fb791
	.4byte 0x02004000
	.4byte 0x68736d53

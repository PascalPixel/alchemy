@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0809b208
	.thumb_func
Func_0809b208:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #180]
	ldr	r3, [r3, #0]
	sub	sp, #12
	ldr	r5, [r3, #16]
	mov	r8, r3
	bl	Func_080916b0
	movs	r2, #1
	negs	r2, r2
	adds	r0, r2, #0
	adds	r1, r2, #0
	movs	r3, #0
	bl	Func_080933f8
	bl	Func_08097384
	movs	r0, #10
	bl	Func_080030f8
	mov	r3, r8
	movs	r1, #128
	movs	r2, #24
	ldrsh	r0, [r3, r2]
	lsls	r1, r1, #7
	movs	r2, #0
	bl	Func_08092adc
	movs	r0, #30
	bl	Func_080030f8
	ldr	r3, [pc, #124]
	movs	r0, #131
	str	r3, [r5, #108]
	bl	Func_080f9010
	movs	r1, #28
	adds	r0, r5, #0
	bl	Func_08009080
	movs	r0, #40
	bl	Func_080030f8
	movs	r0, #220
	bl	Func_080f9010
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_08009240
	adds	r0, r5, #0
	movs	r1, #3
	bl	Func_08009080
	ldr	r3, [pc, #80]
	adds	r2, r5, #0
	str	r3, [r5, #108]
	adds	r2, #100
	movs	r3, #0
	strh	r3, [r2, #0]
	movs	r0, #70
	bl	Func_080030f8
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_080091e0
	ldr	r6, [pc, #44]
	adds	r3, r5, #0
	adds	r3, #85
	strb	r6, [r3, #0]
	ldr	r3, [pc, #52]
	str	r3, [r5, #108]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r5, #56]
	ldr	r3, [r5, #8]
	mov	r6, sp
	str	r3, [r6, #0]
	ldr	r3, [r5, #12]
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	adds	r0, r6, #0
	str	r3, [r6, #8]
	bl	Func_080974d8
	mov	r5, r8
	movs	r7, #0
	adds	r5, #88
	b.n	.L0
	.4byte 0x00000000
	.4byte 0x03001f30
	.4byte 0x08096b89
	.4byte 0x0809b0b1
	.4byte 0x0809b0dd
.L0:
	movs	r1, #142
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #8]
	adds	r0, r5, #0
	lsls	r1, r1, #1
	bl	Func_0809ba90
	adds	r0, r5, #0
	ldr	r1, [pc, #116]
	bl	Func_0809ba7c
	adds	r0, r5, #0
	movs	r1, #7
	bl	Func_0809ba70
	bl	Func_08004458
	lsls	r1, r0, #3
	subs	r1, r1, r0
	lsrs	r1, r1, #16
	ldr	r0, [r5, #0]
	bl	Func_08009248
	bl	Func_08004458
	ldr	r2, [pc, #88]
	lsrs	r0, r0, #1
	adds	r0, r0, r2
	str	r0, [r5, #44]
	str	r0, [r5, #40]
	adds	r7, #1
	movs	r0, #1
	bl	Func_080030f8
	adds	r5, #72
	cmp	r7, #23
	bls.n	.L0
	movs	r0, #70
	bl	Func_080030f8
	mov	r2, r8
	movs	r7, #0
	movs	r1, #2
	adds	r2, #152
.L2:
	movs	r3, #5
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L1
	strb	r1, [r2, #0]
.L1:
	adds	r7, #1
	adds	r2, #72
	cmp	r7, #23
	bls.n	.L2
	movs	r0, #40
	bl	Func_080030f8
	bl	Func_0809748c
	movs	r0, #10
	bl	Func_080030f8
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0809b11d
	.4byte 0x00013333

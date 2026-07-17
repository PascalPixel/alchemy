@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08095dd0
	.thumb_func
Func_08095dd0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #12
	adds	r6, r0, #0
	bl	Func_08092054
	ldr	r3, [pc, #412]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl	Func_08092054
	mov	r8, r0
	cmp	r5, #0
	bne.n	.L0
	b.n	.L1
.L0:
	bl	Func_080958a8
	ldr	r3, [pc, #388]
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #388]
	mov	sl, r3
	bl	Func_080b0048
	movs	r0, #30
	bl	Func_080030f8
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	adds	r0, r6, #0
	bl	Func_08092adc
	movs	r0, #20
	bl	Func_080030f8
	movs	r0, #173
	bl	Func_080f9010
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_080925cc
	movs	r0, #174
	bl	Func_080f9010
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_080925cc
	movs	r0, #175
	bl	Func_080f9010
	movs	r1, #1
	adds	r0, r6, #0
	bl	Func_080925cc
	movs	r0, #20
	bl	Func_080030f8
	movs	r0, #140
	bl	Func_080f9010
	ldr	r3, [pc, #308]
	adds	r2, r5, #0
	str	r3, [r5, #108]
	adds	r2, #100
	movs	r3, #0
	strh	r3, [r2, #0]
	movs	r0, #80
	bl	Func_080030f8
	ldr	r3, [pc, #292]
	adds	r0, r5, #0
	str	r3, [r5, #108]
	movs	r1, #3
	bl	Func_08009080
	ldr	r3, [r5, #8]
	mov	r7, sp
	str	r3, [r7, #0]
	ldr	r3, [r5, #12]
	str	r3, [r7, #4]
	ldr	r3, [r5, #16]
	adds	r0, r7, #0
	str	r3, [r7, #8]
	bl	Func_080974d8
	mov	r5, sl
	adds	r5, #88
	movs	r6, #23
.L2:
	movs	r1, #142
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #8]
	adds	r0, r5, #0
	lsls	r1, r1, #1
	bl	Func_0809ba90
	adds	r0, r5, #0
	ldr	r1, [pc, #244]
	bl	Func_0809ba7c
	adds	r0, r5, #0
	movs	r1, #7
	bl	Func_0809ba70
	ldr	r0, [r5, #0]
	movs	r1, #10
	bl	Func_08009248
	bl	Func_08004458
	movs	r1, #3
	bl	Func_080022f4
	movs	r3, #128
	lsls	r3, r3, #9
	adds	r0, r0, r3
	str	r0, [r5, #44]
	str	r0, [r5, #40]
	subs	r6, #1
	movs	r0, #1
	bl	Func_080030f8
	adds	r5, #72
	cmp	r6, #0
	bge.n	.L2
	movs	r0, #60
	bl	Func_080030f8
	ldr	r5, [pc, #160]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r5, r5, r2
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	ldr	r0, [r5, #0]
	bl	Func_08092adc
	movs	r0, #20
	bl	Func_080030f8
	ldr	r0, [r5, #0]
	bl	Func_08092054
	movs	r1, #28
	bl	Func_08009080
	movs	r0, #20
	bl	Func_080030f8
	mov	r2, sl
	movs	r1, #2
	adds	r2, #152
	movs	r6, #23
.L4:
	movs	r3, #5
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L3
	strb	r1, [r2, #0]
.L3:
	subs	r6, #1
	adds	r2, #72
	cmp	r6, #0
	bge.n	.L4
	movs	r0, #60
	bl	Func_080030f8
	ldr	r3, [pc, #108]
	mov	r2, r8
	str	r3, [r2, #108]
	movs	r0, #100
	bl	Func_080030f8
	mov	r2, sl
	movs	r1, #5
	adds	r2, #152
	movs	r6, #23
.L6:
	movs	r3, #5
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L5
	strb	r1, [r2, #0]
.L5:
	subs	r6, #1
	adds	r2, #72
	cmp	r6, #0
	bge.n	.L6
	movs	r0, #10
	bl	Func_080030f8
	movs	r5, #0
	mov	r3, r8
	str	r5, [r3, #108]
	movs	r3, #128
	lsls	r3, r3, #9
	mov	r2, r8
	str	r3, [r2, #24]
	str	r3, [r2, #28]
	movs	r0, #30
	bl	Func_080030f8
	bl	Func_080b0050
	bl	Func_080958e4
.L1:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
	.4byte 0x03001f30
	.4byte 0x00201090
	.4byte 0x08095bad
	.4byte 0x08095bd9
	.4byte 0x08095c09
	.4byte 0x08095b8d

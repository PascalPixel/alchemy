@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080965a8
	.thumb_func
Func_080965a8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #52
	adds	r5, r0, #0
	bl	Func_08092054
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L0
	b.n	.L1
.L0:
	bl	Func_080958a8
	ldr	r3, [pc, #516]
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #516]
	str	r3, [sp, #4]
	bl	Func_080b0048
	movs	r0, #30
	bl	Func_080030f8
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #91
	strb	r3, [r2, #0]
	movs	r0, #173
	bl	Func_080f9010
	movs	r1, #1
	adds	r0, r5, #0
	bl	Func_080925cc
	movs	r0, #175
	bl	Func_080f9010
	movs	r1, #1
	adds	r0, r5, #0
	bl	Func_080925cc
	movs	r0, #20
	bl	Func_080030f8
	movs	r0, #152
	bl	Func_080f9010
	movs	r1, #3
	movs	r2, #14
	adds	r0, r5, #0
	bl	Func_08092560
	movs	r0, #152
	bl	Func_080f9010
	movs	r1, #5
	movs	r2, #16
	adds	r0, r5, #0
	bl	Func_08092560
	movs	r0, #152
	bl	Func_080f9010
	movs	r1, #7
	movs	r2, #18
	adds	r0, r5, #0
	bl	Func_08092560
	movs	r0, #20
	bl	Func_080030f8
	ldr	r3, [r6, #80]
	ldr	r3, [r3, #40]
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	mov	r9, r1
	add	r1, sp, #20
	movs	r3, #0
	str	r1, [sp, #0]
	mov	sl, r3
	mov	r8, r6
	mov	fp, r1
	movs	r7, #7
.L3:
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	ldr	r1, [r6, #8]
	mov	r0, r9
	bl	Func_080090c8
	ldr	r3, [sp, #0]
	adds	r5, r0, #0
	stmia	r3!, {r5}
	adds	r2, r3, #0
	str	r2, [sp, #0]
	cmp	r5, #0
	beq.n	.L2
	movs	r3, #240
	lsls	r3, r3, #8
	adds	r2, r5, #0
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	subs	r2, #50
	movs	r3, #2
	strb	r3, [r2, #0]
	adds	r1, r5, #0
	adds	r1, #90
	ldrb	r3, [r1, #0]
	movs	r2, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	ldr	r3, [pc, #324]
	str	r3, [r5, #108]
	ldrh	r3, [r6, #6]
	movs	r1, #9
	strh	r3, [r5, #6]
	bl	Func_08009240
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_08009080
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_080091e0
	mov	r1, sl
	ldr	r0, [r5, #80]
	bl	Func_08096c48
	mov	r1, r8
	str	r1, [r5, #104]
	mov	sl, r0
	mov	r8, r5
.L2:
	subs	r7, #1
	cmp	r7, #0
	bge.n	.L3
	mov	r2, sl
	ldrb	r2, [r2, #28]
	movs	r0, #153
	mov	r8, r2
	bl	Func_080f9010
	adds	r2, r6, #0
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r7, #14
.L4:
	ldr	r3, [r6, #12]
	movs	r1, #128
	lsls	r1, r1, #12
	adds	r3, r3, r1
	str	r3, [r6, #12]
	movs	r0, #1
	subs	r7, #1
	bl	Func_080030f8
	cmp	r7, #0
	bge.n	.L4
	adds	r0, r6, #0
	bl	Func_080090d0
	mov	r5, fp
	movs	r7, #7
.L5:
	ldmia	r5!, {r0}
	subs	r7, #1
	bl	Func_080090d0
	cmp	r7, #0
	bge.n	.L5
	mov	r2, r8
	cmp	r2, #96
	beq.n	.L6
	mov	r0, r8
	bl	Func_08003f3c
.L6:
	movs	r0, #10
	bl	Func_080030f8
	ldr	r5, [pc, #192]
	movs	r3, #250
	lsls	r3, r3, #1
	adds	r5, r5, r3
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
	ldr	r3, [r6, #8]
	add	r5, sp, #8
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	adds	r0, r5, #0
	str	r3, [r5, #8]
	bl	Func_080974d8
	ldr	r6, [sp, #4]
	movs	r7, #23
	adds	r6, #88
.L7:
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #8]
	adds	r0, r6, #0
	movs	r1, #240
	bl	Func_0809ba90
	adds	r0, r6, #0
	ldr	r1, [pc, #112]
	bl	Func_0809ba7c
	adds	r0, r6, #0
	movs	r1, #7
	bl	Func_0809ba70
	ldr	r0, [r6, #0]
	movs	r1, #9
	bl	Func_08009248
	subs	r7, #1
	movs	r0, #1
	bl	Func_080030f8
	adds	r6, #72
	cmp	r7, #0
	bge.n	.L7
	movs	r0, #120
	bl	Func_080030f8
	ldr	r2, [sp, #4]
	movs	r1, #2
	adds	r2, #152
	movs	r7, #23
.L9:
	movs	r3, #5
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L8
	strb	r1, [r2, #0]
.L8:
	subs	r7, #1
	adds	r2, #72
	cmp	r7, #0
	bge.n	.L9
	movs	r0, #50
	bl	Func_080030f8
	bl	Func_080b0050
	bl	Func_080958e4
.L1:
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f30
	.4byte 0x00204084
	.4byte 0x08096575
	.4byte 0x02000240
	.4byte 0x0809641d

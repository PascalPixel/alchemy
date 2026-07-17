@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080f9f6c
	.thumb_func
Func_080f9f6c:
	push	{r4, r5, r6, r7, lr}
	mov	r4, r8
	mov	r5, r9
	mov	r6, sl
	mov	r7, fp
	push	{r4, r5, r6, r7}
	sub	sp, #24
	str	r1, [sp, #0]
	adds	r5, r2, #0
	ldr	r1, [pc, #484]
	ldr	r1, [r1, #0]
	str	r1, [sp, #4]
	ldr	r1, [pc, #480]
	adds	r0, r0, r1
	ldrb	r0, [r0, #0]
	strb	r0, [r5, #4]
	ldr	r3, [r5, #64]
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L0
	strb	r0, [r5, #5]
	adds	r3, #1
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L1
	strb	r0, [r5, #6]
	adds	r3, #1
	ldrb	r0, [r3, #0]
	cmp	r0, #128
	bcs.n	.L1
	ldrb	r1, [r5, #4]
	adds	r1, r1, r0
	strb	r1, [r5, #4]
	adds	r3, #1
.L1:
	str	r3, [r5, #64]
.L0:
	movs	r0, #0
	str	r0, [sp, #20]
	adds	r4, r5, #0
	adds	r4, #36
	ldrb	r2, [r4, #0]
	movs	r0, #192
	tst	r0, r2
	beq.n	.L2
	ldrb	r3, [r5, #5]
	movs	r0, #64
	tst	r0, r2
	beq.n	.L3
	ldr	r1, [r5, #44]
	adds	r1, r1, r3
	ldrb	r0, [r1, #0]
	b.n	.L4
.L3:
	adds	r0, r3, #0
.L4:
	lsls	r1, r0, #1
	adds	r1, r1, r0
	lsls	r1, r1, #2
	ldr	r0, [r5, #40]
	adds	r1, r1, r0
	mov	r9, r1
	mov	r6, r9
	ldrb	r1, [r6, #0]
	movs	r0, #192
	tst	r0, r1
	beq.n	.L5
	b.n	.L6
.L5:
	movs	r0, #128
	tst	r0, r2
	beq.n	.L7
	ldrb	r1, [r6, #3]
	movs	r0, #128
	tst	r0, r1
	beq.n	.L8
	subs	r1, #192
	lsls	r1, r1, #1
	str	r1, [sp, #20]
.L8:
	ldrb	r3, [r6, #1]
	b.n	.L7
.L2:
	mov	r9, r4
	ldrb	r3, [r5, #5]
.L7:
	str	r3, [sp, #8]
	ldr	r6, [sp, #0]
	ldrb	r1, [r6, #9]
	ldrb	r0, [r5, #29]
	adds	r0, r0, r1
	cmp	r0, #255
	bls.n	.L9
	movs	r0, #255
.L9:
	str	r0, [sp, #16]
	mov	r6, r9
	ldrb	r0, [r6, #0]
	movs	r6, #7
	ands	r6, r0
	str	r6, [sp, #12]
	beq.n	.L10
	ldr	r0, [sp, #4]
	ldr	r4, [r0, #28]
	cmp	r4, #0
	bne.n	.L11
	b.n	.L6
.L11:
	subs	r6, #1
	lsls	r0, r6, #6
	adds	r4, r4, r0
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L12
	movs	r0, #64
	tst	r0, r1
	bne.n	.L12
	ldrb	r1, [r4, #19]
	ldr	r0, [sp, #16]
	cmp	r1, r0
	bcc.n	.L12
	beq.n	.L13
	b.n	.L6
.L13:
	ldr	r0, [r4, #44]
	cmp	r0, r5
	bcs.n	.L12
	b.n	.L6
.L10:
	ldr	r6, [sp, #16]
	adds	r7, r5, #0
	movs	r2, #0
	mov	r8, r2
	ldr	r4, [sp, #4]
	ldrb	r3, [r4, #6]
	adds	r4, #80
.L20:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L12
	movs	r0, #64
	tst	r0, r1
	beq.n	.L14
	cmp	r2, #0
	bne.n	.L15
	adds	r2, #1
	ldrb	r6, [r4, #19]
	ldr	r7, [r4, #44]
	b.n	.L16
.L14:
	cmp	r2, #0
	bne.n	.L17
.L15:
	ldrb	r0, [r4, #19]
	cmp	r0, r6
	bcs.n	.L18
	adds	r6, r0, #0
	ldr	r7, [r4, #44]
	b.n	.L16
.L18:
	bhi.n	.L17
	ldr	r0, [r4, #44]
	cmp	r0, r7
	bls.n	.L19
	adds	r7, r0, #0
	b.n	.L16
.L19:
	bcc.n	.L17
.L16:
	mov	r8, r4
.L17:
	adds	r4, #64
	subs	r3, #1
	bgt.n	.L20
	mov	r4, r8
	cmp	r4, #0
	beq.n	.L6
.L12:
	adds	r0, r4, #0
	bl	Func_080fa678
	movs	r1, #0
	str	r1, [r4, #48]
	ldr	r3, [r5, #32]
	str	r3, [r4, #52]
	cmp	r3, #0
	beq.n	.L21
	str	r4, [r3, #48]
.L21:
	str	r4, [r5, #32]
	str	r5, [r4, #44]
	ldrb	r0, [r5, #27]
	strb	r0, [r5, #28]
	cmp	r0, r1
	beq.n	.L22
	adds	r1, r5, #0
	bl	Func_080fa1ac
.L22:
	ldr	r0, [sp, #0]
	adds	r1, r5, #0
	bl	Func_080fac44
	ldr	r0, [r5, #4]
	str	r0, [r4, #16]
	ldr	r0, [sp, #16]
	strb	r0, [r4, #19]
	ldr	r0, [sp, #8]
	strb	r0, [r4, #8]
	ldr	r0, [sp, #20]
	strb	r0, [r4, #20]
	mov	r6, r9
	ldrb	r0, [r6, #0]
	strb	r0, [r4, #1]
	ldr	r7, [r6, #4]
	str	r7, [r4, #36]
	ldr	r0, [r6, #8]
	str	r0, [r4, #4]
	ldrh	r0, [r5, #30]
	strh	r0, [r4, #12]
	bl	Func_080f9f3c
	ldrb	r1, [r4, #8]
	movs	r0, #8
	ldrsb	r0, [r5, r0]
	adds	r3, r1, r0
	bpl.n	.L23
	movs	r3, #0
.L23:
	ldr	r6, [sp, #12]
	cmp	r6, #0
	beq.n	.L24
	mov	r6, r9
	ldrb	r0, [r6, #2]
	strb	r0, [r4, #30]
	ldrb	r1, [r6, #3]
	movs	r0, #128
	tst	r0, r1
	bne.n	.L25
	movs	r0, #112
	tst	r0, r1
	bne.n	.L26
.L25:
	movs	r1, #8
.L26:
	strb	r1, [r4, #31]
	ldrb	r2, [r5, #9]
	adds	r1, r3, #0
	ldr	r0, [sp, #12]
	ldr	r3, [sp, #4]
	ldr	r3, [r3, #48]
	bl	Func_080f9ee8
	b.n	.L27
.L24:
	ldrb	r2, [r5, #9]
	adds	r1, r3, #0
	adds	r0, r7, #0
	bl	Func_080fa1fc
.L27:
	str	r0, [r4, #32]
	movs	r0, #128
	strb	r0, [r4, #0]
	ldrb	r1, [r5, #0]
	movs	r0, #240
	ands	r0, r1
	strb	r0, [r5, #0]
.L6:
	add	sp, #24
	pop	{r0, r1, r2, r3, r4, r5, r6, r7}
	mov	r8, r0
	mov	r9, r1
	mov	sl, r2
	mov	fp, r3
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03007ff0
	.4byte 0x080fba14

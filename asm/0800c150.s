@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0800c150
	.thumb_func
Func_0800c150:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	adds	r7, r0, #0
	mov	fp, r3
	mov	r9, r1
	str	r2, [sp, #0]
	bl	Func_0800c0cc
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L0
	ldr	r2, [pc, #84]
	adds	r3, r7, r2
.L0:
	asrs	r5, r3, #12
	ldr	r3, [pc, #76]
	ands	r7, r3
	bl	Func_0800c0cc
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L1
	b.n	.L2
.L1:
	movs	r3, #16
	strh	r3, [r6, #32]
	cmp	r5, #0
	beq.n	.L3
	cmp	r5, #2
	beq.n	.L4
	b.n	.L5
.L3:
	adds	r0, r7, #0
	bl	Func_0800bc70
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L6
	adds	r2, r6, #0
	movs	r3, #1
	adds	r2, #84
	strb	r3, [r2, #0]
	adds	r0, r7, #0
	str	r5, [r6, #80]
	bl	Func_08185000
	ldrb	r3, [r0, #9]
	lsrs	r3, r3, #1
	strh	r3, [r6, #32]
	b.n	.L5
.L6:
	adds	r2, r6, #0
	ldr	r3, [pc, #4]
	adds	r2, #84
	strb	r3, [r2, #0]
	b.n	.L5
	.4byte 0x00000000
	.4byte 0x00000fff
.L4:
	ldr	r3, [pc, #192]
	ldr	r1, [r3, #0]
	adds	r0, r1, #0
	adds	r0, #24
	ldr	r3, [r0, #0]
	lsls	r2, r3, #2
	adds	r1, r1, r2
	adds	r3, #1
	str	r3, [r0, #0]
	mov	sl, r1
	adds	r3, r6, #0
	movs	r4, #8
	add	r4, sl
	adds	r3, #84
	strb	r5, [r3, #0]
	mov	r8, r4
	add	r0, sp, #4
	movs	r3, #0
	str	r3, [r0, #0]
	str	r4, [r6, #80]
	ldr	r3, [pc, #156]
	mov	r1, r8
	ldr	r2, [pc, #156]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r7, #0
	bl	Func_0800bc70
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L7
	adds	r0, r7, #0
	bl	Func_08185000
	ldrb	r3, [r0, #9]
	lsrs	r3, r3, #1
	mov	r2, r8
	strh	r3, [r6, #32]
	movs	r3, #12
	add	r3, sl
	str	r5, [r2, #0]
	mov	r8, r3
.L7:
	adds	r0, r7, #1
	bl	Func_0800bc70
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L5
	mov	r4, r8
	str	r5, [r4, #0]
.L5:
	cmp	r6, #0
	beq.n	.L2
	adds	r0, r6, #0
	mov	r1, r9
	ldr	r2, [sp, #0]
	mov	r3, fp
	bl	Func_0800d130
	ldr	r3, [pc, #88]
	str	r3, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r6, #48]
	movs	r3, #85
	adds	r3, r3, r6
	mov	ip, r3
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r1, #0
	movs	r3, #3
	mov	r4, ip
	strh	r1, [r6, #4]
	str	r2, [r6, #24]
	str	r2, [r6, #28]
	str	r2, [r6, #52]
	ldr	r0, [pc, #40]
	strb	r3, [r4, #0]
	str	r2, [r6, #72]
	adds	r3, r6, #0
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r3, #89
	str	r2, [r6, #68]
	strb	r0, [r3, #0]
	adds	r0, r6, #0
	adds	r0, #90
	movs	r3, #1
	strb	r3, [r0, #0]
	str	r1, [r6, #76]
	mov	r1, r9
	strh	r2, [r6, #6]
	cmp	r1, #0
	bge.n	.L8
	ldr	r2, [pc, #24]
	adds	r1, r1, r2
	b.n	.L8
	.4byte 0x00000000
	.4byte 0x03001e68
	.4byte 0x040000d4
	.4byte 0x85000004
	.4byte 0x0801358c
	.4byte 0x0000ffff
.L8:
	adds	r3, r6, #0
	adds	r3, #100
	asrs	r2, r1, #16
	strh	r2, [r3, #0]
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L9
	ldr	r4, [pc, #32]
	adds	r3, r3, r4
.L9:
	adds	r2, r6, #0
	asrs	r3, r3, #16
	adds	r2, #102
	strh	r3, [r2, #0]
.L2:
	adds	r0, r6, #0
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x0000ffff

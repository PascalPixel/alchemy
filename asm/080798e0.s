@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080798e0
	.thumb_func
Func_080798e0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #20
	mov	r9, r1
	bl	Func_08077394
	adds	r2, r0, #0
	ldr	r0, [pc, #172]
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L0
	movs	r1, #148
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r0, [r3, #0]
	bl	Func_080773d8
	adds	r0, #52
	ldrb	r0, [r0, #0]
	cmp	r0, #43
	bls.n	.L1
	movs	r0, #0
.L1:
	lsls	r3, r0, #1
	ldr	r2, [pc, #144]
	adds	r3, r3, r0
	lsls	r3, r3, #3
	adds	r3, r3, r2
	adds	r2, r3, #0
	movs	r7, #0
	mov	r1, r9
	adds	r2, #8
.L2:
	ldmia	r2!, {r3}
	adds	r7, #1
	stmia	r1!, {r3}
	cmp	r7, #3
	ble.n	.L2
	b.n	.L3
.L0:
	movs	r0, #148
	lsls	r0, r0, #1
	adds	r3, r2, r0
	ldrb	r0, [r3, #0]
	add	r3, sp, #4
	adds	r1, r2, #0
	mov	r8, r3
	adds	r1, #248
	mov	r2, r8
	bl	Func_080797fc
	ldr	r0, [pc, #96]
	movs	r4, #0
	mov	sl, r0
	movs	r7, #3
.L6:
	mov	r1, r8
	ldr	r5, [r4, r1]
	movs	r1, #10
	adds	r0, r5, #0
	str	r4, [sp, #0]
	bl	Func_080022fc
	movs	r1, #10
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	Func_080022ec
	ldr	r4, [sp, #0]
	cmp	r0, #15
	ble.n	.L4
	movs	r0, #15
.L4:
	cmp	r0, #0
	bge.n	.L5
	movs	r0, #0
.L5:
	lsls	r2, r0, #2
	mov	r3, r9
	mov	r0, sl
	adds	r1, r4, r3
	ldrh	r3, [r0, r2]
	adds	r3, r3, r6
	strh	r3, [r1, #0]
	add	r2, sl
	ldrh	r3, [r2, #2]
	subs	r7, #1
	adds	r3, r3, r6
	strh	r3, [r1, #2]
	adds	r4, #4
	cmp	r7, #0
	bge.n	.L6
.L3:
	add	sp, #20
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x00000129
	.4byte 0x08088e38
	.4byte 0x08088df8

@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b5d3c
	.thumb_func
Func_080b5d3c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	add	r1, sp, #8
	mov	r9, r1
	mov	r0, r9
	bl	Func_080b6a60
	movs	r3, #28
	movs	r2, #0
	add	r3, sp
	adds	r4, r0, #0
	mov	fp, r2
	mov	r8, r2
	mov	sl, r3
.L2:
	movs	r3, #0
	mov	r1, sl
	mov	r2, r8
	strb	r3, [r1, r2]
	cmp	r4, #0
	ble.n	.L0
	movs	r1, #140
	lsls	r1, r1, #1
	mov	r7, sl
	add	r1, r8
	mov	r6, r9
	adds	r5, r4, #0
.L1:
	ldrh	r0, [r6, #0]
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	bl	Func_08077008
	ldr	r1, [sp, #4]
	mov	r2, r8
	ldrb	r3, [r7, r2]
	ldrb	r2, [r0, r1]
	subs	r5, #1
	adds	r3, r3, r2
	mov	r2, r8
	adds	r6, #2
	strb	r3, [r7, r2]
	ldr	r4, [sp, #0]
	cmp	r5, #0
	bne.n	.L1
.L0:
	movs	r3, #1
	add	r8, r3
	mov	r1, r8
	cmp	r1, #3
	ble.n	.L2
	movs	r2, #0
	mov	r8, r2
.L6:
	mov	r0, r8
	bl	Func_080771e0
	cmp	r0, #0
	beq.n	.L3
	mov	r3, sl
	adds	r0, #4
	ldrb	r2, [r3, #0]
	ldrb	r3, [r0, #0]
	movs	r4, #0
	cmp	r2, r3
	bcc.n	.L4
	mov	r1, sl
.L5:
	adds	r4, #1
	cmp	r4, #3
	bgt.n	.L4
	adds	r1, #1
	adds	r0, #1
	ldrb	r2, [r1, #0]
	ldrb	r3, [r0, #0]
	cmp	r2, r3
	bcs.n	.L5
.L4:
	cmp	r4, #4
	bne.n	.L3
	movs	r3, #1
	mov	r1, r8
	mov	r2, fp
	lsls	r3, r1
	orrs	r2, r3
	mov	fp, r2
.L3:
	movs	r3, #1
	add	r8, r3
	mov	r1, r8
	cmp	r1, #31
	ble.n	.L6
	movs	r0, #0
	bl	Func_08077000
	mov	r2, fp
	str	r2, [r0, #0]
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	bx	lr

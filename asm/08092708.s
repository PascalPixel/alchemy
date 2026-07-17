@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08092708
	.thumb_func
Func_08092708:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r2
	mov	sl, r0
	adds	r5, r1, #0
	bl	Func_08092054
	adds	r6, r0, #0
	ldr	r2, [r6, #16]
	mov	r9, r2
	cmp	r6, #0
	beq.n	.L0
	ldr	r0, [pc, #220]
	bl	Func_080f9010
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	Func_08009080
	adds	r7, r6, #0
	movs	r0, #10
	bl	Func_080030f8
	adds	r7, #85
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_08009080
	ldrb	r2, [r7, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r7, #0]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r6, #40]
	movs	r3, #192
	lsls	r3, r3, #12
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	add	r3, r9
	adds	r0, r6, #0
	bl	Func_08009150
	movs	r0, #6
	bl	Func_080030f8
	movs	r0, #217
	bl	Func_080f9010
	movs	r1, #160
	movs	r5, #0
	lsls	r1, r1, #7
	mov	r0, sl
	movs	r2, #0
	bl	Func_08092adc
	strb	r5, [r7, #0]
.L2:
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #132]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	str	r3, [r6, #60]
	movs	r0, #1
	bl	Func_080030f8
	movs	r3, #1
	negs	r3, r3
	cmp	r8, r3
	beq.n	.L1
	movs	r3, #1
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L1
	adds	r0, r6, #0
	mov	r1, r8
	bl	Func_08092624
.L1:
	adds	r5, #1
	cmp	r5, #13
	bls.n	.L2
	movs	r3, #3
	strb	r3, [r7, #0]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r6, #40]
	movs	r3, #128
	lsls	r3, r3, #13
	ldr	r2, [r6, #12]
	add	r3, r9
	ldr	r1, [r6, #8]
	adds	r0, r6, #0
	bl	Func_08009150
	adds	r0, r6, #0
	bl	Func_08009158
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #20]
	movs	r5, #0
	cmp	r2, r3
	ble.n	.L3
.L4:
	movs	r0, #1
	adds	r5, #1
	bl	Func_080030f8
	cmp	r5, #179
	bhi.n	.L3
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #20]
	cmp	r2, r3
	bgt.n	.L4
.L3:
	movs	r0, #2
	bl	Func_080030f8
	bl	Func_0809202c
.L0:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00000121
	.4byte 0xfffe0000

@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0802938c
	.thumb_func
Func_0802938c:
	push	{r5, r6, lr}
	ldr	r6, [pc, #312]
	ldr	r3, [r6, #0]
	adds	r5, r2, #0
	movs	r2, #1
	ands	r3, r2
	adds	r4, r5, #4
	cmp	r3, #0
	beq.n	.L0
	ldr	r3, [r1, #0]
	ldr	r2, [r4, #0]
	lsls	r3, r3, #4
	adds	r3, r3, r2
	ldr	r2, [r5, #0]
	lsls	r3, r3, #4
	adds	r5, r3, r2
	adds	r0, r5, #0
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L1
	adds	r0, r5, #0
	bl	Func_080770d0
	b.n	.L2
.L1:
	adds	r0, r5, #0
	bl	Func_080770c8
	b.n	.L2
.L0:
	ldr	r3, [pc, #260]
	ldr	r3, [r3, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L3
	ldr	r3, [r6, #0]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L4
.L3:
	movs	r0, #1
	negs	r0, r0
	b.n	.L5
.L4:
	ldr	r0, [r6, #0]
	movs	r3, #64
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L6
	ldr	r3, [r4, #0]
	subs	r3, #1
	str	r3, [r4, #0]
	cmp	r3, #0
	bge.n	.L7
	movs	r3, #15
	str	r3, [r4, #0]
	b.n	.L7
.L6:
	ldr	r3, [r6, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L8
	ldr	r3, [r4, #0]
	adds	r3, #1
	str	r3, [r4, #0]
	cmp	r3, #15
	ble.n	.L7
	str	r0, [r4, #0]
	b.n	.L7
.L8:
	ldr	r0, [r6, #0]
	movs	r3, #32
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L9
	ldr	r3, [r5, #0]
	subs	r3, #1
	str	r3, [r5, #0]
	cmp	r3, #0
	bge.n	.L7
	movs	r3, #15
	str	r3, [r5, #0]
	b.n	.L7
.L9:
	ldr	r3, [r6, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L10
	ldr	r3, [r5, #0]
	adds	r3, #1
	str	r3, [r5, #0]
	cmp	r3, #15
	ble.n	.L7
	str	r0, [r5, #0]
	b.n	.L7
.L10:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L11
	ldr	r3, [r6, #0]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L11
	ldr	r3, [r1, #0]
	subs	r3, #10
	b.n	.L12
.L11:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L13
	ldr	r3, [r6, #0]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L13
	ldr	r3, [r1, #0]
	adds	r3, #10
	str	r3, [r1, #0]
	cmp	r3, #15
	ble.n	.L2
	movs	r3, #0
	str	r3, [r1, #0]
	b.n	.L2
.L13:
	ldr	r0, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L14
	ldr	r3, [r1, #0]
	subs	r3, #1
.L12:
	str	r3, [r1, #0]
	cmp	r3, #0
	bge.n	.L2
	movs	r3, #15
	str	r3, [r1, #0]
	b.n	.L2
.L14:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L7
	ldr	r3, [r1, #0]
	adds	r3, #1
	str	r3, [r1, #0]
	cmp	r3, #15
	ble.n	.L2
	str	r0, [r1, #0]
.L2:
	movs	r0, #1
	b.n	.L5
.L7:
	movs	r0, #0
.L5:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x03001b04
	.4byte 0x03001c94
	bx	lr

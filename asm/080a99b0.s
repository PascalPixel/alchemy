@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a99b0
	.thumb_func
Func_080a99b0:
	push	{lr}
	ldr	r3, [r0, #0]
	ldr	r4, [r1, #0]
	cmp	r2, #32
	beq.n	.L0
	cmp	r2, #32
	bgt.n	.L1
	cmp	r2, #16
	beq.n	.L2
	b.n	.L3
.L1:
	cmp	r2, #64
	beq.n	.L4
	cmp	r2, #128
	beq.n	.L5
	b.n	.L3
.L4:
	subs	r4, #1
	cmp	r4, #0
	bge.n	.L6
	movs	r4, #5
.L6:
	cmp	r4, #3
	bgt.n	.L3
	cmp	r4, #3
	bne.n	.L7
	cmp	r3, #4
	bgt.n	.L8
	movs	r3, #0
	b.n	.L9
.L7:
	cmp	r3, #1
	ble.n	.L9
.L8:
	movs	r3, #1
.L9:
	cmp	r4, #3
	bne.n	.L3
	cmp	r3, #1
	bne.n	.L3
	movs	r4, #2
	b.n	.L3
.L5:
	adds	r4, #1
	cmp	r4, #5
	ble.n	.L10
	movs	r4, #0
.L10:
	cmp	r4, #3
	bne.n	.L11
	cmp	r3, #1
	bne.n	.L11
	movs	r4, #4
.L11:
	cmp	r4, #4
	bne.n	.L3
	b.n	.L12
.L0:
	subs	r3, #1
	cmp	r4, #3
	bne.n	.L13
	adds	r3, #1
	b.n	.L3
.L13:
	cmp	r4, #3
	ble.n	.L14
	cmp	r3, #0
	bge.n	.L3
	movs	r3, #7
	b.n	.L3
.L14:
	cmp	r3, #0
	bge.n	.L3
	movs	r3, #1
	b.n	.L3
.L2:
	adds	r3, #1
	cmp	r4, #3
	bne.n	.L15
	subs	r3, #1
	b.n	.L3
.L15:
	cmp	r4, #3
	ble.n	.L16
	cmp	r3, #7
	ble.n	.L3
	b.n	.L12
.L16:
	cmp	r3, #1
	ble.n	.L3
.L12:
	movs	r3, #0
.L3:
	str	r3, [r0, #0]
	lsls	r0, r4, #3
	adds	r0, r0, r4
	adds	r0, r0, r3
	str	r4, [r1, #0]
	pop	{r1}
	bx	r1
	movs	r0, r0
	movs	r0, #1
	bx	lr

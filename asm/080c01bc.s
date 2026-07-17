@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080c01bc
	.thumb_func
Func_080c01bc:
	push	{lr}
	ldr	r3, [pc, #92]
	ldr	r0, [r3, #0]
	subs	r3, #120
	ldr	r1, [r0, #0]
	ldr	r4, [r3, #0]
	movs	r3, #52
	subs	r2, r3, r1
	cmp	r2, #32
	ble.n	.L0
	movs	r2, #32
.L0:
	cmp	r2, #0
	bge.n	.L1
	movs	r2, #0
.L1:
	ldr	r3, [pc, #68]
	strh	r2, [r3, #2]
	cmp	r1, #80
	bhi.n	.L2
	lsls	r2, r1, #1
	adds	r2, r2, r1
	lsls	r3, r2, #4
	subs	r3, r3, r2
	ldr	r2, [pc, #56]
	lsls	r3, r3, #3
	adds	r3, r3, r2
	strh	r3, [r4, #54]
.L2:
	ldr	r3, [r0, #0]
	adds	r2, r3, #1
	str	r2, [r0, #0]
	cmp	r2, #80
	bhi.n	.L3
	movs	r3, #180
	subs	r3, r3, r2
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl	Func_080c0cec
	b.n	.L4
.L3:
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	movs	r3, #100
	bl	Func_080c0cec
.L4:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ef8
	.4byte 0x03001ad0
	.4byte 0x0000af80

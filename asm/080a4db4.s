@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a4db4
	.thumb_func
Func_080a4db4:
	push	{r5, r6, r7, lr}
	sub	sp, #4
	adds	r6, r3, #0
	ldr	r3, [sp, #20]
	adds	r5, r0, #0
	str	r3, [sp, #0]
	movs	r1, #3
	adds	r3, r6, #0
	adds	r7, r2, #0
	bl	Func_080150b0
	movs	r2, #1
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L0
	negs	r3, r5
.L0:
	cmp	r3, #9
	ble.n	.L1
	movs	r2, #2
.L1:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L2
	negs	r3, r5
.L2:
	cmp	r3, #99
	ble.n	.L3
	movs	r2, #3
.L3:
	cmp	r5, #0
	ble.n	.L4
	lsls	r2, r2, #3
	subs	r2, r6, r2
	ldr	r0, [pc, #36]
	adds	r2, #16
	adds	r1, r7, #0
	ldr	r3, [sp, #20]
	bl	Func_08015098
	b.n	.L5
.L4:
	lsls	r2, r2, #3
	subs	r2, r6, r2
	ldr	r0, [pc, #24]
	adds	r2, #16
	adds	r1, r7, #0
	ldr	r3, [sp, #20]
	bl	Func_08015098
.L5:
	add	sp, #4
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080af224
	.4byte 0x080af228

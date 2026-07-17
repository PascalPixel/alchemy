@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08092a74
	.thumb_func
Func_08092a74:
	push	{lr}
	adds	r2, r0, #0
	movs	r0, #0
	cmp	r2, #0
	beq.n	.L0
	adds	r3, r2, #0
	adds	r3, #100
	ldrh	r3, [r3, #0]
	ldrh	r1, [r2, #6]
	subs	r3, r3, r1
	lsls	r3, r3, #16
	asrs	r0, r3, #16
	cmp	r0, #0
	beq.n	.L0
	movs	r3, #128
	lsls	r3, r3, #5
	cmp	r0, r3
	ble.n	.L1
	movs	r0, #128
	lsls	r0, r0, #4
.L1:
	ldr	r3, [pc, #12]
	cmp	r0, r3
	bge.n	.L2
	ldr	r0, [pc, #12]
.L2:
	adds	r3, r1, r0
	strh	r3, [r2, #6]
.L0:
	pop	{r1}
	bx	r1
	.4byte 0xfffff000
	.4byte 0xfffff800

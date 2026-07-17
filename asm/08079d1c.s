@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08079d1c
	.thumb_func
Func_08079d1c:
	push	{r5, r6, lr}
	ldr	r2, [pc, #84]
	adds	r5, r0, #0
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	movs	r0, #1
	cmp	r3, #0
	beq.n	.L0
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_0807882c
	adds	r6, r0, #0
	movs	r0, #1
	cmp	r6, #0
	beq.n	.L0
	ldrh	r3, [r6, #14]
	cmp	r3, #0
	beq.n	.L0
	adds	r0, r5, #0
	bl	Func_08079cbc
	ldrb	r2, [r6, #11]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	adds	r0, r0, r3
	movs	r1, #100
	lsls	r0, r0, #16
	bl	Func_080022ec
	adds	r5, r0, #0
	bl	Func_08079bc4
	ldr	r3, [pc, #24]
	ands	r0, r3
	cmp	r5, r0
	ble.n	.L1
	ldrh	r0, [r6, #14]
	b.n	.L0
.L1:
	movs	r0, #1
.L0:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000129
	.4byte 0x0000ffff

@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b27b0
	.thumb_func
Func_080b27b0:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	Func_08077008
	adds	r2, r0, #0
	movs	r0, #0
	cmp	r5, #0
	bne.n	.L0
	movs	r1, #56
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	ble.n	.L1
.L0:
	cmp	r5, #1
	bne.n	.L2
	ldr	r1, [pc, #56]
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L1
.L2:
	cmp	r5, #2
	bne.n	.L3
	movs	r1, #160
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L1
.L3:
	cmp	r5, #3
	bne.n	.L4
	movs	r1, #152
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L4
.L1:
	movs	r0, #1
.L4:
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000131

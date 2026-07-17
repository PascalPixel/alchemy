@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08093710
	.thumb_func
Func_08093710:
	push	{r5, r6, lr}
	ldr	r3, [pc, #76]
	ldr	r1, [pc, #76]
	movs	r0, #27
	ldr	r6, [r3, #0]
	bl	Func_080048f4
	movs	r2, #207
	lsls	r2, r2, #1
	adds	r3, r0, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	bne.n	.L0
	movs	r2, #214
	lsls	r2, r2, #2
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L0
	movs	r3, #214
	lsls	r3, r3, #2
	adds	r6, r6, r3
.L1:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r2, [pc, #28]
	adds	r5, #1
	cmp	r5, r2
	bgt.n	.L0
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	bne.n	.L1
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e70
	.4byte 0x00000ccc
	.4byte 0x0000012b

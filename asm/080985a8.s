@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080985a8
	.thumb_func
Func_080985a8:
	push	{r5, lr}
	ldr	r3, [pc, #76]
	ldr	r3, [r3, #0]
	ldr	r5, [r3, #20]
	cmp	r5, #0
	beq.n	.L0
	bl	Func_08098698
	adds	r0, r5, #0
	movs	r1, #2
	bl	Func_08009080
	adds	r2, r5, #0
	adds	r2, #89
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_080091e0
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r0, #10
	bl	Func_080030f8
	movs	r0, #126
	bl	Func_080f9010
	movs	r0, #40
	bl	Func_080030f8
	bl	Func_0809748c
.L0:
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x03001f30

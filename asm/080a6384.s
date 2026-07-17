@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a6384
	.thumb_func
Func_080a6384:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #76]
	ldr	r7, [r3, #0]
	bl	Func_08077008
	movs	r3, #228
	lsls	r3, r3, #1
	adds	r6, r7, r3
	adds	r1, r6, #0
	movs	r2, #2
	bl	Func_080a68ec
	movs	r3, #134
	lsls	r3, r3, #2
	adds	r5, r7, r3
	strb	r0, [r5, #0]
	ldr	r0, [r7, #32]
	bl	Func_08015270
	movs	r0, #108
	movs	r1, #32
	movs	r2, #8
	bl	Func_080a1bdc
	adds	r0, r6, #0
	bl	Func_080a68a8
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L0
	ldr	r0, [pc, #20]
	ldr	r1, [r7, #32]
	movs	r2, #0
	movs	r3, #24
	bl	Func_08015080
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x00000af2
	movs	r0, #1
	bx	lr

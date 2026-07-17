@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a3e88
	.thumb_func
Func_080a3e88:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #84]
	mov	r8, r1
	ldr	r7, [r3, #0]
	adds	r6, r0, #0
	bl	Func_08077008
	movs	r2, #228
	lsls	r2, r2, #1
	adds	r5, r7, r2
	adds	r1, r5, #0
	movs	r2, #0
	bl	Func_080a3ddc
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r3, r7, r2
	strb	r0, [r3, #0]
	ldr	r0, [r7, #32]
	bl	Func_08015270
	mov	r0, r8
	bl	Func_080a1cb0
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_080a3e28
	adds	r0, r6, #0
	bl	Func_080a3d6c
	cmp	r0, #0
	bne.n	.L0
	ldr	r0, [pc, #24]
	ldr	r1, [r7, #32]
	movs	r2, #8
	movs	r3, #24
	bl	Func_08015080
.L0:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
	.4byte 0x00000ad7
	bx	lr

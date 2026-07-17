@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08091ff0
	.thumb_func
Func_08091ff0:
	push	{r5, lr}
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #44]
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	adds	r3, r3, r2
	strh	r5, [r3, #0]
	movs	r2, #1
	lsls	r3, r5, #16
	asrs	r3, r3, #16
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L0
	ldr	r5, [pc, #28]
.L0:
	movs	r0, #149
	lsls	r0, r0, #1
	bl	Func_080f9010
	adds	r0, r5, #0
	bl	Func_080f9010
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
	.4byte 0x00000cc8
	.4byte 0x00000121

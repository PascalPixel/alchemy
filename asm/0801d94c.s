@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801d94c
	.thumb_func
Func_0801d94c:
	push	{r5, lr}
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #36]
	ldr	r5, [r3, #0]
	adds	r0, r5, r2
	bl	Func_080b0020
	ldr	r2, [pc, #32]
	adds	r3, r5, r2
	ldrh	r3, [r3, #0]
	adds	r2, #156
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r0, [r5, r3]
	bl	Func_080217a4
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ea0
	.4byte 0x000005a4
	.4byte 0x00000574

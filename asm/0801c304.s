@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0801c304
	.thumb_func
Func_0801c304:
	push	{r5, lr}
	ldr	r3, [pc, #60]
	ldr	r1, [pc, #60]
	ldr	r3, [r3, #0]
	adds	r2, r3, r1
	strh	r0, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r2, #1
	strh	r2, [r3, #0]
	bl	Func_0801a7f4
	bl	Func_0801b228
	movs	r1, #5
	movs	r0, #0
	bl	Func_0801b010
	bl	Func_0801a968
	movs	r0, #1
	bl	Func_0801b424
	adds	r5, r0, #0
	bl	Func_0801b148
	adds	r0, r5, #0
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e98
	.4byte 0x0000039e

@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a5534
	.thumb_func
Func_080a5534:
	push	{r5, lr}
	ldr	r3, [pc, #48]
	ldr	r5, [r3, #0]
	bl	Func_08004080
	ldr	r2, [pc, #44]
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	ldr	r2, [pc, #40]
	movs	r1, #128
	bl	Func_08003fa4
	bl	Func_08004080
	movs	r3, #229
	lsls	r3, r3, #2
	adds	r5, r5, r3
	strh	r0, [r5, #0]
	ldr	r2, [pc, #24]
	movs	r1, #128
	bl	Func_08003fa4
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x00000392
	.4byte 0x080aebcc
	.4byte 0x080aeb4c

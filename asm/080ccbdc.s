@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080ccbdc
	.thumb_func
Func_080ccbdc:
	push	{lr}
	ldr	r0, [pc, #44]
	bl	Func_08004278
	ldr	r0, [pc, #40]
	bl	Func_08004278
	movs	r1, #128
	ldr	r3, [pc, #36]
	lsls	r1, r1, #7
	ldr	r0, [pc, #36]
	bl	Func_080072f0
	ldr	r0, [pc, #36]
	bl	Func_08004278
	movs	r0, #40
	bl	Func_08002dd8
	movs	r0, #39
	bl	Func_08002dd8
	pop	{r0}
	bx	r0
	.4byte 0x080cc961
	.4byte 0x080cd261
	.4byte 0x03000164
	.4byte 0x06004000
	.4byte 0x080cd4b5

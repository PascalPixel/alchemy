@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08016738
	.thumb_func
Func_08016738:
	push	{lr}
	movs	r1, #240
	ldr	r3, [pc, #12]
	lsls	r1, r1, #4
	ldr	r2, [pc, #12]
	ldr	r0, [pc, #16]
	bl	Func_080072f0
	pop	{r1}
	bx	r1
	.4byte 0x03000168
	.4byte 0x44444444
	.4byte 0x06002500

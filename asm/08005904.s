@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08005904
	.thumb_func
Func_08005904:
	push	{lr}
	ldr	r3, [pc, #20]
	lsls	r0, r0, #16
	ldr	r3, [r3, #0]
	lsrs	r0, r0, #16
	bl	Func_080072f0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02004c14

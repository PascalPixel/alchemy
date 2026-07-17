@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0809ad70
	.thumb_func
Func_0809ad70:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	ldr	r5, [pc, #20]
	bl	Func_08004458
	lsls	r0, r0, #3
	lsrs	r0, r0, #16
	ldrsb	r1, [r5, r0]
	adds	r0, r6, #0
	bl	Func_08009240
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x0809f160

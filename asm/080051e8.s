@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080051e8
	.thumb_func
Func_080051e8:
	push	{r5, lr}
	sub	sp, #48
	mov	r5, sp
	adds	r2, r5, #0
	bl	Func_08004fe4
	ldr	r3, [pc, #12]
	adds	r0, r5, #0
	bl	Func_080072f0
	add	sp, #48
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x030002c0

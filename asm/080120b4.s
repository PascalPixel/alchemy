@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080120b4
	.thumb_func
Func_080120b4:
	push	{lr}
	cmp	r0, #0
	bge.n	.L0
	adds	r0, #15
.L0:
	asrs	r0, r0, #4
	cmp	r1, #0
	bge.n	.L1
	adds	r1, #15
.L1:
	asrs	r3, r1, #4
	lsls	r3, r3, #7
	adds	r3, r0, r3
	ldr	r2, [pc, #12]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrb	r0, [r3, #1]
	lsrs	r0, r0, #6
	pop	{r1}
	bx	r1
	.4byte 0x02010000

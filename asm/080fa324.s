@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080fa324
	.thumb_func
Func_080fa324:
	push	{lr}
	lsls	r0, r0, #16
	ldr	r2, [pc, #28]
	ldr	r1, [pc, #32]
	lsrs	r0, r0, #13
	adds	r0, r0, r1
	ldrh	r3, [r0, #4]
	lsls	r1, r3, #1
	adds	r1, r1, r3
	lsls	r1, r1, #2
	adds	r1, r1, r2
	ldr	r2, [r1, #0]
	ldr	r1, [r0, #0]
	adds	r0, r2, #0
	bl	Func_080faa58
	pop	{r0}
	bx	r0
	.4byte 0x080fc624
	.4byte 0x080fc684

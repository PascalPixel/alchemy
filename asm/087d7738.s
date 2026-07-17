@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_087d7738
	.thumb_func
Func_087d7738:
	push	{r4, r5, lr}
	lsrs	r2, r0, #32
	lsls	r7, r1, #5
	lsls	r0, r1, #4
	movs	r1, #84
	lsrs	r1, r0, #32
	lsls	r3, r7, #11
	asrs	r6, r0, #5
	bne.n	.L0
	lsls	r0, r5, #17
	lsls	r2, r3, #8
	add	r0, sp, #896
	lsrs	r4, r0, #32
.L0:
	lsls	r0, r0, #7
	lsls	r0, r4, #12
	strh	r2, [r3, r4]
	str	r2, [r7, #12]
	lsls	r4, r7, #6
	lsrs	r4, r7, #22
	bx	r0

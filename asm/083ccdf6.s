@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_083ccdf6
	.thumb_func
Func_083ccdf6:
	push	{r0, r1, lr}
	lsls	r0, r1, #4
	str	r7, [r4, r0]
	lsls	r5, r2, #4
	asrs	r7, r0, #10
	lsls	r1, r0, #4
	asrs	r3, r2, #20
	asrs	r7, r2, #24
	asrs	r0, r5, #1
	adds	r7, #96
	asrs	r6, r1, #12
	lsrs	r4, r2, #4
	adds	r1, #1
	asrs	r7, r2, #28
	bx	r3

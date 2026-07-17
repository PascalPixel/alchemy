@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_087e3c08
	.thumb_func
Func_087e3c08:
	push	{r0, r2, r6, r7, lr}
	add	r1, pc, #208
	bge.n	Func_087e3bde
	ldr	r0, [r4, #56]
	ldrsh	r3, [r4, r2]
	movs	r3, r6
	asrs	r6, r4, #21
	ldrb	r6, [r2, #13]
	ands	r5, r1
	ldrb	r7, [r0, r4]
	pop	{r0, r1, r5, r7, pc}

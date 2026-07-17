@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_087f14aa
	.thumb_func
Func_087f14aa:
	push	{r4, lr}
	movs	r0, #23
	lsls	r0, r0, #8
	lsls	r1, r4, #24
	lsls	r2, r4, #4
	lsls	r0, r6, #31
	lsls	r3, r7, #3
	pop	{r0}
	bx	r0
	ldr	r0, [pc, #0]
	bx	lr
	.4byte 0x02975c24

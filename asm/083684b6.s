@ Reconstructed thumb disassembly of a code-gap function the call-graph
@ discovery never reached. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_083684b6
	.thumb_func
Func_083684b6:
	push	{r0, r1, r4, r7, lr}
	ldrh	r1, [r6, #2]
	adds	r0, #45
	lsls	r6, r3, #22
	str	r4, [sp, #324]
	ldr	r6, [r2, #4]
	movs	r7, #163
	ldrb	r7, [r4, #16]
	movs	r4, #53
	blt.n	Func_083685a6
	pop	{r0, r2, r4, r7, pc}

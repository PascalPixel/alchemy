@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_083e7280
	.thumb_func
Func_083e7280:
	push	{r0, r1, r2, r5, r6, r7, lr}
	subs	r2, r0, #2
	movs	r0, #131
	movs	r1, r1
	ldr	r1, [sp, #176]
	adds	r2, #97
	stmia	r7!, {r1}
	mov	pc, r4

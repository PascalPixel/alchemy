@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080f037c
	.thumb_func
Func_080f037c:
	push	{lr}
	movs	r2, #128
	ldr	r1, [pc, #52]
	lsls	r2, r2, #9
	movs	r3, #31
.L0:
	subs	r3, #1
	stmia	r0!, {r1}
	cmp	r3, #0
	bge.n	.L0
	ldr	r4, [pc, #44]
	movs	r3, #239
.L1:
	subs	r3, #1
	stmia	r0!, {r2}
	adds	r2, r2, r4
	cmp	r3, #0
	bge.n	.L1
	movs	r3, #47
.L2:
	subs	r3, #1
	stmia	r0!, {r1}
	cmp	r3, #0
	bge.n	.L2
	movs	r2, #0
	movs	r3, #191
.L3:
	subs	r3, #1
	stmia	r0!, {r2}
	cmp	r3, #0
	bge.n	.L3
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x01ff01ff
	.4byte 0x00020002

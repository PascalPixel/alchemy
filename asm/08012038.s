@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08012038
	.thumb_func
Func_08012038:
	push	{r5, lr}
	ldr	r3, [pc, #52]
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	adds	r4, r2, #0
	asrs	r1, r1, #20
	asrs	r4, r4, #20
	ldr	r2, [pc, #44]
	cmp	r0, #0
	beq.n	.L0
	movs	r2, #3
	ands	r2, r5
	lsls	r3, r2, #1
	adds	r3, r3, r2
	movs	r2, #152
	lsls	r2, r2, #1
	lsls	r3, r3, #4
	adds	r3, r3, r2
	ldr	r2, [r0, r3]
.L0:
	lsls	r3, r4, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldrb	r0, [r2, #2]
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e70
	.4byte 0x02010000

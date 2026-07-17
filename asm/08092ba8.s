@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_08092ba8
	.thumb_func
Func_08092ba8:
	push	{lr}
	ldr	r3, [pc, #44]
	ldr	r2, [r3, #0]
	ldr	r3, [pc, #44]
	ands	r3, r0
	lsls	r3, r3, #2
	adds	r3, #20
	ldr	r2, [r2, r3]
	movs	r1, #1
	negs	r1, r1
	cmp	r2, #0
	beq.n	.L0
	adds	r3, r2, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L0
	ldr	r3, [r2, #80]
	ldr	r3, [r3, #40]
	movs	r2, #0
	ldrsh	r1, [r3, r2]
.L0:
	adds	r0, r1, #0
	pop	{r1}
	bx	r1
	.4byte 0x03001ebc
	.4byte 0x00000fff

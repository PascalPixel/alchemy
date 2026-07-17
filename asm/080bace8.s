@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080bace8
	.thumb_func
Func_080bace8:
	push	{r5, r6, r7, lr}
	sub	sp, #16
	adds	r6, r0, #0
	bl	Func_08077008
	movs	r5, #0
	adds	r7, r0, #0
	b.n	.L0
.L3:
	movs	r2, #149
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L1
	movs	r1, #4
	bl	Func_08009020
	b.n	.L2
.L1:
	movs	r1, #5
	bl	Func_08009020
.L2:
	adds	r5, #1
.L0:
	adds	r0, r6, #0
	bl	Func_080b7dd0
	adds	r1, r5, #0
	ldr	r0, [r0, #0]
	bl	Func_080b7f70
	cmp	r0, #0
	bne.n	.L3
	movs	r2, #149
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L4
	movs	r5, #0
	mov	r7, sp
	b.n	.L5
.L6:
	lsls	r3, r5, #2
	ldr	r2, [r0, #40]
	str	r0, [r7, r3]
	movs	r3, #6
	strb	r3, [r2, #5]
	movs	r3, #255
	strb	r3, [r2, #22]
	adds	r5, #1
.L5:
	adds	r0, r6, #0
	bl	Func_080b7dd0
	adds	r1, r5, #0
	ldr	r0, [r0, #0]
	bl	Func_080b7f70
	cmp	r0, #0
	bne.n	.L6
	movs	r0, #4
	bl	Func_080030f8
	adds	r0, r6, #0
	bl	Func_080bac6c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	Func_08009108
	adds	r0, r6, #0
	bl	Func_080b7e60
.L4:
	add	sp, #16
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0

@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b82c4
	.thumb_func
Func_080b82c4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r1, #0
	mov	r9, r2
	mov	fp, r3
	bl	Func_080b7dd0
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	Func_080b7dd0
	ldr	r7, [r5, #0]
	ldr	r6, [r0, #0]
	movs	r2, #75
	mov	r8, r2
	ldr	r3, [r6, #8]
	ldr	r2, [r7, #8]
	subs	r3, r3, r2
	mov	r0, r8
	muls	r0, r3
	movs	r1, #100
	mov	sl, r2
	bl	Func_080022ec
	ldr	r3, [r6, #16]
	ldr	r6, [r7, #16]
	subs	r3, r3, r6
	adds	r5, r0, #0
	movs	r1, #100
	mov	r0, r8
	muls	r0, r3
	bl	Func_080022ec
	mov	r3, sl
	adds	r3, r3, r5
	adds	r6, r6, r0
	asrs	r5, r5, #8
	asrs	r0, r0, #8
	adds	r2, r0, #0
	muls	r2, r0
	mov	r8, r3
	adds	r3, r5, #0
	muls	r3, r5
	adds	r3, r3, r2
	adds	r0, r3, #0
	ldr	r2, [pc, #96]
	bl	Func_080072ec
	mov	r1, r9
	lsls	r0, r0, #8
	bl	Func_080022ec
	adds	r3, r7, #0
	adds	r3, #88
	movs	r1, #1
	str	r0, [r7, #52]
	str	r0, [r7, #48]
	strb	r1, [r3, #0]
	subs	r3, #3
	ldrb	r2, [r3, #0]
	movs	r3, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	mov	r2, fp
	str	r2, [r7, #40]
.L0:
	mov	r3, fp
	str	r3, [r7, #40]
	ldr	r3, [pc, #56]
	str	r3, [r7, #72]
	adds	r3, r7, #0
	adds	r3, #90
	strb	r1, [r3, #0]
	adds	r0, r7, #0
	bl	Func_08009140
	adds	r0, r7, #0
	mov	r1, r8
	movs	r2, #0
	adds	r3, r6, #0
	bl	Func_08009150
	adds	r0, r7, #0
	movs	r1, #2
	bl	Func_08009080
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x030001d8
	.4byte 0x0000ab85

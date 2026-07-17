@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_0808ddec
	.thumb_func
Func_0808ddec:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #1
	sub	sp, #4
	negs	r1, r1
	movs	r2, #32
	mov	r9, r0
	str	r1, [sp, #0]
	mov	fp, r2
	bl	Func_0808ba1c
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L0
	movs	r3, #0
	mov	sl, r3
.L9:
	cmp	sl, r9
	beq.n	.L1
	mov	r0, sl
	bl	Func_0808ba1c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L1
	movs	r1, #89
	adds	r1, r1, r6
	ldrb	r2, [r1, #0]
	movs	r3, #8
	ands	r3, r2
	mov	r8, r1
	cmp	r3, #0
	bne.n	.L1
	ldr	r4, [r6, #12]
	ldr	r1, [r7, #12]
	subs	r3, r4, r1
	cmp	r3, #0
	blt.n	.L2
	ldr	r2, [pc, #196]
	cmp	r3, r2
	ble.n	.L3
	b.n	.L1
.L2:
	ldr	r2, [pc, #188]
	subs	r3, r1, r4
	cmp	r3, r2
	bgt.n	.L1
.L3:
	ldr	r2, [r6, #8]
	ldr	r3, [r7, #8]
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L4
	ldr	r3, [pc, #176]
	adds	r0, r0, r3
.L4:
	subs	r2, r4, r1
	asrs	r0, r0, #16
	cmp	r2, #0
	bge.n	.L5
	ldr	r1, [pc, #164]
	adds	r2, r2, r1
.L5:
	asrs	r1, r2, #16
	ldr	r3, [r7, #16]
	ldr	r2, [r6, #16]
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L6
	ldr	r3, [pc, #148]
	adds	r2, r2, r3
.L6:
	asrs	r3, r2, #16
	adds	r2, r0, #0
	muls	r2, r0
	adds	r0, r2, #0
	adds	r2, r1, #0
	muls	r2, r1
	adds	r1, r3, #0
	muls	r1, r3
	adds	r0, r0, r2
	adds	r3, r1, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #124]
	bl	Func_080072f0
	mov	r3, r8
	ldrb	r2, [r3, #0]
	movs	r3, #4
	ands	r3, r2
	adds	r5, r0, #0
	cmp	r3, #0
	beq.n	.L7
	lsls	r0, r5, #2
	adds	r0, r0, r5
	lsls	r0, r0, #1
	movs	r1, #13
	bl	Func_080022ec
	adds	r5, r0, #0
.L7:
	cmp	r5, fp
	bge.n	.L1
	ldr	r3, [r7, #16]
	ldr	r0, [r6, #16]
	ldr	r1, [r6, #8]
	subs	r0, r0, r3
	ldr	r3, [r7, #8]
	subs	r1, r1, r3
	bl	Func_080044d0
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	cmp	r5, #11
	ble.n	.L8
	ldrh	r3, [r7, #6]
	subs	r3, r0, r3
	lsls	r3, r3, #16
	ldr	r1, [pc, #60]
	asrs	r0, r3, #16
	cmp	r0, r1
	blt.n	.L1
	ldr	r2, [pc, #56]
	cmp	r0, r2
	bgt.n	.L1
.L8:
	mov	r3, sl
	str	r3, [sp, #0]
	mov	fp, r5
.L1:
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #66
	ble.n	.L9
.L0:
	ldr	r0, [sp, #0]
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x002fffff
	.4byte 0x0000ffff
	.4byte 0x030001d8
	.4byte 0xffffd001
	.4byte 0x00002fff

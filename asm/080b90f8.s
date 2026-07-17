@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080b90f8
	.thumb_func
Func_080b90f8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #0
	sub	sp, #36
	str	r2, [sp, #4]
	ldr	r3, [pc, #232]
	ldr	r2, [r3, #0]
	adds	r3, r2, #0
	adds	r3, #69
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L0
	movs	r3, #1
	str	r3, [sp, #4]
	b.n	.L1
.L0:
	adds	r2, #70
	str	r2, [sp, #0]
	ldrb	r2, [r2, #0]
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r2, [pc, #204]
	lsls	r3, r3, #4
	adds	r2, r2, r3
	add	r3, sp, #8
	mov	sl, r3
	movs	r0, #1
	mov	r1, sl
	mov	r9, r2
	bl	Func_080b6b40
	ldr	r2, [sp, #4]
	mov	r8, r0
	movs	r6, #0
	cmp	r2, r8
	bge.n	.L2
	mov	fp, sl
	movs	r7, #0
	mov	r5, r8
.L3:
	mov	r3, fp
	ldrsh	r0, [r7, r3]
	bl	Func_08077008
	ldrb	r3, [r0, #15]
	subs	r5, #1
	adds	r6, r6, r3
	adds	r7, #2
	cmp	r5, #0
	bne.n	.L3
.L2:
	lsls	r0, r6, #5
	subs	r0, r0, r6
	lsls	r0, r0, #2
	adds	r0, r0, r6
	mov	r1, r8
	lsls	r0, r0, #2
	bl	Func_080022ec
	mov	r1, sl
	add	r9, r0
	movs	r0, #2
	bl	Func_080b6b40
	movs	r6, #0
	mov	r8, r0
	cmp	r6, r8
	bge.n	.L4
	movs	r7, #0
	mov	r5, r8
.L5:
	mov	r3, sl
	ldrsh	r0, [r7, r3]
	bl	Func_08077008
	ldrb	r3, [r0, #15]
	subs	r5, #1
	adds	r6, r6, r3
	adds	r7, #2
	cmp	r5, #0
	bne.n	.L5
.L4:
	lsls	r0, r6, #5
	subs	r0, r0, r6
	lsls	r0, r0, #2
	adds	r0, r0, r6
	lsls	r0, r0, #2
	mov	r1, r8
	bl	Func_080022ec
	mov	r3, r9
	subs	r3, r3, r0
	mov	r9, r3
	cmp	r3, #0
	ble.n	.L6
	bl	Func_08004458
	ldr	r3, [pc, #64]
	muls	r3, r0
	lsrs	r3, r3, #16
	cmp	r3, r9
	bcs.n	.L6
	movs	r2, #1
	str	r2, [sp, #4]
.L6:
	ldr	r2, [sp, #0]
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
.L1:
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #48]
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #2
	bne.n	.L7
	movs	r3, #0
	str	r3, [sp, #4]
.L7:
	ldr	r0, [sp, #4]
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e74
	.4byte 0x00001388
	.4byte 0x00002710
	.4byte 0x02000240
	.4byte 0x0000022b

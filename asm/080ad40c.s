@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080ad40c
	.thumb_func
Func_080ad40c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #228]
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #228]
	sub	sp, #36
	mov	r8, r3
	bl	Func_08015418
	movs	r0, #0
	str	r0, [sp, #8]
	movs	r0, #137
	movs	r3, #162
	lsls	r0, r0, #2
	movs	r1, #12
	movs	r7, #141
	movs	r2, #145
	lsls	r3, r3, #1
	add	r0, r8
	add	r1, sp
	lsls	r7, r7, #2
	lsls	r2, r2, #2
	str	r3, [sp, #4]
	str	r0, [sp, #0]
	mov	fp, r1
	add	r7, r8
	mov	r9, r2
.L4:
	ldr	r2, [sp, #0]
	ldmia	r2!, {r6}
	adds	r1, r2, #0
	str	r1, [sp, #0]
	cmp	r6, #0
	beq.n	.L0
	ldr	r0, [sp, #4]
	mov	r2, r8
	ldrsh	r3, [r0, r2]
	movs	r2, #241
	lsls	r3, r3, #16
	lsls	r2, r2, #17
	movs	r0, #13
	subs	r2, r2, r3
	negs	r0, r0
	ldrb	r3, [r6, #9]
	mov	sl, r2
	adds	r2, r0, #0
	ands	r3, r2
	strb	r3, [r6, #9]
	mov	r1, r9
	mov	r2, r8
	ldr	r5, [r1, r2]
	cmp	r5, #0
	bge.n	.L1
	negs	r3, r5
	mov	r4, fp
	str	r3, [sp, #12]
	str	r3, [r4, #4]
	b.n	.L2
.L1:
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #3
	subs	r0, r0, r5
	bl	Func_080022ec
	mov	r4, fp
	adds	r0, r5, r0
	str	r0, [sp, #12]
	str	r0, [r4, #4]
	ldr	r3, [sp, #12]
	mov	r0, r8
	mov	r1, r9
	str	r3, [r0, r1]
.L2:
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	add	r1, sp, #20
	lsls	r3, r3, #16
	str	r3, [r1, #0]
	mov	r3, sl
	str	r3, [r1, #4]
	movs	r0, #8
	ldrsh	r3, [r7, r0]
	lsls	r3, r3, #16
	add	r3, sl
	str	r3, [r1, #8]
	movs	r3, #0
	str	r3, [r1, #12]
	movs	r3, #8
	ldrsh	r2, [r7, r3]
	movs	r3, #128
	lsls	r3, r3, #8
	cmp	r2, #0
	blt.n	.L3
	movs	r3, #128
	lsls	r3, r3, #7
.L3:
	adds	r0, r6, #0
	adds	r2, r4, #0
	bl	Func_08009008
.L0:
	ldr	r1, [sp, #4]
	ldr	r2, [sp, #8]
	movs	r0, #4
	adds	r1, #2
	adds	r2, #1
	adds	r7, #2
	add	r9, r0
	str	r1, [sp, #4]
	str	r2, [sp, #8]
	cmp	r2, #3
	ble.n	.L4
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
	.4byte 0x06002500

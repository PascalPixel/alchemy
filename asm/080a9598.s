@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080a9598
	.thumb_func
Func_080a9598:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #280]
	ldr	r3, [r3, #0]
	mov	r9, r3
	ldr	r3, [pc, #276]
	add	r3, r9
	adds	r7, r0, #0
	ldrb	r0, [r3, #0]
	adds	r5, r2, #0
	sub	sp, #4
	bl	Func_08077008
	movs	r3, #96
	str	r3, [sp, #0]
	movs	r2, #8
	movs	r3, #224
	mov	fp, r0
	movs	r1, #128
	adds	r0, r7, #0
	bl	Func_08015068
	ldr	r2, [r5, #8]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	mov	r8, r3
	ldr	r3, [r5, #20]
	mov	r2, r8
	subs	r3, r3, r2
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	sl, r3
	cmp	r3, #5
	bls.n	.L0
	movs	r3, #5
	mov	sl, r3
.L0:
	movs	r3, #52
	str	r3, [sp, #0]
	movs	r0, #5
	mov	r1, r8
	adds	r2, r7, #0
	movs	r3, #119
	bl	Func_080a2324
	movs	r2, #28
	ldr	r3, [r5, #8]
	ldr	r1, [r5, #20]
	adds	r0, r7, #0
	str	r2, [sp, #0]
	movs	r2, #5
	bl	Func_080a21b0
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L1
	ldr	r0, [pc, #176]
	adds	r1, r7, #0
	movs	r2, #120
	movs	r3, #8
	bl	Func_08015080
	b.n	.L2
.L1:
	mov	r2, sl
	movs	r6, #0
	cmp	r2, #0
	bls.n	.L2
	mov	r2, r8
	lsls	r3, r2, #1
	movs	r2, #228
	add	r3, r9
	lsls	r2, r2, #1
	adds	r5, r3, r2
	ldr	r3, [pc, #144]
	mov	r8, r3
.L3:
	ldrh	r3, [r5, #0]
	mov	r0, r8
	ands	r0, r3
	ldr	r3, [pc, #140]
	adds	r0, r0, r3
	lsls	r3, r6, #4
	adds	r3, #8
	adds	r1, r7, #0
	movs	r2, #128
	bl	Func_08015080
	adds	r3, r6, #1
	lsls	r3, r3, #24
	lsrs	r6, r3, #24
	adds	r5, #2
	cmp	sl, r6
	bhi.n	.L3
.L2:
	mov	r0, fp
	adds	r1, r7, #0
	movs	r2, #40
	movs	r3, #0
	bl	Func_08015090
	ldr	r5, [pc, #104]
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #32
	movs	r3, #16
	adds	r5, #1
	bl	Func_08015080
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #32
	movs	r3, #24
	bl	Func_08015080
	mov	r2, fp
	movs	r3, #16
	ldrh	r0, [r2, #60]
	movs	r1, #3
	str	r3, [sp, #0]
	adds	r2, r7, #0
	movs	r3, #80
	bl	Func_080150b0
	mov	r3, fp
	ldrh	r0, [r3, #62]
	movs	r3, #24
	str	r3, [sp, #0]
	movs	r1, #3
	adds	r2, r7, #0
	movs	r3, #80
	bl	Func_080150b0
	movs	r0, #1
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
	.4byte 0x03001f2c
	.4byte 0x0000021a
	.4byte 0x00000ad7
	.4byte 0x000001ff
	.4byte 0x00000182
	.4byte 0x00000af7

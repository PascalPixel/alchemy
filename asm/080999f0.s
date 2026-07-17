@ Reconstructed thumb disassembly of a code-gap function, bounded by a
@ control-flow walk. Verified byte-identical by build_asm.py.
.syntax unified
	.thumb
	.global Func_080999f0
	.thumb_func
Func_080999f0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #772]
	ldr	r6, [r3, #0]
	ldr	r0, [r6, #16]
	movs	r1, #0
	mov	sl, r0
	movs	r2, #0
	movs	r0, #239
	movs	r3, #0
	sub	sp, #44
	mov	r8, r1
	bl	Func_08096c80
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L0
	b.n	.L1
.L0:
	bl	Func_08097384
	movs	r0, #138
	bl	Func_080f9010
	ldr	r3, [r6, #20]
	cmp	r3, #0
	bne.n	.L2
	mov	r2, sl
	ldr	r3, [r2, #8]
	str	r3, [r6, #4]
	ldr	r3, [r2, #16]
	str	r3, [r6, #12]
	adds	r5, r6, #0
	ldmia	r5!, {r1}
	movs	r0, #128
	lsls	r0, r0, #13
	adds	r2, r5, #0
	bl	Func_0800447c
	ldr	r1, [r5, #0]
	ldr	r2, [r6, #12]
	movs	r0, #0
	bl	Func_080091a8
	str	r0, [r6, #8]
.L2:
	mov	r3, sp
	adds	r3, #20
	str	r3, [sp, #4]
	mov	r0, sl
	ldr	r1, [sp, #4]
	ldr	r3, [r0, #8]
	str	r3, [r1, #0]
	movs	r2, #128
	ldr	r3, [r0, #12]
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r1, #4]
	ldr	r3, [r0, #16]
	str	r3, [r1, #8]
	add	r3, sp, #8
	mov	fp, r3
	ldr	r3, [r6, #4]
	mov	r0, fp
	str	r3, [r0, #0]
	movs	r1, #128
	ldr	r2, [r6, #8]
	lsls	r1, r1, #14
	adds	r3, r2, r1
	str	r3, [r0, #4]
	ldr	r3, [r6, #12]
	str	r3, [r0, #8]
	adds	r3, r6, #0
	adds	r3, #52
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L3
	movs	r0, #160
	lsls	r0, r0, #15
	adds	r3, r2, r0
	mov	r1, fp
	str	r3, [r1, #4]
.L3:
	ldr	r2, [sp, #4]
	mov	sl, fp
	mov	r9, r2
.L4:
	mov	r0, sl
	mov	r1, r9
	ldr	r5, [r1, #0]
	ldr	r3, [r0, #0]
	subs	r3, r3, r5
	mov	r0, r8
	muls	r0, r3
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r5, r0
	str	r5, [r7, #8]
	mov	r2, sl
	mov	r0, r9
	ldr	r3, [r2, #4]
	ldr	r5, [r0, #4]
	subs	r3, r3, r5
	mov	r0, r8
	muls	r0, r3
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r5, r0
	str	r5, [r7, #12]
	mov	r2, r9
	mov	r1, sl
	ldr	r5, [r2, #8]
	ldr	r3, [r1, #8]
	subs	r3, r3, r5
	mov	r0, r8
	muls	r0, r3
	movs	r1, #10
	bl	Func_080022ec
	movs	r3, #192
	lsls	r3, r3, #8
	adds	r5, r5, r0
	movs	r1, #10
	mov	r0, r8
	muls	r0, r3
	str	r5, [r7, #16]
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r0, r0, r3
	str	r0, [r7, #24]
	str	r0, [r7, #28]
	movs	r0, #1
	bl	Func_080030f8
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #11
	blt.n	.L4
	movs	r0, #10
	bl	Func_080030f8
	adds	r3, r6, #0
	adds	r3, #69
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L5
	adds	r3, r6, #0
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r2, #10
	mov	r9, r2
	cmp	r3, #0
	bne.n	.L6
	movs	r3, #24
	mov	r9, r3
.L6:
	movs	r0, #0
	mov	r8, r0
	cmp	r8, r9
	bge.n	.L7
	mov	r1, r9
	subs	r1, #1
	add	r6, sp, #32
	str	r1, [sp, #0]
	mov	sl, r6
.L10:
	ldr	r3, [r7, #8]
	mov	r2, sl
	str	r3, [r2, #0]
	ldr	r3, [r7, #12]
	str	r3, [r2, #4]
	ldr	r3, [r7, #16]
	str	r3, [r2, #8]
	bl	Func_08004458
	movs	r3, #192
	lsls	r5, r0, #2
	lsls	r3, r3, #10
	adds	r5, r5, r0
	adds	r5, r5, r3
	bl	Func_08004458
	mov	r2, sl
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_0800447c
	ldr	r0, [sp, #0]
	cmp	r8, r0
	bne.n	.L8
	movs	r0, #25
	bl	Func_080030f8
	ldr	r3, [r7, #8]
	str	r3, [r6, #0]
	ldr	r3, [r7, #12]
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	str	r3, [r6, #8]
.L8:
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	movs	r0, #240
	bl	Func_08096c80
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L9
	ldr	r3, [r6, #4]
	ldr	r1, [pc, #352]
	adds	r3, r3, r1
	str	r3, [r5, #20]
	ldr	r3, [pc, #348]
	adds	r2, r5, #0
	str	r3, [r5, #108]
	adds	r2, #85
	movs	r3, #2
	strb	r3, [r2, #0]
.L9:
	movs	r0, #132
	bl	Func_080f9010
	movs	r0, #6
	bl	Func_080030f8
	movs	r2, #1
	add	r8, r2
	cmp	r8, r9
	blt.n	.L10
.L7:
	movs	r0, #10
	bl	Func_080030f8
	b.n	.L11
.L5:
	adds	r3, r6, #0
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r0, #10
	mov	r9, r0
	cmp	r3, #0
	bne.n	.L12
	movs	r1, #30
	mov	r9, r1
.L12:
	mov	r2, r9
	cmp	r2, #0
	beq.n	.L13
	add	r6, sp, #32
	mov	r8, r9
.L15:
	ldr	r3, [r7, #8]
	str	r3, [r6, #0]
	ldr	r3, [r7, #12]
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	str	r3, [r6, #8]
	bl	Func_08004458
	movs	r3, #192
	lsls	r5, r0, #2
	lsls	r3, r3, #10
	adds	r5, r5, r0
	adds	r5, r5, r3
	bl	Func_08004458
	adds	r2, r6, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_0800447c
	movs	r0, #142
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	lsls	r0, r0, #1
	bl	Func_08096c80
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L14
	ldr	r3, [pc, #220]
	adds	r2, r5, #0
	str	r3, [r5, #108]
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r1, [r5, #80]
	movs	r0, #13
	ldrb	r2, [r1, #9]
	negs	r0, r0
	adds	r3, r0, #0
	ands	r2, r3
	movs	r3, #8
	orrs	r2, r3
	strb	r2, [r1, #9]
	adds	r0, r5, #0
	movs	r1, #8
	bl	Func_08009080
	adds	r0, r5, #0
	movs	r1, #7
	bl	Func_08009240
.L14:
	movs	r0, #6
	bl	Func_080030f8
	movs	r1, #1
	negs	r1, r1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L15
.L13:
	movs	r0, #70
	bl	Func_080030f8
.L11:
	movs	r3, #0
	ldr	r6, [sp, #4]
	mov	r8, r3
	mov	sl, fp
.L16:
	mov	r0, sl
	ldr	r5, [r0, #0]
	ldr	r3, [r6, #0]
	subs	r3, r3, r5
	mov	r0, r8
	muls	r0, r3
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r5, r0
	str	r5, [r7, #8]
	mov	r1, sl
	ldr	r5, [r1, #4]
	ldr	r3, [r6, #4]
	subs	r3, r3, r5
	mov	r0, r8
	muls	r0, r3
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r5, r0
	str	r5, [r7, #12]
	mov	r2, sl
	ldr	r5, [r2, #8]
	ldr	r3, [r6, #8]
	subs	r3, r3, r5
	mov	r0, r8
	muls	r0, r3
	movs	r1, #10
	bl	Func_080022ec
	ldr	r3, [pc, #84]
	adds	r5, r5, r0
	movs	r1, #10
	mov	r0, r8
	muls	r0, r3
	str	r5, [r7, #16]
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #9
	adds	r0, r0, r3
	str	r0, [r7, #24]
	str	r0, [r7, #28]
	movs	r0, #1
	bl	Func_080030f8
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #11
	blt.n	.L16
	adds	r0, r7, #0
	bl	Func_080090d0
	bl	Func_0809748c
.L1:
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f30
	.4byte 0xffe00000
	.4byte 0x08099921
	.4byte 0x080999a9
	.4byte 0xffff4000

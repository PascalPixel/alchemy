.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080030f8, 0x080030f8
	.set sub_08004458, 0x08004458
	.set sub_0800447c, 0x0800447c
	.set sub_08009080, 0x08009080
	.set sub_080090d0, 0x080090d0
	.set sub_080091a8, 0x080091a8
	.set sub_08009240, 0x08009240
	.set sub_08096c80, 0x08096c80
	.set sub_08097384, 0x08097384
	.set sub_0809748c, 0x0809748c
	.set sub_080f9010, 0x080f9010
	.global Overlay_080999f0
Overlay_080999f0:
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
	bl	sub_08096c80
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_08099a1e
	b.n	.L_08099cf0
.L_08099a1e:
	bl	sub_08097384
	movs	r0, #138
	bl	sub_080f9010
	ldr	r3, [r6, #20]
	cmp	r3, #0
	bne.n	.L_08099a52
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
	bl	sub_0800447c
	ldr	r1, [r5, #0]
	ldr	r2, [r6, #12]
	movs	r0, #0
	bl	sub_080091a8
	str	r0, [r6, #8]
.L_08099a52:
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
	beq.n	.L_08099a9e
	movs	r0, #160
	lsls	r0, r0, #15
	adds	r3, r2, r0
	mov	r1, fp
	str	r3, [r1, #4]
.L_08099a9e:
	ldr	r2, [sp, #4]
	mov	sl, fp
	mov	r9, r2
.L_08099aa4:
	mov	r0, sl
	mov	r1, r9
	ldr	r5, [r1, #0]
	ldr	r3, [r0, #0]
	subs	r3, r3, r5
	mov	r0, r8
	muls	r0, r3
	movs	r1, #10
	bl	sub_080022ec
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
	bl	sub_080022ec
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
	bl	sub_080022ec
	movs	r3, #192
	lsls	r3, r3, #8
	adds	r5, r5, r0
	movs	r1, #10
	mov	r0, r8
	muls	r0, r3
	str	r5, [r7, #16]
	bl	sub_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r0, r0, r3
	str	r0, [r7, #24]
	str	r0, [r7, #28]
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #11
	blt.n	.L_08099aa4
	movs	r0, #10
	bl	sub_080030f8
	adds	r3, r6, #0
	adds	r3, #69
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_08099bd4
	adds	r3, r6, #0
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r2, #10
	mov	r9, r2
	cmp	r3, #0
	bne.n	.L_08099b3e
	movs	r3, #24
	mov	r9, r3
.L_08099b3e:
	movs	r0, #0
	mov	r8, r0
	cmp	r8, r9
	bge.n	.L_08099bcc
	mov	r1, r9
	subs	r1, #1
	add	r6, sp, #32
	str	r1, [sp, #0]
	mov	sl, r6
.L_08099b50:
	ldr	r3, [r7, #8]
	mov	r2, sl
	str	r3, [r2, #0]
	ldr	r3, [r7, #12]
	str	r3, [r2, #4]
	ldr	r3, [r7, #16]
	str	r3, [r2, #8]
	bl	sub_08004458
	movs	r3, #192
	lsls	r5, r0, #2
	lsls	r3, r3, #10
	adds	r5, r5, r0
	adds	r5, r5, r3
	bl	sub_08004458
	mov	r2, sl
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_0800447c
	ldr	r0, [sp, #0]
	cmp	r8, r0
	bne.n	.L_08099b92
	movs	r0, #25
	bl	sub_080030f8
	ldr	r3, [r7, #8]
	str	r3, [r6, #0]
	ldr	r3, [r7, #12]
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	str	r3, [r6, #8]
.L_08099b92:
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	movs	r0, #240
	bl	sub_08096c80
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_08099bb8
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
.L_08099bb8:
	movs	r0, #132
	bl	sub_080f9010
	movs	r0, #6
	bl	sub_080030f8
	movs	r2, #1
	add	r8, r2
	cmp	r8, r9
	blt.n	.L_08099b50
.L_08099bcc:
	movs	r0, #10
	bl	sub_080030f8
	b.n	.L_08099c76
.L_08099bd4:
	adds	r3, r6, #0
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r0, #10
	mov	r9, r0
	cmp	r3, #0
	bne.n	.L_08099bea
	movs	r1, #30
	mov	r9, r1
.L_08099bea:
	mov	r2, r9
	cmp	r2, #0
	beq.n	.L_08099c70
	add	r6, sp, #32
	mov	r8, r9
.L_08099bf4:
	ldr	r3, [r7, #8]
	str	r3, [r6, #0]
	ldr	r3, [r7, #12]
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	str	r3, [r6, #8]
	bl	sub_08004458
	movs	r3, #192
	lsls	r5, r0, #2
	lsls	r3, r3, #10
	adds	r5, r5, r0
	adds	r5, r5, r3
	bl	sub_08004458
	adds	r2, r6, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_0800447c
	movs	r0, #142
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	lsls	r0, r0, #1
	bl	sub_08096c80
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_08099c5e
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
	bl	sub_08009080
	adds	r0, r5, #0
	movs	r1, #7
	bl	sub_08009240
.L_08099c5e:
	movs	r0, #6
	bl	sub_080030f8
	movs	r1, #1
	negs	r1, r1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_08099bf4
.L_08099c70:
	movs	r0, #70
	bl	sub_080030f8
.L_08099c76:
	movs	r3, #0
	ldr	r6, [sp, #4]
	mov	r8, r3
	mov	sl, fp
.L_08099c7e:
	mov	r0, sl
	ldr	r5, [r0, #0]
	ldr	r3, [r6, #0]
	subs	r3, r3, r5
	mov	r0, r8
	muls	r0, r3
	movs	r1, #10
	bl	sub_080022ec
	adds	r5, r5, r0
	str	r5, [r7, #8]
	mov	r1, sl
	ldr	r5, [r1, #4]
	ldr	r3, [r6, #4]
	subs	r3, r3, r5
	mov	r0, r8
	muls	r0, r3
	movs	r1, #10
	bl	sub_080022ec
	adds	r5, r5, r0
	str	r5, [r7, #12]
	mov	r2, sl
	ldr	r5, [r2, #8]
	ldr	r3, [r6, #8]
	subs	r3, r3, r5
	mov	r0, r8
	muls	r0, r3
	movs	r1, #10
	bl	sub_080022ec
	ldr	r3, [pc, #84]
	adds	r5, r5, r0
	movs	r1, #10
	mov	r0, r8
	muls	r0, r3
	str	r5, [r7, #16]
	bl	sub_080022ec
	movs	r3, #128
	lsls	r3, r3, #9
	adds	r0, r0, r3
	str	r0, [r7, #24]
	str	r0, [r7, #28]
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #11
	blt.n	.L_08099c7e
	adds	r0, r7, #0
	bl	sub_080090d0
	bl	sub_0809748c
.L_08099cf0:
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

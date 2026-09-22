.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080030f8, 0x080030f8
	.set sub_0800447c, 0x0800447c
	.set sub_08009080, 0x08009080
	.set sub_080090d0, 0x080090d0
	.set sub_080090f0, 0x080090f0
	.set sub_08009240, 0x08009240
	.set sub_08096c80, 0x08096c80
	.set sub_08097384, 0x08097384
	.set sub_0809748c, 0x0809748c
	.set sub_080f9010, 0x080f9010
	.global Overlay_0809ae64
Overlay_0809ae64:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #76]
	ldr	r5, [r3, #0]
	ldr	r1, [r5, #20]
	ldr	r7, [r5, #16]
	sub	sp, #40
	str	r1, [sp, #0]
	ldr	r3, [r7, #8]
	add	r2, sp, #16
	str	r3, [r2, #0]
	ldr	r3, [r7, #12]
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r2, #4]
	ldr	r3, [r7, #16]
	str	r3, [r2, #8]
	adds	r3, r5, #0
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	fp, r2
	cmp	r3, #0
	beq.n	.L_0809aec4
	ldr	r3, [r7, #8]
	add	r2, sp, #4
	str	r3, [r2, #0]
	ldr	r3, [r7, #12]
	movs	r0, #128
	lsls	r0, r0, #14
	adds	r3, r3, r0
	str	r3, [r2, #4]
	ldr	r3, [r7, #16]
	str	r3, [r2, #8]
	ldr	r1, [r5, #0]
	mov	r9, r2
	bl	sub_0800447c
	b.n	.L_0809aedc
	.2byte 0x1f30
	.2byte 0x0300
.L_0809aec4:
	add	r3, sp, #4
	mov	r9, r3
	ldr	r3, [r5, #4]
	mov	r1, r9
	str	r3, [r1, #0]
	movs	r2, #128
	ldr	r3, [r5, #8]
	lsls	r2, r2, #14
	adds	r3, r3, r2
	str	r3, [r1, #4]
	ldr	r3, [r5, #12]
	str	r3, [r1, #8]
.L_0809aedc:
	ldr	r1, [r5, #4]
	add	r0, sp, #28
	str	r1, [r0, #0]
	movs	r3, #128
	ldr	r2, [r5, #8]
	lsls	r3, r3, #14
	adds	r2, r2, r3
	str	r2, [r0, #4]
	ldr	r3, [r5, #12]
	str	r3, [r0, #8]
	movs	r0, #215
	bl	sub_08096c80
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_0809aefe
	b.n	.L_0809b092
.L_0809aefe:
	bl	sub_08097384
	movs	r0, #138
	bl	sub_080f9010
	ldrh	r3, [r7, #6]
	strh	r3, [r6, #6]
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #32]
	str	r3, [r6, #48]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #5
	bl	sub_08009080
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08009240
	movs	r7, #0
	mov	sl, fp
	mov	r8, r9
	b.n	.L_0809af38
	.4byte 0x00000000
	.2byte 0x4ccc
	.2byte 0x0001
.L_0809af38:
	mov	r2, sl
	mov	r1, r8
	ldr	r5, [r2, #0]
	ldr	r3, [r1, #0]
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	movs	r1, #10
	bl	sub_080022ec
	adds	r5, r5, r0
	str	r5, [r6, #8]
	mov	r2, sl
	mov	r1, r8
	ldr	r5, [r2, #4]
	ldr	r3, [r1, #4]
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	movs	r1, #10
	bl	sub_080022ec
	adds	r5, r5, r0
	str	r5, [r6, #12]
	mov	r2, sl
	mov	r1, r8
	ldr	r5, [r2, #8]
	ldr	r3, [r1, #8]
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	movs	r1, #10
	bl	sub_080022ec
	movs	r3, #192
	lsls	r3, r3, #8
	adds	r5, r5, r0
	movs	r1, #10
	adds	r0, r7, #0
	muls	r0, r3
	str	r5, [r6, #16]
	bl	sub_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r0, r0, r3
	str	r0, [r6, #24]
	str	r0, [r6, #28]
	adds	r7, #1
	movs	r0, #1
	bl	sub_080030f8
	cmp	r7, #11
	blt.n	.L_0809af38
	movs	r0, #10
	bl	sub_080030f8
	adds	r0, r6, #0
	movs	r1, #6
	bl	sub_08009080
	movs	r0, #15
	bl	sub_080030f8
	movs	r5, #9
.L_0809afba:
	ldr	r3, [r6, #12]
	ldr	r1, [pc, #228]
	adds	r3, r3, r1
	str	r3, [r6, #12]
	movs	r0, #1
	subs	r5, #1
	bl	sub_080030f8
	cmp	r5, #0
	bge.n	.L_0809afba
	adds	r0, r6, #0
	movs	r1, #5
	bl	sub_08009080
	movs	r0, #132
	bl	sub_080f9010
	ldr	r2, [sp, #0]
	cmp	r2, #0
	beq.n	.L_0809afee
	ldr	r3, [pc, #196]
	ldr	r2, [r2, #12]
	ldr	r0, [sp, #0]
	adds	r1, r3, #0
	bl	sub_080090f0
.L_0809afee:
	movs	r0, #20
	bl	sub_080030f8
	movs	r5, #12
.L_0809aff6:
	ldr	r3, [r6, #12]
	movs	r1, #192
	lsls	r1, r1, #9
	adds	r3, r3, r1
	str	r3, [r6, #12]
	movs	r0, #1
	subs	r5, #1
	bl	sub_080030f8
	cmp	r5, #0
	bge.n	.L_0809aff6
	movs	r0, #10
	bl	sub_080030f8
	movs	r0, #114
	bl	sub_080f9010
	movs	r7, #0
	mov	sl, r9
	mov	r8, fp
.L_0809b01e:
	mov	r2, r8
	mov	r1, sl
	ldr	r3, [r2, #0]
	ldr	r5, [r1, #0]
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	movs	r1, #10
	bl	sub_080022ec
	adds	r5, r5, r0
	str	r5, [r6, #8]
	mov	r2, r8
	mov	r1, sl
	ldr	r3, [r2, #4]
	ldr	r5, [r1, #4]
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	movs	r1, #10
	bl	sub_080022ec
	adds	r5, r5, r0
	str	r5, [r6, #12]
	mov	r2, r8
	mov	r1, sl
	ldr	r3, [r2, #8]
	ldr	r5, [r1, #8]
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	movs	r1, #10
	bl	sub_080022ec
	ldr	r3, [pc, #72]
	adds	r5, r5, r0
	movs	r1, #10
	adds	r0, r7, #0
	muls	r0, r3
	str	r5, [r6, #16]
	bl	sub_080022ec
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r0, r0, r2
	str	r0, [r6, #24]
	str	r0, [r6, #28]
	adds	r7, #1
	movs	r0, #1
	bl	sub_080030f8
	cmp	r7, #11
	blt.n	.L_0809b01e
	adds	r0, r6, #0
	bl	sub_080090d0
	bl	sub_0809748c
.L_0809b092:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0xfffe0000
	.4byte 0xfff70000
	.4byte 0xffff4000

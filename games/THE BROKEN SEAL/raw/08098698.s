.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08004458, 0x08004458
	.set sub_0800447c, 0x0800447c
	.set sub_08009080, 0x08009080
	.set sub_08009098, 0x08009098
	.set sub_08009150, 0x08009150
	.set sub_080091e0, 0x080091e0
	.set sub_08009240, 0x08009240
	.set sub_08096c80, 0x08096c80
	.set sub_08097384, 0x08097384
	.set sub_080f9010, 0x080f9010
	.global Overlay_08098698
Overlay_08098698:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #76]
	ldr	r3, [r3, #0]
	sub	sp, #12
	mov	r9, r3
	ldr	r7, [r3, #16]
	bl	sub_08097384
	movs	r0, #23
	mov	r8, sp
	mov	sl, r8
	mov	fp, r0
.L_080986bc:
	mov	r2, r9
	movs	r5, #128
	ldr	r3, [r2, #0]
	lsls	r5, r5, #7
	cmp	r3, r5
	bne.n	.L_080986d6
	ldr	r3, [r7, #8]
	mov	r0, sl
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	movs	r2, #160
	lsls	r2, r2, #12
	b.n	.L_080986ea
.L_080986d6:
	movs	r5, #192
	lsls	r5, r5, #8
	cmp	r3, r5
	bne.n	.L_080986f8
	ldr	r3, [r7, #8]
	mov	r0, sl
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	movs	r2, #192
	lsls	r2, r2, #13
.L_080986ea:
	adds	r3, r3, r2
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	str	r3, [r0, #8]
	b.n	.L_08098716
	.2byte 0x1f30
	.2byte 0x0300
.L_080986f8:
	ldr	r3, [r7, #8]
	mov	r5, sl
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	movs	r0, #160
	lsls	r0, r0, #12
	adds	r3, r3, r0
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	str	r3, [r5, #8]
	mov	r2, r9
	ldr	r1, [r2, #0]
	mov	r2, sl
	bl	sub_0800447c
.L_08098716:
	mov	r3, sl
	movs	r0, #142
	ldr	r1, [r3, #0]
	ldr	r2, [r3, #4]
	lsls	r0, r0, #1
	ldr	r3, [r3, #8]
	bl	sub_08096c80
	adds	r6, r0, #0
	ldr	r4, [r6, #80]
	ldrb	r3, [r4, #5]
	adds	r0, r4, #0
	adds	r0, #12
	movs	r1, #32
	movs	r5, #33
	ands	r1, r3
	negs	r5, r5
	ldrb	r3, [r0, #5]
	adds	r2, r5, #0
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r0, #5]
	ldrb	r2, [r4, #5]
	movs	r1, #63
	lsrs	r2, r2, #6
	lsls	r2, r2, #6
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r0, #5]
	ldrb	r3, [r4, #7]
	ldrb	r2, [r0, #7]
	lsrs	r3, r3, #6
	lsls	r3, r3, #6
	ands	r1, r2
	orrs	r1, r3
	strb	r1, [r0, #7]
	ldrh	r1, [r4, #8]
	ldrh	r3, [r0, #8]
	ldr	r2, [pc, #56]
	lsls	r1, r1, #22
	lsrs	r1, r1, #22
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r0, #8]
	ldrb	r2, [r4, #9]
	ldrb	r1, [r0, #9]
	lsrs	r2, r2, #4
	movs	r3, #15
	lsls	r2, r2, #4
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r0, #9]
	cmp	r6, #0
	beq.n	.L_08098812
	ldr	r3, [pc, #28]
	str	r3, [r6, #28]
	str	r3, [r6, #24]
	movs	r3, #192
	lsls	r3, r3, #9
	adds	r2, r6, #0
	adds	r2, #85
	str	r3, [r6, #52]
	str	r3, [r6, #48]
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r0, r6, #0
	b.n	.L_080987a4
	.4byte 0xfffffc00
	.2byte 0xb333
	.2byte 0x0000
.L_080987a4:
	movs	r1, #11
	bl	sub_08009240
	adds	r0, r6, #0
	movs	r1, #7
	bl	sub_08009080
	adds	r0, r6, #0
	ldr	r1, [pc, #140]
	bl	sub_08009098
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_080091e0
	mov	r0, r9
	ldr	r3, [r0, #4]
	mov	r2, r8
	str	r3, [r2, #0]
	ldr	r3, [r0, #8]
	str	r3, [r2, #4]
	ldr	r3, [r0, #12]
	str	r3, [r2, #8]
	movs	r3, #192
	ldr	r1, [r0, #0]
	lsls	r3, r3, #8
	cmp	r1, r3
	bne.n	.L_080987e4
	movs	r0, #224
	lsls	r0, r0, #12
	bl	sub_0800447c
.L_080987e4:
	bl	sub_08004458
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r0, #128
	lsls	r0, r0, #11
	lsls	r5, r5, #1
	adds	r5, r5, r0
	bl	sub_08004458
	mov	r2, r8
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_0800447c
	mov	r5, r8
	mov	r2, r8
	ldr	r1, [r2, #0]
	ldr	r3, [r5, #8]
	ldr	r2, [r2, #4]
	adds	r0, r6, #0
	bl	sub_08009150
.L_08098812:
	movs	r0, #131
	bl	sub_080f9010
	movs	r0, #2
	bl	sub_080030f8
	movs	r0, #1
	negs	r0, r0
	add	fp, r0
	mov	r2, fp
	cmp	r2, #0
	blt.n	.L_0809882c
	b.n	.L_080986bc
.L_0809882c:
	movs	r0, #8
	bl	sub_080030f8
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0809f0b4

.syntax unified
	.thumb
	.set sub_08017884, 0x08017884
	.global Func_0801713c
	.thumb_func
Func_0801713c:
	push	{r4, r5, r6, r7, lr}
	sub	sp, #128
	adds	r5, r1, #0
	adds	r6, r2, #0
	adds	r7, r3, #0
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	ldr	r2, [pc, #32]
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #32]
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
	ldr	r3, [pc, #24]
	movs	r0, #1
	eors	r3, r0
	mov	r2, sp
	ldr	r0, [pc, #20]
	ldr	r1, [pc, #16]
	subs	r0, r0, r1
	lsls	r0, r0, #15
	b.n	.L_08017188
	movs	r0, r0
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x08017119
	.2byte 0x713d
	.2byte 0x0801
.L_0801717c:
	ldrh	r0, [r3, #0]
	strh	r0, [r2, #0]
	adds	r3, #2
	adds	r2, #2
	subs	r0, r1, #1
	lsls	r0, r0, #16
.L_08017188:
	lsrs	r1, r0, #16
	cmp	r1, #0
	bne.n	.L_0801717c
	mov	r3, sp
	adds	r3, #1
	ldr	r0, [pc, #32]
	ldrb	r0, [r0, #28]
	lsls	r4, r0
	adds	r0, r4, #0
	movs	r2, #224
	lsls	r2, r2, #20
	adds	r1, r5, r2
	adds	r0, r0, r1
	adds	r1, r6, #0
	adds	r2, r7, #0
	bl	sub_08017884
	add	sp, #128
	pop	{r4, r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.2byte 0x7e14
	.2byte 0x0801
	push	{r4, r5, lr}
	adds	r4, r0, #0
	adds	r3, r1, #0
	lsls	r2, r2, #16
	ldr	r0, [pc, #28]
	adds	r2, r2, r0
	lsrs	r2, r2, #16
	ldr	r0, [pc, #28]
	cmp	r2, r0
	beq.n	.L_080171f2
	adds	r5, r0, #0
.L_080171ce:
	ldrb	r1, [r3, #0]
	ldrb	r0, [r4, #0]
	adds	r4, #1
	adds	r3, #1
	cmp	r1, r0
	beq.n	.L_080171e8
	subs	r0, r3, #1
	b.n	.L_080171f4
	movs	r0, r0
	.4byte 0xffff0000
	.2byte 0xffff
	.2byte 0x0000
.L_080171e8:
	subs	r0, r2, #1
	lsls	r0, r0, #16
	lsrs	r2, r0, #16
	cmp	r2, r5
	bne.n	.L_080171ce
.L_080171f2:
	movs	r0, #0
.L_080171f4:
	pop	{r4, r5}
	pop	{r1}
	bx	r1
	.align 2, 0

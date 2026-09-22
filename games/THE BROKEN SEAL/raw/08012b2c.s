.syntax unified
	.thumb
	.set sub_0800447c, 0x0800447c
	.global Overlay_08012b2c
Overlay_08012b2c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #464]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #40]
	ldrb	r4, [r3, #4]
	ldr	r3, [pc, #460]
	ldr	r3, [r3, #0]
	adds	r7, r2, #0
	movs	r2, #2
	ands	r3, r2
	sub	sp, #4
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_08012b5c
	ldr	r3, [pc, #444]
	ldr	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r5, r3, #16
.L_08012b5c:
	cmp	r4, #6
	beq.n	.L_08012c0c
	cmp	r4, #6
	bhi.n	.L_08012b72
	cmp	r4, #4
	beq.n	.L_08012c0c
	cmp	r4, #4
	bhi.n	.L_08012bca
	cmp	r4, #3
	beq.n	.L_08012b8a
	b.n	.L_08012cae
.L_08012b72:
	cmp	r4, #20
	beq.n	.L_08012c44
	cmp	r4, #20
	bhi.n	.L_08012b80
	cmp	r4, #8
	beq.n	.L_08012bca
	b.n	.L_08012cae
.L_08012b80:
	cmp	r4, #44
	beq.n	.L_08012bca
	cmp	r4, #88
	beq.n	.L_08012bca
	b.n	.L_08012cae
.L_08012b8a:
	lsls	r0, r0, #16
	mov	r8, r0
	lsls	r6, r1, #16
	movs	r4, #5
.L_08012b92:
	lsls	r5, r5, #16
	movs	r3, #0
	mov	r2, r8
	lsrs	r5, r5, #16
	movs	r0, #224
	str	r2, [r7, #0]
	str	r3, [r7, #4]
	str	r6, [r7, #8]
	adds	r1, r5, #0
	adds	r2, r7, #0
	lsls	r0, r0, #14
	str	r4, [sp, #0]
	bl	sub_0800447c
	ldr	r3, [pc, #360]
	ldr	r4, [sp, #0]
	adds	r5, r5, r3
	lsls	r5, r5, #16
	subs	r4, #1
	adds	r7, #16
	asrs	r5, r5, #16
	cmp	r4, #0
	bge.n	.L_08012b92
	movs	r4, #6
.L_08012bc2:
	adds	r4, #1
	cmp	r4, #9
	ble.n	.L_08012bc2
	b.n	.L_08012cf8
.L_08012bca:
	lsls	r0, r0, #16
	mov	r8, r0
	lsls	r6, r1, #16
	movs	r4, #7
.L_08012bd2:
	lsls	r5, r5, #16
	movs	r3, #0
	mov	r2, r8
	lsrs	r5, r5, #16
	movs	r0, #224
	str	r2, [r7, #0]
	str	r3, [r7, #4]
	str	r6, [r7, #8]
	adds	r1, r5, #0
	adds	r2, r7, #0
	lsls	r0, r0, #14
	str	r4, [sp, #0]
	bl	sub_0800447c
	movs	r3, #128
	lsls	r3, r3, #6
	ldr	r4, [sp, #0]
	adds	r5, r5, r3
	lsls	r5, r5, #16
	subs	r4, #1
	adds	r7, #16
	asrs	r5, r5, #16
	cmp	r4, #0
	bge.n	.L_08012bd2
	movs	r4, #8
.L_08012c04:
	adds	r4, #1
	cmp	r4, #9
	ble.n	.L_08012c04
	b.n	.L_08012cf8
.L_08012c0c:
	lsls	r0, r0, #16
	movs	r4, #0
	mov	r8, r0
	lsls	r6, r1, #16
.L_08012c14:
	lsls	r5, r5, #16
	movs	r3, #0
	mov	r2, r8
	lsrs	r5, r5, #16
	movs	r0, #224
	str	r2, [r7, #0]
	str	r3, [r7, #4]
	str	r6, [r7, #8]
	adds	r1, r5, #0
	adds	r2, r7, #0
	lsls	r0, r0, #14
	str	r4, [sp, #0]
	bl	sub_0800447c
	ldr	r3, [pc, #232]
	ldr	r4, [sp, #0]
	adds	r5, r5, r3
	lsls	r5, r5, #16
	adds	r4, #1
	adds	r7, #16
	asrs	r5, r5, #16
	cmp	r4, #9
	ble.n	.L_08012c14
	b.n	.L_08012cf8
.L_08012c44:
	movs	r2, #128
	lsls	r3, r5, #16
	lsls	r2, r2, #23
	adds	r3, r3, r2
	asrs	r5, r3, #16
	movs	r3, #160
	movs	r4, #0
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	lsls	r3, r3, #14
	mov	sl, r0
	mov	fp, r4
	mov	r8, r1
	mov	r9, r3
	adds	r6, r7, #0
.L_08012c62:
	mov	r2, sl
	lsls	r5, r5, #16
	str	r2, [r6, #0]
	mov	r3, fp
	lsrs	r5, r5, #16
	mov	r2, r8
	str	r3, [r6, #4]
	str	r2, [r6, #8]
	adds	r1, r5, #0
	adds	r2, r7, #0
	mov	r0, r9
	str	r4, [sp, #0]
	bl	sub_0800447c
	mov	r3, sl
	str	r3, [r6, #16]
	mov	r3, fp
	str	r3, [r6, #20]
	adds	r2, r7, #0
	mov	r3, r8
	adds	r2, #16
	str	r3, [r6, #24]
	adds	r1, r5, #0
	mov	r0, r9
	bl	sub_0800447c
	movs	r2, #128
	lsls	r2, r2, #8
	ldr	r4, [sp, #0]
	adds	r5, r5, r2
	lsls	r5, r5, #16
	adds	r4, #1
	adds	r6, #32
	adds	r7, #32
	asrs	r5, r5, #16
	cmp	r4, #1
	ble.n	.L_08012c62
	b.n	.L_08012cf8
.L_08012cae:
	movs	r2, #128
	lsls	r3, r5, #16
	lsls	r2, r2, #22
	adds	r3, r3, r2
	lsls	r0, r0, #16
	asrs	r5, r3, #16
	mov	r8, r0
	lsls	r6, r1, #16
	movs	r4, #3
.L_08012cc0:
	mov	r3, r8
	lsls	r5, r5, #16
	str	r3, [r7, #0]
	lsrs	r5, r5, #16
	movs	r3, #0
	movs	r0, #224
	adds	r2, r7, #0
	str	r3, [r7, #4]
	str	r6, [r7, #8]
	adds	r1, r5, #0
	lsls	r0, r0, #14
	str	r4, [sp, #0]
	bl	sub_0800447c
	movs	r2, #128
	lsls	r2, r2, #7
	ldr	r4, [sp, #0]
	adds	r5, r5, r2
	lsls	r5, r5, #16
	subs	r4, #1
	adds	r7, #16
	asrs	r5, r5, #16
	cmp	r4, #0
	bge.n	.L_08012cc0
	movs	r4, #5
.L_08012cf2:
	subs	r4, #1
	cmp	r4, #0
	bge.n	.L_08012cf2
.L_08012cf8:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e60
	.4byte 0x03001ae8
	.4byte 0x03001800
	.4byte 0x00002aaa
	.4byte 0x00001999

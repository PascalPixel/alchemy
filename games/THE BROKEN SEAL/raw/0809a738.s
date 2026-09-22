.syntax unified
	.thumb
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08004458, 0x08004458
	.set sub_08009098, 0x08009098
	.global Overlay_0809a738
Overlay_0809a738:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	bl	sub_08004458
	ldrh	r2, [r7, #6]
	movs	r1, #128
	mov	r8, r2
	lsls	r1, r1, #10
	adds	r6, r0, #0
	mov	r0, r8
	adds	r6, r6, r1
	bl	sub_0800231c
	ldr	r5, [pc, #140]
	adds	r1, r0, #0
	adds	r0, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r5
	mov	sl, r0
	mov	r0, r8
	bl	sub_08002322
	adds	r1, r0, #0
	adds	r0, r6, #0
	mov	ip, pc
	bx	r5
	ldr	r3, [r7, #8]
	add	r3, sl
	str	r3, [r7, #8]
	ldr	r3, [r7, #16]
	adds	r3, r3, r0
	str	r3, [r7, #16]
	ldr	r1, [pc, #104]
	ldrh	r3, [r7, #6]
	adds	r3, r3, r1
	strh	r3, [r7, #6]
	adds	r5, r7, #0
	adds	r5, #102
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	ldrh	r2, [r5, #0]
	cmp	r3, #0
	beq.n	.L_0809a7a6
	subs	r3, r2, #1
	strh	r3, [r5, #0]
	ldrh	r3, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r3, r2
	strh	r3, [r7, #6]
	b.n	.L_0809a7be
.L_0809a7a6:
	bl	sub_08004458
	lsls	r0, r0, #5
	lsrs	r0, r0, #16
	cmp	r0, #0
	bne.n	.L_0809a7be
	bl	sub_08004458
	lsls	r0, r0, #4
	lsrs	r0, r0, #16
	adds	r0, #8
	strh	r0, [r5, #0]
.L_0809a7be:
	adds	r2, r7, #0
	adds	r2, #100
	ldrh	r3, [r2, #0]
	movs	r1, #202
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r1, r1, #15
	lsls	r3, r3, #16
	cmp	r3, r1
	bne.n	.L_0809a7da
	ldr	r1, [pc, #28]
	adds	r0, r7, #0
	bl	sub_08009098
.L_0809a7da:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03000118

.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08009080, 0x08009080
	.set sub_08009088, 0x08009088
	.set sub_08015130, 0x08015130
	.set sub_080b6b40, 0x080b6b40
	.set sub_080b7dd0, 0x080b7dd0
	.set sub_080b8000, 0x080b8000
	.set sub_080b8808, 0x080b8808
	.set sub_080c9018, 0x080c9018
	.set sub_080c9020, 0x080c9020
	.global Overlay_080b8c1c
Overlay_080b8c1c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #388]
	adds	r5, r0, #0
	ldr	r2, [r3, #0]
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	sub	sp, #92
	ldr	r1, [pc, #380]
	cmp	r3, #4
	bgt.n	.L_080b8c3a
	movs	r1, #128
	lsls	r1, r1, #6
.L_080b8c3a:
	ldr	r3, [r2, #0]
	cmp	r3, r1
	bne.n	.L_080b8c4c
	movs	r3, #40
	str	r3, [r2, #4]
	movs	r0, #40
	bl	sub_080030f8
	b.n	.L_080b8c58
.L_080b8c4c:
	movs	r3, #40
	str	r1, [r2, #0]
	str	r3, [r2, #4]
	movs	r0, #40
	bl	sub_080030f8
.L_080b8c58:
	movs	r2, #8
	ldrsh	r3, [r5, r2]
	add	r6, sp, #8
	str	r3, [r6, #0]
	movs	r1, #12
	ldrsh	r3, [r5, r1]
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	str	r3, [r6, #16]
	movs	r1, #10
	ldrsh	r3, [r5, r1]
	str	r0, [r6, #8]
	str	r3, [r6, #12]
	bl	sub_080b8808
	cmp	r0, #0
	bge.n	.L_080b8c80
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080b8d9c
.L_080b8c80:
	ldr	r3, [r6, #12]
	cmp	r3, #127
	ble.n	.L_080b8c8c
	add	r7, sp, #44
	movs	r0, #2
	b.n	.L_080b8c90
.L_080b8c8c:
	add	r7, sp, #44
	movs	r0, #1
.L_080b8c90:
	adds	r1, r7, #0
	bl	sub_080b6b40
	str	r0, [r6, #20]
	ldr	r3, [pc, #280]
	ldr	r3, [r3, #0]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	movs	r3, #2
	negs	r3, r3
	ands	r0, r3
	bl	sub_08015130
	ldr	r0, [r6, #8]
	bl	sub_080b7dd0
	ldr	r0, [r0, #0]
	movs	r1, #3
	mov	sl, r0
	bl	sub_08009080
	mov	r0, sl
	movs	r1, #16
	bl	sub_08009088
	ldrh	r3, [r5, #10]
	cmp	r3, #7
	bhi.n	.L_080b8cda
	movs	r2, #1
	mov	r8, r2
	str	r2, [r6, #4]
	movs	r0, #1
	adds	r1, r7, #0
	bl	sub_080b6b40
	mov	r3, r8
	b.n	.L_080b8ce8
.L_080b8cda:
	movs	r3, #0
	str	r3, [r6, #4]
	movs	r0, #2
	adds	r1, r7, #0
	bl	sub_080b6b40
	movs	r3, #1
.L_080b8ce8:
	str	r3, [r6, #20]
	ldr	r3, [r6, #20]
	movs	r7, #0
	adds	r2, r6, #0
	cmp	r3, #0
	beq.n	.L_080b8d36
	movs	r5, #0
.L_080b8cf6:
	lsls	r3, r7, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	sub_080b7dd0
	ldr	r3, [r0, #0]
	ldr	r1, [r3, #80]
	adds	r3, r1, #0
	adds	r3, #39
	ldrb	r3, [r3, #0]
	subs	r3, #1
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_080b8d2a
	mov	ip, r3
	adds	r3, r5, r6
	adds	r2, r3, #0
	adds	r2, #52
	adds	r1, #40
.L_080b8d1c:
	ldmia	r1!, {r3}
	ldrb	r3, [r3, #5]
	adds	r0, #1
	strb	r3, [r2, #0]
	adds	r2, #1
	cmp	r0, ip
	bne.n	.L_080b8d1c
.L_080b8d2a:
	ldr	r3, [r6, #20]
	adds	r7, #1
	adds	r5, #4
	adds	r2, r6, #0
	cmp	r7, r3
	bne.n	.L_080b8cf6
.L_080b8d36:
	movs	r7, #0
	adds	r0, r6, #0
	str	r7, [r6, #0]
	str	r7, [r6, #24]
	bl	sub_080c9020
	movs	r3, #1
	str	r3, [r6, #0]
	adds	r0, r6, #0
	bl	sub_080c9020
	movs	r3, #2
	str	r3, [r6, #0]
	adds	r0, r6, #0
	bl	sub_080c9020
	movs	r3, #3
	str	r3, [r6, #0]
	adds	r0, r6, #0
	bl	sub_080c9020
	adds	r0, r6, #0
	str	r7, [r6, #0]
	bl	sub_080c9018
	mov	r0, sl
	movs	r1, #1
	bl	sub_08009080
	add	r5, sp, #8
	ldr	r3, [r5, #20]
	adds	r2, r5, #0
	cmp	r3, #0
	beq.n	.L_080b8d94
	movs	r6, #36
.L_080b8d7c:
	str	r2, [sp, #4]
	ldrsh	r0, [r2, r6]
	str	r2, [sp, #0]
	bl	sub_080b8000
	ldr	r1, [sp, #4]
	ldr	r3, [r1, #20]
	adds	r7, #1
	adds	r6, #2
	ldr	r2, [sp, #0]
	cmp	r7, r3
	bne.n	.L_080b8d7c
.L_080b8d94:
	ldr	r0, [r5, #8]
	bl	sub_080b8000
	movs	r0, #0
.L_080b8d9c:
	add	sp, #92
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f00
	.4byte 0xffffe000
	.4byte 0x03001e74

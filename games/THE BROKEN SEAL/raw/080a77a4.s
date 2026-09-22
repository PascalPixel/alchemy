.syntax unified
	.thumb
	.set sub_080030f8, 0x080030f8
	.set sub_08015070, 0x08015070
	.set sub_08015270, 0x08015270
	.set sub_080770c0, 0x080770c0
	.set sub_080a17c4, 0x080a17c4
	.set sub_080a1ac0, 0x080a1ac0
	.set sub_080a7a34, 0x080a7a34
	.set sub_080a7d68, 0x080a7d68
	.global Overlay_080a77a4
Overlay_080a77a4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r2, #28
	adds	r2, r2, r0
	ldr	r3, [pc, #112]
	lsls	r0, r0, #2
	mov	sl, r0
	ldr	r5, [r3, #0]
	mov	r3, sl
	adds	r3, #20
	ldr	r0, [r5, r3]
	movs	r6, #0
	movs	r3, #1
	strb	r3, [r0, #5]
	strh	r6, [r0, #12]
	ldr	r0, [r5, #16]
	sub	sp, #4
	mov	r8, r2
	ldrsb	r7, [r5, r2]
	bl	sub_08015270
	movs	r0, #185
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080a77ee
	movs	r3, #3
	ldr	r0, [r5, #16]
	movs	r1, #9
	str	r3, [sp, #0]
	movs	r2, #1
	movs	r3, #9
	bl	sub_08015070
.L_080a77ee:
	movs	r3, #1
	negs	r3, r3
	cmp	r7, r3
	bne.n	.L_080a77fe
	ldr	r3, [pc, #40]
	mov	r2, r8
	strb	r3, [r5, r2]
	b.n	.L_080a780c
.L_080a77fe:
	lsls	r0, r7, #1
	adds	r0, r0, r7
	lsls	r0, r0, #3
	subs	r0, #10
	movs	r1, #16
	bl	sub_080a1ac0
.L_080a780c:
	movs	r2, #136
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #3
	bne.n	.L_080a7828
	bl	sub_080a7d68
	b.n	.L_080a782c
	movs	r0, r0
	.4byte 0x00000000
	.2byte 0x1f2c
	.2byte 0x0300
.L_080a7828:
	bl	sub_080a7a34
.L_080a782c:
	adds	r6, r0, #0
	mov	r3, sl
	adds	r3, #20
	ldr	r0, [r5, r3]
	bl	sub_080a17c4
	movs	r0, #1
	bl	sub_080030f8
	adds	r0, r6, #0
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1

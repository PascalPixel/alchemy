.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_0803939c, 0x0803939c
	.global Func_0803cb1c
	.thumb_func
Func_0803cb1c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	movs	r5, #215
	movs	r7, #161
	mov	r8, r3
	lsls	r5, r5, #3
	lsls	r7, r7, #3
	add	r5, r8
	add	r7, r8
	movs	r6, #0
.L_0803cb38:
	ldr	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L_0803cb4a
	ldrh	r3, [r0, #22]
	cmp	r3, #0
	beq.n	.L_0803cb4a
	movs	r1, #0
	bl	sub_0803939c
.L_0803cb4a:
	adds	r6, #1
	adds	r5, #40
	cmp	r6, #3
	bne.n	.L_0803cb38
.L_0803cb52:
	movs	r5, #215
	lsls	r5, r5, #3
	movs	r1, #1
	add	r5, r8
	movs	r6, #0
.L_0803cb5c:
	ldr	r2, [r5, #0]
	cmp	r2, #0
	beq.n	.L_0803cb74
	ldr	r3, [r2, #24]
	cmp	r3, #0
	bne.n	.L_0803cb72
	ldrh	r3, [r2, #22]
	cmp	r3, #0
	bne.n	.L_0803cb72
	str	r3, [r5, #0]
	b.n	.L_0803cb74
.L_0803cb72:
	movs	r1, #0
.L_0803cb74:
	adds	r6, #1
	adds	r5, #40
	cmp	r6, #3
	bne.n	.L_0803cb5c
	movs	r6, #0
	cmp	r1, #0
	bne.n	.L_0803cb9c
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_0803cb52
.L_0803cb8a:
	ldrh	r3, [r7, #22]
	cmp	r3, #0
	beq.n	.L_0803cb98
	adds	r0, r7, #0
	movs	r1, #0
	bl	sub_0803939c
.L_0803cb98:
	adds	r7, #36
	adds	r6, #1
.L_0803cb9c:
	cmp	r6, #12
	bne.n	.L_0803cb8a
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000

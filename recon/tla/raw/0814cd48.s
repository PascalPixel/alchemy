.syntax unified
	.thumb
	.set sub_08020030, 0x08020030
	.set sub_08118098, 0x08118098
	.set sub_081180d8, 0x081180d8
	.set sub_08118100, 0x08118100
	.global Func_0814cd48
	.thumb_func
Func_0814cd48:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	mov	r9, r3
	str	r0, [sp, #16]
	mov	sl, r1
	mov	fp, r2
	bl	sub_08118098
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #92]
	mov	r8, r0
	movs	r0, #0
	str	r3, [sp, #12]
	str	r0, [sp, #8]
	b.n	.L_0814ce00
.L_0814cd74:
	movs	r2, #1
	negs	r2, r2
	cmp	r9, r2
	beq.n	.L_0814cd8c
	add	r1, sp, #52
	movs	r3, #240
	ldrb	r0, [r1, #0]
	lsls	r3, r3, #7
	ldr	r1, [sp, #12]
	adds	r3, #220
	add	r3, r9
	strb	r0, [r1, r3]
.L_0814cd8c:
	mov	r3, r8
	adds	r3, #42
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0814cdfa
	cmp	sl, r2
	beq.n	.L_0814cdea
	ldrb	r2, [r6, #27]
	movs	r4, #0
	cmp	r2, #0
	beq.n	.L_0814cdea
	adds	r1, r6, #0
	adds	r1, #40
.L_0814cda6:
	ldmia	r1!, {r5}
	cmp	r5, #0
	beq.n	.L_0814cde4
	mov	r0, r8
	ldr	r3, [r0, #36]
	cmp	r5, r3
	beq.n	.L_0814cde4
	ldr	r3, [r0, #32]
	cmp	r5, r3
	beq.n	.L_0814cde4
	mov	r3, sl
	ldrb	r7, [r5, #5]
	cmp	r3, #0
	bne.n	.L_0814cdd4
	ldr	r0, [sp, #16]
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	bl	sub_08118100
	strb	r0, [r5, #5]
	ldr	r1, [sp, #4]
	ldr	r4, [sp, #0]
	b.n	.L_0814cdd8
.L_0814cdd4:
	mov	r0, sl
	strb	r0, [r5, #5]
.L_0814cdd8:
	ldrb	r3, [r5, #5]
	cmp	r3, r7
	beq.n	.L_0814cde2
	movs	r3, #255
	strb	r3, [r5, #22]
.L_0814cde2:
	ldrb	r2, [r6, #27]
.L_0814cde4:
	adds	r4, #1
	cmp	r4, r2
	bne.n	.L_0814cda6
.L_0814cdea:
	movs	r1, #1
	negs	r1, r1
	cmp	fp, r1
	beq.n	.L_0814cdfa
	adds	r0, r6, #0
	mov	r1, fp
	bl	sub_08020030
.L_0814cdfa:
	ldr	r3, [sp, #8]
	adds	r3, #1
	str	r3, [sp, #8]
.L_0814ce00:
	mov	r1, r8
	ldr	r0, [r1, #0]
	ldr	r1, [sp, #8]
	bl	sub_081180d8
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_0814cd74
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.align 2, 0

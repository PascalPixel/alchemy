.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_080ad3f8, 0x080ad3f8
	.set sub_080aec04, 0x080aec04
	.set sub_080aec1c, 0x080aec1c
	.set sub_080b02d4, 0x080b02d4
	.global Func_080aef34
	.thumb_func
Func_080aef34:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r5, r1, #0
	mov	r8, r0
	sub	sp, #4
	bl	sub_08016ca4
	lsls	r5, r5, #1
	mov	r9, r5
	mov	r3, r9
	adds	r7, r0, #0
	adds	r3, #216
	ldrh	r5, [r7, r3]
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_080aec1c
	cmp	r0, #0
	bne.n	.L_080aef66
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080aeffc
.L_080aef66:
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r3, r5, #0
	ands	r3, r1
	mov	sl, r1
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L_080aeffc
	adds	r0, r5, #0
	bl	sub_080aec04
	ldrb	r2, [r0, #2]
	cmp	r2, #6
	beq.n	.L_080aefda
	cmp	r2, #11
	beq.n	.L_080aefda
	movs	r6, #0
	movs	r5, #216
	b.n	.L_080aef90
.L_080aef8c:
	adds	r5, #2
	adds	r6, #1
.L_080aef90:
	cmp	r6, #14
	bgt.n	.L_080aefae
	ldrh	r3, [r5, r7]
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080aef8c
	ldrh	r0, [r5, r7]
	str	r2, [sp, #0]
	bl	sub_080aec04
	ldrb	r3, [r0, #2]
	ldr	r2, [sp, #0]
	cmp	r3, r2
	bne.n	.L_080aef8c
.L_080aefae:
	cmp	r6, #15
	beq.n	.L_080aefda
	lsls	r3, r6, #1
	adds	r6, r3, #0
	adds	r6, #216
	ldrh	r0, [r7, r6]
	bl	sub_080aec04
	ldrb	r2, [r0, #3]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080aefce
	movs	r0, #2
	negs	r0, r0
	b.n	.L_080aeffc
.L_080aefce:
	ldrh	r2, [r7, r6]
	movs	r3, #253
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r7, r6]
.L_080aefda:
	mov	r3, r9
	adds	r3, #216
	ldrh	r2, [r7, r3]
	ldr	r1, [pc, #20]
	mov	r0, r8
	orrs	r2, r1
	strh	r2, [r7, r3]
	bl	sub_080b02d4
	mov	r0, r8
	bl	sub_080ad3f8
	movs	r0, #0
	b.n	.L_080aeffc
	movs	r0, r0
	.2byte 0x0200
	.2byte 0x0000
.L_080aeffc:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}

.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_080ad000, 0x080ad000
	.set sub_080ad188, 0x080ad188
	.set sub_0811a038, 0x0811a038
	.global Func_08118f6c
	.thumb_func
Func_08118f6c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	mov	r1, sp
	adds	r1, #8
	adds	r0, r1, #0
	str	r1, [sp, #4]
	bl	sub_0811a038
	mov	fp, r0
	movs	r0, #0
	bl	sub_080ad000
	ldr	r0, [r0, #0]
	ldr	r3, [pc, #184]
	mov	r9, r0
	mov	r2, r9
	movs	r1, #28
	ands	r2, r3
	add	r1, sp
	movs	r3, #0
	mov	r9, r2
	mov	r8, r3
	mov	sl, r1
.L_08118fa6:
	mov	r2, sl
	movs	r3, #0
	mov	r1, r8
	strb	r3, [r2, r1]
	mov	r2, fp
	cmp	r2, #0
	ble.n	.L_08118fde
	movs	r1, #140
	ldr	r6, [sp, #4]
	lsls	r1, r1, #1
	mov	r7, sl
	add	r1, r8
	mov	r5, fp
.L_08118fc0:
	ldrh	r0, [r6, #0]
	str	r1, [sp, #0]
	bl	sub_08016ca4
	ldr	r1, [sp, #0]
	mov	r2, r8
	ldrb	r3, [r7, r2]
	ldrb	r2, [r0, r1]
	subs	r5, #1
	adds	r3, r3, r2
	mov	r2, r8
	adds	r6, #2
	strb	r3, [r7, r2]
	cmp	r5, #0
	bne.n	.L_08118fc0
.L_08118fde:
	movs	r3, #1
	add	r8, r3
	mov	r1, r8
	cmp	r1, #3
	ble.n	.L_08118fa6
	movs	r2, #0
	mov	r8, r2
.L_08118fec:
	mov	r0, r8
	bl	sub_080ad188
	cmp	r0, #0
	beq.n	.L_08119028
	mov	r3, sl
	adds	r0, #4
	ldrb	r2, [r3, #0]
	ldrb	r3, [r0, #0]
	movs	r4, #0
	cmp	r2, r3
	bcc.n	.L_08119018
	mov	r1, sl
.L_08119006:
	adds	r4, #1
	cmp	r4, #3
	bgt.n	.L_08119018
	adds	r1, #1
	adds	r0, #1
	ldrb	r2, [r1, #0]
	ldrb	r3, [r0, #0]
	cmp	r2, r3
	bcs.n	.L_08119006
.L_08119018:
	cmp	r4, #4
	bne.n	.L_08119028
	movs	r3, #1
	mov	r1, r8
	mov	r2, r9
	lsls	r3, r1
	orrs	r2, r3
	mov	r9, r2
.L_08119028:
	movs	r3, #1
	add	r8, r3
	mov	r1, r8
	cmp	r1, #15
	ble.n	.L_08118fec
	movs	r0, #0
	bl	sub_080ad000
	mov	r2, r9
	str	r2, [r0, #0]
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xffff0000

.syntax unified
	.thumb
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08020010, 0x08020010
	.set sub_08020030, 0x08020030
	.set sub_08020040, 0x08020040
	.set sub_08020048, 0x08020048
	.global Func_08043a64
	.thumb_func
Func_08043a64:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	str	r2, [sp, #4]
	str	r1, [sp, #8]
	mov	r8, r3
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	movs	r2, #0
	ldr	r1, [r3, #0]
	mov	fp, r2
	movs	r3, #44
	mov	r2, r8
	ldrsb	r3, [r2, r3]
	movs	r2, #1
	negs	r2, r2
	mov	sl, r0
	cmp	r3, r2
	beq.n	.L_08043b18
	movs	r3, #140
	lsls	r3, r3, #1
	movs	r2, #0
	adds	r6, r1, #0
	adds	r7, r1, r3
	mov	r9, r2
	adds	r6, #248
	movs	r4, #44
.L_08043aa6:
	mov	r3, r8
	ldrsb	r0, [r4, r3]
	str	r4, [sp, #0]
	bl	sub_08020040
	adds	r5, r0, #0
	ldr	r4, [sp, #0]
	cmp	r5, #0
	beq.n	.L_08043ad0
	movs	r1, #1
	bl	sub_08020030
	movs	r3, #0
	strb	r3, [r5, #26]
	movs	r1, #13
	ldrb	r3, [r5, #9]
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	strb	r3, [r5, #9]
	ldr	r4, [sp, #0]
.L_08043ad0:
	str	r5, [r6, #0]
	mov	r1, sl
	movs	r2, #12
	ldrsh	r3, [r1, r2]
	ldr	r2, [sp, #8]
	adds	r4, #1
	adds	r3, r2, r3
	add	r3, r9
	lsls	r3, r3, #3
	adds	r3, #16
	strh	r3, [r7, #0]
	movs	r2, #14
	ldrsh	r3, [r1, r2]
	ldr	r1, [sp, #4]
	movs	r2, #3
	adds	r3, r1, r3
	lsls	r3, r3, #3
	adds	r3, #16
	strh	r3, [r7, #16]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r6, #64]
	movs	r3, #1
	add	fp, r3
	mov	r1, fp
	adds	r7, #2
	add	r9, r2
	adds	r6, #4
	cmp	r1, #3
	bgt.n	.L_08043b18
	mov	r2, r8
	ldrsb	r3, [r4, r2]
	movs	r1, #1
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L_08043aa6
.L_08043b18:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl	sub_080145a8
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x3b71
	.2byte 0x0804
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r0, [pc, #40]
	ldr	r7, [r3, #0]
	bl	sub_08014644
	movs	r3, #0
	mov	r8, r3
	movs	r5, #248
	movs	r6, #3
.L_08043b50:
	ldr	r0, [r5, r7]
	cmp	r0, #0
	beq.n	.L_08043b5e
	bl	sub_08020048
	mov	r3, r8
	str	r3, [r5, r7]
.L_08043b5e:
	subs	r6, #1
	adds	r5, #4
	cmp	r6, #0
	bge.n	.L_08043b50
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x3b71
	.2byte 0x0804
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r2, #0
	adds	r3, #220
	ldr	r3, [r3, #0]
	mov	r8, r2
	movs	r2, #250
	lsls	r2, r2, #17
	mov	sl, r2
	movs	r2, #140
	sub	sp, #28
	lsls	r2, r2, #1
	adds	r5, r3, #0
	add	r4, sp, #4
	add	r6, sp, #12
	adds	r7, r3, r2
	adds	r5, #248
.L_08043b9a:
	ldr	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L_08043bd2
	ldr	r3, [r5, #64]
	adds	r1, r6, #0
	str	r3, [sp, #4]
	str	r4, [sp, #0]
	ldr	r3, [r5, #64]
	str	r3, [r4, #4]
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	mov	r3, sl
	str	r3, [r6, #4]
	movs	r2, #16
	ldrsh	r3, [r7, r2]
	adds	r2, r4, #0
	lsls	r3, r3, #16
	add	r3, sl
	str	r3, [r6, #8]
	movs	r3, #0
	str	r3, [r6, #12]
	movs	r3, #128
	lsls	r3, r3, #7
	bl	sub_08020010
	ldr	r4, [sp, #0]
.L_08043bd2:
	movs	r3, #1
	add	r8, r3
	mov	r2, r8
	adds	r7, #2
	adds	r5, #4
	cmp	r2, #3
	ble.n	.L_08043b9a
	add	sp, #28
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.align 2, 0

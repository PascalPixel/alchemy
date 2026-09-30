.syntax unified
	.thumb
	.global Func_081054cc
	.thumb_func
Func_081054cc:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #18
	adds	r3, r3, r2
	strb	r0, [r3, #0]
	bx	lr
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	str	r3, [sp, #4]
	adds	r6, r2, #0
	movs	r2, #12
	ldrsh	r3, [r0, r2]
	mov	sl, r1
	lsls	r3, r3, #3
	add	r3, sl
	adds	r3, #8
	str	r3, [sp, #0]
	movs	r1, #0
	mov	fp, r1
	movs	r1, #14
	ldrsh	r3, [r0, r1]
	movs	r2, #16
	lsls	r3, r3, #3
	adds	r3, r3, r6
	adds	r7, r3, #0
	negs	r2, r2
	mov	r8, r0
	adds	r7, #8
	movs	r5, #0
	mov	r9, r2
.L_0810551c:
	ldr	r1, [sp, #4]
	ldrb	r3, [r1, r5]
	cmp	r3, #0
	beq.n	.L_08105548
	ldr	r1, [sp, #0]
	adds	r2, r7, #0
	adds	r0, r5, #0
	bl	0x08105498
	ldr	r0, [pc, #72]
	mov	r2, sl
	adds	r2, #16
	adds	r3, r6, #0
	adds	r0, r5, r0
	mov	r1, r8
	bl	0x08038080
	movs	r2, #1
	adds	r6, #16
	adds	r7, #16
	add	fp, r2
	b.n	.L_08105552
.L_08105548:
	adds	r0, r5, #0
	mov	r1, r9
	mov	r2, r9
	bl	0x08105498
.L_08105552:
	adds	r5, #1
	cmp	r5, #4
	ble.n	.L_0810551c
	mov	r3, fp
	cmp	r3, #0
	bne.n	.L_0810556a
	ldr	r0, [pc, #28]
	mov	r1, r8
	mov	r2, sl
	adds	r3, r6, #0
	bl	0x08038080
.L_0810556a:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00001106
	.2byte 0x1105
	.2byte 0x0000

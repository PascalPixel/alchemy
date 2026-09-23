.syntax unified
	.thumb
	.set sub_08077008, 0x08077008
	.global Unnamed_080ac8fc
	.global Func_080ac8fc
	.thumb_func
Unnamed_080ac8fc:
Func_080ac8fc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r1, #0
	mov	fp, r0
	adds	r0, r5, #0
	sub	sp, #4
	adds	r6, r2, #0
	bl	sub_08077008
	mov	sl, r0
	movs	r0, #1
	negs	r0, r0
	movs	r7, #0
	cmp	r6, r0
	bne.n	.L_080ac994
	lsls	r5, r5, #8
	movs	r6, #132
	movs	r1, #0
	lsls	r6, r6, #1
	str	r5, [sp, #0]
	mov	r8, r1
	add	r6, sl
	mov	r9, r1
.L_080ac934:
	mov	r3, r8
	ldr	r2, [r6, #0]
	lsls	r1, r3, #5
	mov	r0, fp
	lsls	r3, r7, #1
	movs	r5, #0
	mov	lr, r9
	mov	ip, r2
	adds	r4, r3, r0
.L_080ac946:
	movs	r2, #1
	lsls	r2, r5
	mov	r3, ip
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ac960
	adds	r3, r1, #0
	ldr	r2, [pc, #168]
	ldr	r0, [sp, #0]
	orrs	r3, r5
	orrs	r3, r2
	orrs	r3, r0
	b.n	.L_080ac976
.L_080ac960:
	mov	r3, lr
	adds	r3, #248
	mov	r0, sl
	ldr	r3, [r0, r3]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ac97c
	ldr	r2, [sp, #0]
	adds	r3, r1, #0
	orrs	r3, r5
	orrs	r3, r2
.L_080ac976:
	strh	r3, [r4, #0]
	adds	r7, #1
	adds	r4, #2
.L_080ac97c:
	adds	r5, #1
	cmp	r5, #19
	ble.n	.L_080ac946
	movs	r0, #1
	add	r8, r0
	movs	r3, #4
	mov	r1, r8
	adds	r6, #4
	add	r9, r3
	cmp	r1, #3
	ble.n	.L_080ac934
	b.n	.L_080ac9ea
.L_080ac994:
	movs	r0, #132
	lsls	r3, r6, #2
	lsls	r0, r0, #1
	adds	r2, r3, r0
	mov	r1, sl
	ldr	r2, [r1, r2]
	lsls	r4, r6, #5
	mov	ip, r2
	mov	r6, fp
	lsls	r2, r7, #1
	adds	r0, r2, r6
	ldr	r2, [pc, #84]
	movs	r1, #1
	adds	r3, #248
	movs	r5, #0
	mov	lr, r1
	mov	r9, r2
	mov	r8, r3
.L_080ac9b8:
	mov	r1, lr
	lsls	r1, r5
	mov	r3, ip
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ac9ce
	adds	r3, r4, #0
	orrs	r3, r5
	mov	r6, r9
	orrs	r3, r6
	b.n	.L_080ac9de
.L_080ac9ce:
	mov	r2, sl
	mov	r6, r8
	ldr	r3, [r2, r6]
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ac9e4
	adds	r3, r4, #0
	orrs	r3, r5
.L_080ac9de:
	strh	r3, [r0, #0]
	adds	r7, #1
	adds	r0, #2
.L_080ac9e4:
	adds	r5, #1
	cmp	r5, #19
	ble.n	.L_080ac9b8
.L_080ac9ea:
	adds	r0, r7, #0
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0xffff8000

.syntax unified
	.thumb
	.set sub_080022f4, 0x080022f4
	.set sub_08002304, 0x08002304
	.set sub_080030f8, 0x080030f8
	.set sub_08003f3c, 0x08003f3c
	.set sub_080041d8, 0x080041d8
	.set sub_08015010, 0x08015010
	.set sub_08015018, 0x08015018
	.set sub_08015038, 0x08015038
	.set sub_08015048, 0x08015048
	.set sub_08015060, 0x08015060
	.set sub_08015068, 0x08015068
	.set sub_08015078, 0x08015078
	.set sub_08015080, 0x08015080
	.set sub_08015090, 0x08015090
	.set sub_080150b0, 0x080150b0
	.set sub_08015120, 0x08015120
	.set sub_08015270, 0x08015270
	.set sub_08015278, 0x08015278
	.set sub_08015280, 0x08015280
	.set sub_080152a8, 0x080152a8
	.set sub_080153f8, 0x080153f8
	.set sub_08077008, 0x08077008
	.set sub_08077010, 0x08077010
	.set sub_080771b0, 0x080771b0
	.set sub_080771b8, 0x080771b8
	.set sub_080771c0, 0x080771c0
	.set sub_080771c8, 0x080771c8
	.set sub_08077208, 0x08077208
	.set sub_08077210, 0x08077210
	.set sub_080a1a40, 0x080a1a40
	.set sub_080a1ac0, 0x080a1ac0
	.set sub_080aa538, 0x080aa538
	.set sub_080aafb8, 0x080aafb8
	.set sub_080ab1f4, 0x080ab1f4
	.set sub_080ab21c, 0x080ab21c
	.set sub_080ab2ec, 0x080ab2ec
	.set sub_080ad5b4, 0x080ad5b4
	.set sub_080ad5f4, 0x080ad5f4
	.set sub_080ad608, 0x080ad608
	.set sub_080ae714, 0x080ae714
	.set sub_080b50f8, 0x080b50f8
	.set sub_080f9010, 0x080f9010
	.global Func_080ac8fc
	.thumb_func
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

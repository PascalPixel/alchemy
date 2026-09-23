.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.global Func_08143eb4
	.thumb_func
Func_08143eb4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #96]
	adds	r5, r1, #0
	movs	r1, #128
	sub	sp, #4
	mov	sl, r0
	subs	r7, r2, r0
	lsls	r1, r1, #23
	subs	r0, r6, r5
	mov	r9, r2
	mov	r8, r0
	mov	fp, r1
	str	r3, [sp, #0]
	cmp	r5, #0
	bge.n	.L_08143ee6
	movs	r5, #0
.L_08143ee6:
	cmp	r5, #127
	ble.n	.L_08143eec
	movs	r5, #127
.L_08143eec:
	cmp	r6, #0
	bge.n	.L_08143ef2
	movs	r6, #0
.L_08143ef2:
	cmp	r6, #127
	ble.n	.L_08143ef8
	movs	r6, #127
.L_08143ef8:
	adds	r2, r7, #0
	cmp	r7, #0
	bge.n	.L_08143f00
	negs	r2, r7
.L_08143f00:
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_08143f08
	negs	r3, r3
.L_08143f08:
	cmp	r2, r3
	bge.n	.L_08143f9c
	mov	r2, r8
	cmp	r2, #0
	bge.n	.L_08143f24
	mov	ip, sl
	mov	sl, r9
	mov	r9, ip
	mov	ip, r5
	mov	r3, r9
	adds	r5, r6, #0
	mov	r0, sl
	mov	r6, ip
	subs	r7, r3, r0
.L_08143f24:
	subs	r1, r6, r5
	lsls	r0, r7, #8
	cmp	r7, #0
	bge.n	.L_08143f34
	mov	r0, r9
	mov	r2, sl
	subs	r3, r2, r0
	lsls	r0, r3, #8
.L_08143f34:
	cmp	r1, #0
	bge.n	.L_08143f3a
	subs	r1, r5, r6
.L_08143f3a:
	bl	sub_08002054
	adds	r4, r0, #0
	lsls	r4, r4, #23
	mov	r1, sl
	cmp	r7, #0
	bge.n	.L_08143f4e
	movs	r7, #1
	negs	r7, r7
	b.n	.L_08143f50
.L_08143f4e:
	movs	r7, #1
.L_08143f50:
	adds	r0, r5, #0
	cmp	r0, r6
	beq.n	.L_08144034
	movs	r2, #7
	mov	ip, r2
.L_08143f5a:
	lsrs	r2, r0, #3
	lsrs	r3, r1, #3
	lsls	r2, r2, #4
	mov	r5, ip
	adds	r2, r2, r3
	adds	r3, r0, #0
	ands	r3, r5
	lsls	r2, r2, #3
	adds	r2, r2, r3
	adds	r3, r1, #0
	ands	r3, r5
	lsls	r2, r2, #3
	adds	r2, r2, r3
	ldr	r3, [sp, #0]
	ldr	r5, [sp, #36]
	ldrb	r3, [r3, r2]
	mov	lr, r3
	cmp	lr, r5
	bge.n	.L_08143f84
	ldr	r3, [sp, #0]
	strb	r5, [r3, r2]
.L_08143f84:
	add	fp, r4
	mov	r5, fp
	cmp	r5, #0
	bge.n	.L_08143f94
	lsls	r3, r5, #1
	lsrs	r3, r3, #1
	adds	r1, r1, r7
	mov	fp, r3
.L_08143f94:
	adds	r0, #1
	cmp	r0, r6
	bne.n	.L_08143f5a
	b.n	.L_08144034
.L_08143f9c:
	cmp	r7, #0
	bge.n	.L_08143fba
	mov	ip, sl
	mov	sl, r9
	mov	r9, ip
	mov	ip, r5
	adds	r5, r6, #0
	mov	r6, ip
	subs	r2, r6, r5
	mov	r1, sl
	mov	r0, r9
	mov	r8, r2
	subs	r7, r0, r1
	mov	r1, r8
	b.n	.L_08143fbc
.L_08143fba:
	subs	r1, r6, r5
.L_08143fbc:
	lsls	r0, r1, #8
	cmp	r1, #0
	bge.n	.L_08143fc6
	subs	r3, r5, r6
	lsls	r0, r3, #8
.L_08143fc6:
	cmp	r7, #0
	blt.n	.L_08143fce
	adds	r1, r7, #0
	b.n	.L_08143fd4
.L_08143fce:
	mov	r3, sl
	mov	r6, r9
	subs	r1, r3, r6
.L_08143fd4:
	bl	sub_08002054
	adds	r4, r0, #0
	mov	r0, r8
	lsls	r4, r4, #23
	adds	r1, r5, #0
	cmp	r0, #0
	bge.n	.L_08143fec
	movs	r2, #1
	negs	r2, r2
	mov	r8, r2
	b.n	.L_08143ff0
.L_08143fec:
	movs	r3, #1
	mov	r8, r3
.L_08143ff0:
	mov	r0, sl
	cmp	r0, r9
	beq.n	.L_08144034
	movs	r5, #7
.L_08143ff8:
	lsrs	r2, r1, #3
	lsrs	r3, r0, #3
	lsls	r2, r2, #4
	adds	r2, r2, r3
	adds	r3, r1, #0
	ands	r3, r5
	lsls	r2, r2, #3
	adds	r2, r2, r3
	ldr	r6, [sp, #0]
	adds	r3, r0, #0
	ands	r3, r5
	lsls	r2, r2, #3
	adds	r2, r2, r3
	ldrb	r3, [r6, r2]
	ldr	r6, [sp, #36]
	cmp	r3, r6
	bge.n	.L_0814401e
	ldr	r3, [sp, #0]
	strb	r6, [r3, r2]
.L_0814401e:
	add	fp, r4
	mov	r6, fp
	cmp	r6, #0
	bge.n	.L_0814402e
	lsls	r3, r6, #1
	lsrs	r3, r3, #1
	add	r1, r8
	mov	fp, r3
.L_0814402e:
	adds	r0, #1
	cmp	r0, r9
	bne.n	.L_08143ff8
.L_08144034:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.align 2, 0

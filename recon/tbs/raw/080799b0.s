.syntax unified
	.thumb
	.set sub_08079338, 0x08079338
	.set sub_080797ec, 0x080797ec
	.set sub_080797fc, 0x080797fc
	.global Func_080799b0
	.thumb_func
Func_080799b0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #1
	adds	r5, r0, #0
	negs	r2, r2
	sub	sp, #20
	mov	r8, r2
	movs	r0, #0
	cmp	r5, #7
	bgt.n	.L_08079aba
	add	r6, sp, #4
	adds	r0, r5, #0
	adds	r2, r6, #0
	bl	sub_080797fc
	movs	r0, #32
	bl	sub_08079338
	cmp	r0, #0
	beq.n	.L_080799ee
	movs	r0, #200
	cmp	r5, #0
	beq.n	.L_08079aba
	movs	r0, #201
	cmp	r5, #1
	beq.n	.L_08079aba
.L_080799ee:
	movs	r0, #202
	cmp	r5, #5
	beq.n	.L_08079aba
	movs	r0, #1
	negs	r0, r0
	cmp	r8, r0
	bne.n	.L_08079aba
	mov	ip, r8
	mov	r5, r8
	movs	r0, #0
	adds	r2, r6, #0
.L_08079a04:
	ldmia	r2!, {r3}
	cmp	ip, r3
	bge.n	.L_08079a0e
	mov	ip, r3
	adds	r5, r0, #0
.L_08079a0e:
	adds	r0, #1
	cmp	r0, #3
	ble.n	.L_08079a04
	movs	r4, #1
	negs	r4, r4
	mov	ip, r4
	movs	r0, #0
	adds	r2, r6, #0
.L_08079a1e:
	cmp	r0, r5
	beq.n	.L_08079a2c
	ldr	r3, [r2, #0]
	cmp	ip, r3
	bge.n	.L_08079a2c
	mov	ip, r3
	adds	r4, r0, #0
.L_08079a2c:
	adds	r0, #1
	adds	r2, #4
	cmp	r0, #3
	ble.n	.L_08079a1e
	lsls	r3, r4, #2
	ldr	r3, [r6, r3]
	adds	r1, r5, #0
	cmp	r3, #9
	ble.n	.L_08079a46
	adds	r1, r4, #0
	b.n	.L_08079a46
.L_08079a42:
	mov	r8, r0
	b.n	.L_08079aac
.L_08079a46:
	adds	r0, r5, #0
	bl	sub_080797ec
	ldr	r3, [pc, #124]
	ldr	r7, [pc, #128]
	mov	sl, r3
	mov	lr, sl
	str	r6, [sp, #0]
	ldr	r5, [pc, #124]
	mov	fp, r0
	mov	r9, r6
	movs	r0, #202
	add	r7, lr
.L_08079a60:
	mov	r6, sl
	ldr	r3, [r5, r6]
	cmp	r3, fp
	bne.n	.L_08079aa2
	ldrb	r3, [r7, #0]
	ldr	r1, [sp, #0]
	lsls	r2, r3, #2
	adds	r2, r2, r3
	ldr	r3, [r1, #0]
	lsls	r2, r2, #1
	movs	r4, #0
	cmp	r3, r2
	blt.n	.L_08079a9e
	mov	r2, lr
	adds	r3, r5, r2
	mov	ip, r9
	adds	r1, r3, #4
.L_08079a82:
	adds	r4, #1
	cmp	r4, #3
	bgt.n	.L_08079a9e
	adds	r1, #1
	ldrb	r3, [r1, #0]
	lsls	r2, r3, #2
	adds	r2, r2, r3
	movs	r3, #4
	add	ip, r3
	mov	r6, ip
	ldr	r3, [r6, #0]
	lsls	r2, r2, #1
	cmp	r3, r2
	bge.n	.L_08079a82
.L_08079a9e:
	cmp	r4, #4
	beq.n	.L_08079a42
.L_08079aa2:
	subs	r0, #1
	subs	r7, #84
	subs	r5, #84
	cmp	r0, #0
	bge.n	.L_08079a60
.L_08079aac:
	movs	r1, #1
	negs	r1, r1
	cmp	r8, r1
	bne.n	.L_08079ab8
	movs	r2, #0
	mov	r8, r2
.L_08079ab8:
	mov	r0, r8
.L_08079aba:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x08084b1c
	.4byte 0x0000424c
	.4byte 0x00004248

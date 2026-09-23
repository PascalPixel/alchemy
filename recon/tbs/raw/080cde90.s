.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.global Func_080cde90
	.thumb_func
Func_080cde90:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r3, #0
	ldr	r3, [pc, #392]
	adds	r5, r1, #0
	ldr	r3, [r3, #0]
	adds	r4, r2, #0
	subs	r1, r6, r5
	movs	r2, #128
	sub	sp, #4
	mov	r8, r0
	subs	r7, r4, r0
	mov	sl, r1
	mov	r9, r2
	mov	fp, r3
	cmp	r5, #0
	bge.n	.L_080cdebe
	movs	r5, #0
.L_080cdebe:
	cmp	r5, #127
	ble.n	.L_080cdec4
	movs	r5, #127
.L_080cdec4:
	cmp	r6, #0
	bge.n	.L_080cdeca
	movs	r6, #0
.L_080cdeca:
	cmp	r6, #127
	ble.n	.L_080cded0
	movs	r6, #127
.L_080cded0:
	adds	r2, r7, #0
	cmp	r7, #0
	bge.n	.L_080cded8
	negs	r2, r7
.L_080cded8:
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_080cdee0
	negs	r3, r3
.L_080cdee0:
	cmp	r2, r3
	bge.n	.L_080cdf74
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_080cdefa
	mov	ip, r8
	mov	r8, r4
	mov	r4, ip
	mov	ip, r5
	mov	r1, r8
	adds	r5, r6, #0
	mov	r6, ip
	subs	r7, r4, r1
.L_080cdefa:
	subs	r1, r6, r5
	lsls	r0, r7, #8
	cmp	r7, #0
	bge.n	.L_080cdf08
	mov	r2, r8
	subs	r3, r2, r4
	lsls	r0, r3, #8
.L_080cdf08:
	cmp	r1, #0
	bge.n	.L_080cdf0e
	subs	r1, r5, r6
.L_080cdf0e:
	bl	sub_080022ec
	mov	ip, r0
	adds	r0, r5, #0
	mov	r1, r8
	cmp	r0, r6
	beq.n	.L_080ce018
	movs	r3, #128
	ldr	r5, [pc, #272]
	lsls	r3, r3, #1
	movs	r4, #7
	mov	lr, r3
	mov	r8, r5
.L_080cdf28:
	lsrs	r2, r0, #3
	lsrs	r3, r1, #3
	lsls	r2, r2, #4
	adds	r2, r2, r3
	adds	r3, r0, #0
	ands	r3, r4
	lsls	r2, r2, #3
	adds	r2, r2, r3
	adds	r3, r1, #0
	ands	r3, r4
	lsls	r2, r2, #3
	mov	r5, fp
	adds	r2, r2, r3
	ldrb	r3, [r5, r2]
	ldr	r5, [sp, #36]
	cmp	r3, r5
	bge.n	.L_080cdf4e
	mov	r3, fp
	strb	r5, [r3, r2]
.L_080cdf4e:
	add	r9, ip
	mov	r3, r9
	mov	r5, lr
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_080cdf6c
	cmp	r7, #0
	ble.n	.L_080cdf62
	adds	r1, #1
	b.n	.L_080cdf64
.L_080cdf62:
	subs	r1, #1
.L_080cdf64:
	mov	r2, r9
	mov	r3, r8
	ands	r2, r3
	mov	r9, r2
.L_080cdf6c:
	adds	r0, #1
	cmp	r0, r6
	bne.n	.L_080cdf28
	b.n	.L_080ce018
.L_080cdf74:
	cmp	r7, #0
	bge.n	.L_080cdf90
	mov	ip, r8
	mov	r8, r4
	mov	r4, ip
	mov	ip, r5
	adds	r5, r6, #0
	mov	r6, ip
	subs	r2, r6, r5
	mov	r1, r8
	mov	sl, r2
	subs	r7, r4, r1
	mov	r1, sl
	b.n	.L_080cdf92
.L_080cdf90:
	subs	r1, r6, r5
.L_080cdf92:
	lsls	r0, r1, #8
	cmp	r1, #0
	bge.n	.L_080cdf9c
	subs	r3, r5, r6
	lsls	r0, r3, #8
.L_080cdf9c:
	cmp	r7, #0
	blt.n	.L_080cdfae
	adds	r1, r7, #0
	str	r4, [sp, #0]
	bl	sub_080022ec
	mov	ip, r0
	ldr	r4, [sp, #0]
	b.n	.L_080cdfbc
.L_080cdfae:
	mov	r3, r8
	subs	r1, r3, r4
	str	r4, [sp, #0]
	bl	sub_080022ec
	ldr	r4, [sp, #0]
	mov	ip, r0
.L_080cdfbc:
	mov	r0, r8
	adds	r1, r5, #0
	cmp	r0, r4
	beq.n	.L_080ce018
	ldr	r6, [pc, #104]
	movs	r7, #128
	movs	r5, #7
	lsls	r7, r7, #1
	mov	lr, r6
.L_080cdfce:
	lsrs	r2, r1, #3
	lsrs	r3, r0, #3
	lsls	r2, r2, #4
	adds	r2, r2, r3
	adds	r3, r1, #0
	ands	r3, r5
	lsls	r2, r2, #3
	adds	r2, r2, r3
	adds	r3, r0, #0
	ands	r3, r5
	lsls	r2, r2, #3
	mov	r6, fp
	adds	r2, r2, r3
	ldrb	r3, [r6, r2]
	ldr	r6, [sp, #36]
	cmp	r3, r6
	bge.n	.L_080cdff4
	mov	r3, fp
	strb	r6, [r3, r2]
.L_080cdff4:
	add	r9, ip
	mov	r3, r9
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_080ce012
	mov	r6, sl
	cmp	r6, #0
	ble.n	.L_080ce008
	adds	r1, #1
	b.n	.L_080ce00a
.L_080ce008:
	subs	r1, #1
.L_080ce00a:
	mov	r2, r9
	mov	r3, lr
	ands	r2, r3
	mov	r9, r2
.L_080ce012:
	adds	r0, #1
	cmp	r0, r4
	bne.n	.L_080cdfce
.L_080ce018:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ef0
	.4byte 0xfffffeff

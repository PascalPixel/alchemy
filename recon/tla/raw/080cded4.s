.syntax unified
	.thumb
	.set sub_080ad010, 0x080ad010
	.set sub_080cad84, 0x080cad84
	.set sub_080cb09c, 0x080cb09c
	.set sub_080ccd48, 0x080ccd48
	.set sub_080cd91c, 0x080cd91c
	.set sub_080ce574, 0x080ce574
	.global Func_080cded4
	.thumb_func
Func_080cded4:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #162
	adds	r2, r5, r3
	movs	r3, #0
	strh	r3, [r2, #0]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #164
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r6, r0, #0
	cmp	r3, #0
	beq.n	.L_080cdf0e
	movs	r3, #1
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_080cdf0e
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #144
	bl	sub_080ce574
.L_080cdf0e:
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #172
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080cdf32
	movs	r3, #2
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_080cdf32
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #155
	bl	sub_080ce574
.L_080cdf32:
	ldr	r3, [pc, #36]
	movs	r2, #155
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080cdf56
	movs	r3, #4
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_080cdf56
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #139
	bl	sub_080ce574
.L_080cdf56:
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
.L_080cdf5c:
	push	{lr}
	ldr	r2, [pc, #28]
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #118
	adds	r3, r2, r1
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	movs	r0, #8
	cmp	r3, #0
	bne.n	.L_080cdf7a
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r2, r1
	ldr	r0, [r3, #0]
.L_080cdf7a:
	pop	{pc}
	.2byte 0x0240
	.2byte 0x0200
.L_080cdf80:
	push	{lr}
	cmp	r0, r1
	beq.n	.L_080cdf90
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	cmp	r0, r3
	bne.n	.L_080cdf94
.L_080cdf90:
	movs	r0, #1
	b.n	.L_080cdfae
.L_080cdf94:
	movs	r3, #255
	lsls	r3, r3, #1
	cmp	r0, r3
	bne.n	.L_080cdfac
	adds	r0, r1, #0
	bl	sub_080ad010
	ldrb	r3, [r0, #2]
	movs	r0, #1
	subs	r3, #1
	cmp	r3, #3
	bls.n	.L_080cdfae
.L_080cdfac:
	movs	r0, #0
.L_080cdfae:
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	sub	sp, #4
	ldr	r6, [r3, #16]
	mov	r8, r0
	bl	.L_080cdf5c
	bl	sub_080cad84
	ldrh	r0, [r0, #6]
	mov	fp, r0
	bl	.L_080cdf5c
	bl	sub_080cd91c
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	mov	r2, r8
	ands	r2, r3
	mov	r9, r0
	mov	r8, r2
	bl	sub_080cb09c
	ldr	r1, [r6, #0]
	movs	r3, #1
	negs	r3, r3
	mov	sl, r0
	cmp	r1, r3
	beq.n	.L_080ce09a
.L_080cdffc:
	movs	r3, #4
	ldrsh	r5, [r6, r3]
	movs	r3, #240
	lsls	r3, r3, #8
	ldrh	r2, [r6, #4]
	ands	r5, r3
	ldr	r3, [pc, #40]
	movs	r4, #255
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r7, r3, #16
	movs	r3, #15
	ands	r3, r1
	ands	r4, r2
	cmp	r3, #4
	bne.n	.L_080ce08c
	movs	r2, #6
	ldrsh	r0, [r6, r2]
	str	r4, [sp, #0]
	bl	sub_080ccd48
	ldr	r4, [sp, #0]
	cmp	r0, #0
	beq.n	.L_080ce08c
	cmp	r7, #0
	beq.n	.L_080ce052
	b.n	.L_080ce038
	movs	r0, r0
	.2byte 0x0800
	.2byte 0x0000
.L_080ce038:
	mov	r2, fp
	subs	r3, r5, r2
	movs	r2, #184
	lsls	r2, r2, #5
	adds	r2, #255
	adds	r3, r3, r2
	movs	r2, #188
	lsls	r3, r3, #16
	lsls	r2, r2, #6
	lsrs	r3, r3, #16
	adds	r2, #254
	cmp	r3, r2
	bhi.n	.L_080ce08c
.L_080ce052:
	ldr	r1, [r6, #0]
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_080ce05c
	adds	r3, #255
.L_080ce05c:
	lsls	r3, r3, #8
	lsrs	r0, r3, #16
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_080ce076
	mov	r1, r8
	str	r4, [sp, #0]
	bl	.L_080cdf80
	ldr	r4, [sp, #0]
	cmp	r0, #0
	beq.n	.L_080ce08c
	ldr	r1, [r6, #0]
.L_080ce076:
	movs	r3, #16
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080ce086
	cmp	r4, r9
	bne.n	.L_080ce08c
	adds	r0, r6, #0
	b.n	.L_080ce09c
.L_080ce086:
	adds	r0, r6, #0
	cmp	r4, sl
	beq.n	.L_080ce09c
.L_080ce08c:
	adds	r6, #12
	ldr	r3, [r6, #0]
	movs	r2, #1
	negs	r2, r2
	adds	r1, r3, #0
	cmp	r3, r2
	bne.n	.L_080cdffc
.L_080ce09a:
	movs	r0, #0
.L_080ce09c:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000

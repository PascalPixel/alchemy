.syntax unified
	.thumb
	.set sub_080cad84, 0x080cad84
	.set sub_080ccd48, 0x080ccd48
	.global Func_080ccd78
	.thumb_func
Func_080ccd78:
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
	ldr	r6, [pc, #236]
	mov	fp, r1
	movs	r1, #133
	lsls	r1, r1, #2
	ldr	r5, [r3, #16]
	mov	r8, r3
	adds	r3, r6, r1
	adds	r7, r0, #0
	ldr	r0, [r3, #0]
	bl	sub_080cad84
	ldr	r2, [r5, #0]
	ldrh	r0, [r0, #6]
	movs	r3, #1
	negs	r3, r3
	mov	sl, r0
	cmp	r2, r3
	beq.n	.L_080cce6c
	movs	r1, #252
	lsls	r1, r1, #6
	adds	r1, #254
	mov	r9, r1
.L_080ccdb8:
	movs	r3, #15
	ands	r3, r2
	cmp	r3, r7
	bne.n	.L_080cce5e
	ldrb	r3, [r5, #4]
	cmp	r3, fp
	beq.n	.L_080ccdce
	cmp	r7, #8
	beq.n	.L_080ccdce
	cmp	r7, #9
	bne.n	.L_080cce5e
.L_080ccdce:
	movs	r2, #6
	ldrsh	r0, [r5, r2]
	bl	sub_080ccd48
	cmp	r0, #0
	beq.n	.L_080cce5e
	ldr	r2, [r5, #0]
	movs	r3, #128
	lsls	r3, r3, #4
	ands	r3, r2
	movs	r6, #0
	movs	r4, #12
	cmp	r3, #0
	beq.n	.L_080ccdec
	movs	r4, #2
.L_080ccdec:
	movs	r0, #192
	lsls	r0, r0, #3
	adds	r3, r2, #0
	movs	r1, #128
	ands	r3, r0
	lsls	r1, r1, #2
	cmp	r3, r1
	beq.n	.L_080cce14
	cmp	r3, r1
	bgt.n	.L_080cce06
	cmp	r3, #0
	beq.n	.L_080cce56
	b.n	.L_080cce58
.L_080cce06:
	movs	r1, #128
	lsls	r1, r1, #3
	cmp	r3, r1
	beq.n	.L_080cce24
	cmp	r3, r0
	beq.n	.L_080cce2e
	b.n	.L_080cce58
.L_080cce14:
	movs	r3, #194
	lsls	r3, r3, #1
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, r4
	ble.n	.L_080cce58
	b.n	.L_080cce56
.L_080cce24:
	ldr	r1, [pc, #88]
	ldrb	r3, [r1, #0]
	cmp	r3, #2
	beq.n	.L_080cce58
	b.n	.L_080cce3c
.L_080cce2e:
	movs	r3, #194
	lsls	r3, r3, #1
	add	r3, r8
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, r4
	ble.n	.L_080cce58
.L_080cce3c:
	movs	r3, #240
	lsls	r3, r3, #8
	ands	r2, r3
	mov	r1, sl
	subs	r3, r2, r1
	movs	r2, #248
	lsls	r2, r2, #5
	adds	r2, #255
	adds	r3, r3, r2
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	cmp	r3, r9
	bhi.n	.L_080cce58
.L_080cce56:
	movs	r6, #1
.L_080cce58:
	adds	r0, r5, #0
	cmp	r6, #0
	bne.n	.L_080cce6e
.L_080cce5e:
	adds	r5, #12
	ldr	r3, [r5, #0]
	movs	r1, #1
	negs	r1, r1
	adds	r2, r3, #0
	cmp	r3, r1
	bne.n	.L_080ccdb8
.L_080cce6c:
	movs	r0, #0
.L_080cce6e:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x02000452

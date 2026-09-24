.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.global Curve_LookupScaledValue
	.global Func_08079b24
	.thumb_func
Curve_LookupScaledValue:
Func_08079b24:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #148]
	mov	r8, r1
	movs	r1, #0
	ldrsh	r2, [r5, r1]
	adds	r6, r0, #0
	movs	r4, #16
	ldrsh	r3, [r5, r4]
	movs	r0, #5
	cmp	r6, r2
	ble.n	.L_08079b42
	adds	r6, r2, #0
	b.n	.L_08079b48
.L_08079b42:
	cmp	r6, r3
	bge.n	.L_08079b48
	adds	r6, r3, #0
.L_08079b48:
	movs	r1, #0
	movs	r4, #0
	cmp	r1, r0
	bge.n	.L_08079b78
	movs	r7, #0
	ldrsh	r3, [r5, r7]
	cmp	r6, r3
	bgt.n	.L_08079b72
	mov	ip, r5
	movs	r2, #0
.L_08079b5c:
	adds	r1, #1
	adds	r2, #4
	cmp	r1, r0
	bge.n	.L_08079b76
	adds	r4, r2, #0
	mov	r3, ip
	ldrsh	r3, [r4, r3]
	mov	lr, r3
	cmp	r6, lr
	ble.n	.L_08079b5c
	b.n	.L_08079b78
.L_08079b72:
	movs	r4, #0
	b.n	.L_08079b78
.L_08079b76:
	lsls	r4, r1, #2
.L_08079b78:
	cmp	r1, r0
	bne.n	.L_08079b82
	subs	r3, r4, #2
	ldrsh	r0, [r5, r3]
	b.n	.L_08079b9e
.L_08079b82:
	subs	r3, r4, #4
	ldrsh	r1, [r5, r3]
	ldrsh	r0, [r5, r4]
	subs	r3, r4, #2
	ldrsh	r2, [r5, r3]
	adds	r3, r4, #2
	ldrsh	r5, [r5, r3]
	subs	r1, r1, r0
	subs	r2, r2, r5
	subs	r0, r6, r0
	muls	r0, r2
	bl	sub_080022ec
	adds	r0, r0, r5
.L_08079b9e:
	mov	r7, r8
	cmp	r7, #0
	beq.n	.L_08079bae
	cmp	r7, #1
	bne.n	.L_08079bae
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r0, r3, #1
.L_08079bae:
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r0, r0, r1
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x08089258

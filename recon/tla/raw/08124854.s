.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08016ca4, 0x08016ca4
	.set sub_080ad000, 0x080ad000
	.set sub_080ad008, 0x080ad008
	.set sub_080ad0d0, 0x080ad0d0
	.set sub_080ad158, 0x080ad158
	.set sub_080ad168, 0x080ad168
	.set sub_081201c4, 0x081201c4
	.set sub_08120360, 0x08120360
	.set sub_081234a4, 0x081234a4
	.set sub_08124810, 0x08124810
	.global Func_08124854
	.thumb_func
Func_08124854:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	bl	sub_08016ca4
	movs	r2, #153
	lsls	r2, r2, #1
	adds	r1, r0, #0
	adds	r5, r1, r2
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_081248ac
	adds	r3, #255
	strb	r3, [r5, #0]
	lsls	r3, r3, #24
	movs	r7, #0
	cmp	r3, #0
	bne.n	.L_08124888
	movs	r2, #52
	adds	r2, #255
	adds	r3, r1, r2
	strb	r7, [r3, #0]
	movs	r0, #1
	b.n	.L_081248ae
.L_08124888:
	movs	r3, #52
	adds	r3, #255
	adds	r6, r1, r3
	movs	r3, #0
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	bge.n	.L_081248ac
	ldrb	r1, [r5, #0]
	mov	r0, r8
	movs	r2, #30
	bl	sub_08124810
	cmp	r0, #0
	beq.n	.L_081248ac
	strb	r7, [r6, #0]
	movs	r0, #1
	strb	r7, [r5, #0]
	b.n	.L_081248ae
.L_081248ac:
	movs	r0, #0
.L_081248ae:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	bl	sub_08016ca4
	movs	r2, #154
	lsls	r2, r2, #1
	adds	r1, r0, #0
	adds	r5, r1, r2
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_0812490c
	adds	r3, #255
	strb	r3, [r5, #0]
	lsls	r3, r3, #24
	movs	r7, #0
	cmp	r3, #0
	bne.n	.L_081248e8
	movs	r2, #54
	adds	r2, #255
	adds	r3, r1, r2
	strb	r7, [r3, #0]
	movs	r0, #1
	b.n	.L_0812490e
.L_081248e8:
	movs	r3, #54
	adds	r3, #255
	adds	r6, r1, r3
	movs	r3, #0
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	bge.n	.L_0812490c
	ldrb	r1, [r5, #0]
	mov	r0, r8
	movs	r2, #20
	bl	sub_08124810
	cmp	r0, #0
	beq.n	.L_0812490c
	strb	r7, [r6, #0]
	movs	r0, #1
	strb	r7, [r5, #0]
	b.n	.L_0812490e
.L_0812490c:
	movs	r0, #0
.L_0812490e:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	bl	sub_08016ca4
	movs	r2, #155
	lsls	r2, r2, #1
	adds	r1, r0, #0
	adds	r5, r1, r2
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_0812496c
	adds	r3, #255
	strb	r3, [r5, #0]
	lsls	r3, r3, #24
	movs	r7, #0
	cmp	r3, #0
	bne.n	.L_08124948
	movs	r2, #56
	adds	r2, #255
	adds	r3, r1, r2
	strb	r7, [r3, #0]
	movs	r0, #1
	b.n	.L_0812496e
.L_08124948:
	movs	r3, #56
	adds	r3, #255
	adds	r6, r1, r3
	movs	r3, #0
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	bge.n	.L_0812496c
	ldrb	r1, [r5, #0]
	mov	r0, r8
	movs	r2, #20
	bl	sub_08124810
	cmp	r0, #0
	beq.n	.L_0812496c
	strb	r7, [r6, #0]
	movs	r0, #1
	strb	r7, [r5, #0]
	b.n	.L_0812496e
.L_0812496c:
	movs	r0, #0
.L_0812496e:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	sub_08016ca4
	movs	r3, #156
	lsls	r3, r3, #1
	adds	r5, r0, r3
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_081249ac
	adds	r3, #255
	strb	r3, [r5, #0]
	lsls	r3, r3, #24
	movs	r0, #1
	cmp	r3, #0
	beq.n	.L_081249ae
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	movs	r2, #30
	bl	sub_08124810
	cmp	r0, #0
	beq.n	.L_081249ac
	movs	r3, #0
	strb	r3, [r5, #0]
	movs	r0, #1
	b.n	.L_081249ae
.L_081249ac:
	movs	r0, #0
.L_081249ae:
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	sub_08016ca4
	movs	r3, #58
	adds	r3, #255
	adds	r5, r0, r3
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_081249e8
	adds	r3, #255
	strb	r3, [r5, #0]
	lsls	r3, r3, #24
	movs	r0, #1
	cmp	r3, #0
	beq.n	.L_081249ea
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	movs	r2, #60
	bl	sub_08124810
	cmp	r0, #0
	beq.n	.L_081249e8
	movs	r3, #0
	strb	r3, [r5, #0]
	movs	r0, #1
	b.n	.L_081249ea
.L_081249e8:
	movs	r0, #0
.L_081249ea:
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	sub_08016ca4
	movs	r3, #157
	lsls	r3, r3, #1
	adds	r5, r0, r3
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_08124a24
	adds	r3, #255
	strb	r3, [r5, #0]
	lsls	r3, r3, #24
	movs	r0, #1
	cmp	r3, #0
	beq.n	.L_08124a26
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	movs	r2, #70
	bl	sub_08124810
	cmp	r0, #0
	beq.n	.L_08124a24
	movs	r3, #0
	strb	r3, [r5, #0]
	movs	r0, #1
	b.n	.L_08124a26
.L_08124a24:
	movs	r0, #0
.L_08124a26:
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	sub_08016ca4
	movs	r3, #60
	adds	r3, #255
	adds	r5, r0, r3
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_08124a60
	adds	r3, #255
	strb	r3, [r5, #0]
	lsls	r3, r3, #24
	movs	r0, #1
	cmp	r3, #0
	beq.n	.L_08124a62
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	movs	r2, #40
	bl	sub_08124810
	cmp	r0, #0
	beq.n	.L_08124a60
	movs	r3, #0
	strb	r3, [r5, #0]
	movs	r0, #1
	b.n	.L_08124a62
.L_08124a60:
	movs	r0, #0
.L_08124a62:
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	sub_08016ca4
	movs	r3, #158
	lsls	r3, r3, #1
	adds	r5, r0, r3
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_08124a9c
	adds	r3, #255
	strb	r3, [r5, #0]
	lsls	r3, r3, #24
	movs	r0, #1
	cmp	r3, #0
	beq.n	.L_08124a9e
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	movs	r2, #50
	bl	sub_08124810
	cmp	r0, #0
	beq.n	.L_08124a9c
	movs	r3, #0
	strb	r3, [r5, #0]
	movs	r0, #1
	b.n	.L_08124a9e
.L_08124a9c:
	movs	r0, #0
.L_08124a9e:
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	sub_08016ca4
	movs	r3, #62
	adds	r3, #255
	adds	r5, r0, r3
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_08124af4
	cmp	r3, #7
	bls.n	.L_08124ac0
	adds	r3, #248
	strb	r3, [r5, #0]
	adds	r2, r3, #0
.L_08124ac0:
	movs	r3, #7
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08124ad0
	adds	r3, r2, #0
	adds	r3, #255
	strb	r3, [r5, #0]
	adds	r2, r3, #0
.L_08124ad0:
	lsls	r3, r2, #24
	lsrs	r3, r3, #24
	movs	r0, #1
	cmp	r3, #0
	beq.n	.L_08124af6
	cmp	r3, #7
	bhi.n	.L_08124af4
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	movs	r2, #30
	bl	sub_08124810
	cmp	r0, #0
	beq.n	.L_08124af4
	movs	r3, #0
	strb	r3, [r5, #0]
	movs	r0, #1
	b.n	.L_08124af6
.L_08124af4:
	movs	r0, #0
.L_08124af6:
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	sl, r0
	bl	sub_08016ca4
	movs	r7, #159
	mov	r8, r0
	lsls	r7, r7, #1
	add	r7, r8
	ldrb	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_08124b92
	bl	sub_081234a4
	mov	r2, r8
	movs	r3, #52
	ldrsh	r6, [r2, r3]
	ldrb	r3, [r7, #0]
	movs	r1, #56
	ldrsh	r5, [r2, r1]
	adds	r3, #1
	adds	r0, r3, #0
	muls	r0, r6
	movs	r1, #10
	bl	sub_08002054
	mov	r9, r5
	adds	r5, r5, r0
	cmp	r5, r6
	ble.n	.L_08124b3c
	adds	r5, r6, #0
.L_08124b3c:
	mov	r1, r9
	subs	r6, r5, r1
	cmp	r6, #0
	beq.n	.L_08124b7a
	mov	r1, sl
	movs	r0, #0
	bl	sub_08120360
	mov	r1, r8
	movs	r2, #52
	ldrsh	r3, [r1, r2]
	cmp	r5, r3
	bne.n	.L_08124b60
	ldr	r1, [pc, #72]
	movs	r0, #4
	bl	sub_08120360
	b.n	.L_08124b70
.L_08124b60:
	movs	r0, #1
	adds	r1, r6, #0
	bl	sub_08120360
	ldr	r1, [pc, #56]
	movs	r0, #4
	bl	sub_08120360
.L_08124b70:
	mov	r2, r8
	strh	r5, [r2, #56]
	mov	r0, sl
	bl	sub_080ad0d0
.L_08124b7a:
	bl	sub_081201c4
	movs	r2, #159
	lsls	r2, r2, #1
	add	r2, r8
	ldrb	r3, [r2, #0]
	movs	r0, #1
	adds	r3, #255
	strb	r3, [r2, #0]
	lsls	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_08124b94
.L_08124b92:
	movs	r0, #0
.L_08124b94:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00000c6c
	.2byte 0x0c69
	.2byte 0x0000
	push	{lr}
	bl	sub_08016ca4
	movs	r3, #64
	adds	r3, #255
	adds	r1, r0, r3
	ldrb	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_08124bc8
	adds	r3, #255
	strb	r3, [r1, #0]
	lsls	r3, r3, #24
	movs	r0, #1
	cmp	r3, #0
	beq.n	.L_08124bca
.L_08124bc8:
	movs	r0, #0
.L_08124bca:
	pop	{pc}
	push	{lr}
	bl	sub_08016ca4
	movs	r3, #163
	lsls	r3, r3, #1
	adds	r1, r0, r3
	ldrb	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_08124bf8
	adds	r3, #255
	strb	r3, [r1, #0]
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_08124bf8
	movs	r1, #72
	adds	r1, #255
	adds	r2, r0, r1
	strb	r3, [r2, #0]
	movs	r0, #1
	b.n	.L_08124bfa
.L_08124bf8:
	movs	r0, #0
.L_08124bfa:
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #0
	sub	sp, #4
	bl	sub_080ad000
	movs	r1, #148
	adds	r3, r0, #0
	lsls	r1, r1, #1
	adds	r7, r3, #0
	adds	r3, r3, r1
	ldr	r3, [r3, #0]
	movs	r2, #0
	adds	r7, #8
	mov	r8, r2
	cmp	r2, r3
	bge.n	.L_08124c54
	adds	r5, r7, #0
.L_08124c22:
	movs	r3, #3
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	ble.n	.L_08124c42
	ldrb	r0, [r5, #2]
	str	r2, [sp, #0]
	bl	sub_08016ca4
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	ldr	r2, [sp, #0]
	cmp	r3, #0
	beq.n	.L_08124c42
	ldrb	r3, [r5, #3]
	subs	r3, #1
	strb	r3, [r5, #3]
.L_08124c42:
	movs	r1, #144
	movs	r3, #1
	lsls	r1, r1, #1
	add	r8, r3
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	adds	r5, #4
	cmp	r8, r3
	blt.n	.L_08124c22
.L_08124c54:
	movs	r1, #144
	movs	r3, #0
	lsls	r1, r1, #1
	mov	r8, r3
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	cmp	r8, r3
	bge.n	.L_08124ca0
	adds	r6, r7, #0
.L_08124c66:
	movs	r3, #3
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	bne.n	.L_08124c8e
	ldrb	r5, [r6, #2]
	ldrb	r1, [r6, #0]
	ldrb	r2, [r6, #1]
	adds	r0, r5, #0
	bl	sub_080ad158
	ldrb	r2, [r6, #1]
	ldrb	r1, [r6, #0]
	adds	r0, r5, #0
	bl	sub_080ad168
	adds	r0, r5, #0
	bl	sub_080ad008
	movs	r2, #1
	b.n	.L_08124c94
.L_08124c8e:
	movs	r3, #1
	adds	r6, #4
	add	r8, r3
.L_08124c94:
	movs	r1, #144
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	cmp	r8, r3
	blt.n	.L_08124c66
.L_08124ca0:
	adds	r0, r2, #0
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000

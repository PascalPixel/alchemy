.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.set sub_08038028, 0x08038028
	.set sub_080ad348, 0x080ad348
	.set sub_080ad3f8, 0x080ad3f8
	.set sub_080af8d0, 0x080af8d0
	.set sub_080af91c, 0x080af91c
	.set sub_080b0084, 0x080b0084
	.global Func_080afb80
	.thumb_func
Func_080afb80:
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	adds	r7, r1, #0
	bl	sub_08016ca4
	ldrb	r1, [r0, #15]
	movs	r3, #146
	lsls	r3, r3, #1
	adds	r5, r0, r3
	adds	r1, #1
	adds	r0, r6, #0
	bl	sub_080af8d0
	ldr	r3, [r5, #0]
	cmp	r3, r0
	bcc.n	.L_080afbb0
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	sub_080af91c
	cmp	r0, #0
	beq.n	.L_080afbb0
	adds	r0, r7, #0
	b.n	.L_080afbb2
.L_080afbb0:
	movs	r0, #0
.L_080afbb2:
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	adds	r6, r0, #0
	mov	r8, r1
	bl	sub_08016ca4
	ldrb	r5, [r0, #15]
	cmp	r5, r8
	bge.n	.L_080afbde
	mov	r3, r8
	mov	r7, sp
	subs	r5, r3, r5
.L_080afbd0:
	adds	r0, r6, #0
	adds	r1, r7, #0
	subs	r5, #1
	bl	sub_080af91c
	cmp	r5, #0
	bne.n	.L_080afbd0
.L_080afbde:
	adds	r0, r6, #0
	bl	sub_080ad3f8
	add	sp, #16
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #48
	adds	r6, r1, #0
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	mov	r8, r2
	subs	r6, #8
	cmp	r0, #127
	ble.n	.L_080afc10
	ldr	r0, [sp, #12]
	cmp	r0, #134
	ble.n	.L_080afc14
.L_080afc10:
	movs	r0, #0
	b.n	.L_080afd9c
.L_080afc14:
	movs	r1, #248
	lsls	r1, r1, #2
	movs	r0, #0
	cmp	r6, r1
	bls.n	.L_080afc20
	b.n	.L_080afd9c
.L_080afc20:
	ldr	r0, [sp, #12]
	bl	sub_08016ca4
	movs	r1, #166
	ldr	r3, [pc, #384]
	lsls	r1, r1, #1
	adds	r5, r0, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22be
	lsls	r2, r2, #1
	cmp	r6, r2
	bcc.n	.L_080afc3c
	movs	r6, #0
.L_080afc3c:
	movs	r3, #76
	adds	r2, r6, #0
	muls	r2, r3
	ldr	r3, [pc, #364]
	ldr	r0, [pc, #364]
	adds	r4, r2, r3
	ldrb	r3, [r4, #15]
	ldrb	r2, [r4, #28]
	strb	r3, [r5, #15]
	ldrh	r3, [r4, #16]
	adds	r0, r6, r0
	strh	r3, [r5, #16]
	strh	r3, [r5, #56]
	strh	r3, [r5, #52]
	ldrh	r3, [r4, #18]
	add	r6, sp, #16
	strh	r3, [r5, #18]
	strh	r3, [r5, #58]
	strh	r3, [r5, #54]
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r5, #20]
	strh	r3, [r5, #22]
	ldrh	r3, [r4, #20]
	adds	r1, r6, #0
	strh	r3, [r5, #24]
	ldrh	r3, [r4, #22]
	str	r4, [sp, #0]
	strh	r3, [r5, #26]
	ldrh	r3, [r4, #24]
	movs	r7, #0
	strh	r3, [r5, #28]
	ldrb	r3, [r4, #26]
	strb	r3, [r5, #30]
	ldrb	r3, [r4, #27]
	strb	r3, [r5, #31]
	adds	r3, r5, #0
	adds	r3, #32
	strb	r2, [r3, #0]
	ldrb	r3, [r4, #29]
	adds	r2, r5, #0
	adds	r2, #33
	strb	r3, [r2, #0]
	movs	r2, #15
	bl	sub_08038028
	ldrh	r3, [r6, r7]
	ldr	r4, [sp, #0]
	cmp	r3, #0
	beq.n	.L_080afcba
	adds	r0, r6, #0
	adds	r1, r5, #0
	movs	r2, #0
.L_080afca6:
	ldrh	r3, [r2, r0]
	adds	r7, #1
	strb	r3, [r1, #0]
	adds	r2, #2
	adds	r1, #1
	cmp	r7, #13
	bgt.n	.L_080afcba
	ldrh	r3, [r2, r6]
	cmp	r3, #0
	bne.n	.L_080afca6
.L_080afcba:
	mov	r3, r8
	cmp	r3, #8
	bgt.n	.L_080afcc6
	adds	r3, #49
	strb	r3, [r5, r7]
	adds	r7, #1
.L_080afcc6:
	adds	r0, r4, #2
	mov	sl, r0
	movs	r3, #0
	mov	ip, sl
	strb	r3, [r5, r7]
	mov	r1, ip
	movs	r3, #0
	strb	r3, [r5, #14]
	movs	r0, #28
	str	r1, [sp, #4]
	adds	r4, #30
	movs	r1, #36
	mov	r9, r3
	movs	r7, #0
	mov	fp, r0
	mov	r8, r1
	mov	lr, r4
	mov	ip, r5
.L_080afcea:
	mov	r2, lr
	ldrh	r3, [r2, #0]
	movs	r1, #2
	ldr	r0, [sp, #4]
	add	lr, r1
	cmp	r3, #0
	beq.n	.L_080afd28
	mov	r2, r8
	mov	r1, sl
	ldrb	r3, [r2, r1]
	movs	r4, #0
	cmp	r4, r3
	bge.n	.L_080afd28
	mov	r1, ip
	mov	r6, fp
	adds	r1, #216
.L_080afd0a:
	mov	r3, r9
	cmp	r3, #14
	bgt.n	.L_080afd1e
	ldrh	r3, [r0, r6]
	movs	r0, #2
	strh	r3, [r1, #0]
	movs	r3, #1
	adds	r1, #2
	add	ip, r0
	add	r9, r3
.L_080afd1e:
	mov	r0, sl
	ldrb	r3, [r0, r2]
	adds	r4, #1
	cmp	r4, r3
	blt.n	.L_080afd0a
.L_080afd28:
	movs	r0, #2
	movs	r1, #1
	adds	r7, #1
	add	fp, r0
	add	r8, r1
	cmp	r7, #3
	ble.n	.L_080afcea
	movs	r2, #144
	lsls	r2, r2, #1
	movs	r0, #42
	adds	r3, r5, r2
	adds	r0, #255
	movs	r2, #0
	str	r2, [r3, #0]
	adds	r3, r5, r0
	strb	r2, [r3, #0]
	add	r2, sp, #8
	ldrh	r2, [r2, #0]
	movs	r1, #165
	lsls	r1, r1, #1
	adds	r6, r5, r1
	strh	r2, [r6, #0]
	adds	r1, r5, #0
	adds	r1, #36
	ldr	r0, [sp, #12]
	bl	sub_080b0084
	ldr	r0, [sp, #12]
	bl	sub_080ad3f8
	movs	r3, #149
	lsls	r3, r3, #1
	adds	r1, r5, r3
	movs	r3, #1
	strb	r3, [r1, #0]
	movs	r0, #255
	ldrh	r2, [r6, #0]
	lsls	r0, r0, #8
	adds	r0, #247
	adds	r3, r2, r0
	movs	r0, #228
	lsls	r3, r3, #16
	lsls	r0, r0, #14
	cmp	r3, r0
	bhi.n	.L_080afd88
	movs	r3, #2
	strb	r3, [r1, #0]
	ldrh	r2, [r6, #0]
.L_080afd88:
	ldr	r0, [pc, #44]
	adds	r3, r2, r0
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #12
	cmp	r3, r2
	bhi.n	.L_080afd9a
	movs	r3, #2
	strb	r3, [r1, #0]
.L_080afd9a:
	movs	r0, #1
.L_080afd9c:
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03000258
	.4byte 0x080b9e7c
	.4byte 0x0000042c
	.2byte 0xfe8f
	.2byte 0xffff
.L_080afdbc:
	push	{r5, r6, lr}
	movs	r6, #0
	movs	r5, #0
.L_080afdc2:
	adds	r0, r5, #0
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080afdce
	adds	r6, #1
.L_080afdce:
	adds	r5, #1
	cmp	r5, #7
	ble.n	.L_080afdc2
	adds	r0, r6, #0
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	.L_080afdbc
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_08016cfc
	movs	r2, #0
	cmp	r2, r5
	bge.n	.L_080afe0a
	ldr	r0, [pc, #40]
	movs	r3, #134
	lsls	r3, r3, #2
	adds	r1, r0, r3
.L_080afdf6:
	ldrb	r3, [r1, #0]
	adds	r1, #1
	cmp	r3, r6
	beq.n	.L_080afe06
	adds	r2, #1
	cmp	r2, r5
	blt.n	.L_080afdf6
	b.n	.L_080afe0c
.L_080afe06:
	adds	r0, r5, #0
	b.n	.L_080afe16
.L_080afe0a:
	ldr	r0, [pc, #12]
.L_080afe0c:
	movs	r1, #134
	lsls	r1, r1, #2
	adds	r3, r2, r1
	strb	r6, [r0, r3]
	adds	r0, r5, #1
.L_080afe16:
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	bl	.L_080afdbc
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_08016d18
	movs	r1, #0
	cmp	r1, r6
	bge.n	.L_080afe4e
	ldr	r0, [pc, #64]
	movs	r2, #134
	lsls	r2, r2, #2
	ldrb	r3, [r0, r2]
	cmp	r3, r5
	beq.n	.L_080afe4e
	adds	r2, r0, r2
.L_080afe40:
	adds	r1, #1
	cmp	r1, r6
	bge.n	.L_080afe4e
	adds	r2, #1
	ldrb	r3, [r2, #0]
	cmp	r3, r5
	bne.n	.L_080afe40
.L_080afe4e:
	subs	r0, r6, #1
	cmp	r1, r0
	bge.n	.L_080afe6c
	ldr	r3, [pc, #28]
	movs	r4, #134
	adds	r3, r1, r3
	lsls	r4, r4, #2
	adds	r2, r3, r4
	subs	r1, r0, r1
.L_080afe60:
	ldrb	r3, [r2, #1]
	subs	r1, #1
	strb	r3, [r2, #0]
	adds	r2, #1
	cmp	r1, #0
	bne.n	.L_080afe60
.L_080afe6c:
	bl	.L_080afdbc
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r0, #0
	cmp	r5, #0
	beq.n	.L_080afea6
	bl	.L_080afdbc
	movs	r1, #0
	cmp	r0, #0
	beq.n	.L_080afea2
	ldr	r3, [pc, #28]
	movs	r4, #134
	lsls	r4, r4, #2
	adds	r2, r3, r4
.L_080afe94:
	ldrb	r3, [r2, #0]
	adds	r1, #1
	strh	r3, [r5, #0]
	adds	r2, #1
	adds	r5, #2
	cmp	r1, r0
	bne.n	.L_080afe94
.L_080afea2:
	ldr	r3, [pc, #4]
	strh	r3, [r5, #0]
.L_080afea6:
	pop	{r5, pc}
	.4byte 0x000000ff
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r1, [pc, #24]
	ldr	r2, [pc, #24]
	ldr	r3, [r1, #16]
	adds	r3, r3, r0
	cmp	r3, r2
	ble.n	.L_080afec0
	adds	r3, r2, #0
.L_080afec0:
	cmp	r3, #0
	bge.n	.L_080afec6
	movs	r3, #0
.L_080afec6:
	str	r3, [r1, #16]
	adds	r0, r3, #0
	pop	{pc}
	.4byte 0x02000240
	.2byte 0x423f
	.2byte 0x000f
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r2, #156
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r2, [r3, #0]
	ldr	r1, [pc, #24]
	adds	r2, r2, r0
	cmp	r2, r1
	ble.n	.L_080afeea
	adds	r2, r1, #0
.L_080afeea:
	cmp	r2, #0
	bge.n	.L_080afef0
	movs	r2, #0
.L_080afef0:
	str	r2, [r3, #0]
	adds	r0, r2, #0
	pop	{pc}
	movs	r0, r0
	.4byte 0x02000240
	.2byte 0x423f
	.2byte 0x000f
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r2, #158
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	adds	r2, r2, r0
	cmp	r2, #28
	ble.n	.L_080aff16
	movs	r2, #28
.L_080aff16:
	cmp	r2, #0
	bge.n	.L_080aff1c
	movs	r2, #0
.L_080aff1c:
	strb	r2, [r3, #0]
	adds	r0, r2, #0
	pop	{pc}
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r7, [pc, #68]
	movs	r3, #28
	adds	r3, r3, r7
	mov	sl, r3
	movs	r3, #1
	sub	sp, #4
	adds	r5, r0, #0
	movs	r1, #0
	mov	r8, r3
.L_080aff42:
	movs	r0, #0
	ldrb	r6, [r7, #0]
	str	r1, [sp, #0]
	bl	sub_080ad348
	ldr	r3, [r0, #0]
	mov	r2, r8
	lsls	r2, r6
	ands	r3, r2
	adds	r7, #1
	ldr	r1, [sp, #0]
	cmp	r3, #0
	beq.n	.L_080aff62
	strb	r6, [r5, #0]
	adds	r1, #1
	adds	r5, #1
.L_080aff62:
	cmp	r7, sl
	bls.n	.L_080aff42
	movs	r3, #32
	adds	r0, r1, #0
	strb	r3, [r5, #0]
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x14ec
	.2byte 0x080c
	push	{lr}
	cmp	r0, #28
	bls.n	.L_080aff86
	movs	r0, #0
	b.n	.L_080aff8c
.L_080aff86:
	ldr	r3, [pc, #8]
	lsls	r0, r0, #3
	adds	r0, r0, r3
.L_080aff8c:
	pop	{pc}
	movs	r0, r0
	.2byte 0x150c
	.2byte 0x080c
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r0, #0
	bl	sub_080ad348
	ldr	r3, [r0, #0]
	movs	r2, #1
	lsls	r2, r5
	orrs	r3, r2
	str	r3, [r0, #0]
	pop	{r5, pc}
	.align 2, 0

.syntax unified
	.thumb
	.set sub_08001c90, 0x08001c90
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_08013ba4, 0x08013ba4
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_080149e0, 0x080149e0
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014d00, 0x08014d00
	.set sub_0801587c, 0x0801587c
	.set sub_0811a31c, 0x0811a31c
	.set sub_08126ae4, 0x08126ae4
	.set sub_08126f8c, 0x08126f8c
	.set sub_08126ff4, 0x08126ff4
	.set sub_08138048, 0x08138048
	.global Func_08126b64
	.thumb_func
Func_08126b64:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	mov	r5, sp
	adds	r7, r0, #0
	adds	r1, r5, #0
	movs	r0, #3
	bl	sub_0811a31c
	cmp	r0, #0
	ble.n	.L_08126b98
	mov	r8, r5
	movs	r6, #0
	adds	r5, r0, #0
.L_08126b82:
	mov	r2, r8
	ldrsh	r0, [r6, r2]
	cmp	r0, r7
	beq.n	.L_08126b90
	movs	r1, #1
	bl	sub_08126ae4
.L_08126b90:
	subs	r5, #1
	adds	r6, #2
	cmp	r5, #0
	bne.n	.L_08126b82
.L_08126b98:
	add	sp, #28
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
.L_08126ba0:
	push	{r5, r6, r7, lr}
	sub	sp, #28
	mov	r5, sp
	movs	r0, #3
	adds	r1, r5, #0
	bl	sub_0811a31c
	cmp	r0, #0
	ble.n	.L_08126bc8
	adds	r7, r5, #0
	movs	r6, #0
	adds	r5, r0, #0
.L_08126bb8:
	ldrsh	r0, [r6, r7]
	movs	r1, #0
	subs	r5, #1
	bl	sub_08126ae4
	adds	r6, #2
	cmp	r5, #0
	bne.n	.L_08126bb8
.L_08126bc8:
	add	sp, #28
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #36]
	cmp	r4, #0
	bne.n	.L_08126bda
	b.n	.L_08126cfa
.L_08126bda:
	movs	r2, #207
	lsls	r2, r2, #3
	adds	r3, r4, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #6
	bne.n	.L_08126c20
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #118
	adds	r0, r4, r3
	ldrh	r3, [r0, #0]
	lsrs	r1, r3, #1
	cmp	r1, #16
	bls.n	.L_08126bf8
	movs	r1, #16
.L_08126bf8:
	ldr	r3, [pc, #28]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #24]
	adds	r2, #2
	subs	r3, r3, r1
	lsls	r3, r3, #8
	orrs	r3, r1
	strh	r3, [r2, #0]
	ldrh	r3, [r0, #0]
	adds	r3, #1
	strh	r3, [r0, #0]
	b.n	.L_08126cfa
	movs	r0, r0
	.4byte 0x00003f40
	.2byte 0x0010
	.2byte 0x0000
.L_08126c20:
	cmp	r3, #4
	bne.n	.L_08126c60
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r2, #118
	adds	r4, r4, r2
	ldrh	r3, [r4, #0]
	lsrs	r0, r3, #1
	cmp	r0, #16
	bls.n	.L_08126c36
	movs	r0, #16
.L_08126c36:
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #24]
	movs	r1, #128
	subs	r3, r3, r0
	lsls	r1, r1, #19
	lsls	r2, r0, #8
	orrs	r2, r3
	adds	r1, #82
	strh	r2, [r1, #0]
	ldrh	r3, [r4, #0]
	adds	r3, #1
	strh	r3, [r4, #0]
	b.n	.L_08126cfa
	.4byte 0x00003f40
	.2byte 0x0010
	.2byte 0x0000
.L_08126c60:
	cmp	r3, #2
	bne.n	.L_08126cb0
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #118
	adds	r4, r4, r3
	ldrh	r3, [r4, #0]
	ldr	r2, [pc, #36]
	movs	r0, #128
	ldrsb	r1, [r2, r3]
	ldr	r3, [pc, #28]
	lsls	r2, r1, #8
	subs	r3, r3, r1
	lsls	r0, r0, #19
	orrs	r2, r3
	adds	r0, #82
	strh	r2, [r0, #0]
	movs	r3, #15
	ldrh	r2, [r4, #0]
	adds	r1, r2, #1
	ands	r1, r3
	b.n	.L_08126ca4
	movs	r0, r0
	.4byte 0x00003f40
	.4byte 0x00000010
	.2byte 0xce54
	.2byte 0x0812
.L_08126ca4:
	cmp	r2, #14
	bls.n	.L_08126cac
	movs	r3, #16
	orrs	r1, r3
.L_08126cac:
	strh	r1, [r4, #0]
	b.n	.L_08126cfa
.L_08126cb0:
	cmp	r3, #0
	beq.n	.L_08126cfa
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #2
	strh	r3, [r2, #0]
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #118
	adds	r0, r4, r3
	ldr	r2, [pc, #28]
	ldrh	r3, [r0, #0]
	movs	r1, #128
	ldrsb	r3, [r2, r3]
	lsls	r1, r1, #19
	adds	r1, #84
	strh	r3, [r1, #0]
	movs	r3, #15
	ldrh	r2, [r0, #0]
	adds	r1, r2, #1
	ands	r1, r3
	b.n	.L_08126cf0
	.4byte 0x00003f90
	.4byte 0x00000010
	.2byte 0xce54
	.2byte 0x0812
.L_08126cf0:
	cmp	r2, #14
	bls.n	.L_08126cf8
	movs	r3, #16
	orrs	r1, r3
.L_08126cf8:
	strh	r1, [r0, #0]
.L_08126cfa:
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
	sub	sp, #28
	mov	r8, r0
	mov	sl, r1
	ldr	r5, [r3, #36]
	cmp	r1, #0
	bne.n	.L_08126d40
	ldr	r0, [pc, #224]
	bl	sub_08014644
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #84
	mov	r1, sl
	strh	r1, [r3, #0]
	bl	.L_08126ba0
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #128
	lsls	r0, r0, #19
	adds	r0, #80
	movs	r1, #0
	bl	sub_08013ba4
.L_08126d40:
	cmp	r5, #0
	beq.n	.L_08126dee
	mov	r2, sl
	cmp	r2, #0
	beq.n	.L_08126dee
	movs	r1, #207
	lsls	r1, r1, #3
	adds	r3, r5, r1
	mov	r1, sl
	strh	r1, [r3, #0]
	movs	r1, #192
	lsls	r1, r1, #3
	adds	r1, #118
	movs	r2, #0
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #84
	strh	r2, [r3, #0]
	movs	r2, #16
	subs	r3, #2
	strh	r2, [r3, #0]
	mov	r5, sp
	adds	r1, r5, #0
	movs	r0, #3
	bl	sub_0811a31c
	movs	r6, #0
	adds	r7, r0, #0
	cmp	r6, r7
	bcs.n	.L_08126d9e
	movs	r2, #1
	mov	fp, r5
	mov	r9, r2
	movs	r5, #0
.L_08126d88:
	mov	r3, fp
	ldrsh	r0, [r5, r3]
	mov	r2, r9
	mov	r1, sl
	ands	r1, r2
	adds	r6, #1
	bl	sub_08126ae4
	adds	r5, #2
	cmp	r6, r7
	bcc.n	.L_08126d88
.L_08126d9e:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_08126dd2
	ldrh	r0, [r3, #0]
	movs	r1, #2
	movs	r6, #0
	add	r8, r1
	cmp	r0, #255
	beq.n	.L_08126dd2
	movs	r7, #1
	adds	r5, r7, #0
	mov	r2, sl
	ands	r5, r2
.L_08126db8:
	adds	r1, r5, #0
	eors	r1, r7
	adds	r6, #1
	bl	sub_08126ae4
	cmp	r6, #13
	bhi.n	.L_08126dd2
	mov	r3, r8
	ldrh	r0, [r3, #0]
	movs	r1, #2
	add	r8, r1
	cmp	r0, #255
	bne.n	.L_08126db8
.L_08126dd2:
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #128
	lsls	r0, r0, #19
	adds	r0, #80
	movs	r1, #0
	bl	sub_08013ba4
	movs	r1, #200
	ldr	r0, [pc, #20]
	lsls	r1, r1, #4
	bl	sub_080145a8
.L_08126dee:
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x6bcd
	.2byte 0x0812
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #96]
	sub	sp, #16
	str	r2, [sp, #12]
	movs	r1, #158
	ldr	r2, [r3, #92]
	ldr	r3, [r3, #104]
	mov	r9, r2
	lsls	r1, r1, #5
	add	r1, r9
	movs	r2, #0
	mov	fp, r3
	movs	r3, #15
	str	r2, [r1, #0]
	movs	r7, #142
	str	r3, [sp, #8]
	lsls	r7, r7, #5
	add	r7, r9
.L_08126e34:
	ldr	r0, [r7, #24]
	cmp	r0, #0
	beq.n	.L_08126eb8
	ldr	r3, [r7, #0]
	asrs	r3, r3, #8
	adds	r0, r3, #0
	muls	r0, r3
	ldr	r3, [r7, #4]
	asrs	r3, r3, #8
	adds	r2, r3, #0
	muls	r2, r3
	adds	r3, r2, #0
	adds	r0, r0, r3
	ldr	r3, [r7, #8]
	asrs	r3, r3, #8
	adds	r2, r3, #0
	muls	r2, r3
	adds	r3, r2, #0
	adds	r0, r0, r3
	bl	sub_080149e0
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #255
	cmp	r0, r3
	bgt.n	.L_08126e6e
	movs	r3, #0
	str	r3, [r7, #24]
	b.n	.L_08126eb2
.L_08126e6e:
	movs	r1, #128
	ldr	r3, [pc, #432]
	lsls	r1, r1, #9
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x69bb
	ldr	r2, [pc, #428]
	subs	r3, #1
	str	r3, [r7, #24]
	mov	sl, r0
	mov	r8, r2
	adds	r5, r7, #0
	movs	r6, #2
.L_08126e88:
	ldr	r0, [r5, #0]
	mov	r1, sl
	negs	r0, r0
	asrs	r0, r0, #8
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x2198
	lsls	r1, r1, #9
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x68ea
	subs	r6, #1
	asrs	r3, r2, #7
	subs	r2, r2, r3
	ldr	r3, [r5, #0]
	adds	r2, r2, r0
	adds	r3, r3, r2
	str	r2, [r5, #12]
	stmia	r5!, {r3}
	cmp	r6, #0
	bge.n	.L_08126e88
.L_08126eb2:
	ldr	r0, [r7, #24]
	cmp	r0, #0
	bne.n	.L_08126f50
.L_08126eb8:
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #188
	add	r3, r9
	ldr	r3, [r3, #0]
	cmp	r3, #24
	bgt.n	.L_08126f4c
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	movs	r3, #128
	lsls	r3, r3, #9
	adds	r3, r3, r0
	lsrs	r6, r3, #1
	adds	r0, r5, #0
	mov	r8, r3
	bl	sub_08002090
	ldr	r2, [pc, #324]
	adds	r1, r6, #0
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x6038
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r1, r6, #0
	ldr	r3, [pc, #308]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x683a
	movs	r1, #1
	adds	r3, r2, #0
	ands	r3, r1
	str	r0, [r7, #4]
	cmp	r3, #0
	beq.n	.L_08126f0a
	negs	r3, r2
	str	r3, [r7, #0]
.L_08126f0a:
	ldr	r2, [r7, #4]
	adds	r3, r2, #0
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08126f18
	negs	r3, r2
	str	r3, [r7, #4]
.L_08126f18:
	bl	sub_08014878
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r0, r0, r2
	lsrs	r0, r0, #2
	ldr	r3, [r7, #4]
	str	r0, [r7, #8]
	ldr	r0, [r7, #0]
	asrs	r2, r3, #8
	negs	r0, r0
	negs	r3, r3
	asrs	r1, r0, #7
	asrs	r3, r3, #7
	asrs	r0, r0, #8
	adds	r3, r3, r0
	adds	r1, r1, r2
	str	r3, [r7, #16]
	mov	r2, r8
	movs	r3, #0
	str	r3, [r7, #20]
	lsrs	r3, r2, #13
	adds	r3, #1
	str	r1, [r7, #12]
	str	r3, [r7, #24]
	adds	r0, r3, #0
.L_08126f4c:
	cmp	r0, #0
	beq.n	sub_08126f8c
.L_08126f50:
	ldr	r3, [r7, #0]
	adds	r6, r0, #0
	asrs	r3, r3, #10
	adds	r5, r3, #0
	ldr	r3, [r7, #4]
	adds	r5, #64
	asrs	r3, r3, #10
	adds	r4, r3, #0
	adds	r4, #64
	cmp	r6, #0
	bge.n	.L_08126f6a
	movs	r6, #0
	b.n	.L_08126f70
.L_08126f6a:
	cmp	r6, #6
	ble.n	.L_08126f70
	movs	r6, #6
.L_08126f70:
	ldr	r3, [pc, #184]
	ldr	r2, [pc, #188]
	ldrb	r0, [r3, r6]
	lsls	r3, r6, #2
	ldr	r1, [r2, r3]
	lsrs	r3, r0, #1
	subs	r2, r5, r3
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	add	r1, r9
	subs	r3, r4, r3
	ldr	r0, [sp, #12]
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x9b02
	adds	r7, #28
	subs	r3, #1
	str	r3, [sp, #8]
	cmp	r3, #0
	blt.n	.L_08126f9a
	b.n	.L_08126e34
.L_08126f9a:
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r3, [r3, #0]
	movs	r5, #156
	lsls	r5, r5, #5
	mov	fp, r3
	add	r5, r9
	movs	r7, #2
.L_08126fac:
	ldr	r1, [r5, #0]
	ldr	r3, [r5, #8]
	ldr	r2, [r5, #4]
	adds	r1, r1, r3
	ldr	r3, [r5, #12]
	str	r1, [r5, #0]
	asrs	r4, r1, #10
	ldr	r1, [r5, #16]
	adds	r2, r2, r3
	str	r2, [r5, #4]
	asrs	r6, r2, #10
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_08126fca
	adds	r2, r1, #7
.L_08126fca:
	asrs	r2, r2, #3
	movs	r3, #3
	subs	r0, r3, r2
	cmp	r0, #0
	blt.n	sub_08126ff4
	adds	r3, r1, #1
	str	r3, [r5, #16]
	ldr	r2, [pc, #88]
	lsls	r3, r0, #2
	ldr	r1, [r2, r3]
	movs	r0, #32
	adds	r2, r4, #0
	adds	r3, r6, #0
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	add	r1, r9
	adds	r2, #48
	adds	r3, #48
	ldr	r0, [sp, #12]
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x3f01
	adds	r5, #20
	cmp	r7, #0
	bge.n	.L_08126fac
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #188
	add	r2, r9
	ldr	r3, [r2, #0]
	add	sp, #16
	adds	r3, #1
	str	r3, [r2, #0]
	movs	r2, #158
	lsls	r2, r2, #5
	add	r2, r9
	movs	r3, #1
	str	r3, [r2, #0]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03000230
	.4byte 0x0300021c
	.4byte 0x08129828
	.4byte 0x0812980c
	.2byte 0x9830
	.2byte 0x0812
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #96]
	ldr	r3, [r3, #92]
	cmp	r0, #0
	beq.n	.L_08127060
	movs	r1, #158
	lsls	r1, r1, #5
	adds	r2, r3, r1
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08127060
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r2, #128
	ldr	r1, [pc, #8]
	lsls	r2, r2, #7
	bl	sub_08001c90
.L_08127060:
	pop	{pc}
	movs	r0, r0
	.2byte 0x4000
	.2byte 0x0600
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	str	r0, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #176
	ldr	r2, [r3, #0]
	movs	r1, #152
	movs	r3, #1
	lsls	r1, r1, #5
	str	r3, [r2, #8]
	adds	r1, #208
	movs	r0, #92
	bl	sub_08014d00
	movs	r1, #128
	mov	r9, r0
	lsls	r1, r1, #7
	movs	r0, #96
	movs	r7, #142
	bl	sub_08014cc0
	lsls	r7, r7, #5
	movs	r1, #1
	movs	r2, #15
	mov	fp, r1
	add	r7, r9
	mov	sl, r2
.L_081270ac:
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	movs	r3, #128
	lsls	r3, r3, #9
	adds	r3, r3, r0
	lsrs	r6, r3, #1
	adds	r0, r5, #0
	mov	r8, r3
	bl	sub_08002090
	ldr	r2, [pc, #452]
	adds	r1, r6, #0
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x6038
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r1, r6, #0
	ldr	r3, [pc, #432]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x683a
	mov	r1, fp
	adds	r3, r2, #0
	ands	r3, r1
	str	r0, [r7, #4]
	cmp	r3, #0
	beq.n	.L_081270f0
	negs	r3, r2
	str	r3, [r7, #0]
.L_081270f0:
	ldr	r2, [r7, #4]
	mov	r1, fp
	adds	r3, r2, #0
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08127100
	negs	r3, r2
	str	r3, [r7, #4]
.L_08127100:
	bl	sub_08014878
	movs	r2, #128
	lsls	r2, r2, #8
	adds	r0, r0, r2
	lsrs	r0, r0, #2
	ldr	r3, [r7, #4]
	str	r0, [r7, #8]
	ldr	r0, [r7, #0]
	asrs	r2, r3, #8
	negs	r0, r0
	negs	r3, r3
	asrs	r1, r0, #7
	asrs	r3, r3, #7
	asrs	r0, r0, #8
	adds	r1, r1, r2
	adds	r3, r3, r0
	str	r1, [r7, #12]
	str	r3, [r7, #16]
	mov	r1, r8
	movs	r3, #0
	movs	r2, #1
	str	r3, [r7, #20]
	negs	r2, r2
	lsrs	r3, r1, #13
	adds	r3, #1
	add	sl, r2
	str	r3, [r7, #24]
	mov	r3, sl
	adds	r7, #28
	cmp	r3, #0
	bge.n	.L_081270ac
	movs	r1, #128
	movs	r5, #156
	ldr	r7, [pc, #324]
	lsls	r1, r1, #5
	lsls	r5, r5, #5
	movs	r2, #2
	mov	r8, r1
	movs	r6, #0
	add	r5, r9
	mov	sl, r2
.L_08127154:
	adds	r0, r6, #0
	bl	sub_08002090
	mov	r1, r8
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x6028
	adds	r0, r6, #0
	bl	sub_08002096
	mov	r1, r8
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x6068
	adds	r0, r6, #0
	bl	sub_08002090
	movs	r1, #128
	lsls	r1, r1, #2
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x60a8
	adds	r0, r6, #0
	bl	sub_08002096
	movs	r1, #128
	lsls	r1, r1, #2
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x23aa
	movs	r1, #1
	lsls	r3, r3, #7
	negs	r1, r1
	adds	r3, #85
	add	sl, r1
	movs	r2, #0
	adds	r6, r6, r3
	mov	r3, sl
	str	r0, [r5, #12]
	str	r2, [r5, #16]
	adds	r5, #20
	cmp	r3, #0
	bge.n	.L_08127154
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #188
	add	r3, r9
	str	r2, [r3, #0]
	movs	r3, #158
	lsls	r3, r3, #5
	add	r3, r9
	str	r2, [r3, #0]
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #204
	add	r3, r9
	str	r2, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r0, [r3, #96]
	ldr	r3, [pc, #188]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4e2f
	adds	r0, r6, #0
	bl	sub_08013300
	adds	r5, r0, #0
	movs	r0, #160
	adds	r1, r5, #0
	ldr	r3, [pc, #176]
	movs	r2, #128
	lsls	r0, r0, #19
	adds	r5, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4649
	adds	r0, r5, #0
	bl	sub_0801587c
	ldr	r1, [sp, #4]
	cmp	r1, #1
	beq.n	.L_08127214
	cmp	r1, #1
	bgt.n	.L_08127208
	cmp	r1, #0
	beq.n	.L_08127210
	b.n	.L_0812721c
.L_08127208:
	ldr	r2, [sp, #4]
	cmp	r2, #2
	beq.n	.L_08127218
	b.n	.L_0812721c
.L_08127210:
	ldr	r0, [pc, #136]
	b.n	.L_0812721e
.L_08127214:
	adds	r0, r6, #0
	b.n	.L_0812721e
.L_08127218:
	ldr	r0, [pc, #132]
	b.n	.L_0812721e
.L_0812721c:
	ldr	r0, [pc, #132]
.L_0812721e:
	bl	sub_08013300
	adds	r5, r0, #0
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	movs	r1, #160
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r5, #0
	lsls	r1, r1, #19
	adds	r2, #32
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #0
	subs	r3, #172
	str	r2, [r3, #0]
	adds	r3, #4
	str	r2, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #1
	subs	r3, #12
	strh	r1, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r1, [r3, #0]
	movs	r0, #104
	movs	r1, #3
	movs	r5, #144
	bl	sub_08138048
	lsls	r5, r5, #3
	movs	r1, #19
	movs	r0, #188
	bl	sub_08138048
	adds	r1, r5, #0
	ldr	r0, [pc, #56]
	bl	sub_080145a8
	adds	r1, r5, #0
	ldr	r0, [pc, #52]
	bl	sub_080145a8
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0300021c
	.4byte 0x03000258
	.4byte 0x0000018d
	.4byte 0x03000730
	.4byte 0x0000018c
	.4byte 0x0000018e
	.4byte 0x0000018f
	.4byte 0x08126e01
	.2byte 0x7039
	.2byte 0x0812
	push	{lr}
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r3, [pc, #56]
	ldr	r0, [pc, #56]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	movs	r0, #96
	bl	sub_0801314c
	movs	r0, #92
	bl	sub_0801314c
	ldr	r3, [pc, #20]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r0, [pc, #24]
	bl	sub_08014644
	ldr	r0, [pc, #20]
	bl	sub_08014644
	b.n	.L_08127300
	.4byte 0x00001341
	.4byte 0x03000258
	.4byte 0x06004000
	.4byte 0x08127039
	.2byte 0x6e01
	.2byte 0x0812
.L_08127300:
	pop	{pc}
	movs	r0, r0
	bx	lr
	.2byte 0x0000

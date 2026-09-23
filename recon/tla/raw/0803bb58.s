.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_080143ac, 0x080143ac
	.set sub_08014d78, 0x08014d78
	.set sub_08038eb0, 0x08038eb0
	.set sub_08039500, 0x08039500
	.set sub_0803a404, 0x0803a404
	.set sub_0803a8fc, 0x0803a8fc
	.global Func_0803bb58
	.thumb_func
Func_0803bb58:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #108
	str	r2, [sp, #20]
	str	r1, [sp, #24]
	mov	r8, r3
	movs	r3, #192
	lsls	r3, r3, #18
	add	r6, sp, #44
	ldr	r1, [r3, #60]
	movs	r3, #15
	str	r3, [r6, #0]
	str	r3, [r6, #4]
	str	r3, [r6, #8]
	str	r3, [r6, #12]
	str	r3, [r6, #16]
	str	r3, [r6, #20]
	str	r3, [r6, #24]
	str	r3, [r6, #28]
	str	r3, [r6, #32]
	str	r3, [r6, #36]
	str	r3, [r6, #40]
	str	r3, [r6, #44]
	str	r3, [r6, #48]
	str	r3, [r6, #52]
	str	r3, [r6, #56]
	str	r3, [r6, #60]
	ldr	r3, [pc, #512]
	movs	r2, #0
	mov	ip, r3
	mov	r3, sp
	adds	r3, #28
	mov	r9, r2
	str	r2, [sp, #16]
	str	r3, [sp, #12]
	movs	r2, #36
	movs	r4, #0
	add	r2, sp
	adds	r5, r0, #0
	movs	r7, #0
	movs	r0, #0
	mov	fp, r2
	mov	sl, r4
.L_0803bbb8:
	movs	r2, #244
	lsls	r3, r5, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldrh	r2, [r1, r3]
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r5, #1
	adds	r3, #255
	ands	r5, r3
	cmp	r2, #31
	bls.n	.L_0803bbfc
	cmp	r2, #176
	beq.n	.L_0803bbfc
	cmp	r2, #32
	bne.n	.L_0803bbde
	adds	r7, #5
	adds	r0, #1
	b.n	.L_0803bbb8
.L_0803bbde:
	ldr	r3, [pc, #448]
	subs	r2, #32
	lsls	r2, r2, #5
	ldrh	r2, [r3, r2]
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #60
	ldrh	r3, [r3, r1]
	cmp	r3, #1
	beq.n	.L_0803bbf6
	cmp	r3, #5
	bne.n	.L_0803bbf8
.L_0803bbf6:
	adds	r2, #1
.L_0803bbf8:
	adds	r7, r7, r2
	b.n	.L_0803bbb8
.L_0803bbfc:
	cmp	r2, #28
	bhi.n	.L_0803bbb8
	lsls	r3, r2, #2
	mov	r2, ip
	ldr	r3, [r3, r2]
	mov	pc, r3
	pop	{r1, r4, r5, r7}
	lsrs	r3, r0, #32
	pop	{r2, r3, r6, r7}
	lsrs	r3, r0, #32
	.2byte 0xbbb8
	lsrs	r3, r0, #32
	pop	{r2, r3, r4, r5, r6}
	lsrs	r3, r0, #32
	.2byte 0xbbb8
	lsrs	r3, r0, #32
	.2byte 0xbbb8
	lsrs	r3, r0, #32
	.2byte 0xbbb8
	lsrs	r3, r0, #32
	.2byte 0xbbb8
	lsrs	r3, r0, #32
	pop	{r2, pc}
	lsrs	r3, r0, #32
	pop	{r4, pc}
	lsrs	r3, r0, #32
	pop	{r2, pc}
	lsrs	r3, r0, #32
	.2byte 0xbbb8
	lsrs	r3, r0, #32
	.2byte 0xbbb8
	lsrs	r3, r0, #32
	.2byte 0xbbb8
	lsrs	r3, r0, #32
	pop	{r1, r3, r4, r5, r6, r7}
	lsrs	r3, r0, #32
	pop	{r2, pc}
	lsrs	r3, r0, #32
	.2byte 0xbbb8
	lsrs	r3, r0, #32
	pop	{r2, pc}
	lsrs	r3, r0, #32
	.2byte 0xbbb8
	lsrs	r3, r0, #32
	.2byte 0xbbb8
	lsrs	r3, r0, #32
	.2byte 0xbbb8
	lsrs	r3, r0, #32
	.2byte 0xbbb8
	lsrs	r3, r0, #32
	.2byte 0xbbb8
	lsrs	r3, r0, #32
	.2byte 0xbbb8
	lsrs	r3, r0, #32
	.2byte 0xbbb8
	lsrs	r3, r0, #32
	.2byte 0xbbb8
	lsrs	r3, r0, #32
	.2byte 0xbbb8
	lsrs	r3, r0, #32
	.2byte 0xbbb8
	lsrs	r3, r0, #32
	pop	{r1, r3, r4, r5, r6, r7}
	lsrs	r3, r0, #32
	mov	r3, fp
	mov	r2, sl
	adds	r0, #1
	strh	r0, [r3, r2]
	ldr	r3, [sp, #12]
	strh	r7, [r3, r2]
	cmp	r4, #0
	bne.n	.L_0803bc92
	cmp	r9, r7
	bcs.n	.L_0803bc92
	mov	r9, r7
.L_0803bc92:
	ldr	r3, [sp, #16]
	cmp	r3, #2
	bhi.n	.L_0803bca0
	adds	r3, #1
	str	r3, [sp, #16]
	lsls	r3, r3, #1
	mov	sl, r3
.L_0803bca0:
	lsls	r2, r4, #2
	ldr	r3, [r6, r2]
	movs	r0, #0
	adds	r3, #15
	str	r3, [r6, r2]
	ldr	r2, [pc, #240]
	movs	r7, #0
	mov	ip, r2
	b.n	.L_0803bbb8
	mov	r3, fp
	mov	r2, sl
	adds	r0, #1
	strh	r0, [r3, r2]
	ldr	r3, [sp, #12]
	strh	r7, [r3, r2]
	cmp	r4, #0
	bne.n	.L_0803bcc8
	cmp	r9, r7
	bcs.n	.L_0803bcc8
	mov	r9, r7
.L_0803bcc8:
	adds	r4, #1
	b.n	.L_0803bd34
	mov	r3, fp
	mov	r2, sl
	adds	r0, #1
	strh	r0, [r3, r2]
	ldr	r3, [sp, #12]
	strh	r7, [r3, r2]
	cmp	r4, #0
	bne.n	.L_0803bce2
	cmp	r9, r7
	bcs.n	.L_0803bce2
	mov	r9, r7
.L_0803bce2:
	adds	r4, #1
	str	r0, [sp, #8]
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	bl	sub_0803a404
	ldr	r3, [pc, #172]
	ldr	r0, [sp, #8]
	mov	ip, r3
	ldr	r1, [sp, #4]
	ldr	r4, [sp, #0]
	b.n	.L_0803bbb8
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r5, #1
	adds	r3, #255
	ands	r5, r3
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r5, #1
	adds	r3, #255
	ands	r5, r3
	b.n	.L_0803bbb8
	movs	r2, #244
	lsls	r3, r5, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldrh	r2, [r1, r3]
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #60
	adds	r3, r3, r1
	strh	r2, [r3, #0]
	movs	r3, #128
	ldr	r2, [pc, #116]
	lsls	r3, r3, #1
	adds	r5, #1
	adds	r3, #255
	ands	r5, r3
	mov	ip, r2
	b.n	.L_0803bbb8
.L_0803bd34:
	ldrb	r3, [r1, #4]
	cmp	r3, #0
	beq.n	.L_0803bd3e
	movs	r3, #2
	add	r9, r3
.L_0803bd3e:
	movs	r0, #0
	cmp	r0, r4
	bcs.n	.L_0803bd6a
	adds	r1, r6, #0
	adds	r5, r1, #0
.L_0803bd48:
	cmp	r0, #0
	bne.n	.L_0803bd54
	ldr	r3, [r5, #0]
	ldr	r2, [sp, #20]
	str	r3, [r2, #0]
	b.n	.L_0803bd62
.L_0803bd54:
	ldr	r2, [sp, #20]
	ldr	r3, [r2, #0]
	ldr	r2, [r1, #0]
	cmp	r3, r2
	bcs.n	.L_0803bd62
	ldr	r3, [sp, #20]
	str	r2, [r3, #0]
.L_0803bd62:
	adds	r0, #1
	adds	r1, #4
	cmp	r0, r4
	bcc.n	.L_0803bd48
.L_0803bd6a:
	ldr	r3, [sp, #24]
	mov	r2, r9
	str	r2, [r3, #0]
	mov	r3, r9
	adds	r3, #19
	lsrs	r3, r3, #3
	lsls	r3, r3, #3
	subs	r3, #16
	mov	r2, r8
	mov	r9, r3
	cmp	r2, #0
	beq.n	.L_0803bdd2
	movs	r6, #0
	movs	r5, #0
.L_0803bd86:
	mov	r2, fp
	ldrh	r3, [r5, r2]
	cmp	r3, #1
	bhi.n	.L_0803bda4
	ldr	r3, [pc, #8]
	mov	r2, r8
	strh	r3, [r2, #0]
	b.n	.L_0803bdc4
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x0803bc08
	.2byte 0xa4e0
	.2byte 0x0805
.L_0803bda4:
	ldr	r2, [sp, #12]
	ldrh	r3, [r5, r2]
	mov	r2, r9
	subs	r0, r2, r3
	subs	r0, #4
	cmp	r0, #0
	bge.n	.L_0803bdb4
	movs	r0, #0
.L_0803bdb4:
	mov	r3, fp
	ldrh	r1, [r5, r3]
	lsls	r0, r0, #8
	subs	r1, #1
	bl	sub_08002054
	mov	r2, r8
	strh	r0, [r2, #0]
.L_0803bdc4:
	movs	r3, #2
	add	r8, r3
	ldr	r2, [sp, #16]
	adds	r6, #1
	adds	r5, #2
	cmp	r6, r2
	bls.n	.L_0803bd86
.L_0803bdd2:
	bl	sub_0803a404
	add	sp, #108
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #144
	str	r2, [sp, #12]
	mov	r9, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #60]
	mov	r8, r0
	movs	r0, #154
	lsls	r0, r0, #5
	adds	r3, r6, r0
	ldrh	r3, [r3, #0]
	adds	r7, r1, #0
	movs	r1, #240
	str	r3, [sp, #8]
	lsls	r1, r1, #4
	adds	r1, #56
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	ldr	r2, [sp, #176]
	str	r3, [sp, #4]
	cmp	r2, #1
	beq.n	.L_0803be96
	mov	r3, r8
	ldrh	r2, [r3, #22]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0803be96
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #148
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #0]
	cmp	r3, r8
	bne.n	.L_0803be48
	ldr	r0, [pc, #316]
	bl	sub_08013300
	movs	r0, #3
	mov	fp, r0
	cmp	r7, #32
	bne.n	.L_0803be48
	b.n	.L_0803c058
.L_0803be48:
	ldr	r0, [pc, #300]
	bl	sub_08013300
	movs	r1, #4
	mov	sl, r0
	mov	fp, r1
	cmp	r7, #32
	bne.n	.L_0803be5a
	b.n	.L_0803c058
.L_0803be5a:
	ldr	r5, [pc, #288]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #268]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r2, sl
	str	r2, [sp, #0]
	adds	r1, r7, #0
	ldr	r2, [sp, #12]
	mov	r3, r9
	mov	r0, r8
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c05
	adds	r0, r6, #0
	bl	sub_08013164
	adds	r0, r5, #0
	b.n	.L_0803c05a
.L_0803be96:
	movs	r3, #5
	mov	fp, r3
	cmp	r7, #32
	bne.n	.L_0803bea0
	b.n	.L_0803c058
.L_0803bea0:
	bl	sub_08038eb0
	adds	r5, r0, #0
	movs	r0, #0
	cmp	r5, #0
	bne.n	.L_0803beae
	b.n	.L_0803c05a
.L_0803beae:
	ldr	r0, [pc, #212]
	ldr	r2, [pc, #212]
	subs	r3, r5, r6
	adds	r3, r3, r0
	adds	r1, r3, #0
	muls	r1, r2
	movs	r3, #1
	movs	r2, #0
	strb	r3, [r5, #5]
	strb	r2, [r5, #4]
	ldr	r3, [sp, #176]
	mov	sl, r1
	cmp	r3, #1
	bne.n	.L_0803bed4
	movs	r0, #1
	movs	r3, #2
	mov	fp, r0
	strb	r3, [r5, #5]
	b.n	.L_0803bf20
.L_0803bed4:
	movs	r1, #240
	lsls	r1, r1, #4
	adds	r1, #60
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	cmp	r3, #3
	beq.n	.L_0803bef6
	cmp	r3, #3
	bgt.n	.L_0803beec
	cmp	r3, #2
	beq.n	.L_0803bf0a
	b.n	.L_0803bf10
.L_0803beec:
	cmp	r3, #4
	beq.n	.L_0803befc
	cmp	r3, #5
	beq.n	.L_0803bf06
	b.n	.L_0803bf10
.L_0803bef6:
	movs	r3, #5
	strb	r3, [r5, #5]
	b.n	.L_0803bf10
.L_0803befc:
	movs	r3, #6
	strb	r3, [r5, #5]
	movs	r3, #8
	strh	r3, [r5, #12]
	b.n	.L_0803bf10
.L_0803bf06:
	movs	r3, #7
	b.n	.L_0803bf0c
.L_0803bf0a:
	movs	r3, #4
.L_0803bf0c:
	strb	r3, [r5, #5]
	strh	r2, [r5, #12]
.L_0803bf10:
	add	r1, sp, #16
	adds	r0, r7, #0
	bl	sub_0803a8fc
	cmp	r0, #0
	bne.n	.L_0803bf1e
	movs	r0, #1
.L_0803bf1e:
	mov	fp, r0
.L_0803bf20:
	ldrb	r3, [r5, #5]
	cmp	r3, #2
	bne.n	.L_0803bf90
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #70
	adds	r6, r6, r2
	ldrh	r3, [r6, #0]
	adds	r7, r5, #0
	adds	r7, #16
	cmp	r3, #99
	bne.n	.L_0803bf3e
	bl	sub_080143ac
	strh	r0, [r6, #0]
.L_0803bf3e:
	mov	r3, r8
	ldrh	r2, [r3, #12]
	movs	r0, #255
	ldrh	r3, [r3, #8]
	lsls	r0, r0, #8
	adds	r0, #254
	adds	r3, r3, r0
	adds	r2, r2, r3
	ldr	r3, [pc, #36]
	lsls	r2, r2, #3
	adds	r2, #4
	ands	r2, r3
	ldrh	r1, [r7, #6]
	ldr	r3, [pc, #48]
	ands	r3, r1
	mov	r1, r8
	orrs	r3, r2
	ldrb	r2, [r1, #10]
	strh	r3, [r7, #6]
	ldrb	r3, [r1, #14]
	adds	r2, #254
	adds	r3, r3, r2
	lsls	r3, r3, #3
	subs	r3, #1
	strb	r3, [r7, #4]
	b.n	.L_0803c032
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x00000013
	.4byte 0x0000031c
	.4byte 0x080385e0
	.4byte 0xfffff8d0
	.4byte 0xb6db6db7
	.2byte 0xfe00
	.2byte 0xffff
.L_0803bf90:
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #72
	adds	r3, r6, r2
	ldrh	r1, [r3, #0]
	ldr	r2, [pc, #76]
	add	r1, sl
	lsls	r1, r1, #5
	adds	r1, r1, r2
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	add	r0, sp, #16
	adds	r2, #32
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldrb	r2, [r6, #5]
	movs	r3, #4
	adds	r1, r5, #0
	ands	r3, r2
	adds	r1, #20
	cmp	r3, #0
	beq.n	.L_0803bfec
	ldr	r0, [sp, #4]
	lsrs	r3, r0, #1
	mov	r0, r8
	ldrh	r2, [r0, #14]
	add	r3, r9
	lsls	r2, r2, #3
	adds	r3, r3, r2
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #254
	adds	r3, r3, r2
	ldr	r2, [pc, #8]
	orrs	r3, r2
	strh	r3, [r1, #0]
	adds	r1, #2
	b.n	.L_0803c008
	movs	r0, r0
	.4byte 0x00000400
	.2byte 0x0000
	.2byte 0x0601
.L_0803bfec:
	ldr	r0, [sp, #4]
	lsrs	r3, r0, #1
	mov	r0, r8
	ldrh	r2, [r0, #14]
	add	r3, r9
	lsls	r2, r2, #3
	adds	r3, r3, r2
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #254
	adds	r3, r3, r2
	strh	r3, [r1, #0]
	adds	r1, r5, #0
	adds	r1, #22
.L_0803c008:
	ldr	r3, [sp, #8]
	ldr	r0, [sp, #12]
	lsrs	r2, r3, #1
	adds	r2, r0, r2
	mov	r0, r8
	ldrh	r3, [r0, #12]
	adds	r7, r5, #0
	lsls	r3, r3, #3
	adds	r2, r2, r3
	ldr	r3, [pc, #56]
	adds	r2, #2
	orrs	r2, r3
	strh	r2, [r1, #0]
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #72
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	adds	r7, #16
	add	r3, sl
	strh	r3, [r1, #2]
.L_0803c032:
	movs	r3, #253
	strb	r3, [r5, #15]
	ldrh	r3, [r7, #6]
	movs	r2, #0
	lsls	r3, r3, #23
	lsrs	r3, r3, #23
	strh	r3, [r5, #6]
	str	r2, [r5, #0]
	ldrb	r3, [r7, #4]
	mov	r0, r8
	strh	r3, [r5, #8]
	mov	r3, sl
	strb	r3, [r5, #14]
	adds	r1, r5, #0
	bl	sub_08039500
	b.n	.L_0803c058
	.2byte 0x4000
	.2byte 0x0000
.L_0803c058:
	mov	r0, fp
.L_0803c05a:
	add	sp, #144
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #140
	str	r2, [sp, #8]
	str	r3, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #60]
	mov	sl, r1
	movs	r1, #154
	lsls	r1, r1, #5
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	movs	r2, #240
	str	r3, [sp, #0]
	lsls	r2, r2, #4
	adds	r2, #56
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	mov	fp, r0
	mov	r8, r3
	bl	sub_08038eb0
	adds	r7, r0, #0
	movs	r0, #0
	cmp	r7, #0
	beq.n	.L_0803c160
	ldr	r1, [pc, #168]
	ldr	r2, [pc, #172]
	subs	r3, r7, r6
	adds	r3, r3, r1
	adds	r1, r3, #0
	muls	r1, r2
	add	r5, sp, #12
	movs	r3, #1
	movs	r2, #0
	mov	r9, r1
	strb	r3, [r7, #5]
	strb	r2, [r7, #4]
	adds	r1, r5, #0
	mov	r0, sl
	bl	sub_0803a8fc
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #72
	adds	r4, r6, r3
	ldrh	r1, [r4, #0]
	ldr	r2, [pc, #136]
	add	r1, r9
	lsls	r1, r1, #5
	adds	r1, r1, r2
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	mov	sl, r0
	adds	r3, #212
	adds	r0, r5, #0
	adds	r2, #32
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [sp, #4]
	mov	r1, r8
	lsrs	r3, r1, #1
	mov	r1, fp
	adds	r3, r2, r3
	ldrh	r2, [r1, #14]
	mov	r0, fp
	lsls	r2, r2, #3
	adds	r3, r3, r2
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #254
	adds	r3, r3, r2
	strh	r3, [r7, #20]
	ldr	r3, [sp, #0]
	ldr	r1, [sp, #8]
	lsrs	r2, r3, #1
	adds	r2, r1, r2
	mov	r1, fp
	ldrh	r3, [r1, #12]
	adds	r1, r7, #0
	lsls	r3, r3, #3
	adds	r2, r2, r3
	ldr	r3, [pc, #48]
	adds	r2, #2
	orrs	r2, r3
	strh	r2, [r7, #22]
	adds	r2, r7, #0
	ldrh	r3, [r4, #0]
	adds	r2, #16
	add	r3, r9
	strh	r3, [r7, #24]
	movs	r3, #253
	strb	r3, [r7, #15]
	ldrh	r3, [r2, #6]
	lsls	r3, r3, #23
	lsrs	r3, r3, #23
	strh	r3, [r7, #6]
	ldrb	r3, [r2, #4]
	mov	r2, r9
	strh	r3, [r7, #8]
	movs	r3, #0
	strb	r2, [r7, #14]
	str	r3, [r7, #0]
	bl	sub_08039500
	mov	r0, sl
	b.n	.L_0803c160
	movs	r0, r0
	.4byte 0x00004000
	.4byte 0xfffff8d0
	.4byte 0xb6db6db7
	.2byte 0x0000
	.2byte 0x0601
.L_0803c160:
	add	sp, #140
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000

.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_080145a8, 0x080145a8
	.set sub_08014d00, 0x08014d00
	.set sub_081a8264, 0x081a8264
	.set sub_081a8278, 0x081a8278
	.global Func_081a7870
	.thumb_func
Func_081a7870:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #4
	adds	r7, r0, #0
	adds	r6, r1, #0
	adds	r5, r2, #0
	mov	r8, r3
	cmp	r3, #0
	ble.n	.L_081a78b2
	ldr	r1, [pc, #52]
	movs	r2, #160
	lsls	r2, r2, #3
	mov	sl, r1
	adds	r2, #255
.L_081a7890:
	movs	r1, #0
	ldrsh	r3, [r7, r1]
	movs	r1, #0
	ldrsh	r0, [r6, r1]
	str	r2, [sp, #0]
	subs	r0, r0, r3
	mov	r1, r8
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x9a00
	strh	r0, [r5, #0]
	subs	r2, #1
	adds	r7, #2
	adds	r6, #2
	adds	r5, #2
	cmp	r2, #0
	bge.n	.L_081a7890
.L_081a78b2:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0528
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #128
	ldr	r5, [r3, #0]
	movs	r2, #192
	movs	r1, #224
	lsls	r2, r2, #6
	lsls	r1, r1, #5
	adds	r2, #1
	adds	r4, r5, r1
	adds	r1, r5, r2
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	bne.n	.L_081a78e2
	b.n	.L_081a7a18
.L_081a78e2:
	adds	r2, #1
	adds	r3, r5, r2
	ldrb	r2, [r3, #0]
	adds	r2, #1
	strb	r2, [r3, #0]
	lsls	r2, r2, #24
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	asrs	r2, r2, #24
	cmp	r2, r3
	bge.n	.L_081a791a
	movs	r3, #128
	movs	r6, #160
	lsls	r3, r3, #3
	lsls	r6, r6, #3
	adds	r1, r5, r3
	movs	r0, #0
	adds	r6, #255
.L_081a7906:
	ldrh	r3, [r1, #0]
	ldrh	r2, [r4, #0]
	adds	r0, #1
	adds	r3, r3, r2
	strh	r3, [r1, #0]
	adds	r4, #2
	adds	r1, #2
	cmp	r0, r6
	ble.n	.L_081a7906
	b.n	.L_081a793e
.L_081a791a:
	movs	r3, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #5
	lsls	r2, r2, #3
	lsls	r3, r3, #19
	adds	r0, r5, r1
	adds	r3, #212
	adds	r1, r5, r2
	ldr	r2, [pc, #68]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #192
	lsls	r3, r3, #6
	adds	r3, #1
	adds	r2, r5, r3
	movs	r3, #0
	strb	r3, [r2, #0]
.L_081a793e:
	movs	r1, #192
	lsls	r1, r1, #6
	adds	r3, r5, r1
	ldrb	r2, [r3, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r3, r3, #10
	movs	r2, #160
	adds	r3, r5, r3
	lsls	r2, r2, #6
	adds	r4, r3, r2
	ldr	r7, [pc, #20]
	movs	r3, #248
	movs	r2, #128
	ldr	r6, [pc, #20]
	lsls	r3, r3, #7
	movs	r0, #128
	lsls	r2, r2, #3
	mov	ip, r3
	lsls	r0, r0, #2
	adds	r1, r5, r2
	b.n	.L_081a7978
	movs	r0, r0
	.4byte 0x000003e0
	.4byte 0x0000001f
	.2byte 0x0300
	.2byte 0x8400
.L_081a7978:
	ldrh	r3, [r1, #0]
	mov	r2, ip
	ands	r2, r3
	ldrh	r3, [r1, #2]
	subs	r0, #1
	lsls	r3, r3, #16
	asrs	r3, r3, #21
	ands	r3, r7
	orrs	r2, r3
	ldrh	r3, [r1, #4]
	adds	r1, #6
	lsls	r3, r3, #16
	asrs	r3, r3, #26
	ands	r3, r6
	orrs	r2, r3
	strh	r2, [r4, #0]
	adds	r4, #2
	cmp	r0, #0
	bne.n	.L_081a7978
	movs	r3, #192
	lsls	r3, r3, #6
	adds	r1, r5, r3
	ldrb	r3, [r1, #0]
	movs	r2, #1
	eors	r3, r2
	strb	r3, [r1, #0]
	ldrb	r3, [r1, #0]
	movs	r1, #160
	lsls	r3, r3, #10
	adds	r0, r5, r3
	ldr	r5, [pc, #100]
	lsls	r1, r1, #6
	adds	r6, r0, r1
	ldr	r4, [pc, #100]
	ldrh	r3, [r4, #0]
	adds	r1, r3, #0
	strh	r4, [r4, #0]
	ldrh	r2, [r5, #0]
	cmp	r2, #31
	bgt.n	.L_081a79e6
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r5
	adds	r3, #4
	adds	r2, #1
	stmia	r3!, {r6}
	strh	r2, [r5, #0]
	movs	r2, #160
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #128
	str	r2, [r3, #0]
.L_081a79e6:
	strh	r1, [r4, #0]
	ldrh	r3, [r4, #0]
	adds	r6, r3, #0
	strh	r4, [r4, #0]
	ldrh	r2, [r5, #0]
	cmp	r2, #31
	bgt.n	.L_081a7a16
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	movs	r1, #168
	adds	r2, #1
	adds	r3, r3, r5
	lsls	r1, r1, #6
	adds	r3, #4
	strh	r2, [r5, #0]
	adds	r2, r0, r1
	stmia	r3!, {r2}
	ldr	r2, [pc, #24]
	stmia	r3!, {r2}
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #128
	str	r2, [r3, #0]
.L_081a7a16:
	strh	r6, [r4, #0]
.L_081a7a18:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x020038e0
	.4byte 0x04000208
	.2byte 0x0200
	.2byte 0x0500
.L_081a7a28:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r9, r1
	mov	r8, r2
	movs	r1, #128
	adds	r2, r3, #0
	movs	r3, #128
	sub	sp, #48
	lsls	r1, r1, #2
	lsls	r3, r3, #8
	str	r1, [sp, #44]
	cmp	r0, r3
	bne.n	.L_081a7a50
	mov	r1, r9
	ldrh	r0, [r1, #0]
.L_081a7a50:
	cmp	r2, #1
	bne.n	.L_081a7a5c
	movs	r3, #128
	lsls	r3, r3, #1
	str	r3, [sp, #44]
	b.n	.L_081a7a72
.L_081a7a5c:
	cmp	r2, #2
	bne.n	.L_081a7a72
	movs	r1, #192
	lsls	r1, r1, #3
	add	r8, r1
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r3, #128
	str	r1, [sp, #44]
	lsls	r3, r3, #2
	add	r9, r3
.L_081a7a72:
	movs	r3, #128
	lsls	r3, r3, #8
	cmp	r0, r3
	bcs.n	.L_081a7ace
	ldr	r2, [pc, #56]
	adds	r3, r0, #0
	ands	r3, r2
	movs	r2, #2
	mov	r1, r8
	add	r8, r2
	ldr	r2, [pc, #48]
	strh	r3, [r1, #0]
	adds	r3, r0, #0
	ands	r3, r2
	mov	r1, r8
	lsls	r3, r3, #5
	strh	r3, [r1, #0]
	ldr	r3, [pc, #36]
	movs	r2, #2
	add	r8, r2
	ands	r0, r3
	lsls	r3, r0, #10
	mov	r1, r8
	strh	r3, [r1, #0]
	ldr	r3, [sp, #44]
	add	r8, r2
	subs	r3, #1
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r2, r2, #1
	movs	r4, #128
	movs	r3, #128
	b.n	.L_081a7ac0
	.4byte 0x00007c00
	.4byte 0x000003e0
	.2byte 0x001f
	.2byte 0x0000
.L_081a7ac0:
	lsls	r4, r4, #24
	mov	r0, r8
	lsrs	r2, r2, #1
	lsls	r3, r3, #19
	adds	r3, #212
	subs	r0, #6
	b.n	.L_081a8136
.L_081a7ace:
	movs	r3, #128
	lsls	r3, r3, #13
	cmp	r0, r3
	bcc.n	.L_081a7ad8
	b.n	.L_081a7f16
.L_081a7ad8:
	ldr	r1, [pc, #832]
	adds	r0, r0, r1
	cmp	r0, #6
	bls.n	.L_081a7ae2
	b.n	.L_081a7ec8
.L_081a7ae2:
	ldr	r2, [pc, #828]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x081a7b08
	.4byte 0x081a7b56
	.4byte 0x081a7bf2
	.4byte 0x081a7c78
	.4byte 0x081a7d16
	.4byte 0x081a7da0
	.2byte 0x7e34
	.2byte 0x081a
	ldr	r3, [sp, #44]
	movs	r2, #0
	mov	fp, r2
	cmp	fp, r3
	bcc.n	.L_081a7b14
	b.n	.L_081a813e
.L_081a7b14:
	ldr	r7, [pc, #780]
	mov	r5, r8
.L_081a7b18:
	mov	r1, r9
	ldrh	r6, [r1, #0]
	movs	r3, #248
	lsls	r0, r6, #11
	lsls	r3, r3, #8
	movs	r2, #2
	ands	r0, r3
	movs	r3, #248
	lsls	r3, r3, #9
	add	r9, r2
	lsls	r2, r6, #7
	ands	r2, r3
	movs	r3, #248
	lsls	r3, r3, #7
	ands	r3, r6
	adds	r0, r0, r2
	adds	r0, r0, r3
	movs	r1, #7
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x1c06
	strh	r6, [r5, #0]
	strh	r6, [r5, #2]
	strh	r6, [r5, #4]
	ldr	r1, [sp, #44]
	movs	r3, #1
	add	fp, r3
	adds	r5, #6
	cmp	fp, r1
	bcc.n	.L_081a7b18
	b.n	.L_081a813e
	ldr	r3, [sp, #44]
	movs	r2, #0
	mov	fp, r2
	cmp	fp, r3
	bcc.n	.L_081a7b62
	b.n	.L_081a813e
.L_081a7b62:
	ldr	r2, [pc, #708]
	movs	r1, #31
	mov	sl, r1
.L_081a7b68:
	mov	r3, r9
	ldrh	r6, [r3, #0]
	movs	r1, #2
	mov	r3, sl
	adds	r4, r6, #0
	lsrs	r0, r6, #5
	ands	r4, r3
	ands	r0, r3
	add	r9, r1
	lsrs	r3, r6, #10
	mov	r1, sl
	ands	r3, r1
	adds	r0, r4, r0
	adds	r0, r0, r3
	str	r2, [sp, #4]
	ldr	r3, [pc, #668]
	movs	r1, #10
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c06
	lsls	r3, r6, #2
	adds	r4, r3, #5
	lsls	r3, r6, #1
	adds	r3, r3, r6
	adds	r5, r3, #5
	adds	r7, r5, #0
	ldr	r2, [sp, #4]
	cmp	r4, #7
	bgt.n	.L_081a7ba4
	movs	r4, #8
.L_081a7ba4:
	cmp	r5, #7
	bgt.n	.L_081a7bb0
	movs	r7, #8
	cmp	r5, #7
	bgt.n	.L_081a7bb0
	movs	r5, #8
.L_081a7bb0:
	cmp	r4, #28
	ble.n	.L_081a7bb6
	movs	r4, #28
.L_081a7bb6:
	cmp	r7, #28
	ble.n	.L_081a7bbc
	movs	r7, #28
.L_081a7bbc:
	cmp	r5, #28
	ble.n	.L_081a7bc2
	movs	r5, #28
.L_081a7bc2:
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r3, #2
	add	r8, r3
	lsls	r3, r7, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r3, #2
	add	r8, r3
	lsls	r3, r4, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r3, #2
	add	r8, r3
	ldr	r3, [sp, #44]
	movs	r1, #1
	add	fp, r1
	cmp	fp, r3
	bcc.n	.L_081a7b68
	b.n	.L_081a813e
	ldr	r2, [sp, #44]
	movs	r1, #0
	mov	fp, r1
	cmp	fp, r2
	bcc.n	.L_081a7bfe
	b.n	.L_081a813e
.L_081a7bfe:
	movs	r3, #31
	mov	sl, r3
.L_081a7c02:
	mov	r1, r9
	ldrh	r6, [r1, #0]
	mov	r3, sl
	adds	r4, r6, #0
	ands	r4, r3
	lsrs	r7, r6, #5
	lsrs	r5, r6, #10
	ands	r7, r3
	ands	r5, r3
	lsrs	r3, r4, #1
	subs	r4, r4, r3
	movs	r2, #2
	movs	r1, #3
	adds	r0, r7, #0
	add	r9, r2
	str	r4, [sp, #0]
	bl	sub_08002054
	ldr	r4, [sp, #0]
	subs	r7, r7, r0
	adds	r4, #6
	adds	r0, r4, #0
	bl	sub_081a8264
	adds	r7, #4
	adds	r4, r0, #0
	adds	r0, r7, #0
	str	r4, [sp, #0]
	bl	sub_081a8264
	subs	r5, #6
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_081a8264
	ldr	r2, [pc, #480]
	adds	r5, r0, #0
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	ldr	r2, [pc, #464]
	lsls	r3, r7, #1
	ldrh	r3, [r2, r3]
	ldr	r4, [sp, #0]
	mov	r2, r8
	strh	r3, [r2, #2]
	ldr	r2, [pc, #460]
	lsls	r3, r4, #1
	ldrh	r3, [r2, r3]
	movs	r2, #6
	strh	r3, [r1, #4]
	ldr	r1, [sp, #44]
	movs	r3, #1
	add	fp, r3
	add	r8, r2
	cmp	fp, r1
	bcc.n	.L_081a7c02
	b.n	.L_081a813e
	ldr	r3, [sp, #44]
	movs	r2, #0
	mov	fp, r2
	cmp	fp, r3
	bcc.n	.L_081a7c84
	b.n	.L_081a813e
.L_081a7c84:
	ldr	r1, [pc, #424]
	mov	sl, r1
.L_081a7c88:
	mov	r2, r9
	ldrh	r6, [r2, #0]
	movs	r1, #31
	adds	r4, r6, #0
	movs	r3, #2
	lsrs	r7, r6, #5
	lsrs	r5, r6, #10
	ands	r4, r1
	add	r9, r3
	ands	r7, r1
	ands	r5, r1
	cmp	r4, #9
	bgt.n	.L_081a7ca4
	movs	r4, #10
.L_081a7ca4:
	cmp	r7, #15
	bgt.n	.L_081a7caa
	movs	r7, #16
.L_081a7caa:
	cmp	r5, #15
	bgt.n	.L_081a7cb0
	movs	r5, #16
.L_081a7cb0:
	cmp	r4, #28
	ble.n	.L_081a7cb6
	movs	r4, #28
.L_081a7cb6:
	cmp	r7, #24
	ble.n	.L_081a7cbc
	movs	r7, #24
.L_081a7cbc:
	cmp	r5, #26
	ble.n	.L_081a7cc2
	movs	r5, #26
.L_081a7cc2:
	adds	r0, r4, #0
	bl	sub_081a8264
	adds	r7, #2
	adds	r4, r0, #0
	adds	r0, r7, #0
	str	r4, [sp, #0]
	bl	sub_081a8264
	adds	r5, #2
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_081a8264
	adds	r5, r0, #0
	mov	r2, sl
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	mov	r1, sl
	lsls	r3, r7, #1
	ldrh	r3, [r1, r3]
	movs	r2, #2
	add	r8, r2
	mov	r2, r8
	ldr	r4, [sp, #0]
	strh	r3, [r2, #0]
	movs	r3, #2
	add	r8, r3
	lsls	r3, r4, #1
	ldrh	r3, [r1, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	ldr	r1, [sp, #44]
	movs	r3, #1
	movs	r2, #2
	add	fp, r3
	add	r8, r2
	cmp	fp, r1
	bcc.n	.L_081a7c88
	b.n	.L_081a813e
	ldr	r3, [sp, #44]
	movs	r2, #0
	mov	fp, r2
	cmp	fp, r3
	bcc.n	.L_081a7d22
	b.n	.L_081a813e
.L_081a7d22:
	ldr	r1, [pc, #264]
	mov	sl, r1
.L_081a7d26:
	mov	r2, r9
	ldrh	r6, [r2, #0]
	movs	r1, #31
	adds	r4, r6, #0
	lsrs	r7, r6, #5
	ands	r4, r1
	ands	r7, r1
	lsrs	r5, r6, #10
	adds	r0, r4, r7
	ands	r5, r1
	movs	r3, #2
	movs	r1, #3
	adds	r0, r0, r5
	str	r4, [sp, #0]
	add	r9, r3
	bl	sub_08002054
	bl	sub_081a8264
	ldr	r4, [sp, #0]
	asrs	r3, r4, #1
	adds	r4, r3, r0
	asrs	r3, r7, #1
	adds	r7, r3, r0
	asrs	r3, r5, #1
	adds	r5, r3, r0
	adds	r0, r4, #0
	bl	sub_081a8264
	adds	r4, r0, #0
	adds	r0, r7, #0
	str	r4, [sp, #0]
	bl	sub_081a8264
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_081a8264
	adds	r5, r0, #0
	mov	r2, sl
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	lsls	r3, r7, #1
	ldrh	r3, [r2, r3]
	ldr	r4, [sp, #0]
	mov	r2, r8
	strh	r3, [r2, #2]
	mov	r1, sl
	lsls	r3, r4, #1
	ldrh	r3, [r1, r3]
	movs	r1, #1
	strh	r3, [r2, #4]
	ldr	r2, [sp, #44]
	movs	r3, #6
	add	fp, r1
	add	r8, r3
	cmp	fp, r2
	bcc.n	.L_081a7d26
	b.n	.L_081a813e
	ldr	r1, [sp, #44]
	movs	r3, #0
	mov	fp, r3
	cmp	fp, r1
	bcc.n	.L_081a7dac
	b.n	.L_081a813e
.L_081a7dac:
	movs	r2, #31
	mov	sl, r2
.L_081a7db0:
	mov	r3, r9
	ldrh	r6, [r3, #0]
	mov	r2, sl
	lsrs	r7, r6, #5
	lsrs	r5, r6, #10
	ands	r7, r2
	ands	r5, r2
	adds	r4, r6, #0
	ands	r4, r2
	asrs	r3, r7, #3
	asrs	r2, r5, #3
	adds	r3, r3, r2
	adds	r4, r4, r3
	movs	r1, #2
	adds	r0, r4, #0
	add	r9, r1
	bl	sub_081a8264
	movs	r1, #3
	adds	r4, r0, #0
	adds	r0, r7, #0
	str	r4, [sp, #0]
	bl	sub_08002054
	movs	r1, #3
	subs	r7, r7, r0
	adds	r0, r5, #0
	bl	sub_08002054
	ldr	r1, [pc, #68]
	subs	r5, r5, r0
	lsls	r3, r5, #1
	ldrh	r3, [r1, r3]
	mov	r2, r8
	strh	r3, [r2, #0]
	lsls	r3, r7, #1
	ldrh	r3, [r1, r3]
	ldr	r4, [sp, #0]
	mov	r1, r8
	strh	r3, [r1, #2]
	ldr	r2, [pc, #36]
	lsls	r3, r4, #1
	ldrh	r3, [r2, r3]
	mov	r2, r8
	strh	r3, [r2, #4]
	ldr	r2, [sp, #44]
	movs	r1, #1
	movs	r3, #6
	add	fp, r1
	add	r8, r3
	cmp	fp, r2
	bcc.n	.L_081a7db0
	b.n	.L_081a813e
	movs	r0, r0
	.4byte 0xfffeffff
	.4byte 0x081a7aec
	.4byte 0x03000528
	.4byte 0x081a87ba
	.4byte 0x081a87fa
	.2byte 0x877a
	.2byte 0x081a
	ldr	r1, [sp, #44]
	movs	r3, #0
	mov	fp, r3
	cmp	fp, r1
	bcc.n	.L_081a7e40
	b.n	.L_081a813e
.L_081a7e40:
	movs	r2, #31
	mov	sl, r2
.L_081a7e44:
	mov	r3, r9
	ldrh	r6, [r3, #0]
	mov	r2, sl
	adds	r4, r6, #0
	ands	r4, r2
	lsrs	r7, r6, #5
	ands	r7, r2
	lsrs	r3, r4, #1
	movs	r1, #2
	subs	r4, r4, r3
	adds	r0, r7, #0
	add	r9, r1
	lsrs	r5, r6, #10
	movs	r1, #3
	ands	r5, r2
	str	r4, [sp, #0]
	bl	sub_08002054
	ldr	r4, [sp, #0]
	subs	r7, r7, r0
	adds	r4, #6
	adds	r0, r4, #0
	bl	sub_081a8264
	adds	r7, #4
	adds	r4, r0, #0
	adds	r0, r7, #0
	str	r4, [sp, #0]
	bl	sub_081a8264
	subs	r5, #6
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_081a8264
	ldr	r2, [pc, #48]
	adds	r5, r0, #0
	lsls	r3, r5, #1
	ldrh	r3, [r2, r3]
	mov	r1, r8
	strh	r3, [r1, #0]
	ldr	r2, [pc, #40]
	lsls	r3, r7, #1
	ldrh	r3, [r2, r3]
	ldr	r4, [sp, #0]
	mov	r2, r8
	strh	r3, [r2, #2]
	ldr	r2, [pc, #32]
	lsls	r3, r4, #1
	ldrh	r3, [r2, r3]
	movs	r2, #6
	strh	r3, [r1, #4]
	ldr	r1, [sp, #44]
	movs	r3, #1
	add	fp, r3
	add	r8, r2
	cmp	fp, r1
	bcc.n	.L_081a7e44
	b.n	.L_081a813e
	movs	r0, r0
	.4byte 0x081a87fa
	.4byte 0x081a87ba
	.2byte 0x877a
	.2byte 0x081a
.L_081a7ec8:
	ldr	r3, [sp, #44]
	movs	r2, #0
	mov	fp, r2
	cmp	fp, r3
	bcc.n	.L_081a7ed4
	b.n	.L_081a813e
.L_081a7ed4:
	ldr	r4, [pc, #8]
	ldr	r0, [pc, #12]
	ldr	r2, [pc, #12]
	mov	r1, r8
	b.n	.L_081a7eec
	movs	r0, r0
	.4byte 0x00007c00
	.4byte 0x000003e0
	.2byte 0x001f
	.2byte 0x0000
.L_081a7eec:
	mov	r3, r9
	ldrh	r6, [r3, #0]
	movs	r3, #2
	add	r9, r3
	adds	r3, r6, #0
	ands	r3, r4
	strh	r3, [r1, #0]
	adds	r3, r6, #0
	ands	r3, r0
	lsls	r3, r3, #5
	ands	r6, r2
	strh	r3, [r1, #2]
	lsls	r3, r6, #10
	strh	r3, [r1, #4]
	movs	r3, #1
	add	fp, r3
	ldr	r3, [sp, #44]
	adds	r1, #6
	cmp	fp, r3
	bcc.n	.L_081a7eec
	b.n	.L_081a813e
.L_081a7f16:
	movs	r3, #128
	lsls	r3, r3, #14
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_081a7fbc
	movs	r3, #31
	adds	r1, r0, #0
	ands	r1, r3
	str	r0, [sp, #40]
	lsrs	r2, r0, #5
	lsrs	r0, r0, #10
	str	r1, [sp, #40]
	adds	r1, r0, #0
	ands	r2, r3
	ands	r1, r3
	ldr	r3, [sp, #44]
	str	r2, [sp, #36]
	movs	r2, #0
	mov	fp, r2
	str	r0, [sp, #32]
	str	r1, [sp, #32]
	cmp	fp, r3
	bcc.n	.L_081a7f46
	b.n	.L_081a813e
.L_081a7f46:
	mov	sl, r8
.L_081a7f48:
	mov	r1, r9
	ldrh	r6, [r1, #0]
	movs	r3, #248
	lsls	r0, r6, #11
	lsls	r3, r3, #8
	movs	r2, #2
	ands	r0, r3
	movs	r3, #248
	lsls	r3, r3, #9
	add	r9, r2
	lsls	r2, r6, #7
	ands	r2, r3
	movs	r3, #248
	lsls	r3, r3, #7
	ands	r3, r6
	adds	r0, r0, r2
	adds	r0, r0, r3
	movs	r1, #96
	ldr	r3, [pc, #328]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9a0a
	adds	r6, r0, #0
	adds	r1, r2, #0
	muls	r1, r6
	ldr	r3, [sp, #36]
	mov	r8, r1
	ldr	r1, [sp, #32]
	mov	r0, r8
	adds	r7, r3, #0
	muls	r7, r6
	adds	r5, r1, #0
	muls	r5, r6
	bl	sub_081a8278
	mov	r8, r0
	adds	r0, r7, #0
	bl	sub_081a8278
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_081a8278
	mov	r2, sl
	mov	r3, sl
	mov	r1, r8
	adds	r5, r0, #0
	strh	r5, [r2, #0]
	strh	r7, [r3, #2]
	strh	r1, [r2, #4]
	ldr	r2, [sp, #44]
	movs	r1, #1
	movs	r3, #6
	add	fp, r1
	add	sl, r3
	cmp	fp, r2
	bcc.n	.L_081a7f48
	b.n	.L_081a813e
.L_081a7fbc:
	movs	r3, #128
	lsls	r3, r3, #15
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_081a7fc8
	b.n	.L_081a80c4
.L_081a7fc8:
	movs	r3, #31
	str	r0, [sp, #28]
	adds	r1, r0, #0
	lsrs	r2, r0, #5
	lsrs	r0, r0, #10
	ands	r1, r3
	mov	sl, r0
	str	r1, [sp, #28]
	mov	r1, sl
	ands	r2, r3
	ands	r1, r3
	ldr	r3, [sp, #44]
	str	r2, [sp, #24]
	movs	r2, #0
	mov	fp, r2
	mov	sl, r1
	cmp	fp, r3
	bcc.n	.L_081a7fee
	b.n	.L_081a813e
.L_081a7fee:
	ldr	r2, [sp, #24]
	ldr	r1, [sp, #28]
	ldr	r3, [sp, #28]
	adds	r1, r1, r2
	str	r1, [sp, #20]
	lsls	r1, r2, #16
	mov	r2, sl
	lsls	r3, r3, #16
	lsls	r2, r2, #16
	str	r3, [sp, #16]
	str	r1, [sp, #12]
	str	r2, [sp, #8]
	mov	r7, r8
.L_081a8008:
	mov	r3, r9
	ldrh	r6, [r3, #0]
	movs	r2, #31
	movs	r1, #2
	adds	r4, r6, #0
	lsrs	r0, r6, #5
	ands	r4, r2
	ands	r0, r2
	lsrs	r3, r6, #10
	add	r9, r1
	ldr	r1, [sp, #20]
	adds	r0, r4, r0
	ands	r3, r2
	adds	r0, r0, r3
	add	r1, sl
	ldr	r3, [pc, #144]
	lsls	r0, r0, #4
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9b07
	adds	r6, r0, #0
	adds	r0, r3, #0
	muls	r0, r6
	ldr	r2, [sp, #16]
	lsrs	r0, r0, #4
	asrs	r1, r2, #4
	ldr	r3, [pc, #124]
	lsls	r0, r0, #16
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9906
	mov	r8, r0
	adds	r0, r1, #0
	muls	r0, r6
	ldr	r2, [sp, #12]
	lsrs	r0, r0, #4
	asrs	r1, r2, #4
	ldr	r3, [pc, #104]
	lsls	r0, r0, #16
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c05
	mov	r0, sl
	muls	r0, r6
	ldr	r2, [sp, #8]
	lsrs	r0, r0, #4
	asrs	r1, r2, #4
	ldr	r3, [pc, #84]
	lsls	r0, r0, #16
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4641
	lsrs	r1, r1, #16
	mov	r8, r1
	adds	r6, r0, #0
	mov	r0, r8
	bl	sub_081a8264
	lsrs	r5, r5, #16
	mov	r8, r0
	adds	r0, r5, #0
	bl	sub_081a8264
	lsrs	r6, r6, #16
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_081a8264
	ldr	r2, [pc, #44]
	lsls	r0, r0, #1
	ldrh	r3, [r2, r0]
	lsls	r5, r5, #1
	strh	r3, [r7, #0]
	movs	r1, #1
	ldrh	r3, [r2, r5]
	add	fp, r1
	strh	r3, [r7, #2]
	mov	r3, r8
	lsls	r3, r3, #1
	mov	r8, r3
	ldrh	r3, [r2, r3]
	strh	r3, [r7, #4]
	ldr	r2, [sp, #44]
	adds	r7, #6
	cmp	fp, r2
	bcc.n	.L_081a8008
	b.n	.L_081a813e
	movs	r0, r0
	.4byte 0x03000528
	.4byte 0x0300021c
	.2byte 0x877a
	.2byte 0x081a
.L_081a80c4:
	movs	r3, #128
	lsls	r3, r3, #16
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_081a811a
	ldr	r1, [sp, #44]
	movs	r3, #0
	mov	fp, r3
	cmp	fp, r1
	bcs.n	.L_081a813e
	ldr	r4, [pc, #8]
	ldr	r0, [pc, #12]
	ldr	r2, [pc, #12]
	mov	r1, r8
	b.n	.L_081a80f0
	movs	r0, r0
	.4byte 0x00007c00
	.4byte 0x000003e0
	.2byte 0x001f
	.2byte 0x0000
.L_081a80f0:
	mov	r3, r9
	ldrh	r6, [r3, #0]
	movs	r3, #2
	add	r9, r3
	adds	r3, r6, #0
	ands	r3, r4
	strh	r3, [r1, #0]
	adds	r3, r6, #0
	ands	r3, r0
	lsls	r3, r3, #5
	ands	r6, r2
	strh	r3, [r1, #2]
	lsls	r3, r6, #10
	strh	r3, [r1, #4]
	movs	r3, #1
	add	fp, r3
	ldr	r3, [sp, #44]
	adds	r1, #6
	cmp	fp, r3
	bcc.n	.L_081a80f0
	b.n	.L_081a813e
.L_081a811a:
	cmp	r2, #2
	bne.n	.L_081a8124
	movs	r1, #192
	lsls	r1, r1, #3
	adds	r0, r0, r1
.L_081a8124:
	ldr	r3, [sp, #44]
	movs	r4, #132
	lsls	r2, r3, #1
	adds	r2, r2, r3
	movs	r3, #128
	lsls	r4, r4, #24
	lsrs	r2, r2, #1
	lsls	r3, r3, #19
	adds	r3, #212
.L_081a8136:
	mov	r1, r8
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_081a813e:
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r1, #192
	lsls	r1, r1, #6
	adds	r1, #4
	movs	r0, #128
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #0
	adds	r4, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r1, r4, #0
	ldr	r2, [pc, #68]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #132
	movs	r0, #160
	lsls	r2, r2, #24
	lsls	r0, r0, #19
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r1, r4, r2
	movs	r2, #132
	lsls	r2, r2, #24
	ldr	r0, [pc, #44]
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #128
	lsls	r3, r3, #5
	movs	r0, #128
	adds	r2, r4, r3
	adds	r1, r4, #0
	movs	r3, #0
	lsls	r0, r0, #9
	bl	.L_081a7a28
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #16]
	bl	sub_080145a8
	add	sp, #4
	pop	{pc}
	movs	r0, r0
	.4byte 0x85000c01
	.4byte 0x05000200
	.4byte 0x081a78c1

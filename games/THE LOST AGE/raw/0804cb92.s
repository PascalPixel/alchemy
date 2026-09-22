.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_0801314c, 0x0801314c
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_0801401c, 0x0801401c
	.set sub_08014128, 0x08014128
	.set sub_08014274, 0x08014274
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014d00, 0x08014d00
	.set sub_08014d78, 0x08014d78
	.set sub_0801591c, 0x0801591c
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_080393fc, 0x080393fc
	.set sub_080396bc, 0x080396bc
	.set sub_08042010, 0x08042010
	.set sub_0804bbd0, 0x0804bbd0
	.set sub_0804bdc2, 0x0804bdc2
	.set sub_0804bfac, 0x0804bfac
	.set sub_081180d0, 0x081180d0
	.set sub_081c0010, 0x081c0010
	.global Overlay_0804cb92
Overlay_0804cb92:
	movs	r0, #110
	bl	sub_081c0010
	add	r4, sp, #64
	ldrh	r4, [r4, #0]
	ldr	r0, [sp, #76]
	strh	r4, [r0, #0]
	ldr	r5, [sp, #72]
	adds	r5, #64
	ldrh	r6, [r5, #0]
	cmp	r6, #0
	beq.n	.L_0804cbb6
	bl	sub_08014878
	ldrh	r3, [r5, #0]
	muls	r3, r0
	lsrs	r3, r3, #20
	adds	r6, r6, r3
.L_0804cbb6:
	ldr	r0, [sp, #76]
	strh	r6, [r0, #4]
	ldr	r1, [sp, #44]
	cmp	r1, #0
	beq.n	.L_0804cbde
	ldr	r2, [sp, #28]
	ldr	r4, [sp, #84]
	adds	r3, r2, r4
	subs	r2, r3, #2
	ldrh	r1, [r3, #0]
	ldrh	r3, [r2, #0]
	cmp	r1, r3
	bne.n	.L_0804cbde
	lsls	r2, r6, #16
	asrs	r3, r2, #16
	ldr	r0, [sp, #76]
	lsrs	r2, r2, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	strh	r3, [r0, #4]
.L_0804cbde:
	ldr	r2, [sp, #76]
	movs	r1, #4
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	bge.n	.L_0804cbf0
	movs	r3, #250
	lsls	r3, r3, #3
	adds	r4, r2, #0
	strh	r3, [r4, #4]
.L_0804cbf0:
	add	r0, sp, #60
	ldrh	r0, [r0, #0]
	ldr	r1, [sp, #76]
	strh	r0, [r1, #6]
	add	r1, sp, #56
	ldrh	r1, [r1, #0]
	ldr	r2, [sp, #76]
	strh	r1, [r2, #8]
	add	r2, sp, #68
	ldrh	r2, [r2, #0]
	ldr	r3, [sp, #76]
	strh	r2, [r3, #10]
	ldr	r3, [sp, #44]
	ldr	r2, [sp, #80]
	adds	r3, #1
	lsls	r4, r3, #2
	lsls	r0, r3, #4
	lsls	r1, r3, #1
	str	r3, [sp, #44]
	str	r4, [sp, #24]
	str	r0, [sp, #20]
	str	r1, [sp, #28]
	cmp	r3, r2
	bge.n	.L_0804cc24
	bl	sub_0804bfac
.L_0804cc24:
	ldr	r3, [sp, #44]
	ldr	r4, [sp, #80]
	cmp	r3, r4
	bge.n	.L_0804cc30
	bl	sub_0804bdc2
.L_0804cc30:
	ldr	r1, [sp, #36]
	ldr	r0, [r1, #0]
	ldr	r3, [r0, #80]
	cmp	r3, #0
	beq.n	.L_0804cc4c
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #8]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #10]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #12]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #14]
.L_0804cc4c:
	ldr	r0, [r0, #68]
	cmp	r0, #0
	beq.n	.L_0804cc58
	movs	r1, #1
	bl	sub_0803939c
.L_0804cc58:
	ldr	r0, [sp, #48]
	bl	sub_08014274
	ldr	r0, [sp, #52]
	bl	sub_08014274
	ldr	r0, [pc, #28]
	bl	sub_08014644
	ldr	r3, [sp, #36]
	ldr	r2, [r3, #0]
	b.n	.L_0804cc88
	.4byte 0x00000045
	.4byte 0x00000044
	.4byte 0x00000056
	.4byte 0x00000053
	.4byte 0x02003a74
	.2byte 0xb6a1
	.2byte 0x0804
.L_0804cc88:
	ldr	r3, [r2, #80]
	cmp	r3, #0
	beq.n	.L_0804cd78
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #36]
	ldr	r3, [r2, #68]
	movs	r6, #0
	cmp	r3, #0
	bne.n	.L_0804ccca
	adds	r7, r5, #0
	adds	r7, #82
	ldrb	r3, [r7, #0]
	cmp	r3, #0
	bne.n	.L_0804ccce
	movs	r3, #42
	str	r3, [sp, #0]
	movs	r1, #16
	movs	r2, #30
	movs	r3, #4
	movs	r0, #0
	bl	sub_08039260
	ldr	r4, [sp, #36]
	ldr	r3, [r4, #0]
	str	r0, [r3, #68]
	bl	sub_080396bc
	add	r0, sp, #108
	mov	r9, r0
	bl	sub_0804bbd0
	b.n	.L_0804ccd6
.L_0804ccca:
	adds	r7, r5, #0
	adds	r7, #82
.L_0804ccce:
	ldr	r1, [sp, #36]
	movs	r3, #0
	ldr	r2, [r1, #0]
	str	r3, [r2, #68]
.L_0804ccd6:
	adds	r5, #80
	ldrb	r2, [r5, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r3, [pc, #188]
	lsls	r2, r2, #3
	adds	r1, r2, r3
	ldrb	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_0804ccf6
	movs	r2, #1
	negs	r2, r2
	str	r2, [sp, #80]
	b.n	.L_0804cd68
.L_0804ccf6:
	ldr	r3, [pc, #172]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L_0804cd10
	adds	r6, #1
	cmp	r6, #24
	ble.n	.L_0804cd46
	movs	r3, #1
	negs	r3, r3
	str	r3, [sp, #80]
	b.n	.L_0804cd68
.L_0804cd10:
	ldrh	r2, [r1, #8]
	movs	r6, #0
	adds	r3, r2, #0
	cmp	r3, #69
	bne.n	.L_0804cd2c
	ldrh	r3, [r1, #10]
	cmp	r3, #68
	bne.n	.L_0804cd2c
	ldrh	r3, [r1, #12]
	cmp	r3, #86
	bne.n	.L_0804cd2c
	ldrh	r3, [r1, #14]
	cmp	r3, #83
	beq.n	.L_0804cd68
.L_0804cd2c:
	adds	r3, r2, #0
	cmp	r3, #86
	bne.n	.L_0804cd44
	ldrh	r3, [r1, #10]
	cmp	r3, #83
	bne.n	.L_0804cd44
	ldrh	r3, [r1, #12]
	cmp	r3, #83
	bne.n	.L_0804cd44
	ldrh	r3, [r1, #14]
	cmp	r3, #84
	beq.n	.L_0804cd46
.L_0804cd44:
	movs	r6, #1
.L_0804cd46:
	movs	r0, #1
	bl	sub_08013560
	ldrb	r2, [r5, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r3, [pc, #72]
	lsls	r2, r2, #3
	adds	r1, r2, r3
	ldrb	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_0804ccf6
	movs	r4, #1
	negs	r4, r4
	str	r4, [sp, #80]
.L_0804cd68:
	ldr	r0, [sp, #36]
	ldr	r3, [r0, #0]
	ldr	r0, [r3, #68]
	cmp	r0, #0
	beq.n	.L_0804cd78
	movs	r1, #1
	bl	sub_0803939c
.L_0804cd78:
	movs	r0, #0
	bl	sub_081180d0
	movs	r0, #228
	bl	sub_0801314c
	ldr	r1, [sp, #80]
	cmp	r1, #0
	blt.n	.L_0804cd90
	ldr	r3, [sp, #92]
	adds	r1, r1, r3
	str	r1, [sp, #80]
.L_0804cd90:
	ldr	r0, [sp, #80]
	add	sp, #108
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02003874
	.2byte 0x124c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #192
	lsls	r0, r0, #18
	adds	r3, r0, #0
	adds	r3, #232
	ldr	r3, [r3, #0]
	ldr	r0, [r0, #124]
	mov	r8, r3
	ldr	r3, [pc, #744]
	ldr	r1, [pc, #748]
	ldr	r3, [r3, #0]
	movs	r2, #31
	lsls	r3, r3, #1
	ands	r3, r2
	lsls	r3, r3, #1
	ldrh	r7, [r1, r3]
	mov	r9, r0
	ldr	r0, [pc, #736]
	sub	sp, #12
	adds	r3, r7, r0
	mov	r6, r8
	cmp	r3, #0
	bge.n	.L_0804cde6
	adds	r3, r7, #0
	subs	r3, #253
.L_0804cde6:
	movs	r1, #152
	asrs	r3, r3, #2
	lsls	r1, r1, #1
	ldr	r4, [pc, #716]
	adds	r7, r3, r1
	ldr	r3, [sp, #4]
	lsls	r1, r7, #16
	movs	r2, #255
	lsrs	r1, r1, #16
	ands	r3, r4
	lsls	r2, r2, #8
	adds	r2, #255
	orrs	r3, r1
	ands	r3, r2
	lsls	r1, r1, #16
	orrs	r3, r1
	str	r3, [sp, #4]
	add	r0, sp, #4
	ldr	r3, [r0, #4]
	movs	r5, #0
	ands	r3, r4
	str	r3, [r0, #4]
	bl	sub_0801401c
	movs	r2, #142
	add	r2, r8
	mov	fp, r0
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	mov	sl, r2
	cmp	r5, r3
	bcs.n	.L_0804cee2
	ldr	r4, [pc, #664]
.L_0804ce28:
	movs	r1, #12
	ldrsh	r2, [r6, r1]
	cmp	r2, #0
	beq.n	.L_0804ced4
	mov	r3, r8
	adds	r3, #140
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r5, r3
	bne.n	.L_0804cea8
	lsls	r3, r7, #3
	subs	r3, r3, r7
	cmp	r3, #0
	bge.n	.L_0804ce4c
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r3, r3, r1
.L_0804ce4c:
	asrs	r3, r3, #9
	adds	r3, r2, r3
	movs	r2, #14
	ldrsh	r1, [r6, r2]
	subs	r3, #20
	mov	ip, r3
	cmp	r1, #0
	beq.n	.L_0804ce70
	lsls	r3, r7, #1
	adds	r3, r3, r7
	cmp	r3, #0
	bge.n	.L_0804ce66
	adds	r3, #255
.L_0804ce66:
	asrs	r3, r3, #8
	adds	r3, r1, r3
	adds	r1, r3, #0
	subs	r1, #20
	b.n	.L_0804ce84
.L_0804ce70:
	lsls	r3, r7, #4
	subs	r3, r3, r7
	cmp	r3, #0
	bge.n	.L_0804ce7a
	adds	r3, #255
.L_0804ce7a:
	asrs	r3, r3, #8
	adds	r1, r3, #0
	subs	r1, #30
	movs	r3, #255
	ands	r1, r3
.L_0804ce84:
	adds	r0, r6, #0
	movs	r3, #0
	mov	r2, fp
	stmia	r0!, {r3}
	lsls	r3, r2, #25
	orrs	r3, r1
	mov	r1, ip
	lsls	r2, r1, #16
	orrs	r3, r2
	ldr	r2, [pc, #556]
	movs	r1, #246
	orrs	r3, r2
	stmia	r0!, {r3}
	ldrh	r3, [r6, #18]
	lsls	r3, r3, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #2]
	b.n	.L_0804cec6
.L_0804cea8:
	movs	r3, #14
	ldrsh	r1, [r6, r3]
	adds	r0, r6, #0
	movs	r3, #0
	stmia	r0!, {r3}
	lsls	r3, r2, #16
	orrs	r1, r3
	ldr	r3, [pc, #528]
	orrs	r1, r3
	ldrh	r3, [r6, #18]
	stmia	r0!, {r1}
	lsls	r3, r3, #2
	adds	r3, r3, r4
	ldrh	r3, [r3, #2]
	movs	r1, #245
.L_0804cec6:
	lsrs	r3, r3, #5
	str	r3, [r0, #0]
	adds	r0, r6, #0
	str	r4, [sp, #0]
	bl	sub_08014128
	ldr	r4, [sp, #0]
.L_0804ced4:
	mov	r1, sl
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	adds	r5, #1
	adds	r6, #20
	cmp	r5, r3
	bcc.n	.L_0804ce28
.L_0804cee2:
	mov	r2, r9
	cmp	r2, #0
	bne.n	.L_0804ceea
	b.n	.L_0804d0a2
.L_0804ceea:
	movs	r3, #136
	lsls	r3, r3, #3
	adds	r3, #255
	add	r3, r9
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_0804cefe
	b.n	.L_0804d0a2
.L_0804cefe:
	mov	r1, sl
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r3, #0
	bne.n	.L_0804cf0a
	b.n	.L_0804d0a2
.L_0804cf0a:
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #57
	add	r3, r9
	ldrb	r2, [r3, #0]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #5
	adds	r3, r3, r2
	lsls	r3, r3, #2
	add	r3, r9
	mov	ip, r3
	mov	r3, r8
	adds	r3, #148
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_0804cfea
	movs	r3, #140
	add	r3, r8
	mov	sl, r3
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	lsls	r0, r7, #1
	lsls	r2, r3, #2
	adds	r2, r2, r3
	lsls	r2, r2, #2
	adds	r1, r2, #0
	adds	r1, #12
	mov	r2, r8
	ldrsh	r4, [r2, r1]
	mov	lr, r0
	adds	r3, r0, r7
	ldr	r0, [pc, #380]
	lsls	r3, r3, #2
	adds	r2, r3, r0
	cmp	r2, #0
	bge.n	.L_0804cf5a
	adds	r0, #255
	adds	r2, r3, r0
.L_0804cf5a:
	asrs	r2, r2, #8
	subs	r3, r4, r2
	lsls	r3, r3, #8
	adds	r2, r4, r2
	adds	r3, r3, r2
	mov	r2, r8
	adds	r4, r3, #0
	adds	r3, r2, r1
	movs	r0, #2
	ldrsh	r3, [r3, r0]
	ldr	r1, [pc, #352]
	lsls	r2, r7, #5
	adds	r0, r3, #0
	adds	r3, r2, r1
	adds	r4, #23
	adds	r0, #24
	cmp	r3, #0
	bge.n	.L_0804cf82
	ldr	r1, [pc, #340]
	adds	r3, r2, r1
.L_0804cf82:
	asrs	r3, r3, #9
	adds	r3, r0, r3
	mov	r1, ip
	adds	r0, r3, #1
	movs	r5, #24
	adds	r1, #102
	cmp	r5, r0
	bcs.n	.L_0804cfa6
	movs	r6, #255
.L_0804cf94:
	ldrh	r2, [r1, #0]
	adds	r3, r6, #0
	ands	r3, r2
	orrs	r3, r4
	adds	r5, #1
	strh	r3, [r1, #0]
	adds	r1, #4
	cmp	r5, r0
	bcc.n	.L_0804cf94
.L_0804cfa6:
	mov	r3, r8
	mov	r1, sl
	movs	r2, #12
	ldrsh	r4, [r3, r2]
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r3, #0
	bne.n	.L_0804cfcc
	mov	r2, lr
	ldr	r0, [pc, #272]
	adds	r3, r2, r7
	lsls	r1, r3, #2
	adds	r3, r1, r0
	cmp	r3, #0
	bge.n	.L_0804cfc8
	ldr	r2, [pc, #272]
	adds	r3, r1, r2
.L_0804cfc8:
	asrs	r3, r3, #8
	subs	r4, r4, r3
.L_0804cfcc:
	mov	r1, ip
	lsls	r4, r4, #8
	adds	r1, #6
	movs	r5, #0
	movs	r0, #255
.L_0804cfd6:
	ldrh	r2, [r1, #0]
	adds	r3, r0, #0
	ands	r3, r2
	orrs	r3, r4
	adds	r5, #1
	strh	r3, [r1, #0]
	adds	r1, #4
	cmp	r5, #23
	bls.n	.L_0804cfd6
	b.n	.L_0804d0a2
.L_0804cfea:
	movs	r3, #140
	add	r3, r8
	mov	lr, r3
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	lsls	r6, r7, #1
	lsls	r2, r3, #2
	adds	r2, r2, r3
	lsls	r2, r2, #2
	adds	r1, r2, #0
	adds	r1, #12
	mov	r2, r8
	ldr	r0, [pc, #200]
	ldrsh	r4, [r2, r1]
	adds	r3, r6, r7
	lsls	r3, r3, #2
	adds	r2, r3, r0
	cmp	r2, #0
	bge.n	.L_0804d014
	adds	r0, #255
	adds	r2, r3, r0
.L_0804d014:
	asrs	r2, r2, #8
	subs	r3, r4, r2
	lsls	r3, r3, #8
	adds	r2, r4, r2
	adds	r3, r3, r2
	mov	r2, r8
	adds	r4, r3, #0
	adds	r3, r2, r1
	movs	r1, #2
	ldrsh	r0, [r3, r1]
	ldr	r1, [pc, #164]
	lsls	r2, r7, #5
	adds	r3, r2, r1
	adds	r4, #23
	cmp	r3, #0
	bge.n	.L_0804d038
	ldr	r1, [pc, #156]
	adds	r3, r2, r1
.L_0804d038:
	asrs	r3, r3, #9
	subs	r3, r0, r3
	subs	r0, r3, #1
	lsls	r3, r0, #2
	add	r3, ip
	adds	r5, r0, #0
	adds	r1, r3, #6
	cmp	r5, #135
	bhi.n	.L_0804d05e
	movs	r0, #255
.L_0804d04c:
	ldrh	r2, [r1, #0]
	adds	r3, r0, #0
	ands	r3, r2
	orrs	r3, r4
	adds	r5, #1
	strh	r3, [r1, #0]
	adds	r1, #4
	cmp	r5, #135
	bls.n	.L_0804d04c
.L_0804d05e:
	mov	r3, r8
	mov	r1, lr
	movs	r2, #12
	ldrsh	r4, [r3, r2]
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r3, #0
	bne.n	.L_0804d082
	ldr	r2, [pc, #92]
	adds	r3, r6, r7
	lsls	r1, r3, #2
	adds	r3, r1, r2
	cmp	r3, #0
	bge.n	.L_0804d07e
	ldr	r0, [pc, #92]
	adds	r3, r1, r0
.L_0804d07e:
	asrs	r3, r3, #8
	subs	r4, r4, r3
.L_0804d082:
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r1, #38
	lsls	r4, r4, #8
	add	r1, ip
	movs	r5, #136
	movs	r0, #255
.L_0804d090:
	ldrh	r2, [r1, #0]
	adds	r3, r0, #0
	ands	r3, r2
	orrs	r3, r4
	adds	r5, #1
	strh	r3, [r1, #0]
	adds	r1, #4
	cmp	r5, #159
	bls.n	.L_0804d090
.L_0804d0a2:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0300122c
	.4byte 0x0805e9c4
	.4byte 0xffffff00
	.4byte 0xffff0000
	.4byte 0x020036e0
	.4byte 0x80002300
	.4byte 0x80002000
	.4byte 0xfffff4ff
	.4byte 0xffffe0ff
	.4byte 0xffffe2fe
	.2byte 0xf5fe
	.2byte 0xffff
	.2byte 0xb520
	movs	r1, #152
	movs	r0, #232
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #0
	adds	r5, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r2, #133
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, r5, #0
	adds	r2, #38
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #118
	ldr	r0, [pc, #8]
	bl	sub_080145a8
	adds	r0, r5, #0
	add	sp, #4
	pop	{r5, pc}
	.2byte 0xcda9
	.2byte 0x0804
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #232
	ldr	r5, [r3, #0]
	ldr	r0, [pc, #68]
	bl	sub_08014644
	ldr	r0, [r5, #120]
	cmp	r0, #0
	beq.n	.L_0804d134
	movs	r1, #2
	bl	sub_0803939c
.L_0804d134:
	adds	r2, r5, #0
	adds	r2, #142
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	movs	r6, #0
	cmp	r6, r3
	bge.n	.L_0804d158
	adds	r7, r2, #0
	adds	r5, #18
.L_0804d146:
	ldrh	r0, [r5, #0]
	bl	sub_08014274
	movs	r2, #0
	ldrsh	r3, [r7, r2]
	adds	r6, #1
	adds	r5, #20
	cmp	r6, r3
	blt.n	.L_0804d146
.L_0804d158:
	movs	r0, #232
	bl	sub_0801314c
	movs	r0, #1
	bl	sub_08013560
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xcda9
	.2byte 0x0804
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #232
	ldr	r3, [r3, #0]
	movs	r1, #140
	mov	r8, r3
	add	r1, r8
	mov	sl, r1
	mov	r2, sl
	movs	r3, #146
	strh	r0, [r2, #0]
	add	r3, r8
	mov	fp, r3
.L_0804d194:
	mov	r1, r8
	ldr	r0, [r1, #120]
	bl	sub_080393fc
	mov	r3, fp
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	cmp	r0, #0
	beq.n	.L_0804d1bc
	mov	r2, sl
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	adds	r0, r0, r3
	b.n	.L_0804d1cc
.L_0804d1b0:
	movs	r0, #113
	bl	sub_081c0010
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0804d278
.L_0804d1bc:
	mov	r0, sl
	movs	r2, #0
	ldrsh	r3, [r0, r2]
	mov	r1, r8
	adds	r3, #132
	ldrb	r2, [r1, r3]
	ldr	r3, [pc, #184]
	adds	r0, r2, r3
.L_0804d1cc:
	mov	r2, r8
	ldr	r1, [r2, #120]
	movs	r3, #0
	movs	r2, #0
	bl	sub_08042010
	movs	r3, #142
	ldr	r6, [pc, #172]
	add	r3, r8
	mov	r7, sl
	mov	r9, r3
.L_0804d1e2:
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [r6, #4]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_0804d26c
	ldr	r2, [r6, #4]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_0804d1b0
	ldr	r2, [r6, #4]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_0804d1b0
	ldr	r2, [r6, #12]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_0804d21a
	ldr	r5, [r6, #12]
	movs	r3, #64
	ands	r5, r3
	cmp	r5, #0
	beq.n	.L_0804d238
.L_0804d21a:
	movs	r0, #111
	bl	sub_081c0010
	ldrh	r3, [r7, #0]
	subs	r3, #1
	strh	r3, [r7, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	bge.n	.L_0804d194
	mov	r0, r9
	ldrh	r3, [r0, #0]
	mov	r1, sl
	subs	r3, #1
	strh	r3, [r1, #0]
	b.n	.L_0804d194
.L_0804d238:
	ldr	r2, [r6, #12]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_0804d24c
	ldr	r2, [r6, #12]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0804d1e2
.L_0804d24c:
	movs	r0, #111
	bl	sub_081c0010
	ldrh	r3, [r7, #0]
	mov	r1, r9
	adds	r3, #1
	strh	r3, [r7, #0]
	lsls	r3, r3, #16
	movs	r0, #0
	ldrsh	r2, [r1, r0]
	asrs	r3, r3, #16
	cmp	r3, r2
	blt.n	.L_0804d194
	mov	r2, sl
	strh	r5, [r2, #0]
	b.n	.L_0804d194
.L_0804d26c:
	movs	r0, #112
	bl	sub_081c0010
	mov	r1, sl
	movs	r3, #0
	ldrsh	r0, [r1, r3]
.L_0804d278:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0000003a
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #232
	ldr	r6, [r3, #0]
	movs	r2, #1
	adds	r5, r6, #0
	movs	r3, #12
	adds	r5, #140
	adds	r7, r1, #0
	mov	r9, r2
	mov	sl, r3
	strh	r0, [r5, #0]
	cmp	r7, r0
	bge.n	.L_0804d2ba
	subs	r2, #2
	mov	r9, r2
.L_0804d2ba:
	mov	r8, r0
	movs	r3, #146
	adds	r3, r3, r6
	mov	fp, r3
	b.n	.L_0804d2d6
.L_0804d2c4:
	ldrh	r3, [r5, #0]
	movs	r0, #111
	add	r3, r9
	strh	r3, [r5, #0]
	bl	sub_081c0010
	movs	r2, #0
	mov	sl, r2
	add	r8, r9
.L_0804d2d6:
	ldr	r0, [r6, #120]
	bl	sub_080393fc
	mov	r2, fp
	movs	r3, #0
	ldrsh	r0, [r2, r3]
	cmp	r0, #0
	beq.n	.L_0804d2ee
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	adds	r0, r0, r3
	b.n	.L_0804d2fa
.L_0804d2ee:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	adds	r3, #132
	ldrb	r2, [r6, r3]
	ldr	r3, [pc, #68]
	adds	r0, r2, r3
.L_0804d2fa:
	movs	r3, #0
	ldr	r1, [r6, #120]
	movs	r2, #0
	bl	sub_08042010
	movs	r3, #0
	ldrsh	r1, [r5, r3]
	ldr	r0, [pc, #52]
	subs	r3, r1, r7
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L_0804d314
	subs	r2, r7, r1
.L_0804d314:
	ldrb	r0, [r0, r2]
	add	r0, sl
	bl	sub_08013560
	cmp	r8, r7
	bne.n	.L_0804d2c4
	movs	r0, #48
	bl	sub_08013560
	movs	r0, #112
	bl	sub_081c0010
	adds	r0, r7, #0
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0000003a
	.2byte 0xf89f
	.2byte 0x0805
.L_0804d344:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	movs	r3, #128
	lsls	r3, r3, #3
	mov	r8, r3
	mov	sl, r0
	mov	r0, r8
	adds	r5, r1, #0
	bl	sub_08014d78
	adds	r6, r0, #0
	ldr	r0, [pc, #40]
	bl	sub_08013300
	lsls	r5, r5, #1
	ldrh	r3, [r5, r0]
	adds	r1, r6, #0
	adds	r0, r0, r3
	bl	sub_0801591c
	mov	r0, sl
	mov	r1, r8
	adds	r2, r6, #0
	bl	sub_080142d4
	adds	r0, r6, #0
	bl	sub_08013164
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.2byte 0x01d7
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #232
	ldr	r3, [r3, #0]
	mov	sl, r0
	mov	r8, r3
	mov	r2, r8
	adds	r2, #142
	movs	r1, #0
	ldrsh	r7, [r2, r1]
	ldrh	r3, [r2, #0]
	cmp	r7, #5
	bgt.n	.L_0804d3e0
	adds	r3, #1
	strh	r3, [r2, #0]
	bl	sub_080143ac
	mov	r1, sl
	adds	r6, r0, #0
	lsls	r5, r7, #2
	bl	.L_0804d344
	lsls	r3, r7, #1
	adds	r5, r5, r7
	adds	r3, r3, r7
	lsls	r5, r5, #2
	lsls	r3, r3, #3
	add	r5, r8
	adds	r3, #32
	strh	r3, [r5, #12]
	movs	r3, #136
	strh	r3, [r5, #14]
	adds	r3, r7, #0
	adds	r3, #132
	mov	r1, sl
	mov	r2, r8
	strh	r6, [r5, #18]
	strb	r1, [r2, r3]
.L_0804d3e0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #232
	ldr	r7, [r3, #0]
	movs	r3, #144
	adds	r3, r3, r7
	mov	r8, r3
	adds	r3, r7, #0
	mov	sl, r0
	adds	r1, #2
	mov	r5, r8
	adds	r3, #146
	strh	r1, [r5, #0]
	mov	r6, sl
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r6, [r3, #0]
	movs	r1, #142
	adds	r1, r1, r7
	movs	r2, #0
	ldrsh	r6, [r1, r2]
	mov	r9, r1
	mov	r1, r8
	movs	r3, #0
	ldrsh	r0, [r1, r3]
	movs	r1, #3
	lsls	r0, r0, #1
	sub	sp, #4
	bl	sub_08002054
	lsls	r5, r6, #1
	adds	r5, r5, r6
	adds	r5, r5, r0
	lsrs	r3, r5, #31
	adds	r5, r5, r3
	asrs	r5, r5, #1
	movs	r3, #15
	movs	r1, #0
	subs	r0, r3, r5
	cmp	r1, r6
	bge.n	.L_0804d462
	mov	r2, sl
	lsls	r4, r2, #3
	mov	ip, r9
	adds	r2, r7, #0
.L_0804d44c:
	lsls	r3, r0, #3
	strh	r3, [r2, #12]
	strh	r4, [r2, #14]
	mov	r6, ip
	movs	r5, #0
	ldrsh	r3, [r6, r5]
	adds	r1, #1
	adds	r0, #3
	adds	r2, #20
	cmp	r1, r3
	blt.n	.L_0804d44c
.L_0804d462:
	mov	r3, r8
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	movs	r3, #2
	str	r3, [sp, #0]
	mov	r1, sl
	movs	r3, #3
	bl	sub_08039260
	str	r0, [r7, #120]
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}

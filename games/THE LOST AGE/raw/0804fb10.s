.syntax unified
	.thumb
	.set sub_0804f5a0, 0x0804f5a0
	.set sub_0804fb0c, 0x0804fb0c
	.set sub_0804fbf8, 0x0804fbf8
	.set sub_0804fc78, 0x0804fc78
	.set sub_0804fdd8, 0x0804fdd8
	.set sub_0804fe3a, 0x0804fe3a
	.set sub_0804fe40, 0x0804fe40
	.set sub_0804fe88, 0x0804fe88
	.global Overlay_0804fb10
Overlay_0804fb10:
	push	{r4, r5, r7, lr}
	add	r4, pc, #268
	lsls	r2, r7, #1
	.2byte 0xe935
	.2byte 0xc006
	adds	r7, #116
	strb	r0, [r0, r4]
	lsls	r3, r2, #26
	ldr	r4, [pc, #768]
	movs	r7, r6
	str	r3, [sp, #340]
	stmia	r0!, {r1, r2}
	adds	r7, #76
	ldr	r4, [pc, #0]
	movs	r5, r2
	movs	r0, r0
	movs	r0, r0
	.2byte 0xfffe
	.2byte 0x0000
	movs	r0, r0
	.2byte 0xfe00
	.2byte 0x4ffd
	movs	r2, r1
	ldr	r7, [sp, #980]
	add	r0, pc, #80
	str	r3, [sp, #744]
	lsls	r2, r2, #26
	adds	r0, r4, #4
	movs	r1, r5
	ldr	r5, [sp, #372]
	add	r0, pc, #24
	.2byte 0xbbaa
	add	r0, pc, #12
	movs	r5, #59
	.2byte 0xe801
	.2byte 0x26ea
	strb	r1, [r0, #16]
	lsls	r7, r6, #13
	.2byte 0xbba0
	movs	r3, r3
	lsrs	r2, r5, #10
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	.2byte 0xff00
	.2byte 0x00ff
	movs	r0, r0
	movs	r0, r0
	ldrb	r4, [r5, #30]
	movs	r3, r1
	ldrsh	r5, [r6, r7]
	lsls	r6, r3, #27
	strb	r0, [r0, #29]
	lsls	r7, r4, #6
	ldr	r0, [r7, #124]
	lsls	r7, r4, #25
	ldr	r5, [sp, #512]
	lsls	r5, r1, #13
	ldr	r2, [pc, #576]
	add	r0, pc, #4
	subs	r3, r5, r6
	ble.n	.L_0804fb8e
.L_0804fb8c:
	lsls	r5, r3, #3
.L_0804fb8e:
	.2byte 0xeee8
	.2byte 0x4006
	adds	r7, #119
	strb	r0, [r0, r0]
	movs	r5, r2
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
.L_0804fb9e:
	.2byte 0xfffe
	.2byte 0x0000
	movs	r0, r0
	ldr	r5, [sp, #992]
	.2byte 0xfe54
	.2byte 0xabfb
	ldmia	r4!, {r1, r2}
.L_0804fbac:
	ldr	r4, [r4, #56]
	lsrs	r2, r5, #18
	ldmia	r7, {r4, r5, r6, r7}
	str	r6, [r5, #76]
	.2byte 0xfeff
	.2byte 0x01e4
	ldrb	r6, [r3, #6]
	ldrh	r6, [r3, r2]
	lsls	r6, r5, #3
	bls.n	.L_0804fb9e
	ldmia	r4, {r0, r3, r4, r6, r7}
	ldmia	r4!, {r1}
	ldr	r2, [r1, #104]
	ble.n	.L_0804fbca
	strh	r1, [r1, r1]
.L_0804fbca:
	strb	r0, [r0, r4]
	add	sp, pc
	stmia	r0!, {r1, r4, r5}
	.2byte 0xf74c
	.2byte 0x4004
	ldrh	r2, [r7, r6]
	add	r0, pc, #20
	ldr	r3, [sp, #684]
	add	r0, pc, #16
	lsls	r2, r5, #2
	movs	r0, r0
	movs	r0, r0
	.2byte 0xfff0
	.2byte 0x0000
	movs	r0, r0
	stmia	r7!, {r5, r7}
	.2byte 0xd27f
	.2byte 0xf800
	.2byte 0x969b
	str	r0, [sp, #24]
	strb	r2, [r3, r4]
	strh	r2, [r3, #0]
	.2byte 0xff7f
	.2byte 0xc9db
	ldr	r0, [pc, #0]
	ldr	r7, [r1, #84]
.L_0804fbfe:
	str	r6, [r0, #0]
	push	{r1, r2, r5, lr}
	lsls	r7, r2, #10
	ldr	r4, [r5, #76]
	ldr	r3, [r7, #72]
	ble.n	sub_0804fb0c
	.2byte 0xd86d
	ldr	r1, [sp, #512]
	lsls	r1, r5, #1
	strb	r4, [r6, #29]
	ands	r3, r0
	lsls	r5, r2, #5
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	.2byte 0xffe0
	.2byte 0x0000
	.2byte 0xfe00
	.2byte 0x149a
	lsls	r0, r1, #5
	ands	r4, r2
	str	r0, [sp, #596]
	asrs	r2, r0, #21
	ldrb	r0, [r1, #31]
	lsrs	r2, r2, #21
	ldr	r2, [pc, #672]
	add	r2, sp, #1020
	ands	r6, r3
	ldrh	r5, [r2, r1]
	lsls	r2, r1, #13
	ldrb	r4, [r1, #31]
	str	r3, [r4, #68]
	.2byte 0xf300
	.2byte 0x2edf
	adds	r0, #6
.L_0804fc44:
	pop	{r0, r2, r3, r4, r5, pc}
	asrs	r3, r3, #18
	.2byte 0xbbb0
	movs	r4, #205
	adds	r3, #1
	bhi.n	sub_0804fbf8
	str	r4, [r5, #4]
	subs	r7, r6, r5
	lsls	r0, r3
.L_0804fc56:
	lsls	r7, r6, #5
	movs	r6, r6
	movs	r0, r0
	.2byte 0xfc00
	.2byte 0x00ff
	movs	r0, r0
	movs	r0, r0
	.2byte 0xff3c
	.2byte 0x0149
	bmi.n	.L_0804fbac
	strh	r7, [r2, #6]
	ldrsb	r7, [r7, r6]
	.2byte 0xd776
	bvs.n	sub_0804fc78
	.2byte 0xbfe4
	.2byte 0x0bb1
	.2byte 0xf2f8
	.2byte 0x5d3c
	stmia	r0!, {r0, r2, r3}
	ble.n	.L_0804fbfe
	cmp	r6, #243
	.2byte 0xb180
	lsls	r5, r2, #11
	strb	r0, [r2, r4]
	blt.n	.L_0804fc44
	bge.n	.L_0804fb8c
	lsls	r5, r5, #1
	strb	r4, [r6, #29]
	add	r0, pc, #12
	subs	r3, r7, r6
	movs	r4, #0
	movs	r0, r0
	movs	r0, r0
	.2byte 0xf800
	.2byte 0x00ff
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	add	r0, pc, #0
	strb	r1, [r7, r7]
	.2byte 0xf200
	.2byte 0x2907
	ldrsb	r0, [r0, r5]
	cmp	r2, #90
	str	r1, [r5, r1]
	add	r7, sp, #924
	strb	r2, [r0, #25]
	ldrb	r7, [r2, #8]
	strh	r7, [r2, #42]
	cmp	r6, #236
	movs	r7, #164
	str	r2, [sp, #596]
	str	r1, [r7, #124]
	ldrh	r1, [r0, #54]
	ldr	r2, [pc, #676]
	lsls	r1, r5, #1
	movs	r0, r0
	movs	r0, r0
	movs	r0, r0
	.2byte 0xfc00
	.2byte 0x00ff
	movs	r0, r0
	movs	r0, r0
	add	r0, pc, #0
	strb	r1, [r7, r7]
	.2byte 0xf200
	.2byte 0x2907
	ldrsb	r0, [r0, r5]
	cmp	r2, #90
	str	r0, [sp, #420]
	adds	r7, #243
	ldr	r3, [sp, #132]
.L_0804fce8:
	ldmia	r4!, {r0, r2, r7}
	str	r0, [sp, #672]
.L_0804fcec:
	sbcs	r1, r1
	adds	r6, #116
	str	r3, [sp, #148]
	.2byte 0x441e
	add	r2, sp, #932
	adds	r4, #148
	str	r2, [sp, #672]
	lsls	r0, r2, #20
	lsrs	r0, r1, #10
	str	r0, [sp, #40]
	add	r2, sp
	.2byte 0xd035
	ldr	r0, [sp, #140]
	movs	r1, r3
	movs	r0, r0
	.2byte 0xfc00
	.2byte 0x00ff
	movs	r0, r0
	movs	r0, r0
	b.n	.L_0804fd16
	svc	231
.L_0804fd16:
	add	r5, pc, #508
	add	r2, sp, #4
	cmp	r7, #242
	lsrs	r5, r2, #21
	.2byte 0xffa8
	.2byte 0x93b2
	lsls	r4, r2, #25
	ldrb	r4, [r4, #9]
	ldr	r1, [pc, #232]
	lsrs	r6, r6, #12
	.2byte 0xb363
	bmi.n	.L_0804fc56
	cmn	r6, r2
	strh	r6, [r6, r1]
	ldmia	r6!, {r0, r1, r3, r4, r7}
	add	r1, pc, #408
	str	r2, [sp, #428]
	adds	r5, #36
	add	r0, pc, #212
	str	r2, [sp, #428]
	adds	r4, #84
	ldr	r6, [pc, #576]
	ldr	r1, [pc, #840]
	strh	r3, [r0, r0]
	add	r4, pc, #628
	lsls	r2, r4, #6
	movs	r0, r0
	movs	r0, r0
	.2byte 0xf000
	.2byte 0x00ff
	movs	r0, r0
.L_0804fd54:
	movs	r0, r0
	movs	r0, r0
	b.n	.L_0804fd5c
	svc	231
.L_0804fd5c:
	add	r5, pc, #508
.L_0804fd5e:
	add	r2, sp, #4
	cmp	r7, #242
	lsrs	r5, r2, #21
	ldr	r1, [pc, #672]
	add	r5, pc, #284
	b.n	sub_0804f5a0
	adds	r3, #63
	.2byte 0xed45
	.2byte 0xec2c
.L_0804fd70:
	cmp	r2, #218
.L_0804fd72:
	strb	r2, [r5, #29]
	bvc.n	sub_0804fe3a
	ldr	r2, [pc, #344]
	asrs	r6, r6, #29
	str	r6, [sp, #840]
	.2byte 0xf4c8
	.2byte 0xb480
	asrs	r4, r0, #18
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0x0000
	.2byte 0xfffc
	.2byte 0x0000
	movs	r0, r0
	b.n	.L_0804fd54
	ldrb	r7, [r3, #31]
	lsls	r5, r4, #6
	.2byte 0xf2aa
	.2byte 0x552f
	add	r4, pc, #52
	asrs	r6, r4, #20
	ldr	r2, [pc, #596]
	bpl.n	sub_0804fdd8
	add	r5, pc, #1020
	ldr	r2, [sp, #552]
	ldmia	r4, {r3, r4}
	cmp	r2, #232
	ldmia	r2!, {r1, r3, r5, r6}
	ldmia	r2!, {r5, r6}
	ldr	r2, [pc, #336]
	lsls	r6, r6, #24
	.2byte 0xb253
	ldr	r1, [sp, #164]
	lsrs	r0, r3, #32
	add	r4, pc, #76
	str	r0, [r0, r2]
	lsls	r1, r0, #5
	movs	r0, #164
	adds	r0, #10
	.2byte 0xbad0
	movs	r1, r0
	movs	r0, r0
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0x0000
	movs	r0, r0
	.2byte 0xfcf0
	.2byte 0x2ffb
	lsls	r5, r2, #3
	.2byte 0xf9d5
	.2byte 0xaa97
	adds	r2, #6
	add	r2, pc, #744
	ldr	r1, [pc, #328]
	.2byte 0xfea3
	.2byte 0x5533
	add	r4, pc, #848
	adds	r5, #7
	cmp	sl, sl
	bpl.n	sub_0804fe88
	str	r5, [sp, #128]
	str	r2, [sp, #580]
	lsrs	r5, r1, #29
	ldrh	r2, [r2, #10]
	subs	r2, #68
	.2byte 0xb20d
	bpl.n	.L_0804fd72
	strh	r2, [r4, #2]
	ldr	r4, [pc, #848]
	lsls	r5, r6, #12
	ldmia	r4!, {r2, r5, r7}
	ldrsb	r4, [r6, r5]
	eors	r2, r5
	lsls	r2, r3, #17
	lsrs	r5, r3, #21
	movs	r0, r0
	movs	r0, r0
	.2byte 0xc000
	.2byte 0x00ff
	.2byte 0x0000
	str	r0, [r0, #0]
	add	r4, pc, #984
	.2byte 0xfe01
	.2byte 0x7fe9
	bpl.n	sub_0804fe40
	mov	r8, r4
	.2byte 0x4526
	strh	r2, [r6, #24]
	.4byte 0x29b2ffda

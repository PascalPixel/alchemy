.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08002f40, 0x08002f40
	.set sub_080053e8, 0x080053e8
	.global Func_080f2028
	.thumb_func
Func_080f2028:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #416]
	ldr	r6, [r3, #0]
	ldr	r3, [pc, #416]
	ldrb	r3, [r3, #0]
	movs	r1, #0
	mov	r8, r1
	cmp	r3, #0
	bne.n	.L_080f2056
	ldr	r2, [r6, #12]
	movs	r3, #3
	adds	r2, #1
	ands	r3, r2
	str	r2, [r6, #12]
	cmp	r3, #0
	bne.n	.L_080f2056
	ldr	r3, [r6, #20]
	adds	r3, #1
	str	r3, [r6, #20]
.L_080f2056:
	ldr	r3, [pc, #388]
	ldrh	r2, [r3, #6]
	movs	r3, #48
	subs	r3, r3, r2
	ldr	r2, [r6, #20]
	mov	sl, r3
	movs	r3, #144
	subs	r1, r3, r2
	movs	r2, #140
	ldr	r3, [r6, #8]
	lsls	r2, r2, #1
	cmp	r3, r2
	blt.n	.L_080f2072
	b.n	.L_080f23ae
.L_080f2072:
	ldr	r3, [r6, #12]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080f207e
	b.n	.L_080f2208
.L_080f207e:
	ldr	r3, [pc, #352]
	mov	r2, sl
	ldrb	r3, [r3, #0]
	subs	r2, r1, r2
	adds	r7, r3, #0
	mov	r9, r2
	subs	r3, #104
	mov	r0, r9
	muls	r0, r3
	movs	r1, #80
	bl	sub_080022ec
	add	r0, sl
	adds	r5, r0, #0
	subs	r5, #16
	subs	r7, #16
	cmp	r5, #255
	ble.n	.L_080f20aa
	ldr	r3, [pc, #320]
.L_080f20a4:
	adds	r5, r5, r3
	cmp	r5, #255
	bgt.n	.L_080f20a4
.L_080f20aa:
	cmp	r5, #0
	bge.n	.L_080f20b8
	movs	r3, #128
	lsls	r3, r3, #1
.L_080f20b2:
	adds	r5, r5, r3
	cmp	r5, #0
	blt.n	.L_080f20b2
.L_080f20b8:
	adds	r0, r7, #4
	lsls	r0, r0, #16
	adds	r3, r0, #0
	ldr	r2, [pc, #296]
	orrs	r3, r5
	orrs	r3, r2
	movs	r1, #24
	str	r3, [r6, r1]
	adds	r1, r7, #0
	adds	r1, #20
	lsls	r1, r1, #16
	adds	r3, r1, #0
	ldr	r2, [pc, #280]
	orrs	r3, r5
	orrs	r3, r2
	adds	r2, r5, #0
	adds	r2, #16
	movs	r4, #32
	lsls	r2, r2, #24
	str	r3, [r6, r4]
	lsrs	r2, r2, #24
	ldr	r3, [pc, #268]
	orrs	r0, r2
	orrs	r0, r3
	ldr	r3, [pc, #264]
	movs	r4, #40
	orrs	r1, r2
	str	r0, [r6, r4]
	movs	r2, #232
	orrs	r1, r3
	movs	r0, #48
	movs	r3, #28
	str	r1, [r6, r0]
	str	r2, [r6, r3]
	movs	r3, #36
	str	r2, [r6, r3]
	movs	r3, #44
	str	r2, [r6, r3]
	movs	r3, #52
	str	r2, [r6, r3]
	ldr	r3, [pc, #212]
	ldrb	r3, [r3, #2]
	adds	r7, r3, #0
	subs	r3, #104
	mov	r0, r9
	muls	r0, r3
	movs	r1, #80
	bl	sub_080022ec
	add	r0, sl
	adds	r5, r0, #0
	subs	r5, #16
	subs	r7, #16
	cmp	r5, #255
	ble.n	.L_080f212e
	ldr	r3, [pc, #188]
.L_080f2128:
	adds	r5, r5, r3
	cmp	r5, #255
	bgt.n	.L_080f2128
.L_080f212e:
	cmp	r5, #0
	bge.n	.L_080f213c
	movs	r3, #128
	lsls	r3, r3, #1
.L_080f2136:
	adds	r5, r5, r3
	cmp	r5, #0
	blt.n	.L_080f2136
.L_080f213c:
	adds	r3, r7, #4
	ldr	r2, [pc, #184]
	lsls	r3, r3, #16
	orrs	r3, r5
	orrs	r3, r2
	movs	r1, #56
	str	r3, [r6, r1]
	movs	r2, #60
	movs	r3, #128
	str	r3, [r6, r2]
	movs	r3, #5
	mov	r8, r3
	ldr	r3, [pc, #136]
	ldrb	r3, [r3, #4]
	adds	r7, r3, #0
	subs	r3, #104
	mov	r0, r9
	muls	r0, r3
	movs	r1, #80
	bl	sub_080022ec
	add	r0, sl
	adds	r5, r0, #0
	subs	r5, #32
	subs	r7, #32
	cmp	r5, #255
	ble.n	.L_080f217a
	ldr	r3, [pc, #112]
.L_080f2174:
	adds	r5, r5, r3
	cmp	r5, #255
	bgt.n	.L_080f2174
.L_080f217a:
	cmp	r5, #0
	bge.n	.L_080f2188
	movs	r3, #128
	lsls	r3, r3, #1
.L_080f2182:
	adds	r5, r5, r3
	cmp	r5, #0
	blt.n	.L_080f2182
.L_080f2188:
	adds	r0, r7, #4
	lsls	r0, r0, #16
	mov	r1, r8
	adds	r3, r0, #0
	ldr	r2, [pc, #100]
	lsls	r1, r1, #3
	orrs	r3, r5
	mov	ip, r1
	orrs	r3, r2
	adds	r1, #24
	str	r3, [r6, r1]
	adds	r1, r7, #0
	adds	r1, #36
	lsls	r1, r1, #16
	adds	r3, r1, #0
	ldr	r2, [pc, #84]
	orrs	r3, r5
	orrs	r3, r2
	adds	r2, r5, #0
	mov	r4, ip
	adds	r2, #32
	adds	r4, #32
	lsls	r2, r2, #24
	str	r3, [r6, r4]
	lsrs	r2, r2, #24
	ldr	r3, [pc, #68]
	orrs	r0, r2
	orrs	r0, r3
	ldr	r3, [pc, #64]
	orrs	r1, r2
	adds	r4, #8
	str	r0, [r6, r4]
	orrs	r1, r3
	mov	r0, ip
	mov	r3, ip
	movs	r2, #192
	b.n	.L_080f2396
	movs	r0, r0
	.4byte 0x03001efc
	.4byte 0x03001d20
	.4byte 0x03001ad0
	.4byte 0x080f39ab
	.4byte 0xffffff00
	.4byte 0x40002400
	.4byte 0x50002400
	.4byte 0x60002400
	.4byte 0x70002400
	.4byte 0x80002400
	.4byte 0x90002400
	.4byte 0xa0002400
	.2byte 0x2400
	.2byte 0xb000
.L_080f2208:
	ldr	r3, [pc, #504]
	mov	r2, sl
	ldrb	r3, [r3, #1]
	subs	r2, r1, r2
	adds	r7, r3, #0
	mov	r9, r2
	subs	r3, #104
	mov	r0, r9
	muls	r0, r3
	movs	r1, #80
	bl	sub_080022ec
	add	r0, sl
	adds	r5, r0, #0
	subs	r5, #16
	subs	r7, #16
	cmp	r5, #255
	ble.n	.L_080f2234
	ldr	r3, [pc, #472]
.L_080f222e:
	adds	r5, r5, r3
	cmp	r5, #255
	bgt.n	.L_080f222e
.L_080f2234:
	cmp	r5, #0
	bge.n	.L_080f2242
	movs	r3, #128
	lsls	r3, r3, #1
.L_080f223c:
	adds	r5, r5, r3
	cmp	r5, #0
	blt.n	.L_080f223c
.L_080f2242:
	adds	r0, r7, #4
	lsls	r0, r0, #16
	adds	r3, r0, #0
	ldr	r2, [pc, #448]
	orrs	r3, r5
	orrs	r3, r2
	movs	r1, #24
	str	r3, [r6, r1]
	adds	r1, r7, #0
	adds	r1, #20
	lsls	r1, r1, #16
	adds	r3, r1, #0
	ldr	r2, [pc, #436]
	orrs	r3, r5
	orrs	r3, r2
	adds	r2, r5, #0
	adds	r2, #16
	movs	r4, #32
	lsls	r2, r2, #24
	str	r3, [r6, r4]
	lsrs	r2, r2, #24
	ldr	r3, [pc, #420]
	orrs	r0, r2
	orrs	r0, r3
	ldr	r3, [pc, #420]
	movs	r4, #40
	orrs	r1, r2
	str	r0, [r6, r4]
	movs	r2, #232
	orrs	r1, r3
	movs	r0, #48
	movs	r3, #28
	str	r1, [r6, r0]
	str	r2, [r6, r3]
	movs	r3, #36
	str	r2, [r6, r3]
	movs	r3, #44
	str	r2, [r6, r3]
	movs	r3, #52
	str	r2, [r6, r3]
	ldr	r3, [pc, #368]
	ldrb	r3, [r3, #3]
	adds	r7, r3, #0
	subs	r3, #104
	mov	r0, r9
	muls	r0, r3
	movs	r1, #80
	bl	sub_080022ec
	add	r0, sl
	adds	r5, r0, #0
	subs	r5, #16
	subs	r7, #16
	cmp	r5, #255
	ble.n	.L_080f22b8
	ldr	r3, [pc, #340]
.L_080f22b2:
	adds	r5, r5, r3
	cmp	r5, #255
	bgt.n	.L_080f22b2
.L_080f22b8:
	cmp	r5, #0
	bge.n	.L_080f22c6
	movs	r3, #128
	lsls	r3, r3, #1
.L_080f22c0:
	adds	r5, r5, r3
	cmp	r5, #0
	blt.n	.L_080f22c0
.L_080f22c6:
	adds	r0, r7, #4
	lsls	r0, r0, #16
	adds	r3, r0, #0
	ldr	r2, [pc, #316]
	orrs	r3, r5
	orrs	r3, r2
	movs	r1, #56
	str	r3, [r6, r1]
	adds	r1, r7, #0
	adds	r1, #20
	lsls	r1, r1, #16
	adds	r3, r1, #0
	ldr	r2, [pc, #304]
	orrs	r3, r5
	orrs	r3, r2
	adds	r2, r5, #0
	adds	r2, #16
	movs	r4, #64
	lsls	r2, r2, #24
	str	r3, [r6, r4]
	lsrs	r2, r2, #24
	ldr	r3, [pc, #288]
	orrs	r0, r2
	orrs	r0, r3
	ldr	r3, [pc, #288]
	movs	r4, #72
	orrs	r1, r2
	str	r0, [r6, r4]
	movs	r2, #224
	orrs	r1, r3
	movs	r0, #80
	movs	r3, #60
	str	r1, [r6, r0]
	str	r2, [r6, r3]
	movs	r3, #68
	str	r2, [r6, r3]
	movs	r3, #76
	str	r2, [r6, r3]
	movs	r3, #84
	str	r2, [r6, r3]
	movs	r3, #8
	mov	r8, r3
	ldr	r3, [pc, #232]
	ldrb	r3, [r3, #5]
	adds	r7, r3, #0
	subs	r3, #104
	mov	r0, r9
	muls	r0, r3
	movs	r1, #80
	bl	sub_080022ec
	add	r0, sl
	adds	r5, r0, #0
	subs	r5, #32
	subs	r7, #32
	cmp	r5, #255
	ble.n	.L_080f2340
	ldr	r3, [pc, #204]
.L_080f233a:
	adds	r5, r5, r3
	cmp	r5, #255
	bgt.n	.L_080f233a
.L_080f2340:
	cmp	r5, #0
	bge.n	.L_080f234e
	movs	r3, #128
	lsls	r3, r3, #1
.L_080f2348:
	adds	r5, r5, r3
	cmp	r5, #0
	blt.n	.L_080f2348
.L_080f234e:
	adds	r0, r7, #4
	lsls	r0, r0, #16
	mov	r1, r8
	adds	r3, r0, #0
	ldr	r2, [pc, #196]
	lsls	r1, r1, #3
	orrs	r3, r5
	mov	ip, r1
	orrs	r3, r2
	adds	r1, #24
	str	r3, [r6, r1]
	adds	r1, r7, #0
	adds	r1, #36
	lsls	r1, r1, #16
	adds	r3, r1, #0
	ldr	r2, [pc, #176]
	orrs	r3, r5
	orrs	r3, r2
	adds	r2, r5, #0
	mov	r4, ip
	adds	r2, #32
	adds	r4, #32
	lsls	r2, r2, #24
	str	r3, [r6, r4]
	lsrs	r2, r2, #24
	ldr	r3, [pc, #160]
	orrs	r0, r2
	orrs	r0, r3
	ldr	r3, [pc, #160]
	orrs	r1, r2
	adds	r4, #8
	str	r0, [r6, r4]
	orrs	r1, r3
	mov	r0, ip
	mov	r3, ip
	movs	r2, #160
.L_080f2396:
	adds	r3, #28
	adds	r0, #48
	str	r1, [r6, r0]
	str	r2, [r6, r3]
	adds	r3, #8
	str	r2, [r6, r3]
	adds	r3, #8
	str	r2, [r6, r3]
	adds	r3, #8
	str	r2, [r6, r3]
	movs	r2, #4
	add	r8, r2
.L_080f23ae:
	mov	r3, r8
	cmp	r3, #119
	bhi.n	.L_080f23c8
	lsls	r3, r3, #3
	ldr	r2, [pc, #116]
	adds	r3, #24
.L_080f23ba:
	movs	r1, #1
	add	r8, r1
	mov	r1, r8
	str	r2, [r6, r3]
	adds	r3, #8
	cmp	r1, #119
	bls.n	.L_080f23ba
.L_080f23c8:
	ldr	r2, [pc, #100]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #2
	strh	r3, [r2, #0]
	mov	r2, r8
	lsls	r5, r2, #3
	adds	r7, r6, #0
	movs	r4, #132
	lsrs	r2, r5, #2
	lsls	r4, r4, #24
	adds	r7, #24
	movs	r1, #224
	ldr	r3, [pc, #76]
	adds	r0, r7, #0
	lsls	r1, r1, #19
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #68]
	ldrh	r2, [r3, #6]
	movs	r3, #32
	subs	r2, r3, r2
	b.n	.L_080f243c
	movs	r0, r0
	.4byte 0x00003f50
	.4byte 0x00000e0e
	.4byte 0x080f39ab
	.4byte 0xffffff00
	.4byte 0x40002400
	.4byte 0x50002400
	.4byte 0x60002400
	.4byte 0x70002400
	.4byte 0x80002400
	.4byte 0x90002400
	.4byte 0xa0002400
	.4byte 0xb0002400
	.4byte 0x400020a0
	.4byte 0x04000050
	.4byte 0x040000d4
	.2byte 0x1ad0
	.2byte 0x0300
.L_080f243c:
	cmp	r2, #255
	ble.n	.L_080f2448
	ldr	r3, [pc, #72]
.L_080f2442:
	adds	r2, r2, r3
	cmp	r2, #255
	bgt.n	.L_080f2442
.L_080f2448:
	cmp	r2, #0
	bge.n	.L_080f2456
	movs	r3, #128
	lsls	r3, r3, #1
.L_080f2450:
	adds	r2, r2, r3
	cmp	r2, #0
	blt.n	.L_080f2450
.L_080f2456:
	ldr	r3, [pc, #56]
	movs	r4, #224
	orrs	r2, r3
	movs	r3, #128
	lsls	r3, r3, #4
	lsls	r4, r4, #19
	adds	r0, r6, #0
	str	r2, [r6, #120]
	adds	r0, #120
	str	r3, [r6, #124]
	adds	r1, r5, r4
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #40]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r7, #0
	adds	r1, r4, #0
	ldr	r2, [pc, #32]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0xffffff00
	.4byte 0xc05c2000
	.4byte 0x040000d4
	.4byte 0x84000002
	.2byte 0x0008
	.2byte 0x8400
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #60]
	ldr	r6, [pc, #52]
	ldr	r7, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	strh	r6, [r3, #0]
	ldr	r0, [pc, #52]
	bl	sub_08002f40
	ldr	r3, [pc, #48]
	adds	r4, r0, #0
	ldr	r1, [pc, #48]
	ldr	r2, [pc, #52]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #40]
	strh	r6, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	mov	r8, r3
	ldr	r5, [pc, #40]
	add	r4, r8
	adds	r1, r5, #0
	adds	r0, r4, #0
	bl	sub_080053e8
	ldr	r3, [pc, #16]
	b.n	.L_080f24fc
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001efc
	.4byte 0x00000015
	.4byte 0x040000d4
	.4byte 0x05000200
	.4byte 0x84000080
	.2byte 0x0000
	.2byte 0x0201
.L_080f24fc:
	adds	r0, r5, #0
	ldr	r1, [pc, #308]
	ldr	r2, [pc, #308]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #308]
	bl	sub_08002f40
	movs	r1, #160
	adds	r4, r0, #0
	ldr	r3, [pc, #300]
	lsls	r1, r1, #19
	ldr	r2, [pc, #300]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #160
	lsls	r3, r3, #19
	add	r4, r8
	strh	r6, [r3, #0]
	adds	r1, r5, #0
	adds	r0, r4, #0
	bl	sub_080053e8
	movs	r1, #192
	ldr	r3, [pc, #272]
	ldr	r0, [pc, #280]
	lsls	r1, r1, #19
	ldr	r2, [pc, #280]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #276]
	ldr	r1, [pc, #280]
	ldr	r2, [pc, #280]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [pc, #280]
	ldr	r1, [pc, #280]
	ldr	r3, [pc, #284]
	movs	r4, #0
.L_080f254a:
	movs	r0, #29
.L_080f254c:
	adds	r2, r3, #0
	movs	r6, #128
	lsls	r3, r2, #16
	lsls	r6, r6, #9
	adds	r3, r3, r6
	subs	r0, #1
	strh	r2, [r1, #0]
	asrs	r3, r3, #16
	adds	r1, #2
	cmp	r0, #0
	bge.n	.L_080f254c
	strh	r5, [r1, #0]
	adds	r4, #1
	adds	r1, #2
	strh	r5, [r1, #0]
	adds	r1, #2
	cmp	r4, #10
	ble.n	.L_080f254a
	ldr	r3, [pc, #244]
	movs	r4, #11
.L_080f2574:
	movs	r0, #29
.L_080f2576:
	adds	r2, r3, #0
	movs	r6, #128
	lsls	r3, r2, #16
	lsls	r6, r6, #9
	adds	r3, r3, r6
	subs	r0, #1
	strh	r2, [r1, #0]
	asrs	r3, r3, #16
	adds	r1, #2
	cmp	r0, #0
	bge.n	.L_080f2576
	strh	r5, [r1, #0]
	adds	r4, #1
	adds	r1, #2
	strh	r5, [r1, #0]
	adds	r1, #2
	cmp	r4, #31
	ble.n	.L_080f2574
	movs	r3, #150
	ldr	r1, [pc, #204]
	lsls	r3, r3, #1
	movs	r4, #0
.L_080f25a2:
	movs	r0, #29
.L_080f25a4:
	adds	r2, r3, #0
	movs	r6, #128
	lsls	r3, r2, #16
	lsls	r6, r6, #9
	adds	r3, r3, r6
	subs	r0, #1
	strh	r2, [r1, #0]
	asrs	r3, r3, #16
	adds	r1, #2
	cmp	r0, #0
	bge.n	.L_080f25a4
	strh	r5, [r1, #0]
	adds	r4, #1
	adds	r1, #2
	strh	r5, [r1, #0]
	adds	r1, #2
	cmp	r4, #10
	ble.n	.L_080f25a2
	movs	r3, #0
	movs	r4, #11
.L_080f25cc:
	movs	r0, #29
.L_080f25ce:
	adds	r2, r3, #0
	movs	r6, #128
	lsls	r3, r2, #16
	lsls	r6, r6, #9
	adds	r3, r3, r6
	subs	r0, #1
	strh	r2, [r1, #0]
	asrs	r3, r3, #16
	adds	r1, #2
	cmp	r0, #0
	bge.n	.L_080f25ce
	strh	r5, [r1, #0]
	adds	r4, #1
	adds	r1, #2
	strh	r5, [r1, #0]
	adds	r1, #2
	cmp	r4, #31
	ble.n	.L_080f25cc
	ldr	r2, [pc, #124]
	ldr	r3, [pc, #40]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r1, [pc, #40]
	ldr	r3, [pc, #112]
	ldr	r2, [pc, #40]
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	ldr	r2, [pc, #100]
	ldr	r3, [pc, #24]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #96]
	movs	r2, #0
	movs	r4, #3
	b.n	.L_080f2680
	.4byte 0x00001f43
	.4byte 0x00001e81
	.4byte 0x000000f0
	.4byte 0x0000009f
	.4byte 0x00001616
	.4byte 0x06010000
	.4byte 0x80000f00
	.4byte 0x00000017
	.4byte 0x040000d4
	.4byte 0x84000080
	.4byte 0x02012940
	.4byte 0x80002760
	.4byte 0x0201a140
	.4byte 0x06004ec0
	.4byte 0x80004ec0
	.4byte 0x000001ff
	.4byte 0x0600f000
	.4byte 0x00000267
	.4byte 0x0000013b
	.4byte 0x0600f800
	.4byte 0x0400000a
	.4byte 0x04000040
	.4byte 0x04000048
	.2byte 0x1ad0
	.2byte 0x0300
.L_080f2680:
	subs	r4, #1
	strh	r2, [r3, #2]
	strh	r2, [r3, #0]
	adds	r3, #4
	cmp	r4, #0
	bge.n	.L_080f2680
	ldr	r0, [pc, #60]
	movs	r3, #0
	movs	r2, #96
	strh	r2, [r0, #6]
	strh	r2, [r0, #10]
	str	r3, [r7, #8]
	str	r3, [r7, #0]
	str	r3, [r7, #4]
	str	r3, [r7, #12]
	str	r3, [r7, #20]
	str	r3, [r7, #16]
	ldr	r1, [pc, #44]
	ldr	r3, [pc, #44]
	ldr	r2, [pc, #48]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #16]
	adds	r1, #64
	strh	r3, [r1, #0]
	ldr	r2, [pc, #40]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #12]
	strh	r3, [r1, #0]
	b.n	.L_080f26e0
	movs	r0, r0
	.4byte 0x00003fbf
	.4byte 0x00001010
	.4byte 0x00003f44
	.4byte 0x03001ad0
	.4byte 0x04000010
	.4byte 0x040000d4
	.4byte 0x84000004
	.2byte 0x0052
	.2byte 0x0400
.L_080f26e0:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000

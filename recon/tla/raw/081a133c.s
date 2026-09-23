.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080140d8, 0x080140d8
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_080144c0, 0x080144c0
	.set sub_080145a8, 0x080145a8
	.set sub_08014d78, 0x08014d78
	.set sub_08014dac, 0x08014dac
	.set sub_081a04d0, 0x081a04d0
	.set sub_081a0674, 0x081a0674
	.global Func_081a133c
	.thumb_func
Func_081a133c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #416]
	ldr	r2, [pc, #416]
	ldr	r5, [r3, #0]
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	ldrh	r1, [r2, #0]
	cmp	r3, #63
	bgt.n	.L_081a138c
	adds	r3, r1, #1
	strh	r3, [r2, #0]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, #0
	bge.n	.L_081a1360
	adds	r3, #3
.L_081a1360:
	asrs	r1, r3, #2
	ldr	r3, [pc, #392]
	ldrh	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081a137c
	movs	r3, #16
	subs	r3, r3, r1
	movs	r2, #128
	lsls	r3, r3, #8
	lsls	r2, r2, #19
	orrs	r3, r1
	b.n	.L_081a1388
.L_081a137c:
	movs	r2, #16
	subs	r2, r2, r1
	lsls	r3, r1, #8
	orrs	r3, r2
	movs	r2, #128
	lsls	r2, r2, #19
.L_081a1388:
	adds	r2, #82
	strh	r3, [r2, #0]
.L_081a138c:
	ldr	r3, [pc, #352]
	ldr	r1, [pc, #356]
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	movs	r6, #0
	ldrsh	r3, [r1, r6]
	adds	r0, r1, #0
	cmp	r2, r3
	beq.n	.L_081a13c4
	subs	r1, r2, r3
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_081a13a8
	negs	r2, r1
.L_081a13a8:
	ldr	r3, [pc, #332]
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r2, r3
	ble.n	.L_081a13bc
	cmp	r1, #0
	ble.n	.L_081a13ba
	adds	r1, r3, #0
	b.n	.L_081a13bc
.L_081a13ba:
	negs	r1, r3
.L_081a13bc:
	ldrh	r3, [r0, #0]
	adds	r3, r3, r1
	strh	r3, [r0, #0]
	adds	r1, r0, #0
.L_081a13c4:
	ldr	r0, [pc, #284]
	movs	r6, #0
	ldrsh	r3, [r1, r6]
	adds	r4, r0, #0
	lsls	r2, r3, #16
	ldr	r3, [r0, #0]
	cmp	r3, r2
	beq.n	.L_081a1420
	subs	r1, r2, r3
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_081a13de
	negs	r2, r1
.L_081a13de:
	ldr	r0, [pc, #280]
	movs	r6, #0
	ldrsh	r3, [r0, r6]
	lsls	r3, r3, #14
	cmp	r2, r3
	ble.n	.L_081a13f4
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_081a13f2
	adds	r3, r1, #7
.L_081a13f2:
	asrs	r1, r3, #3
.L_081a13f4:
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_081a13fc
	negs	r2, r1
.L_081a13fc:
	movs	r6, #0
	ldrsh	r3, [r0, r6]
	lsls	r3, r3, #16
	cmp	r2, r3
	ble.n	.L_081a1410
	cmp	r1, #0
	ble.n	.L_081a140e
	adds	r1, r3, #0
	b.n	.L_081a1410
.L_081a140e:
	negs	r1, r3
.L_081a1410:
	ldr	r3, [r4, #0]
	ldr	r2, [pc, #232]
	adds	r3, r3, r1
	str	r3, [r4, #0]
	adds	r0, r4, #0
	ldr	r3, [r2, #0]
	adds	r3, r3, r1
	str	r3, [r2, #0]
.L_081a1420:
	ldr	r3, [r0, #0]
	movs	r2, #128
	eors	r3, r5
	lsls	r2, r2, #12
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081a14d4
	ldr	r3, [pc, #208]
	movs	r7, #192
	ldr	r6, [r3, #0]
	ldr	r3, [pc, #196]
	lsls	r7, r7, #19
	ldr	r3, [r3, #0]
	lsrs	r3, r3, #19
	lsls	r1, r3, #6
	ldr	r3, [pc, #172]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_081a144a
	ldr	r7, [pc, #184]
.L_081a144a:
	ldr	r3, [r0, #0]
	cmp	r3, r5
	ble.n	.L_081a1496
	lsrs	r3, r3, #19
	movs	r2, #240
	lsls	r2, r2, #3
	lsls	r3, r3, #6
	adds	r3, r3, r2
	mov	ip, r3
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r3, #255
	adds	r4, r1, r2
	ands	r4, r3
	movs	r0, #128
	movs	r3, #128
	lsls	r3, r3, #5
	lsls	r0, r0, #4
	movs	r5, #0
	mov	r8, r3
	mov	lr, r0
.L_081a1474:
	mov	r2, ip
	adds	r0, r6, r2
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, r7, r4
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r5, #1
	add	r6, r8
	add	r7, lr
	cmp	r5, #14
	ble.n	.L_081a1474
	b.n	.L_081a14d4
.L_081a1496:
	ldr	r3, [r0, #0]
	movs	r4, #224
	lsrs	r3, r3, #19
	lsls	r3, r3, #6
	mov	ip, r3
	lsls	r4, r4, #3
	movs	r3, #128
	movs	r0, #128
	adds	r4, #255
	lsls	r3, r3, #5
	lsls	r0, r0, #4
	ands	r4, r1
	mov	r8, r3
	mov	lr, r0
	movs	r5, #14
.L_081a14b4:
	mov	r2, ip
	adds	r0, r6, r2
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, r7, r4
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	subs	r5, #1
	add	r6, r8
	add	r7, lr
	cmp	r5, #0
	bge.n	.L_081a14b4
.L_081a14d4:
	ldr	r3, [pc, #36]
	ldrh	r1, [r3, #2]
	ldr	r3, [pc, #44]
	strh	r1, [r3, #8]
	strh	r1, [r3, #12]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x0200751c
	.4byte 0x02007516
	.4byte 0x02007514
	.4byte 0x02007522
	.4byte 0x02007520
	.4byte 0x02007524
	.4byte 0x02007518
	.4byte 0x02007528
	.4byte 0x06008000
	.2byte 0x1120
	.2byte 0x0300
	push	{r5, lr}
	ldr	r3, [pc, #104]
	movs	r2, #0
	strb	r2, [r3, #0]
	ldr	r3, [pc, #100]
	strb	r2, [r3, #0]
	ldr	r3, [pc, #100]
	strb	r2, [r3, #0]
	bl	sub_080144c0
	ldr	r3, [pc, #96]
	movs	r2, #0
	str	r2, [r3, #0]
	ldr	r3, [pc, #96]
	ldr	r1, [pc, #96]
	str	r2, [r3, #0]
	ldr	r3, [pc, #96]
	strh	r2, [r1, #0]
	ldr	r0, [pc, #96]
	str	r2, [r3, #0]
	ldr	r1, [pc, #96]
	ldr	r3, [pc, #60]
	strh	r2, [r0, #0]
	strh	r3, [r1, #0]
	ldr	r3, [pc, #92]
	movs	r1, #144
	strh	r2, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #88]
	bl	sub_080145a8
	movs	r3, #64
	movs	r5, #128
	lsls	r5, r5, #19
	strh	r3, [r5, #0]
	ldr	r0, [pc, #80]
	bl	sub_081a0674
	ldr	r0, [pc, #76]
	bl	sub_081a0674
	movs	r0, #0
	bl	sub_081a04d0
	movs	r0, #1
	bl	sub_081a04d0
	movs	r2, #248
	movs	r3, #128
	lsls	r2, r2, #5
	lsls	r3, r3, #19
	b.n	.L_081a15ac
	.4byte 0x00000001
	.4byte 0x0300123c
	.4byte 0x03001110
	.4byte 0x03001200
	.4byte 0x02007518
	.4byte 0x0200751c
	.4byte 0x02007520
	.4byte 0x02007528
	.4byte 0x02007522
	.4byte 0x02007524
	.4byte 0x02007514
	.4byte 0x081a133d
	.4byte 0x06007800
	.2byte 0xf800
	.2byte 0x0600
.L_081a15ac:
	adds	r2, #138
	adds	r3, #12
	strh	r2, [r3, #0]
	movs	r2, #240
	lsls	r2, r2, #4
	adds	r2, #131
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r3, #226
	lsls	r3, r3, #5
	strh	r3, [r5, #0]
	movs	r2, #160
	movs	r3, #128
	lsls	r2, r2, #6
	lsls	r3, r3, #19
	adds	r2, #68
	adds	r3, #80
	strh	r2, [r3, #0]
	movs	r2, #16
	adds	r3, #2
	strh	r2, [r3, #0]
	bl	.L_081a1830
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	adds	r7, r1, #0
	adds	r4, r0, #0
	movs	r1, #0
	mov	sl, r2
	str	r1, [sp, #0]
	cmp	r4, #0
	bne.n	.L_081a1648
	ldr	r3, [pc, #396]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_081a1610
	movs	r1, #192
	movs	r2, #160
	lsls	r1, r1, #19
	lsls	r2, r2, #19
	ldr	r3, [pc, #380]
	b.n	.L_081a1616
.L_081a1610:
	ldr	r1, [pc, #380]
	ldr	r2, [pc, #384]
	ldr	r3, [pc, #384]
.L_081a1616:
	mov	fp, r1
	mov	r8, r2
	add	r4, sp, #4
	str	r3, [r4, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r4, #0
	adds	r1, r7, #0
	ldr	r2, [pc, #368]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #0
	str	r3, [r4, #0]
	movs	r2, #133
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r4, #0
	mov	r1, r8
	adds	r2, #64
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L_081a16e8
.L_081a1648:
	ldr	r3, [pc, #316]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_081a165c
	movs	r2, #192
	movs	r3, #160
	lsls	r2, r2, #19
	lsls	r3, r3, #19
	b.n	.L_081a1664
.L_081a165c:
	ldr	r1, [pc, #320]
	ldr	r2, [pc, #304]
	ldr	r3, [pc, #304]
	str	r1, [sp, #0]
.L_081a1664:
	mov	fp, r2
	mov	r8, r3
	add	r0, sp, #4
	movs	r3, #0
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	mov	r1, fp
	ldr	r2, [pc, #300]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r4, #0
	bl	sub_08013300
	mov	r9, r0
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
	ldr	r0, [pc, #272]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r1, r7, #0
	ldr	r2, [sp, #0]
	add	r0, r9
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c30
	bl	sub_08013164
	ldr	r1, [pc, #244]
	ldr	r0, [pc, #248]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_081a16e6
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r1
	adds	r2, #1
	adds	r3, #4
	strh	r2, [r1, #0]
	mov	r1, r9
	stmia	r3!, {r1}
	mov	r2, r8
	stmia	r3!, {r2}
	movs	r2, #132
	lsls	r2, r2, #24
	adds	r2, #64
	str	r2, [r3, #0]
.L_081a16e6:
	strh	r4, [r0, #0]
.L_081a16e8:
	ldr	r3, [pc, #204]
	ldr	r2, [pc, #208]
	str	r7, [r3, #0]
	ldr	r3, [pc, #208]
	mov	r1, sl
	ldr	r3, [r3, #0]
	mov	ip, r7
	lsrs	r3, r3, #19
	lsls	r3, r3, #6
	mov	r9, r3
	ldr	r3, [pc, #196]
	mov	r7, fp
	strh	r1, [r3, #0]
	mov	r3, sl
	strh	r3, [r2, #0]
	ldr	r2, [pc, #192]
	mov	r1, sl
	lsls	r3, r1, #16
	str	r3, [r2, #0]
	movs	r3, #128
	movs	r2, #0
	lsls	r3, r3, #5
	mov	r8, r2
	mov	fp, r3
.L_081a1718:
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_081a1720
	adds	r3, #7
.L_081a1720:
	movs	r1, #224
	lsls	r1, r1, #3
	adds	r1, #255
	asrs	r3, r3, #3
	mov	r4, r9
	lsls	r5, r3, #6
	ands	r4, r1
	mov	lr, r1
	movs	r6, #30
.L_081a1732:
	mov	r2, ip
	adds	r0, r2, r5
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, r7, r4
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r4, #64
	mov	r3, lr
	subs	r6, #1
	adds	r5, #64
	ands	r4, r3
	cmp	r6, #0
	bge.n	.L_081a1732
	movs	r2, #1
	movs	r1, #128
	add	r8, r2
	lsls	r1, r1, #4
	mov	r3, r8
	add	ip, fp
	adds	r7, r7, r1
	cmp	r3, #14
	ble.n	.L_081a1718
	ldr	r1, [pc, #28]
	add	sp, #8
	ldrh	r3, [r1, #0]
	eors	r3, r2
	strh	r3, [r1, #0]
	ldr	r2, [pc, #88]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x00000000
	.4byte 0x02007514
	.4byte 0x01010101
	.4byte 0x06008000
	.4byte 0x05000100
	.4byte 0x81818181
	.4byte 0x85003c00
	.4byte 0x80808080
	.4byte 0x85000200
	.4byte 0x00000234
	.4byte 0x081a029c
	.4byte 0x020038e0
	.4byte 0x04000208
	.4byte 0x02007528
	.4byte 0x02007522
	.4byte 0x02007518
	.4byte 0x02007520
	.4byte 0x0200751c
	.2byte 0x7516
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r0, [pc, #52]
	ldr	r1, [pc, #52]
	movs	r2, #0
	ldrsh	r3, [r0, r2]
	ldr	r2, [r1, #0]
	lsls	r3, r3, #16
	movs	r5, #0
	cmp	r2, r3
	beq.n	.L_081a1804
	adds	r7, r1, #0
	adds	r6, r0, #0
.L_081a17e8:
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #44
	adds	r5, #1
	adds	r3, #255
	cmp	r5, r3
	bgt.n	.L_081a1804
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	ldr	r2, [r7, #0]
	lsls	r3, r3, #16
	cmp	r2, r3
	bne.n	.L_081a17e8
.L_081a1804:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02007520
	.2byte 0x751c
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r3, [pc, #24]
	movs	r6, #23
	ldr	r5, [r3, #0]
.L_081a1818:
	adds	r0, r5, #0
	movs	r1, #0
	subs	r6, #1
	adds	r5, #12
	bl	sub_080140d8
	cmp	r6, #0
	bge.n	.L_081a1818
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x752c
	.2byte 0x0200
.L_081a1830:
	push	{r5, r6, r7, lr}
	sub	sp, #4
	bl	sub_080143ac
	adds	r5, r0, #0
	movs	r0, #144
	lsls	r0, r0, #1
	bl	sub_08014dac
	ldr	r6, [pc, #152]
	movs	r1, #128
	str	r0, [r6, #0]
	lsls	r1, r1, #2
	adds	r0, r5, #0
	movs	r2, #0
	bl	sub_080142d4
	ldr	r3, [pc, #140]
	ldr	r2, [pc, #140]
	adds	r4, r0, #0
	lsls	r1, r4, #5
	mov	r0, sp
	str	r3, [r0, #0]
	adds	r1, r1, r2
	movs	r3, #128
	movs	r2, #133
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r6, [r6, #0]
	ldr	r0, [pc, #116]
	movs	r1, #0
	movs	r5, #0
	adds	r2, r6, #0
.L_081a187a:
	lsls	r3, r1, #21
	orrs	r3, r0
	adds	r1, #1
	str	r5, [r2, #0]
	str	r3, [r2, #4]
	str	r4, [r2, #8]
	adds	r6, #12
	adds	r2, #12
	cmp	r1, #7
	bls.n	.L_081a187a
	movs	r0, #128
	ldr	r5, [pc, #88]
	lsls	r0, r0, #4
	movs	r1, #0
	movs	r7, #0
	orrs	r0, r4
	adds	r2, r6, #0
.L_081a189c:
	lsls	r3, r1, #21
	orrs	r3, r5
	adds	r1, #1
	str	r7, [r2, #0]
	str	r3, [r2, #4]
	str	r0, [r2, #8]
	adds	r6, #12
	adds	r2, #12
	cmp	r1, #7
	bls.n	.L_081a189c
	movs	r2, #128
	ldr	r0, [pc, #60]
	lsls	r2, r2, #4
	movs	r1, #0
	movs	r5, #0
	orrs	r2, r4
.L_081a18bc:
	lsls	r3, r1, #21
	orrs	r3, r0
	adds	r1, #1
	str	r5, [r6, #0]
	str	r3, [r6, #4]
	str	r2, [r6, #8]
	adds	r6, #12
	cmp	r1, #7
	bls.n	.L_081a18bc
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #32]
	bl	sub_080145a8
	add	sp, #4
	pop	{r5, r6, r7, pc}
	.4byte 0x0200752c
	.4byte 0x11111111
	.4byte 0x06010000
	.4byte 0x80004078
	.4byte 0x80004088
	.4byte 0x40004098
	.2byte 0x1811
	.2byte 0x081a
	push	{lr}
	ldr	r3, [pc, #40]
	ldr	r3, [r3, #0]
	cmp	r0, #0
	bne.n	.L_081a1912
	movs	r2, #0
	movs	r1, #0
.L_081a1906:
	adds	r2, #1
	strb	r1, [r3, #4]
	adds	r3, #12
	cmp	r2, #7
	bls.n	.L_081a1906
	b.n	.L_081a1920
.L_081a1912:
	movs	r2, #0
	movs	r1, #120
.L_081a1916:
	adds	r2, #1
	strb	r1, [r3, #4]
	adds	r3, #12
	cmp	r2, #7
	bls.n	.L_081a1916
.L_081a1920:
	pop	{pc}
	movs	r0, r0
	.4byte 0x0200752c

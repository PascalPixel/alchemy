.syntax unified
	.thumb
	.set sub_020004be, 0x020004be
	.set sub_020005c0, 0x020005c0
	.set sub_020005d2, 0x020005d2
	.set sub_02000620, 0x02000620
	.set sub_02000656, 0x02000656
	.set sub_02000662, 0x02000662
	.set sub_02000668, 0x02000668
	.set sub_0200066c, 0x0200066c
	.set sub_0200066e, 0x0200066e
	.set sub_0200067c, 0x0200067c
	.set sub_02000680, 0x02000680
	.set sub_02000692, 0x02000692
	.set sub_0200069e, 0x0200069e
	.set sub_020006a4, 0x020006a4
	.set sub_020006ac, 0x020006ac
	.set sub_020006b4, 0x020006b4
	.set sub_020006b8, 0x020006b8
	.set sub_020006c0, 0x020006c0
	.set sub_020006ce, 0x020006ce
	.set sub_020006e2, 0x020006e2
	.set sub_020006ec, 0x020006ec
	.set sub_020006f0, 0x020006f0
	.set sub_020006f4, 0x020006f4
	.set sub_02000706, 0x02000706
	.set sub_0200070a, 0x0200070a
	.set sub_0200070c, 0x0200070c
	.set sub_02000710, 0x02000710
	.set sub_02000716, 0x02000716
	.set sub_02000718, 0x02000718
	.set sub_0200072a, 0x0200072a
	.set sub_02000736, 0x02000736
	.set sub_0200073e, 0x0200073e
	.set sub_02000740, 0x02000740
	.set sub_02000742, 0x02000742
	.set sub_02000744, 0x02000744
	.set sub_02000750, 0x02000750
	.set sub_0200075c, 0x0200075c
	.set sub_02000766, 0x02000766
	.set sub_02000768, 0x02000768
	.set sub_0200076a, 0x0200076a
	.set sub_0200076e, 0x0200076e
	.set sub_02000784, 0x02000784
	.set sub_02000824, 0x02000824
	.set sub_02000848, 0x02000848
	.set sub_0200089a, 0x0200089a
	.set sub_02000942, 0x02000942
	.set sub_0200097c, 0x0200097c
	.set sub_020009e4, 0x020009e4
	.set sub_020009e6, 0x020009e6
	.set sub_020009e8, 0x020009e8
	.set sub_020009f4, 0x020009f4
	.global Overlay_02000000
Overlay_02000000:
	.4byte 0x47204c00
	.4byte 0x02008055
	.4byte 0x47204c00
	.4byte 0x02008031
	.4byte 0x47204c00
	.4byte 0x0200803d
	.4byte 0x47204c00
	.4byte 0x02008045
	.4byte 0x47204c00
	.4byte 0x0200804d
	.4byte 0x47204c00
	.4byte 0x02008039
	.4byte 0x47704800
	.4byte 0x020085f8
	.4byte 0x47702000
	.4byte 0x47704800
	.4byte 0x02008628
	.4byte 0x47704800
	.4byte 0x0200862c
	.4byte 0x47704800
	.2byte 0x8644
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r2, [pc, #328]
	movs	r1, #225
	lsls	r1, r1, #1
	adds	r5, r2, r1
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #10
	bne.n	.L_020000b4
	movs	r1, #250
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldr	r0, [r3, #0]
	bl	sub_02000620
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #75
	bl	sub_0200066c
	movs	r0, #0
	bl	.L_0200036a
	movs	r0, #120
	bl	sub_020005c0
	ldr	r2, [pc, #280]
	ldr	r3, [r2, #0]
	movs	r5, #0
	cmp	r3, #0
	bne.n	.L_020000aa
	adds	r6, r2, #0
.L_02000096:
	movs	r0, #1
	bl	sub_020005d2
	ldr	r2, [pc, #264]
	adds	r5, #1
	cmp	r5, r2
	bgt.n	.L_020000aa
	ldr	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_02000096
.L_020000aa:
	ldr	r0, [pc, #256]
	movs	r1, #2
	bl	sub_02000668
	b.n	.L_02000196
.L_020000b4:
	cmp	r3, #9
	bne.n	.L_020000ea
	movs	r0, #67
	bl	sub_020006ac
	movs	r0, #0
	bl	sub_02000692
	movs	r0, #17
	bl	sub_020006b8
	movs	r0, #60
	bl	sub_02000656
	bl	sub_02000662
	movs	r0, #240
	bl	sub_02000680
	movs	r0, #19
	bl	sub_020006ce
	ldr	r0, [pc, #204]
	movs	r1, #2
	bl	sub_0200069e
	b.n	.L_02000196
.L_020000ea:
	ldr	r0, [pc, #200]
	bl	sub_0200066e
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #2
	bne.n	.L_0200015c
.L_020000f8:
	movs	r0, #19
	bl	sub_020006ec
	movs	r0, #0
	bl	sub_020006e2
	movs	r0, #0
	bl	sub_020006f0
	bl	sub_020006a4
	cmp	r0, #0
	ble.n	.L_02000152
	movs	r0, #70
	bl	sub_02000706
	movs	r0, #1
	bl	sub_020006f4
	cmp	r0, #0
	bne.n	.L_02000152
	movs	r0, #17
	bl	sub_02000716
	movs	r0, #30
	bl	sub_020006b4
	bl	sub_020006c0
	ldr	r2, [pc, #132]
	ldr	r3, [r2, #0]
	movs	r5, #0
	cmp	r3, #0
	bne.n	.L_020000f8
	adds	r6, r2, #0
.L_0200013e:
	movs	r0, #1
	adds	r5, #1
	bl	sub_0200067c
	cmp	r5, #119
	bgt.n	.L_020000f8
	ldr	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L_0200013e
	b.n	.L_020000f8
.L_02000152:
	ldr	r0, [pc, #92]
	movs	r1, #1
	bl	sub_02000710
	b.n	.L_0200017a
.L_0200015c:
	movs	r0, #64
	bl	sub_02000750
	movs	r0, #0
	bl	sub_0200073e
	bl	sub_0200070a
	ldr	r0, [pc, #76]
	movs	r1, #16
	bl	sub_0200072a
	movs	r0, #17
	bl	sub_02000768
.L_0200017a:
	movs	r0, #17
	bl	sub_0200076e
	movs	r0, #30
	bl	sub_0200070c
	bl	sub_02000718
	movs	r0, #60
	bl	sub_02000736
	movs	r0, #19
	bl	sub_02000784
.L_02000196:
	movs	r0, #0
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x03001c94
	.4byte 0x00000e0f
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000b
	.4byte 0x03001ae8
	.2byte 0x0004
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #164
	lsls	r0, r0, #3
	bl	sub_02000710
	ldr	r6, [pc, #88]
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	movs	r2, #1
	negs	r2, r2
	adds	r5, r0, #0
	cmp	r3, r2
	bne.n	.L_020001e0
	bl	sub_02000744
	strh	r0, [r6, #0]
.L_020001e0:
	ldr	r0, [pc, #68]
	bl	sub_0200075c
	adds	r1, r5, #0
	bl	sub_02000742
	ldr	r3, [pc, #60]
	adds	r0, r5, #0
	ldr	r1, [pc, #60]
	ldr	r2, [pc, #64]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r2, r5, #0
	movs	r1, #160
	adds	r2, #32
	lsls	r1, r1, #3
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	bl	sub_02000766
	movs	r2, #128
	ldr	r1, [pc, #32]
	lsls	r2, r2, #24
.L_0200020e:
	ldr	r3, [r1, #8]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0200020e
	adds	r0, r5, #0
	bl	sub_0200076a
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x02008650
	.4byte 0x0000001c
	.4byte 0x040000d4
	.4byte 0x050003e0
	.2byte 0x0008
	.2byte 0x8400
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #148]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	ldr	r2, [pc, #144]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r4, [pc, #144]
	ldrh	r3, [r3, #2]
	movs	r2, #136
	sub	sp, #4
	adds	r6, r4, #0
	lsrs	r7, r3, #5
	movs	r5, #0
	mov	r8, r2
.L_0200025a:
	movs	r2, #18
	subs	r2, r2, r5
	lsls	r2, r2, #3
	movs	r3, #232
	subs	r3, r3, r2
	movs	r2, #0
	stmia	r6!, {r2}
	mov	r1, r8
	lsls	r3, r3, #16
	movs	r2, #132
	orrs	r3, r1
	lsls	r2, r2, #8
	orrs	r3, r2
	stmia	r6!, {r3}
	movs	r3, #240
	lsls	r3, r3, #8
	orrs	r3, r7
	stmia	r6!, {r3}
	ldr	r3, [pc, #96]
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	subs	r1, r2, r5
	cmp	r1, #0
	bge.n	.L_02000294
	movs	r1, #0
.L_02000294:
	cmp	r1, #2
	bgt.n	.L_020002a6
	ldr	r3, [pc, #72]
	ldr	r3, [r3, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020002a6
	movs	r1, #0
.L_020002a6:
	cmp	r1, #0
	beq.n	.L_020002b8
	adds	r0, r4, #0
	movs	r1, #255
	adds	r4, #12
	str	r4, [sp, #0]
	bl	sub_02000824
	ldr	r4, [sp, #0]
.L_020002b8:
	adds	r5, #1
	adds	r7, #2
	cmp	r5, #17
	ble.n	.L_0200025a
	ldr	r2, [pc, #28]
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02008650
	.4byte 0x03001b10
	.4byte 0x020086a0
	.4byte 0x0200868c
	.2byte 0x1e40
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	bl	sub_02000740
	movs	r0, #30
	bl	sub_0200089a
	ldr	r2, [pc, #40]
	ldr	r3, [pc, #36]
	movs	r0, #0
	strh	r3, [r2, #0]
	bl	sub_020004be
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #28]
	bl	sub_02000848
	ldr	r7, [pc, #28]
	ldr	r5, [pc, #28]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r7, #0]
	cmp	r2, #31
	bgt.n	.L_02000350
	b.n	.L_02000330
	.4byte 0x00000000
	.4byte 0x0200868c
	.4byte 0x02008239
	.4byte 0x02002090
	.2byte 0x0208
	.2byte 0x0400
.L_02000330:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r7
	strh	r2, [r7, #0]
	movs	r2, #170
	adds	r3, #4
	lsls	r2, r2, #5
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_02000350:
	strh	r1, [r5, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r7, #0]
	cmp	r2, #31
	bgt.n	.L_0200037a
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r7
	strh	r2, [r7, #0]
.L_0200036a:
	ldr	r2, [pc, #208]
	adds	r3, #4
	stmia	r3!, {r2}
	ldr	r2, [pc, #204]
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0200037a:
	strh	r1, [r5, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r7, #0]
	cmp	r2, #31
	bgt.n	.L_020003a4
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r7
	adds	r3, #4
	strh	r2, [r7, #0]
	movs	r2, #16
	stmia	r3!, {r2}
	ldr	r2, [pc, #168]
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_020003a4:
	strh	r1, [r5, #0]
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r2, [r7, #0]
	cmp	r2, #31
	bgt.n	.L_020003ce
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r7
	strh	r2, [r7, #0]
	ldr	r2, [pc, #136]
	adds	r3, #4
	stmia	r3!, {r2}
	ldr	r2, [pc, #132]
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_020003ce:
	strh	r1, [r5, #0]
	movs	r0, #120
	bl	sub_0200097c
	movs	r6, #0
.L_020003d8:
	ldrh	r3, [r5, #0]
	adds	r1, r3, #0
	strh	r5, [r5, #0]
	ldrh	r3, [r7, #0]
	cmp	r3, #31
	bgt.n	.L_02000402
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r2, r2, #2
	adds	r3, #1
	adds	r2, r7, r2
	strh	r3, [r7, #0]
	movs	r3, #16
	adds	r2, #4
	subs	r3, r3, r6
	stmia	r2!, {r3}
	ldr	r3, [pc, #72]
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_02000402:
	strh	r1, [r5, #0]
	movs	r0, #3
	adds	r6, #1
	bl	sub_02000942
	cmp	r6, #16
	ble.n	.L_020003d8
	ldr	r6, [pc, #60]
	movs	r3, #224
	ldr	r1, [r6, #0]
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r5, #228
	movs	r3, #0
	str	r3, [r2, #0]
	lsls	r5, r5, #1
	movs	r3, #1
	str	r3, [r1, r5]
	bl	sub_020009e8
	bl	sub_020009f4
	ldr	r2, [r6, #0]
	movs	r3, #60
	str	r3, [r2, r5]
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x00002fce
	.4byte 0x04000050
	.4byte 0x04000054
	.4byte 0x00001010
	.4byte 0x04000052
	.2byte 0x1ebc
	.2byte 0x0300
	push	{r5, r6, lr}
	movs	r0, #0
	ldr	r5, [pc, #64]
	bl	sub_020009e4
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #52]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #60]
	movs	r3, #0
	strh	r3, [r2, #10]
	adds	r0, r5, #0
	bl	sub_020009e6
	movs	r1, #160
	ldr	r6, [pc, #52]
	adds	r4, r0, #0
	ldr	r3, [pc, #52]
	lsls	r1, r1, #19
	ldr	r2, [pc, #52]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #224
	lsls	r3, r3, #1
	adds	r4, r4, r3
	adds	r0, r4, #0
	ldr	r1, [pc, #40]
	bl	sub_020009e4
	ldr	r3, [pc, #28]
	ldr	r0, [pc, #32]
	ldr	r1, [pc, #36]
	ldr	r2, [pc, #36]
	b.n	.L_020004c0
	.4byte 0x00000681
	.4byte 0x0000001a
	.4byte 0x0400000c
	.4byte 0x03001ad0
	.4byte 0x000001ff
	.4byte 0x040000d4
	.4byte 0x84000070
	.4byte 0x02010000
	.4byte 0x06006800
	.2byte 0x2580
	.2byte 0x8400
.L_020004c0:
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #208
	ldr	r1, [pc, #88]
	lsls	r3, r3, #1
	movs	r4, #0
.L_020004cc:
	movs	r0, #0
.L_020004ce:
	adds	r2, r3, #0
	movs	r5, #128
	lsls	r3, r2, #16
	lsls	r5, r5, #9
	adds	r3, r3, r5
	adds	r0, #1
	strh	r2, [r1, #0]
	asrs	r3, r3, #16
	adds	r1, #2
	cmp	r0, #29
	bls.n	.L_020004ce
	strh	r6, [r1, #0]
	adds	r4, #1
	adds	r1, #2
	strh	r6, [r1, #0]
	adds	r1, #2
	cmp	r4, #19
	bls.n	.L_020004cc
	ldr	r3, [pc, #48]
	movs	r4, #0
	movs	r2, #0
.L_020004f8:
	adds	r4, #1
	strh	r2, [r3, #2]
	strh	r2, [r3, #0]
	adds	r3, #4
	cmp	r4, #3
	bls.n	.L_020004f8
	ldr	r3, [pc, #32]
	ldr	r0, [pc, #28]
	ldr	r1, [pc, #32]
	ldr	r2, [pc, #36]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #32]
	ldr	r2, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #5
	strh	r3, [r2, #20]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x06003000
	.4byte 0x03001ad0
	.4byte 0x040000d4
	.4byte 0x04000010
	.4byte 0x84000004
	.4byte 0x03001e70
	.4byte 0x47204c00
	.4byte 0x080000c1
	.4byte 0x47204c00
	.4byte 0x080000d1
	.4byte 0x47204c00
	.4byte 0x08000171
	.4byte 0x47204c00
	.4byte 0x08000179
	.4byte 0x47204c00
	.4byte 0x080001a9
	.4byte 0x47204c00
	.4byte 0x080001c9
	.4byte 0x47204c00
	.4byte 0x080001d1
	.4byte 0x47204c00
	.4byte 0x080001e9
	.4byte 0x47204c00
	.4byte 0x08000291
	.4byte 0x47204c00
	.4byte 0x08000299
	.4byte 0x47204c00
	.4byte 0x080002a9
	.4byte 0x47204c00
	.4byte 0x080002d1
	.4byte 0x47204c00
	.4byte 0x08015319
	.4byte 0x47204c00
	.4byte 0x08077279
	.4byte 0x47204c00
	.4byte 0x0808a011
	.4byte 0x47204c00
	.4byte 0x0808a081
	.4byte 0x47204c00
	.4byte 0x0808a239
	.4byte 0x47204c00
	.4byte 0x0808a361
	.4byte 0x47204c00
	.4byte 0x0808a371
	.4byte 0x47204c00
	.4byte 0x080f0001
	.4byte 0x47204c00
	.4byte 0x080f2001
	.4byte 0x47204c00
	.4byte 0x080f2019
	.4byte 0x47204c00
	.4byte 0x080f2021
	.4byte 0x47204c00
	.4byte 0x080f9011
	.4byte 0xffff0000
	.4byte 0x00000000
	.4byte 0x40000000
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.2byte 0xffff

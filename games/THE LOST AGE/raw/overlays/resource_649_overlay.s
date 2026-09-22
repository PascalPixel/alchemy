.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_02000604, 0x02000604
	.set sub_0200061a, 0x0200061a
	.set sub_02000674, 0x02000674
	.set sub_02000698, 0x02000698
	.set sub_020006a4, 0x020006a4
	.set sub_020006b0, 0x020006b0
	.set sub_020006ba, 0x020006ba
	.set sub_020006c0, 0x020006c0
	.set sub_020006c8, 0x020006c8
	.set sub_020006d2, 0x020006d2
	.set sub_020006e4, 0x020006e4
	.set sub_020006e6, 0x020006e6
	.set sub_020006f0, 0x020006f0
	.set sub_020006fc, 0x020006fc
	.set sub_02000706, 0x02000706
	.set sub_02000708, 0x02000708
	.set sub_02000712, 0x02000712
	.set sub_02000728, 0x02000728
	.set sub_02000734, 0x02000734
	.set sub_02000742, 0x02000742
	.set sub_02000746, 0x02000746
	.set sub_0200074c, 0x0200074c
	.set sub_0200075e, 0x0200075e
	.set sub_02000760, 0x02000760
	.set sub_02000762, 0x02000762
	.set sub_0200076c, 0x0200076c
	.set sub_02000776, 0x02000776
	.set sub_02000794, 0x02000794
	.set sub_020007a0, 0x020007a0
	.set sub_020007a8, 0x020007a8
	.set sub_020007ac, 0x020007ac
	.set sub_020007ae, 0x020007ae
	.set sub_020007b0, 0x020007b0
	.set sub_020007c6, 0x020007c6
	.set sub_020007c8, 0x020007c8
	.set sub_020007ce, 0x020007ce
	.set sub_020007e0, 0x020007e0
	.set sub_020007e2, 0x020007e2
	.set sub_020007f2, 0x020007f2
	.set sub_020007f4, 0x020007f4
	.set sub_0200080e, 0x0200080e
	.set sub_02000824, 0x02000824
	.set sub_0200089a, 0x0200089a
	.set sub_020008b8, 0x020008b8
	.set sub_0200091a, 0x0200091a
	.set sub_020009ce, 0x020009ce
	.set sub_02000a10, 0x02000a10
	.set sub_02000a5c, 0x02000a5c
	.set sub_02000a62, 0x02000a62
	.set sub_02000a7a, 0x02000a7a
	.set sub_02000a86, 0x02000a86
	.set sub_02000a92, 0x02000a92
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x0200805d, 0x02008039, 0x02008045, 0x0200804d, 0x02008055, 0x02008041, 0x02008205
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x864c
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x867c
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8680
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8698
	.2byte 0x0200
	push	{r5, r6, lr}
	ldr	r5, [pc, #392]
	movs	r2, #241
	lsls	r2, r2, #1
	adds	r6, r5, r2
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #10
	bne.n	.L_020000ba
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	bl	sub_02000674
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r0, #75
	bl	sub_020006c8
	movs	r0, #0
	bl	.L_020003a6
	movs	r0, #120
	bl	sub_02000604
	movs	r5, #0
	b.n	.L_02000098
.L_02000096:
	adds	r5, #1
.L_02000098:
	movs	r3, #209
	lsls	r3, r3, #4
	adds	r3, #255
	cmp	r5, r3
	bgt.n	.L_020000b0
	movs	r0, #1
	bl	sub_0200061a
	ldr	r3, [pc, #320]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq.n	.L_02000096
.L_020000b0:
	ldr	r0, [pc, #316]
	movs	r1, #2
	bl	sub_020006ba
	b.n	.L_020001e2
.L_020000ba:
	cmp	r3, #8
	bne.n	.L_020000f0
	movs	r0, #68
	bl	sub_02000706
	movs	r0, #1
	bl	sub_020006e4
	movs	r0, #78
	bl	sub_02000712
	movs	r0, #60
	bl	sub_02000698
	bl	sub_020006a4
	movs	r0, #240
	bl	sub_020006d2
	movs	r0, #0
	bl	sub_02000728
	ldr	r0, [pc, #264]
	movs	r1, #2
	bl	sub_020006f0
	b.n	.L_020001e2
.L_020000f0:
	ldr	r0, [pc, #256]
	bl	sub_020006b0
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #1
	bne.n	.L_02000162
.L_020000fe:
	movs	r0, #0
	bl	sub_02000746
	movs	r0, #0
	bl	sub_02000734
	movs	r0, #0
	bl	sub_02000742
	bl	sub_020006e6
	cmp	r0, #0
	ble.n	.L_02000158
	movs	r0, #40
	bl	sub_02000760
	movs	r0, #1
	bl	sub_0200075e
	movs	r0, #1
	bl	sub_0200074c
	cmp	r0, #0
	bne.n	.L_02000158
	movs	r0, #78
	bl	sub_02000776
	movs	r0, #30
	bl	sub_020006fc
	bl	sub_02000708
	movs	r5, #0
	b.n	.L_02000144
.L_02000142:
	adds	r5, #1
.L_02000144:
	cmp	r5, #119
	bgt.n	.L_020000fe
	movs	r0, #1
	bl	sub_020006c0
	ldr	r3, [pc, #156]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq.n	.L_02000142
	b.n	.L_020000fe
.L_02000158:
	ldr	r0, [pc, #156]
	movs	r1, #1
	bl	sub_02000762
	b.n	.L_020001c6
.L_02000162:
	cmp	r3, #2
	bne.n	.L_02000192
	movs	r0, #40
	bl	sub_020007ae
	movs	r0, #0
	bl	sub_020007ac
	movs	r2, #147
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r5, r2
	adds	r2, #1
	ldrb	r0, [r3, #0]
	adds	r3, r5, r2
	ldrb	r1, [r3, #0]
	bl	sub_02000760
	bl	sub_0200076c
	ldr	r0, [pc, #112]
	movs	r1, #99
	bl	sub_02000794
.L_02000192:
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #3
	bne.n	.L_020001c6
	movs	r0, #75
	bl	sub_020007e2
	movs	r0, #0
	bl	sub_020007c8
	movs	r2, #147
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r5, r2
	adds	r2, #1
	ldrb	r0, [r3, #0]
	adds	r3, r5, r2
	ldrb	r1, [r3, #0]
	bl	sub_02000794
	bl	sub_020007a8
	ldr	r0, [pc, #64]
	movs	r1, #13
	bl	sub_020007c8
.L_020001c6:
	movs	r0, #78
	bl	sub_0200080e
	movs	r0, #30
	bl	sub_02000794
	bl	sub_020007a0
	movs	r0, #60
	bl	sub_020007ce
	movs	r0, #0
	bl	sub_02000824
.L_020001e2:
	movs	r0, #0
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x03001150
	.4byte 0x00000000
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0x00000005
	.2byte 0x0009
	.2byte 0x0000
	movs	r0, #0
	bx	lr
	push	{r5, r6, lr}
	movs	r0, #164
	lsls	r0, r0, #3
	bl	sub_02000794
	ldr	r6, [pc, #88]
	adds	r5, r0, #0
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_02000228
	bl	sub_020007c8
	strh	r0, [r6, #0]
.L_02000228:
	ldr	r0, [pc, #68]
	bl	sub_020007e0
	adds	r1, r5, #0
	bl	sub_020007c6
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r5, #0
	ldr	r1, [pc, #48]
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r2, r5, #0
	movs	r1, #160
	adds	r2, #32
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	lsls	r1, r1, #3
	bl	sub_020007f2
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #212
.L_0200025e:
	ldr	r3, [r2, #8]
	cmp	r3, #0
	blt.n	.L_0200025e
	adds	r0, r5, #0
	bl	sub_020007f4
	pop	{r5, r6, pc}
	.4byte 0x020086a4
	.4byte 0x00000025
	.2byte 0x03e0
	.2byte 0x0500
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #140]
	ldr	r2, [pc, #140]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	ldr	r4, [pc, #140]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	sub	sp, #4
	lsrs	r5, r3, #5
	movs	r7, #0
	adds	r6, r4, #0
.L_02000292:
	movs	r2, #18
	subs	r2, r2, r7
	lsls	r2, r2, #3
	movs	r3, #232
	subs	r3, r3, r2
	movs	r2, #0
	movs	r1, #136
	str	r2, [r6, #0]
	lsls	r3, r3, #16
	movs	r2, #132
	orrs	r3, r1
	lsls	r2, r2, #8
	orrs	r3, r2
	str	r3, [r6, #4]
	movs	r3, #240
	lsls	r3, r3, #8
	orrs	r3, r5
	str	r3, [r6, #8]
	ldr	r3, [pc, #92]
	adds	r6, #12
	ldrh	r3, [r3, #0]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	subs	r1, r2, r7
	cmp	r1, #0
	bge.n	.L_020002ce
	movs	r1, #0
.L_020002ce:
	cmp	r1, #2
	bgt.n	.L_020002e0
	ldr	r3, [pc, #68]
	movs	r2, #1
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020002e0
	movs	r1, #0
.L_020002e0:
	cmp	r1, #0
	beq.n	.L_020002f2
	adds	r0, r4, #0
	movs	r1, #255
	adds	r4, #12
	str	r4, [sp, #0]
	bl	sub_0200089a
	ldr	r4, [sp, #0]
.L_020002f2:
	adds	r7, #1
	adds	r5, #2
	cmp	r7, #17
	ble.n	.L_02000292
	ldr	r2, [pc, #24]
	add	sp, #4
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x020086a4
	.4byte 0x020036e0
	.4byte 0x020086f0
	.4byte 0x020086dc
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, lr}
	bl	sub_020007b0
	movs	r0, #30
	bl	sub_0200091a
	ldr	r2, [pc, #40]
	ldr	r3, [pc, #36]
	movs	r0, #0
	strh	r3, [r2, #0]
	bl	.L_0200053a
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #28]
	bl	sub_020008b8
	ldr	r0, [pc, #28]
	ldr	r1, [pc, #28]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_02000384
	b.n	.L_02000364
	.4byte 0x00000000
	.4byte 0x020086dc
	.4byte 0x02008279
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
.L_02000364:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r0
	strh	r2, [r0, #0]
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
.L_02000384:
	strh	r4, [r1, #0]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_020003b6
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r0, #0]
	movs	r2, #188
	adds	r3, r3, r0
	lsls	r2, r2, #6
	adds	r3, #4
	adds	r2, #206
.L_020003a6:
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_020003b6:
	strh	r4, [r1, #0]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_020003e4
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r0
	adds	r3, #4
	strh	r2, [r0, #0]
	movs	r2, #16
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #84
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_020003e4:
	strh	r4, [r1, #0]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_02000416
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r0, #0]
	movs	r2, #128
	adds	r3, r3, r0
	lsls	r2, r2, #5
	adds	r3, #4
	adds	r2, #16
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_02000416:
	strh	r4, [r1, #0]
	movs	r0, #120
	bl	sub_02000a10
	movs	r5, #0
.L_02000420:
	ldr	r1, [pc, #100]
	ldr	r0, [pc, #104]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r3, [r1, #0]
	cmp	r3, #31
	bgt.n	.L_02000452
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r2, r2, #2
	adds	r3, #1
	adds	r2, r2, r1
	strh	r3, [r1, #0]
	movs	r3, #16
	adds	r2, #4
	subs	r3, r3, r5
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #84
	stmia	r2!, {r3}
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_02000452:
	strh	r4, [r0, #0]
	movs	r0, #3
	adds	r5, #1
	bl	sub_020009ce
	cmp	r5, #16
	ble.n	.L_02000420
	movs	r6, #192
	lsls	r6, r6, #18
	ldr	r1, [r6, #108]
	movs	r3, #214
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r5, #218
	movs	r3, #0
	str	r3, [r2, #0]
	lsls	r5, r5, #1
	movs	r3, #1
	str	r3, [r1, r5]
	bl	sub_02000a86
	bl	sub_02000a92
	ldr	r2, [r6, #108]
	movs	r3, #60
	str	r3, [r2, r5]
	pop	{r5, r6, pc}
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
	push	{r5, r6, lr}
	movs	r0, #0
	ldr	r5, [pc, #64]
	bl	sub_02000a5c
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r2, [pc, #52]
	movs	r3, #0
	strh	r3, [r2, #10]
	adds	r0, r5, #0
	bl	sub_02000a62
	movs	r6, #128
	movs	r3, #128
	movs	r2, #132
	lsls	r6, r6, #1
	lsls	r3, r3, #19
	movs	r1, #160
	lsls	r2, r2, #24
	adds	r6, #255
	adds	r4, r0, #0
	adds	r3, #212
	lsls	r1, r1, #19
	adds	r2, #112
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #224
	lsls	r3, r3, #1
	adds	r4, r4, r3
	b.n	.L_020004e0
	.4byte 0x00000681
	.4byte 0x00000022
	.2byte 0x1120
	.2byte 0x0300
.L_020004e0:
	adds	r0, r4, #0
	ldr	r1, [pc, #124]
	bl	sub_02000a7a
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #112]
	ldr	r1, [pc, #112]
	ldr	r2, [pc, #116]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r1, [pc, #112]
	movs	r3, #208
	lsls	r3, r3, #1
	movs	r0, #0
.L_02000500:
	movs	r4, #0
.L_02000502:
	adds	r2, r3, #0
	movs	r5, #128
	lsls	r3, r2, #16
	lsls	r5, r5, #9
	adds	r3, r3, r5
	adds	r4, #1
	strh	r2, [r1, #0]
	asrs	r3, r3, #16
	adds	r1, #2
	cmp	r4, #29
	bls.n	.L_02000502
	strh	r6, [r1, #0]
	adds	r0, #1
	adds	r1, #2
	strh	r6, [r1, #0]
	adds	r1, #2
	cmp	r0, #19
	bls.n	.L_02000500
	ldr	r2, [pc, #72]
	movs	r0, #0
.L_0200052a:
	movs	r3, #0
	adds	r0, #1
	strh	r3, [r2, #2]
	strh	r3, [r2, #0]
	adds	r2, #4
	cmp	r0, #3
	bls.n	.L_0200052a
	movs	r3, #128
.L_0200053a:
	movs	r1, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r1, r1, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #40]
	adds	r1, #16
	adds	r2, #4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #32]
	movs	r3, #160
	lsls	r3, r3, #5
	strh	r3, [r2, #20]
	pop	{r5, r6, pc}
	.2byte 0x0000
	.4byte 0x02010000
	.4byte 0x06006800
	.4byte 0x84002580
	.4byte 0x06003000
	.4byte 0x03001120
	.irp EntryTarget, 0x080000c1, 0x080000d1, 0x08000171, 0x08000179, 0x080001a9, 0x080001c9, 0x080001d1, 0x080001e9, 0x08000291, 0x08000299, 0x080002a9, 0x080002d1, 0x080382f1, 0x08038349, 0x080ad219, 0x080ad221, 0x080c8011, 0x080c8089, 0x080c8269, 0x080c83a9, 0x080c83b9, 0x081a0001, 0x081a6001, 0x081a6019, 0x081a6021, 0x081a6029, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
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

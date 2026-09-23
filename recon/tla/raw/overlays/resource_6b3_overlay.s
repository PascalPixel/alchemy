.syntax unified
.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_020005f2, 0x020005f2
	.set sub_020005fc, 0x020005fc
	.set sub_02000674, 0x02000674
	.set sub_0200067e, 0x0200067e
	.set sub_02000688, 0x02000688
	.set sub_020006e0, 0x020006e0
	.set sub_02000708, 0x02000708
	.set sub_02000726, 0x02000726
	.set sub_0200076c, 0x0200076c
	.set sub_02000780, 0x02000780
	.set sub_02000798, 0x02000798
	.set sub_020007a2, 0x020007a2
	.set sub_020007b0, 0x020007b0
	.set sub_020007ba, 0x020007ba
	.set sub_020007e0, 0x020007e0
	.set sub_020007ea, 0x020007ea
	.set sub_020007f4, 0x020007f4
	.set sub_020007fe, 0x020007fe
	.set sub_02000808, 0x02000808
	.set sub_02000812, 0x02000812
	.set sub_0200081c, 0x0200081c
	.set sub_02000826, 0x02000826
	.set sub_02000830, 0x02000830
	.set sub_02000842, 0x02000842
	.set sub_02000850, 0x02000850
	.set sub_0200085a, 0x0200085a
	.set sub_02000864, 0x02000864
	.set sub_0200086e, 0x0200086e
	.set sub_02000876, 0x02000876
	.set sub_02000878, 0x02000878
	.set sub_0200088c, 0x0200088c
	.set sub_02000892, 0x02000892
	.set sub_02000898, 0x02000898
	.set sub_020008ba, 0x020008ba
	.set sub_020008be, 0x020008be
	.set sub_02000924, 0x02000924
	.set sub_0200096c, 0x0200096c
	.set sub_0200097a, 0x0200097a
	.set sub_0200098e, 0x0200098e
	.set sub_020009a6, 0x020009a6
	.set sub_020009b2, 0x020009b2
	.set sub_020009e0, 0x020009e0
	.set sub_020009e2, 0x020009e2
	.set sub_020009f0, 0x020009f0
	.set sub_02000a82, 0x02000a82
	.set sub_02000b2a, 0x02000b2a
	.set sub_02000b4c, 0x02000b4c
	.set sub_02000b7c, 0x02000b7c
	.set sub_02000b86, 0x02000b86
	.set sub_02000bfa, 0x02000bfa
	.set sub_02000c3a, 0x02000c3a
	.set sub_02000c6c, 0x02000c6c
	.set sub_02000c7a, 0x02000c7a
	.set sub_02000c8c, 0x02000c8c
	.set sub_02000c9c, 0x02000c9c
	.set sub_02000cbc, 0x02000cbc
	.set sub_02000cce, 0x02000cce
	.set sub_02000d84, 0x02000d84
	.set sub_02000d9a, 0x02000d9a
	.set sub_02000da6, 0x02000da6
	.set sub_02000dcc, 0x02000dcc
	.set sub_02000df8, 0x02000df8
	.set sub_02000e16, 0x02000e16
	.set sub_02000e1c, 0x02000e1c
	.set sub_02000e40, 0x02000e40
	.set sub_02000e46, 0x02000e46
	.set sub_02000e58, 0x02000e58
	.set sub_02000e76, 0x02000e76
	.set sub_02000e8a, 0x02000e8a
	.set sub_02000e94, 0x02000e94
	.set sub_02000ea8, 0x02000ea8
	.set sub_02000eb6, 0x02000eb6
	.set sub_02000ebc, 0x02000ebc
	.set sub_02000ed2, 0x02000ed2
	.set sub_02000ee4, 0x02000ee4
	.set sub_02000eee, 0x02000eee
	.set sub_02000ef4, 0x02000ef4
	.set sub_02000f02, 0x02000f02
	.set sub_02000f0a, 0x02000f0a
	.set sub_02000f0c, 0x02000f0c
	.set sub_02000f0e, 0x02000f0e
	.set sub_02000f14, 0x02000f14
	.set sub_02000f26, 0x02000f26
	.set sub_02000f30, 0x02000f30
	.set sub_02000f40, 0x02000f40
	.set sub_02000f52, 0x02000f52
	.set sub_02000f72, 0x02000f72
	.set sub_02000f80, 0x02000f80
	.global Overlay_02000000
Overlay_02000000:
	.irp EntryTarget, 0x02008319, 0x02008039, 0x02008045, 0x0200804d, 0x02008055, 0x02008041, 0x02008789
	overlay_veneer \EntryTarget
	.endr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x8894
	.2byte 0x0200
	movs	r0, #0
	bx	lr
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x88c4
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x88c8
	.2byte 0x0200
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x88e0
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #40]
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_02000084
	ldr	r3, [pc, #32]
	ldr	r0, [pc, #32]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r3, r3, #16
	str	r3, [r0, #12]
	ldr	r3, [pc, #28]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r3, r3, #16
	str	r3, [r0, #16]
	bl	sub_020008be
.L_02000084:
	pop	{pc}
	.2byte 0x0000
	.4byte 0x02008a34
	.4byte 0x02008a38
	.4byte 0x02008a00
	.2byte 0x8a2c
	.2byte 0x0200
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	movs	r0, #128
	lsls	r0, r0, #4
	sub	sp, #4
	bl	sub_02000842
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	mov	r8, r0
	adds	r3, #212
	ldr	r0, [pc, #120]
	ldr	r1, [pc, #124]
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #120]
	ldr	r6, [pc, #120]
	bl	sub_02000898
	mov	r1, r8
	bl	sub_02000876
	ldr	r5, [pc, #112]
	bl	sub_0200088c
	movs	r1, #128
	mov	r2, r8
	str	r0, [r5, #0]
	lsls	r1, r1, #4
	ldr	r5, [pc, #104]
	bl	sub_02000892
	movs	r3, #192
	str	r0, [r5, #0]
	movs	r1, #0
	str	r0, [sp, #0]
	movs	r2, #0
	adds	r0, r6, #0
	lsls	r3, r3, #24
	bl	sub_02000924
	movs	r3, #240
	strh	r3, [r6, #30]
	ldrb	r3, [r6, #9]
	movs	r2, #13
	ldrb	r1, [r6, #5]
	negs	r2, r2
	ands	r2, r3
	movs	r3, #33
	negs	r3, r3
	ands	r3, r1
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r2, r3
	movs	r3, #224
	orrs	r2, r3
	strb	r2, [r6, #9]
	mov	r0, r8
	bl	sub_020008ba
	ldr	r5, [pc, #16]
	ldr	r3, [pc, #44]
	movs	r1, #144
	strb	r5, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl	sub_020008ba
	add	sp, #4
	b.n	.L_02000150
	.4byte 0x00000000
	.4byte 0x02008874
	.4byte 0x050003c0
	.4byte 0x000001f8
	.4byte 0x02008a00
	.4byte 0x020088f0
	.4byte 0x02008a30
	.4byte 0x02008a34
	.2byte 0x805d
	.2byte 0x0200
.L_02000150:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r6, r2, #0
	movs	r2, #128
	lsls	r2, r2, #8
	mov	r8, r2
	adds	r7, r0, #0
	mov	sl, r3
	mov	r3, r8
	ands	r3, r7
	mov	r8, r3
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	ands	r7, r3
	adds	r5, r1, #0
	adds	r0, r7, #0
	movs	r1, #0
	mov	r9, r1
	bl	sub_020009b2
	adds	r3, r0, #0
	movs	r2, #4
	lsls	r3, r3, #16
	orrs	r3, r2
	ldr	r2, [pc, #160]
	adds	r1, r5, #0
	ldr	r0, [r2, #0]
	adds	r2, r6, #0
	bl	sub_0200098e
	mov	r1, r8
	adds	r5, r0, #0
	cmp	r1, #0
	bne.n	.L_020001c8
	movs	r1, #0
	mov	r2, sl
	ldr	r3, [sp, #28]
	adds	r0, r7, #0
	bl	sub_020009b2
	ldr	r2, [pc, #132]
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r2, [pc, #132]
	mov	r1, sl
	lsls	r3, r1, #3
	strh	r3, [r2, #0]
	ldr	r1, [sp, #28]
	ldr	r2, [pc, #124]
	lsls	r3, r1, #3
	strh	r3, [r2, #0]
	mov	r9, r0
.L_020001c8:
	ldr	r2, [sp, #36]
	cmp	r2, #2
	bne.n	.L_020001e2
	ldrh	r3, [r5, #14]
	movs	r2, #0
	adds	r3, #1
	strh	r3, [r5, #14]
	strh	r2, [r5, #8]
	strh	r2, [r5, #10]
	b.n	.L_020001e2
.L_020001dc:
	movs	r0, #1
	bl	sub_0200096c
.L_020001e2:
	bl	sub_020009e0
	cmp	r0, #0
	beq.n	.L_020001dc
	movs	r0, #1
	bl	sub_0200097a
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_020009e2
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_0200020c
	movs	r1, #2
	mov	r0, r9
	bl	sub_020009f0
	ldr	r3, [pc, #48]
	mov	r1, r8
	strb	r1, [r3, #0]
.L_0200020c:
	ldr	r2, [sp, #32]
	cmp	r2, #0
	ble.n	.L_02000220
	adds	r5, r2, #0
.L_02000214:
	movs	r0, #1
	subs	r5, #1
	bl	sub_020009a6
	cmp	r5, #0
	bne.n	.L_02000214
.L_02000220:
	ldr	r2, [pc, #16]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02008a28
	.4byte 0x02008a34
	.4byte 0x02008a38
	.2byte 0x8a2c
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #28
	movs	r4, #0
	movs	r3, #0
	str	r4, [sp, #24]
	str	r4, [sp, #20]
	mov	r9, r3
	ldr	r3, [pc, #184]
	movs	r5, #128
	lsls	r5, r5, #8
	add	r4, sp, #12
	mov	r8, r0
	adds	r6, r1, #0
	mov	sl, r2
	ands	r5, r0
	add	r2, sp, #20
	ldr	r0, [r3, #0]
	add	r1, sp, #24
	add	r3, sp, #16
	str	r4, [sp, #0]
	bl	sub_02000a82
	ldr	r2, [sp, #16]
	movs	r7, #0
	cmp	r2, #24
	bgt.n	.L_020002a4
	cmp	r5, #0
	bne.n	.L_02000292
	movs	r3, #25
	subs	r3, r3, r2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r3, #5
	b.n	.L_0200029c
.L_02000292:
	movs	r3, #30
	subs	r3, r3, r2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
.L_0200029c:
	str	r3, [sp, #24]
	ldr	r3, [sp, #24]
	subs	r4, r3, #5
	b.n	.L_020002b2
.L_020002a4:
	movs	r3, #30
	subs	r3, r3, r2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [sp, #24]
	adds	r4, r3, #0
.L_020002b2:
	ldr	r3, [sp, #12]
	subs	r3, #1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r2, r3, #1
	cmp	r6, #0
	bne.n	.L_020002c8
	movs	r3, #3
	str	r3, [sp, #20]
	adds	r7, r2, #1
	b.n	.L_020002f2
.L_020002c8:
	cmp	r6, #1
	bne.n	.L_020002d8
	movs	r3, #14
	subs	r3, r3, r2
	str	r3, [sp, #20]
	adds	r3, r3, r2
	subs	r7, r3, #2
	b.n	.L_020002f2
.L_020002d8:
	cmp	r6, #2
	bne.n	.L_020002f2
	cmp	r2, #3
	bne.n	.L_020002ec
	movs	r3, #13
	str	r3, [sp, #20]
	movs	r3, #2
	movs	r7, #15
	mov	r9, r3
	b.n	.L_020002f2
.L_020002ec:
	movs	r3, #16
	str	r3, [sp, #20]
	movs	r7, #15
.L_020002f2:
	mov	r3, sl
	str	r3, [sp, #4]
	mov	r3, r9
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #20]
	str	r3, [sp, #8]
	mov	r0, r8
	adds	r3, r4, #0
	str	r7, [sp, #0]
	bl	.L_0200045e
	add	sp, #28
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x8a28
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #840]
	movs	r2, #139
	lsls	r2, r2, #2
	adds	r6, r3, r2
	subs	r2, #24
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	ldrb	r7, [r6, #0]
	bl	sub_02000b4c
	movs	r3, #0
	mov	r8, r3
	mov	r2, r8
	movs	r5, #1
	adds	r0, #85
	strb	r2, [r0, #0]
	strb	r5, [r6, #0]
	bl	sub_02000b86
	bl	sub_02000b2a
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #140
	strb	r5, [r3, #6]
	adds	r3, r3, r2
	strb	r5, [r3, #0]
	ldr	r2, [pc, #784]
	ldr	r3, [pc, #784]
	str	r3, [r2, #0]
	bl	.L_020003fa
	movs	r0, #30
	bl	sub_02000b7c
	movs	r0, #5
	movs	r1, #0
	movs	r2, #40
	bl	.L_020005b6
	movs	r0, #1
	movs	r1, #1
	movs	r2, #40
	bl	.L_020005c0
	movs	r0, #6
	movs	r1, #0
	movs	r2, #40
	bl	.L_020005ca
	movs	r0, #2
	movs	r1, #1
	movs	r2, #40
	bl	.L_020005d4
	movs	r0, #3
	movs	r1, #0
	movs	r2, #30
	bl	.L_020005de
	movs	r0, #7
	movs	r1, #1
	movs	r2, #30
	bl	.L_020005e8
	movs	r0, #56
	movs	r1, #0
	movs	r2, #40
	bl	sub_020005f2
	movs	r0, #60
	movs	r1, #1
	movs	r2, #30
	bl	sub_020005fc
	movs	r0, #61
	movs	r1, #1
	movs	r2, #40
	bl	.L_02000606
	movs	r0, #0
	movs	r1, #0
	movs	r2, #40
	bl	.L_02000610
	movs	r0, #62
	movs	r1, #1
	movs	r2, #30
	bl	.L_0200061a
	movs	r0, #0
	movs	r1, #0
	movs	r2, #30
	bl	.L_02000624
	movs	r0, #0
	movs	r1, #0
	movs	r2, #30
	bl	.L_0200062e
	movs	r0, #5
	movs	r1, #1
	movs	r2, #40
	bl	.L_02000638
	movs	r0, #4
	movs	r1, #0
.L_020003fa:
	movs	r2, #30
	bl	.L_02000642
	movs	r1, #1
	movs	r2, #30
	movs	r0, #1
	bl	.L_0200064c
	movs	r0, #67
	bl	sub_02000c7a
	ldr	r5, [pc, #608]
	movs	r3, #16
	strh	r3, [r5, #14]
	strh	r3, [r5, #10]
	ldr	r1, [pc, #604]
	movs	r2, #0
	ldr	r0, [pc, #604]
	bl	sub_02000c6c
	movs	r0, #100
	bl	sub_02000c3a
	movs	r0, #60
	movs	r1, #2
	movs	r2, #30
	bl	sub_02000674
	movs	r0, #61
	movs	r1, #2
	movs	r2, #40
	bl	sub_0200067e
	movs	r0, #0
	movs	r1, #2
	movs	r2, #30
	bl	sub_02000688
	movs	r0, #62
	movs	r1, #2
	movs	r2, #30
	bl	.L_02000692
	movs	r0, #1
	movs	r1, #2
	movs	r2, #40
	bl	.L_0200069c
	movs	r1, #2
	movs	r2, #40
.L_0200045e:
	movs	r0, #5
	bl	.L_020006a6
	movs	r0, #120
	bl	sub_02000cbc
	movs	r0, #1
	bl	sub_02000bfa
	bl	sub_02000cce
	movs	r0, #30
	bl	sub_02000c8c
	ldr	r1, [pc, #508]
	movs	r2, #120
	ldr	r0, [pc, #512]
	bl	sub_02000cce
	movs	r0, #120
	bl	sub_02000c9c
	movs	r0, #5
	movs	r1, #2
	movs	r2, #30
	bl	.L_020006d6
	movs	r0, #7
	movs	r1, #2
	movs	r2, #30
	bl	sub_020006e0
	movs	r0, #56
	movs	r1, #2
	movs	r2, #40
	bl	.L_020006ea
	movs	r0, #4
	movs	r1, #2
	movs	r2, #40
	bl	.L_020006f4
	movs	r0, #4
	movs	r1, #2
	movs	r2, #40
	bl	.L_020006fe
	movs	r0, #4
	movs	r1, #2
	movs	r2, #40
	bl	sub_02000708
	movs	r0, #4
	movs	r1, #2
	movs	r2, #40
	bl	.L_02000712
	movs	r0, #4
	movs	r1, #2
	movs	r2, #30
	bl	.L_0200071c
	movs	r0, #0
	movs	r1, #2
	movs	r2, #30
	bl	sub_02000726
	movs	r0, #62
	movs	r1, #2
	movs	r2, #30
	bl	.L_02000730
	movs	r0, #1
	movs	r1, #2
	movs	r2, #40
	bl	.L_0200073a
	movs	r0, #1
	movs	r1, #2
	movs	r2, #40
	bl	.L_02000744
	movs	r0, #6
	movs	r1, #2
	movs	r2, #30
	bl	.L_0200074e
	movs	r0, #5
	movs	r1, #2
	movs	r2, #30
	bl	.L_02000758
	movs	r0, #3
	movs	r1, #2
	movs	r2, #40
	bl	.L_02000762
	movs	r1, #2
	movs	r2, #30
	movs	r0, #1
	bl	sub_0200076c
	movs	r0, #78
	bl	sub_02000d9a
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #64
	movs	r1, #2
	movs	r2, #30
	bl	sub_02000780
	movs	r0, #0
	movs	r1, #2
	movs	r2, #30
	bl	.L_0200078a
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #65
	movs	r1, #2
	movs	r2, #30
	bl	sub_02000798
	movs	r0, #0
	movs	r1, #2
	movs	r2, #30
	bl	sub_020007a2
	movs	r0, #128
	lsls	r0, r0, #8
	adds	r0, #69
	movs	r1, #2
	movs	r2, #30
	bl	sub_020007b0
	movs	r0, #1
	movs	r1, #2
	movs	r2, #30
	bl	sub_020007ba
	ldr	r1, [pc, #252]
	movs	r2, #0
	ldr	r0, [pc, #260]
	bl	sub_02000dcc
	movs	r0, #60
	bl	sub_02000d9a
	movs	r0, #73
	bl	sub_02000df8
	movs	r0, #30
	bl	sub_02000da6
	movs	r0, #64
	movs	r1, #2
	movs	r2, #30
	bl	sub_020007e0
	movs	r0, #65
	movs	r1, #2
	movs	r2, #30
	bl	sub_020007ea
	movs	r0, #67
	movs	r1, #2
	movs	r2, #30
	bl	sub_020007f4
	movs	r0, #62
	movs	r1, #2
.L_020005b6:
	movs	r2, #30
	bl	sub_020007fe
	movs	r0, #70
	movs	r1, #2
.L_020005c0:
	movs	r2, #30
	bl	sub_02000808
	movs	r0, #68
	movs	r1, #2
.L_020005ca:
	movs	r2, #30
	bl	sub_02000812
	movs	r0, #56
	movs	r1, #2
.L_020005d4:
	movs	r2, #30
	bl	sub_0200081c
	movs	r0, #1
	movs	r1, #2
.L_020005de:
	movs	r2, #40
	bl	sub_02000826
	movs	r1, #2
	movs	r2, #40
.L_020005e8:
	movs	r0, #56
	bl	sub_02000830
	movs	r0, #200
	bl	sub_02000e46
	movs	r0, #1
	bl	sub_02000d84
	bl	sub_02000e58
	movs	r0, #30
	bl	sub_02000e16
	movs	r0, #62
.L_02000606:
	movs	r1, #2
	movs	r2, #20
	bl	sub_02000850
	movs	r0, #0
.L_02000610:
	movs	r1, #2
	movs	r2, #40
	bl	sub_0200085a
	movs	r0, #66
.L_0200061a:
	movs	r1, #2
	movs	r2, #20
	bl	sub_02000864
	movs	r0, #62
.L_02000624:
	movs	r1, #2
	movs	r2, #30
	bl	sub_0200086e
	movs	r0, #66
.L_0200062e:
	movs	r1, #2
	movs	r2, #40
	bl	sub_02000878
	ldr	r1, [pc, #64]
.L_02000638:
	movs	r2, #0
	movs	r0, #0
	bl	sub_02000e8a
	movs	r0, #60
.L_02000642:
	bl	sub_02000e58
	mov	r3, r8
	mov	r2, r8
	strh	r3, [r5, #14]
.L_0200064c:
	strh	r2, [r5, #10]
	movs	r0, #0
	bl	sub_02000eb6
	ldr	r0, [pc, #48]
	ldr	r1, [pc, #32]
	movs	r2, #0
	bl	sub_02000ea8
	movs	r0, #60
	bl	sub_02000e76
	b.n	.L_02000692
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x02008a28
	.4byte 0x0000306e
	.4byte 0x03001120
	.4byte 0x02010000
	.4byte 0x00000079
	.4byte 0x0000007a
	.4byte 0x0000007b
	.2byte 0x007d
	.2byte 0x0000
.L_0200068c:
	movs	r0, #1
	bl	sub_02000e1c
.L_02000692:
	ldr	r3, [pc, #212]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	beq.n	.L_0200068c
	ldr	r1, [pc, #208]
.L_0200069c:
	movs	r2, #0
	ldr	r0, [pc, #208]
	bl	sub_02000eee
	movs	r0, #120
.L_020006a6:
	bl	sub_02000ebc
	movs	r0, #120
	bl	sub_02000f02
	movs	r0, #1
	bl	sub_02000e40
	bl	sub_02000f14
	movs	r0, #180
	bl	sub_02000ed2
	ldr	r1, [pc, #168]
	movs	r2, #0
	ldr	r0, [pc, #172]
	bl	sub_02000f14
	movs	r0, #150
	lsls	r0, r0, #1
	bl	sub_02000ee4
	ldr	r1, [pc, #152]
	movs	r2, #0
.L_020006d6:
	ldr	r0, [pc, #160]
	bl	sub_02000f26
	movs	r0, #180
	bl	sub_02000ef4
	movs	r0, #78
	bl	sub_02000f52
	ldr	r3, [pc, #144]
.L_020006ea:
	movs	r2, #139
	lsls	r2, r2, #2
	adds	r3, r3, r2
	movs	r0, #150
	strb	r7, [r3, #0]
.L_020006f4:
	lsls	r0, r0, #1
	bl	sub_02000f0c
	ldr	r3, [pc, #108]
	movs	r5, #0
.L_020006fe:
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_0200071e
.L_02000704:
	movs	r0, #1
	bl	sub_02000e94
	movs	r3, #168
	lsls	r3, r3, #6
	adds	r5, #1
	adds	r3, #47
.L_02000712:
	cmp	r5, r3
	bgt.n	.L_0200071e
	ldr	r3, [pc, #80]
	ldr	r3, [r3, #4]
	cmp	r3, #0
.L_0200071c:
	beq.n	.L_02000704
.L_0200071e:
	ldr	r1, [pc, #76]
	movs	r2, #0
	movs	r0, #0
	bl	sub_02000f72
	movs	r0, #60
	bl	sub_02000f40
	movs	r2, #0
.L_02000730:
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #80
	strh	r2, [r3, #0]
	movs	r2, #130
.L_0200073a:
	lsls	r2, r2, #5
	subs	r3, #80
	strh	r2, [r3, #0]
	bl	sub_02000f0e
.L_02000744:
	bl	sub_02000f0a
	ldr	r2, [pc, #52]
	movs	r3, #1
	movs	r0, #190
.L_0200074e:
	strb	r3, [r2, #0]
	lsls	r0, r0, #1
	bl	sub_02000f30
	ldr	r0, [pc, #44]
.L_02000758:
	movs	r1, #2
	bl	sub_02000f80
	movs	r0, #0
	pop	{r3}
.L_02000762:
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x03001150
	.4byte 0x02010000
	.4byte 0x0000007e
	.4byte 0x0000007f
	.4byte 0x00000080
	.4byte 0x02000240
	.4byte 0x0300120c
	.2byte 0x0001
	.2byte 0x0000
	movs	r0, #0
.L_0200078a:
	bx	lr
	.irp EntryTarget, 0x080000c1, 0x080000d1, 0x08000169, 0x08000179, 0x080001a9, 0x080001c9, 0x080001d1, 0x080001d9, 0x080001f1, 0x08000291, 0x080003d9, 0x08038001, 0x08038019, 0x08038039, 0x08038049, 0x080380f9, 0x08038109, 0x080c8011, 0x080c8089, 0x080c8269, 0x080c85c9, 0x080c87c1, 0x080c87c9, 0x081a0009, 0x081a0011, 0x081a0019, 0x081a0021, 0x081a0029, 0x081c0011
	overlay_veneer \EntryTarget
	.endr
	.4byte 0x7fff44e0
	.4byte 0x0000318c
	.4byte 0x01400180
	.4byte 0x00c00100
	.4byte 0x020001c0
	.4byte 0x294a0240
	.4byte 0x001f5294
	.4byte 0x7c0003ff
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

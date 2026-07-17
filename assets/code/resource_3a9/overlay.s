.syntax unified
	.thumb
	.set sub_020004d0, 0x020004d0
	.set sub_02000558, 0x02000558
	.set sub_0200055e, 0x0200055e
	.set sub_0200057a, 0x0200057a
	.set sub_02000580, 0x02000580
	.set sub_0200059a, 0x0200059a
	.set sub_020005a6, 0x020005a6
	.set sub_020005ac, 0x020005ac
	.set sub_020005b6, 0x020005b6
	.set sub_020005ba, 0x020005ba
	.set sub_020005be, 0x020005be
	.set sub_020005ce, 0x020005ce
	.set sub_020005de, 0x020005de
	.set sub_020005e6, 0x020005e6
	.set sub_020005fa, 0x020005fa
	.set sub_02000616, 0x02000616
	.set sub_02000626, 0x02000626
	.set sub_0200062a, 0x0200062a
	.set sub_0200062c, 0x0200062c
	.set sub_02000638, 0x02000638
	.set sub_0200063e, 0x0200063e
	.set sub_02000646, 0x02000646
	.set sub_02000658, 0x02000658
	.set sub_0200065e, 0x0200065e
	.set sub_0200067a, 0x0200067a
	.set sub_0200068a, 0x0200068a
	.set sub_02000692, 0x02000692
	.set sub_0200069e, 0x0200069e
	.set sub_020006e6, 0x020006e6
	.set sub_020006f0, 0x020006f0
	.set sub_02000710, 0x02000710
	.set sub_02000714, 0x02000714
	.set sub_02000718, 0x02000718
	.set sub_02000730, 0x02000730
	.set sub_0200075c, 0x0200075c
	.set sub_02000762, 0x02000762
	.set sub_02000768, 0x02000768
	.set sub_02000772, 0x02000772
	.set sub_02000774, 0x02000774
	.set sub_020007b4, 0x020007b4
	.set sub_020007ba, 0x020007ba
	.set sub_020007be, 0x020007be
	.set sub_020007c0, 0x020007c0
	.set sub_020007c6, 0x020007c6
	.set sub_020007cc, 0x020007cc
	.set sub_020007d2, 0x020007d2
	.set sub_020007d8, 0x020007d8
	.set sub_020007de, 0x020007de
	.set sub_020007e4, 0x020007e4
	.set sub_02000800, 0x02000800
	.set sub_02000806, 0x02000806
	.set sub_0200080e, 0x0200080e
	.global Overlay_02000000
Overlay_02000000:
	.4byte 0x47204c00
	.4byte 0x02008309
	.4byte 0x47204c00
	.4byte 0x02008031
	.4byte 0x47204c00
	.4byte 0x02008075
	.4byte 0x47204c00
	.4byte 0x0200807d
	.4byte 0x47204c00
	.4byte 0x020080e5
	.4byte 0x47204c00
	.2byte 0x8071
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #36]
	movs	r1, #224
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_02000048
	ldr	r0, [pc, #24]
	b.n	.L_02000054
.L_02000048:
	ldr	r3, [pc, #24]
	cmp	r2, r3
	bne.n	.L_02000052
	ldr	r0, [pc, #24]
	b.n	.L_02000054
.L_02000052:
	ldr	r0, [pc, #24]
.L_02000054:
	pop	{r1}
	bx	r1
	.4byte 0x02000240
	.4byte 0x00000064
	.4byte 0x020084d0
	.4byte 0x00000065
	.4byte 0x020086c8
	.4byte 0x020084a0
	.4byte 0x47702000
	.4byte 0x47704800
	.2byte 0x8728
	.2byte 0x0200
	push	{r5, lr}
	ldr	r1, [pc, #72]
	movs	r0, #224
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #64]
	cmp	r2, r3
	bne.n	.L_020000b6
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #9
	blt.n	.L_020000aa
	cmp	r3, #15
	ble.n	.L_020000a6
	cmp	r3, #17
	bne.n	.L_020000aa
.L_020000a6:
	ldr	r5, [pc, #40]
	b.n	.L_020000ac
.L_020000aa:
	ldr	r5, [pc, #40]
.L_020000ac:
	adds	r0, r5, #0
	bl	sub_020004d0
	adds	r0, r5, #0
	b.n	.L_020000c2
.L_020000b6:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_020000c0
	ldr	r0, [pc, #28]
	b.n	.L_020000c2
.L_020000c0:
	ldr	r0, [pc, #28]
.L_020000c2:
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x02000240
	.4byte 0x00000064
	.4byte 0x020088d4
	.4byte 0x0200879c
	.4byte 0x00000065
	.4byte 0x02008a0c
	.2byte 0x8784
	.2byte 0x0200
	push	{lr}
	ldr	r1, [pc, #64]
	movs	r0, #224
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	ldr	r3, [pc, #56]
	cmp	r2, r3
	bne.n	.L_02000116
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #9
	blt.n	.L_02000112
	cmp	r3, #15
	ble.n	.L_0200010e
	cmp	r3, #17
	bne.n	.L_02000112
.L_0200010e:
	ldr	r0, [pc, #32]
	b.n	.L_02000122
.L_02000112:
	ldr	r0, [pc, #32]
	b.n	.L_02000122
.L_02000116:
	ldr	r3, [pc, #32]
	cmp	r2, r3
	bne.n	.L_02000120
	ldr	r0, [pc, #28]
	b.n	.L_02000122
.L_02000120:
	ldr	r0, [pc, #28]
.L_02000122:
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000064
	.4byte 0x02008c88
	.4byte 0x02008a48
	.4byte 0x00000065
	.4byte 0x02008eb0
	.2byte 0x8a3c
	.2byte 0x0200
	push	{lr}
	bl	sub_02000558
	ldr	r0, [pc, #28]
	bl	sub_020005a6
	movs	r1, #0
	movs	r0, #12
	bl	sub_020005b6
	movs	r0, #145
	lsls	r0, r0, #4
	bl	sub_0200055e
	bl	sub_0200057a
	pop	{r0}
	bx	r0
	.2byte 0x1add
	.2byte 0x0000
	push	{lr}
	bl	sub_02000580
	ldr	r0, [pc, #20]
	bl	sub_020005ce
	movs	r1, #0
	movs	r0, #16
	bl	sub_020005e6
	bl	sub_0200059a
	pop	{r0}
	bx	r0
	.2byte 0x1ae3
	.2byte 0x0000
	push	{lr}
	movs	r0, #0
	bl	sub_020005ba
	ldr	r2, [pc, #80]
	ldrh	r3, [r0, #6]
	adds	r3, r3, r2
	ldr	r2, [pc, #80]
	lsls	r3, r3, #16
	cmp	r3, r2
	bhi.n	.L_020001ac
	movs	r0, #7
	movs	r1, #8
	bl	sub_02000638
	b.n	.L_020001e4
.L_020001ac:
	bl	sub_020005be
	ldr	r0, [pc, #60]
	bl	sub_020005ac
	cmp	r0, #0
	beq.n	.L_020001ca
	ldr	r0, [pc, #56]
	bl	sub_02000616
	movs	r0, #8
	movs	r1, #0
	bl	sub_02000626
	b.n	.L_020001e0
.L_020001ca:
	ldr	r0, [pc, #44]
	bl	sub_02000626
	movs	r0, #8
	movs	r1, #0
	bl	sub_0200063e
	movs	r0, #145
	lsls	r0, r0, #4
	bl	sub_020005de
.L_020001e0:
	bl	sub_020005fa
.L_020001e4:
	pop	{r0}
	bx	r0
	.4byte 0xffff9fff
	.4byte 0x3ffe0000
	.4byte 0x00000911
	.4byte 0x00001afb
	.2byte 0x1ad7
	.2byte 0x0000
	push	{lr}
	movs	r0, #0
	bl	sub_0200062a
	ldr	r2, [pc, #44]
	ldrh	r3, [r0, #6]
	adds	r3, r3, r2
	ldr	r2, [pc, #44]
	lsls	r3, r3, #16
	cmp	r3, r2
	bhi.n	.L_0200021a
	movs	r0, #8
	bl	sub_0200069e
	b.n	.L_02000230
.L_0200021a:
	bl	sub_0200062c
	ldr	r0, [pc, #28]
	bl	sub_0200067a
	movs	r0, #8
	movs	r1, #0
	bl	sub_0200068a
	bl	sub_02000646
.L_02000230:
	pop	{r0}
	bx	r0
	.4byte 0x00005fff
	.4byte 0x3ffe0000
	.2byte 0x1a8f
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #188]
	ldr	r7, [r3, #0]
	bl	sub_02000658
	movs	r5, #8
	movs	r6, #0
.L_0200024e:
	adds	r0, r5, #0
	bl	sub_0200067a
	cmp	r0, #0
	beq.n	.L_0200025e
	adds	r3, r0, #0
	adds	r3, #85
	strb	r6, [r3, #0]
.L_0200025e:
	adds	r5, #1
	cmp	r5, #65
	bls.n	.L_0200024e
	movs	r2, #182
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #13
	beq.n	.L_0200028a
	cmp	r3, #13
	bgt.n	.L_0200027c
	cmp	r3, #12
	beq.n	.L_02000286
	b.n	.L_020002fa
.L_0200027c:
	cmp	r3, #16
	beq.n	.L_0200028e
	cmp	r3, #19
	beq.n	.L_02000292
	b.n	.L_020002fa
.L_02000286:
	movs	r5, #0
	b.n	.L_02000294
.L_0200028a:
	movs	r5, #1
	b.n	.L_02000294
.L_0200028e:
	movs	r5, #2
	b.n	.L_02000294
.L_02000292:
	movs	r5, #3
.L_02000294:
	movs	r0, #158
	bl	sub_02000730
	lsls	r4, r5, #3
	ldr	r0, [pc, #100]
	adds	r3, r4, #4
	ldrh	r1, [r0, r3]
	adds	r3, r3, r0
	ldrh	r2, [r3, #2]
	ldr	r0, [r0, r4]
	bl	sub_02000692
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #7
	lsls	r1, r1, #8
	movs	r0, #0
	bl	sub_020006f0
	movs	r0, #0
	bl	sub_020006e6
	movs	r3, #0
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r1, #2
	movs	r0, #0
	bl	sub_02000714
	movs	r2, #8
	movs	r1, #3
	negs	r2, r2
	movs	r0, #0
	bl	sub_02000718
	movs	r0, #10
	bl	sub_020006e6
	movs	r2, #182
	lsls	r2, r2, #1
	adds	r3, r7, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	sub_0200075c
	bl	sub_02000768
	bl	sub_02000774
	bl	sub_02000710
.L_020002fa:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
	.2byte 0x8ef8
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r2, #224
	ldr	r1, [r3, #0]
	ldr	r3, [pc, #28]
	lsls	r2, r2, #1
	str	r3, [r1, r2]
	ldr	r3, [pc, #28]
	ldrsh	r2, [r3, r2]
	ldr	r3, [pc, #28]
	cmp	r2, r3
	bne.n	.L_02000324
	bl	sub_0200065e
.L_02000324:
	movs	r0, #0
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x00000209
	.4byte 0x02000240
	.2byte 0x0064
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #160]
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	sub	sp, #8
	cmp	r3, #15
	bgt.n	.L_0200035a
	cmp	r3, #9
	bge.n	.L_02000376
	cmp	r3, #3
	beq.n	.L_02000360
	b.n	.L_020003c2
.L_0200035a:
	cmp	r3, #17
	beq.n	.L_02000376
	b.n	.L_020003c2
.L_02000360:
	movs	r3, #4
	movs	r2, #2
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #30
	movs	r1, #14
	movs	r2, #30
	movs	r3, #16
	bl	sub_02000762
	b.n	.L_020003d8
.L_02000376:
	ldr	r0, [pc, #108]
	bl	sub_02000772
	cmp	r0, #0
	beq.n	.L_020003b8
	movs	r0, #10
	bl	sub_020007b4
	movs	r0, #11
	bl	sub_020007ba
	movs	r0, #12
	bl	sub_020007c0
	movs	r0, #13
	bl	sub_020007c6
	movs	r0, #14
	bl	sub_020007cc
	movs	r0, #17
	bl	sub_020007d2
	movs	r0, #18
	bl	sub_020007d8
	movs	r0, #19
	bl	sub_020007de
	movs	r0, #15
	bl	sub_020007e4
	b.n	.L_020003d8
.L_020003b8:
	movs	r0, #13
	movs	r1, #2
	bl	sub_0200080e
	b.n	.L_020003d8
.L_020003c2:
	ldr	r0, [pc, #32]
	bl	sub_020007be
	cmp	r0, #0
	beq.n	.L_020003d8
	movs	r0, #16
	bl	sub_02000800
	movs	r0, #17
	bl	sub_02000806
.L_020003d8:
	add	sp, #8
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000911
	.4byte 0x47204c00
	.4byte 0x08009179
	.4byte 0x47204c00
	.4byte 0x08009181
	.4byte 0x47204c00
	.4byte 0x080770c1
	.4byte 0x47204c00
	.4byte 0x080770c9
	.4byte 0x47204c00
	.4byte 0x0808a011
	.4byte 0x47204c00
	.4byte 0x0808a019
	.4byte 0x47204c00
	.4byte 0x0808a021
	.4byte 0x47204c00
	.4byte 0x0808a039
	.4byte 0x47204c00
	.4byte 0x0808a081
	.4byte 0x47204c00
	.4byte 0x0808a089
	.4byte 0x47204c00
	.4byte 0x0808a091
	.4byte 0x47204c00
	.4byte 0x0808a0d9
	.4byte 0x47204c00
	.4byte 0x0808a101
	.4byte 0x47204c00
	.4byte 0x0808a159
	.4byte 0x47204c00
	.4byte 0x0808a171
	.4byte 0x47204c00
	.4byte 0x0808a181
	.4byte 0x47204c00
	.4byte 0x0808a191
	.4byte 0x47204c00
	.4byte 0x0808a249
	.4byte 0x47204c00
	.4byte 0x0808a369
	.4byte 0x47204c00
	.4byte 0x0808a371
	.4byte 0x47204c00
	.4byte 0x080b0011
	.4byte 0x47204c00
	.4byte 0x080b0019
	.4byte 0x47204c00
	.4byte 0x080f9011
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000090
	.4byte 0xc00000c0
	.4byte 0x00050000
	.4byte 0x00f5002d
	.4byte 0x000000e6
	.4byte 0xffff0002
	.4byte 0x00000190
	.4byte 0xc00000c0
	.4byte 0x00ff0000
	.4byte 0x01e5002d
	.4byte 0x000000e6
	.4byte 0xffff0003
	.4byte 0x00000270
	.4byte 0xc00000f0
	.4byte 0x01f40000
	.4byte 0x02df002d
	.4byte 0x00000113
	.4byte 0xffff0004
	.4byte 0x00000080
	.4byte 0xc00001b0
	.4byte 0x00230000
	.4byte 0x01130122
	.4byte 0x000001d4
	.4byte 0xffff0005
	.4byte 0x00000180
	.4byte 0xc0000180
	.4byte 0x01310000
	.4byte 0x022100ff
	.4byte 0x000001e5
	.4byte 0xffff0006
	.4byte 0x000001d0
	.4byte 0xc00001c0
	.4byte 0x01310000
	.4byte 0x022100ff
	.4byte 0x000001e5
	.4byte 0xffff0007
	.4byte 0x000002d0
	.4byte 0xc00001f0
	.4byte 0x02760000
	.4byte 0x036b0131
	.4byte 0x00000217
	.4byte 0xffff0008
	.4byte 0x00000320
	.4byte 0xc00001b8
	.4byte 0x02760000
	.4byte 0x036b0131
	.4byte 0x00000217
	.4byte 0xffff0009
	.4byte 0x00000030
	.4byte 0xc0000280
	.4byte 0x00000000
	.4byte 0x013101ef
	.4byte 0x00000307
	.4byte 0xffff000a
	.4byte 0x000000e0
	.4byte 0xc00002e0
	.4byte 0x00000000
	.4byte 0x013101ef
	.4byte 0x00000307
	.4byte 0xffff000b
	.4byte 0x00000198
	.4byte 0x40000268
	.4byte 0x01630000
	.4byte 0x02b2020d
	.4byte 0x000002c6
	.4byte 0xffff000c
	.4byte 0x00000228
	.4byte 0x40000288
	.4byte 0x01630000
	.4byte 0x02b2020d
	.4byte 0x000002c6
	.4byte 0xffff000d
	.4byte 0x00000278
	.4byte 0x40000288
	.4byte 0x01630000
	.4byte 0x02b2020d
	.4byte 0x000002c6
	.4byte 0xffff000e
	.4byte 0x00000228
	.4byte 0xc00002a8
	.4byte 0x01630000
	.4byte 0x02b2020d
	.4byte 0x000002c6
	.4byte 0xffff000f
	.4byte 0x00000278
	.4byte 0xc00002a8
	.4byte 0x01630000
	.4byte 0x02b2020d
	.4byte 0x000002c6
	.4byte 0xffff0010
	.4byte 0x00000328
	.4byte 0x40000268
	.4byte 0x02e90000
	.4byte 0x03a7023a
	.4byte 0x000002d0
	.4byte 0xffff0011
	.4byte 0x000001b8
	.4byte 0xc0000368
	.4byte 0x01540000
	.4byte 0x020802e4
	.4byte 0x00000384
	.4byte 0xffff0012
	.4byte 0x00000248
	.4byte 0xc0000368
	.4byte 0x021c0000
	.4byte 0x02da02e4
	.4byte 0x00000384
	.4byte 0xffff0013
	.4byte 0x00000348
	.4byte 0x40000318
	.4byte 0x02c60000
	.4byte 0x03ac02e4
	.4byte 0x00000384
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000f8
	.4byte 0xc0000130
	.4byte 0x006e0000
	.4byte 0x01860023
	.4byte 0x00000159
	.4byte 0xffff0002
	.4byte 0x000000f8
	.4byte 0x40000080
	.4byte 0x006e0000
	.4byte 0x01860023
	.4byte 0x00000159
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000064
	.4byte 0x00101063
	.4byte 0x00202063
	.4byte 0x00303063
	.4byte 0x00404063
	.4byte 0x0050c063
	.4byte 0x00605063
	.4byte 0x00706063
	.4byte 0x0080d063
	.4byte 0x0090b064
	.4byte 0x00a07063
	.4byte 0x00b09064
	.4byte 0x00c11064
	.4byte 0x00d12064
	.4byte 0x00e13064
	.4byte 0x00f10064
	.4byte 0x0100f064
	.4byte 0x0110c064
	.4byte 0x0120d064
	.4byte 0x0130e064
	.4byte 0x00000065
	.4byte 0x00108063
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x00650000
	.4byte 0x00000000
	.4byte 0x00b50000
	.4byte 0x00003000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x00a70000
	.4byte 0x00000000
	.4byte 0x006d0000
	.4byte 0x0000b000
	.4byte 0x00000065
	.4byte 0x00000001
	.4byte 0x017f0000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x0000b000
	.4byte 0x00000073
	.4byte 0x00000001
	.4byte 0x015a0000
	.4byte 0x00000000
	.4byte 0x006d0000
	.4byte 0x0000f000
	.4byte 0x0000007e
	.4byte 0x00000002
	.4byte 0x023d0000
	.4byte 0x00000000
	.4byte 0x00a30000
	.4byte 0x0000b000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x029e0000
	.4byte 0x00000000
	.4byte 0x009a0000
	.4byte 0x00005000
	.4byte 0x0000007f
	.4byte 0x00000001
	.4byte 0x00b70000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00005000
	.4byte 0x0000006f
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01710000
	.4byte 0x00003000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x02740000
	.4byte 0x00007000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x030c0000
	.4byte 0x00000000
	.4byte 0x02790000
	.4byte 0x00013000
	.4byte 0x0000008f
	.4byte 0x00000001
	.4byte 0x031b0000
	.4byte 0x00000000
	.4byte 0x03260000
	.4byte 0x0000d000
	.4byte 0x0000008f
	.4byte 0x00000001
	.4byte 0x03590000
	.4byte 0x00000000
	.4byte 0x03490000
	.4byte 0x00005000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000077
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02730000
	.4byte 0x00011000
	.4byte 0x00000080
	.4byte 0x00000002
	.4byte 0x01f90000
	.4byte 0x00000000
	.4byte 0x02990000
	.4byte 0x00000000
	.4byte 0x00000084
	.4byte 0x00000001
	.4byte 0x00420000
	.4byte 0x00000000
	.4byte 0x026e0000
	.4byte 0x0001b000
	.4byte 0x00000094
	.4byte 0x00000001
	.4byte 0x00c40000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00005000
	.4byte 0x000000ab
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00025000
	.4byte 0x000000ab
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x023a0000
	.4byte 0x00005000
	.4byte 0x00000072
	.4byte 0x00000001
	.4byte 0x00cf0000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x00008000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02830000
	.4byte 0x00007000
	.4byte 0x0000006f
	.4byte 0x00000001
	.4byte 0x00f70000
	.4byte 0x00000000
	.4byte 0x02e00000
	.4byte 0x0001b000
	.4byte 0x0000009d
	.4byte 0x00000001
	.4byte 0x01070000
	.4byte 0x00000000
	.4byte 0x02730000
	.4byte 0x00007000
	.4byte 0x00000043
	.4byte 0x00000001
	.4byte 0x01a10000
	.4byte 0x00000000
	.4byte 0x032a0000
	.4byte 0x00001000
	.4byte 0x00000042
	.4byte 0x00000001
	.4byte 0x01cb0000
	.4byte 0x00000000
	.4byte 0x03340000
	.4byte 0x00009000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0079
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x006a0000
	.4byte 0x00005000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x0000c602
	.4byte 0xffff0010
	.4byte 0x02008241
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x0000c602
	.4byte 0xffff0013
	.4byte 0x02008241
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001abf
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001ac0
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001ac3
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001ac4
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001ac7
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001ac8
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001acb
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001acc
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0200816d
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001ae6
	.4byte 0x00000000
	.4byte 0x09110012
	.4byte 0x00001ae7
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001afe
	.4byte 0x00000000
	.4byte 0x09110013
	.4byte 0x00001ae8
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001aff
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001ac1
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001ac2
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001ac5
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001ac6
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001ac9
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001aca
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001acd
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001ace
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001af5
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001af6
	.4byte 0x00008d15
	.4byte 0x09110012
	.4byte 0x00001af7
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001b03
	.4byte 0x00008d15
	.4byte 0x09110013
	.4byte 0x00001af8
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001b04
	.4byte 0x00000033
	.4byte 0x0f830064
	.4byte 0x0020000b
	.4byte 0x00000173
	.4byte 0x0f840065
	.4byte 0x001000e2
	.4byte 0x00000033
	.4byte 0x0f850066
	.4byte 0x001000bc
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x004029a8
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x004029a9
	.4byte 0x00000173
	.4byte 0xffff0065
	.4byte 0x004029aa
	.4byte 0x00000173
	.4byte 0xffff00cb
	.4byte 0x004029ab
	.4byte 0x00000173
	.4byte 0xffff00cc
	.4byte 0x004029ac
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte 0x02008241
	.4byte 0x0000c602
	.4byte 0xffff000d
	.4byte 0x02008241
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0200818d
	.4byte 0x00000000
	.4byte 0x09110009
	.4byte 0x00001ada
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001afc
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001adb
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001adc
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008145
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001ade
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001adf
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001ae0
	.4byte 0x00000000
	.4byte 0x09110010
	.4byte 0x00001ae1
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001afd
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001ae2
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001ae9
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001aea
	.4byte 0x00008d15
	.4byte 0x09110008
	.4byte 0x00001aeb
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001b00
	.4byte 0x00008d15
	.4byte 0x09110009
	.4byte 0x00001aec
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001b01
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001aed
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001aee
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001aef
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001af0
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001af1
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001af2
	.4byte 0x00008d15
	.4byte 0x09110010
	.4byte 0x00001af3
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001b02
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001af4
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001af9
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001afa
	.4byte 0x00000033
	.4byte 0x0f830064
	.4byte 0x0020000b
	.4byte 0x00000173
	.4byte 0x0f840065
	.4byte 0x001000e2
	.4byte 0x00000033
	.4byte 0x0f850066
	.4byte 0x001000bc
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x004029a8
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x004029a9
	.4byte 0x00000173
	.4byte 0xffff0065
	.4byte 0x004029aa
	.4byte 0x00000173
	.4byte 0xffff00cb
	.4byte 0x004029ab
	.4byte 0x00000173
	.4byte 0xffff00cc
	.4byte 0x004029ac
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020081fd
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001a90
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x007d0007
	.4byte 0x00020001
	.4byte 0x00080005
	.4byte 0x0001007d
	.4byte 0x00050002
	.4byte 0x0000ffff
	.4byte 0x02008ee0
	.4byte 0x00620022
	.4byte 0x02008ee0
	.4byte 0x00620027
	.4byte 0x02008ee0
	.4byte 0x00600032
	.4byte 0x02008ee0
	.4byte 0x006b0034

.syntax unified
	.thumb
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_080049ac, 0x080049ac
	.set sub_08004bd4, 0x08004bd4
	.set sub_08004c1c, 0x08004c1c
	.set sub_08004c6c, 0x08004c6c
	.set sub_08004cb4, 0x08004cb4
	.set sub_08004cf0, 0x08004cf0
	.set sub_08005258, 0x08005258
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080072f8, 0x080072f8
	.set sub_080b5040, 0x080b5040
	.set sub_080c9048, 0x080c9048
	.set sub_080cd104, 0x080cd104
	.set sub_080cd594, 0x080cd594
	.set sub_080d40ec, 0x080d40ec
	.set sub_080d6750, 0x080d6750
	.set sub_080d67dc, 0x080d67dc
	.set sub_080dbb24, 0x080dbb24
	.set sub_080e0524, 0x080e0524
	.set sub_080e1df8, 0x080e1df8
	.set sub_080e3944, 0x080e3944
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Overlay_080e15e8
Overlay_080e15e8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #108]
	ldr	r1, [r3, #0]
	sub	sp, #336
	str	r1, [sp, #132]
	adds	r2, r3, #0
	subs	r2, #20
	ldr	r2, [r2, #0]
	str	r2, [sp, #128]
	adds	r2, r3, #0
	subs	r2, #16
	ldr	r2, [r2, #0]
	str	r2, [sp, #124]
	subs	r3, #12
	ldr	r2, [sp, #128]
	ldr	r4, [pc, #84]
	ldr	r3, [r3, #0]
	str	r3, [sp, #120]
	adds	r3, r2, r4
	str	r0, [r3, #0]
	movs	r0, #128
	lsls	r0, r0, #6
	bl	sub_080cd594
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #56]
	ldr	r5, [pc, #68]
	strh	r3, [r2, #0]
	ldr	r1, [sp, #128]
	adds	r0, r5, #0
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	adds	r0, r5, #0
	ldr	r1, [sp, #128]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r2, #192
	ldr	r5, [sp, #128]
	lsls	r2, r2, #5
	adds	r1, r5, r2
	ldr	r0, [pc, #36]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [pc, #32]
	movs	r3, #0
	ldr	r1, [sp, #120]
	movs	r2, #0
	b.n	.L_080e167c
	.4byte 0x00000100
	.4byte 0x03001f00
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x000000bc
	.4byte 0x00000075
	.2byte 0x0073
	.2byte 0x0000
.L_080e167c:
	bl	sub_080e0524
	ldr	r5, [pc, #136]
	movs	r3, #0
	mov	r9, r3
	movs	r4, #0
	movs	r0, #64
.L_080e168a:
	ldr	r2, [sp, #128]
	movs	r1, #0
	adds	r3, r4, r2
	ldr	r2, [pc, #124]
	mov	r8, r1
	ldr	r1, [sp, #120]
	mov	ip, r0
	adds	r3, r3, r2
.L_080e169a:
	ldrb	r2, [r1, #0]
	adds	r1, #1
	cmp	r2, ip
	ble.n	.L_080e16a4
	mov	r2, ip
.L_080e16a4:
	cmp	r2, #0
	bge.n	.L_080e16aa
	movs	r2, #0
.L_080e16aa:
	strb	r2, [r3, #0]
	movs	r2, #1
	add	r8, r2
	adds	r3, #1
	cmp	r8, r5
	bne.n	.L_080e169a
	add	r9, r2
	mov	r3, r9
	add	r4, r8
	subs	r0, #7
	cmp	r3, #8
	bne.n	.L_080e168a
	bl	sub_080c9048
	ldr	r2, [pc, #60]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #0]
	ldr	r4, [sp, #128]
	ldr	r5, [pc, #60]
	ldr	r0, [pc, #60]
	adds	r3, r4, r5
	movs	r6, #0
	ldr	r2, [pc, #60]
	str	r6, [r3, #0]
	movs	r1, #2
	adds	r3, r4, r0
	str	r1, [r3, #0]
	adds	r5, #12
	adds	r3, r4, r2
	movs	r2, #1
	str	r2, [r3, #0]
	adds	r3, r4, r5
	str	r6, [r3, #0]
	ldr	r0, [sp, #132]
	movs	r5, #144
	lsls	r5, r5, #3
	str	r2, [r0, #16]
	b.n	.L_080e1724
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00002784
	.4byte 0x00000302
	.4byte 0x00002710
	.4byte 0x0400000c
	.4byte 0x00007790
	.4byte 0x00007794
	.2byte 0x7798
	.2byte 0x0000
.L_080e1724:
	mov	sl, r1
	ldr	r0, [pc, #132]
	adds	r1, r5, #0
	bl	sub_080041d8
	adds	r1, r5, #0
	ldr	r0, [pc, #124]
	bl	sub_080041d8
	movs	r2, #239
	ldr	r1, [sp, #128]
	lsls	r2, r2, #7
	adds	r1, r1, r2
	str	r6, [r1, #0]
	mov	r8, r1
	movs	r0, #0
	movs	r1, #0
	bl	sub_080cd104
	ldr	r1, [pc, #104]
	movs	r0, #1
	movs	r2, #0
	bl	sub_080b5040
	ldr	r2, [pc, #96]
	movs	r3, #240
	str	r3, [r2, #16]
	movs	r0, #0
	movs	r1, #1
	bl	sub_080cd104
	ldr	r6, [pc, #88]
	ldr	r5, [pc, #52]
	movs	r3, #3
	strh	r5, [r6, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r0, #46
	str	r3, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [pc, #72]
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r2, #128
	str	r3, [sp, #108]
	ldr	r3, [pc, #28]
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	adds	r2, #82
	strh	r5, [r6, #0]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #24]
	subs	r2, #2
	strh	r3, [r2, #0]
	mov	r4, r8
	mov	r3, sl
	str	r3, [r4, #0]
	b.n	.L_080e17c4
	.4byte 0x00000080
	.4byte 0x00007741
	.4byte 0x0000100f
	.4byte 0x00003f44
	.4byte 0x080c90e5
	.4byte 0x080cd261
	.4byte 0x0000003d
	.4byte 0x03001ce0
	.4byte 0x04000020
	.2byte 0x1e50
	.2byte 0x0300
.L_080e17c4:
	ldr	r5, [sp, #128]
	ldr	r0, [pc, #108]
	movs	r3, #75
	adds	r2, r5, r0
	str	r3, [r2, #0]
	movs	r2, #152
	lsls	r2, r2, #1
	add	r2, sp
	adds	r5, r2, #0
	str	r2, [sp, #48]
	ldr	r2, [pc, #92]
	movs	r1, #0
	add	r2, sp
	mov	r9, r1
	movs	r6, #63
	adds	r0, r5, #0
	movs	r1, #3
	adds	r4, r2, #0
.L_080e17e8:
	strb	r1, [r0, #0]
	strb	r1, [r4, #0]
	ldrb	r3, [r0, #0]
	subs	r4, #1
	cmp	r3, #63
	bls.n	.L_080e17f6
	strb	r6, [r5, #0]
.L_080e17f6:
	ldrb	r3, [r2, #0]
	cmp	r3, #63
	bls.n	.L_080e17fe
	strb	r6, [r2, #0]
.L_080e17fe:
	movs	r3, #1
	add	r9, r3
	mov	r3, r9
	adds	r5, #1
	adds	r0, #1
	subs	r2, #1
	adds	r1, #8
	cmp	r3, #16
	bne.n	.L_080e17e8
	ldr	r2, [pc, #40]
	ldr	r3, [pc, #28]
	mov	r5, sp
	ldr	r0, [pc, #40]
	ldr	r1, [pc, #40]
	movs	r4, #0
	adds	r5, #136
	strh	r3, [r2, #0]
	str	r4, [sp, #116]
	str	r5, [sp, #96]
	str	r0, [sp, #28]
	str	r4, [sp, #24]
	str	r4, [sp, #20]
	str	r1, [sp, #16]
	b.n	.L_080e1cc2
	movs	r0, r0
	.4byte 0x00000784
	.4byte 0x00007784
	.4byte 0x0000014f
	.4byte 0x0400000c
	.4byte 0xfffff460
	.2byte 0x6c00
	.2byte 0xfff2
.L_080e1848:
	.2byte 0x4a93
	ldr	r0, [sp, #20]
	ldr	r1, [pc, #588]
	movs	r3, #0
	bl	sub_080d40ec
	ldr	r2, [sp, #116]
	cmp	r2, #150
	bne.n	.L_080e1870
	ldr	r3, [sp, #128]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r5, [sp, #128]
	ldr	r0, [pc, #564]
	ldr	r3, [pc, #568]
	adds	r2, r5, r0
	b.n	.L_080e1882
.L_080e1870:
	ldr	r1, [sp, #128]
	movs	r2, #239
	lsls	r2, r2, #7
	ldr	r4, [sp, #104]
	ldr	r5, [pc, #548]
	adds	r3, r1, r2
	str	r4, [r3, #0]
	adds	r2, r1, r5
	movs	r3, #75
.L_080e1882:
	str	r3, [r2, #0]
	movs	r0, #255
	movs	r1, #192
	ldr	r3, [pc, #540]
	lsls	r1, r1, #8
	lsls	r0, r0, #17
	bl	sub_080072f0
	adds	r1, r0, #0
	movs	r0, #255
	lsls	r0, r0, #17
	ldr	r2, [pc, #528]
	bl	sub_08005258
	bl	sub_080049ac
	ldr	r0, [sp, #116]
	cmp	r0, #128
	ble.n	.L_080e1922
	adds	r6, r0, #0
	subs	r6, #128
	cmp	r6, #22
	ble.n	.L_080e18b2
	movs	r6, #20
.L_080e18b2:
	add	r0, sp, #148
	movs	r3, #0
	str	r3, [r0, #0]
	negs	r3, r6
	lsls	r3, r3, #17
	str	r3, [r0, #4]
	movs	r3, #128
	lsls	r3, r3, #18
	str	r3, [r0, #8]
	asrs	r3, r6, #2
	adds	r3, #2
	str	r3, [sp, #104]
	cmp	r3, #8
	ble.n	.L_080e18d2
	movs	r1, #8
	str	r1, [sp, #104]
.L_080e18d2:
	movs	r5, #128
	bl	sub_08004cb4
	lsls	r5, r5, #8
	ldr	r0, [pc, #468]
	bl	sub_08004bd4
	adds	r0, r5, #0
	bl	sub_08004c6c
	adds	r0, r5, #0
	bl	sub_08004bd4
	lsls	r0, r6, #12
	bl	sub_08004c1c
	ldr	r2, [sp, #116]
	cmp	r2, #150
	ble.n	.L_080e1908
	ldr	r0, [sp, #96]
	ldr	r3, [sp, #16]
	movs	r4, #5
	str	r3, [r0, #0]
	str	r3, [r0, #4]
	str	r3, [r0, #8]
	str	r4, [sp, #104]
	b.n	.L_080e191c
.L_080e1908:
	lsls	r3, r6, #1
	adds	r3, r3, r6
	movs	r2, #128
	ldr	r0, [sp, #96]
	lsls	r3, r3, #10
	lsls	r2, r2, #9
	subs	r2, r2, r3
	str	r2, [r0, #0]
	str	r2, [r0, #4]
	str	r2, [r0, #8]
.L_080e191c:
	bl	sub_08004cf0
	b.n	.L_080e194a
.L_080e1922:
	add	r0, sp, #148
	movs	r3, #0
	str	r3, [r0, #0]
	str	r3, [r0, #4]
	movs	r3, #128
	lsls	r3, r3, #17
	str	r3, [r0, #8]
	bl	sub_08004cb4
	ldr	r0, [pc, #376]
	bl	sub_08004bd4
	ldr	r0, [sp, #116]
	lsls	r5, r0, #8
	adds	r0, r5, #0
	bl	sub_08004c6c
	adds	r0, r5, #0
	bl	sub_08004bd4
.L_080e194a:
	ldr	r1, [sp, #116]
	cmp	r1, #149
	ble.n	.L_080e1952
	b.n	.L_080e1a64
.L_080e1952:
	mov	r3, sp
	movs	r2, #0
	adds	r3, #160
	mov	r9, r2
	str	r3, [sp, #52]
	ldr	r2, [pc, #340]
	add	r6, sp, #292
	adds	r5, r3, #0
	movs	r7, #0
.L_080e1964:
	ldrsh	r3, [r7, r2]
	subs	r3, #96
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	movs	r3, #0
	str	r3, [r6, #4]
	adds	r3, r7, #2
	ldrsh	r3, [r3, r2]
	subs	r3, #96
	lsls	r3, r3, #16
	adds	r1, r5, #0
	str	r3, [r6, #8]
	adds	r0, r6, #0
	str	r2, [sp, #12]
	bl	sub_080e3944
	ldr	r3, [r5, #0]
	asrs	r3, r3, #17
	adds	r3, #64
	str	r3, [r5, #0]
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	adds	r3, #60
	str	r3, [r5, #4]
	movs	r3, #1
	add	r9, r3
	mov	r4, r9
	adds	r5, #12
	adds	r7, #4
	ldr	r2, [sp, #12]
	cmp	r4, #6
	bne.n	.L_080e1964
	ldr	r1, [sp, #24]
	movs	r5, #0
	movs	r0, #4
	str	r0, [sp, #40]
	str	r5, [sp, #36]
	str	r1, [sp, #32]
	mov	r9, r5
.L_080e19b2:
	ldr	r2, [sp, #32]
	ldr	r3, [pc, #256]
	adds	r2, r2, r3
	mov	sl, r2
	cmp	r2, #48
	ble.n	.L_080e19c2
	movs	r4, #48
	mov	sl, r4
.L_080e19c2:
	mov	r5, sl
	cmp	r5, #0
	blt.n	.L_080e1a48
	movs	r0, #0
	mov	r8, r0
	cmp	r5, #0
	beq.n	.L_080e1a48
	ldr	r1, [sp, #104]
	ldr	r2, [sp, #40]
	ldr	r3, [sp, #104]
	asrs	r1, r1, #31
	lsls	r3, r3, #1
	str	r1, [sp, #44]
	ldr	r5, [sp, #52]
	ldr	r6, [sp, #36]
	ldr	r7, [pc, #216]
	str	r2, [sp, #92]
	mov	fp, r3
.L_080e19e6:
	adds	r3, r6, #0
	adds	r3, #12
	ldr	r3, [r5, r3]
	ldr	r4, [r5, r6]
	subs	r3, r3, r4
	mov	r0, r8
	muls	r0, r3
	ldr	r1, [pc, #200]
	movs	r0, r0
	mov	ip, pc
	bx	r7
	adds	r4, r4, r0
	adds	r3, r6, #0
	ldr	r0, [sp, #92]
	adds	r3, #16
	ldr	r2, [r5, r3]
	ldr	r3, [r5, r0]
	subs	r2, r2, r3
	mov	r0, r8
	muls	r0, r2
	ldr	r1, [pc, #176]
	mov	ip, pc
	bx	r7
	adds	r3, r3, r0
	mov	r2, fp
	ldr	r0, [pc, #168]
	subs	r2, #2
	ldrh	r1, [r0, r2]
	ldr	r2, [sp, #120]
	ldr	r0, [sp, #44]
	adds	r1, r2, r1
	lsrs	r2, r0, #31
	ldr	r0, [sp, #104]
	adds	r2, r0, r2
	asrs	r2, r2, #1
	subs	r4, r4, r2
	mov	r2, fp
	subs	r3, r3, r0
	str	r0, [sp, #0]
	str	r2, [sp, #4]
	ldr	r0, [sp, #124]
	adds	r2, r4, #0
	ldr	r4, [sp, #108]
	bl	sub_080072f4
	movs	r0, #1
	add	r8, r0
	cmp	r8, sl
	bne.n	.L_080e19e6
.L_080e1a48:
	ldr	r1, [sp, #40]
	ldr	r2, [sp, #36]
	ldr	r3, [sp, #32]
	movs	r4, #1
	add	r9, r4
	adds	r1, #24
	adds	r2, #24
	subs	r3, #48
	mov	r5, r9
	str	r1, [sp, #40]
	str	r2, [sp, #36]
	str	r3, [sp, #32]
	cmp	r5, #3
	bne.n	.L_080e19b2
.L_080e1a64:
	ldr	r0, [sp, #116]
	cmp	r0, #179
	ble.n	.L_080e1a6c
	b.n	.L_080e1b8e
.L_080e1a6c:
	movs	r5, #0
	cmp	r0, #155
	ble.n	.L_080e1a76
	adds	r5, r0, #0
	subs	r5, #156
.L_080e1a76:
	cmp	r5, #7
	ble.n	.L_080e1a7c
	movs	r5, #7
.L_080e1a7c:
	ldr	r1, [sp, #116]
	cmp	r1, #139
	bgt.n	.L_080e1acc
	movs	r3, #3
	movs	r2, #7
	movs	r0, #47
	movs	r1, #7
	str	r3, [sp, #0]
	bl	sub_080ed408
	ldr	r2, [pc, #52]
	ldr	r2, [r2, #0]
	str	r2, [sp, #88]
	b.n	.L_080e1ae2
	.4byte 0x00005555
	.4byte 0x0000aaab
	.4byte 0x00007784
	.4byte 0x1a1a1a1a
	.4byte 0x0300013c
	.4byte 0x7fff0000
	.4byte 0xfffff000
	.4byte 0x080eda98
	.4byte 0xffffff00
	.4byte 0x03000118
	.4byte 0x00000555
	.4byte 0x080ede48
	.2byte 0x1f0c
	.2byte 0x0300
.L_080e1acc:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	bl	sub_080ed408
	ldr	r4, [pc, #720]
	ldr	r4, [r4, #0]
	str	r4, [sp, #88]
.L_080e1ae2:
	ldr	r0, [sp, #100]
	mov	sl, r0
	cmp	r0, #128
	ble.n	.L_080e1aee
	movs	r1, #128
	mov	sl, r1
.L_080e1aee:
	movs	r2, #0
	mov	r3, sl
	mov	r9, r2
	cmp	r3, #0
	beq.n	.L_080e1b88
	lsls	r3, r5, #1
	adds	r3, r3, r5
	ldr	r7, [sp, #104]
	lsls	r3, r3, #7
	movs	r4, #146
	adds	r3, r3, r5
	adds	r7, #1
	lsls	r4, r4, #1
	lsls	r3, r3, #1
	add	r4, sp
	lsls	r0, r7, #1
	str	r3, [sp, #84]
	mov	r8, r4
	add	r6, sp, #160
	mov	fp, r0
.L_080e1b16:
	mov	r1, r9
	lsls	r5, r1, #9
	adds	r0, r5, #0
	bl	sub_08002322
	lsls	r3, r0, #1
	adds	r3, r3, r0
	mov	r2, r8
	lsls	r3, r3, #5
	str	r3, [r2, #0]
	adds	r0, r5, #0
	bl	sub_0800231c
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #5
	negs	r3, r3
	mov	r4, r8
	str	r3, [r4, #8]
	adds	r1, r6, #0
	mov	r0, r8
	bl	sub_080e3944
	ldr	r2, [r6, #0]
	movs	r5, #6
	ldrsh	r3, [r6, r5]
	asrs	r2, r2, #17
	ldr	r0, [pc, #612]
	adds	r2, #64
	adds	r3, #60
	mov	r1, fp
	str	r2, [r6, #0]
	str	r3, [r6, #4]
	subs	r1, #2
	ldrh	r1, [r0, r1]
	ldr	r0, [sp, #84]
	ldr	r4, [sp, #128]
	adds	r1, r0, r1
	lsrs	r0, r7, #31
	ldr	r5, [pc, #592]
	adds	r0, r7, r0
	adds	r1, r4, r1
	asrs	r0, r0, #1
	adds	r1, r1, r5
	subs	r2, r2, r0
	movs	r5, #1
	mov	r0, fp
	str	r0, [sp, #4]
	subs	r3, r3, r7
	str	r7, [sp, #0]
	ldr	r0, [sp, #124]
	ldr	r4, [sp, #88]
	add	r9, r5
	bl	sub_080072f4
	cmp	r9, sl
	bne.n	.L_080e1b16
.L_080e1b88:
	movs	r0, #47
	bl	sub_08002dd8
.L_080e1b8e:
	ldr	r3, [sp, #116]
	subs	r3, #151
	cmp	r3, #16
	bhi.n	.L_080e1c90
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #116]
	movs	r4, #0
	mov	r8, r0
	cmp	r1, #151
	ble.n	.L_080e1ba6
	adds	r4, r1, #0
	subs	r4, #152
.L_080e1ba6:
	cmp	r4, #15
	ble.n	.L_080e1bac
	movs	r4, #15
.L_080e1bac:
	lsls	r3, r4, #1
	ldr	r5, [sp, #48]
	negs	r3, r3
	adds	r7, r3, #0
	adds	r3, r4, r5
	movs	r2, #1
	adds	r6, r4, #0
	adds	r3, #1
	mov	r9, r2
	adds	r7, #30
	adds	r6, #49
	mov	sl, r3
.L_080e1bc4:
	mov	r0, r9
	adds	r3, r4, r0
	cmp	r3, #15
	bgt.n	.L_080e1be6
	movs	r2, #1
	movs	r3, #16
	subs	r3, r3, r0
	str	r2, [sp, #4]
	str	r4, [sp, #8]
	str	r7, [sp, #0]
	ldr	r0, [sp, #124]
	mov	r1, sl
	adds	r2, r6, #0
	ldr	r5, [sp, #108]
	bl	sub_080072f8
	ldr	r4, [sp, #8]
.L_080e1be6:
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	subs	r7, #2
	adds	r6, #1
	add	sl, r0
	cmp	r1, #10
	bne.n	.L_080e1bc4
	movs	r2, #0
	mov	r3, r8
	mov	r9, r2
	cmp	r3, #0
	beq.n	.L_080e1c32
	lsls	r2, r4, #1
	movs	r3, #32
	adds	r6, r4, #0
	subs	r3, r3, r2
	adds	r6, #48
	mov	sl, r3
	movs	r7, #1
.L_080e1c0e:
	ldr	r2, [sp, #48]
	mov	r3, r9
	mov	r5, sl
	adds	r1, r2, r4
	str	r5, [sp, #0]
	str	r4, [sp, #8]
	adds	r3, #16
	str	r7, [sp, #4]
	ldr	r0, [sp, #124]
	adds	r2, r6, #0
	ldr	r5, [sp, #108]
	bl	sub_080072f8
	movs	r0, #1
	add	r9, r0
	ldr	r4, [sp, #8]
	cmp	r9, r8
	bne.n	.L_080e1c0e
.L_080e1c32:
	movs	r1, #2
	str	r1, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	movs	r3, #3
	movs	r0, #47
	bl	sub_080ed408
	ldr	r2, [sp, #28]
	movs	r5, #96
	movs	r6, #32
	str	r5, [sp, #4]
	str	r6, [sp, #0]
	ldr	r3, [pc, #352]
	subs	r2, #56
	mov	r8, r2
	ldr	r1, [sp, #128]
	ldr	r4, [r3, #0]
	ldr	r0, [sp, #124]
	movs	r2, #32
	mov	r3, r8
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	movs	r4, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	str	r4, [sp, #0]
	bl	sub_080ed408
	str	r5, [sp, #4]
	str	r6, [sp, #0]
	ldr	r5, [pc, #308]
	ldr	r0, [sp, #124]
	ldr	r4, [r5, #0]
	ldr	r1, [sp, #128]
	movs	r2, #64
	mov	r3, r8
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
.L_080e1c90:
	ldr	r0, [sp, #128]
	ldr	r1, [pc, #296]
	movs	r3, #1
	adds	r2, r0, r1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r0, #192
	ldr	r2, [sp, #28]
	ldr	r3, [sp, #24]
	ldr	r4, [sp, #20]
	ldr	r5, [sp, #16]
	ldr	r1, [sp, #116]
	lsls	r0, r0, #5
	adds	r2, #20
	adds	r3, #4
	adds	r4, #2
	adds	r5, r5, r0
	adds	r1, #1
	str	r2, [sp, #28]
	str	r3, [sp, #24]
	str	r4, [sp, #20]
	str	r5, [sp, #16]
	str	r1, [sp, #116]
.L_080e1cc2:
	ldr	r2, [sp, #116]
	cmp	r2, #170
	beq.n	.L_080e1d00
	ldr	r4, [sp, #20]
	movs	r3, #2
	str	r3, [sp, #104]
	str	r4, [sp, #100]
	cmp	r2, #16
	bne.n	.L_080e1cda
	movs	r0, #140
	bl	sub_080f9010
.L_080e1cda:
	ldr	r5, [sp, #116]
	cmp	r5, #132
	bne.n	.L_080e1ce6
	movs	r0, #131
	bl	sub_080f9010
.L_080e1ce6:
	ldr	r0, [sp, #116]
	cmp	r0, #151
	bne.n	.L_080e1cf2
	movs	r0, #145
	bl	sub_080f9010
.L_080e1cf2:
	ldr	r3, [pc, #204]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080e1d00
	b.n	.L_080e1848
.L_080e1d00:
	ldr	r1, [sp, #132]
	movs	r3, #0
	str	r3, [r1, #16]
	ldr	r0, [pc, #188]
	bl	sub_08004278
	bl	sub_080d67dc
	ldr	r4, [pc, #180]
	ldr	r2, [sp, #128]
	adds	r3, r2, r4
	ldr	r0, [r3, #0]
	bl	sub_080d6750
	ldr	r1, [pc, #172]
	movs	r0, #9
	movs	r2, #1
	bl	sub_080dbb24
	ldr	r0, [pc, #168]
	ldr	r1, [pc, #168]
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [pc, #164]
	ldr	r1, [sp, #128]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r2, #192
	ldr	r5, [sp, #128]
	lsls	r2, r2, #7
	adds	r1, r5, r2
	movs	r3, #0
	movs	r2, #1
	ldr	r0, [pc, #144]
	bl	sub_080e0524
	ldr	r0, [pc, #140]
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #136]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	movs	r2, #0
	ldr	r1, [sp, #120]
	movs	r3, #0
	ldr	r0, [pc, #124]
	bl	sub_080e0524
	ldr	r2, [pc, #120]
	ldr	r3, [pc, #56]
	movs	r4, #128
	strh	r3, [r2, #0]
	movs	r3, #160
	movs	r5, #0
	lsls	r3, r3, #15
	lsls	r4, r4, #15
	str	r3, [sp, #80]
	str	r4, [sp, #76]
	str	r5, [sp, #72]
	str	r5, [sp, #68]
	ldr	r2, [pc, #100]
	ldr	r3, [pc, #104]
	mov	r9, r5
	movs	r1, #0
.L_080e1d90:
	movs	r0, #1
	add	r9, r0
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r9, r2
	bne.n	.L_080e1d90
	ldr	r2, [sp, #128]
	movs	r3, #225
	movs	r1, #0
	lsls	r3, r3, #7
	mov	r9, r1
	adds	r5, r2, r3
	b.n	sub_080e1df8

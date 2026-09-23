.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_08002304, 0x08002304
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_08004bd4, 0x08004bd4
	.set sub_08004c1c, 0x08004c1c
	.set sub_08004c6c, 0x08004c6c
	.set sub_080051d8, 0x080051d8
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080072f8, 0x080072f8
	.set sub_080072fc, 0x080072fc
	.set sub_08007300, 0x08007300
	.set sub_08007308, 0x08007308
	.set sub_0800730c, 0x0800730c
	.set sub_08009008, 0x08009008
	.set sub_08009020, 0x08009020
	.set sub_08009030, 0x08009030
	.set sub_08009038, 0x08009038
	.set sub_08009080, 0x08009080
	.set sub_08009088, 0x08009088
	.set sub_080b5028, 0x080b5028
	.set sub_080b5038, 0x080b5038
	.set sub_080b5040, 0x080b5040
	.set sub_080b5078, 0x080b5078
	.set sub_080b5088, 0x080b5088
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080c9048, 0x080c9048
	.set sub_080cd104, 0x080cd104
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080cef64, 0x080cef64
	.set sub_080d655c, 0x080d655c
	.set sub_080d6750, 0x080d6750
	.set sub_080d67dc, 0x080d67dc
	.set sub_080d6888, 0x080d6888
	.set sub_080dbb24, 0x080dbb24
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e38b8, 0x080e38b8
	.set sub_080e3908, 0x080e3908
	.set sub_080e3944, 0x080e3944
	.set sub_080e396c, 0x080e396c
	.set sub_080e3980, 0x080e3980
	.set sub_080e46f0, 0x080e46f0
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Func_080e823c
	.thumb_func
Func_080e823c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #100]
	ldr	r1, [r3, #0]
	sub	sp, #84
	str	r1, [sp, #48]
	subs	r2, r3, #4
	ldr	r2, [r2, #0]
	str	r2, [sp, #44]
	ldr	r3, [r3, #4]
	str	r3, [sp, #36]
	ldr	r3, [pc, #84]
	adds	r5, r2, r3
	str	r0, [r5, #0]
	movs	r0, #0
	bl	sub_080cd594
	bl	sub_080c9048
	ldr	r2, [pc, #60]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r6, [sp, #44]
	movs	r7, #239
	lsls	r7, r7, #7
	adds	r2, r6, r7
	movs	r3, #0
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #44]
	bl	sub_080041d8
	movs	r1, #0
	movs	r0, #1
	bl	sub_080cd104
	ldr	r0, [r5, #0]
	bl	sub_080d6750
	ldr	r1, [pc, #28]
	movs	r0, #9
	movs	r2, #2
	bl	sub_080dbb24
	movs	r1, #13
	b.n	.L_080e82c0
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001ef0
	.4byte 0x00007828
	.4byte 0x080cd261
	.2byte 0x017b
	.2byte 0x0000
.L_080e82c0:
	movs	r0, #0
	negs	r1, r1
	ldr	r6, [pc, #196]
	mov	r8, r0
	adds	r7, r1, #0
.L_080e82ca:
	movs	r0, #195
	lsls	r0, r0, #1
	bl	sub_08009030
	ldr	r2, [sp, #44]
	adds	r5, r0, #0
	str	r5, [r6, r2]
	cmp	r5, #0
	beq.n	.L_080e8302
	adds	r2, r5, #0
	adds	r2, #38
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #3
	mov	r0, r8
	bl	sub_080022fc
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_08009020
	ldr	r3, [sp, #44]
	ldr	r1, [r6, r3]
	ldrb	r3, [r1, #9]
	movs	r2, #4
	ands	r3, r7
	orrs	r3, r2
	strb	r3, [r1, #9]
.L_080e8302:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r6, #4
	cmp	r1, #6
	bne.n	.L_080e82ca
	movs	r6, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r6, [sp, #0]
	bl	sub_080ed408
	ldr	r5, [pc, #112]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r1, #7
	str	r3, [sp, #60]
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	str	r3, [sp, #0]
	bl	sub_080ed408
	adds	r5, #188
	mov	r2, sp
	ldr	r3, [r5, #0]
	adds	r2, #60
	str	r2, [sp, #16]
	str	r3, [r2, #4]
	ldr	r2, [pc, #80]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	subs	r2, #8
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, #6
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r2, #0
	ldr	r1, [pc, #56]
	movs	r0, #1
	bl	sub_080b5040
	movs	r0, #1
	movs	r1, #1
	bl	sub_080cd104
	ldr	r0, [pc, #44]
	ldr	r1, [sp, #36]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [pc, #36]
	ldr	r1, [sp, #44]
	b.n	.L_080e83a4
	movs	r0, r0
	.4byte 0x00002737
	.4byte 0x000000f0
	.4byte 0x00001088
	.4byte 0x000077fc
	.4byte 0x03001e50
	.4byte 0x04000048
	.4byte 0x0000003c
	.4byte 0x00000073
	.2byte 0x00c0
	.2byte 0x0000
.L_080e83a4:
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r7, [sp, #44]
	movs	r0, #239
	lsls	r0, r0, #7
	ldr	r1, [pc, #36]
	adds	r3, r7, r0
	str	r6, [r3, #0]
	adds	r2, r7, r1
	movs	r3, #50
	str	r3, [r2, #0]
	movs	r2, #188
	movs	r3, #184
	lsls	r3, r3, #15
	lsls	r2, r2, #16
	movs	r6, #160
	b.n	.L_080e83f8
	.4byte 0x00007741
	.4byte 0x00000080
	.4byte 0x00001010
	.4byte 0x00003f44
	.2byte 0x7784
	.2byte 0x0000
.L_080e83f8:
	ldr	r0, [sp, #44]
	movs	r1, #225
	lsls	r6, r6, #16
	movs	r7, #0
	lsls	r1, r1, #7
	str	r2, [sp, #28]
	str	r3, [sp, #32]
	str	r3, [sp, #24]
	str	r6, [sp, #20]
	mov	r8, r7
	movs	r6, #0
	adds	r5, r0, r1
.L_080e8410:
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	movs	r3, #1
	ldr	r2, [pc, #788]
	add	r8, r3
	mov	r0, r8
	str	r7, [r5, #4]
	str	r6, [r5, #12]
	str	r6, [r5, #16]
	str	r6, [r5, #24]
	adds	r7, r7, r2
	adds	r5, #28
	cmp	r0, #6
	bne.n	.L_080e8410
	ldr	r6, [sp, #44]
	ldr	r7, [pc, #768]
	movs	r1, #0
	mov	r8, r1
	movs	r2, #24
	adds	r3, r6, r7
.L_080e8440:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r1, #58
	bne.n	.L_080e8440
	movs	r2, #0
	mov	r8, r2
	movs	r1, #1
	movs	r2, #128
	ldr	r3, [pc, #740]
	negs	r1, r1
	lsls	r2, r2, #3
.L_080e845c:
	movs	r6, #1
	add	r8, r6
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_080e845c
	ldr	r7, [sp, #44]
	ldr	r0, [pc, #724]
	ldr	r1, [pc, #724]
	adds	r2, r7, r0
	movs	r3, #24
	str	r3, [r2, #0]
	movs	r3, #0
	adds	r2, r7, r1
	str	r3, [r2, #0]
	str	r3, [sp, #40]
	ldr	r3, [pc, #712]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080e848a
	b.n	.L_080e8968
.L_080e848a:
	mov	r2, sp
	mov	r3, sp
	adds	r2, #68
	adds	r3, #52
	str	r2, [sp, #8]
	str	r3, [sp, #12]
.L_080e8496:
	ldr	r6, [sp, #40]
	cmp	r6, #94
	bne.n	.L_080e84a2
	movs	r0, #156
	bl	sub_080f9010
.L_080e84a2:
	ldr	r7, [sp, #40]
	cmp	r7, #136
	bne.n	.L_080e84ae
	movs	r0, #156
	bl	sub_080f9010
.L_080e84ae:
	ldr	r0, [sp, #40]
	cmp	r0, #178
	bne.n	.L_080e84ba
	movs	r0, #156
	bl	sub_080f9010
.L_080e84ba:
	movs	r2, #130
	ldr	r1, [sp, #40]
	lsls	r2, r2, #1
	cmp	r1, r2
	bne.n	.L_080e84ca
	movs	r0, #145
	bl	sub_080f9010
.L_080e84ca:
	ldr	r3, [pc, #640]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #52]
	str	r4, [sp, #56]
	ldr	r3, [sp, #40]
	subs	r3, #96
	cmp	r3, #155
	bhi.n	.L_080e84e4
	ldr	r3, [sp, #44]
	ldr	r6, [pc, #624]
	adds	r2, r3, r6
	b.n	.L_080e84f4
.L_080e84e4:
	ldr	r7, [sp, #40]
	ldr	r0, [pc, #620]
	adds	r3, r7, r0
	cmp	r3, #3
	bhi.n	.L_080e84f8
	ldr	r1, [sp, #44]
	ldr	r3, [pc, #604]
	adds	r2, r1, r3
.L_080e84f4:
	movs	r3, #1
	str	r3, [r2, #0]
.L_080e84f8:
	movs	r3, #0
	ldr	r0, [sp, #44]
	ldr	r1, [pc, #600]
	str	r3, [sp, #80]
	str	r3, [sp, #72]
	ldr	r5, [sp, #8]
	ldr	r7, [pc, #596]
	mov	r8, r3
	adds	r6, r0, r1
.L_080e850a:
	ldr	r3, [pc, #596]
	mov	r2, r8
	ldrb	r3, [r3, r2]
	ldr	r0, [sp, #28]
	lsls	r3, r3, #16
	adds	r3, r3, r0
	adds	r3, r3, r7
	str	r3, [r5, #0]
	ldr	r3, [pc, #584]
	ldrb	r3, [r3, r2]
	ldr	r1, [sp, #32]
	lsls	r3, r3, #16
	adds	r3, r3, r1
	adds	r3, r3, r7
	str	r3, [r5, #8]
	ldr	r2, [sp, #12]
	movs	r3, #0
	ldmia	r6!, {r0}
	adds	r1, r5, #0
	bl	sub_08009008
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #7
	bne.n	.L_080e850a
	ldr	r6, [sp, #40]
	cmp	r6, #90
	bgt.n	.L_080e8566
	lsls	r5, r6, #9
	adds	r0, r5, #0
	bl	sub_08002322
	movs	r7, #156
	lsls	r0, r0, #4
	lsls	r7, r7, #16
	adds	r7, r0, r7
	adds	r0, r5, #0
	str	r7, [sp, #20]
	bl	sub_0800231c
	movs	r1, #184
	lsls	r0, r0, #4
	lsls	r1, r1, #15
	adds	r1, r0, r1
	str	r1, [sp, #24]
.L_080e8566:
	ldr	r2, [sp, #40]
	cmp	r2, #196
	bgt.n	.L_080e8600
	ldr	r7, [sp, #44]
	movs	r3, #0
	mov	r8, r3
	movs	r6, #91
	mov	sl, r7
.L_080e8576:
	ldr	r0, [sp, #40]
	cmp	r0, r6
	blt.n	.L_080e858c
	adds	r3, r6, #4
	cmp	r0, r3
	bge.n	.L_080e858c
	ldr	r1, [sp, #24]
	movs	r2, #128
	lsls	r2, r2, #12
	adds	r2, r1, r2
	str	r2, [sp, #24]
.L_080e858c:
	ldr	r7, [sp, #40]
	adds	r3, r6, #3
	cmp	r7, r3
	bne.n	.L_080e85d8
	ldr	r5, [pc, #464]
	movs	r0, #255
	movs	r7, #0
	mov	r9, r0
	add	r5, sl
.L_080e859e:
	movs	r3, #128
	lsls	r3, r3, #15
	str	r3, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	bl	sub_08004458
	mov	r1, r9
	ands	r0, r1
	subs	r0, #127
	lsls	r0, r0, #10
	str	r0, [r5, #12]
	bl	sub_08004458
	mov	r2, r9
	ands	r0, r2
	subs	r0, #127
	lsls	r0, r0, #10
	str	r0, [r5, #16]
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r7, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #4
	bne.n	.L_080e859e
.L_080e85d8:
	adds	r3, r6, #0
	ldr	r7, [sp, #40]
	adds	r3, #20
	cmp	r7, r3
	blt.n	.L_080e85f0
	adds	r3, #16
	cmp	r7, r3
	bge.n	.L_080e85f0
	ldr	r0, [sp, #24]
	ldr	r1, [pc, #384]
	adds	r1, r0, r1
	str	r1, [sp, #24]
.L_080e85f0:
	movs	r3, #1
	add	r8, r3
	movs	r2, #224
	mov	r7, r8
	adds	r6, #40
	add	sl, r2
	cmp	r7, #3
	bne.n	.L_080e8576
.L_080e8600:
	ldr	r3, [sp, #40]
	subs	r3, #244
	cmp	r3, #7
	bhi.n	.L_080e8610
	ldr	r0, [sp, #20]
	ldr	r1, [pc, #356]
	adds	r1, r0, r1
	str	r1, [sp, #20]
.L_080e8610:
	ldr	r3, [sp, #40]
	subs	r3, #252
	cmp	r3, #23
	bhi.n	.L_080e8624
	ldr	r3, [sp, #40]
	ldr	r2, [sp, #20]
	subs	r3, #250
	lsls	r3, r3, #16
	subs	r3, r2, r3
	str	r3, [sp, #20]
.L_080e8624:
	ldr	r3, [sp, #40]
	ldr	r6, [pc, #332]
	cmp	r3, r6
	bgt.n	.L_080e866e
	ldr	r0, [sp, #24]
	movs	r3, #255
	ldr	r1, [sp, #44]
	lsls	r3, r3, #24
	ldr	r2, [pc, #320]
	str	r3, [sp, #72]
	adds	r3, r0, r3
	str	r3, [sp, #76]
	adds	r3, r1, r2
	ldr	r7, [sp, #20]
	ldr	r0, [r3, #0]
	add	r3, sp, #68
	adds	r1, r3, #0
	ldr	r2, [sp, #12]
	movs	r3, #0
	str	r7, [sp, #68]
	bl	sub_08009008
	movs	r7, #128
	ldr	r6, [sp, #20]
	ldr	r0, [sp, #44]
	ldr	r1, [pc, #292]
	lsls	r7, r7, #14
	adds	r3, r6, r7
	add	r2, sp, #68
	str	r3, [sp, #68]
	adds	r3, r0, r1
	ldr	r0, [r3, #0]
	adds	r1, r2, #0
	movs	r3, #0
	ldr	r2, [sp, #12]
	bl	sub_08009008
.L_080e866e:
	ldr	r6, [sp, #8]
	movs	r3, #0
	str	r3, [r6, #4]
	ldr	r7, [sp, #44]
	movs	r0, #225
	lsls	r0, r0, #7
	mov	r8, r3
	mov	fp, r6
	adds	r5, r7, r0
	mov	r9, r7
.L_080e8682:
	ldr	r3, [r5, #24]
	cmp	r3, #2
	bne.n	.L_080e868a
	b.n	.L_080e8796
.L_080e868a:
	ldr	r3, [r5, #0]
	mov	r1, fp
	str	r3, [r1, #0]
	ldr	r3, [r5, #4]
	mov	r2, r8
	str	r3, [r1, #8]
	ldr	r6, [pc, #232]
	lsls	r3, r2, #2
	ldr	r7, [sp, #44]
	adds	r3, r3, r6
	ldr	r0, [r7, r3]
	ldr	r2, [sp, #12]
	movs	r3, #0
	bl	sub_08009008
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r0, [sp, #40]
	cmp	r0, #96
	ble.n	.L_080e86c6
	movs	r1, #128
	lsls	r1, r1, #7
	adds	r3, r2, r1
	str	r3, [r5, #16]
.L_080e86c6:
	movs	r2, #240
	ldr	r3, [r5, #4]
	lsls	r2, r2, #15
	cmp	r3, r2
	ble.n	.L_080e8796
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #1
	bne.n	.L_080e8788
	ldr	r3, [r5, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	ldr	r6, [pc, #156]
	str	r3, [r5, #16]
	movs	r3, #255
	movs	r7, #0
	mov	sl, r3
	add	r6, r9
.L_080e86f0:
	ldr	r3, [r5, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #0]
	ldr	r0, [pc, #96]
	ldr	r3, [r5, #4]
	adds	r3, r3, r0
	str	r3, [r6, #4]
	bl	sub_08004458
	mov	r1, sl
	ands	r0, r1
	subs	r0, #127
	lsls	r0, r0, #10
	str	r0, [r6, #12]
	bl	sub_08004458
	mov	r2, sl
	ands	r0, r2
	subs	r0, #127
	lsls	r0, r0, #10
	str	r0, [r6, #16]
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r7, #1
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r7, #2
	bne.n	.L_080e86f0
	b.n	.L_080e8796
	movs	r0, r0
	.4byte 0xfff00000
	.4byte 0x00007140
	.4byte 0x02010018
	.4byte 0x000077b4
	.4byte 0x000077b8
	.4byte 0x03001b04
	.4byte 0x080edac8
	.4byte 0x000077a8
	.4byte 0xfffffefc
	.4byte 0x000077d8
	.4byte 0xffe00000
	.4byte 0x080eeed8
	.4byte 0x080eeee1
	.4byte 0x00007128
	.4byte 0xfffe0000
	.4byte 0xffff0000
	.4byte 0x00000103
	.4byte 0x000077f4
	.4byte 0x000077f8
	.4byte 0x000077fc
	.2byte 0x73c8
	.2byte 0x0000
.L_080e8788:
	ldr	r3, [sp, #40]
	cmp	r3, #199
	bgt.n	.L_080e8796
	movs	r3, #0
	str	r3, [r5, #4]
	str	r3, [r5, #16]
	str	r3, [r5, #24]
.L_080e8796:
	movs	r7, #1
	add	r8, r7
	movs	r6, #56
	mov	r0, r8
	adds	r5, #28
	add	r9, r6
	cmp	r0, #6
	beq.n	.L_080e87a8
	b.n	.L_080e8682
.L_080e87a8:
	ldr	r2, [sp, #44]
	ldr	r3, [pc, #520]
	movs	r1, #0
	mov	r8, r1
	adds	r5, r2, r3
.L_080e87b2:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_080e87fc
	cmp	r0, #23
	bhi.n	.L_080e87ec
	movs	r1, #6
	bl	sub_080022ec
	ldr	r3, [pc, #500]
	adds	r0, #3
	lsls	r0, r0, #1
	ldrh	r1, [r3, r0]
	ldr	r3, [pc, #496]
	ldr	r6, [sp, #44]
	ldrh	r4, [r3, r0]
	movs	r7, #2
	ldrsh	r2, [r5, r7]
	adds	r1, r6, r1
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	lsrs	r0, r4, #1
	subs	r2, r2, r0
	subs	r3, r3, r0
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #60]
	bl	sub_080072f4
.L_080e87ec:
	adds	r0, r5, #0
	movs	r1, #60
	ldr	r2, [pc, #460]
	bl	sub_080e3908
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_080e87fc:
	movs	r7, #1
	add	r8, r7
	mov	r0, r8
	adds	r5, #28
	cmp	r0, #56
	bne.n	.L_080e87b2
	movs	r2, #130
	ldr	r1, [sp, #40]
	lsls	r2, r2, #1
	cmp	r1, r2
	bne.n	.L_080e88c8
	movs	r3, #0
	mov	r8, r3
	ldr	r6, [sp, #44]
	ldr	r3, [pc, #424]
	ldr	r3, [r6, r3]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_080e8854
	ldr	r7, [pc, #416]
	adds	r5, r6, r7
	movs	r6, #36
.L_080e8828:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r1, #4
	bl	sub_080b5088
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #8
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #7
	mov	r3, r8
	negs	r2, r2
	bl	sub_080d6888
	movs	r3, #1
	add	r8, r3
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	adds	r6, #2
	cmp	r8, r3
	bne.n	.L_080e8828
.L_080e8854:
	ldr	r6, [sp, #44]
	ldr	r7, [pc, #368]
	movs	r3, #8
	adds	r2, r6, r7
	str	r3, [r2, #0]
	movs	r1, #130
	ldr	r0, [sp, #40]
	lsls	r1, r1, #1
	cmp	r0, r1
	bne.n	.L_080e88c8
	movs	r2, #0
	ldr	r7, [pc, #352]
	mov	r8, r2
.L_080e886e:
	bl	sub_08004458
	ldr	r5, [pc, #348]
	ands	r5, r0
	bl	sub_08004458
	ldr	r3, [pc, #344]
	adds	r6, r0, #0
	ands	r6, r3
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r7, #0]
	movs	r3, #184
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	adds	r0, r6, #0
	bl	sub_08002322
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #7
	str	r3, [r7, #16]
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r3, #32
	str	r3, [r7, #24]
	movs	r6, #128
	movs	r3, #1
	add	r8, r3
	lsls	r6, r6, #2
	adds	r7, #28
	cmp	r8, r6
	bne.n	.L_080e886e
.L_080e88c8:
	ldr	r0, [pc, #268]
	movs	r7, #0
	ldr	r6, [pc, #252]
	mov	r8, r7
	mov	sl, r0
.L_080e88d2:
	ldr	r0, [r6, #24]
	cmp	r0, #0
	blt.n	.L_080e8922
	asrs	r0, r0, #3
	adds	r0, #1
	lsls	r5, r0, #1
	mov	r1, r8
	subs	r3, r5, #2
	mov	r2, sl
	movs	r4, #1
	ands	r4, r1
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #36]
	adds	r1, r3, r1
	lsrs	r3, r0, #31
	movs	r7, #2
	ldrsh	r2, [r6, r7]
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r6, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r5, [sp, #4]
	ldr	r0, [sp, #16]
	lsls	r4, r4, #2
	ldr	r4, [r4, r0]
	ldr	r0, [sp, #48]
	bl	sub_080072f4
	movs	r2, #128
	adds	r0, r6, #0
	movs	r1, #62
	lsls	r2, r2, #5
	bl	sub_080e3908
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L_080e8922:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #2
	adds	r6, #28
	cmp	r8, r2
	bne.n	.L_080e88d2
	movs	r0, #8
	movs	r1, #8
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r6, [pc, #156]
	ldr	r3, [sp, #44]
	adds	r2, r3, r6
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_080030f8
	ldr	r7, [sp, #40]
	movs	r0, #160
	adds	r7, #1
	lsls	r0, r0, #1
	str	r7, [sp, #40]
	cmp	r7, r0
	beq.n	.L_080e8968
	ldr	r3, [pc, #132]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080e8968
	b.n	.L_080e8496
.L_080e8968:
	movs	r0, #134
	bl	sub_080b50e8
	bl	sub_080d67dc
	ldr	r3, [pc, #112]
	ldr	r2, [sp, #44]
	movs	r1, #0
	mov	r8, r1
	adds	r5, r2, r3
.L_080e897c:
	movs	r6, #1
	add	r8, r6
	ldmia	r5!, {r0}
	mov	r7, r8
	bl	sub_08009038
	cmp	r7, #15
	bne.n	.L_080e897c
	ldr	r0, [pc, #88]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #84
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00007128
	.4byte 0x080eeeea
	.4byte 0x080eeef8
	.4byte 0xffffc000
	.4byte 0x00007828
	.4byte 0x000077a8
	.4byte 0x02010000
	.4byte 0x000003ff
	.4byte 0x0000ffff
	.4byte 0x080ede48
	.4byte 0x00007824
	.4byte 0x03001b04
	.4byte 0x000077d8
	.2byte 0xd261
	.2byte 0x080c
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #92]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #228
	str	r3, [sp, #44]
	movs	r3, #0
	str	r3, [sp, #36]
	str	r3, [sp, #28]
	str	r3, [sp, #24]
	ldr	r3, [pc, #76]
	mov	r9, r1
	ldr	r2, [r2, #8]
	add	r3, r9
	str	r2, [sp, #20]
	str	r0, [r3, #0]
	movs	r0, #0
	bl	sub_080cd594
	ldr	r2, [pc, #60]
	ldr	r3, [pc, #48]
	ldr	r0, [pc, #60]
	strh	r3, [r2, #0]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r5, #234
	ldr	r0, [pc, #48]
	movs	r1, #180
	lsls	r5, r5, #2
	lsls	r1, r1, #5
	mov	r2, r9
	movs	r7, #1
	mov	lr, r0
	mov	ip, r1
	adds	r4, r2, r5
.L_080e8a46:
	mov	r3, lr
	mov	r2, r9
	movs	r6, #0
	lsls	r0, r7, #2
	adds	r1, r4, r3
	add	r2, ip
	b.n	.L_080e8a6c
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000052
	.4byte 0x000000c2
	.2byte 0xf1f0
	.2byte 0xffff
.L_080e8a6c:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	cmp	r7, #10
	ble.n	.L_080e8a80
	subs	r3, r3, r0
	adds	r3, #40
	cmp	r3, #0
	bge.n	.L_080e8a7e
	movs	r3, #0
.L_080e8a7e:
	strb	r3, [r1, #0]
.L_080e8a80:
	adds	r6, #1
	adds	r1, #1
	cmp	r6, r5
	bne.n	.L_080e8a6c
	movs	r0, #234
	lsls	r0, r0, #2
	adds	r7, #1
	adds	r4, r4, r0
	cmp	r7, #20
	bne.n	.L_080e8a46
	ldr	r1, [sp, #20]
	ldr	r0, [pc, #840]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	movs	r1, #240
	lsls	r1, r1, #6
	ldr	r0, [pc, #828]
	add	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r2, #1
	ldr	r0, [pc, #820]
	ldr	r1, [pc, #820]
	movs	r3, #0
	bl	sub_080e0524
	ldr	r3, [pc, #816]
	add	r3, r9
	ldr	r3, [r3, #0]
	mov	r1, sp
	ldr	r0, [r3, #4]
	adds	r1, #48
	str	r1, [sp, #16]
	bl	sub_080cef64
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #792]
	movs	r3, #75
	add	r2, r9
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #788]
	lsls	r1, r1, #3
	bl	sub_080041d8
	ldr	r5, [pc, #784]
	movs	r6, #0
	movs	r7, #63
	add	r5, r9
.L_080e8af2:
	ldr	r3, [pc, #764]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080e8b02
	ldr	r3, [pc, #768]
	b.n	.L_080e8b06
.L_080e8b02:
	movs	r3, #224
	lsls	r3, r3, #14
.L_080e8b06:
	str	r3, [r5, #0]
	movs	r3, #0
	str	r3, [r5, #4]
	str	r3, [r5, #8]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r7
	lsls	r0, r0, #13
	str	r0, [r5, #16]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #32
	lsls	r0, r0, #14
	movs	r3, #1
	adds	r6, #1
	str	r0, [r5, #20]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r6, #40
	bne.n	.L_080e8af2
	ldr	r5, [pc, #708]
	movs	r6, #0
	mov	r8, r6
	movs	r7, #63
	add	r5, r9
.L_080e8b46:
	ldr	r3, [pc, #680]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080e8b56
	ldr	r3, [pc, #684]
	b.n	.L_080e8b5a
.L_080e8b56:
	movs	r3, #224
	lsls	r3, r3, #14
.L_080e8b5a:
	str	r3, [r5, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	mov	r2, r8
	str	r3, [r5, #4]
	str	r2, [r5, #8]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r7
	lsls	r0, r0, #12
	str	r0, [r5, #16]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #32
	lsls	r0, r0, #14
	mov	r3, r8
	adds	r6, #1
	str	r0, [r5, #20]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r6, #16
	bne.n	.L_080e8b46
	movs	r5, #225
	lsls	r5, r5, #7
	ldr	r6, [pc, #620]
	movs	r7, #0
	add	r5, r9
.L_080e8b9e:
	ldr	r3, [pc, #592]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080e8bbc
	adds	r0, r6, #0
	bl	sub_08002322
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	asrs	r3, r3, #16
	adds	r3, #88
	b.n	.L_080e8bce
.L_080e8bbc:
	adds	r0, r6, #0
	bl	sub_08002322
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #3
	negs	r3, r3
	asrs	r3, r3, #16
	adds	r3, #16
.L_080e8bce:
	str	r3, [r5, #0]
	adds	r0, r6, #0
	bl	sub_0800231c
	lsls	r0, r0, #4
	asrs	r0, r0, #16
	adds	r0, #40
	str	r0, [r5, #4]
	lsls	r3, r7, #1
	movs	r0, #128
	negs	r3, r3
	lsls	r0, r0, #5
	adds	r7, #1
	str	r3, [r5, #24]
	adds	r6, r6, r0
	adds	r5, #28
	cmp	r7, #8
	bne.n	.L_080e8b9e
	ldr	r0, [pc, #536]
	bl	sub_08002f40
	ldr	r2, [pc, #500]
	movs	r1, #0
	add	r2, r9
	str	r0, [sp, #32]
	str	r1, [sp, #40]
	str	r2, [sp, #12]
.L_080e8c04:
	ldr	r3, [pc, #520]
	ldr	r3, [r3, #0]
	mov	fp, r3
	ldr	r3, [sp, #40]
	cmp	r3, #83
	bne.n	.L_080e8c16
	movs	r0, #134
	bl	sub_080b50e8
.L_080e8c16:
	ldr	r0, [sp, #40]
	cmp	r0, #0
	bne.n	.L_080e8c22
	movs	r0, #136
	bl	sub_080f9010
.L_080e8c22:
	ldr	r1, [sp, #40]
	cmp	r1, #50
	bne.n	.L_080e8c2e
	movs	r0, #136
	bl	sub_080f9010
.L_080e8c2e:
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080e8c4a
	ldr	r3, [sp, #40]
	cmp	r3, #63
	bgt.n	.L_080e8c5e
	mov	r0, fp
	ldrh	r3, [r0, #54]
	ldr	r1, [pc, #464]
	mov	r2, fp
	adds	r3, r3, r1
	b.n	.L_080e8c5c
.L_080e8c4a:
	ldr	r3, [sp, #40]
	cmp	r3, #63
	bgt.n	.L_080e8c5e
	mov	r0, fp
	ldrh	r3, [r0, #54]
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r3, r3, r1
	mov	r2, fp
.L_080e8c5c:
	strh	r3, [r2, #54]
.L_080e8c5e:
	movs	r3, #100
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl	sub_080b5028
	ldr	r3, [sp, #40]
	cmp	r3, #17
	bgt.n	.L_080e8cce
	adds	r0, r3, #0
	movs	r1, #3
	bl	sub_080022ec
	ldr	r2, [pc, #412]
	adds	r5, r0, #0
	lsls	r0, r5, #1
	ldrh	r1, [r2, r0]
	mov	sl, r0
	movs	r3, #240
	ldr	r0, [pc, #404]
	lsls	r3, r3, #6
	mov	r8, r3
	ldrb	r3, [r0, r5]
	ldr	r0, [pc, #400]
	ldrb	r2, [r0, r5]
	ldr	r6, [pc, #400]
	str	r2, [sp, #0]
	ldrb	r2, [r6, r5]
	add	r1, r9
	add	r1, r8
	adds	r3, #60
	str	r2, [sp, #4]
	ldr	r4, [sp, #48]
	movs	r2, #48
	ldr	r0, [sp, #44]
	bl	sub_080072f4
	ldr	r2, [pc, #364]
	ldr	r0, [pc, #368]
	mov	r3, sl
	ldrh	r1, [r2, r3]
	ldrb	r3, [r0, r5]
	ldr	r0, [pc, #364]
	ldrb	r2, [r0, r5]
	str	r2, [sp, #0]
	ldrb	r2, [r6, r5]
	str	r2, [sp, #4]
	ldr	r2, [sp, #16]
	add	r1, r9
	ldr	r4, [r2, #4]
	add	r1, r8
	adds	r3, #60
	ldr	r0, [sp, #44]
	movs	r2, #56
	bl	sub_080072f4
.L_080e8cce:
	ldr	r3, [sp, #40]
	subs	r3, #18
	str	r3, [sp, #8]
	cmp	r3, #40
	bhi.n	.L_080e8d1a
	ldr	r0, [sp, #40]
	cmp	r0, #18
	bne.n	.L_080e8d00
	ldr	r1, [sp, #32]
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	ldrb	r2, [r1, #1]
	lsls	r3, r3, #8
	adds	r3, r3, r2
	str	r3, [sp, #28]
	movs	r3, #2
	ldrsb	r3, [r1, r3]
	ldrb	r2, [r1, #3]
	lsls	r3, r3, #8
	adds	r3, r3, r2
	adds	r3, #16
	adds	r1, #4
	str	r3, [sp, #24]
	str	r1, [sp, #32]
	b.n	.L_080e8d1a
.L_080e8d00:
	ldr	r2, [sp, #32]
	ldr	r0, [sp, #28]
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	adds	r0, r0, r3
	str	r0, [sp, #28]
	ldr	r1, [sp, #24]
	movs	r3, #1
	ldrsb	r3, [r2, r3]
	adds	r2, #2
	adds	r1, r1, r3
	str	r1, [sp, #24]
	str	r2, [sp, #32]
.L_080e8d1a:
	ldr	r3, [sp, #40]
	subs	r3, #78
	cmp	r3, #40
	bhi.n	.L_080e8d3a
	ldr	r2, [sp, #40]
	cmp	r2, #78
	bne.n	.L_080e8d34
	movs	r3, #56
	negs	r3, r3
	movs	r0, #48
	str	r3, [sp, #28]
	str	r0, [sp, #24]
	b.n	.L_080e8d3a
.L_080e8d34:
	ldr	r1, [sp, #24]
	subs	r1, #16
	str	r1, [sp, #24]
.L_080e8d3a:
	movs	r2, #24
	movs	r3, #39
	movs	r6, #19
	mov	sl, r2
	mov	r8, r3
	movs	r7, #156
.L_080e8d46:
	adds	r3, r6, #0
	ldr	r0, [sp, #40]
	adds	r3, #18
	cmp	r0, r3
	ble.n	.L_080e8daa
	adds	r3, #65
	cmp	r0, r3
	bgt.n	.L_080e8daa
	lsls	r0, r6, #3
	adds	r3, r0, #0
	add	r2, sp, #68
	subs	r3, #8
	ldr	r3, [r2, r3]
	str	r3, [r2, r0]
	subs	r3, r0, #4
	ldr	r5, [r2, r3]
	str	r5, [r2, r7]
	cmp	r6, #10
	ble.n	.L_080e8d90
	movs	r3, #234
	lsls	r3, r3, #2
	adds	r1, r6, #0
	muls	r1, r3
	ldr	r3, [pc, #176]
	add	r1, r9
	ldr	r2, [r2, r0]
	adds	r1, r1, r3
	mov	r0, sl
	mov	r3, r8
	str	r0, [sp, #0]
	str	r3, [sp, #4]
	ldr	r4, [sp, #48]
	ldr	r0, [sp, #44]
	adds	r3, r5, #0
	bl	sub_080072f4
	b.n	.L_080e8daa
.L_080e8d90:
	mov	r1, r8
	ldr	r2, [r2, r0]
	str	r1, [sp, #4]
	movs	r1, #180
	mov	r0, sl
	lsls	r1, r1, #5
	str	r0, [sp, #0]
	ldr	r4, [sp, #48]
	ldr	r0, [sp, #44]
	add	r1, r9
	adds	r3, r5, #0
	bl	sub_080072f4
.L_080e8daa:
	subs	r6, #1
	subs	r7, #8
	cmp	r6, #0
	bne.n	.L_080e8d46
	bl	sub_080049ac
	mov	r1, fp
	adds	r1, #12
	mov	r0, fp
	bl	sub_080051d8
	ldr	r2, [sp, #8]
	cmp	r2, #65
	bhi.n	.L_080e8eb4
	ldr	r0, [sp, #12]
	ldr	r3, [r0, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080e8e2c
	ldr	r3, [sp, #28]
	lsrs	r2, r3, #31
	adds	r2, r3, r2
	asrs	r2, r2, #1
	movs	r3, #64
	add	r1, sp, #56
	subs	r3, r3, r2
	b.n	.L_080e8e38
	.4byte 0x00000073
	.4byte 0x000000b4
	.4byte 0x0000007d
	.4byte 0x02010000
	.4byte 0x00007828
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007160
	.4byte 0xffc80000
	.4byte 0x000075c0
	.4byte 0xffffc000
	.4byte 0x000000d3
	.4byte 0x03001e80
	.4byte 0xffffff00
	.4byte 0x080edeb2
	.4byte 0x080edeab
	.4byte 0x080ede9f
	.4byte 0x080edea5
	.2byte 0xf1f0
	.2byte 0xffff
.L_080e8e2c:
	ldr	r0, [sp, #28]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	add	r1, sp, #56
	adds	r3, #64
.L_080e8e38:
	str	r3, [r1, #0]
	ldr	r2, [sp, #24]
	movs	r3, #60
	subs	r3, r3, r2
	str	r3, [r1, #4]
	add	r4, sp, #68
	ldr	r2, [r4, #4]
	subs	r3, r3, r2
	subs	r3, #24
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r0, r3, #1
	cmp	r0, #2
	ble.n	.L_080e8e56
	movs	r0, #2
.L_080e8e56:
	movs	r3, #2
	negs	r3, r3
	cmp	r0, r3
	bge.n	.L_080e8e62
	movs	r0, #2
	negs	r0, r0
.L_080e8e62:
	ldr	r2, [sp, #36]
	adds	r2, r2, r0
	str	r2, [sp, #36]
	cmp	r2, #8
	ble.n	.L_080e8e70
	movs	r3, #8
	str	r3, [sp, #36]
.L_080e8e70:
	movs	r2, #8
	ldr	r0, [sp, #36]
	negs	r2, r2
	cmp	r0, r2
	bge.n	.L_080e8e7c
	str	r2, [sp, #36]
.L_080e8e7c:
	ldr	r3, [sp, #36]
	cmp	r3, #0
	bge.n	.L_080e8e84
	adds	r3, #3
.L_080e8e84:
	ldr	r2, [r1, #0]
	asrs	r3, r3, #2
	adds	r0, r3, #2
	adds	r3, r2, #0
	subs	r3, #12
	str	r3, [r4, #0]
	ldr	r3, [r1, #4]
	adds	r1, r3, #0
	subs	r1, #20
	str	r1, [r4, #4]
	lsls	r1, r0, #3
	adds	r1, r1, r0
	movs	r0, #24
	lsls	r1, r1, #7
	str	r0, [sp, #0]
	movs	r0, #48
	str	r0, [sp, #4]
	add	r1, r9
	subs	r2, #18
	subs	r3, #22
	ldr	r4, [sp, #48]
	ldr	r0, [sp, #44]
	bl	sub_080072f4
.L_080e8eb4:
	ldr	r3, [sp, #40]
	cmp	r3, #83
	bne.n	.L_080e8ee4
	ldr	r3, [pc, #444]
	movs	r2, #8
	add	r3, r9
	str	r2, [r3, #0]
	ldr	r0, [sp, #12]
	ldr	r3, [r0, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r1, #7
	str	r2, [sp, #0]
	movs	r3, #0
	movs	r2, #5
	bl	sub_080d6888
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_080b5088
.L_080e8ee4:
	ldr	r2, [sp, #40]
	cmp	r2, #83
	ble.n	.L_080e8fac
	ldr	r6, [pc, #400]
	movs	r7, #0
	add	r6, r9
.L_080e8ef0:
	ldr	r3, [r6, #4]
	cmp	r3, #0
	blt.n	.L_080e8fa4
	add	r5, sp, #56
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080e3944
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	ldr	r3, [r5, #8]
	cmp	r3, #159
	bgt.n	.L_080e8f10
	movs	r3, #160
	str	r3, [r5, #8]
.L_080e8f10:
	ldr	r2, [pc, #364]
	cmp	r3, r2
	ble.n	.L_080e8f1a
	str	r2, [r5, #8]
	adds	r3, r2, #0
.L_080e8f1a:
	adds	r2, r3, #0
	subs	r2, #160
	cmp	r2, #0
	bge.n	.L_080e8f24
	adds	r2, #63
.L_080e8f24:
	asrs	r2, r2, #6
	movs	r3, #9
	subs	r4, r3, r2
	cmp	r7, #47
	ble.n	.L_080e8f60
	ldr	r3, [r6, #24]
	cmp	r3, #11
	bgt.n	.L_080e8f86
	lsrs	r1, r3, #31
	adds	r1, r3, r1
	asrs	r1, r1, #1
	ldr	r3, [pc, #328]
	lsls	r1, r1, #11
	ldr	r2, [r5, #0]
	adds	r1, r1, r3
	movs	r0, #32
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	movs	r0, #64
	subs	r3, #32
	str	r0, [sp, #4]
	subs	r2, #16
	ldr	r4, [sp, #48]
	ldr	r0, [sp, #44]
	bl	sub_080072f4
	ldr	r3, [r6, #24]
	adds	r3, #1
	str	r3, [r6, #24]
	b.n	.L_080e8f86
.L_080e8f60:
	lsls	r0, r4, #1
	ldr	r2, [pc, #292]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #20]
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r4, [sp, #0]
	subs	r3, r3, r4
	str	r0, [sp, #4]
	ldr	r4, [sp, #48]
	ldr	r0, [sp, #44]
	bl	sub_080072f4
.L_080e8f86:
	ldr	r3, [r6, #0]
	ldr	r2, [r6, #12]
	adds	r3, r3, r2
	ldr	r1, [r6, #16]
	str	r3, [r6, #0]
	ldr	r3, [r6, #4]
	adds	r3, r3, r1
	str	r3, [r6, #4]
	ldr	r2, [r6, #20]
	ldr	r3, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r6, #8]
	ldr	r3, [pc, #236]
	adds	r1, r1, r3
	str	r1, [r6, #16]
.L_080e8fa4:
	adds	r7, #1
	adds	r6, #28
	cmp	r7, #56
	bne.n	.L_080e8ef0
.L_080e8fac:
	ldr	r0, [sp, #40]
	cmp	r0, #50
	bne.n	.L_080e8fd0
	ldr	r2, [pc, #196]
	movs	r3, #12
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r1, [sp, #12]
	ldr	r3, [r1, #0]
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_080d6888
.L_080e8fd0:
	ldr	r3, [sp, #40]
	cmp	r3, #49
	ble.n	.L_080e902a
	movs	r6, #225
	lsls	r6, r6, #7
	movs	r7, #0
	add	r6, r9
.L_080e8fde:
	ldr	r3, [r6, #24]
	cmp	r3, #11
	bhi.n	.L_080e901e
	lsrs	r4, r3, #31
	adds	r4, r3, r4
	asrs	r4, r4, #1
	ldr	r0, [pc, #164]
	lsls	r3, r4, #1
	ldrh	r1, [r0, r3]
	ldr	r3, [pc, #160]
	movs	r2, #240
	ldrb	r5, [r3, r4]
	lsls	r2, r2, #6
	add	r1, r9
	adds	r1, r1, r2
	ldr	r2, [r6, #0]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #148]
	ldrb	r0, [r3, r4]
	ldr	r3, [r6, #4]
	str	r5, [sp, #0]
	adds	r3, r3, r0
	ldr	r0, [pc, #140]
	ldrb	r0, [r0, r4]
	str	r0, [sp, #4]
	ldr	r0, [sp, #16]
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #44]
	bl	sub_080072f4
	ldr	r3, [r6, #24]
.L_080e901e:
	adds	r3, #1
	adds	r7, #1
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r7, #8
	bne.n	.L_080e8fde
.L_080e902a:
	movs	r1, #8
	movs	r0, #8
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #104]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [sp, #40]
	adds	r1, #1
	str	r1, [sp, #40]
	cmp	r1, #150
	beq.n	.L_080e9050
	b.n	.L_080e8c04
.L_080e9050:
	ldr	r0, [pc, #80]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #228
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x000077a8
	.4byte 0x00007160
	.4byte 0x0000031f
	.4byte 0x02010000
	.4byte 0x080ede48
	.4byte 0xffffe000
	.4byte 0x080edeb2
	.4byte 0x080ede9f
	.4byte 0x080edeab
	.4byte 0x080edea5
	.4byte 0x00007824
	.2byte 0xd261
	.2byte 0x080c
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #428]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #88
	str	r3, [sp, #36]
	ldr	r3, [r2, #8]
	str	r3, [sp, #32]
	subs	r2, #108
	ldr	r5, [pc, #412]
	mov	fp, r1
	ldr	r2, [r2, #0]
	add	r5, fp
	str	r2, [sp, #28]
	ldr	r7, [pc, #408]
	str	r0, [r5, #0]
	movs	r0, #0
	bl	sub_080cd594
	ldr	r0, [pc, #400]
	mov	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	adds	r1, r7, #0
	ldr	r0, [pc, #392]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r2, #0
	ldr	r1, [sp, #32]
	ldr	r0, [pc, #380]
	movs	r3, #0
	bl	sub_080e0524
	ldr	r3, [r5, #0]
	movs	r2, #36
	ldrsh	r1, [r3, r2]
	ldr	r0, [r3, #8]
	movs	r2, #4
	movs	r3, #0
	bl	sub_080b5078
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [r5, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	bl	sub_080b5098
	movs	r5, #225
	movs	r2, #0
	lsls	r5, r5, #7
	ldr	r6, [r0, #0]
	mov	r8, r2
	movs	r7, #255
	add	r5, fp
.L_080e912e:
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_08004458
	ands	r0, r7
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #127
	lsls	r0, r0, #12
	str	r0, [r5, #16]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #127
	ldr	r3, [r5, #0]
	lsls	r0, r0, #12
	str	r0, [r5, #20]
	cmp	r3, #0
	ble.n	.L_080e9168
	ldr	r3, [r5, #12]
	negs	r3, r3
	str	r3, [r5, #12]
.L_080e9168:
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_080e9170
	adds	r3, #3
.L_080e9170:
	asrs	r3, r3, #2
	lsls	r3, r3, #1
	adds	r3, #16
	str	r3, [r5, #24]
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	adds	r5, #28
	cmp	r0, #64
	bne.n	.L_080e912e
	ldr	r3, [pc, #224]
	add	r3, fp
	ldr	r3, [r3, #0]
	mov	r2, sp
	adds	r2, #64
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	adds	r1, r2, #0
	str	r2, [sp, #24]
	bl	sub_080e396c
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #220]
	bl	sub_080041d8
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #208]
	movs	r3, #75
	add	r2, fp
	str	r3, [r2, #0]
	movs	r3, #0
	mov	r9, r3
	ldr	r1, [sp, #28]
	ldr	r2, [pc, #168]
	ldr	r3, [pc, #196]
	mov	r0, sp
	adds	r0, #76
	adds	r1, #12
	add	r2, fp
	add	r3, fp
	str	r0, [sp, #16]
	str	r1, [sp, #12]
	str	r2, [sp, #20]
	str	r3, [sp, #8]
.L_080e91d2:
	mov	r0, r9
	cmp	r0, #8
	bne.n	.L_080e91de
	movs	r0, #134
	bl	sub_080b50e8
.L_080e91de:
	ldr	r1, [sp, #20]
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #24]
	cmp	r3, #0
	beq.n	.L_080e91f4
	mov	r2, r9
	cmp	r2, #8
	bne.n	.L_080e91f4
	movs	r0, #212
	bl	sub_080f9010
.L_080e91f4:
	ldr	r5, [pc, #112]
	add	r5, fp
	ldr	r3, [r5, #0]
	ldr	r1, [sp, #16]
	ldr	r0, [r3, #8]
	bl	sub_080e396c
	mov	r3, r9
	subs	r3, #6
	cmp	r3, #5
	bhi.n	.L_080e92ae
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080e9222
	movs	r3, #3
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	str	r3, [sp, #0]
	bl	sub_080ed408
	b.n	.L_080e9232
.L_080e9222:
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	bl	sub_080ed408
.L_080e9232:
	ldr	r3, [pc, #84]
	ldr	r0, [sp, #20]
	adds	r3, #184
	ldr	r4, [r3, #0]
	ldr	r3, [r0, #0]
	ldr	r3, [r3, #4]
	str	r4, [sp, #40]
	cmp	r3, #0
	bne.n	.L_080e928c
	ldr	r2, [sp, #76]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	movs	r1, #48
	ldr	r3, [sp, #80]
	asrs	r2, r2, #1
	str	r1, [sp, #0]
	movs	r1, #72
	str	r1, [sp, #4]
	subs	r2, #24
	subs	r3, #24
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #8]
	bl	sub_080072f4
	b.n	.L_080e92a8
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x02010000
	.4byte 0x00000096
	.4byte 0x00000063
	.4byte 0x00000073
	.4byte 0x080cd261
	.4byte 0x00007784
	.4byte 0xffffaf00
	.2byte 0x1e50
	.2byte 0x0300
.L_080e928c:
	ldr	r2, [sp, #76]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	movs	r1, #48
	ldr	r3, [sp, #80]
	str	r1, [sp, #0]
	movs	r1, #72
	str	r1, [sp, #4]
	asrs	r2, r2, #1
	subs	r3, #24
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #8]
	bl	sub_080072f4
.L_080e92a8:
	movs	r0, #46
	bl	sub_08002dd8
.L_080e92ae:
	mov	r2, r9
	subs	r2, #16
	cmp	r2, #31
	bhi.n	.L_080e9328
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r5, r3, #1
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	bl	sub_080ed408
	ldr	r3, [pc, #444]
	adds	r3, #184
	ldr	r3, [r3, #0]
	str	r3, [sp, #40]
	cmp	r5, #2
	ble.n	.L_080e92da
	movs	r5, #2
.L_080e92da:
	ldr	r1, [sp, #20]
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #24]
	movs	r2, #0
	cmp	r3, #0
	beq.n	.L_080e92ea
	movs	r2, #150
	lsls	r2, r2, #6
.L_080e92ea:
	lsls	r1, r5, #1
	adds	r1, r1, r5
	lsls	r1, r1, #3
	adds	r1, r1, r5
	lsls	r1, r1, #7
	ldr	r3, [sp, #24]
	adds	r1, r2, r1
	ldr	r2, [pc, #404]
	adds	r1, r1, r2
	ldr	r2, [r3, #0]
	ldr	r0, [sp, #24]
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r0, #4]
	movs	r0, #40
	str	r0, [sp, #0]
	asrs	r2, r2, #1
	movs	r0, #80
	str	r0, [sp, #4]
	subs	r2, #20
	subs	r3, #48
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #36]
	bl	sub_080072f4
	ldr	r0, [pc, #372]
	bl	sub_080d655c
	movs	r0, #46
	bl	sub_08002dd8
.L_080e9328:
	mov	r1, r9
	cmp	r1, #8
	bne.n	.L_080e933e
	ldr	r3, [pc, #360]
	add	r0, sp, #48
	str	r3, [r0, #0]
	ldr	r1, [sp, #36]
	ldr	r3, [pc, #356]
	ldr	r2, [pc, #356]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_080e933e:
	bl	sub_080049ac
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #12]
	bl	sub_080051d8
	mov	r2, r9
	cmp	r2, #3
	ble.n	.L_080e93e8
	ldr	r0, [sp, #20]
	ldr	r3, [r0, #0]
	add	r1, sp, #40
	ldr	r0, [r3, #4]
	mov	sl, r1
	bl	sub_080cef64
	movs	r7, #225
	movs	r2, #0
	lsls	r7, r7, #7
	mov	r8, r2
	add	r6, sp, #52
	add	r7, fp
.L_080e936a:
	ldr	r5, [r7, #24]
	cmp	r5, #0
	ble.n	.L_080e93d0
	adds	r1, r6, #0
	adds	r0, r7, #0
	bl	sub_080e3944
	ldr	r2, [r6, #0]
	asrs	r2, r2, #1
	str	r2, [r6, #0]
	ldr	r0, [sp, #24]
	ldr	r3, [r6, #4]
	ldr	r1, [r0, #4]
	adds	r3, r3, r1
	mov	r1, r8
	lsrs	r4, r1, #31
	asrs	r5, r5, #3
	add	r4, r8
	adds	r5, #2
	movs	r1, #1
	asrs	r4, r4, #1
	subs	r3, #112
	ands	r4, r1
	lsls	r1, r5, #1
	str	r3, [r6, #4]
	ldr	r0, [pc, #260]
	str	r1, [sp, #4]
	subs	r1, #2
	ldrh	r1, [r0, r1]
	ldr	r0, [sp, #32]
	adds	r1, r0, r1
	lsrs	r0, r5, #31
	adds	r0, r5, r0
	str	r5, [sp, #0]
	asrs	r0, r0, #1
	subs	r2, r2, r0
	lsls	r4, r4, #2
	mov	r0, sl
	subs	r3, r3, r5
	ldr	r4, [r4, r0]
	ldr	r0, [sp, #36]
	bl	sub_080072f4
	adds	r0, r7, #0
	movs	r1, #60
	ldr	r2, [pc, #224]
	bl	sub_080e38b8
	ldr	r3, [r7, #24]
	subs	r3, #1
	str	r3, [r7, #24]
.L_080e93d0:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r7, #28
	cmp	r2, #64
	bne.n	.L_080e936a
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
.L_080e93e8:
	mov	r3, r9
	cmp	r3, #8
	bne.n	.L_080e9404
	ldr	r0, [sp, #20]
	ldr	r3, [r0, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r1, #4
	bl	sub_080b5088
	ldr	r2, [pc, #172]
	movs	r3, #4
	add	r2, fp
	str	r3, [r2, #0]
.L_080e9404:
	mov	r2, r9
	cmp	r2, #6
	bne.n	.L_080e9422
	ldr	r0, [sp, #20]
	ldr	r3, [r0, #0]
	movs	r2, #1
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #10
	negs	r2, r2
	subs	r3, #1
	bl	sub_080d6888
.L_080e9422:
	mov	r2, r9
	cmp	r2, #14
	bne.n	.L_080e9440
	ldr	r0, [sp, #20]
	ldr	r3, [r0, #0]
	movs	r2, #1
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #10
	negs	r2, r2
	subs	r3, #1
	bl	sub_080d6888
.L_080e9440:
	movs	r1, #16
	movs	r0, #16
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #96]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r3, #216
	ldr	r2, [sp, #8]
	movs	r0, #1
	lsls	r3, r3, #4
	add	r9, r0
	adds	r2, r2, r3
	mov	r1, r9
	str	r2, [sp, #8]
	cmp	r1, #64
	beq.n	.L_080e9470
	b.n	.L_080e91d2
.L_080e9470:
	ldr	r0, [pc, #64]
	bl	sub_08004278
	bl	sub_080cdbc0
	add	sp, #88
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e50
	.4byte 0x02010000
	.4byte 0x00002710
	.4byte 0x3f3f3f3f
	.4byte 0x040000d4
	.4byte 0x85001000
	.4byte 0x080ede48
	.4byte 0xfffffc00
	.4byte 0x000077a8
	.4byte 0x00007824
	.2byte 0xd261
	.2byte 0x080c
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #112]
	adds	r3, r2, #0
	adds	r5, r0, #0
	ldmia	r3!, {r0}
	ldr	r3, [r3, #0]
	sub	sp, #52
	str	r3, [sp, #40]
	ldr	r2, [r2, #8]
	str	r2, [sp, #32]
	mov	fp, r0
	ldr	r0, [r5, #8]
	bl	sub_080b5098
	ldr	r6, [pc, #88]
	ldr	r0, [r0, #0]
	movs	r3, #1
	add	r6, fp
	str	r0, [sp, #28]
	str	r3, [r5, #24]
	movs	r0, #1
	str	r5, [r6, #0]
	bl	sub_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #60]
	strh	r3, [r2, #0]
	ldr	r3, [r6, #0]
	mov	r1, sp
	ldr	r0, [r3, #4]
	adds	r1, #44
	str	r1, [sp, #24]
	bl	sub_080cef64
	movs	r1, #2
	ldr	r0, [sp, #28]
	bl	sub_08009080
	movs	r1, #48
	ldr	r0, [sp, #28]
	bl	sub_08009088
	ldr	r0, [pc, #40]
	mov	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r1, #128
	lsls	r1, r1, #6
	ldr	r0, [pc, #28]
	add	r1, fp
	movs	r2, #1
	movs	r3, #0
	b.n	.L_080e954c
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000052
	.4byte 0x00000055
	.2byte 0x007d
	.2byte 0x0000
.L_080e954c:
	bl	sub_080e0524
	movs	r2, #0
	ldr	r0, [pc, #776]
	ldr	r1, [sp, #32]
	movs	r3, #0
	bl	sub_080e0524
	movs	r2, #0
	str	r2, [sp, #36]
	mov	r9, r2
	mov	r8, fp
.L_080e9564:
	movs	r7, #225
	movs	r3, #0
	lsls	r7, r7, #7
	mov	sl, r3
	add	r7, r8
.L_080e956e:
	mov	r0, sl
	lsls	r6, r0, #1
	bl	sub_08004458
	ldr	r3, [pc, #744]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r3, r6, #0
	muls	r3, r0
	adds	r0, r5, #0
	str	r3, [r7, #0]
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	mov	r1, sl
	negs	r3, r3
	str	r3, [r7, #4]
	lsrs	r3, r1, #31
	add	r3, sl
	asrs	r3, r3, #1
	movs	r2, #1
	adds	r3, #25
	add	sl, r2
	str	r3, [r7, #24]
	mov	r3, sl
	adds	r7, #28
	cmp	r3, #16
	bne.n	.L_080e956e
	mov	r1, r9
	lsls	r3, r1, #3
	subs	r3, r3, r1
	ldr	r2, [pc, #684]
	movs	r0, #0
	lsls	r3, r3, #2
	mov	sl, r0
	adds	r7, r3, r2
.L_080e95be:
	bl	sub_08004458
	ldr	r5, [pc, #676]
	ands	r5, r0
	bl	sub_08004458
	ldr	r3, [pc, #660]
	adds	r6, r0, #0
	ands	r6, r3
	ldr	r3, [pc, #664]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #4]
	ldr	r0, [sp, #36]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r1, [pc, #656]
	adds	r3, r0, r3
	ldrb	r3, [r1, r3]
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	movs	r3, #176
	lsls	r3, r3, #15
	str	r3, [r7, #4]
	adds	r0, r6, #0
	bl	sub_08002322
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #32
	str	r3, [r7, #24]
	movs	r2, #1
	movs	r3, #170
	add	sl, r2
	lsls	r3, r3, #1
	adds	r7, #28
	cmp	sl, r3
	bne.n	.L_080e95be
	ldr	r1, [sp, #36]
	movs	r0, #224
	lsls	r0, r0, #1
	adds	r1, #1
	add	r9, r3
	add	r8, r0
	str	r1, [sp, #36]
	cmp	r1, #3
	bne.n	.L_080e9564
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #556]
	movs	r3, #75
	add	r2, fp
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #548]
	lsls	r1, r1, #3
	bl	sub_080041d8
	movs	r2, #0
	mov	r9, r2
.L_080e965c:
	mov	r3, r9
	cmp	r3, #4
	bne.n	.L_080e9668
	movs	r0, #212
	bl	sub_080f9010
.L_080e9668:
	mov	r0, r9
	cmp	r0, #8
	bne.n	.L_080e9674
	ldr	r3, [pc, #524]
	add	r3, fp
	str	r0, [r3, #0]
.L_080e9674:
	mov	r1, r9
	cmp	r1, #18
	bne.n	.L_080e9680
	movs	r0, #145
	bl	sub_080f9010
.L_080e9680:
	mov	r2, r9
	cmp	r2, #40
	bne.n	.L_080e968c
	movs	r0, #134
	bl	sub_080b50e8
.L_080e968c:
	mov	r3, r9
	cmp	r3, #39
	bgt.n	.L_080e9724
	ldr	r3, [pc, #472]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	movs	r1, #128
	cmp	r3, #1
	bne.n	.L_080e96ce
	mov	r0, r9
	cmp	r0, #9
	bgt.n	.L_080e96b8
	lsls	r3, r0, #2
	add	r3, r9
	lsls	r3, r3, #1
	adds	r2, r3, #0
	lsls	r3, r0, #4
	adds	r5, r3, #0
	subs	r2, #8
	subs	r5, #128
	b.n	.L_080e96fe
.L_080e96b8:
	mov	r2, r9
	cmp	r2, #20
	ble.n	.L_080e96ca
	mov	r0, r9
	lsls	r3, r0, #1
	adds	r5, r3, #0
	adds	r2, #62
	subs	r5, #24
	b.n	.L_080e96fe
.L_080e96ca:
	movs	r2, #82
	b.n	.L_080e96fc
.L_080e96ce:
	mov	r2, r9
	cmp	r2, #9
	bgt.n	.L_080e96e6
	lsls	r3, r2, #2
	add	r3, r9
	lsls	r3, r3, #1
	mov	r0, r9
	subs	r2, r1, r3
	lsls	r3, r0, #4
	adds	r5, r3, #0
	subs	r5, #128
	b.n	.L_080e96fe
.L_080e96e6:
	mov	r2, r9
	cmp	r2, #20
	ble.n	.L_080e96fa
	movs	r3, #58
	mov	r0, r9
	subs	r2, r3, r2
	lsls	r3, r0, #1
	adds	r5, r3, #0
	subs	r5, #24
	b.n	.L_080e96fe
.L_080e96fa:
	movs	r2, #38
.L_080e96fc:
	movs	r5, #16
.L_080e96fe:
	adds	r3, r5, #0
	adds	r3, #128
	cmp	r3, #104
	ble.n	.L_080e970c
	subs	r3, r1, r5
	adds	r1, r3, #0
	subs	r1, #24
.L_080e970c:
	cmp	r1, #0
	ble.n	.L_080e9724
	movs	r3, #64
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	subs	r2, #32
	ldr	r4, [sp, #44]
	ldr	r0, [sp, #40]
	mov	r1, fp
	adds	r3, r5, #0
	bl	sub_080072f4
.L_080e9724:
	mov	r1, r9
	cmp	r1, #16
	ble.n	.L_080e9730
	ldr	r0, [pc, #340]
	bl	sub_080e46f0
.L_080e9730:
	movs	r2, #0
	movs	r3, #22
	movs	r0, #16
	mov	r1, fp
	str	r2, [sp, #36]
	str	r3, [sp, #20]
	str	r2, [sp, #16]
	str	r0, [sp, #12]
	str	r1, [sp, #8]
.L_080e9742:
	ldr	r2, [sp, #36]
	ldr	r3, [sp, #12]
	lsls	r1, r2, #3
	cmp	r9, r3
	bne.n	.L_080e9754
	ldr	r2, [pc, #300]
	movs	r3, #12
	add	r2, fp
	str	r3, [r2, #0]
.L_080e9754:
	ldr	r0, [sp, #12]
	cmp	r9, r0
	blt.n	.L_080e980c
	adds	r3, r1, #0
	adds	r3, #18
	cmp	r9, r3
	bge.n	.L_080e9790
	ldr	r3, [pc, #264]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #4]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #36]
	ldr	r1, [pc, #252]
	adds	r3, r2, r3
	ldrb	r2, [r1, r3]
	movs	r3, #32
	movs	r1, #128
	str	r3, [sp, #0]
	lsls	r1, r1, #6
	movs	r3, #64
	str	r3, [sp, #4]
	subs	r2, #16
	ldr	r4, [sp, #44]
	ldr	r0, [sp, #40]
	add	r1, fp
	movs	r3, #56
	bl	sub_080072f4
.L_080e9790:
	ldr	r0, [pc, #240]
	ldr	r1, [sp, #8]
	movs	r2, #225
	movs	r3, #0
	lsls	r2, r2, #7
	mov	sl, r3
	mov	r8, r0
	adds	r5, r1, r2
.L_080e97a0:
	movs	r3, #6
	ldrsh	r7, [r5, r3]
	ldr	r3, [pc, #196]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #4]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r2, [sp, #36]
	movs	r0, #2
	ldrsh	r1, [r5, r0]
	ldr	r0, [pc, #184]
	adds	r3, r2, r3
	ldrb	r3, [r0, r3]
	ldr	r0, [r5, #24]
	adds	r6, r1, r3
	cmp	r0, #17
	bhi.n	.L_080e97f2
	movs	r1, #3
	bl	sub_080022ec
	mov	r2, r8
	ldrb	r1, [r2, r0]
	movs	r3, #128
	lsls	r1, r1, #11
	lsls	r3, r3, #6
	movs	r0, #32
	add	r1, fp
	adds	r1, r1, r3
	str	r0, [sp, #0]
	adds	r2, r6, #0
	movs	r0, #64
	adds	r3, r7, #0
	str	r0, [sp, #4]
	subs	r2, #16
	adds	r3, #56
	ldr	r4, [sp, #44]
	ldr	r0, [sp, #40]
	bl	sub_080072f4
	ldr	r0, [r5, #24]
.L_080e97f2:
	cmp	r0, #0
	ble.n	.L_080e97fa
	subs	r3, r0, #1
	b.n	.L_080e97fe
.L_080e97fa:
	movs	r3, #1
	negs	r3, r3
.L_080e97fe:
	str	r3, [r5, #24]
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	adds	r5, #28
	cmp	r1, #12
	bne.n	.L_080e97a0
.L_080e980c:
	ldr	r3, [sp, #12]
	adds	r3, #5
	cmp	r9, r3
	ble.n	.L_080e98e2
	movs	r2, #0
	ldr	r7, [sp, #16]
	mov	sl, r2
.L_080e981a:
	lsls	r3, r7, #4
	adds	r3, r7, r3
	lsls	r3, r3, #2
	add	r3, sl
	lsls	r2, r3, #3
	subs	r2, r2, r3
	ldr	r3, [pc, #60]
	lsls	r2, r2, #2
	adds	r6, r2, r3
	ldr	r3, [r6, #24]
	cmp	r3, #0
	ble.n	.L_080e98d6
	movs	r2, #128
	adds	r0, r6, #0
	movs	r1, #64
	lsls	r2, r2, #5
	bl	sub_080e3908
	ldr	r3, [r6, #24]
	movs	r0, #216
	ldr	r1, [r6, #4]
	subs	r3, #1
	lsls	r0, r0, #15
	str	r3, [r6, #24]
	cmp	r1, r0
	ble.n	.L_080e9888
	ldr	r3, [r6, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
	b.n	.L_080e98d6
	.4byte 0x00000073
	.4byte 0x0000ffff
	.4byte 0x02010000
	.4byte 0x000001ff
	.4byte 0x00007828
	.4byte 0x080eef06
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x000077a8
	.4byte 0x000000c0
	.2byte 0xef0c
	.2byte 0x080e
.L_080e9888:
	ldr	r0, [r6, #0]
	ldr	r2, [pc, #288]
	cmp	r0, r2
	bhi.n	.L_080e98d6
	cmp	r1, #0
	blt.n	.L_080e98d6
	asrs	r1, r1, #16
	mov	r8, r1
	asrs	r6, r0, #16
	movs	r1, #5
	adds	r0, r3, #0
	bl	sub_080022ec
	adds	r0, #1
	lsls	r5, r0, #1
	mov	r3, sl
	ldr	r2, [pc, #260]
	movs	r4, #1
	ands	r4, r3
	subs	r3, r5, #2
	ldrh	r1, [r2, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r6, r6, r3
	mov	r3, r8
	ldr	r2, [sp, #32]
	subs	r3, r3, r0
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #24]
	lsls	r4, r4, #2
	adds	r1, r2, r1
	ldr	r4, [r4, r0]
	adds	r2, r6, #0
	ldr	r0, [sp, #40]
	mov	r8, r3
	bl	sub_080072f4
.L_080e98d6:
	movs	r1, #1
	movs	r2, #128
	add	sl, r1
	lsls	r2, r2, #1
	cmp	sl, r2
	bne.n	.L_080e981a
.L_080e98e2:
	ldr	r2, [pc, #208]
	movs	r3, #0
	mov	r0, fp
	mov	sl, r3
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_080e992e
	ldr	r1, [sp, #20]
	movs	r6, #36
	mov	r8, r1
.L_080e98f8:
	cmp	r9, r8
	bne.n	.L_080e991c
	mov	r3, fp
	adds	r5, r3, r2
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #10
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, sl
	bl	sub_080d6888
	ldr	r3, [r5, #0]
	movs	r1, #4
	ldrsh	r0, [r3, r6]
	bl	sub_080b5088
.L_080e991c:
	ldr	r2, [pc, #148]
	movs	r3, #1
	mov	r0, fp
	add	sl, r3
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	adds	r6, #2
	cmp	sl, r3
	bne.n	.L_080e98f8
.L_080e992e:
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #16]
	adds	r1, #8
	adds	r2, #5
	ldr	r3, [sp, #12]
	ldr	r0, [sp, #8]
	str	r1, [sp, #20]
	str	r2, [sp, #16]
	movs	r1, #224
	ldr	r2, [sp, #36]
	lsls	r1, r1, #1
	adds	r3, #8
	adds	r0, r0, r1
	adds	r2, #1
	str	r3, [sp, #12]
	str	r0, [sp, #8]
	str	r2, [sp, #36]
	cmp	r2, #2
	beq.n	.L_080e9956
	b.n	.L_080e9742
.L_080e9956:
	movs	r0, #16
	movs	r1, #16
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r3, [pc, #84]
	movs	r5, #1
	add	r3, fp
	str	r5, [r3, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	cmp	r0, #80
	beq.n	.L_080e997c
	b.n	.L_080e965c
.L_080e997c:
	ldr	r0, [sp, #28]
	movs	r1, #16
	bl	sub_08009088
	ldr	r0, [pc, #52]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x007effff
	.4byte 0x080ede48
	.4byte 0x00007828
	.4byte 0x00007824
	.2byte 0xd261
	.2byte 0x080c
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #96]
	adds	r3, r6, #0
	ldmia	r3!, {r1}
	sub	sp, #52
	str	r1, [sp, #36]
	ldr	r3, [r3, #0]
	str	r3, [sp, #32]
	ldr	r3, [pc, #84]
	ldr	r2, [r6, #8]
	adds	r5, r1, r3
	str	r2, [sp, #20]
	str	r0, [r5, #0]
	movs	r0, #1
	bl	sub_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #60]
	strh	r3, [r2, #0]
	ldr	r3, [r5, #0]
	add	r5, sp, #40
	movs	r4, #36
	ldrsh	r0, [r3, r4]
	adds	r1, r5, #0
	bl	sub_080e3980
	ldr	r3, [r5, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [sp, #16]
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r0, [r6, #28]
	movs	r3, #1
	str	r0, [sp, #24]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	b.n	.L_080e9a3c
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.2byte 0x0052
	.2byte 0x0400
.L_080e9a3c:
	bl	sub_080ed408
	ldr	r3, [pc, #500]
	ldr	r2, [sp, #36]
	ldr	r6, [r6, #32]
	adds	r1, r2, r3
	ldr	r0, [pc, #496]
	movs	r2, #1
	movs	r3, #1
	str	r6, [sp, #28]
	bl	sub_080e0524
	ldr	r0, [pc, #488]
	ldr	r1, [sp, #36]
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	movs	r6, #221
	ldr	r4, [sp, #36]
	lsls	r6, r6, #4
	adds	r1, r4, r6
	ldr	r0, [pc, #472]
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [pc, #468]
	ldr	r1, [sp, #20]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	movs	r1, #239
	ldr	r0, [sp, #36]
	lsls	r1, r1, #7
	adds	r3, r0, r1
	str	r5, [r3, #0]
	ldr	r3, [pc, #448]
	movs	r1, #144
	adds	r2, r0, r3
	movs	r3, #75
	str	r3, [r2, #0]
	ldr	r0, [pc, #444]
	lsls	r1, r1, #3
	bl	sub_080041d8
	movs	r6, #225
	ldr	r5, [sp, #36]
	movs	r4, #0
	lsls	r6, r6, #7
	mov	r8, r4
	adds	r7, r5, r6
.L_080e9aa6:
	mov	r0, r8
	lsls	r6, r0, #1
	bl	sub_08004458
	ldr	r3, [pc, #420]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r3, r6, #0
	muls	r3, r0
	adds	r0, r5, #0
	str	r3, [r7, #0]
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	mov	r1, r8
	negs	r3, r3
	str	r3, [r7, #4]
	lsrs	r3, r1, #31
	add	r3, r8
	asrs	r3, r3, #1
	movs	r2, #1
	adds	r3, #25
	add	r8, r2
	str	r3, [r7, #24]
	mov	r3, r8
	adds	r7, #28
	cmp	r3, #32
	bne.n	.L_080e9aa6
	movs	r4, #0
	movs	r1, #1
	movs	r2, #171
	ldr	r3, [pc, #360]
	mov	r8, r4
	negs	r1, r1
	lsls	r2, r2, #2
.L_080e9af4:
	movs	r5, #1
	add	r8, r5
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_080e9af4
	ldr	r0, [sp, #16]
	lsls	r0, r0, #16
	movs	r6, #0
	str	r0, [sp, #8]
	ldr	r7, [pc, #336]
	mov	r8, r6
.L_080e9b0c:
	bl	sub_08004458
	ldr	r6, [pc, #332]
	ands	r6, r0
	bl	sub_08004458
	ldr	r3, [pc, #312]
	adds	r5, r0, #0
	ldr	r1, [sp, #8]
	ands	r5, r3
	movs	r3, #176
	lsls	r3, r3, #15
	str	r1, [r7, #0]
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r6, #32
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #32
	str	r3, [r7, #24]
	movs	r2, #1
	movs	r3, #170
	add	r8, r2
	lsls	r3, r3, #1
	adds	r7, #28
	cmp	r8, r3
	bne.n	.L_080e9b0c
	movs	r4, #0
	mov	fp, r4
.L_080e9b66:
	mov	r3, fp
	subs	r3, #25
	cmp	r3, #22
	bhi.n	.L_080e9b74
	ldr	r0, [pc, #244]
	bl	sub_080e46f0
.L_080e9b74:
	mov	r5, fp
	cmp	r5, #56
	ble.n	.L_080e9b80
	ldr	r0, [pc, #236]
	bl	sub_080e46f0
.L_080e9b80:
	mov	r6, fp
	cmp	r6, #8
	bne.n	.L_080e9b8e
	ldr	r0, [sp, #36]
	ldr	r1, [pc, #224]
	adds	r3, r0, r1
	str	r6, [r3, #0]
.L_080e9b8e:
	mov	r2, fp
	cmp	r2, #48
	bne.n	.L_080e9b9e
	ldr	r3, [sp, #36]
	ldr	r4, [pc, #212]
	adds	r2, r3, r4
	movs	r3, #8
	str	r3, [r2, #0]
.L_080e9b9e:
	mov	r5, fp
	cmp	r5, #60
	bne.n	.L_080e9bae
	ldr	r6, [sp, #36]
	ldr	r0, [pc, #196]
	movs	r3, #16
	adds	r2, r6, r0
	str	r3, [r2, #0]
.L_080e9bae:
	mov	r1, fp
	cmp	r1, #4
	bne.n	.L_080e9bba
	movs	r0, #212
	bl	sub_080f9010
.L_080e9bba:
	mov	r2, fp
	cmp	r2, #32
	bne.n	.L_080e9bc6
	movs	r0, #164
	bl	sub_080f9010
.L_080e9bc6:
	mov	r3, fp
	cmp	r3, #60
	bne.n	.L_080e9bd8
	movs	r0, #145
	bl	sub_080f9010
	movs	r0, #134
	bl	sub_080b50e8
.L_080e9bd8:
	mov	r4, fp
	cmp	r4, #55
	ble.n	.L_080e9c86
	ldr	r6, [pc, #144]
	ldr	r0, [sp, #36]
	movs	r1, #225
	movs	r5, #0
	lsls	r1, r1, #7
	mov	r8, r5
	mov	sl, r6
	adds	r5, r0, r1
.L_080e9bee:
	ldr	r0, [sp, #16]
	movs	r4, #2
	ldrsh	r3, [r5, r4]
	adds	r6, r3, r0
	ldr	r0, [r5, #24]
	movs	r2, #6
	ldrsh	r7, [r5, r2]
	cmp	r0, #17
	bhi.n	.L_080e9c30
	movs	r1, #3
	bl	sub_080022ec
	mov	r2, sl
	ldrb	r1, [r2, r0]
	ldr	r3, [sp, #36]
	movs	r0, #32
	lsls	r1, r1, #11
	movs	r4, #221
	adds	r1, r3, r1
	lsls	r4, r4, #4
	adds	r2, r6, #0
	str	r0, [sp, #0]
	adds	r3, r7, #0
	movs	r0, #64
	str	r0, [sp, #4]
	adds	r1, r1, r4
	subs	r2, #16
	adds	r3, #48
	ldr	r0, [sp, #32]
	ldr	r6, [sp, #24]
	bl	sub_080072fc
	ldr	r0, [r5, #24]
.L_080e9c30:
	cmp	r0, #0
	ble.n	.L_080e9c74
	subs	r3, r0, #1
	b.n	.L_080e9c78
	.4byte 0x00004e20
	.4byte 0x00000056
	.4byte 0x00000085
	.4byte 0x0000007d
	.4byte 0x00000073
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x0000ffff
	.4byte 0x02010018
	.4byte 0x02014ad0
	.4byte 0x000001ff
	.4byte 0x000000c0
	.4byte 0x000000c4
	.4byte 0x000077a8
	.2byte 0xef12
	.2byte 0x080e
.L_080e9c74:
	movs	r3, #1
	negs	r3, r3
.L_080e9c78:
	str	r3, [r5, #24]
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #16
	bne.n	.L_080e9bee
.L_080e9c86:
	mov	r2, fp
	cmp	r2, #28
	bne.n	.L_080e9d0a
	movs	r3, #0
	movs	r4, #63
	ldr	r7, [pc, #760]
	mov	r8, r3
	mov	sl, r4
.L_080e9c96:
	movs	r5, #1
	ldr	r3, [r7, #24]
	negs	r5, r5
	cmp	r3, r5
	bne.n	.L_080e9cfc
	bl	sub_08004458
	adds	r6, r0, #0
	mov	r0, sl
	ands	r6, r0
	bl	sub_08004458
	ldr	r3, [pc, #736]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r3, r6, #0
	muls	r3, r0
	ldr	r1, [sp, #8]
	asrs	r3, r3, #3
	adds	r3, r3, r1
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	movs	r2, #192
	lsls	r2, r2, #15
	asrs	r3, r3, #2
	adds	r3, r3, r2
	str	r3, [r7, #4]
	bl	sub_08004458
	mov	r3, sl
	ands	r0, r3
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r7, #12]
	bl	sub_08004458
	mov	r4, sl
	ands	r0, r4
	negs	r0, r0
	subs	r0, #8
	lsls	r0, r0, #13
	movs	r3, #0
	str	r0, [r7, #16]
	str	r3, [r7, #24]
.L_080e9cfc:
	movs	r5, #1
	movs	r6, #128
	add	r8, r5
	lsls	r6, r6, #1
	adds	r7, #28
	cmp	r8, r6
	bne.n	.L_080e9c96
.L_080e9d0a:
	mov	r0, fp
	subs	r0, #32
	str	r0, [sp, #12]
	cmp	r0, #31
	bhi.n	.L_080e9dec
	movs	r1, #0
	movs	r2, #63
	ldr	r7, [pc, #624]
	mov	r9, r1
	mov	r8, r1
	mov	sl, r2
.L_080e9d20:
	movs	r4, #1
	ldr	r3, [r7, #24]
	negs	r4, r4
	cmp	r3, r4
	bne.n	.L_080e9d90
	bl	sub_08004458
	adds	r6, r0, #0
	bl	sub_08004458
	mov	r5, sl
	ldr	r3, [pc, #600]
	ands	r6, r5
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r3, r6, #0
	muls	r3, r0
	ldr	r0, [sp, #8]
	asrs	r3, r3, #3
	adds	r3, r3, r0
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	movs	r1, #192
	lsls	r1, r1, #15
	asrs	r3, r3, #2
	adds	r3, r3, r1
	str	r3, [r7, #4]
	bl	sub_08004458
	mov	r2, sl
	ands	r0, r2
	subs	r0, #32
	lsls	r0, r0, #14
	str	r0, [r7, #12]
	bl	sub_08004458
	mov	r3, sl
	ands	r0, r3
	negs	r0, r0
	movs	r4, #1
	subs	r0, #8
	add	r9, r4
	lsls	r0, r0, #13
	movs	r3, #0
	mov	r5, r9
	str	r0, [r7, #16]
	str	r3, [r7, #24]
	cmp	r5, #16
	beq.n	.L_080e9d9e
.L_080e9d90:
	movs	r6, #1
	movs	r0, #171
	add	r8, r6
	lsls	r0, r0, #2
	adds	r7, #28
	cmp	r8, r0
	bne.n	.L_080e9d20
.L_080e9d9e:
	ldr	r1, [sp, #12]
	cmp	r1, #31
	bhi.n	.L_080e9dec
	mov	r2, fp
	ldr	r3, [sp, #16]
	ldr	r1, [pc, #488]
	lsls	r0, r2, #4
	movs	r4, #34
	subs	r3, #17
	adds	r0, r0, r1
	movs	r1, #104
	mov	sl, r3
	mov	r8, r4
	ldr	r5, [sp, #24]
	bl	sub_080022fc
	mov	r9, r5
	mov	r2, r8
	adds	r5, r0, #0
	movs	r6, #104
	movs	r3, #4
	subs	r3, r3, r5
	str	r2, [sp, #0]
	ldr	r1, [sp, #36]
	mov	r2, sl
	str	r6, [sp, #4]
	ldr	r0, [sp, #32]
	bl	sub_08007308
	movs	r3, #108
	mov	r4, r8
	subs	r3, r3, r5
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #36]
	mov	r2, sl
	bl	sub_08007308
.L_080e9dec:
	mov	r5, fp
	cmp	r5, #71
	bgt.n	.L_080e9ee8
	movs	r6, #0
	ldr	r5, [pc, #404]
	mov	r8, r6
.L_080e9df8:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_080e9eda
	mov	r0, r8
	movs	r1, #3
	bl	sub_080022fc
	ldr	r3, [r5, #16]
	adds	r4, r0, #2
	cmp	r3, #0
	ble.n	.L_080e9e10
	adds	r4, #2
.L_080e9e10:
	mov	r0, fp
	cmp	r0, #68
	ble.n	.L_080e9e1c
	cmp	r4, #5
	bgt.n	.L_080e9e1c
	movs	r4, #6
.L_080e9e1c:
	mov	r1, fp
	cmp	r1, #70
	ble.n	.L_080e9e28
	cmp	r4, #6
	bgt.n	.L_080e9e28
	movs	r4, #7
.L_080e9e28:
	mov	r2, fp
	cmp	r2, #72
	ble.n	.L_080e9e34
	cmp	r4, #7
	bgt.n	.L_080e9e34
	movs	r4, #8
.L_080e9e34:
	mov	r3, fp
	cmp	r3, #74
	ble.n	.L_080e9e40
	cmp	r4, #8
	bgt.n	.L_080e9e40
	movs	r4, #9
.L_080e9e40:
	mov	r6, fp
	cmp	r6, #76
	ble.n	.L_080e9e48
	movs	r4, #10
.L_080e9e48:
	lsls	r0, r4, #1
	ldr	r2, [pc, #332]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #20]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	str	r4, [sp, #0]
	subs	r3, r3, r4
	str	r0, [sp, #4]
	ldr	r4, [sp, #24]
	ldr	r0, [sp, #32]
	bl	sub_080072f4
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r1, [r5, #16]
	ldr	r3, [r5, #4]
	mov	r6, fp
	adds	r3, r3, r1
	str	r3, [r5, #4]
	cmp	r6, #80
	ble.n	.L_080e9e8e
	ldr	r0, [pc, #272]
	adds	r3, r1, r0
	b.n	.L_080e9e9c
.L_080e9e8e:
	movs	r2, #3
	mov	r4, r8
	ldr	r3, [pc, #268]
	ands	r2, r4
	lsls	r2, r2, #2
	ldr	r3, [r3, r2]
	adds	r3, r1, r3
.L_080e9e9c:
	str	r3, [r5, #16]
	ldr	r2, [r5, #12]
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r3, r3, #1
	cmp	r3, #0
	bge.n	.L_080e9eac
	adds	r3, #63
.L_080e9eac:
	ldr	r2, [r5, #16]
	asrs	r3, r3, #6
	str	r3, [r5, #12]
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r2, r3, #1
	cmp	r2, #0
	bge.n	.L_080e9ebe
	adds	r2, #63
.L_080e9ebe:
	ldr	r3, [r5, #24]
	asrs	r2, r2, #6
	adds	r3, #1
	str	r2, [r5, #16]
	str	r3, [r5, #24]
	cmp	r2, #0
	ble.n	.L_080e9eda
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	cmp	r3, #108
	ble.n	.L_080e9eda
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L_080e9eda:
	movs	r0, #1
	movs	r1, #171
	add	r8, r0
	lsls	r1, r1, #1
	adds	r5, #28
	cmp	r8, r1
	bne.n	.L_080e9df8
.L_080e9ee8:
	mov	r2, fp
	cmp	r2, #95
	bgt.n	.L_080e9f4e
	ldr	r2, [sp, #16]
	mov	r3, fp
	subs	r2, #18
	movs	r1, #120
	cmp	r3, #60
	ble.n	.L_080e9f02
	ldr	r5, [pc, #168]
	lsls	r3, r3, #3
	adds	r4, r3, r5
	b.n	.L_080e9f26
.L_080e9f02:
	mov	r6, fp
	cmp	r6, #32
	ble.n	.L_080e9f16
	ldr	r0, [sp, #12]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	adds	r4, r3, #0
	adds	r4, #16
	b.n	.L_080e9f26
.L_080e9f16:
	mov	r3, fp
	cmp	r3, #9
	bgt.n	.L_080e9f24
	lsls	r3, r3, #4
	adds	r4, r3, #0
	subs	r4, #128
	b.n	.L_080e9f26
.L_080e9f24:
	movs	r4, #16
.L_080e9f26:
	adds	r3, r4, #0
	adds	r3, #120
	cmp	r3, #108
	ble.n	.L_080e9f34
	subs	r3, r1, r4
	adds	r1, r3, #0
	subs	r1, #12
.L_080e9f34:
	cmp	r1, #0
	ble.n	.L_080e9f4e
	ldr	r5, [sp, #36]
	ldr	r6, [pc, #108]
	movs	r3, #36
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	adds	r3, r4, #0
	ldr	r0, [sp, #32]
	adds	r1, r5, r6
	ldr	r4, [sp, #28]
	bl	sub_080072f4
.L_080e9f4e:
	mov	r5, fp
	cmp	r5, #59
	ble.n	.L_080e9ffc
	movs	r6, #0
	ldr	r7, [pc, #84]
	mov	r8, r6
.L_080e9f5a:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	ble.n	.L_080e9fee
	movs	r2, #128
	adds	r0, r7, #0
	movs	r1, #64
	lsls	r2, r2, #6
	bl	sub_080e3908
	ldr	r3, [r7, #24]
	movs	r1, #216
	ldr	r6, [r7, #4]
	subs	r0, r3, #1
	lsls	r1, r1, #15
	str	r0, [r7, #24]
	cmp	r6, r1
	ble.n	.L_080e9fb0
	ldr	r3, [r7, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #16]
	b.n	.L_080e9fee
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0xffffff00
	.4byte 0x080ede48
	.4byte 0xffff8000
	.4byte 0x080eef18
	.4byte 0xfffffe3e
	.4byte 0x00004e20
	.2byte 0x4ad0
	.2byte 0x0201
.L_080e9fb0:
	ldr	r5, [r7, #0]
	ldr	r2, [pc, #264]
	cmp	r5, r2
	bhi.n	.L_080e9fee
	cmp	r6, #0
	blt.n	.L_080e9fee
	movs	r1, #5
	bl	sub_080022ec
	adds	r0, #1
	lsls	r4, r0, #1
	ldr	r2, [pc, #248]
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #20]
	adds	r1, r3, r1
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	asrs	r5, r5, #16
	asrs	r6, r6, #16
	subs	r5, r5, r3
	subs	r6, r6, r0
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #32]
	adds	r2, r5, #0
	adds	r3, r6, #0
	ldr	r4, [sp, #24]
	bl	sub_080072f4
.L_080e9fee:
	movs	r5, #1
	movs	r6, #170
	add	r8, r5
	lsls	r6, r6, #1
	adds	r7, #28
	cmp	r8, r6
	bne.n	.L_080e9f5a
.L_080e9ffc:
	mov	r0, fp
	cmp	r0, #68
	bne.n	.L_080ea042
	ldr	r3, [pc, #192]
	ldr	r2, [sp, #36]
	ldr	r3, [r2, r3]
	ldr	r3, [r3, #20]
	movs	r1, #0
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L_080ea042
	ldr	r3, [pc, #176]
	movs	r6, #36
	adds	r5, r2, r3
.L_080ea018:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, r8
	bl	sub_080d6888
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r1, #7
	bl	sub_080b5088
	ldr	r3, [r5, #0]
	movs	r2, #1
	ldr	r3, [r3, #20]
	add	r8, r2
	adds	r6, #2
	cmp	r8, r3
	bne.n	.L_080ea018
.L_080ea042:
	mov	r3, fp
	cmp	r3, #9
	bne.n	.L_080ea056
	movs	r1, #128
	ldr	r3, [pc, #124]
	ldr	r0, [sp, #32]
	lsls	r1, r1, #7
	ldr	r2, [pc, #120]
	bl	sub_080072f0
.L_080ea056:
	mov	r4, fp
	cmp	r4, #60
	bne.n	.L_080ea06a
	movs	r1, #128
	ldr	r3, [pc, #104]
	ldr	r0, [sp, #32]
	lsls	r1, r1, #7
	ldr	r2, [pc, #100]
	bl	sub_080072f0
.L_080ea06a:
	movs	r1, #16
	movs	r0, #16
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r6, [pc, #88]
	ldr	r5, [sp, #36]
	movs	r3, #1
	adds	r2, r5, r6
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_080030f8
	movs	r0, #1
	add	fp, r0
	mov	r1, fp
	cmp	r1, #102
	beq.n	.L_080ea092
	b.n	.L_080e9b66
.L_080ea092:
	ldr	r0, [pc, #64]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x007effff
	.4byte 0x080ede48
	.4byte 0x00007828
	.4byte 0x03000168
	.4byte 0x3f3f3f3f
	.4byte 0x00007824
	.2byte 0xd261
	.2byte 0x080c
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #112]
	ldr	r1, [r5, #0]
	sub	sp, #184
	str	r1, [sp, #104]
	subs	r3, r5, #4
	ldr	r3, [r3, #0]
	str	r3, [sp, #100]
	ldr	r2, [r5, #16]
	str	r2, [sp, #96]
	ldr	r3, [r5, #4]
	str	r3, [sp, #80]
	adds	r3, r5, #0
	ldr	r4, [sp, #100]
	ldr	r1, [pc, #88]
	subs	r3, #112
	ldr	r3, [r3, #0]
	adds	r6, r4, r1
	str	r3, [sp, #76]
	str	r0, [r6, #0]
	movs	r0, #128
	lsls	r0, r0, #6
	bl	sub_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #56]
	mov	r8, r2
	mov	r4, r8
	strh	r3, [r4, #0]
	bl	sub_080c9048
	ldr	r2, [pc, #48]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r3, [sp, #0]
	bl	sub_080ed408
	ldr	r5, [r5, #24]
	movs	r0, #239
	str	r5, [sp, #88]
	ldr	r5, [sp, #100]
	lsls	r0, r0, #7
	adds	r3, r5, r0
	movs	r1, #144
	movs	r5, #0
	str	r5, [r3, #0]
	b.n	.L_080ea164
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x03001ef0
	.4byte 0x00007828
	.2byte 0x0020
	.2byte 0x0400
.L_080ea164:
	lsls	r1, r1, #3
	ldr	r0, [pc, #140]
	bl	sub_080041d8
	movs	r1, #0
	movs	r0, #0
	bl	sub_080cd104
	ldr	r0, [r6, #0]
	bl	sub_080d6750
	movs	r1, #191
	lsls	r1, r1, #1
	movs	r0, #16
	movs	r2, #1
	bl	sub_080dbb24
	ldr	r2, [pc, #112]
	movs	r3, #240
	str	r3, [r2, #16]
	movs	r0, #1
	bl	sub_080030f8
	movs	r2, #0
	ldr	r1, [pc, #100]
	movs	r0, #1
	bl	sub_080b5040
	ldr	r1, [sp, #96]
	movs	r3, #1
	str	r3, [r1, #16]
	ldr	r3, [pc, #92]
	movs	r0, #0
	strh	r5, [r3, #4]
	movs	r1, #1
	bl	sub_080cd104
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	mov	r2, r8
	strh	r3, [r2, #0]
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #44]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #60]
	ldr	r5, [pc, #64]
	movs	r4, #128
	str	r3, [sp, #72]
	ldr	r1, [sp, #100]
	lsls	r4, r4, #12
	movs	r6, #0
	ldr	r0, [pc, #56]
	movs	r2, #1
	movs	r3, #1
	str	r4, [sp, #68]
	str	r5, [sp, #64]
	str	r6, [sp, #60]
	b.n	.L_080ea214
	.4byte 0x00007741
	.4byte 0x00000080
	.4byte 0x00001010
	.4byte 0x00003f44
	.4byte 0x080cd261
	.4byte 0x03001ce0
	.4byte 0x0000003b
	.4byte 0x03001ad0
	.4byte 0x04000052
	.4byte 0xffc00000
	.4byte 0x0000ffff
	.2byte 0x00bb
	.2byte 0x0000
.L_080ea214:
	bl	sub_080e0524
	movs	r3, #192
	ldr	r2, [sp, #100]
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #328]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	ldr	r5, [pc, #324]
	ldr	r4, [sp, #100]
	ldr	r0, [pc, #324]
	adds	r1, r4, r5
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	ldr	r6, [pc, #316]
	ldr	r0, [pc, #316]
	ldr	r1, [sp, #80]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	movs	r7, #0
	movs	r5, #0
	movs	r4, #64
.L_080ea24e:
	ldr	r2, [sp, #100]
	adds	r3, r5, r2
	ldr	r2, [pc, #300]
	ldr	r1, [sp, #80]
	movs	r0, #0
	mov	ip, r4
	adds	r3, r3, r2
.L_080ea25c:
	ldrb	r2, [r1, #0]
	adds	r1, #1
	cmp	r2, ip
	ble.n	.L_080ea266
	mov	r2, ip
.L_080ea266:
	cmp	r2, #0
	bge.n	.L_080ea26c
	movs	r2, #0
.L_080ea26c:
	adds	r0, #1
	strb	r2, [r3, #0]
	adds	r3, #1
	cmp	r0, r6
	bne.n	.L_080ea25c
	adds	r7, #1
	adds	r5, r5, r6
	subs	r4, #7
	cmp	r7, #8
	bne.n	.L_080ea24e
	ldr	r0, [pc, #256]
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #252]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	sub_080072f0
	ldr	r6, [pc, #244]
	ldr	r5, [pc, #248]
	movs	r7, #0
.L_080ea29a:
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r3, #72
	str	r3, [r5, #0]
	bl	sub_08004458
	ands	r0, r6
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r6
	str	r0, [r5, #16]
	bl	sub_08004458
	adds	r7, #1
	ands	r0, r6
	str	r0, [r5, #16]
	adds	r5, #28
	cmp	r7, #128
	bne.n	.L_080ea29a
	movs	r3, #255
	movs	r4, #0
	ldr	r6, [pc, #200]
	movs	r7, #0
	mov	sl, r3
	mov	r8, r4
.L_080ea2d2:
	bl	sub_08004458
	ldr	r3, [pc, #180]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002322
	mov	r3, sl
	muls	r3, r0
	asrs	r3, r3, #2
	str	r3, [r6, #0]
	adds	r0, r5, #0
	bl	sub_0800231c
	mov	r3, sl
	muls	r3, r0
	asrs	r3, r3, #2
	str	r3, [r6, #8]
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	mov	r0, r8
	lsls	r3, r3, #16
	adds	r7, #1
	str	r3, [r6, #4]
	str	r0, [r6, #12]
	str	r0, [r6, #16]
	str	r5, [r6, #24]
	adds	r6, #28
	cmp	r7, #128
	bne.n	.L_080ea2d2
	ldr	r1, [sp, #100]
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r4, [pc, #120]
	adds	r2, r1, r3
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r1, r4
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r2, [pc, #112]
	ldr	r3, [pc, #56]
	mov	r6, sp
	strh	r3, [r2, #0]
	mov	r0, sp
	mov	r1, sp
	ldr	r2, [pc, #104]
	ldr	r3, [pc, #104]
	movs	r5, #0
	adds	r6, #172
	adds	r0, #144
	adds	r1, #116
	str	r5, [sp, #84]
	str	r6, [sp, #16]
	str	r0, [sp, #20]
	str	r1, [sp, #24]
	str	r2, [sp, #56]
	str	r3, [sp, #12]
.L_080ea34e:
	ldr	r4, [sp, #84]
	cmp	r4, #143
	bne.n	.L_080ea3b4
	movs	r1, #128
	ldr	r3, [pc, #80]
	ldr	r0, [sp, #104]
	lsls	r1, r1, #7
	ldr	r2, [pc, #76]
	bl	sub_080072f0
	movs	r0, #145
	b.n	.L_080ea3b0
	movs	r0, r0
	.4byte 0x00000784
	.4byte 0x00000067
	.4byte 0x0000095c
	.4byte 0x000000ce
	.4byte 0x00000302
	.4byte 0x00000073
	.4byte 0x00002710
	.4byte 0x00000064
	.4byte 0x03001388
	.4byte 0x0000ffff
	.4byte 0x02010000
	.4byte 0x02010e00
	.4byte 0x00007784
	.4byte 0x0400000c
	.4byte 0x03001e50
	.4byte 0xfffffa70
	.4byte 0x03000168
	.2byte 0x2a2a
	.2byte 0x2a2a
.L_080ea3b0:
	bl	sub_080f9010
.L_080ea3b4:
	ldr	r5, [sp, #84]
	cmp	r5, #80
	bne.n	.L_080ea3c0
	movs	r0, #142
	bl	sub_080f9010
.L_080ea3c0:
	ldr	r6, [sp, #84]
	cmp	r6, #0
	bge.n	.L_080ea496
	bl	sub_080049ac
	ldr	r0, [sp, #76]
	adds	r1, r0, #0
	adds	r1, #12
	bl	sub_080051d8
	add	r0, sp, #160
	mov	r8, r0
	ldr	r6, [pc, #892]
	movs	r7, #0
	mov	sl, r8
.L_080ea3de:
	adds	r0, r6, #0
	movs	r1, #60
	ldr	r2, [pc, #888]
	bl	sub_080e38b8
	mov	r1, sl
	adds	r0, r6, #0
	bl	sub_080e3944
	mov	r1, sl
	ldr	r3, [r1, #0]
	asrs	r3, r3, #1
	str	r3, [r1, #0]
	ldr	r3, [r1, #4]
	ldr	r0, [r1, #8]
	subs	r3, #120
	str	r3, [r1, #4]
	cmp	r0, #99
	bgt.n	.L_080ea40c
	movs	r3, #100
	mov	r2, r8
	str	r3, [r2, #8]
	movs	r0, #100
.L_080ea40c:
	movs	r3, #225
	lsls	r3, r3, #2
	cmp	r0, r3
	ble.n	.L_080ea41a
	mov	r4, r8
	str	r3, [r4, #8]
	adds	r0, r3, #0
.L_080ea41a:
	adds	r5, r0, #0
	subs	r5, #100
	mov	r0, r8
	str	r5, [r0, #8]
	movs	r1, #100
	adds	r0, r5, #0
	bl	sub_080022ec
	ldr	r1, [sp, #64]
	adds	r4, r0, #1
	cmp	r1, r5
	ble.n	.L_080ea434
	str	r5, [sp, #64]
.L_080ea434:
	ldr	r2, [sp, #60]
	cmp	r2, r5
	bge.n	.L_080ea43c
	str	r5, [sp, #60]
.L_080ea43c:
	lsls	r0, r4, #1
	ldr	r2, [pc, #800]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #80]
	mov	r5, r8
	adds	r1, r3, r1
	lsrs	r3, r4, #31
	ldr	r2, [r5, #0]
	adds	r3, r4, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r4, [sp, #0]
	subs	r3, r3, r4
	str	r0, [sp, #4]
	ldr	r4, [sp, #88]
	ldr	r0, [sp, #104]
	bl	sub_080072f4
	ldr	r5, [sp, #84]
	cmp	r5, #64
	bne.n	.L_080ea48e
	ldr	r0, [r6, #24]
	bl	sub_08002322
	lsls	r2, r0, #8
	ldr	r3, [r6, #12]
	subs	r2, r2, r0
	asrs	r2, r2, #3
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r0, [r6, #24]
	bl	sub_0800231c
	lsls	r2, r0, #8
	ldr	r3, [r6, #16]
	subs	r2, r2, r0
	asrs	r2, r2, #3
	adds	r3, r3, r2
	str	r3, [r6, #16]
.L_080ea48e:
	adds	r7, #1
	adds	r6, #28
	cmp	r7, #128
	bne.n	.L_080ea3de
.L_080ea496:
	ldr	r6, [sp, #84]
	cmp	r6, #72
	bne.n	.L_080ea4ae
	ldr	r0, [sp, #100]
	ldr	r1, [pc, #708]
	movs	r2, #24
	adds	r3, r0, r1
	str	r2, [r3, #0]
	ldr	r3, [pc, #704]
	adds	r2, r0, r3
	movs	r3, #0
	str	r3, [r2, #0]
.L_080ea4ae:
	ldr	r4, [sp, #16]
	movs	r3, #0
	str	r3, [r4, #8]
	str	r3, [r4, #4]
	ldr	r6, [pc, #692]
	movs	r7, #0
.L_080ea4ba:
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_080ea4c2
	adds	r3, r7, #3
.L_080ea4c2:
	asrs	r3, r3, #2
	ldr	r5, [sp, #84]
	adds	r3, #80
	cmp	r5, r3
	ble.n	.L_080ea55c
	ldr	r3, [r6, #0]
	cmp	r3, #0
	ble.n	.L_080ea55c
	bl	sub_080049ac
	ldr	r0, [r6, #16]
	bl	sub_08004c6c
	ldr	r0, [r6, #12]
	bl	sub_08004bd4
	ldr	r0, [r6, #16]
	lsls	r3, r5, #9
	adds	r0, r0, r3
	bl	sub_08004c1c
	ldr	r3, [r6, #0]
	ldr	r0, [sp, #16]
	str	r3, [r0, #0]
	ldr	r3, [r6, #0]
	subs	r3, #2
	str	r3, [r6, #0]
	cmp	r3, #0
	bge.n	.L_080ea500
	movs	r3, #0
	str	r3, [r6, #0]
.L_080ea500:
	add	r5, sp, #160
	ldr	r0, [sp, #16]
	adds	r1, r5, #0
	bl	sub_080e3944
	movs	r3, #60
	ldr	r0, [r5, #8]
	negs	r3, r3
	cmp	r0, r3
	bge.n	.L_080ea518
	str	r3, [r5, #8]
	adds	r0, r3, #0
.L_080ea518:
	cmp	r0, #60
	ble.n	.L_080ea522
	movs	r3, #60
	str	r3, [r5, #8]
	movs	r0, #60
.L_080ea522:
	adds	r0, #60
	str	r0, [r5, #8]
	movs	r1, #20
	bl	sub_080022ec
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	adds	r0, #2
	adds	r2, #60
	adds	r3, #80
	str	r2, [r5, #0]
	str	r3, [r5, #4]
	ldr	r4, [pc, #548]
	lsls	r5, r0, #1
	subs	r1, r5, #2
	ldrh	r1, [r4, r1]
	ldr	r4, [sp, #80]
	adds	r1, r4, r1
	lsrs	r4, r0, #31
	adds	r4, r0, r4
	asrs	r4, r4, #1
	subs	r3, r3, r0
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	subs	r2, r2, r4
	ldr	r0, [sp, #104]
	ldr	r5, [sp, #88]
	bl	sub_080072f8
.L_080ea55c:
	adds	r7, #1
	adds	r6, #28
	cmp	r7, #64
	bne.n	.L_080ea4ba
	ldr	r3, [pc, #520]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	ldr	r6, [sp, #20]
	str	r3, [sp, #116]
	str	r4, [sp, #120]
	movs	r3, #0
	str	r3, [r6, #12]
	str	r3, [r6, #4]
	ldr	r0, [sp, #100]
	ldr	r1, [pc, #504]
	movs	r7, #0
	adds	r5, r0, r1
.L_080ea57e:
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_080ea586
	adds	r3, r7, #3
.L_080ea586:
	asrs	r3, r3, #2
	lsls	r2, r3, #2
	subs	r2, r7, r2
	movs	r4, #152
	ldr	r6, [sp, #20]
	lsls	r4, r4, #15
	lsls	r2, r2, #21
	adds	r2, r2, r4
	str	r2, [r6, #0]
	ldr	r0, [sp, #72]
	lsls	r3, r3, #21
	adds	r3, r3, r0
	str	r3, [r6, #8]
	ldmia	r5!, {r0}
	ldr	r1, [sp, #20]
	ldr	r2, [sp, #24]
	movs	r3, #0
	adds	r7, #1
	bl	sub_08009008
	cmp	r7, #16
	bne.n	.L_080ea57e
	ldr	r1, [sp, #72]
	ldr	r2, [sp, #68]
	ldr	r3, [sp, #84]
	adds	r1, r1, r2
	str	r1, [sp, #72]
	cmp	r3, #47
	bgt.n	.L_080ea5c6
	ldr	r4, [pc, #436]
	adds	r2, r2, r4
	str	r2, [sp, #68]
.L_080ea5c6:
	ldr	r5, [sp, #84]
	cmp	r5, #32
	ble.n	.L_080ea5de
	ldr	r6, [sp, #68]
	lsls	r3, r6, #4
	subs	r3, r3, r6
	lsls	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_080ea5da
	adds	r3, #63
.L_080ea5da:
	asrs	r3, r3, #6
	str	r3, [sp, #68]
.L_080ea5de:
	ldr	r0, [sp, #84]
	cmp	r0, #144
	bne.n	.L_080ea5e8
	ldr	r1, [pc, #404]
	str	r1, [sp, #68]
.L_080ea5e8:
	ldr	r2, [sp, #84]
	cmp	r2, #146
	bne.n	.L_080ea5f4
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [sp, #68]
.L_080ea5f4:
	ldr	r4, [sp, #84]
	cmp	r4, #72
	bne.n	.L_080ea612
	ldr	r6, [sp, #100]
	ldr	r0, [pc, #372]
	movs	r7, #0
	adds	r5, r6, r0
.L_080ea602:
	adds	r1, r7, #0
	ldmia	r5!, {r0}
	adds	r1, #16
	adds	r7, #1
	bl	sub_08009020
	cmp	r7, #16
	bne.n	.L_080ea602
.L_080ea612:
	ldr	r1, [sp, #84]
	cmp	r1, #76
	bne.n	.L_080ea630
	ldr	r2, [sp, #100]
	ldr	r3, [pc, #344]
	movs	r7, #0
	adds	r5, r2, r3
.L_080ea620:
	adds	r1, r7, #0
	ldmia	r5!, {r0}
	adds	r1, #32
	adds	r7, #1
	bl	sub_08009020
	cmp	r7, #16
	bne.n	.L_080ea620
.L_080ea630:
	ldr	r3, [sp, #84]
	subs	r3, #116
	cmp	r3, #27
	bls.n	.L_080ea63a
	b.n	.L_080ea7ce
.L_080ea63a:
	cmp	r3, #0
	bge.n	.L_080ea642
	ldr	r3, [sp, #84]
	subs	r3, #113
.L_080ea642:
	asrs	r1, r3, #2
	cmp	r1, #6
	ble.n	.L_080ea64a
	movs	r1, #6
.L_080ea64a:
	ldr	r3, [pc, #308]
	ldrb	r3, [r3, r1]
	ldr	r2, [pc, #308]
	mov	sl, r3
	lsls	r3, r1, #1
	ldrh	r6, [r2, r3]
	movs	r4, #192
	lsls	r4, r4, #3
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	str	r5, [sp, #0]
	adds	r6, r6, r4
	bl	sub_080ed408
	movs	r1, #60
	mov	r9, r1
	ldr	r0, [sp, #100]
	mov	r2, r9
	mov	r3, sl
	subs	r2, r2, r3
	mov	r4, sl
	lsls	r3, r3, #1
	str	r3, [sp, #4]
	str	r4, [sp, #0]
	adds	r6, r0, r6
	movs	r5, #80
	ldr	r0, [pc, #256]
	subs	r5, r5, r3
	mov	r8, r3
	ldr	r4, [r0, #0]
	adds	r3, r5, #0
	ldr	r0, [sp, #104]
	adds	r1, r6, #0
	mov	r9, r2
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	movs	r1, #2
	str	r1, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	movs	r3, #7
	movs	r0, #47
	bl	sub_080ed408
	mov	r3, r8
	mov	r2, sl
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	ldr	r0, [pc, #208]
	adds	r3, r5, #0
	ldr	r4, [r0, #0]
	adds	r1, r6, #0
	ldr	r0, [sp, #104]
	movs	r2, #60
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	movs	r1, #2
	str	r1, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	movs	r3, #11
	movs	r0, #47
	bl	sub_080ed408
	mov	r3, r8
	mov	r2, sl
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	ldr	r5, [pc, #160]
	ldr	r0, [sp, #104]
	ldr	r4, [r5, #0]
	adds	r1, r6, #0
	mov	r2, r9
	movs	r3, #80
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #2
	str	r0, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #15
	movs	r0, #47
	bl	sub_080ed408
	mov	r2, r8
	mov	r1, sl
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	movs	r3, #80
	ldr	r4, [r5, #0]
	ldr	r0, [sp, #104]
	adds	r1, r6, #0
	movs	r2, #60
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	ldr	r3, [pc, #100]
	movs	r7, #0
	mov	r8, r3
.L_080ea72c:
	adds	r4, r7, #0
	movs	r5, #3
	ands	r4, r5
	str	r4, [sp, #8]
	bl	sub_08004458
	ldr	r3, [pc, #84]
	adds	r6, r0, #0
	ands	r6, r3
	adds	r0, r6, #0
	bl	sub_08002322
	ldr	r4, [sp, #8]
	adds	r5, r0, #0
	mov	r0, r8
	ldrb	r3, [r0, r4]
	lsls	r5, r5, #4
	lsrs	r3, r3, #1
	adds	r0, r6, #0
	asrs	r5, r5, #16
	subs	r5, r5, r3
	b.n	.L_080ea794
	.4byte 0x02010e00
	.4byte 0xfffffc00
	.4byte 0x080ede48
	.4byte 0x000077b4
	.4byte 0x000077b8
	.4byte 0x02010000
	.4byte 0x080edad0
	.4byte 0x000077d8
	.4byte 0xffffc000
	.4byte 0xfff80000
	.4byte 0x080eef28
	.4byte 0x080eef30
	.4byte 0x03001f0c
	.4byte 0x080eef4a
	.2byte 0xffff
	.2byte 0x0000
.L_080ea794:
	bl	sub_0800231c
	ldr	r2, [pc, #524]
	ldr	r4, [sp, #8]
	adds	r3, r0, #0
	ldrb	r0, [r2, r4]
	lsls	r3, r3, #4
	lsrs	r2, r0, #1
	ldr	r1, [pc, #516]
	asrs	r3, r3, #16
	subs	r3, r3, r2
	lsls	r2, r4, #1
	ldrh	r1, [r1, r2]
	ldr	r2, [sp, #100]
	mov	r6, r8
	adds	r1, r2, r1
	ldrb	r2, [r6, r4]
	adds	r5, #60
	str	r2, [sp, #0]
	str	r0, [sp, #4]
	adds	r3, #80
	ldr	r0, [sp, #104]
	adds	r2, r5, #0
	ldr	r4, [sp, #88]
	adds	r7, #1
	bl	sub_080072f4
	cmp	r7, #6
	bne.n	.L_080ea72c
.L_080ea7ce:
	ldr	r5, [sp, #84]
	cmp	r5, #143
	ble.n	.L_080ea8c2
	ldr	r0, [pc, #472]
	lsls	r6, r5, #4
	movs	r1, #3
	str	r1, [sp, #0]
	adds	r6, r6, r0
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	bl	sub_080ed408
	ldr	r7, [sp, #56]
	movs	r5, #64
	movs	r2, #24
	str	r2, [sp, #0]
	str	r5, [sp, #4]
	adds	r7, #188
	ldr	r1, [sp, #100]
	ldr	r0, [sp, #104]
	ldr	r4, [r7, #0]
	adds	r3, r6, #0
	movs	r2, #36
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	bl	sub_080ed408
	movs	r4, #24
	str	r5, [sp, #4]
	str	r4, [sp, #0]
	adds	r3, r6, #0
	ldr	r1, [sp, #100]
	movs	r2, #60
	ldr	r4, [r7, #0]
	ldr	r0, [sp, #104]
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	ldr	r3, [sp, #84]
	subs	r3, #144
	lsrs	r2, r3, #31
	ldr	r6, [sp, #100]
	ldr	r0, [pc, #372]
	adds	r3, r3, r2
	ldr	r5, [sp, #12]
	asrs	r1, r3, #1
	adds	r2, r6, r0
	movs	r3, #75
	mov	r8, r5
	str	r3, [r2, #0]
	cmp	r1, #6
	bgt.n	.L_080ea8c2
	lsls	r5, r1, #1
	adds	r5, r5, r1
	ldr	r3, [pc, #352]
	lsls	r5, r5, #7
	ldrh	r3, [r3, #14]
	adds	r5, r5, r1
	lsls	r5, r5, #1
	ldr	r1, [pc, #348]
	adds	r5, r5, r3
	movs	r2, #2
	adds	r5, r5, r1
	str	r2, [sp, #0]
	movs	r3, #3
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	bl	sub_080ed408
	ldr	r7, [r7, #0]
	adds	r6, r6, r5
	movs	r3, #4
	mov	r9, r7
	mov	sl, r6
	movs	r7, #0
	mov	fp, r3
.L_080ea882:
	lsls	r6, r7, #9
	adds	r0, r6, #0
	bl	sub_08002322
	mov	r5, r8
	muls	r5, r0
	mov	r4, fp
	adds	r0, r6, #0
	asrs	r5, r5, #16
	subs	r5, r5, r4
	bl	sub_0800231c
	mov	r3, r8
	muls	r3, r0
	adds	r5, #60
	movs	r0, #16
	asrs	r3, r3, #17
	movs	r6, #8
	str	r0, [sp, #4]
	adds	r3, #72
	str	r6, [sp, #0]
	ldr	r0, [sp, #104]
	mov	r1, sl
	adds	r2, r5, #0
	adds	r7, #1
	bl	sub_08007308
	cmp	r7, #128
	bne.n	.L_080ea882
	movs	r0, #47
	bl	sub_08002dd8
.L_080ea8c2:
	ldr	r1, [sp, #100]
	ldr	r3, [pc, #248]
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r4, [sp, #12]
	ldr	r5, [sp, #84]
	adds	r4, #10
	adds	r5, #1
	str	r4, [sp, #12]
	str	r5, [sp, #84]
	cmp	r5, #160
	beq.n	.L_080ea8f6
	cmp	r5, #4
	bgt.n	.L_080ea8e8
	b.n	.L_080ea34e
.L_080ea8e8:
	ldr	r3, [pc, #216]
	ldr	r3, [r3, #0]
	movs	r6, #3
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L_080ea8f6
	b.n	.L_080ea34e
.L_080ea8f6:
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r3, [pc, #204]
	ldr	r0, [sp, #104]
	movs	r2, #0
	bl	sub_080072f0
	ldr	r1, [pc, #196]
	ldr	r0, [sp, #100]
	movs	r7, #0
	adds	r5, r0, r1
.L_080ea90c:
	ldmia	r5!, {r0}
	adds	r7, #1
	bl	sub_08009038
	cmp	r7, #16
	bne.n	.L_080ea90c
	movs	r2, #13
	negs	r2, r2
	ldr	r6, [pc, #172]
	movs	r7, #0
	mov	r8, r2
.L_080ea922:
	movs	r0, #195
	lsls	r0, r0, #1
	bl	sub_08009030
	ldr	r3, [sp, #100]
	adds	r5, r0, #0
	str	r5, [r6, r3]
	cmp	r5, #0
	beq.n	.L_080ea958
	adds	r2, r5, #0
	adds	r2, #38
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #3
	adds	r0, r7, #0
	bl	sub_080022fc
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_08009020
	ldr	r4, [sp, #100]
	ldr	r2, [r6, r4]
	ldrb	r3, [r2, #9]
	mov	r5, r8
	ands	r3, r5
	strb	r3, [r2, #9]
.L_080ea958:
	adds	r7, #1
	adds	r6, #4
	cmp	r7, #16
	bne.n	.L_080ea922
	movs	r0, #46
	bl	sub_08002dd8
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	sub_080ed408
	ldr	r3, [pc, #88]
	adds	r3, #184
	ldr	r3, [r3, #0]
	ldr	r6, [sp, #100]
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r1, r6, r2
	str	r3, [sp, #88]
	ldr	r0, [pc, #76]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	ldr	r0, [pc, #68]
	ldr	r1, [sp, #80]
	movs	r7, #0
	movs	r4, #1
.L_080ea998:
	ldrb	r3, [r1, #0]
	adds	r2, r3, #0
	cmp	r2, #32
	bls.n	.L_080ea9dc
	adds	r3, #224
	strb	r3, [r1, #0]
	b.n	.L_080ea9e2
	movs	r0, r0
	.4byte 0x080eef50
	.4byte 0x080eef3e
	.4byte 0xfffff720
	.4byte 0x00007784
	.4byte 0x080ede48
	.4byte 0x00002710
	.4byte 0x00007824
	.4byte 0x03001b04
	.4byte 0x03000168
	.4byte 0x000077d8
	.4byte 0x03001e50
	.4byte 0x00000064
	.2byte 0x0302
	.2byte 0x0000
.L_080ea9dc:
	cmp	r2, #0
	beq.n	.L_080ea9e2
	strb	r4, [r1, #0]
.L_080ea9e2:
	adds	r7, #1
	adds	r1, #1
	cmp	r7, r0
	bne.n	.L_080ea998
	ldr	r4, [sp, #100]
	ldr	r5, [pc, #88]
	ldr	r6, [pc, #92]
	adds	r3, r4, r5
	movs	r5, #0
	str	r5, [r3, #0]
	adds	r3, r4, r6
	str	r5, [r3, #0]
	ldr	r1, [pc, #84]
	movs	r0, #1
	movs	r2, #0
	bl	sub_080b5040
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #76]
	adds	r2, #8
	str	r3, [r2, #0]
	ldr	r3, [pc, #48]
	add	r4, sp, #140
	subs	r2, #28
	strh	r3, [r2, #0]
	str	r5, [r4, #0]
	ldr	r3, [pc, #64]
	adds	r0, r4, #0
	ldr	r1, [sp, #104]
	ldr	r2, [pc, #60]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #60
	ldr	r6, [sp, #100]
	str	r0, [sp, #52]
	movs	r1, #44
	movs	r0, #239
	str	r1, [sp, #48]
	movs	r3, #0
	lsls	r0, r0, #7
	ldr	r1, [pc, #44]
	str	r3, [sp, #32]
	str	r3, [sp, #40]
	b.n	.L_080eaa68
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000784
	.4byte 0x000077b4
	.4byte 0x000077b8
	.4byte 0x0000003e
	.4byte 0x04000020
	.4byte 0xffffc400
	.4byte 0x040000d4
	.4byte 0x85001000
	.2byte 0x7784
	.2byte 0x0000
.L_080eaa68:
	movs	r5, #2
	adds	r3, r6, r0
	movs	r2, #6
	str	r2, [sp, #44]
	str	r5, [sp, #36]
	adds	r2, r6, r1
	str	r5, [r3, #0]
	movs	r3, #75
	str	r3, [r2, #0]
	ldr	r2, [sp, #32]
	str	r2, [r4, #0]
	ldr	r3, [pc, #252]
	adds	r0, r4, #0
	ldr	r1, [sp, #100]
	ldr	r2, [pc, #248]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #0
	str	r3, [sp, #84]
.L_080eaa8e:
	ldr	r4, [sp, #84]
	cmp	r4, #66
	bne.n	.L_080eaaa0
	movs	r0, #145
	bl	sub_080f9010
	movs	r0, #141
	bl	sub_080f9010
.L_080eaaa0:
	ldr	r5, [sp, #84]
	cmp	r5, #155
	bne.n	.L_080eaaac
	movs	r0, #162
	bl	sub_080f9010
.L_080eaaac:
	ldr	r6, [sp, #84]
	cmp	r6, #217
	bne.n	.L_080eaab8
	movs	r0, #156
	bl	sub_080f9010
.L_080eaab8:
	movs	r1, #140
	ldr	r0, [sp, #84]
	lsls	r1, r1, #1
	cmp	r0, r1
	bne.n	.L_080eaac8
	movs	r0, #157
	bl	sub_080f9010
.L_080eaac8:
	movs	r3, #150
	ldr	r2, [sp, #84]
	lsls	r3, r3, #1
	cmp	r2, r3
	bne.n	.L_080eaad8
	movs	r0, #145
	bl	sub_080b50e8
.L_080eaad8:
	ldr	r3, [pc, #168]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080eabaa
	ldr	r3, [sp, #84]
	subs	r3, #5
	cmp	r3, #144
	bhi.n	.L_080eab6c
	ldr	r1, [pc, #152]
	movs	r4, #150
	str	r4, [sp, #84]
	ldr	r5, [pc, #152]
	ldrh	r3, [r5, #0]
	adds	r0, r3, #0
	movs	r6, #130
	ldr	r2, [pc, #144]
	lsls	r6, r6, #2
	strh	r6, [r2, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_080eab22
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	adds	r3, #4
	strh	r2, [r1, #0]
	movs	r2, #128
	stmia	r3!, {r2}
	ldr	r2, [pc, #116]
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_080eab22:
	ldr	r3, [pc, #104]
	strh	r0, [r3, #0]
	ldrh	r3, [r3, #0]
	adds	r0, r3, #0
	movs	r4, #130
	ldr	r5, [pc, #92]
	lsls	r4, r4, #2
	strh	r4, [r5, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_080eab54
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r1
	adds	r3, #4
	adds	r2, #1
	movs	r6, #0
	stmia	r3!, {r6}
	strh	r2, [r1, #0]
	ldr	r2, [pc, #72]
	stmia	r3!, {r2}
	movs	r2, #192
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_080eab54:
	ldr	r1, [pc, #52]
	strh	r0, [r1, #0]
	ldr	r0, [pc, #60]
	movs	r3, #128
	ldr	r2, [sp, #100]
	lsls	r3, r3, #7
	adds	r1, r2, r3
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	b.n	.L_080eabaa
.L_080eab6c:
	ldr	r3, [sp, #84]
	subs	r3, #155
	cmp	r3, #58
	bhi.n	.L_080eab9c
	movs	r4, #214
	str	r4, [sp, #84]
	b.n	.L_080eabaa
	movs	r0, r0
	.4byte 0x040000d4
	.4byte 0x85000e10
	.4byte 0x03001b04
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x04000020
	.4byte 0x04000028
	.2byte 0x0070
	.2byte 0x0000
.L_080eab9c:
	ldr	r3, [sp, #84]
	subs	r3, #219
	cmp	r3, #60
	bhi.n	.L_080eabaa
	movs	r5, #140
	lsls	r5, r5, #1
	str	r5, [sp, #84]
.L_080eabaa:
	ldr	r6, [sp, #84]
	cmp	r6, #64
	bne.n	.L_080eac4c
	ldr	r1, [pc, #208]
	ldr	r0, [pc, #212]
	ldrh	r3, [r0, #0]
	adds	r0, r3, #0
	movs	r2, #130
	ldr	r3, [pc, #204]
	lsls	r2, r2, #2
	strh	r2, [r3, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_080eabe2
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	adds	r3, #4
	strh	r2, [r1, #0]
	movs	r2, #128
	stmia	r3!, {r2}
	ldr	r2, [pc, #176]
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_080eabe2:
	ldr	r4, [pc, #164]
	strh	r0, [r4, #0]
	ldrh	r3, [r4, #0]
	adds	r0, r3, #0
	movs	r5, #130
	ldr	r6, [pc, #152]
	lsls	r5, r5, #2
	strh	r5, [r6, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_080eac14
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r1
	adds	r2, #1
	adds	r3, #4
	strh	r2, [r1, #0]
	movs	r1, #0
	stmia	r3!, {r1}
	ldr	r2, [pc, #132]
	stmia	r3!, {r2}
	movs	r2, #192
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_080eac14:
	ldr	r2, [pc, #112]
	strh	r0, [r2, #0]
	movs	r3, #0
	add	r4, sp, #140
	str	r3, [sp, #140]
	adds	r0, r4, #0
	ldr	r3, [pc, #112]
	ldr	r1, [sp, #104]
	ldr	r2, [pc, #112]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r5, [sp, #100]
	movs	r6, #128
	lsls	r6, r6, #7
	ldr	r0, [pc, #104]
	adds	r1, r5, r6
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	movs	r2, #192
	lsls	r2, r2, #7
	adds	r1, r5, r2
	ldr	r0, [pc, #92]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
.L_080eac4c:
	ldr	r3, [sp, #84]
	cmp	r3, #66
	bne.n	.L_080eac66
	ldr	r2, [pc, #80]
	ldr	r1, [pc, #40]
	movs	r7, #0
.L_080eac58:
	ldrh	r3, [r2, #0]
	adds	r7, #1
	eors	r3, r1
	strh	r3, [r2, #0]
	adds	r2, #2
	cmp	r7, #128
	bne.n	.L_080eac58
.L_080eac66:
	ldr	r4, [sp, #84]
	cmp	r4, #69
	bne.n	.L_080eacac
	ldr	r3, [pc, #56]
	add	r5, sp, #140
	str	r3, [sp, #140]
	adds	r0, r5, #0
	ldr	r3, [pc, #28]
	ldr	r1, [sp, #104]
	ldr	r2, [pc, #28]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L_080eacac
	.4byte 0x00007fff
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x04000020
	.4byte 0x04000028
	.4byte 0x040000d4
	.4byte 0x85001000
	.4byte 0x00000070
	.4byte 0x00000065
	.4byte 0x050000c0
	.2byte 0x3f3f
	.2byte 0x3f3f
.L_080eacac:
	ldr	r6, [sp, #84]
	cmp	r6, #70
	bne.n	.L_080eacbc
	movs	r0, #1
	ldr	r1, [pc, #280]
	movs	r2, #7
	bl	sub_080b5038
.L_080eacbc:
	ldr	r0, [sp, #84]
	cmp	r0, #150
	bne.n	.L_080ead02
	movs	r3, #0
	movs	r1, #112
	movs	r2, #32
	movs	r4, #4
	movs	r5, #8
	add	r6, sp, #140
	str	r1, [sp, #52]
	str	r2, [sp, #48]
	str	r3, [sp, #40]
	str	r3, [sp, #140]
	str	r4, [sp, #36]
	str	r5, [sp, #44]
	ldr	r3, [pc, #248]
	adds	r0, r6, #0
	ldr	r1, [sp, #100]
	ldr	r2, [pc, #244]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #244]
	adds	r1, r6, #0
	str	r3, [sp, #140]
	adds	r0, r1, #0
	ldr	r3, [pc, #228]
	ldr	r1, [sp, #104]
	ldr	r2, [pc, #236]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	ldr	r1, [pc, #232]
	movs	r2, #0
	bl	sub_080b5040
.L_080ead02:
	ldr	r2, [sp, #84]
	cmp	r2, #214
	bne.n	.L_080ead82
	movs	r3, #0
	add	r4, sp, #140
	str	r3, [sp, #140]
	adds	r0, r4, #0
	ldr	r3, [pc, #192]
	ldr	r1, [sp, #100]
	ldr	r2, [pc, #192]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #192]
	adds	r5, r4, #0
	str	r3, [sp, #140]
	adds	r0, r5, #0
	ldr	r3, [pc, #176]
	ldr	r1, [sp, #104]
	ldr	r2, [pc, #184]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	ldr	r1, [pc, #184]
	movs	r2, #0
	bl	sub_080b5040
	movs	r0, #225
	ldr	r6, [sp, #100]
	lsls	r0, r0, #7
	movs	r7, #0
	adds	r5, r6, r0
.L_080ead40:
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r3, #96
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r3, #16
	lsls	r3, r3, #15
	ldr	r2, [r5, #0]
	str	r3, [r5, #16]
	movs	r3, #128
	asrs	r2, r2, #7
	lsls	r3, r3, #8
	subs	r3, r3, r2
	str	r3, [r5, #8]
	adds	r7, #1
	movs	r3, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #16
	bne.n	.L_080ead40
.L_080ead82:
	movs	r2, #140
	ldr	r1, [sp, #84]
	lsls	r2, r2, #1
	cmp	r1, r2
	bne.n	.L_080eae44
	bl	sub_080d67dc
	movs	r3, #0
	add	r4, sp, #140
	str	r3, [sp, #140]
	adds	r0, r4, #0
	ldr	r3, [pc, #56]
	ldr	r1, [sp, #100]
	ldr	r2, [pc, #56]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #72]
	adds	r5, r4, #0
	str	r3, [sp, #140]
	adds	r0, r5, #0
	ldr	r3, [pc, #40]
	ldr	r1, [sp, #104]
	ldr	r2, [pc, #48]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [sp, #96]
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #16]
	movs	r6, #0
	strh	r3, [r2, #0]
	str	r6, [r0, #16]
	ldr	r1, [sp, #100]
	movs	r2, #225
	lsls	r2, r2, #7
	movs	r7, #0
	adds	r5, r1, r2
	b.n	.L_080eadf4
	.4byte 0x00001010
	.4byte 0x0000003e
	.4byte 0x040000d4
	.4byte 0x85000e10
	.4byte 0x3f3f3f3f
	.4byte 0x85001000
	.4byte 0x00000036
	.4byte 0x0000003a
	.4byte 0x01010101
	.2byte 0x0052
	.2byte 0x0400
.L_080eadf4:
	bl	sub_08004458
	movs	r3, #63
	movs	r4, #128
	lsls	r4, r4, #1
	ands	r3, r0
	adds	r3, r3, r4
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r3, #96
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r3, #16
	lsls	r3, r3, #15
	movs	r6, #0
	adds	r7, #1
	str	r3, [r5, #16]
	str	r6, [r5, #8]
	adds	r5, #28
	cmp	r7, #16
	bne.n	.L_080eadf4
	ldr	r1, [sp, #100]
	movs	r2, #239
	movs	r0, #136
	lsls	r2, r2, #7
	ldr	r4, [pc, #72]
	adds	r3, r1, r2
	lsls	r0, r0, #1
	str	r0, [sp, #32]
	str	r6, [r3, #0]
	adds	r3, r1, r4
	str	r6, [r3, #0]
.L_080eae44:
	ldr	r5, [sp, #84]
	ldr	r6, [pc, #60]
	cmp	r5, r6
	ble.n	.L_080eae9e
	ldr	r5, [pc, #56]
	ldr	r6, [pc, #44]
	movs	r7, #0
.L_080eae52:
	ldrh	r2, [r5, #0]
	lsls	r3, r2, #16
	lsrs	r0, r3, #26
	ands	r0, r6
	lsrs	r1, r3, #21
	movs	r4, #31
	ands	r1, r6
	ands	r4, r2
	adds	r0, #1
	adds	r1, #1
	adds	r4, #1
	cmp	r0, #31
	ble.n	.L_080eae6e
	movs	r0, #31
.L_080eae6e:
	cmp	r1, #31
	ble.n	.L_080eae74
	movs	r1, #31
.L_080eae74:
	cmp	r4, #31
	ble.n	.L_080eae8c
	movs	r4, #31
	b.n	.L_080eae8c
	.4byte 0x0000001f
	.4byte 0x00007784
	.4byte 0x00000117
	.2byte 0x0002
	.2byte 0x0500
.L_080eae8c:
	lsls	r3, r0, #10
	lsls	r2, r1, #5
	orrs	r3, r2
	orrs	r3, r4
	adds	r7, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r7, #63
	bne.n	.L_080eae52
.L_080eae9e:
	ldr	r0, [sp, #84]
	cmp	r0, #182
	bne.n	.L_080eaeb6
	ldr	r3, [pc, #788]
	add	r1, sp, #140
	str	r3, [sp, #140]
	adds	r0, r1, #0
	ldr	r3, [pc, #784]
	ldr	r1, [sp, #104]
	ldr	r2, [pc, #784]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_080eaeb6:
	ldr	r2, [sp, #84]
	cmp	r2, #63
	bgt.n	.L_080eaf88
	ldr	r4, [sp, #84]
	movs	r3, #7
	ldr	r5, [sp, #100]
	movs	r6, #128
	subs	r2, #4
	lsls	r6, r6, #7
	ands	r3, r4
	mov	r8, r2
	adds	r7, r5, r6
	cmp	r3, #3
	ble.n	.L_080eaed8
	ldr	r0, [sp, #100]
	ldr	r1, [pc, #752]
	adds	r7, r0, r1
.L_080eaed8:
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	str	r5, [sp, #0]
	bl	sub_080ed408
	movs	r2, #24
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	ldr	r3, [pc, #732]
	mov	r6, r8
	subs	r6, #24
	ldr	r0, [sp, #104]
	ldr	r4, [r3, #0]
	adds	r1, r7, #0
	adds	r3, r6, #0
	movs	r2, #36
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	str	r5, [sp, #0]
	bl	sub_080ed408
	movs	r4, #24
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [pc, #684]
	adds	r3, r6, #0
	ldr	r4, [r0, #0]
	adds	r1, r7, #0
	ldr	r0, [sp, #104]
	movs	r2, #59
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	movs	r1, #7
	movs	r2, #7
	movs	r3, #11
	movs	r0, #47
	str	r5, [sp, #0]
	bl	sub_080ed408
	movs	r1, #24
	str	r1, [sp, #0]
	str	r1, [sp, #4]
	ldr	r2, [pc, #644]
	adds	r6, #23
	ldr	r4, [r2, #0]
	ldr	r0, [sp, #104]
	adds	r1, r7, #0
	movs	r2, #36
	adds	r3, r6, #0
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	movs	r1, #7
	movs	r2, #7
	movs	r3, #15
	movs	r0, #47
	str	r5, [sp, #0]
	bl	sub_080ed408
	movs	r3, #24
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	ldr	r5, [pc, #600]
	ldr	r0, [sp, #104]
	ldr	r4, [r5, #0]
	adds	r1, r7, #0
	movs	r2, #59
	adds	r3, r6, #0
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
.L_080eaf88:
	ldr	r3, [sp, #84]
	subs	r3, #64
	cmp	r3, #1
	bhi.n	.L_080eafac
	movs	r3, #16
	ldr	r6, [sp, #100]
	movs	r2, #192
	lsls	r2, r2, #7
	str	r3, [sp, #0]
	movs	r3, #17
	str	r3, [sp, #4]
	adds	r1, r6, r2
	ldr	r0, [sp, #104]
	movs	r2, #52
	movs	r3, #51
	ldr	r4, [sp, #88]
	bl	sub_080072f4
.L_080eafac:
	ldr	r3, [sp, #84]
	subs	r3, #66
	cmp	r3, #1
	bhi.n	.L_080eafce
	ldr	r2, [pc, #536]
	ldr	r6, [sp, #100]
	movs	r3, #41
	movs	r5, #24
	str	r3, [sp, #4]
	adds	r1, r6, r2
	str	r5, [sp, #0]
	ldr	r0, [sp, #104]
	movs	r2, #48
	movs	r3, #40
	ldr	r4, [sp, #88]
	bl	sub_080072f4
.L_080eafce:
	ldr	r3, [sp, #84]
	subs	r3, #68
	cmp	r3, #7
	bhi.n	.L_080eb054
	ldr	r5, [sp, #84]
	movs	r6, #76
	subs	r6, r6, r5
	ldr	r0, [sp, #100]
	ldr	r1, [pc, #500]
	lsrs	r5, r6, #31
	adds	r5, r6, r5
	movs	r2, #38
	adds	r0, r0, r1
	mov	r8, r2
	asrs	r5, r5, #1
	ldr	r2, [sp, #88]
	movs	r4, #49
	mov	fp, r0
	subs	r4, r4, r5
	movs	r1, #44
	mov	r3, r8
	movs	r0, #22
	subs	r3, r3, r6
	str	r0, [sp, #0]
	mov	sl, r1
	str	r1, [sp, #4]
	mov	r9, r2
	str	r4, [sp, #8]
	ldr	r0, [sp, #104]
	adds	r2, r4, #0
	mov	r1, fp
	mov	r8, r3
	bl	sub_08007308
	ldr	r4, [sp, #8]
	adds	r6, #38
	movs	r3, #22
	mov	r0, sl
	adds	r2, r4, #0
	str	r3, [sp, #0]
	str	r0, [sp, #4]
	mov	r1, fp
	ldr	r0, [sp, #104]
	adds	r3, r6, #0
	bl	sub_08007308
	adds	r5, #49
	movs	r1, #22
	mov	r2, sl
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	mov	r1, fp
	adds	r2, r5, #0
	mov	r3, r8
	ldr	r0, [sp, #104]
	bl	sub_08007308
	movs	r3, #22
	mov	r4, sl
	str	r3, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #104]
	mov	r1, fp
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	sub_08007308
.L_080eb054:
	ldr	r3, [sp, #84]
	subs	r3, #78
	cmp	r3, #1
	bhi.n	.L_080eb078
	movs	r3, #16
	ldr	r5, [sp, #100]
	movs	r6, #192
	str	r3, [sp, #0]
	lsls	r6, r6, #7
	movs	r3, #17
	str	r3, [sp, #4]
	ldr	r0, [sp, #104]
	adds	r1, r5, r6
	movs	r2, #52
	movs	r3, #51
	ldr	r4, [sp, #88]
	bl	sub_080072f4
.L_080eb078:
	ldr	r3, [sp, #84]
	subs	r3, #80
	cmp	r3, #1
	bhi.n	.L_080eb09a
	ldr	r2, [pc, #332]
	ldr	r6, [sp, #100]
	movs	r3, #41
	movs	r5, #24
	str	r3, [sp, #4]
	adds	r1, r6, r2
	str	r5, [sp, #0]
	ldr	r0, [sp, #104]
	movs	r2, #48
	movs	r3, #40
	ldr	r4, [sp, #88]
	bl	sub_080072f4
.L_080eb09a:
	ldr	r3, [sp, #84]
	subs	r3, #82
	cmp	r3, #3
	bhi.n	.L_080eb126
	ldr	r5, [sp, #84]
	ldr	r0, [pc, #300]
	lsls	r7, r5, #1
	ldr	r6, [sp, #100]
	adds	r2, r7, #0
	subs	r2, #164
	adds	r0, r6, r0
	str	r0, [sp, #28]
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080eb0bc
	adds	r3, r7, #0
	subs	r3, #161
.L_080eb0bc:
	asrs	r3, r3, #2
	movs	r5, #49
	subs	r5, r5, r3
	mov	r9, r3
	movs	r6, #38
	ldr	r3, [sp, #88]
	subs	r6, r6, r2
	movs	r1, #22
	movs	r2, #44
	str	r1, [sp, #0]
	mov	r8, r2
	str	r2, [sp, #4]
	mov	sl, r3
	ldr	r1, [sp, #28]
	ldr	r0, [sp, #104]
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	sub_0800730c
	subs	r7, #126
	movs	r4, #22
	mov	fp, r7
	mov	r0, r8
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r1, [sp, #28]
	adds	r2, r5, #0
	ldr	r0, [sp, #104]
	mov	r3, fp
	bl	sub_0800730c
	movs	r1, #49
	add	r9, r1
	movs	r2, #22
	mov	r3, r8
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	ldr	r1, [sp, #28]
	mov	r2, r9
	adds	r3, r6, #0
	ldr	r0, [sp, #104]
	bl	sub_0800730c
	mov	r5, r8
	movs	r4, #22
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #104]
	ldr	r1, [sp, #28]
	mov	r2, r9
	mov	r3, fp
	bl	sub_0800730c
.L_080eb126:
	ldr	r3, [sp, #84]
	subs	r3, #72
	cmp	r3, #3
	bhi.n	.L_080eb190
	ldr	r6, [sp, #84]
	ldr	r0, [pc, #164]
	lsls	r3, r6, #4
	ldr	r2, [pc, #164]
	adds	r0, r0, r3
	movs	r1, #5
	movs	r3, #20
	mov	r8, r0
	movs	r7, #0
	mov	fp, r1
	mov	r9, r2
	mov	sl, r3
.L_080eb146:
	lsls	r6, r7, #8
	adds	r0, r6, #0
	bl	sub_08002322
	mov	r5, r8
	muls	r5, r0
	mov	r4, fp
	adds	r0, r6, #0
	asrs	r5, r5, #16
	subs	r5, r5, r4
	bl	sub_0800231c
	mov	r3, r8
	muls	r3, r0
	mov	r2, sl
	subs	r2, #2
	mov	r6, r9
	ldrh	r1, [r6, r2]
	ldr	r0, [sp, #80]
	adds	r5, #60
	movs	r2, #10
	mov	r4, sl
	asrs	r3, r3, #17
	movs	r6, #128
	adds	r1, r0, r1
	str	r2, [sp, #0]
	adds	r3, #50
	adds	r2, r5, #0
	str	r4, [sp, #4]
	ldr	r0, [sp, #104]
	ldr	r5, [sp, #88]
	adds	r7, #1
	lsls	r6, r6, #1
	bl	sub_080072f8
	cmp	r7, r6
	bne.n	.L_080eb146
.L_080eb190:
	ldr	r0, [sp, #84]
	cmp	r0, #85
	bgt.n	.L_080eb198
	b.n	.L_080eb364
.L_080eb198:
	cmp	r0, #213
	ble.n	.L_080eb19e
	b.n	.L_080eb364
.L_080eb19e:
	ldr	r1, [sp, #36]
	movs	r7, #0
	cmp	r1, #0
	bne.n	.L_080eb1a8
	b.n	.L_080eb364
.L_080eb1a8:
	ldr	r2, [sp, #40]
	movs	r3, #0
	mov	lr, r2
	mov	sl, r3
	mov	r8, r2
	cmp	r2, #0
	bge.n	.L_080eb1b8
	b.n	.L_080eb354
.L_080eb1b8:
	b.n	.L_080eb1e0
	movs	r0, r0
	.4byte 0x3f3f3f3f
	.4byte 0x040000d4
	.4byte 0x85001000
	.4byte 0x00004240
	.4byte 0x03001f0c
	.4byte 0x00006110
	.4byte 0x000064e8
	.4byte 0xfffffb90
	.2byte 0xde48
	.2byte 0x080e
.L_080eb1e0:
	mov	r4, lr
	lsrs	r3, r4, #31
	add	r3, lr
	asrs	r3, r3, #1
	mov	r9, r3
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_080eb1f2
	adds	r3, #7
.L_080eb1f2:
	ldr	r5, [sp, #52]
	ldr	r6, [sp, #52]
	ldr	r1, [sp, #44]
	asrs	r3, r3, #3
	adds	r0, r5, r3
	subs	r4, r6, r3
	mov	r3, sl
	muls	r3, r1
	cmp	r3, #0
	bge.n	.L_080eb208
	adds	r3, #7
.L_080eb208:
	ldr	r5, [sp, #48]
	asrs	r3, r3, #3
	mov	r6, sl
	adds	r2, r5, r3
	lsls	r3, r6, #3
	cmp	r3, #0
	bge.n	.L_080eb218
	adds	r3, #7
.L_080eb218:
	ldr	r5, [sp, #48]
	asrs	r3, r3, #3
	subs	r1, r5, r3
	cmp	r1, #0
	bge.n	.L_080eb224
	movs	r1, #0
.L_080eb224:
	cmp	r2, #119
	ble.n	.L_080eb22a
	movs	r2, #119
.L_080eb22a:
	cmp	r4, #0
	bge.n	.L_080eb230
	movs	r4, #0
.L_080eb230:
	cmp	r0, #119
	ble.n	.L_080eb236
	movs	r0, #119
.L_080eb236:
	lsls	r3, r2, #4
	subs	r3, r3, r2
	lsls	r5, r3, #3
	ldr	r3, [sp, #100]
	movs	r2, #20
	adds	r6, r5, r0
	strb	r2, [r3, r6]
	lsls	r3, r1, #4
	subs	r3, r3, r1
	ldr	r6, [sp, #100]
	lsls	r1, r3, #3
	adds	r3, r1, r0
	strb	r2, [r6, r3]
	adds	r3, r5, r4
	strb	r2, [r6, r3]
	adds	r3, r1, r4
	strb	r2, [r6, r3]
	ldr	r3, [sp, #52]
	ldr	r2, [sp, #52]
	add	r3, r9
	mov	r4, r9
	adds	r0, r3, #1
	subs	r3, r2, r4
	adds	r4, r3, #1
	cmp	r4, #0
	bge.n	.L_080eb26c
	movs	r4, #0
.L_080eb26c:
	cmp	r0, #119
	ble.n	.L_080eb272
	movs	r0, #119
.L_080eb272:
	ldr	r6, [sp, #100]
	movs	r2, #20
	adds	r3, r5, r0
	strb	r2, [r6, r3]
	adds	r3, r1, r0
	strb	r2, [r6, r3]
	adds	r3, r5, r4
	strb	r2, [r6, r3]
	mov	r0, sl
	adds	r3, r1, r4
	strb	r2, [r6, r3]
	lsrs	r3, r0, #31
	add	r3, sl
	asrs	r3, r3, #1
	mov	ip, r3
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_080eb298
	adds	r3, #7
.L_080eb298:
	ldr	r1, [sp, #52]
	ldr	r2, [sp, #52]
	ldr	r5, [sp, #44]
	asrs	r3, r3, #3
	adds	r0, r1, r3
	subs	r4, r2, r3
	mov	r3, lr
	muls	r3, r5
	cmp	r3, #0
	bge.n	.L_080eb2ae
	adds	r3, #7
.L_080eb2ae:
	ldr	r6, [sp, #48]
	asrs	r3, r3, #3
	mov	r1, lr
	adds	r2, r6, r3
	lsls	r3, r1, #3
	cmp	r3, #0
	bge.n	.L_080eb2be
	adds	r3, #7
.L_080eb2be:
	ldr	r5, [sp, #48]
	asrs	r3, r3, #3
	subs	r1, r5, r3
	cmp	r4, #0
	bge.n	.L_080eb2ca
	movs	r4, #0
.L_080eb2ca:
	cmp	r0, #119
	ble.n	.L_080eb2d0
	movs	r0, #119
.L_080eb2d0:
	cmp	r1, #0
	bge.n	.L_080eb2d6
	movs	r1, #0
.L_080eb2d6:
	cmp	r2, #119
	ble.n	.L_080eb2dc
	movs	r2, #119
.L_080eb2dc:
	lsls	r3, r2, #4
	subs	r3, r3, r2
	lsls	r6, r3, #3
	ldr	r2, [sp, #100]
	movs	r5, #20
	adds	r3, r6, r0
	strb	r5, [r2, r3]
	lsls	r3, r1, #4
	subs	r3, r3, r1
	lsls	r2, r3, #3
	adds	r3, r2, r0
	ldr	r0, [sp, #100]
	strb	r5, [r0, r3]
	adds	r3, r6, r4
	strb	r5, [r0, r3]
	adds	r3, r2, r4
	strb	r5, [r0, r3]
	ldr	r3, [sp, #52]
	ldr	r1, [sp, #52]
	add	r3, ip
	mov	r4, ip
	adds	r0, r3, #1
	subs	r3, r1, r4
	adds	r4, r3, #1
	cmp	r4, #0
	bge.n	.L_080eb312
	movs	r4, #0
.L_080eb312:
	cmp	r0, #119
	ble.n	.L_080eb318
	movs	r0, #119
.L_080eb318:
	ldr	r1, [sp, #100]
	adds	r3, r6, r0
	strb	r5, [r1, r3]
	adds	r3, r2, r0
	strb	r5, [r1, r3]
	adds	r3, r6, r4
	strb	r5, [r1, r3]
	adds	r3, r2, r4
	mov	r2, sl
	mov	r4, r8
	strb	r5, [r1, r3]
	lsls	r3, r2, #1
	subs	r3, r4, r3
	subs	r3, #1
	mov	r8, r3
	cmp	r3, #0
	bge.n	.L_080eb34a
	mov	r5, lr
	lsls	r3, r5, #1
	add	r3, r8
	movs	r6, #1
	subs	r3, #2
	negs	r6, r6
	mov	r8, r3
	add	lr, r6
.L_080eb34a:
	movs	r0, #1
	add	sl, r0
	cmp	lr, sl
	blt.n	.L_080eb354
	b.n	.L_080eb1e0
.L_080eb354:
	ldr	r1, [sp, #40]
	ldr	r2, [sp, #36]
	adds	r1, #1
	adds	r7, #1
	str	r1, [sp, #40]
	cmp	r7, r2
	beq.n	.L_080eb364
	b.n	.L_080eb1a8
.L_080eb364:
	ldr	r3, [sp, #84]
	subs	r3, #86
	mov	r8, r3
	cmp	r3, #63
	bhi.n	.L_080eb3e4
	ldr	r5, [sp, #84]
	ldr	r4, [sp, #84]
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	subs	r4, #70
	asrs	r3, r3, #1
	ldr	r6, [pc, #656]
	movs	r7, #0
	mov	r9, r4
	mov	sl, r3
.L_080eb382:
	adds	r0, r6, #0
	bl	sub_08002322
	mov	r3, r8
	muls	r3, r0
	adds	r0, r6, #0
	asrs	r5, r3, #16
	bl	sub_0800231c
	mov	r3, r9
	muls	r3, r0
	mov	r1, sl
	adds	r2, r1, r7
	asrs	r0, r3, #16
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080eb3a6
	adds	r3, r2, #3
.L_080eb3a6:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	lsls	r1, r3, #3
	subs	r1, r1, r3
	lsls	r1, r1, #2
	ldr	r2, [sp, #100]
	subs	r1, r1, r3
	lsls	r1, r1, #6
	movs	r3, #128
	adds	r1, r2, r1
	lsls	r3, r3, #7
	adds	r1, r1, r3
	adds	r3, r0, #0
	movs	r0, #32
	adds	r2, r5, #0
	str	r0, [sp, #0]
	movs	r5, #128
	movs	r0, #54
	str	r0, [sp, #4]
	adds	r2, #44
	adds	r3, #17
	ldr	r0, [sp, #104]
	ldr	r4, [sp, #88]
	lsls	r5, r5, #5
	adds	r7, #1
	bl	sub_080072f4
	adds	r6, r6, r5
	cmp	r7, #9
	bne.n	.L_080eb382
.L_080eb3e4:
	movs	r3, #3
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	str	r3, [sp, #0]
	bl	sub_080ed408
	ldr	r6, [pc, #540]
	ldr	r0, [sp, #84]
	ldr	r4, [r6, #0]
	str	r4, [sp, #92]
	cmp	r0, #85
	ble.n	.L_080eb410
	movs	r3, #120
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	ldr	r0, [sp, #104]
	ldr	r1, [sp, #100]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080072f4
.L_080eb410:
	ldr	r1, [sp, #84]
	ldr	r2, [pc, #512]
	cmp	r1, r2
	ble.n	.L_080eb41e
	ldr	r3, [sp, #32]
	subs	r3, #8
	str	r3, [sp, #32]
.L_080eb41e:
	ldr	r4, [sp, #84]
	ldr	r5, [pc, #500]
	cmp	r4, r5
	ble.n	.L_080eb45a
	ldr	r3, [pc, #500]
	ldr	r6, [sp, #100]
	ldr	r3, [r6, r3]
	ldr	r3, [r3, #20]
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_080eb45a
	ldr	r0, [pc, #484]
	adds	r5, r6, r0
	movs	r6, #36
.L_080eb43a:
	ldr	r3, [r5, #0]
	movs	r2, #0
	ldrsh	r0, [r3, r6]
	movs	r3, #1
	str	r2, [sp, #0]
	negs	r3, r3
	movs	r1, #14
	movs	r2, #5
	bl	sub_080d6888
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	adds	r7, #1
	adds	r6, #2
	cmp	r7, r3
	bne.n	.L_080eb43a
.L_080eb45a:
	ldr	r3, [sp, #84]
	cmp	r3, #238
	bne.n	.L_080eb472
	ldr	r3, [pc, #444]
	add	r4, sp, #140
	str	r3, [sp, #140]
	adds	r0, r4, #0
	ldr	r3, [pc, #440]
	ldr	r1, [sp, #104]
	ldr	r2, [pc, #440]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_080eb472:
	ldr	r3, [sp, #84]
	subs	r3, #214
	cmp	r3, #65
	bhi.n	.L_080eb482
	ldr	r2, [pc, #432]
	ldrh	r3, [r2, #4]
	adds	r3, #8
	strh	r3, [r2, #4]
.L_080eb482:
	ldr	r3, [sp, #84]
	subs	r3, #246
	cmp	r3, #33
	bhi.n	.L_080eb4a4
	ldr	r5, [sp, #32]
	ldr	r6, [sp, #100]
	movs	r0, #239
	lsls	r0, r0, #7
	adds	r3, r6, r0
	adds	r5, #8
	movs	r1, #0
	str	r5, [sp, #32]
	str	r1, [r3, #0]
	ldr	r3, [pc, #400]
	adds	r2, r6, r3
	movs	r3, #75
	str	r3, [r2, #0]
.L_080eb4a4:
	ldr	r4, [sp, #84]
	cmp	r4, #213
	bgt.n	.L_080eb4ac
	b.n	.L_080eb6d2
.L_080eb4ac:
	add	r3, sp, #124
	movs	r5, #0
	str	r5, [r3, #12]
	str	r5, [r3, #4]
	ldr	r0, [sp, #100]
	movs	r6, #108
	ldr	r1, [pc, #376]
	movs	r2, #225
	add	r6, sp
	lsls	r2, r2, #7
	mov	r8, r6
	movs	r7, #0
	adds	r4, r3, #0
	adds	r6, r0, r1
	adds	r5, r0, r2
.L_080eb4ca:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	beq.n	.L_080eb56a
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	asrs	r2, r2, #7
	ldr	r0, [pc, #352]
	adds	r3, r3, r2
	str	r3, [sp, #108]
	cmp	r3, r0
	bgt.n	.L_080eb4e6
	movs	r3, #128
	lsls	r3, r3, #4
	str	r3, [sp, #108]
.L_080eb4e6:
	mov	r1, r8
	str	r3, [r1, #4]
	movs	r2, #255
	ldr	r3, [r5, #0]
	lsls	r2, r2, #16
	str	r2, [r4, #4]
	str	r3, [r4, #0]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r4, #8]
	adds	r1, r4, #0
	mov	r2, r8
	movs	r3, #0
	ldr	r0, [r6, #0]
	str	r4, [sp, #8]
	bl	sub_08009008
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #16]
	subs	r2, r2, r3
	ldr	r3, [pc, #300]
	str	r2, [r5, #4]
	ldr	r4, [sp, #8]
	cmp	r2, r3
	bgt.n	.L_080eb56a
	ldr	r0, [sp, #84]
	ldr	r1, [pc, #248]
	cmp	r0, r1
	bgt.n	.L_080eb530
	bl	sub_08004458
	movs	r3, #63
	ldr	r2, [sp, #32]
	ands	r3, r0
	adds	r3, r3, r2
	adds	r3, #32
	b.n	.L_080eb540
.L_080eb530:
	str	r4, [sp, #8]
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	ldr	r0, [sp, #32]
	adds	r3, r3, r0
	subs	r3, #32
.L_080eb540:
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r4, [sp, #8]
	ldr	r1, [sp, #84]
	ldr	r2, [pc, #244]
	cmp	r1, r2
	ble.n	.L_080eb554
	movs	r3, #0
	str	r3, [r5, #24]
	b.n	.L_080eb56a
.L_080eb554:
	ldr	r0, [sp, #84]
	cmp	r0, #245
	ble.n	.L_080eb560
	movs	r3, #192
	lsls	r3, r3, #15
	b.n	.L_080eb568
.L_080eb560:
	ldr	r2, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #16
	subs	r3, r3, r2
.L_080eb568:
	str	r3, [r5, #4]
.L_080eb56a:
	adds	r7, #1
	adds	r6, #4
	adds	r5, #28
	cmp	r7, #16
	bne.n	.L_080eb4ca
	ldr	r1, [sp, #84]
	ldr	r2, [pc, #156]
	cmp	r1, r2
	bgt.n	.L_080eb644
	ldr	r4, [sp, #32]
	movs	r3, #15
	mov	sl, r3
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	asrs	r1, r1, #31
	asrs	r3, r3, #1
	movs	r7, #0
	mov	r8, r1
	mov	r9, r3
.L_080eb590:
	bl	sub_08004458
	movs	r2, #1
	ands	r2, r7
	mov	r5, sl
	ands	r0, r5
	lsls	r3, r2, #2
	lsrs	r5, r7, #31
	adds	r3, r3, r2
	adds	r5, r7, r5
	asrs	r5, r5, #1
	lsls	r3, r3, #2
	lsls	r2, r5, #2
	subs	r3, r3, r0
	adds	r3, r3, r2
	add	r3, r9
	adds	r6, r3, #0
	bl	sub_08004458
	mov	r2, r8
	ldr	r4, [sp, #84]
	lsrs	r3, r2, #31
	adds	r3, r4, r3
	asrs	r3, r3, #1
	mov	r1, sl
	lsls	r5, r5, #5
	ands	r0, r1
	adds	r2, r3, r7
	subs	r6, #16
	subs	r5, r5, r0
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080eb5d4
	adds	r3, r2, #3
.L_080eb5d4:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	lsls	r1, r3, #3
	subs	r1, r1, r3
	lsls	r1, r1, #2
	ldr	r0, [sp, #100]
	subs	r1, r1, r3
	lsls	r1, r1, #6
	movs	r3, #32
	movs	r2, #128
	adds	r1, r0, r1
	lsls	r2, r2, #7
	str	r3, [sp, #0]
	movs	r3, #54
	adds	r1, r1, r2
	str	r3, [sp, #4]
	ldr	r0, [sp, #104]
	adds	r2, r6, #0
	adds	r3, r5, #0
	ldr	r4, [sp, #92]
	adds	r7, #1
	bl	sub_080072f4
	cmp	r7, #8
	bne.n	.L_080eb590
	b.n	.L_080eb6d2
	movs	r0, r0
	.4byte 0xffffc000
	.4byte 0x03001f0c
	.4byte 0x00000117
	.4byte 0x0000013f
	.4byte 0x00007828
	.4byte 0x3f3f3f3f
	.4byte 0x040000d4
	.4byte 0x85001000
	.4byte 0x03001ad0
	.4byte 0x00007784
	.4byte 0x000077d8
	.4byte 0x000007ff
	.4byte 0x000fffff
	.2byte 0x012d
	.2byte 0x0000
.L_080eb644:
	ldr	r0, [sp, #32]
	ldr	r5, [sp, #84]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r5, r5, #31
	movs	r6, #15
	asrs	r3, r3, #1
	movs	r7, #0
	mov	r8, r5
	mov	sl, r6
	mov	r9, r3
.L_080eb65a:
	bl	sub_08004458
	movs	r2, #1
	ands	r2, r7
	lsls	r3, r2, #2
	lsrs	r5, r7, #31
	adds	r3, r3, r2
	mov	r1, sl
	adds	r5, r7, r5
	ands	r0, r1
	asrs	r5, r5, #1
	lsls	r3, r3, #2
	lsls	r2, r5, #2
	adds	r3, r3, r0
	subs	r3, r3, r2
	add	r3, r9
	adds	r6, r3, #0
	bl	sub_08004458
	mov	r2, sl
	ands	r0, r2
	lsls	r5, r5, #5
	subs	r5, r5, r0
	mov	r4, r8
	ldr	r0, [sp, #84]
	lsrs	r3, r4, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	adds	r2, r3, r7
	subs	r6, #16
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080eb69e
	adds	r3, r2, #3
.L_080eb69e:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r2, r3
	lsls	r1, r3, #3
	subs	r1, r1, r3
	lsls	r1, r1, #2
	ldr	r2, [sp, #100]
	subs	r1, r1, r3
	lsls	r1, r1, #6
	movs	r3, #128
	adds	r1, r2, r1
	lsls	r3, r3, #7
	adds	r1, r1, r3
	movs	r3, #32
	str	r3, [sp, #0]
	movs	r3, #54
	str	r3, [sp, #4]
	ldr	r0, [sp, #104]
	adds	r2, r6, #0
	adds	r3, r5, #0
	ldr	r4, [sp, #92]
	adds	r7, #1
	bl	sub_080072f4
	cmp	r7, #8
	bne.n	.L_080eb65a
.L_080eb6d2:
	movs	r0, #47
	bl	sub_08002dd8
	ldr	r5, [sp, #84]
	cmp	r5, #63
	ble.n	.L_080eb6ec
	bl	sub_08004458
	movs	r3, #3
	ands	r3, r0
	ldr	r2, [pc, #92]
	adds	r3, #30
	strh	r3, [r2, #6]
.L_080eb6ec:
	ldr	r6, [sp, #100]
	ldr	r0, [pc, #88]
	movs	r3, #1
	adds	r2, r6, r0
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [sp, #84]
	movs	r2, #160
	adds	r1, #1
	lsls	r2, r2, #1
	str	r1, [sp, #84]
	cmp	r1, r2
	beq.n	.L_080eb70e
	bl	.L_080eaa8e
.L_080eb70e:
	ldr	r3, [sp, #100]
	ldr	r4, [pc, #56]
	movs	r7, #0
	adds	r6, r3, r4
.L_080eb716:
	ldmia	r6!, {r0}
	adds	r7, #1
	bl	sub_08009038
	cmp	r7, #16
	bne.n	.L_080eb716
	ldr	r0, [pc, #44]
	bl	sub_08004278
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #184
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001ad0
	.4byte 0x00007824
	.4byte 0x000077d8
	.2byte 0xd261
	.2byte 0x080c
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #96]
	ldr	r1, [r5, #0]
	sub	sp, #176
	str	r1, [sp, #80]
	subs	r3, r5, #4
	ldr	r3, [r3, #0]
	str	r3, [sp, #76]
	ldr	r4, [pc, #84]
	ldr	r2, [r5, #4]
	adds	r6, r3, r4
	str	r2, [sp, #68]
	str	r0, [r6, #0]
	movs	r0, #0
	bl	sub_080cd594
	bl	sub_080c9048
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #52]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #52]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r7, [sp, #76]
	movs	r0, #239
	lsls	r0, r0, #7
	movs	r3, #0
	adds	r7, r7, r0
	movs	r1, #144
	str	r3, [r7, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #40]
	bl	sub_080041d8
	movs	r0, #1
	movs	r1, #0
	bl	sub_080cd104
	ldr	r1, [pc, #28]
	movs	r0, #9
	movs	r2, #1
	b.n	.L_080eb7d8
	.4byte 0x00000784
	.4byte 0x00000000
	.4byte 0x03001ef0
	.4byte 0x00007828
	.4byte 0x0400000c
	.4byte 0x080cd261
	.2byte 0x0175
	.2byte 0x0000
.L_080eb7d8:
	bl	sub_080dbb24
	ldr	r2, [pc, #76]
	movs	r3, #240
	str	r3, [r2, #16]
	ldr	r0, [r6, #0]
	bl	sub_080d6750
	ldr	r2, [pc, #68]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	subs	r2, #8
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r2, #0
	ldr	r1, [pc, #52]
	movs	r0, #1
	bl	sub_080b5040
	movs	r0, #1
	movs	r1, #1
	bl	sub_080cd104
	ldr	r0, [pc, #40]
	ldr	r1, [sp, #68]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [pc, #32]
	ldr	r1, [sp, #76]
	movs	r2, #1
	movs	r3, #1
	b.n	.L_080eb840
	movs	r0, r0
	.4byte 0x00002737
	.4byte 0x000000ca
	.4byte 0x03001ce0
	.4byte 0x04000048
	.4byte 0x0000003a
	.4byte 0x00000073
	.2byte 0x0095
	.2byte 0x0000
.L_080eb840:
	bl	sub_080e0524
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	movs	r1, #0
	subs	r2, #2
	strh	r3, [r2, #0]
	str	r1, [sp, #64]
	str	r1, [sp, #60]
	ldr	r3, [pc, #32]
	ldrh	r3, [r3, #4]
	str	r3, [sp, #56]
	ldr	r5, [r5, #16]
	movs	r2, #1
	str	r5, [sp, #52]
	str	r1, [sp, #48]
	str	r2, [r7, #0]
	b.n	.L_080eb88c
	movs	r0, r0
	.4byte 0x00007741
	.4byte 0x00000080
	.4byte 0x0000100e
	.4byte 0x00003f44
	.2byte 0x1ad0
	.2byte 0x0300
.L_080eb88c:
	ldr	r4, [sp, #76]
	ldr	r7, [pc, #120]
	ldr	r0, [sp, #64]
	adds	r3, r4, r7
	str	r0, [r3, #0]
	ldr	r1, [sp, #52]
	movs	r3, #225
	str	r2, [r1, #16]
	lsls	r3, r3, #7
	movs	r2, #0
	mov	r8, r2
	movs	r6, #31
	adds	r5, r4, r3
.L_080eb8a6:
	bl	sub_08004458
	ands	r0, r6
	adds	r0, #16
	str	r0, [r5, #0]
	bl	sub_08004458
	ands	r0, r6
	adds	r0, #48
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #16
	lsls	r0, r0, #16
	str	r0, [r5, #16]
	bl	sub_08004458
	movs	r1, #48
	bl	sub_08002304
	movs	r4, #1
	add	r8, r4
	adds	r0, #2
	mov	r7, r8
	str	r0, [r5, #24]
	adds	r5, #28
	cmp	r7, #64
	bne.n	.L_080eb8a6
	movs	r3, #3
	movs	r2, #7
	movs	r0, #46
	movs	r1, #7
	str	r3, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [pc, #24]
	adds	r3, #184
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #24]
	str	r3, [sp, #72]
	movs	r0, #0
	ldr	r3, [pc, #4]
	mov	fp, r0
	strh	r3, [r2, #0]
	b.n	.L_080ebc56
	.4byte 0x00000786
	.4byte 0x00007784
	.4byte 0x03001e50
	.2byte 0x000c
	.2byte 0x0400
.L_080eb914:
	mov	r3, fp
	subs	r3, #24
	cmp	r3, #31
	bhi.n	.L_080eb922
	ldr	r1, [sp, #48]
	adds	r1, #1
	str	r1, [sp, #48]
.L_080eb922:
	ldr	r2, [sp, #48]
	cmp	r2, #24
	ble.n	.L_080eb92c
	movs	r3, #24
	str	r3, [sp, #48]
.L_080eb92c:
	mov	r4, fp
	cmp	r4, #135
	bgt.n	.L_080eb944
	ldr	r7, [pc, #472]
	ldr	r0, [sp, #48]
	ldrh	r3, [r7, #4]
	adds	r1, r7, #0
	subs	r3, r3, r0
	strh	r3, [r1, #4]
	ldr	r2, [sp, #60]
	adds	r2, r2, r0
	str	r2, [sp, #60]
.L_080eb944:
	mov	r3, fp
	cmp	r3, #149
	bgt.n	.L_080eba18
	ldr	r3, [pc, #452]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	mov	r7, fp
	str	r3, [sp, #92]
	str	r4, [sp, #96]
	movs	r4, #0
	mov	sl, r4
	cmp	r7, #103
	ble.n	.L_080eb966
	ldr	r0, [pc, #436]
	lsls	r3, r7, #4
	adds	r0, r0, r3
	mov	sl, r0
.L_080eb966:
	mov	r3, fp
	subs	r3, #8
	cmp	r3, #23
	bhi.n	.L_080eb978
	ldr	r1, [sp, #64]
	ldr	r2, [sp, #48]
	adds	r3, r1, r2
	subs	r3, #8
	str	r3, [sp, #64]
.L_080eb978:
	mov	r3, fp
	cmp	r3, #7
	ble.n	.L_080eb9be
	movs	r5, #96
	cmp	r3, #104
	bgt.n	.L_080eb986
	movs	r5, #32
.L_080eb986:
	mov	r4, fp
	ldr	r7, [pc, #396]
	lsls	r3, r4, #10
	adds	r0, r3, r7
	ldr	r3, [pc, #396]
	movs	r1, #128
	ands	r0, r3
	lsls	r1, r1, #8
	cmp	r0, r1
	ble.n	.L_080eb99e
	ldr	r2, [pc, #388]
	adds	r0, r0, r2
.L_080eb99e:
	bl	sub_08002322
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #16
	str	r3, [sp, #44]
	mov	r4, fp
	movs	r3, #31
	ands	r3, r4
	cmp	r3, #8
	bne.n	.L_080eb9be
	ldr	r7, [sp, #76]
	ldr	r0, [pc, #364]
	movs	r3, #4
	adds	r2, r7, r0
	str	r3, [r2, #0]
.L_080eb9be:
	add	r3, sp, #160
	movs	r2, #0
	str	r2, [r3, #12]
	movs	r2, #255
	lsls	r2, r2, #16
	str	r2, [r3, #4]
	adds	r6, r3, #0
	ldr	r2, [sp, #76]
	ldr	r3, [pc, #344]
	movs	r1, #0
	mov	r8, r1
	add	r7, sp, #92
	adds	r5, r2, r3
.L_080eb9d8:
	ldr	r3, [pc, #336]
	mov	r4, r8
	ldr	r0, [sp, #64]
	ldrb	r3, [r3, r4]
	mov	r1, sl
	adds	r3, r0, r3
	subs	r3, r3, r1
	movs	r2, #224
	lsls	r2, r2, #16
	lsls	r3, r3, #16
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r3, [pc, #316]
	ldrb	r3, [r3, r4]
	ldr	r4, [sp, #44]
	movs	r0, #144
	subs	r3, r3, r4
	lsls	r0, r0, #15
	lsls	r3, r3, #16
	adds	r3, r3, r0
	str	r3, [r6, #8]
	adds	r1, r6, #0
	adds	r2, r7, #0
	ldmia	r5!, {r0}
	movs	r3, #0
	bl	sub_08009008
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #9
	bne.n	.L_080eb9d8
.L_080eba18:
	mov	r3, fp
	cmp	r3, #26
	bgt.n	.L_080ebab0
	ldr	r7, [sp, #60]
	lsls	r3, r3, #3
	adds	r7, #4
	mov	sl, r3
	cmp	r7, #10
	ble.n	.L_080eba2c
	movs	r7, #10
.L_080eba2c:
	mov	r4, sl
	cmp	r4, #64
	ble.n	.L_080eba36
	movs	r0, #64
	mov	sl, r0
.L_080eba36:
	movs	r1, #0
	mov	r2, sl
	mov	r8, r1
	cmp	r2, #0
	beq.n	.L_080ebab0
	ldr	r3, [sp, #60]
	ldr	r4, [sp, #60]
	lsls	r3, r3, #1
	str	r3, [sp, #40]
	adds	r3, r3, r4
	lsls	r3, r3, #2
	adds	r3, #48
	str	r3, [sp, #36]
	lsrs	r3, r7, #31
	adds	r3, r7, r3
	asrs	r3, r3, #1
	lsls	r0, r7, #1
	str	r3, [sp, #32]
	mov	r9, r0
.L_080eba5c:
	mov	r1, r8
	lsls	r6, r1, #10
	adds	r0, r6, #0
	bl	sub_08002322
	ldr	r3, [sp, #40]
	adds	r3, #8
	adds	r5, r3, #0
	muls	r5, r0
	ldr	r2, [sp, #60]
	asrs	r5, r5, #16
	adds	r0, r6, #0
	adds	r5, r5, r2
	bl	sub_0800231c
	ldr	r4, [sp, #36]
	adds	r3, r4, #0
	muls	r3, r0
	mov	r2, r9
	ldr	r0, [pc, #176]
	ldr	r4, [sp, #32]
	subs	r2, #2
	ldrh	r1, [r0, r2]
	adds	r5, #96
	ldr	r2, [sp, #68]
	asrs	r3, r3, #16
	subs	r5, r5, r4
	mov	r0, r9
	adds	r3, #64
	adds	r1, r2, r1
	str	r0, [sp, #4]
	subs	r3, r3, r7
	str	r7, [sp, #0]
	ldr	r0, [sp, #80]
	adds	r2, r5, #0
	ldr	r4, [sp, #72]
	bl	sub_080072f4
	movs	r0, #1
	add	r8, r0
	cmp	r8, sl
	bne.n	.L_080eba5c
.L_080ebab0:
	mov	r1, fp
	cmp	r1, #24
	bne.n	.L_080ebacc
	ldr	r3, [sp, #76]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r7, [sp, #76]
	ldr	r0, [pc, #112]
	movs	r3, #50
	adds	r2, r7, r0
	str	r3, [r2, #0]
.L_080ebacc:
	mov	r1, fp
	cmp	r1, #28
	bne.n	.L_080ebad8
	ldr	r2, [pc, #104]
	ldr	r3, [pc, #48]
	strh	r3, [r2, #0]
.L_080ebad8:
	mov	r2, fp
	cmp	r2, #17
	ble.n	.L_080ebbbe
	ldr	r4, [sp, #76]
	movs	r7, #225
	movs	r3, #0
	lsls	r7, r7, #7
	mov	r8, r3
	movs	r6, #31
	adds	r5, r4, r7
.L_080ebaec:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L_080ebb78
	movs	r1, #3
	mov	r0, r8
	bl	sub_080022fc
	adds	r0, #1
	lsls	r4, r0, #1
	ldr	r2, [pc, #52]
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	b.n	.L_080ebb40
	movs	r0, r0
	.4byte 0x00000784
	.4byte 0x03001ad0
	.4byte 0x080edad8
	.4byte 0xfffff980
	.4byte 0xffffe000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x000077a8
	.4byte 0x000077d8
	.4byte 0x080eef56
	.4byte 0x080eef5f
	.4byte 0x080ede48
	.4byte 0x00007784
	.2byte 0x000c
	.2byte 0x0400
.L_080ebb40:
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	ldr	r2, [sp, #68]
	subs	r3, r3, r0
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #80]
	ldr	r4, [sp, #72]
	bl	sub_080072f4
	ldr	r3, [r5, #0]
	adds	r3, #2
	ldr	r2, [r5, #16]
	str	r3, [r5, #0]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #4
	cmp	r3, #0
	bge.n	.L_080ebb72
	adds	r3, #63
.L_080ebb72:
	asrs	r3, r3, #6
	str	r3, [r5, #16]
	b.n	.L_080ebb7c
.L_080ebb78:
	subs	r3, #1
	str	r3, [r5, #24]
.L_080ebb7c:
	ldr	r3, [r5, #0]
	cmp	r3, #128
	bgt.n	.L_080ebb88
	ldr	r3, [r5, #24]
	cmp	r3, #1
	bne.n	.L_080ebbb2
.L_080ebb88:
	bl	sub_08004458
	ldr	r7, [sp, #64]
	ands	r0, r6
	adds	r0, r0, r7
	adds	r0, #172
	str	r0, [r5, #0]
	bl	sub_08004458
	ldr	r1, [sp, #44]
	ands	r0, r6
	subs	r0, r0, r1
	adds	r0, #56
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #16
	lsls	r0, r0, #15
	str	r0, [r5, #16]
.L_080ebbb2:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r5, #28
	cmp	r3, #48
	bne.n	.L_080ebaec
.L_080ebbbe:
	mov	r4, fp
	cmp	r4, #31
	ble.n	.L_080ebc0e
	mov	r3, fp
	subs	r3, #32
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r5, r3, #1
	cmp	r5, #40
	ble.n	.L_080ebbd4
	movs	r5, #40
.L_080ebbd4:
	movs	r7, #0
	mov	r8, r7
	movs	r6, #0
	movs	r7, #120
.L_080ebbdc:
	bl	sub_08004458
	movs	r3, #3
	ands	r0, r3
	lsls	r1, r0, #1
	adds	r1, r1, r0
	movs	r3, #48
	ldr	r0, [sp, #76]
	str	r3, [sp, #0]
	lsls	r1, r1, #9
	movs	r3, #32
	adds	r1, r0, r1
	str	r3, [sp, #4]
	ldr	r0, [sp, #80]
	adds	r3, r6, #0
	subs	r2, r7, r5
	ldr	r4, [sp, #72]
	bl	sub_080072f4
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r6, #18
	cmp	r1, #6
	bne.n	.L_080ebbdc
.L_080ebc0e:
	ldr	r3, [sp, #76]
	ldr	r4, [pc, #32]
	adds	r2, r3, r4
	ldr	r3, [r2, #0]
	cmp	r3, #0
	ble.n	.L_080ebc3c
	subs	r3, #1
	str	r3, [r2, #0]
	bl	sub_08004458
	ldr	r3, [pc, #12]
	ldr	r7, [pc, #16]
	ands	r0, r3
	adds	r0, #28
	strh	r0, [r7, #6]
	b.n	.L_080ebc42
	movs	r0, r0
	.4byte 0x00000007
	.4byte 0x000077a8
	.2byte 0x1ad0
	.2byte 0x0300
.L_080ebc3c:
	ldr	r0, [pc, #192]
	movs	r3, #32
	strh	r3, [r0, #6]
.L_080ebc42:
	ldr	r1, [sp, #76]
	ldr	r3, [pc, #188]
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r4, #1
	add	fp, r4
.L_080ebc56:
	mov	r7, fp
	cmp	r7, #120
	beq.n	.L_080ebcb0
	movs	r0, #0
	str	r0, [sp, #44]
	cmp	r7, #0
	bne.n	.L_080ebc6a
	movs	r0, #136
	bl	sub_080f9010
.L_080ebc6a:
	mov	r1, fp
	cmp	r1, #26
	bne.n	.L_080ebc76
	movs	r0, #141
	bl	sub_080f9010
.L_080ebc76:
	mov	r2, fp
	cmp	r2, #40
	bne.n	.L_080ebc82
	movs	r0, #154
	bl	sub_080f9010
.L_080ebc82:
	mov	r3, fp
	cmp	r3, #72
	bne.n	.L_080ebc8e
	movs	r0, #154
	bl	sub_080f9010
.L_080ebc8e:
	mov	r4, fp
	cmp	r4, #104
	bne.n	.L_080ebc9a
	movs	r0, #154
	bl	sub_080f9010
.L_080ebc9a:
	ldr	r3, [pc, #108]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080ebca8
	b.n	.L_080eb914
.L_080ebca8:
	mov	r7, fp
	cmp	r7, #16
	bgt.n	.L_080ebcb0
	b.n	.L_080eb914
.L_080ebcb0:
	add	r0, sp, #56
	ldr	r3, [pc, #76]
	ldrh	r0, [r0, #0]
	strh	r0, [r3, #4]
	ldr	r1, [sp, #52]
	movs	r2, #0
	str	r2, [r1, #16]
	bl	sub_080d67dc
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #52]
	strh	r3, [r2, #0]
	ldr	r3, [sp, #76]
	ldr	r4, [pc, #68]
	movs	r2, #0
	mov	r8, r2
	movs	r0, #12
	adds	r1, r3, r4
.L_080ebcd4:
	ldmia	r1!, {r2}
	ldrb	r3, [r2, #9]
	movs	r7, #1
	orrs	r3, r0
	add	r8, r7
	strb	r3, [r2, #9]
	mov	r2, r8
	cmp	r2, #9
	bne.n	.L_080ebcd4
	mov	r4, sp
	adds	r4, #116
	movs	r3, #224
	mov	r2, sp
	str	r4, [sp, #24]
	str	r3, [sp, #28]
	movs	r1, #0
	adds	r3, r4, #0
	adds	r2, #130
	b.n	.L_080ebd14
	movs	r0, r0
	.4byte 0x000000f0
	.4byte 0x03001ad0
	.4byte 0x00007824
	.4byte 0x03001b04
	.4byte 0x04000040
	.2byte 0x77d8
	.2byte 0x0000
.L_080ebd14:
	strb	r1, [r3, #0]
	adds	r3, #1
	cmp	r3, r2
	bne.n	.L_080ebd14
	mov	r7, sp
	adds	r7, #132
	str	r7, [sp, #20]
	ldr	r5, [sp, #20]
	movs	r7, #31
	add	r6, sp, #148
.L_080ebd28:
	bl	sub_08004458
	ands	r0, r7
	strb	r0, [r5, #0]
	adds	r5, #1
	cmp	r5, r6
	bne.n	.L_080ebd28
	movs	r0, #0
	movs	r2, #160
	ldr	r3, [pc, #96]
	mov	r8, r0
	movs	r1, #0
	lsls	r2, r2, #1
.L_080ebd42:
	movs	r4, #1
	add	r8, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_080ebd42
	ldr	r7, [sp, #76]
	movs	r0, #239
	lsls	r0, r0, #7
	ldr	r1, [pc, #72]
	adds	r2, r7, r0
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r7, r1
	movs	r3, #75
	str	r3, [r2, #0]
	ldr	r2, [pc, #64]
	ldr	r3, [pc, #44]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #70
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	movs	r2, #0
	str	r3, [sp, #16]
	mov	fp, r2
.L_080ebd76:
	mov	r4, fp
	cmp	r4, #23
	bgt.n	.L_080ebe3c
	ldr	r3, [pc, #44]
	ldr	r7, [sp, #28]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	subs	r7, #16
	mov	r0, fp
	str	r3, [sp, #84]
	str	r4, [sp, #88]
	str	r7, [sp, #28]
	cmp	r0, #8
	bgt.n	.L_080ebdcc
	b.n	.L_080ebdb0
	.4byte 0x00000784
	.4byte 0x00001010
	.4byte 0x02010018
	.4byte 0x00007784
	.4byte 0x0400000c
	.4byte 0xfffffe20
	.2byte 0xdae0
	.2byte 0x080e
.L_080ebdb0:
	movs	r1, #128
	lsls	r3, r0, #11
	lsls	r1, r1, #7
	movs	r2, #128
	adds	r0, r3, r1
	lsls	r2, r2, #8
	cmp	r0, r2
	ble.n	.L_080ebdc4
	ldr	r4, [pc, #752]
	adds	r0, r3, r4
.L_080ebdc4:
	bl	sub_08002322
	lsls	r0, r0, #6
	b.n	.L_080ebde8
.L_080ebdcc:
	mov	r7, fp
	movs	r1, #128
	lsls	r3, r7, #11
	lsls	r1, r1, #7
	movs	r2, #128
	adds	r0, r3, r1
	lsls	r2, r2, #8
	cmp	r0, r2
	ble.n	.L_080ebde2
	ldr	r4, [pc, #724]
	adds	r0, r3, r4
.L_080ebde2:
	bl	sub_08002322
	lsls	r0, r0, #5
.L_080ebde8:
	asrs	r4, r0, #16
	add	r3, sp, #100
	movs	r2, #0
	str	r2, [r3, #12]
	movs	r2, #255
	lsls	r2, r2, #16
	str	r2, [r3, #4]
	ldr	r0, [sp, #76]
	ldr	r1, [pc, #700]
	movs	r7, #0
	mov	r8, r7
	adds	r6, r3, #0
	add	r7, sp, #84
	adds	r5, r0, r1
.L_080ebe04:
	ldr	r3, [pc, #692]
	mov	r2, r8
	ldr	r0, [sp, #28]
	ldrb	r3, [r3, r2]
	adds	r3, r0, r3
	lsls	r3, r3, #16
	str	r3, [r6, #0]
	ldr	r3, [pc, #684]
	ldrb	r3, [r3, r2]
	movs	r1, #144
	subs	r3, r3, r4
	lsls	r1, r1, #15
	lsls	r3, r3, #16
	adds	r3, r3, r1
	str	r3, [r6, #8]
	adds	r2, r7, #0
	movs	r3, #0
	ldmia	r5!, {r0}
	adds	r1, r6, #0
	str	r4, [sp, #8]
	bl	sub_08009008
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	ldr	r4, [sp, #8]
	cmp	r3, #9
	bne.n	.L_080ebe04
.L_080ebe3c:
	mov	r4, fp
	cmp	r4, #8
	bne.n	.L_080ebe50
	ldr	r7, [sp, #76]
	ldr	r0, [pc, #636]
	adds	r3, r7, r0
	str	r4, [r3, #0]
	movs	r0, #145
	bl	sub_080f9010
.L_080ebe50:
	mov	r1, fp
	cmp	r1, #11
	bne.n	.L_080ebe5c
	movs	r0, #145
	bl	sub_080f9010
.L_080ebe5c:
	mov	r2, fp
	cmp	r2, #46
	bne.n	.L_080ebe68
	movs	r0, #137
	bl	sub_080f9010
.L_080ebe68:
	ldr	r2, [pc, #604]
	ldr	r4, [sp, #76]
	movs	r3, #0
	mov	r8, r3
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_080ebf3a
	movs	r0, #0
	movs	r7, #36
	str	r0, [sp, #12]
	mov	r9, r7
.L_080ebe80:
	ldr	r1, [sp, #24]
	mov	r4, r8
	ldrb	r3, [r1, r4]
	cmp	r3, #0
	bne.n	.L_080ebf1c
	ldr	r7, [sp, #76]
	ldr	r3, [r7, r2]
	add	r5, sp, #148
	mov	r1, r9
	ldrsh	r0, [r3, r1]
	adds	r1, r5, #0
	bl	sub_080e3980
	ldr	r3, [r5, #0]
	ldr	r4, [sp, #28]
	cmp	r3, r4
	ble.n	.L_080ebf1c
	ldr	r7, [sp, #24]
	movs	r3, #1
	mov	r0, r8
	strb	r3, [r7, r0]
	ldr	r1, [sp, #12]
	ldr	r2, [pc, #540]
	mov	sl, r5
	movs	r6, #0
	movs	r7, #255
	adds	r5, r1, r2
.L_080ebeb6:
	mov	r4, sl
	ldr	r3, [r4, #0]
	lsls	r3, r3, #15
	str	r3, [r5, #0]
	ldr	r3, [r4, #4]
	subs	r3, #16
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #128
	lsls	r0, r0, #10
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #192
	lsls	r3, r0, #11
	ldr	r2, [r5, #12]
	str	r3, [r5, #16]
	ldr	r3, [r5, #0]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r3, [r5, #4]
	lsls	r0, r0, #13
	adds	r3, r3, r0
	str	r3, [r5, #4]
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r3, #8
	adds	r6, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r6, #32
	bne.n	.L_080ebeb6
	ldr	r7, [sp, #76]
	ldr	r0, [pc, #448]
	adds	r3, r7, r0
	ldr	r3, [r3, #0]
	mov	r1, r9
	ldrsh	r0, [r3, r1]
	movs	r1, #1
	bl	sub_080b5088
	movs	r0, #134
	bl	sub_080f9010
.L_080ebf1c:
	ldr	r4, [sp, #12]
	movs	r7, #224
	lsls	r7, r7, #2
	adds	r4, r4, r7
	str	r4, [sp, #12]
	ldr	r2, [pc, #416]
	ldr	r1, [sp, #76]
	movs	r3, #2
	add	r9, r3
	ldr	r3, [r1, r2]
	movs	r0, #1
	ldr	r3, [r3, #20]
	add	r8, r0
	cmp	r8, r3
	bne.n	.L_080ebe80
.L_080ebf3a:
	movs	r2, #0
	ldr	r5, [pc, #396]
	mov	r8, r2
	movs	r7, #3
	movs	r6, #6
.L_080ebf44:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	ble.n	.L_080ebf80
	ldr	r3, [pc, #388]
	ldrh	r1, [r3, #4]
	ldr	r3, [sp, #68]
	movs	r4, #2
	ldrsh	r2, [r5, r4]
	adds	r1, r3, r1
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	subs	r2, #1
	subs	r3, #3
	str	r7, [sp, #0]
	str	r6, [sp, #4]
	ldr	r0, [sp, #80]
	ldr	r4, [sp, #72]
	bl	sub_080072f4
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_080ebf80:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #192
	bne.n	.L_080ebf44
	mov	r2, fp
	cmp	r2, #48
	bne.n	.L_080ebf98
	movs	r0, #136
	bl	sub_080f9010
.L_080ebf98:
	mov	r3, fp
	cmp	r3, #40
	ble.n	.L_080ebff6
	ldr	r4, [sp, #76]
	movs	r7, #239
	lsls	r7, r7, #7
	ldr	r0, [pc, #300]
	adds	r2, r4, r7
	movs	r3, #0
	ldr	r6, [sp, #16]
	str	r3, [r2, #0]
	adds	r2, r4, r0
	movs	r3, #75
	movs	r1, #0
	movs	r5, #8
	str	r3, [r2, #0]
	mov	r8, r1
	negs	r5, r5
.L_080ebfbc:
	bl	sub_08004458
	movs	r3, #3
	ands	r0, r3
	lsls	r1, r0, #1
	ldr	r2, [sp, #76]
	ldr	r3, [sp, #20]
	adds	r1, r1, r0
	mov	r4, r8
	lsls	r1, r1, #9
	adds	r1, r2, r1
	ldrb	r2, [r3, r4]
	movs	r3, #48
	str	r3, [sp, #0]
	subs	r2, r2, r6
	movs	r3, #32
	str	r3, [sp, #4]
	adds	r2, #120
	adds	r3, r5, #0
	ldr	r0, [sp, #80]
	ldr	r7, [sp, #72]
	bl	sub_08007300
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #8
	cmp	r1, #16
	bne.n	.L_080ebfbc
.L_080ebff6:
	mov	r2, fp
	cmp	r2, #64
	ble.n	.L_080ec008
	ldr	r3, [sp, #76]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
.L_080ec008:
	mov	r7, fp
	cmp	r7, #58
	bne.n	.L_080ec044
	ldr	r3, [pc, #184]
	ldr	r1, [sp, #76]
	ldr	r3, [r1, r3]
	ldr	r3, [r3, #20]
	movs	r0, #0
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L_080ec044
	ldr	r2, [pc, #168]
	movs	r6, #36
	adds	r5, r1, r2
.L_080ec024:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #14
	subs	r3, #1
	movs	r2, #5
	bl	sub_080d6888
	ldr	r3, [r5, #0]
	movs	r7, #1
	ldr	r3, [r3, #20]
	add	r8, r7
	adds	r6, #2
	cmp	r8, r3
	bne.n	.L_080ec024
.L_080ec044:
	movs	r0, #8
	movs	r1, #8
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r1, [pc, #132]
	ldr	r0, [sp, #76]
	movs	r3, #1
	adds	r2, r0, r1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r3, #1
	ldr	r2, [sp, #16]
	add	fp, r3
	adds	r2, #12
	mov	r4, fp
	str	r2, [sp, #16]
	cmp	r4, #96
	beq.n	.L_080ec072
	b.n	.L_080ebd76
.L_080ec072:
	movs	r0, #134
	bl	sub_080b50e8
	ldr	r1, [pc, #60]
	ldr	r0, [sp, #76]
	movs	r7, #0
	mov	r8, r7
	adds	r6, r0, r1
.L_080ec082:
	ldmia	r6!, {r0}
	bl	sub_08009038
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #9
	bne.n	.L_080ec082
	ldr	r0, [pc, #72]
	bl	sub_08004278
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #176
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0xffffc000
	.4byte 0x000077d8
	.4byte 0x080eef56
	.4byte 0x080eef5f
	.4byte 0x000077a8
	.4byte 0x00007828
	.4byte 0x02010000
	.4byte 0x080ede48
	.4byte 0x00007784
	.4byte 0x00007824
	.4byte 0x080cd261

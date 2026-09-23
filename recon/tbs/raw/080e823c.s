.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080072f4, 0x080072f4
	.set sub_08009008, 0x08009008
	.set sub_08009020, 0x08009020
	.set sub_08009030, 0x08009030
	.set sub_08009038, 0x08009038
	.set sub_080b5040, 0x080b5040
	.set sub_080b5088, 0x080b5088
	.set sub_080b50e8, 0x080b50e8
	.set sub_080c9048, 0x080c9048
	.set sub_080cd104, 0x080cd104
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6750, 0x080d6750
	.set sub_080d67dc, 0x080d67dc
	.set sub_080d6888, 0x080d6888
	.set sub_080dbb24, 0x080dbb24
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e3908, 0x080e3908
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
	.4byte 0x080cd261

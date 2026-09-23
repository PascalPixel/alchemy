.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_08004c1c, 0x08004c1c
	.set sub_08004c6c, 0x08004c6c
	.set sub_08004cb4, 0x08004cb4
	.set sub_080051d8, 0x080051d8
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_08009008, 0x08009008
	.set sub_08009020, 0x08009020
	.set sub_08009030, 0x08009030
	.set sub_08009038, 0x08009038
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080b5118, 0x080b5118
	.set sub_080c9048, 0x080c9048
	.set sub_080cd104, 0x080cd104
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6750, 0x080d6750
	.set sub_080d67dc, 0x080d67dc
	.set sub_080d6888, 0x080d6888
	.set sub_080dbb24, 0x080dbb24
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e38b8, 0x080e38b8
	.set sub_080e3944, 0x080e3944
	.set sub_080e727c, 0x080e727c
	.set sub_080e7338, 0x080e7338
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global BattleEffect_RunParticleStreams
	.global Func_080e7404
	.thumb_func
BattleEffect_RunParticleStreams:
Func_080e7404:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #284
	str	r1, [sp, #76]
	ldr	r3, [pc, #96]
	mov	r2, sp
	ldr	r1, [r3, #0]
	adds	r2, #148
	str	r2, [sp, #60]
	str	r1, [sp, #72]
	subs	r3, #4
	ldr	r3, [r3, #0]
	ldr	r4, [pc, #84]
	str	r3, [r2, #0]
	adds	r3, r3, r4
	str	r0, [r3, #0]
	movs	r0, #128
	lsls	r0, r0, #6
	bl	sub_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r0, [sp, #76]
	cmp	r0, #1
	bne.n	.L_080e74b8
	ldr	r1, [sp, #60]
	ldr	r2, [pc, #52]
	ldr	r3, [r1, #0]
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5098
	movs	r3, #160
	ldr	r2, [r0, #0]
	lsls	r3, r3, #12
	str	r3, [r2, #40]
	ldr	r3, [pc, #40]
	str	r3, [r2, #72]
	ldr	r4, [sp, #60]
	ldr	r0, [pc, #24]
	ldr	r3, [r4, #0]
	adds	r3, r3, r0
	ldr	r3, [r3, #0]
	movs	r5, #1
	ldr	r0, [r3, #8]
	negs	r5, r5
	movs	r3, #0
	b.n	.L_080e7488
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x03001ef0
	.4byte 0x00007828
	.4byte 0x04000020
	.2byte 0x91eb
	.2byte 0x0000
.L_080e7488:
	str	r3, [sp, #0]
	adds	r1, r5, #0
	movs	r2, #2
	adds	r3, r5, #0
	bl	sub_080d6888
	movs	r0, #145
	bl	sub_080f9010
	ldr	r1, [sp, #60]
	ldr	r2, [pc, #20]
	ldr	r3, [r1, #0]
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	ldr	r4, [sp, #76]
	ldr	r3, [r3, #4]
	str	r4, [sp, #64]
	cmp	r3, #1
	beq.n	.L_080e74be
	str	r5, [sp, #64]
	b.n	.L_080e74be
	movs	r0, r0
	.2byte 0x7828
	.2byte 0x0000
.L_080e74b8:
	movs	r0, #1
	negs	r0, r0
	str	r0, [sp, #64]
.L_080e74be:
	bl	sub_080c9048
	ldr	r2, [pc, #56]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r1, [sp, #60]
	movs	r2, #239
	ldr	r3, [r1, #0]
	ldr	r5, [pc, #40]
	lsls	r2, r2, #7
	adds	r3, r3, r2
	movs	r1, #144
	movs	r2, #0
	str	r2, [r3, #0]
	lsls	r1, r1, #3
	adds	r0, r5, #0
	bl	sub_080041d8
	movs	r0, #0
	movs	r1, #0
	bl	sub_080cd104
	adds	r0, r5, #0
	bl	sub_08004278
	ldr	r3, [sp, #76]
	b.n	.L_080e7504
	movs	r0, r0
	.4byte 0x00000000
	.2byte 0xd261
	.2byte 0x080c
.L_080e7504:
	cmp	r3, #1
	bne.n	.L_080e755c
	movs	r4, #0
	ldr	r5, [pc, #68]
	ldr	r6, [pc, #68]
	mov	r8, r4
.L_080e7510:
	adds	r0, r6, #0
	bl	sub_08009030
	ldr	r1, [sp, #60]
	ldr	r3, [r1, #0]
	str	r0, [r3, r5]
	cmp	r0, #0
	beq.n	.L_080e753c
	adds	r2, r0, #0
	adds	r2, #38
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #2
	bl	sub_08009020
	ldr	r2, [sp, #60]
	ldr	r3, [r2, #0]
	ldr	r1, [r3, r5]
	ldrb	r3, [r1, #9]
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r1, #9]
.L_080e753c:
	movs	r4, #1
	ldr	r3, [pc, #24]
	add	r8, r4
	mov	r0, r8
	adds	r5, #4
	adds	r6, r6, r3
	cmp	r0, #2
	bne.n	.L_080e7510
	b.n	.L_080e7566
	movs	r0, r0
	.4byte 0x000077d8
	.4byte 0x000001e3
	.2byte 0x2001
	.2byte 0x0000
.L_080e755c:
	ldr	r1, [pc, #140]
	movs	r0, #1
	movs	r2, #3
	bl	sub_080dbb24
.L_080e7566:
	ldr	r2, [sp, #60]
	movs	r3, #1
	ldr	r1, [r2, #0]
	ldr	r0, [pc, #128]
	movs	r2, #1
	bl	sub_080e0524
	ldr	r3, [sp, #76]
	cmp	r3, #1
	bne.n	.L_080e758e
	ldr	r0, [pc, #120]
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #112]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	sub_080072f0
.L_080e758e:
	movs	r4, #140
	lsls	r4, r4, #1
	add	r4, sp
	ldr	r3, [pc, #100]
	mov	r9, r4
	str	r3, [r4, #0]
	mov	r0, r9
	ldr	r3, [pc, #96]
	ldr	r1, [pc, #100]
	ldr	r2, [pc, #100]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #240
	ldr	r3, [pc, #76]
	lsls	r2, r2, #7
	ldr	r0, [pc, #92]
	bl	sub_080072f0
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [pc, #84]
	ldr	r3, [pc, #32]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	subs	r2, #48
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	subs	r2, #22
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	adds	r2, #2
	ldr	r1, [pc, #68]
	movs	r0, #0
	strh	r3, [r2, #0]
	mov	r8, r0
	movs	r7, #15
	mov	sl, r1
	b.n	.L_080e7618
	.4byte 0x00000000
	.4byte 0x00000100
	.4byte 0x00001f80
	.4byte 0x00002787
	.4byte 0x0000017d
	.4byte 0x000000c1
	.4byte 0x000000c4
	.4byte 0x03001388
	.4byte 0x01010101
	.4byte 0x040000d4
	.4byte 0x02010000
	.4byte 0x85002000
	.4byte 0x06008000
	.4byte 0x04000050
	.2byte 0x0100
	.2byte 0x0500
.L_080e7618:
	bl	sub_08004458
	adds	r6, r0, #0
	bl	sub_08004458
	adds	r5, r0, #0
	bl	sub_08004458
	ands	r5, r7
	ands	r0, r7
	adds	r5, #16
	adds	r0, #16
	ands	r6, r7
	lsls	r0, r0, #10
	lsls	r5, r5, #5
	adds	r6, #16
	orrs	r0, r5
	movs	r4, #1
	orrs	r0, r6
	mov	r2, sl
	add	r8, r4
	strh	r0, [r2, #0]
	movs	r3, #2
	mov	r0, r8
	add	sl, r3
	cmp	r0, #63
	bne.n	.L_080e7618
	mov	r1, r9
	movs	r3, #0
	str	r3, [r1, #0]
	mov	r0, r9
	ldr	r3, [pc, #436]
	ldr	r1, [sp, #72]
	ldr	r2, [pc, #436]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #0
	movs	r3, #127
	mov	r8, r2
	mov	sl, r3
	movs	r7, #7
.L_080e766a:
	bl	sub_08004458
	mov	r4, sl
	adds	r6, r0, #0
	ands	r6, r4
	bl	sub_08004458
	adds	r5, r0, #0
	mov	r0, sl
	ands	r5, r0
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r1, r3, #0
	adds	r1, #64
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_080e7692
	adds	r3, r5, #7
.L_080e7692:
	asrs	r3, r3, #3
	adds	r2, r6, #0
	cmp	r6, #0
	bge.n	.L_080e769c
	adds	r2, r6, #7
.L_080e769c:
	asrs	r2, r2, #3
	lsls	r3, r3, #4
	adds	r3, r3, r2
	ands	r5, r7
	lsls	r3, r3, #3
	adds	r3, r3, r5
	ands	r6, r7
	lsls	r3, r3, #3
	ldr	r2, [sp, #72]
	adds	r3, r3, r6
	strb	r1, [r2, r3]
	movs	r4, #128
	movs	r3, #1
	add	r8, r3
	lsls	r4, r4, #1
	cmp	r8, r4
	bne.n	.L_080e766a
	movs	r2, #128
	ldr	r1, [sp, #72]
	ldr	r3, [pc, #336]
	lsls	r2, r2, #7
	ldr	r0, [pc, #336]
	bl	sub_080072f0
	ldr	r2, [pc, #332]
	movs	r3, #240
	str	r3, [r2, #16]
	ldr	r0, [sp, #60]
	ldr	r1, [pc, #328]
	ldr	r3, [r0, #0]
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	bl	sub_080d6750
	ldr	r3, [sp, #60]
	ldr	r4, [pc, #320]
	ldr	r2, [r3, #0]
	ldr	r0, [pc, #320]
	adds	r3, r2, r4
	movs	r1, #0
	str	r1, [r3, #0]
	subs	r4, #64
	adds	r3, r2, r0
	str	r1, [r3, #0]
	subs	r0, #64
	adds	r3, r2, r4
	str	r1, [r3, #0]
	adds	r1, r2, r0
	movs	r3, #2
	str	r3, [r1, #0]
	ldr	r4, [sp, #64]
	ldr	r3, [pc, #296]
	adds	r0, #8
	adds	r1, r2, r3
	lsls	r3, r4, #7
	adds	r2, r2, r0
	str	r3, [r1, #0]
	mov	r1, r8
	str	r1, [r2, #0]
	ldr	r0, [pc, #284]
	ldr	r1, [pc, #284]
	bl	sub_080041d8
	movs	r1, #144
	ldr	r0, [pc, #280]
	lsls	r1, r1, #3
	bl	sub_080041d8
	add	r2, sp, #152
	mov	sl, r2
	movs	r7, #63
	mov	r5, sl
	add	r6, sp, #280
.L_080e772e:
	bl	sub_08004458
	ands	r0, r7
	strb	r0, [r5, #0]
	adds	r5, #1
	cmp	r5, r6
	bne.n	.L_080e772e
	movs	r3, #1
	movs	r6, #0
	mov	r8, r3
	movs	r5, #0
.L_080e7744:
	mov	r4, r8
	lsrs	r3, r4, #31
	add	r3, r8
	asrs	r3, r3, #1
	movs	r0, #4
	adds	r6, r6, r3
	add	r8, r0
	cmp	r5, r6
	beq.n	.L_080e77b4
	movs	r1, #127
	movs	r2, #0
	mov	r7, sl
	movs	r4, #7
	mov	lr, r1
	mov	ip, r2
.L_080e7762:
	movs	r0, #0
.L_080e7764:
	mov	r1, lr
	adds	r3, r0, #0
	ands	r3, r1
	ldrb	r3, [r7, r3]
	subs	r1, r5, r3
	cmp	r1, #0
	blt.n	.L_080e77a4
	cmp	r1, #127
	bgt.n	.L_080e77a4
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_080e777e
	adds	r2, r1, #7
.L_080e777e:
	asrs	r2, r2, #3
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L_080e7788
	adds	r3, r0, #7
.L_080e7788:
	asrs	r3, r3, #3
	lsls	r2, r2, #5
	adds	r2, r2, r3
	ands	r1, r4
	lsls	r2, r2, #3
	adds	r2, r2, r1
	adds	r3, r0, #0
	ands	r3, r4
	lsls	r2, r2, #3
	adds	r2, r2, r3
	ldr	r3, [pc, #156]
	mov	r1, ip
	adds	r2, r2, r3
	strb	r1, [r2, #0]
.L_080e77a4:
	movs	r2, #128
	adds	r0, #1
	lsls	r2, r2, #1
	cmp	r0, r2
	bne.n	.L_080e7764
	adds	r5, #1
	cmp	r5, r6
	bne.n	.L_080e7762
.L_080e77b4:
	ldr	r4, [sp, #60]
	ldr	r0, [pc, #136]
	ldr	r3, [r4, #0]
	movs	r2, #1
	adds	r3, r3, r0
	str	r2, [r3, #0]
	movs	r0, #1
	bl	sub_080030f8
	cmp	r6, #191
	ble.n	.L_080e7744
	ldr	r2, [pc, #120]
	ldr	r3, [pc, #52]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r2, [pc, #112]
	ldrh	r1, [r2, #4]
	str	r1, [sp, #56]
	ldrh	r3, [r2, #6]
	ldr	r5, [pc, #108]
	str	r3, [sp, #52]
	ldr	r4, [r5, #0]
	movs	r3, #0
	str	r4, [sp, #48]
	strh	r3, [r2, #4]
	movs	r3, #32
	strh	r3, [r2, #6]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	sub_080ed408
	ldr	r5, [r5, #8]
	ldr	r0, [sp, #60]
	b.n	.L_080e7850
	.4byte 0x00003f42
	.4byte 0x00001010
	.4byte 0x040000d4
	.4byte 0x85001000
	.4byte 0x03001388
	.4byte 0x06004000
	.4byte 0x03001ce0
	.4byte 0x00007828
	.4byte 0x000077d0
	.4byte 0x000077d4
	.4byte 0x00007798
	.4byte 0x080c9139
	.4byte 0x000004ff
	.4byte 0x080cd359
	.4byte 0x02010000
	.4byte 0x00007824
	.4byte 0x04000050
	.4byte 0x03001ad0
	.2byte 0x1f00
	.2byte 0x0300
.L_080e7850:
	str	r5, [sp, #68]
	movs	r3, #239
	ldr	r2, [r0, #0]
	lsls	r3, r3, #7
	adds	r1, r2, r3
	ldr	r4, [pc, #304]
	movs	r3, #3
	str	r3, [r1, #0]
	ldr	r3, [pc, #300]
	adds	r2, r2, r4
	str	r3, [r2, #0]
	ldr	r1, [pc, #300]
	ldr	r0, [pc, #300]
	bl	sub_080041d8
	ldr	r1, [sp, #60]
	ldr	r4, [pc, #296]
	ldr	r3, [r1, #0]
	movs	r0, #0
	movs	r2, #1
	mov	r8, r0
	negs	r2, r2
	adds	r3, r3, r4
.L_080e787e:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r1, #64
	bne.n	.L_080e787e
	ldr	r2, [sp, #48]
	movs	r3, #1
	str	r3, [r2, #16]
	ldr	r4, [sp, #60]
	ldr	r0, [pc, #264]
	ldr	r3, [r4, #0]
	movs	r2, #0
	adds	r3, r3, r0
	str	r2, [r3, #0]
	mov	fp, r2
	mov	r1, sp
	mov	r2, sp
	adds	r1, #132
	adds	r2, #88
	movs	r3, #0
	str	r1, [sp, #24]
	str	r2, [sp, #44]
	str	r4, [sp, #40]
	str	r3, [sp, #16]
.L_080e78b2:
	ldr	r4, [sp, #60]
	ldr	r1, [pc, #232]
	ldr	r0, [r4, #0]
	adds	r3, r0, r1
	ldr	r2, [r3, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	bge.n	.L_080e78c4
	adds	r3, #3
.L_080e78c4:
	asrs	r4, r3, #2
	movs	r2, #252
	ldr	r3, [sp, #76]
	lsls	r2, r2, #5
	adds	r5, r0, r2
	cmp	r3, #1
	bne.n	.L_080e78e6
	ldr	r3, [pc, #208]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080e78fa
	mov	r0, fp
	cmp	r0, #16
	ble.n	.L_080e78fa
	b.n	.L_080e7cba
.L_080e78e6:
	ldr	r3, [pc, #188]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080e78fa
	mov	r1, fp
	cmp	r1, #4
	ble.n	.L_080e78fa
	b.n	.L_080e7cba
.L_080e78fa:
	mov	r2, fp
	cmp	r2, #0
	bne.n	.L_080e790a
	movs	r0, #141
	str	r4, [sp, #8]
	bl	sub_080f9010
	ldr	r4, [sp, #8]
.L_080e790a:
	movs	r3, #0
	mov	r8, r3
.L_080e790e:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r1, #15
	bne.n	.L_080e790e
.L_080e791c:
	mov	r1, r8
	subs	r1, #16
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_080e792a
	mov	r3, r8
	subs	r3, #13
.L_080e792a:
	asrs	r3, r3, #2
	adds	r2, r3, r4
	adds	r3, r2, #0
	adds	r1, r2, #0
	subs	r3, #32
	subs	r1, #80
	cmp	r3, #0
	bge.n	.L_080e793c
	movs	r3, #0
.L_080e793c:
	cmp	r3, #31
	ble.n	.L_080e7942
	movs	r3, #31
.L_080e7942:
	cmp	r1, #0
	bge.n	.L_080e7948
	movs	r1, #0
.L_080e7948:
	cmp	r1, #31
	ble.n	.L_080e794e
	movs	r1, #31
.L_080e794e:
	lsls	r2, r1, #5
	lsls	r3, r3, #10
	orrs	r3, r2
	asrs	r2, r1, #1
	orrs	r3, r2
	movs	r2, #1
	add	r8, r2
	strh	r3, [r5, #0]
	mov	r3, r8
	adds	r5, #2
	cmp	r3, #135
	bne.n	.L_080e791c
	ldr	r3, [pc, #32]
.L_080e7968:
	movs	r4, #1
	add	r8, r4
	mov	r0, r8
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r0, #160
	bne.n	.L_080e7968
	ldr	r1, [sp, #64]
	cmp	r1, #1
	bne.n	.L_080e79a8
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_080e7984
	adds	r3, #3
.L_080e7984:
	asrs	r7, r3, #2
	b.n	.L_080e79b6
	.4byte 0x00000000
	.4byte 0x00007784
	.4byte 0x02020202
	.4byte 0x000004fe
	.4byte 0x080e72e1
	.4byte 0x00007098
	.4byte 0x0000778c
	.2byte 0x1b04
	.2byte 0x0300
.L_080e79a8:
	mov	r2, fp
	cmp	r2, #0
	bge.n	.L_080e79b0
	adds	r2, #3
.L_080e79b0:
	asrs	r2, r2, #2
	movs	r3, #64
	subs	r7, r3, r2
.L_080e79b6:
	movs	r2, #96
	mov	r3, fp
	subs	r3, r2, r3
	ldr	r4, [sp, #24]
	mov	sl, r3
	movs	r3, #0
	str	r3, [r4, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r4, #4]
	ldr	r0, [sp, #76]
	cmp	r0, #1
	bne.n	.L_080e7a1e
	ldr	r1, [sp, #16]
	movs	r2, #160
	ldr	r3, [sp, #44]
	lsls	r2, r2, #8
	adds	r6, r1, r2
	str	r6, [sp, #88]
	movs	r4, #160
	str	r6, [r3, #4]
	ldr	r0, [sp, #24]
	lsls	r4, r4, #15
	lsls	r3, r7, #16
	adds	r3, r3, r4
	str	r3, [r0, #0]
	mov	r1, sl
	movs	r3, #64
	subs	r3, r3, r1
	lsls	r3, r3, #16
	str	r3, [r0, #8]
	ldr	r2, [sp, #40]
	ldr	r4, [pc, #484]
	ldr	r3, [r2, #0]
	adds	r3, r3, r4
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #44]
	movs	r3, #0
	bl	sub_08009008
	ldr	r0, [sp, #40]
	ldr	r1, [pc, #468]
	ldr	r3, [r0, #0]
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #44]
	movs	r3, #0
	bl	sub_08009008
	b.n	.L_080e7a54
.L_080e7a1e:
	ldr	r3, [sp, #16]
	movs	r4, #128
	ldr	r0, [sp, #44]
	lsls	r4, r4, #9
	adds	r6, r3, r4
	str	r6, [sp, #88]
	movs	r1, #192
	str	r6, [r0, #4]
	lsls	r1, r1, #15
	ldr	r4, [sp, #24]
	lsls	r3, r7, #16
	adds	r3, r3, r1
	mov	r0, sl
	str	r3, [r4, #0]
	subs	r3, r2, r0
	lsls	r3, r3, #16
	str	r3, [r4, #8]
	ldr	r1, [sp, #60]
	ldr	r2, [pc, #408]
	ldr	r3, [r1, #0]
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #44]
	movs	r3, #0
	bl	sub_08009008
.L_080e7a54:
	movs	r3, #0
	mov	r4, sl
	mov	r8, r3
	movs	r3, #32
	subs	r4, r3, r4
	mov	sl, r4
	movs	r2, #0
.L_080e7a62:
	ldr	r0, [sp, #60]
	ldr	r3, [r0, #0]
	movs	r1, #225
	adds	r3, r3, r2
	lsls	r1, r1, #7
	adds	r5, r3, r1
	movs	r4, #1
	ldr	r3, [r5, #24]
	negs	r4, r4
	cmp	r3, r4
	bne.n	.L_080e7ad2
	bl	sub_08004458
	ldr	r3, [pc, #356]
	ands	r3, r0
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r1, r3, r0
	movs	r3, #0
	str	r3, [r5, #24]
	adds	r0, r1, #0
	str	r1, [sp, #12]
	bl	sub_08002322
	adds	r3, r7, #0
	adds	r3, #96
	lsls	r2, r3, #16
	lsls	r3, r0, #4
	subs	r3, r3, r0
	lsls	r3, r3, #1
	ldr	r1, [sp, #12]
	cmp	r3, #0
	bge.n	.L_080e7aa8
	ldr	r4, [pc, #320]
	adds	r3, r3, r4
.L_080e7aa8:
	asrs	r3, r3, #16
	muls	r3, r6
	adds	r3, r2, r3
	str	r3, [r5, #0]
	adds	r0, r1, #0
	bl	sub_0800231c
	lsls	r3, r0, #4
	subs	r3, r3, r0
	mov	r1, sl
	lsls	r3, r3, #1
	lsls	r2, r1, #16
	cmp	r3, #0
	bge.n	.L_080e7ac8
	ldr	r4, [pc, #288]
	adds	r3, r3, r4
.L_080e7ac8:
	asrs	r3, r3, #16
	muls	r3, r6
	subs	r3, r2, r3
	str	r3, [r5, #4]
	b.n	.L_080e7ade
.L_080e7ad2:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r2, #28
	cmp	r1, #32
	bne.n	.L_080e7a62
.L_080e7ade:
	add	r5, sp, #96
	movs	r3, #0
	str	r3, [r5, #0]
	str	r3, [r5, #4]
	movs	r3, #128
	lsls	r3, r3, #18
	str	r3, [r5, #8]
	bl	sub_080049ac
	adds	r0, r5, #0
	bl	sub_08004cb4
	movs	r0, #128
	lsls	r0, r0, #4
	bl	sub_08004c6c
	ldr	r0, [sp, #16]
	bl	sub_08004c1c
	movs	r2, #0
	ldr	r7, [pc, #228]
	mov	r8, r2
	add	r6, sp, #120
	add	r5, sp, #108
.L_080e7b0e:
	ldrh	r3, [r7, #0]
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	movs	r4, #2
	ldrsh	r3, [r7, r4]
	add	r3, fp
	lsls	r3, r3, #16
	str	r3, [r6, #4]
	ldrh	r3, [r7, #4]
	asrs	r2, r2, #1
	lsls	r2, r2, #16
	lsls	r3, r3, #16
	str	r2, [r6, #0]
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	lsls	r2, r2, #16
	adds	r1, r5, #0
	str	r2, [r6, #8]
	adds	r0, r6, #0
	bl	sub_080e3944
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	adds	r3, r2, #0
	adds	r3, #128
	str	r3, [r5, #0]
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	adds	r1, r3, #0
	adds	r1, #60
	str	r1, [r5, #4]
	ldr	r4, [sp, #60]
	movs	r0, #250
	ldr	r1, [r4, #0]
	lsls	r0, r0, #5
	adds	r1, r1, r0
	movs	r0, #8
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	adds	r2, #124
	adds	r3, #56
	ldr	r0, [pc, #132]
	ldr	r4, [sp, #68]
	bl	sub_080072f4
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r7, #6
	cmp	r1, #7
	bne.n	.L_080e7b0e
	ldr	r2, [sp, #64]
	cmp	r2, #1
	bne.n	.L_080e7b92
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_080e7b8a
	adds	r3, #3
.L_080e7b8a:
	asrs	r3, r3, #2
	adds	r7, r3, #0
	subs	r7, #16
	b.n	.L_080e7ba0
.L_080e7b92:
	mov	r2, fp
	cmp	r2, #0
	bge.n	.L_080e7b9a
	adds	r2, #3
.L_080e7b9a:
	asrs	r2, r2, #2
	movs	r3, #16
	subs	r7, r3, r2
.L_080e7ba0:
	movs	r3, #96
	negs	r3, r3
	add	r3, fp
	movs	r4, #0
	ldr	r5, [pc, #72]
	mov	sl, r3
	mov	r8, r4
.L_080e7bae:
	movs	r0, #2
	ldrsh	r3, [r5, r0]
	add	r3, sl
	cmp	r3, #93
	bgt.n	.L_080e7bf8
	ldr	r2, [sp, #60]
	ldr	r1, [r2, #0]
	movs	r0, #0
	ldrsh	r2, [r5, r0]
	movs	r4, #228
	movs	r0, #24
	lsls	r4, r4, #5
	adds	r2, r2, r7
	adds	r1, r1, r4
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #12
	subs	r3, #12
	ldr	r0, [pc, #28]
	ldr	r4, [sp, #68]
	bl	sub_080072f4
	b.n	.L_080e7c10
	.4byte 0x000077d8
	.4byte 0x000077dc
	.4byte 0x00007fff
	.4byte 0x0000ffff
	.4byte 0x080eee76
	.4byte 0x02010000
	.2byte 0xeea0
	.2byte 0x080e
.L_080e7bf8:
	cmp	r3, #95
	bgt.n	.L_080e7c10
	movs	r1, #0
	ldrsh	r0, [r5, r1]
	add	r2, sp, #284
	adds	r0, r0, r7
	mov	r9, r2
	lsls	r0, r0, #16
	lsls	r1, r3, #16
	movs	r2, #1
	bl	sub_080e7338
.L_080e7c10:
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r5, #4
	cmp	r4, #7
	bne.n	.L_080e7bae
	ldr	r1, [sp, #64]
	lsls	r3, r1, #2
	movs	r0, #0
	adds	r3, r3, r1
	mov	r8, r0
	lsls	r7, r3, #14
	movs	r6, #0
.L_080e7c2a:
	ldr	r3, [sp, #60]
	ldr	r2, [r3, #0]
	movs	r4, #225
	adds	r3, r2, r6
	lsls	r4, r4, #7
	adds	r5, r3, r4
	ldr	r1, [r5, #24]
	cmp	r1, #0
	blt.n	.L_080e7c78
	lsls	r1, r1, #10
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	adds	r1, r2, r1
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	movs	r0, #32
	subs	r3, #16
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #16
	ldr	r0, [pc, #228]
	ldr	r4, [sp, #68]
	bl	sub_080072f4
	ldr	r3, [r5, #0]
	subs	r3, r3, r7
	str	r3, [r5, #0]
	ldr	r0, [pc, #216]
	ldr	r3, [r5, #4]
	adds	r3, r3, r0
	str	r3, [r5, #4]
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #6
	bne.n	.L_080e7c78
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L_080e7c78:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r6, #28
	cmp	r2, #32
	bne.n	.L_080e7c2a
	ldr	r4, [sp, #40]
	ldr	r0, [pc, #184]
	ldr	r3, [r4, #0]
	movs	r2, #1
	adds	r3, r3, r0
	str	r2, [r3, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r2, #128
	ldr	r1, [sp, #16]
	lsls	r2, r2, #1
	adds	r1, r1, r2
	str	r1, [sp, #16]
	ldr	r4, [sp, #40]
	ldr	r0, [pc, #160]
	ldr	r2, [r4, #0]
	movs	r3, #1
	adds	r2, r2, r0
	add	fp, r3
	ldr	r3, [r2, #0]
	mov	r1, fp
	adds	r3, #1
	str	r3, [r2, #0]
	cmp	r1, #192
	beq.n	.L_080e7cba
	b.n	.L_080e78b2
.L_080e7cba:
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [sp, #48]
	movs	r5, #0
	str	r5, [r2, #16]
	ldr	r0, [pc, #128]
	bl	sub_08004278
	ldr	r0, [pc, #124]
	bl	sub_08004278
	ldr	r0, [pc, #124]
	bl	sub_08004278
	add	r4, sp, #56
	add	r0, sp, #52
	ldr	r3, [pc, #116]
	ldrh	r4, [r4, #0]
	ldrh	r0, [r0, #0]
	strh	r4, [r3, #4]
	strh	r0, [r3, #6]
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080d67dc
	ldr	r2, [pc, #100]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #100]
	str	r5, [r3, #0]
	ldr	r3, [pc, #100]
	adds	r2, #12
	str	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #38
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	subs	r2, #70
	strh	r3, [r2, #0]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	sub_080ed408
	ldr	r3, [pc, #68]
	adds	r3, #184
	ldr	r3, [r3, #0]
	ldr	r2, [sp, #60]
	str	r3, [sp, #68]
	ldr	r0, [pc, #64]
	ldr	r1, [r2, #0]
	b.n	.L_080e7d6c
	.4byte 0x00000080
	.4byte 0x00001010
	.4byte 0x00002784
	.4byte 0x02010000
	.4byte 0xfffb0000
	.4byte 0x00007824
	.4byte 0x0000778c
	.4byte 0x080c9139
	.4byte 0x080e72e1
	.4byte 0x080cd359
	.4byte 0x03001ad0
	.4byte 0x04000020
	.4byte 0x04000028
	.4byte 0xfffff000
	.4byte 0x03001e50
	.2byte 0x00c0
	.2byte 0x0000
.L_080e7d6c:
	movs	r3, #0
	movs	r2, #1
	bl	sub_080e0524
	movs	r3, #0
	mov	r8, r3
	movs	r7, #127
	movs	r6, #0
.L_080e7d7c:
	ldr	r4, [sp, #60]
	ldr	r5, [r4, #0]
	movs	r0, #225
	adds	r5, r5, r6
	lsls	r0, r0, #7
	adds	r5, r5, r0
	bl	sub_08004458
	ands	r0, r7
	str	r0, [r5, #0]
	bl	sub_08004458
	movs	r1, #1
	ands	r0, r7
	add	r8, r1
	adds	r0, #127
	mov	r2, r8
	str	r0, [r5, #4]
	adds	r6, #28
	cmp	r2, #32
	bne.n	.L_080e7d7c
	movs	r3, #0
	ldr	r5, [pc, #816]
	mov	r8, r3
	movs	r6, #0
	movs	r7, #255
.L_080e7db0:
	str	r6, [r5, #0]
	str	r6, [r5, #4]
	str	r6, [r5, #8]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #127
	lsls	r0, r0, #12
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r7
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #127
	movs	r4, #1
	lsls	r0, r0, #12
	add	r8, r4
	str	r0, [r5, #20]
	mov	r0, r8
	str	r6, [r5, #24]
	adds	r5, #28
	cmp	r0, #128
	bne.n	.L_080e7db0
	movs	r1, #0
	ldr	r5, [pc, #756]
	mov	r8, r1
	movs	r6, #0
	movs	r7, #255
.L_080e7df0:
	str	r6, [r5, #0]
	str	r6, [r5, #4]
	str	r6, [r5, #8]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #128
	lsls	r0, r0, #13
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r7
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	sub_08004458
	ands	r0, r7
	subs	r0, #128
	movs	r2, #1
	movs	r3, #128
	lsls	r0, r0, #13
	add	r8, r2
	lsls	r3, r3, #2
	str	r0, [r5, #20]
	str	r6, [r5, #24]
	adds	r5, #28
	cmp	r8, r3
	bne.n	.L_080e7df0
	ldr	r4, [sp, #60]
	movs	r0, #239
	ldr	r2, [r4, #0]
	lsls	r0, r0, #7
	adds	r1, r2, r0
	movs	r3, #1
	str	r3, [r1, #0]
	ldr	r1, [pc, #684]
	ldr	r3, [pc, #684]
	adds	r2, r2, r1
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #680]
	lsls	r1, r1, #3
	bl	sub_080041d8
	movs	r0, #232
	ldr	r3, [sp, #60]
	mov	r4, sp
	adds	r4, #80
	lsls	r0, r0, #9
	movs	r2, #0
	str	r3, [sp, #32]
	str	r4, [sp, #28]
	str	r0, [sp, #20]
	mov	fp, r2
.L_080e7e5c:
	ldr	r3, [pc, #656]
	mov	r1, fp
	subs	r1, #16
	ldr	r5, [r3, #0]
	str	r1, [sp, #36]
	cmp	r1, #19
	ble.n	.L_080e7e74
	movs	r0, #2
	movs	r1, #2
	movs	r2, #2
	bl	sub_080e727c
.L_080e7e74:
	mov	r2, fp
	cmp	r2, #0
	bne.n	.L_080e7e80
	movs	r0, #156
	bl	sub_080f9010
.L_080e7e80:
	mov	r3, fp
	cmp	r3, #40
	bne.n	.L_080e7e8c
	movs	r0, #145
	bl	sub_080f9010
.L_080e7e8c:
	mov	r4, fp
	cmp	r4, #48
	bne.n	.L_080e7ebe
	ldr	r0, [sp, #76]
	cmp	r0, #1
	bne.n	.L_080e7eb8
	ldr	r1, [sp, #32]
	ldr	r2, [pc, #600]
	ldr	r3, [r1, #0]
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_08009038
	ldr	r4, [sp, #32]
	ldr	r0, [pc, #588]
	ldr	r3, [r4, #0]
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl	sub_08009038
	bl	sub_080b5118
.L_080e7eb8:
	movs	r0, #134
	bl	sub_080b50e8
.L_080e7ebe:
	bl	sub_080049ac
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	sub_080051d8
	ldr	r7, [pc, #528]
	movs	r1, #0
	movs	r2, #63
	mov	r8, r1
	mov	sl, r2
.L_080e7ed6:
	ldr	r3, [r7, #4]
	cmp	r3, #0
	blt.n	.L_080e7f96
	add	r6, sp, #96
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	sub_080e3944
	ldr	r3, [r6, #0]
	ldr	r2, [r6, #8]
	asrs	r3, r3, #1
	str	r3, [r6, #0]
	cmp	r2, #159
	bgt.n	.L_080e7ef8
	movs	r3, #160
	str	r3, [r6, #8]
	movs	r2, #160
.L_080e7ef8:
	ldr	r3, [pc, #512]
	cmp	r2, r3
	ble.n	.L_080e7f02
	str	r3, [r6, #8]
	adds	r2, r3, #0
.L_080e7f02:
	adds	r3, r2, #0
	subs	r3, #160
	cmp	r3, #0
	bge.n	.L_080e7f0c
	adds	r3, #63
.L_080e7f0c:
	asrs	r3, r3, #6
	movs	r0, #9
	subs	r0, r0, r3
	ldr	r2, [pc, #492]
	lsls	r5, r0, #1
	subs	r3, r5, #2
	ldrh	r4, [r2, r3]
	movs	r3, #1
	mov	r2, r8
	ands	r2, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #7
	adds	r3, r3, r2
	ldr	r2, [sp, #60]
	lsls	r3, r3, #1
	ldr	r1, [r2, #0]
	adds	r4, r4, r3
	movs	r3, #200
	adds	r1, r1, r4
	lsls	r3, r3, #6
	adds	r1, r1, r3
	lsrs	r3, r0, #31
	ldr	r2, [r6, #0]
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r6, #4]
	ldr	r4, [sp, #68]
	subs	r3, r3, r0
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #72]
	bl	sub_080072f4
	ldr	r2, [pc, #432]
	adds	r0, r7, #0
	movs	r1, #64
	bl	sub_080e38b8
	movs	r2, #160
	ldr	r3, [r7, #4]
	lsls	r2, r2, #13
	cmp	r3, r2
	bgt.n	.L_080e7f96
	movs	r3, #0
	str	r3, [r7, #0]
	str	r3, [r7, #8]
	str	r2, [r7, #4]
	bl	sub_08004458
	mov	r1, sl
	ands	r0, r1
	subs	r0, #32
	lsls	r0, r0, #15
	str	r0, [r7, #12]
	bl	sub_08004458
	mov	r2, sl
	ands	r0, r2
	lsls	r0, r0, #13
	str	r0, [r7, #16]
	bl	sub_08004458
	mov	r3, sl
	ands	r0, r3
	subs	r0, #32
	lsls	r0, r0, #15
	str	r0, [r7, #20]
.L_080e7f96:
	movs	r4, #1
	add	r8, r4
	mov	r0, r8
	adds	r7, #28
	cmp	r0, #64
	bne.n	.L_080e7ed6
	movs	r1, #0
	mov	r8, r1
	mov	sl, r1
.L_080e7fa8:
	ldr	r2, [sp, #60]
	movs	r5, #7
	ldr	r1, [r2, #0]
	mov	r2, r8
	ands	r5, r2
	mov	r4, sl
	adds	r3, r1, r4
	movs	r0, #225
	adds	r4, r5, #3
	ldr	r2, [pc, #324]
	lsls	r6, r4, #1
	lsls	r0, r0, #7
	adds	r7, r3, r0
	subs	r3, r6, #2
	ldrh	r0, [r2, r3]
	movs	r3, #1
	mov	r2, r8
	ands	r2, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #7
	adds	r3, r3, r2
	lsls	r3, r3, #1
	ldr	r2, [r7, #0]
	adds	r0, r0, r3
	lsrs	r3, r4, #1
	adds	r1, r1, r0
	subs	r2, r2, r3
	movs	r0, #200
	ldr	r3, [r7, #4]
	lsls	r0, r0, #6
	subs	r3, r3, r4
	adds	r1, r1, r0
	str	r4, [sp, #0]
	str	r6, [sp, #4]
	ldr	r0, [sp, #72]
	ldr	r4, [sp, #68]
	bl	sub_080072f4
	ldr	r3, [r7, #4]
	movs	r0, #10
	subs	r3, r3, r5
	subs	r3, #8
	negs	r0, r0
	str	r3, [r7, #4]
	cmp	r3, r0
	bge.n	.L_080e800a
	movs	r3, #128
	str	r3, [r7, #4]
.L_080e800a:
	movs	r2, #1
	add	r8, r2
	movs	r1, #28
	mov	r3, r8
	add	sl, r1
	cmp	r3, #64
	bne.n	.L_080e7fa8
	movs	r4, #0
	movs	r0, #255
	ldr	r7, [pc, #188]
	mov	r8, r4
	mov	sl, r4
	mov	r9, r0
.L_080e8024:
	movs	r1, #3
	mov	r0, r8
	bl	sub_080022ec
	ldr	r1, [sp, #36]
	cmp	r0, r1
	bge.n	.L_080e80bc
	ldr	r3, [r7, #4]
	cmp	r3, #0
	blt.n	.L_080e80bc
	add	r5, sp, #96
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_080e3944
	ldr	r3, [r5, #0]
	asrs	r6, r3, #1
	str	r6, [r5, #0]
	ldr	r2, [r7, #24]
	cmp	r2, #13
	bhi.n	.L_080e807a
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	ldr	r4, [sp, #60]
	ldr	r2, [pc, #176]
	asrs	r3, r3, #1
	lsls	r3, r3, #1
	ldrh	r2, [r2, r3]
	ldr	r1, [r4, #0]
	adds	r1, r1, r2
	ldr	r2, [pc, #168]
	ldrh	r4, [r2, r3]
	ldr	r3, [r5, #4]
	lsrs	r0, r4, #1
	subs	r2, r6, r0
	subs	r3, r3, r0
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #72]
	ldr	r4, [sp, #68]
	bl	sub_080072f4
	ldr	r2, [r7, #24]
.L_080e807a:
	adds	r3, r2, #1
	str	r3, [r7, #24]
	cmp	r3, #14
	bne.n	.L_080e80b2
	movs	r3, #160
	lsls	r3, r3, #13
	mov	r0, sl
	str	r3, [r7, #4]
	str	r0, [r7, #0]
	bl	sub_08004458
	mov	r1, r9
	ands	r0, r1
	subs	r0, #127
	lsls	r0, r0, #16
	mov	r2, sl
	str	r0, [r7, #8]
	str	r2, [r7, #12]
	bl	sub_08004458
	mov	r3, r9
	ands	r0, r3
	mov	r4, sl
	lsls	r0, r0, #11
	str	r0, [r7, #16]
	str	r4, [r7, #20]
	str	r4, [r7, #24]
	b.n	.L_080e80bc
.L_080e80b2:
	adds	r0, r7, #0
	movs	r1, #64
	movs	r2, #1
	bl	sub_080e38b8
.L_080e80bc:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r7, #28
	cmp	r1, #64
	bne.n	.L_080e8024
	ldr	r2, [sp, #64]
	cmp	r2, #1
	bne.n	.L_080e8110
	mov	r4, fp
	lsrs	r3, r4, #31
	add	r3, fp
	asrs	r3, r3, #1
	adds	r1, r3, #0
	adds	r1, #24
	b.n	.L_080e811c
	.4byte 0x02010000
	.4byte 0x02010e00
	.4byte 0x00007784
	.4byte 0x10101010
	.4byte 0x080cd261
	.4byte 0x03001e80
	.4byte 0x000077d8
	.4byte 0x000077dc
	.4byte 0x0000031f
	.4byte 0x080ede48
	.4byte 0xffffe000
	.4byte 0x080eeebc
	.2byte 0xeeca
	.2byte 0x080e
.L_080e8110:
	mov	r0, fp
	lsrs	r3, r0, #31
	add	r3, fp
	asrs	r3, r3, #1
	movs	r2, #56
	subs	r1, r2, r3
.L_080e811c:
	mov	r3, fp
	lsls	r2, r3, #1
	mov	r4, fp
	movs	r3, #64
	subs	r0, r3, r2
	lsls	r3, r4, #8
	movs	r4, #128
	lsls	r4, r4, #10
	adds	r2, r3, r4
	ldr	r4, [sp, #24]
	movs	r3, #0
	str	r3, [r4, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r4, #4]
	ldr	r3, [sp, #76]
	cmp	r3, #1
	bne.n	.L_080e8186
	ldr	r4, [sp, #20]
	ldr	r2, [sp, #28]
	str	r4, [sp, #80]
	str	r4, [r2, #4]
	movs	r4, #192
	lsls	r3, r1, #16
	lsls	r4, r4, #15
	ldr	r1, [sp, #24]
	adds	r3, r3, r4
	str	r3, [r1, #0]
	movs	r3, #96
	subs	r3, r3, r0
	lsls	r3, r3, #16
	str	r3, [r1, #8]
	ldr	r2, [sp, #32]
	ldr	r4, [pc, #200]
	ldr	r3, [r2, #0]
	adds	r3, r3, r4
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	movs	r3, #0
	bl	sub_08009008
	ldr	r0, [sp, #32]
	ldr	r1, [pc, #184]
	ldr	r3, [r0, #0]
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	movs	r3, #0
	bl	sub_08009008
	b.n	.L_080e81b4
.L_080e8186:
	ldr	r3, [sp, #28]
	str	r2, [sp, #80]
	movs	r4, #192
	str	r2, [r3, #4]
	lsls	r4, r4, #15
	lsls	r3, r1, #16
	ldr	r1, [sp, #24]
	adds	r3, r3, r4
	str	r3, [r1, #0]
	movs	r3, #96
	subs	r3, r3, r0
	lsls	r3, r3, #16
	str	r3, [r1, #8]
	ldr	r2, [sp, #60]
	ldr	r4, [pc, #132]
	ldr	r3, [r2, #0]
	adds	r3, r3, r4
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #28]
	movs	r3, #0
	bl	sub_08009008
.L_080e81b4:
	ldr	r0, [sp, #32]
	ldr	r1, [pc, #120]
	ldr	r3, [r0, #0]
	movs	r2, #1
	adds	r3, r3, r1
	str	r2, [r3, #0]
	movs	r0, #8
	movs	r1, #8
	bl	sub_080e155c
	ldr	r4, [sp, #32]
	ldr	r0, [pc, #104]
	ldr	r3, [r4, #0]
	movs	r1, #1
	adds	r3, r3, r0
	str	r1, [r3, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r3, #128
	ldr	r2, [sp, #20]
	movs	r4, #1
	lsls	r3, r3, #1
	add	fp, r4
	adds	r2, r2, r3
	mov	r0, fp
	str	r2, [sp, #20]
	cmp	r0, #54
	beq.n	.L_080e81f0
	b.n	.L_080e7e5c
.L_080e81f0:
	ldr	r0, [pc, #68]
	bl	sub_08004278
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r1, [sp, #76]
	cmp	r1, #0
	bne.n	.L_080e8210
	ldr	r2, [sp, #60]
	ldr	r4, [pc, #32]
	ldr	r3, [r2, #0]
	adds	r3, r3, r4
	ldr	r0, [r3, #0]
	bl	sub_08009038
.L_080e8210:
	bl	sub_080cdbc0
	add	sp, #284
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000077d8
	.4byte 0x000077dc
	.4byte 0x000077a8
	.4byte 0x00007824
	.4byte 0x080cd261

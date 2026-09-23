.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_08002304, 0x08002304
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080072f4, 0x080072f4
	.set sub_08007300, 0x08007300
	.set sub_08009008, 0x08009008
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
	.set sub_080e3980, 0x080e3980
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Unnamed_080eb754
	.global Func_080eb754
	.thumb_func
Unnamed_080eb754:
Func_080eb754:
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

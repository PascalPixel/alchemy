.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080072f8, 0x080072f8
	.set sub_080072fc, 0x080072fc
	.set sub_08007300, 0x08007300
	.set sub_080b5070, 0x080b5070
	.set sub_080b5088, 0x080b5088
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080d6888, 0x080d6888
	.set sub_080de2f8, 0x080de2f8
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e3908, 0x080e3908
	.set sub_080e3944, 0x080e3944
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.set sub_08004278, 0x08004278
	.set sub_080cdbc0, 0x080cdbc0
	.global BattleFx_RunProjectileVolley
	.global Func_080dea70
	.thumb_func
BattleFx_RunProjectileVolley:
Func_080dea70:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #408]
	adds	r2, r3, #0
	adds	r5, r0, #0
	ldmia	r2!, {r0}
	ldr	r2, [r2, #0]
	sub	sp, #156
	str	r2, [sp, #132]
	adds	r2, r3, #0
	subs	r2, #108
	ldr	r2, [r2, #0]
	str	r2, [sp, #108]
	ldr	r3, [r3, #8]
	mov	r8, r1
	movs	r1, #0
	str	r3, [sp, #104]
	str	r1, [sp, #100]
	ldr	r3, [pc, #380]
	mov	fp, r0
	add	r3, fp
	ldr	r2, [r5, #24]
	str	r5, [r3, #0]
	mov	r3, r8
	str	r2, [sp, #92]
	cmp	r3, #10
	bne.n	.L_080deab8
	movs	r0, #0
	bl	sub_080cd594
	b.n	.L_080deabe
.L_080deab8:
	movs	r0, #1
	bl	sub_080cd594
.L_080deabe:
	ldr	r6, [pc, #348]
	add	r6, fp
	ldr	r2, [r6, #0]
	ldr	r3, [r2, #28]
	cmp	r3, #1
	bne.n	.L_080deae0
	add	r3, sp, #140
	ldr	r2, [r2, #4]
	str	r3, [sp, #0]
	add	r3, sp, #136
	str	r3, [sp, #4]
	adds	r0, r5, #0
	movs	r1, #1
	movs	r3, #2
	bl	sub_080de2f8
	ldr	r2, [r6, #0]
.L_080deae0:
	mov	r4, r8
	cmp	r4, #5
	bne.n	.L_080deb02
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_080deaf0
	movs	r3, #2
	b.n	.L_080deb0a
.L_080deaf0:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	bl	sub_080ed408
	b.n	.L_080deb28
.L_080deb02:
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_080deb1a
	movs	r3, #3
.L_080deb0a:
	str	r3, [sp, #0]
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	bl	sub_080ed408
	b.n	.L_080deb28
.L_080deb1a:
	movs	r3, #3
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	str	r3, [sp, #0]
	bl	sub_080ed408
.L_080deb28:
	ldr	r3, [pc, #244]
	adds	r3, #184
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #244]
	str	r3, [sp, #112]
	ldr	r1, [sp, #104]
	movs	r2, #0
	movs	r3, #0
	mov	r6, r8
	bl	sub_080e0524
	cmp	r6, #0
	beq.n	.L_080deb4a
	cmp	r6, #5
	beq.n	.L_080deb50
	cmp	r6, #8
	bne.n	.L_080debe8
.L_080deb4a:
	mov	r7, r8
	cmp	r7, #5
	bne.n	.L_080deb54
.L_080deb50:
	movs	r0, #2
	str	r0, [sp, #92]
.L_080deb54:
	mov	r1, r8
	cmp	r1, #8
	bne.n	.L_080deb5e
	movs	r2, #0
	str	r2, [sp, #92]
.L_080deb5e:
	ldr	r3, [sp, #92]
	cmp	r3, #0
	bne.n	.L_080deb6c
	movs	r1, #128
	lsls	r1, r1, #5
	ldr	r0, [pc, #188]
	b.n	.L_080deb78
.L_080deb6c:
	ldr	r4, [sp, #92]
	cmp	r4, #1
	bne.n	.L_080deb84
	movs	r1, #128
	lsls	r1, r1, #5
	ldr	r0, [pc, #180]
.L_080deb78:
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	b.n	.L_080deb94
.L_080deb84:
	movs	r1, #128
	lsls	r1, r1, #5
	ldr	r0, [pc, #164]
	add	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
.L_080deb94:
	mov	r6, r8
	cmp	r6, #5
	bne.n	.L_080debae
	ldr	r0, [pc, #152]
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #144]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	sub_080072f0
.L_080debae:
	movs	r1, #128
	lsls	r1, r1, #6
	ldr	r0, [pc, #136]
	add	r1, fp
	movs	r2, #1
	movs	r3, #0
	mov	r7, r8
	bl	sub_080e0524
	cmp	r7, #5
	bne.n	.L_080debd8
	ldr	r0, [pc, #108]
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #104]
	lsls	r0, r0, #19
	movs	r2, #128
	bl	sub_080072f0
.L_080debd8:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #92]
	movs	r3, #75
	b.n	.L_080decd4
.L_080debe8:
	mov	r0, r8
	cmp	r0, #1
	bne.n	.L_080dec4c
	mov	r1, fp
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #76]
	bl	sub_080e0524
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #20]
	strh	r3, [r2, #0]
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #56]
	add	r3, fp
	mov	r1, r8
	str	r1, [r3, #0]
	add	r2, fp
	movs	r3, #0
	b.n	.L_080decd6
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x03001e50
	.4byte 0x00000073
	.4byte 0x0000007f
	.4byte 0x00000080
	.4byte 0x00000081
	.4byte 0x000000b9
	.4byte 0x03001388
	.4byte 0x000000c7
	.4byte 0x00007784
	.4byte 0x0000005d
	.2byte 0x0050
	.2byte 0x0400
.L_080dec4c:
	mov	r2, r8
	cmp	r2, #2
	bne.n	.L_080dec84
	ldr	r0, [pc, #816]
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #808]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #800]
	mov	r1, fp
	bl	sub_080e0524
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #792]
	add	r3, fp
	mov	r4, r8
	str	r4, [r3, #0]
	add	r2, fp
	movs	r3, #50
	b.n	.L_080decd6
.L_080dec84:
	mov	r3, r8
	subs	r3, #3
	cmp	r3, #1
	bls.n	.L_080dec92
	mov	r6, r8
	cmp	r6, #11
	bne.n	.L_080dec9c
.L_080dec92:
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #764]
	mov	r1, fp
	b.n	.L_080decc2
.L_080dec9c:
	mov	r7, r8
	cmp	r7, #6
	bne.n	.L_080decac
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #752]
	mov	r1, fp
	b.n	.L_080decc2
.L_080decac:
	ldr	r0, [pc, #748]
	mov	r1, fp
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	ldr	r1, [pc, #740]
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #740]
	add	r1, fp
.L_080decc2:
	bl	sub_080e0524
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #700]
	movs	r3, #50
.L_080decd4:
	add	r2, fp
.L_080decd6:
	str	r3, [r2, #0]
	ldr	r5, [pc, #716]
	movs	r1, #144
	lsls	r1, r1, #3
	add	r5, fp
	ldr	r0, [pc, #712]
	bl	sub_080041d8
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5098
	ldr	r6, [r0, #0]
	mov	r0, r8
	lsls	r0, r0, #2
	str	r0, [sp, #84]
	ldr	r1, [pc, #696]
	adds	r3, r0, #0
	add	r3, r8
	ldrb	r2, [r1, r3]
	str	r2, [sp, #88]
	adds	r2, r3, #1
	ldrb	r2, [r1, r2]
	mov	sl, r2
	adds	r2, r3, #2
	ldrb	r2, [r1, r2]
	str	r2, [sp, #80]
	adds	r3, #3
	ldrb	r1, [r1, r3]
	str	r1, [sp, #76]
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5070
	str	r0, [sp, #72]
	ldr	r5, [r5, #0]
	ldr	r4, [sp, #88]
	ldr	r3, [r5, #20]
	muls	r3, r4
	cmp	r3, #63
	ble.n	.L_080ded2c
	movs	r3, #1
	str	r3, [r5, #20]
.L_080ded2c:
	movs	r7, #0
	str	r7, [sp, #128]
	ldr	r3, [r5, #20]
	cmp	r3, #0
	bne.n	.L_080ded38
	b.n	.L_080def48
.L_080ded38:
	ldr	r0, [pc, #620]
	movs	r1, #0
	add	r0, fp
	movs	r2, #36
	str	r0, [sp, #68]
	str	r1, [sp, #24]
	str	r2, [sp, #20]
.L_080ded46:
	ldr	r4, [sp, #68]
	ldr	r7, [sp, #20]
	ldr	r3, [r4, #0]
	ldrsh	r0, [r3, r7]
	bl	sub_080b5098
	ldr	r2, [sp, #68]
	ldr	r4, [sp, #20]
	ldr	r3, [r2, #0]
	ldr	r7, [r0, #0]
	ldrsh	r0, [r3, r4]
	bl	sub_080b5070
	ldr	r3, [sp, #88]
	movs	r2, #0
	str	r0, [sp, #64]
	str	r2, [sp, #124]
	cmp	r3, #0
	bne.n	.L_080ded6e
	b.n	.L_080def28
.L_080ded6e:
	ldr	r4, [sp, #72]
	lsrs	r3, r4, #31
	ldr	r0, [sp, #24]
	adds	r3, r4, r3
	asrs	r3, r3, #1
	mov	r9, r3
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #2
	movs	r1, #225
	add	r3, fp
	lsls	r1, r1, #7
	adds	r5, r3, r1
.L_080ded88:
	ldr	r3, [r6, #8]
	mov	r2, r8
	str	r3, [r5, #0]
	cmp	r2, #7
	bne.n	.L_080deda8
	bl	sub_08004458
	movs	r2, #15
	ldr	r3, [r6, #12]
	ands	r2, r0
	lsls	r2, r2, #16
	movs	r4, #232
	adds	r3, r3, r2
	lsls	r4, r4, #14
	adds	r3, r3, r4
	b.n	.L_080dee1a
.L_080deda8:
	mov	r0, r8
	cmp	r0, #10
	bne.n	.L_080dedb4
	ldr	r3, [r6, #12]
	add	r3, r9
	b.n	.L_080dee1a
.L_080dedb4:
	mov	r1, r8
	cmp	r1, #6
	bne.n	.L_080dedc0
	ldr	r3, [r6, #12]
	add	r3, r9
	b.n	.L_080dee1a
.L_080dedc0:
	mov	r2, r8
	cmp	r2, #9
	bne.n	.L_080dedd4
	bl	sub_08004458
	movs	r2, #31
	ldr	r1, [r6, #12]
	ands	r2, r0
	movs	r3, #16
	b.n	.L_080dedfc
.L_080dedd4:
	mov	r3, r8
	subs	r3, #3
	cmp	r3, #1
	bhi.n	.L_080dedea
	bl	sub_08004458
	movs	r2, #31
	ldr	r1, [r6, #12]
	ands	r2, r0
	movs	r3, #16
	b.n	.L_080dedfc
.L_080dedea:
	mov	r3, r8
	cmp	r3, #11
	bne.n	.L_080dee08
	bl	sub_08004458
	movs	r2, #63
	ldr	r1, [r6, #12]
	ands	r2, r0
	movs	r3, #32
.L_080dedfc:
	subs	r3, r3, r2
	add	r1, r9
	lsls	r3, r3, #16
	adds	r1, r1, r3
	str	r1, [r5, #4]
	b.n	.L_080dee1c
.L_080dee08:
	mov	r4, r8
	cmp	r4, #5
	bne.n	.L_080dee14
	ldr	r3, [r6, #12]
	add	r3, r9
	b.n	.L_080dee1a
.L_080dee14:
	ldr	r3, [r6, #12]
	ldr	r0, [sp, #72]
	adds	r3, r3, r0
.L_080dee1a:
	str	r3, [r5, #4]
.L_080dee1c:
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	ldr	r0, [r7, #8]
	ldr	r3, [r5, #0]
	mov	r1, sl
	subs	r0, r0, r3
	bl	sub_080022ec
	mov	r1, r8
	str	r0, [r5, #12]
	cmp	r1, #7
	bne.n	.L_080dee4e
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	ldr	r0, [r7, #12]
	lsls	r3, r3, #16
	adds	r0, r0, r3
	ldr	r3, [r5, #4]
	ldr	r2, [pc, #364]
	subs	r0, r0, r3
	adds	r0, r0, r2
	mov	r1, sl
	b.n	.L_080def00
.L_080dee4e:
	mov	r3, r8
	cmp	r3, #8
	bne.n	.L_080dee70
	bl	sub_08004458
	movs	r3, #7
	ands	r3, r0
	ldr	r0, [r7, #12]
	lsls	r3, r3, #16
	adds	r0, r0, r3
	ldr	r3, [r5, #4]
	movs	r4, #176
	subs	r0, r0, r3
	lsls	r4, r4, #13
	adds	r0, r0, r4
	mov	r1, sl
	b.n	.L_080def00
.L_080dee70:
	mov	r0, r8
	cmp	r0, #9
	bne.n	.L_080dee8a
	bl	sub_08004458
	movs	r2, #63
	ands	r2, r0
	movs	r3, #64
	subs	r3, r3, r2
	ldr	r0, [r7, #12]
	lsls	r3, r3, #16
	adds	r0, r0, r3
	b.n	.L_080deefa
.L_080dee8a:
	mov	r1, r8
	cmp	r1, #10
	bne.n	.L_080deeac
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	ldr	r0, [r7, #12]
	lsls	r3, r3, #16
	adds	r0, r0, r3
	ldr	r3, [r5, #4]
	movs	r2, #128
	subs	r0, r0, r3
	lsls	r2, r2, #11
	adds	r0, r0, r2
	mov	r1, sl
	b.n	.L_080def00
.L_080deeac:
	mov	r3, r8
	subs	r3, #3
	cmp	r3, #1
	bls.n	.L_080deebe
	mov	r3, r8
	cmp	r3, #11
	beq.n	.L_080deebe
	cmp	r3, #5
	bne.n	.L_080deec4
.L_080deebe:
	movs	r3, #0
	str	r3, [r5, #16]
	b.n	.L_080def06
.L_080deec4:
	mov	r4, r8
	cmp	r4, #6
	bne.n	.L_080deee6
	bl	sub_08004458
	adds	r2, r0, #0
	ldr	r0, [sp, #64]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	ldr	r0, [r7, #12]
	asrs	r3, r3, #1
	adds	r0, r0, r3
	movs	r3, #15
	ands	r3, r2
	lsls	r3, r3, #16
	adds	r0, r0, r3
	b.n	.L_080deefa
.L_080deee6:
	bl	sub_08004458
	movs	r3, #15
	ldr	r1, [sp, #64]
	adds	r2, r0, #0
	ldr	r0, [r7, #12]
	ands	r3, r2
	adds	r0, r0, r1
	lsls	r3, r3, #16
	subs	r0, r0, r3
.L_080deefa:
	ldr	r3, [r5, #4]
	mov	r1, sl
	subs	r0, r0, r3
.L_080def00:
	bl	sub_080022ec
	str	r0, [r5, #16]
.L_080def06:
	ldr	r3, [r5, #8]
	ldr	r0, [r7, #16]
	mov	r1, sl
	subs	r0, r0, r3
	bl	sub_080022ec
	movs	r3, #0
	str	r0, [r5, #20]
	str	r3, [r5, #24]
	ldr	r2, [sp, #124]
	ldr	r3, [sp, #88]
	adds	r2, #1
	adds	r5, #28
	str	r2, [sp, #124]
	cmp	r2, r3
	beq.n	.L_080def28
	b.n	.L_080ded88
.L_080def28:
	ldr	r4, [sp, #24]
	ldr	r0, [sp, #20]
	ldr	r1, [sp, #128]
	ldr	r7, [sp, #88]
	adds	r0, #2
	adds	r4, r4, r7
	adds	r1, #1
	str	r4, [sp, #24]
	str	r0, [sp, #20]
	str	r1, [sp, #128]
	ldr	r2, [sp, #68]
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	cmp	r1, r3
	beq.n	.L_080def48
	b.n	.L_080ded46
.L_080def48:
	movs	r3, #0
	str	r3, [sp, #128]
	movs	r2, #128
	ldr	r3, [pc, #104]
	movs	r1, #0
	lsls	r2, r2, #3
.L_080def54:
	str	r1, [r3, #0]
	ldr	r4, [sp, #128]
	adds	r4, #1
	adds	r3, #28
	str	r4, [sp, #128]
	cmp	r4, r2
	bne.n	.L_080def54
	mov	r6, r8
	cmp	r6, #6
	bne.n	.L_080defbc
	ldr	r3, [pc, #60]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r7, [sp, #76]
	ldr	r3, [r3, #20]
	ldr	r0, [sp, #80]
	ldr	r1, [sp, #88]
	muls	r3, r7
	adds	r2, r0, #0
	muls	r2, r1
	adds	r3, r3, r2
	adds	r3, #32
	b.n	.L_080defd4
	movs	r0, r0
	.4byte 0x0000007f
	.4byte 0x03001388
	.4byte 0x0000005c
	.4byte 0x00007784
	.4byte 0x0000005b
	.4byte 0x00000068
	.4byte 0x000000b8
	.4byte 0x000065c0
	.4byte 0x00000092
	.4byte 0x00007828
	.4byte 0x080cd261
	.4byte 0x080eebec
	.4byte 0xfff40000
	.2byte 0x0018
	.2byte 0x0201
.L_080defbc:
	ldr	r3, [pc, #824]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r2, [sp, #76]
	ldr	r3, [r3, #20]
	ldr	r4, [sp, #80]
	ldr	r6, [sp, #88]
	muls	r3, r2
	adds	r2, r4, #0
	muls	r2, r6
	adds	r3, r3, r2
	adds	r3, #16
.L_080defd4:
	str	r3, [sp, #96]
	ldr	r0, [sp, #96]
	movs	r7, #0
	str	r7, [sp, #120]
	cmp	r0, #0
	bne.n	.L_080defe4
	bl	.L_080df864
.L_080defe4:
	ldr	r1, [sp, #108]
	adds	r1, #12
	str	r1, [sp, #48]
.L_080defea:
	ldr	r2, [sp, #100]
	cmp	r2, #0
	ble.n	.L_080deff4
	subs	r2, #1
	str	r2, [sp, #100]
.L_080deff4:
	mov	r3, r8
	cmp	r3, #6
	bne.n	.L_080df014
	ldr	r4, [sp, #120]
	cmp	r4, #4
	bne.n	.L_080df006
	movs	r0, #136
	bl	sub_080f9010
.L_080df006:
	ldr	r6, [sp, #120]
	cmp	r6, #32
	bne.n	.L_080df03a
	movs	r0, #134
	bl	sub_080b50e8
	b.n	.L_080df03a
.L_080df014:
	mov	r7, r8
	cmp	r7, #7
	bne.n	.L_080df028
	ldr	r0, [sp, #120]
	cmp	r0, #48
	bne.n	.L_080df03a
	movs	r0, #133
	bl	sub_080b50e8
	b.n	.L_080df03a
.L_080df028:
	mov	r1, r8
	cmp	r1, #5
	beq.n	.L_080df03a
	ldr	r2, [sp, #120]
	cmp	r2, #16
	bne.n	.L_080df03a
	movs	r0, #133
	bl	sub_080b50e8
.L_080df03a:
	ldr	r7, [pc, #700]
	add	r7, fp
	ldr	r3, [r7, #0]
	ldr	r3, [r3, #28]
	cmp	r3, #1
	bne.n	.L_080df0ec
	ldr	r3, [sp, #120]
	lsls	r5, r3, #11
	adds	r0, r5, #0
	bl	sub_08002322
	ldr	r3, [sp, #140]
	negs	r0, r0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	lsls	r0, r0, #2
	asrs	r3, r3, #1
	asrs	r0, r0, #16
	adds	r0, r0, r3
	subs	r0, #10
	mov	sl, r0
	adds	r0, r5, #0
	bl	sub_0800231c
	lsls	r0, r0, #1
	ldr	r3, [sp, #136]
	asrs	r0, r0, #16
	adds	r0, r0, r3
	ldr	r4, [sp, #120]
	adds	r6, r0, #0
	subs	r6, #24
	cmp	r4, #69
	ble.n	.L_080df084
	lsls	r3, r4, #1
	subs	r3, r6, r3
	adds	r6, r3, #0
	adds	r6, #138
.L_080df084:
	ldr	r3, [r7, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080df09e
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	bl	sub_080ed408
	b.n	.L_080df0ac
.L_080df09e:
	movs	r3, #3
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	str	r3, [sp, #0]
	bl	sub_080ed408
.L_080df0ac:
	ldr	r3, [pc, #588]
	adds	r3, #188
	ldr	r5, [r3, #0]
	ldr	r7, [sp, #120]
	str	r5, [sp, #116]
	cmp	r7, #3
	bgt.n	.L_080df0d0
	movs	r3, #20
	ldr	r1, [pc, #576]
	str	r3, [sp, #0]
	movs	r3, #40
	str	r3, [sp, #4]
	ldr	r0, [sp, #132]
	add	r1, fp
	mov	r2, sl
	adds	r3, r6, #0
	bl	sub_080072f8
.L_080df0d0:
	movs	r3, #20
	ldr	r1, [pc, #556]
	str	r3, [sp, #0]
	movs	r3, #40
	str	r3, [sp, #4]
	ldr	r0, [sp, #132]
	add	r1, fp
	mov	r2, sl
	adds	r3, r6, #0
	bl	sub_080072f8
	movs	r0, #47
	bl	sub_08002dd8
.L_080df0ec:
	bl	sub_080049ac
	ldr	r0, [sp, #108]
	ldr	r1, [sp, #48]
	bl	sub_080051d8
	movs	r0, #0
	str	r0, [sp, #60]
	str	r0, [sp, #128]
	ldr	r2, [pc, #504]
	mov	r1, fp
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L_080df10c
	b.n	.L_080df5c0
.L_080df10c:
	movs	r3, #36
	str	r3, [sp, #32]
	str	r0, [sp, #28]
.L_080df112:
	mov	r4, fp
	ldr	r3, [r4, r2]
	ldr	r6, [sp, #32]
	ldrsh	r0, [r3, r6]
	bl	sub_080b5098
	ldr	r0, [r0, #0]
	ldr	r1, [sp, #88]
	str	r0, [sp, #56]
	movs	r0, #0
	str	r0, [sp, #124]
	cmp	r1, #0
	bne.n	.L_080df12e
	b.n	.L_080df59e
.L_080df12e:
	ldr	r4, [sp, #60]
	lsls	r3, r4, #3
	subs	r3, r3, r4
	ldr	r6, [pc, #460]
	ldr	r2, [sp, #28]
	lsls	r3, r3, #2
	adds	r6, r3, r6
	str	r2, [sp, #44]
	str	r6, [sp, #40]
	str	r0, [sp, #36]
.L_080df142:
	ldr	r7, [sp, #36]
	ldr	r0, [sp, #44]
	ldr	r1, [sp, #120]
	adds	r3, r7, r0
	cmp	r3, r1
	blt.n	.L_080df150
	b.n	.L_080df57c
.L_080df150:
	ldr	r3, [sp, #88]
	ldr	r4, [sp, #128]
	adds	r2, r3, #0
	muls	r2, r4
	ldr	r6, [sp, #124]
	adds	r2, r2, r6
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	movs	r7, #225
	add	r3, fp
	lsls	r7, r7, #7
	adds	r6, r3, r7
	add	r7, sp, #144
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	sub_080e3944
	ldr	r3, [r7, #0]
	asrs	r3, r3, #1
	str	r3, [r7, #0]
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #12]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	ldr	r3, [r6, #16]
	ldr	r2, [r6, #4]
	adds	r2, r2, r3
	str	r2, [r6, #4]
	ldr	r3, [r6, #20]
	ldr	r2, [r6, #8]
	mov	r0, r8
	adds	r2, r2, r3
	str	r2, [r6, #8]
	cmp	r0, #6
	bne.n	.L_080df1fe
	movs	r1, #128
	ldr	r5, [pc, #364]
	movs	r2, #0
	lsls	r1, r1, #2
	mov	sl, r7
	movs	r4, #255
.L_080df1a4:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L_080df1f2
	mov	r0, sl
	ldr	r3, [r0, #0]
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r3, [r0, #4]
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	str	r4, [sp, #8]
	bl	sub_08004458
	ldr	r4, [sp, #8]
	ands	r0, r4
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08004458
	ldr	r4, [sp, #8]
	ands	r0, r4
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	sub_08004458
	movs	r3, #7
	ldr	r2, [sp, #12]
	ands	r3, r0
	adds	r3, #32
	adds	r2, #1
	str	r3, [r5, #24]
	ldr	r1, [sp, #16]
	ldr	r4, [sp, #8]
	cmp	r2, #2
	beq.n	.L_080df1fe
.L_080df1f2:
	movs	r3, #128
	adds	r1, #1
	lsls	r3, r3, #3
	adds	r5, #28
	cmp	r1, r3
	bne.n	.L_080df1a4
.L_080df1fe:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	beq.n	.L_080df206
	b.n	.L_080df3a8
.L_080df206:
	ldr	r4, [sp, #56]
	ldr	r1, [r4, #8]
	ldr	r2, [r6, #0]
	cmp	r1, #0
	bge.n	.L_080df218
	lsrs	r3, r2, #31
	cmp	r3, #0
	bne.n	.L_080df222
	b.n	.L_080df3a8
.L_080df218:
	mvns	r3, r2
	lsrs	r3, r3, #31
	cmp	r3, #0
	bne.n	.L_080df222
	b.n	.L_080df3a8
.L_080df222:
	adds	r3, r2, #0
	cmp	r3, #0
	bge.n	.L_080df22a
	negs	r3, r3
.L_080df22a:
	adds	r2, r1, #0
	cmp	r2, #0
	bge.n	.L_080df232
	negs	r2, r2
.L_080df232:
	cmp	r3, r2
	bge.n	.L_080df238
	b.n	.L_080df3a8
.L_080df238:
	movs	r0, #0
	movs	r3, #1
	mov	r1, r8
	str	r0, [sp, #52]
	ldr	r2, [sp, #40]
	str	r3, [r6, #24]
	cmp	r1, #5
	bne.n	.L_080df252
	movs	r0, #134
	str	r2, [sp, #12]
	bl	sub_080b50e8
	b.n	.L_080df26a
.L_080df252:
	mov	r3, r8
	cmp	r3, #6
	beq.n	.L_080df26c
	ldr	r4, [sp, #100]
	cmp	r4, #0
	bne.n	.L_080df26c
	movs	r0, #8
	str	r0, [sp, #100]
	movs	r0, #132
	str	r2, [sp, #12]
	bl	sub_080f9010
.L_080df26a:
	ldr	r2, [sp, #12]
.L_080df26c:
	mov	r1, r8
	cmp	r1, #2
	bne.n	.L_080df2a0
	str	r2, [sp, #12]
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r3, #32
	lsls	r5, r3, #12
	bl	sub_08004458
	movs	r3, #1
	ands	r0, r3
	ldr	r2, [sp, #12]
	cmp	r0, #0
	beq.n	.L_080df294
	ldr	r3, [r6, #16]
	adds	r3, r3, r5
	b.n	.L_080df298
.L_080df294:
	ldr	r3, [r6, #16]
	subs	r3, r3, r5
.L_080df298:
	str	r3, [r6, #16]
	ldr	r3, [r6, #12]
	negs	r3, r3
	str	r3, [r6, #12]
.L_080df2a0:
	movs	r3, #1
	str	r3, [r2, #24]
	ldr	r3, [r7, #0]
	str	r3, [r2, #0]
	ldr	r3, [r7, #4]
	str	r3, [r2, #4]
	movs	r3, #0
	str	r3, [r2, #8]
	mov	r2, r8
	cmp	r2, #7
	beq.n	.L_080df2be
	ldr	r2, [pc, #84]
	movs	r3, #2
	add	r2, fp
	str	r3, [r2, #0]
.L_080df2be:
	ldr	r5, [pc, #56]
	add	r5, fp
	ldr	r3, [r5, #0]
	ldr	r4, [sp, #32]
	ldrsh	r0, [r3, r4]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r1, #7
	ldr	r3, [sp, #128]
	bl	sub_080d6888
	mov	r2, r8
	cmp	r2, #7
	beq.n	.L_080df31c
	cmp	r2, #9
	beq.n	.L_080df31c
	cmp	r2, #10
	beq.n	.L_080df31c
	cmp	r2, #5
	bne.n	.L_080df310
	ldr	r3, [r5, #0]
	ldr	r4, [sp, #32]
	ldrsh	r0, [r3, r4]
	movs	r1, #4
	bl	sub_080b5088
	b.n	.L_080df31c
	movs	r0, r0
	.4byte 0x00007828
	.4byte 0x03001e50
	.4byte 0x000065c0
	.4byte 0x02010000
	.4byte 0x02013800
	.2byte 0x77a8
	.2byte 0x0000
.L_080df310:
	ldr	r3, [r5, #0]
	ldr	r2, [sp, #32]
	movs	r1, #5
	ldrsh	r0, [r3, r2]
	bl	sub_080b5088
.L_080df31c:
	movs	r2, #5
	mov	r0, r8
	eors	r2, r0
	negs	r3, r2
	orrs	r3, r2
	lsrs	r2, r3, #31
	movs	r3, #12
	subs	r2, r3, r2
	movs	r3, #255
	mov	r9, r3
	ldr	r3, [sp, #84]
	add	r3, r8
	adds	r3, #4
	ldr	r5, [pc, #852]
	movs	r1, #100
	adds	r4, r7, #0
	mov	sl, r3
.L_080df33e:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L_080df39c
	ldr	r3, [r4, #0]
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r3, [r4, #4]
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	str	r4, [sp, #8]
	bl	sub_08004458
	mov	r3, r9
	ldr	r2, [sp, #12]
	ands	r0, r3
	subs	r0, #128
	lsls	r0, r2
	str	r0, [r5, #12]
	bl	sub_08004458
	mov	r3, r9
	ands	r0, r3
	ldr	r2, [sp, #12]
	subs	r0, #128
	lsls	r0, r2
	str	r0, [r5, #16]
	bl	sub_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #16
	str	r3, [r5, #24]
	ldr	r0, [sp, #52]
	adds	r0, #1
	str	r0, [sp, #52]
	ldr	r3, [pc, #772]
	mov	r0, sl
	ldrb	r3, [r3, r0]
	mov	ip, r3
	ldr	r3, [sp, #52]
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #12]
	ldr	r4, [sp, #8]
	cmp	r3, ip
	beq.n	.L_080df3a8
.L_080df39c:
	movs	r0, #128
	adds	r1, #1
	lsls	r0, r0, #2
	adds	r5, #28
	cmp	r1, r0
	bne.n	.L_080df33e
.L_080df3a8:
	mov	r1, r8
	cmp	r1, #0
	beq.n	.L_080df3b6
	cmp	r1, #5
	beq.n	.L_080df3b6
	cmp	r1, #8
	bne.n	.L_080df466
.L_080df3b6:
	ldr	r3, [pc, #732]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080df3d4
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	bl	sub_080ed408
	b.n	.L_080df3e4
.L_080df3d4:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	bl	sub_080ed408
.L_080df3e4:
	ldr	r4, [sp, #92]
	ldr	r3, [pc, #688]
	ldrb	r0, [r3, r4]
	movs	r1, #32
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	ldr	r6, [pc, #680]
	str	r1, [sp, #0]
	str	r0, [sp, #4]
	movs	r1, #128
	lsls	r1, r1, #5
	subs	r3, r3, r0
	subs	r2, #16
	ldr	r4, [r6, #0]
	ldr	r0, [sp, #132]
	add	r1, fp
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	ldr	r3, [pc, #644]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080df42c
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #15
	bl	sub_080ed408
	b.n	.L_080df43c
.L_080df42c:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #11
	bl	sub_080ed408
.L_080df43c:
	movs	r1, #32
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	str	r1, [sp, #0]
	ldr	r7, [sp, #92]
	ldr	r1, [pc, #592]
	ldrb	r1, [r1, r7]
	ldr	r0, [pc, #592]
	str	r1, [sp, #4]
	movs	r1, #128
	lsls	r1, r1, #5
	ldr	r4, [r0, #0]
	subs	r2, #16
	ldr	r0, [sp, #132]
	add	r1, fp
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	b.n	.L_080df57c
.L_080df466:
	mov	r1, r8
	cmp	r1, #1
	bne.n	.L_080df4a6
	ldr	r0, [r6, #0]
	cmp	r0, #0
	bge.n	.L_080df474
	negs	r0, r0
.L_080df474:
	ldr	r2, [sp, #56]
	ldr	r1, [r2, #8]
	cmp	r1, #0
	bge.n	.L_080df47e
	negs	r1, r1
.L_080df47e:
	cmp	r0, r1
	bgt.n	.L_080df57c
	movs	r1, #6
	ldr	r0, [sp, #120]
	bl	sub_080022fc
	lsls	r1, r0, #1
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	adds	r1, r1, r0
	movs	r0, #32
	lsls	r1, r1, #8
	str	r0, [sp, #0]
	movs	r0, #24
	str	r0, [sp, #4]
	add	r1, fp
	subs	r2, #16
	subs	r3, #12
	ldr	r0, [sp, #132]
	b.n	.L_080df55a
.L_080df4a6:
	mov	r6, r8
	cmp	r6, #7
	beq.n	.L_080df4b4
	cmp	r6, #9
	beq.n	.L_080df4b4
	cmp	r6, #10
	bne.n	.L_080df4d4
.L_080df4b4:
	ldr	r0, [sp, #124]
	movs	r3, #3
	ands	r3, r0
	ldr	r2, [pc, #484]
	lsls	r3, r3, #1
	ldrh	r1, [r2, r3]
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	movs	r0, #8
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	add	r1, fp
	subs	r2, #4
	subs	r3, #4
	ldr	r0, [sp, #132]
	b.n	.L_080df55a
.L_080df4d4:
	mov	r6, r8
	cmp	r6, #2
	bne.n	.L_080df4fc
	movs	r1, #6
	ldr	r0, [sp, #124]
	bl	sub_080022fc
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	adds	r1, r0, #0
	movs	r0, #8
	lsls	r1, r1, #7
	str	r0, [sp, #0]
	movs	r0, #16
	str	r0, [sp, #4]
	add	r1, fp
	subs	r2, #4
	subs	r3, #8
	ldr	r0, [sp, #132]
	b.n	.L_080df534
.L_080df4fc:
	mov	r0, r8
	cmp	r0, #3
	bne.n	.L_080df51a
	movs	r1, #18
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	str	r1, [sp, #0]
	movs	r1, #13
	str	r1, [sp, #4]
	mov	r1, fp
	subs	r2, #9
	subs	r3, #7
	ldr	r0, [sp, #132]
	adds	r1, #96
	b.n	.L_080df55a
.L_080df51a:
	mov	r6, r8
	cmp	r6, #4
	bne.n	.L_080df53c
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	movs	r1, #12
	str	r1, [sp, #0]
	movs	r1, #8
	str	r1, [sp, #4]
	subs	r2, #6
	subs	r3, #4
	ldr	r0, [sp, #132]
	mov	r1, fp
.L_080df534:
	ldr	r7, [sp, #112]
	bl	sub_08007300
	b.n	.L_080df57c
.L_080df53c:
	mov	r0, r8
	cmp	r0, #11
	bne.n	.L_080df562
	movs	r1, #29
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	str	r1, [sp, #0]
	movs	r1, #23
	str	r1, [sp, #4]
	movs	r1, #165
	lsls	r1, r1, #1
	subs	r2, #15
	subs	r3, #12
	ldr	r0, [sp, #132]
	add	r1, fp
.L_080df55a:
	ldr	r4, [sp, #112]
	bl	sub_080072f4
	b.n	.L_080df57c
.L_080df562:
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	movs	r1, #40
	str	r1, [sp, #0]
	movs	r1, #64
	str	r1, [sp, #4]
	subs	r2, #20
	subs	r3, #32
	ldr	r0, [sp, #132]
	mov	r1, fp
	ldr	r6, [sp, #112]
	bl	sub_080072fc
.L_080df57c:
	ldr	r7, [sp, #36]
	ldr	r1, [sp, #124]
	ldr	r2, [sp, #40]
	ldr	r3, [sp, #60]
	ldr	r0, [sp, #80]
	ldr	r4, [sp, #88]
	adds	r7, r7, r0
	adds	r1, #1
	adds	r2, #28
	adds	r3, #1
	str	r7, [sp, #36]
	str	r1, [sp, #124]
	str	r2, [sp, #40]
	str	r3, [sp, #60]
	cmp	r1, r4
	beq.n	.L_080df59e
	b.n	.L_080df142
.L_080df59e:
	ldr	r6, [sp, #32]
	ldr	r7, [sp, #28]
	ldr	r1, [sp, #128]
	ldr	r0, [sp, #76]
	adds	r6, #2
	adds	r7, r7, r0
	adds	r1, #1
	str	r6, [sp, #32]
	str	r7, [sp, #28]
	str	r1, [sp, #128]
	ldr	r2, [pc, #224]
	mov	r4, fp
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	cmp	r1, r3
	beq.n	.L_080df5c0
	b.n	.L_080df112
.L_080df5c0:
	mov	r6, r8
	cmp	r6, #0
	beq.n	.L_080df5d0
	cmp	r6, #5
	beq.n	.L_080df5d0
	cmp	r6, #8
	beq.n	.L_080df5d0
	b.n	.L_080df772
.L_080df5d0:
	movs	r7, #0
	ldr	r3, [pc, #192]
	str	r7, [sp, #128]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r0, [sp, #88]
	ldr	r3, [r3, #20]
	muls	r3, r0
	cmp	r3, #0
	bne.n	.L_080df5e6
	b.n	.L_080df772
.L_080df5e6:
	ldr	r1, [pc, #188]
	ldr	r2, [pc, #188]
	ldr	r7, [pc, #192]
	mov	r9, r1
	mov	sl, r2
.L_080df5f0:
	ldr	r1, [r7, #24]
	cmp	r1, #1
	beq.n	.L_080df5f8
	b.n	.L_080df758
.L_080df5f8:
	ldr	r3, [r7, #8]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r6, r3, #1
	ldr	r3, [sp, #128]
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_080df6bc
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	bl	sub_080ed408
	lsls	r5, r6, #1
	mov	r4, r9
	mov	r0, sl
	ldrh	r1, [r4, r5]
	ldr	r2, [r7, #0]
	ldrb	r4, [r0, r6]
	ldr	r3, [pc, #136]
	subs	r2, r2, r4
	ldrb	r0, [r3, r6]
	ldr	r3, [r7, #4]
	str	r4, [sp, #0]
	ldr	r4, [pc, #132]
	subs	r3, r3, r0
	ldrb	r0, [r4, r6]
	str	r0, [sp, #4]
	ldr	r0, [pc, #100]
	add	r1, fp
	ldr	r4, [r0, #0]
	ldr	r0, [sp, #132]
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	movs	r1, #2
	str	r1, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	movs	r3, #15
	movs	r0, #47
	bl	sub_080ed408
	ldr	r3, [pc, #92]
	ldr	r4, [pc, #84]
	ldrb	r0, [r3, r6]
	ldr	r3, [r7, #4]
	adds	r3, r3, r0
	ldrb	r0, [r4, r6]
	mov	r4, sl
	subs	r3, r3, r0
	ldrb	r0, [r4, r6]
	mov	r2, r9
	ldrh	r1, [r2, r5]
	ldr	r4, [pc, #68]
	ldr	r2, [r7, #0]
	str	r0, [sp, #0]
	ldrb	r0, [r4, r6]
	ldr	r6, [pc, #36]
	str	r0, [sp, #4]
	add	r1, fp
	ldr	r4, [r6, #0]
	ldr	r0, [sp, #132]
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	b.n	.L_080df74a
	.4byte 0x02010af0
	.4byte 0x080eebec
	.4byte 0x00007828
	.4byte 0x080eebe9
	.4byte 0x03001f0c
	.4byte 0x080eec52
	.4byte 0x080eec44
	.4byte 0x080eec28
	.4byte 0x02010000
	.4byte 0x080eec3d
	.4byte 0x080eec2f
	.2byte 0xec36
	.2byte 0x080e
.L_080df6bc:
	movs	r0, #2
	str	r0, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	bl	sub_080ed408
	lsls	r5, r6, #1
	mov	r2, r9
	ldrh	r1, [r2, r5]
	ldr	r4, [pc, #436]
	ldr	r3, [pc, #436]
	add	r1, fp
	adds	r1, r1, r3
	ldr	r2, [r7, #0]
	ldrb	r3, [r4, r6]
	mov	r0, sl
	ldrb	r4, [r0, r6]
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	subs	r3, r3, r4
	mov	ip, r3
	ldr	r3, [pc, #420]
	ldrb	r0, [r3, r6]
	str	r4, [sp, #4]
	str	r0, [sp, #0]
	ldr	r0, [pc, #416]
	mov	r3, ip
	ldr	r4, [r0, #0]
	ldr	r0, [sp, #132]
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	movs	r1, #2
	str	r1, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	movs	r3, #15
	movs	r0, #47
	bl	sub_080ed408
	mov	r2, r9
	ldrh	r1, [r2, r5]
	ldr	r3, [pc, #368]
	add	r1, fp
	adds	r1, r1, r3
	ldr	r3, [pc, #376]
	ldr	r2, [r7, #0]
	ldrb	r3, [r3, r6]
	ldr	r4, [pc, #352]
	adds	r2, r2, r3
	ldrb	r3, [r4, r6]
	ldr	r4, [pc, #356]
	ldrb	r0, [r4, r6]
	subs	r2, r2, r3
	ldr	r3, [r7, #4]
	str	r0, [sp, #0]
	mov	r4, sl
	ldrb	r0, [r4, r6]
	ldr	r6, [pc, #344]
	str	r0, [sp, #4]
	ldr	r0, [sp, #132]
	ldr	r4, [r6, #0]
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
.L_080df74a:
	ldr	r3, [r7, #8]
	adds	r3, #1
	str	r3, [r7, #8]
	cmp	r3, #12
	bne.n	.L_080df758
	movs	r3, #0
	str	r3, [r7, #24]
.L_080df758:
	ldr	r0, [sp, #128]
	ldr	r3, [pc, #320]
	adds	r0, #1
	str	r0, [sp, #128]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #88]
	ldr	r3, [r3, #20]
	muls	r3, r1
	adds	r7, #28
	cmp	r0, r3
	beq.n	.L_080df772
	b.n	.L_080df5f0
.L_080df772:
	movs	r2, #100
	str	r2, [sp, #128]
	ldr	r5, [pc, #296]
.L_080df778:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	ble.n	.L_080df7d4
	asrs	r0, r0, #3
	adds	r0, #1
	lsls	r4, r0, #1
	ldr	r2, [pc, #284]
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #104]
	adds	r1, r3, r1
	lsrs	r3, r0, #31
	movs	r6, #2
	ldrsh	r2, [r5, r6]
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #132]
	ldr	r4, [sp, #112]
	bl	sub_080072f4
	movs	r2, #128
	adds	r0, r5, #0
	movs	r1, #60
	lsls	r2, r2, #5
	bl	sub_080e3908
	movs	r6, #224
	ldr	r3, [r5, #4]
	lsls	r6, r6, #15
	cmp	r3, r6
	ble.n	.L_080df7ce
	ldr	r3, [r5, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #16]
.L_080df7ce:
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_080df7d4:
	ldr	r7, [sp, #128]
	movs	r0, #128
	adds	r7, #1
	lsls	r0, r0, #2
	adds	r5, #28
	str	r7, [sp, #128]
	cmp	r7, r0
	bne.n	.L_080df778
	ldr	r6, [pc, #188]
	ldr	r5, [pc, #192]
.L_080df7e8:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	ble.n	.L_080df82a
	asrs	r0, r0, #4
	adds	r0, #1
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r2, [sp, #104]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #132]
	ldr	r4, [sp, #112]
	bl	sub_080072f4
	adds	r0, r5, #0
	movs	r1, #60
	ldr	r2, [pc, #140]
	bl	sub_080e3908
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_080df82a:
	ldr	r7, [sp, #128]
	movs	r0, #128
	adds	r7, #1
	lsls	r0, r0, #3
	adds	r5, #28
	str	r7, [sp, #128]
	cmp	r7, r0
	bne.n	.L_080df7e8
	movs	r1, #4
	movs	r0, #4
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #104]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [sp, #120]
	ldr	r2, [sp, #96]
	adds	r1, #1
	str	r1, [sp, #120]
	cmp	r1, r2
	beq.n	.L_080df864
	bl	.L_080defea
.L_080df864:
	ldr	r0, [pc, #76]
	bl	sub_08004278
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #156
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.2byte 0xec3d
	.2byte 0x080e
	asrs	r2, r1, #10
	movs	r0, r0
	.2byte 0xec2f
	.2byte 0x080e
	subs	r4, r1, #4
	lsls	r0, r0, #12
	.2byte 0xec36
	.2byte 0x080e
	ldrb	r0, [r5, #0]
	movs	r0, r0
	lsrs	r0, r6, #11
	lsls	r1, r0, #8
	udf	#72
	lsrs	r6, r1, #32
	subs	r0, #0
	lsls	r1, r0, #8
	.2byte 0xc000
	.2byte 0xffff
	.2byte 0x7824
	movs	r0, r0
	.4byte 0x080cd261

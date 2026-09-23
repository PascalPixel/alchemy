.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_08002304, 0x08002304
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_08004a28, 0x08004a28
	.set sub_08004a44, 0x08004a44
	.set sub_08004bd4, 0x08004bd4
	.set sub_08004c1c, 0x08004c1c
	.set sub_08004c6c, 0x08004c6c
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_08009008, 0x08009008
	.set sub_08009038, 0x08009038
	.set sub_080b5040, 0x080b5040
	.set sub_080b5088, 0x080b5088
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
	.set sub_080e3944, 0x080e3944
	.set sub_080e396c, 0x080e396c
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global BattleEffect_RunStagedParticles
	.global Func_080dc968
	.thumb_func
BattleEffect_RunStagedParticles:
Func_080dc968:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r7, [pc, #120]
	ldr	r1, [r7, #0]
	sub	sp, #120
	str	r1, [sp, #56]
	adds	r3, r7, #0
	subs	r3, #16
	ldr	r3, [r3, #0]
	str	r3, [sp, #52]
	adds	r3, r7, #0
	subs	r3, #20
	ldr	r3, [r3, #0]
	mov	fp, r3
	adds	r3, r7, #0
	subs	r3, #12
	ldr	r3, [r3, #0]
	str	r3, [sp, #36]
	ldr	r3, [pc, #92]
	ldr	r5, [pc, #92]
	ldrh	r3, [r3, #4]
	add	r5, fp
	str	r3, [sp, #32]
	str	r0, [r5, #0]
	movs	r0, #128
	lsls	r0, r0, #6
	bl	sub_080cd594
	ldr	r2, [pc, #80]
	ldr	r3, [pc, #56]
	mov	r9, r2
	mov	r0, r9
	strh	r3, [r0, #0]
	bl	sub_080c9048
	movs	r1, #160
	lsls	r1, r1, #19
	ldr	r2, [pc, #44]
	mov	r8, r1
	mov	r3, r8
	strh	r2, [r3, #0]
	ldr	r3, [pc, #56]
	strh	r2, [r3, #0]
	movs	r3, #239
	lsls	r3, r3, #7
	add	r3, fp
	movs	r6, #0
	movs	r1, #144
	str	r6, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #44]
	mov	sl, r1
	bl	sub_080041d8
	movs	r1, #0
	movs	r0, #0
	bl	sub_080cd104
	b.n	.L_080dca08
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x03001f00
	.4byte 0x03001ad0
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x05000002
	.2byte 0xd261
	.2byte 0x080c
.L_080dca08:
	ldr	r0, [r5, #0]
	bl	sub_080d6750
	movs	r1, #185
	lsls	r1, r1, #1
	movs	r0, #9
	movs	r2, #1
	bl	sub_080dbb24
	mov	r1, fp
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #220]
	bl	sub_080e0524
	ldr	r0, [pc, #220]
	bl	sub_08002f40
	ldr	r3, [pc, #216]
	adds	r1, r0, #0
	movs	r2, #128
	mov	r0, r8
	bl	sub_080072f0
	movs	r2, #0
	ldr	r1, [sp, #36]
	movs	r3, #0
	ldr	r0, [pc, #204]
	bl	sub_080e0524
	ldr	r0, [pc, #200]
	bl	sub_08002f40
	movs	r3, #2
	str	r0, [sp, #48]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	sub_080ed408
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	movs	r0, #47
	str	r3, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [r7, #8]
	str	r3, [sp, #60]
	ldr	r3, [r7, #12]
	mov	r2, sp
	adds	r2, #60
	str	r2, [sp, #16]
	str	r3, [r2, #4]
	ldr	r2, [pc, #152]
	movs	r3, #240
	str	r3, [r2, #16]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [pc, #144]
	movs	r0, #1
	movs	r2, #0
	bl	sub_080b5040
	ldr	r3, [pc, #140]
	ldr	r2, [pc, #140]
	add	r3, fp
	str	r6, [r3, #0]
	add	r2, fp
	movs	r3, #4
	str	r3, [r2, #0]
	ldr	r2, [pc, #132]
	subs	r3, #5
	add	r2, fp
	str	r3, [r2, #0]
	ldr	r3, [pc, #128]
	add	r3, fp
	str	r6, [r3, #0]
	mov	r1, sl
	ldr	r0, [pc, #124]
	bl	sub_080041d8
	ldr	r0, [sp, #56]
	movs	r3, #1
	str	r3, [r0, #16]
	movs	r1, #1
	movs	r0, #0
	bl	sub_080cd104
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	mov	r1, r9
	strh	r3, [r1, #0]
	ldr	r3, [pc, #40]
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	subs	r2, #2
	strh	r3, [r2, #0]
	movs	r7, #225
	movs	r2, #0
	ldr	r3, [pc, #80]
	lsls	r7, r7, #7
	str	r2, [sp, #28]
	str	r2, [sp, #24]
	str	r2, [sp, #44]
	mov	r8, r3
	add	r7, fp
	b.n	.L_080dcb34
	movs	r0, r0
	.4byte 0x00007741
	.4byte 0x00000080
	.4byte 0x00001010
	.4byte 0x00003f44
	.4byte 0x0000006a
	.4byte 0x000000a0
	.4byte 0x03001388
	.4byte 0x00000073
	.4byte 0x000000d2
	.4byte 0x03001ce0
	.4byte 0x0000003b
	.4byte 0x00007790
	.4byte 0x00007794
	.4byte 0x00007798
	.4byte 0x0000779c
	.4byte 0x080c90e5
	.2byte 0xffff
	.2byte 0x0000
.L_080dcb34:
	bl	sub_08004458
	movs	r1, #96
	bl	sub_08002304
	adds	r0, #12
	lsls	r0, r0, #16
	str	r0, [r7, #0]
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	movs	r3, #0
	str	r3, [r7, #12]
	str	r3, [r7, #16]
	str	r3, [r7, #24]
	ldr	r0, [sp, #44]
	lsls	r2, r0, #1
	adds	r2, r2, r0
	mov	r9, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	ldr	r1, [pc, #152]
	lsls	r3, r3, #7
	adds	r6, r3, r1
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r2, [pc, #144]
	lsls	r3, r3, #5
	adds	r5, r3, r2
.L_080dcb76:
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r3, #48
	str	r3, [r5, #0]
	bl	sub_080049ac
	bl	sub_08004458
	mov	r3, r8
	ands	r0, r3
	bl	sub_08004c6c
	bl	sub_08004458
	mov	r1, r8
	ands	r0, r1
	bl	sub_08004bd4
	bl	sub_08004458
	mov	r2, r8
	ands	r0, r2
	bl	sub_08004c1c
	adds	r0, r6, #0
	bl	sub_08004a28
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	adds	r5, #28
	adds	r6, #48
	cmp	r0, #24
	bne.n	.L_080dcb76
	ldr	r1, [sp, #44]
	adds	r1, #1
	adds	r7, #28
	str	r1, [sp, #44]
	cmp	r1, #16
	bne.n	.L_080dcb34
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #48]
	movs	r3, #50
	add	r2, fp
	str	r3, [r2, #0]
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #28]
	strh	r3, [r2, #0]
	movs	r2, #0
	str	r2, [sp, #40]
	ldr	r3, [pc, #40]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080dcbf4
	b.n	.L_080dcfba
.L_080dcbf4:
	mov	r3, sp
	adds	r3, #108
	str	r3, [sp, #12]
	b.n	.L_080dcc14
	.4byte 0x00000784
	.4byte 0x02013800
	.4byte 0x02010000
	.4byte 0x00007784
	.4byte 0x0400000c
	.2byte 0x1b04
	.2byte 0x0300
.L_080dcc14:
	ldr	r0, [sp, #40]
	cmp	r0, #209
	bgt.n	.L_080dccc2
	cmp	r0, #0
	bne.n	.L_080dcc3e
	ldr	r1, [sp, #48]
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
	adds	r1, #4
	str	r3, [sp, #24]
	str	r1, [sp, #48]
	b.n	.L_080dcc58
.L_080dcc3e:
	ldr	r2, [sp, #48]
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
	str	r2, [sp, #48]
.L_080dcc58:
	add	r2, sp, #80
	movs	r3, #0
	str	r3, [r2, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r2, #4]
	ldr	r0, [sp, #24]
	movs	r3, #0
	str	r3, [sp, #20]
	mov	r9, r3
	movs	r3, #128
	mov	sl, r2
	lsls	r3, r3, #15
	lsls	r2, r0, #16
	subs	r7, r3, r2
.L_080dcc76:
	ldr	r1, [sp, #28]
	movs	r2, #160
	lsls	r3, r1, #16
	lsls	r2, r2, #15
	mov	r0, r9
	adds	r5, r3, r2
	ldr	r1, [pc, #580]
	lsls	r3, r0, #2
	add	r3, fp
	movs	r4, #0
	mov	r8, r7
	adds	r6, r3, r1
.L_080dcc8e:
	mov	r2, sl
	mov	r3, r8
	str	r5, [r2, #0]
	str	r3, [r2, #8]
	ldmia	r6!, {r0}
	mov	r1, sl
	ldr	r2, [pc, #560]
	movs	r3, #0
	str	r4, [sp, #8]
	bl	sub_08009008
	ldr	r4, [sp, #8]
	movs	r0, #128
	lsls	r0, r0, #14
	adds	r4, #1
	adds	r5, r5, r0
	cmp	r4, #3
	bne.n	.L_080dcc8e
	ldr	r2, [sp, #20]
	movs	r1, #3
	adds	r2, #1
	add	r9, r1
	adds	r7, r7, r0
	str	r2, [sp, #20]
	cmp	r2, #3
	bne.n	.L_080dcc76
.L_080dccc2:
	ldr	r3, [sp, #12]
	movs	r1, #0
	str	r1, [r3, #4]
	str	r1, [r3, #8]
	ldr	r0, [sp, #40]
	cmp	r0, #48
	bne.n	.L_080dccde
	ldr	r3, [pc, #508]
	movs	r2, #24
	add	r3, fp
	str	r2, [r3, #0]
	ldr	r3, [pc, #504]
	add	r3, fp
	str	r1, [r3, #0]
.L_080dccde:
	movs	r1, #0
	str	r1, [sp, #44]
.L_080dcce2:
	ldr	r2, [sp, #44]
	lsls	r6, r2, #3
	adds	r7, r6, #0
	ldr	r3, [sp, #40]
	adds	r7, #64
	cmp	r3, r7
	bge.n	.L_080dccf2
	b.n	.L_080dcf88
.L_080dccf2:
	subs	r3, r6, r2
	lsls	r3, r3, #2
	movs	r0, #225
	add	r3, fp
	lsls	r0, r0, #7
	adds	r5, r3, r0
	movs	r2, #2
	ldrsh	r1, [r5, r2]
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	mov	r8, r1
	mov	sl, r3
	ldr	r1, [sp, #40]
	adds	r3, r6, #0
	adds	r3, #84
	cmp	r1, r3
	bne.n	.L_080dcd1a
	movs	r0, #212
	bl	sub_080f9010
.L_080dcd1a:
	adds	r3, r6, #0
	ldr	r2, [sp, #40]
	adds	r3, #85
	cmp	r2, r3
	blt.n	.L_080dcd8e
	ldr	r1, [r5, #12]
	ldr	r3, [r5, #0]
	adds	r3, r3, r1
	ldr	r2, [r5, #16]
	str	r3, [r5, #0]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [pc, #416]
	movs	r0, #128
	adds	r1, r1, r3
	lsls	r0, r0, #10
	str	r1, [r5, #12]
	adds	r2, r2, r0
	ldr	r1, [pc, #408]
	str	r2, [r5, #16]
	movs	r0, #16
	movs	r5, #21
	mov	r2, r8
	mov	r3, sl
	str	r0, [sp, #0]
	ldr	r4, [sp, #60]
	add	r1, fp
	adds	r2, #4
	subs	r3, #40
	str	r5, [sp, #4]
	ldr	r0, [sp, #52]
	bl	sub_080072f4
	movs	r0, #29
	ldr	r1, [pc, #380]
	str	r0, [sp, #0]
	mov	r2, r8
	movs	r0, #35
	mov	r3, sl
	add	r1, fp
	subs	r2, #16
	subs	r3, #19
	str	r0, [sp, #4]
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	bl	sub_080072f4
	ldr	r1, [pc, #360]
	movs	r0, #24
	mov	r2, r8
	mov	r3, sl
	str	r0, [sp, #4]
	add	r1, fp
	subs	r2, #20
	adds	r3, #16
	str	r5, [sp, #0]
	b.n	.L_080dcebc
.L_080dcd8e:
	adds	r3, r6, #0
	ldr	r1, [sp, #40]
	adds	r3, #80
	cmp	r1, r3
	bge.n	.L_080dcd9a
	b.n	.L_080dcf08
.L_080dcd9a:
	subs	r3, r1, r7
	subs	r3, #16
	cmp	r3, #4
	bls.n	.L_080dcda4
	b.n	.L_080dcf88
.L_080dcda4:
	ldr	r2, [pc, #320]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080dcdc0
	.4byte 0x080dcddc
	.4byte 0x080dcdf4
	.4byte 0x080dce26
	.2byte 0xce74
	.2byte 0x080d
	movs	r1, #14
	mov	r2, r8
	mov	r3, sl
	str	r1, [sp, #0]
	movs	r1, #28
	str	r1, [sp, #4]
	subs	r2, #7
	subs	r3, #14
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	mov	r1, fp
	bl	sub_080072f4
	b.n	.L_080dcf88
	movs	r0, #23
	movs	r1, #196
	lsls	r1, r1, #1
	mov	r2, r8
	mov	r3, sl
	str	r0, [sp, #0]
	movs	r0, #44
	str	r0, [sp, #4]
	add	r1, fp
	subs	r2, #11
	subs	r3, #22
	b.n	.L_080dcebc
	movs	r0, #20
	ldr	r1, [pc, #244]
	str	r0, [sp, #0]
	mov	r2, r8
	movs	r0, #30
	mov	r3, sl
	add	r1, fp
	subs	r2, #4
	subs	r3, #31
	str	r0, [sp, #4]
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	bl	sub_080072f4
	movs	r0, #22
	ldr	r1, [pc, #220]
	mov	r2, r8
	mov	r3, sl
	str	r0, [sp, #0]
	movs	r0, #33
	str	r0, [sp, #4]
	add	r1, fp
	subs	r2, #16
	subs	r3, #1
	b.n	.L_080dcebc
	movs	r0, #18
	ldr	r1, [pc, #200]
	str	r0, [sp, #0]
	mov	r2, r8
	movs	r0, #27
	mov	r3, sl
	str	r0, [sp, #4]
	ldr	r4, [sp, #60]
	add	r1, fp
	adds	r2, #1
	subs	r3, #38
	ldr	r0, [sp, #52]
	bl	sub_080072f4
	movs	r1, #201
	movs	r0, #22
	lsls	r1, r1, #4
	mov	r2, r8
	mov	r3, sl
	add	r1, fp
	subs	r2, #11
	subs	r3, #11
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	bl	sub_080072f4
	movs	r0, #19
	ldr	r1, [pc, #148]
	mov	r2, r8
	mov	r3, sl
	str	r0, [sp, #0]
	movs	r0, #28
	str	r0, [sp, #4]
	add	r1, fp
	subs	r2, #19
	adds	r3, #11
	b.n	.L_080dcebc
	ldr	r1, [pc, #132]
	movs	r5, #23
	movs	r0, #16
	mov	r2, r8
	mov	r3, sl
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	ldr	r4, [sp, #60]
	add	r1, fp
	adds	r2, #4
	subs	r3, #40
	ldr	r0, [sp, #52]
	bl	sub_080072f4
	ldr	r1, [pc, #108]
	str	r5, [sp, #0]
	mov	r2, r8
	movs	r5, #28
	mov	r3, sl
	add	r1, fp
	subs	r2, #10
	subs	r3, #17
	ldr	r4, [sp, #60]
	str	r5, [sp, #4]
	ldr	r0, [sp, #52]
	bl	sub_080072f4
	ldr	r1, [pc, #88]
	movs	r0, #20
	mov	r2, r8
	mov	r3, sl
	str	r0, [sp, #0]
	add	r1, fp
	subs	r2, #20
	adds	r3, #11
	str	r5, [sp, #4]
.L_080dcebc:
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	bl	sub_080072f4
	b.n	.L_080dcf88
	movs	r0, r0
	.4byte 0x000077d8
	.4byte 0x080eeb40
	.4byte 0x000077b4
	.4byte 0x000077b8
	.4byte 0xffff0000
	.4byte 0x000016ac
	.4byte 0x000017fc
	.4byte 0x00001bf3
	.4byte 0x080dcdac
	.4byte 0x0000057c
	.4byte 0x000007d4
	.4byte 0x00000aaa
	.4byte 0x00000e74
	.4byte 0x00001088
	.4byte 0x000011f8
	.2byte 0x147c
	.2byte 0x0000
.L_080dcf08:
	ldr	r3, [sp, #44]
	movs	r2, #0
	mov	r9, r2
	lsls	r2, r3, #1
	adds	r3, r2, r3
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r0, [pc, #276]
	lsls	r2, r2, #7
	adds	r7, r2, r0
	lsls	r2, r3, #3
	subs	r2, r2, r3
	ldr	r1, [pc, #268]
	lsls	r2, r2, #5
	add	r6, sp, #96
	adds	r5, r2, r1
.L_080dcf28:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	ble.n	.L_080dcf7a
	adds	r0, r7, #0
	bl	sub_08004a44
	ldr	r3, [r5, #0]
	ldr	r2, [sp, #12]
	str	r3, [r2, #0]
	adds	r1, r6, #0
	ldr	r0, [sp, #12]
	bl	sub_080e3944
	ldr	r3, [r6, #0]
	asrs	r3, r3, #1
	add	r3, r8
	str	r3, [r6, #0]
	ldr	r3, [r6, #4]
	add	r3, sl
	adds	r3, #16
	str	r3, [r6, #4]
	ldr	r3, [r5, #0]
	subs	r3, #4
	str	r3, [r5, #0]
	ldr	r3, [pc, #216]
	ldrh	r1, [r3, #8]
	ldr	r3, [sp, #36]
	movs	r0, #5
	ldr	r2, [r6, #0]
	adds	r1, r3, r1
	ldr	r3, [r6, #4]
	str	r0, [sp, #0]
	movs	r0, #10
	str	r0, [sp, #4]
	ldr	r0, [sp, #16]
	subs	r2, #2
	ldr	r4, [r0, #4]
	subs	r3, #5
	ldr	r0, [sp, #52]
	bl	sub_080072f4
.L_080dcf7a:
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	adds	r7, #48
	adds	r5, #28
	cmp	r2, #24
	bne.n	.L_080dcf28
.L_080dcf88:
	ldr	r3, [sp, #44]
	adds	r3, #1
	str	r3, [sp, #44]
	cmp	r3, #16
	beq.n	.L_080dcf94
	b.n	.L_080dcce2
.L_080dcf94:
	ldr	r2, [pc, #160]
	movs	r3, #1
	add	r2, fp
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_080030f8
	ldr	r0, [sp, #40]
	adds	r0, #1
	str	r0, [sp, #40]
	cmp	r0, #220
	beq.n	.L_080dcfba
	ldr	r3, [pc, #140]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080dcfba
	b.n	.L_080dcc14
.L_080dcfba:
	ldr	r0, [pc, #132]
	bl	sub_08004278
	ldr	r1, [sp, #56]
	movs	r3, #0
	add	r2, sp, #32
	str	r3, [r1, #16]
	ldrh	r2, [r2, #0]
	ldr	r3, [pc, #120]
	strh	r2, [r3, #4]
	bl	sub_080d67dc
	ldr	r5, [pc, #116]
	movs	r3, #0
	str	r3, [sp, #44]
	add	r5, fp
.L_080dcfda:
	ldmia	r5!, {r0}
	bl	sub_08009038
	ldr	r0, [sp, #44]
	adds	r0, #1
	str	r0, [sp, #44]
	cmp	r0, #9
	bne.n	.L_080dcfda
	ldr	r2, [pc, #96]
	ldr	r3, [pc, #52]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	subs	r2, #32
	strh	r3, [r2, #0]
	ldr	r1, [pc, #56]
	ldr	r0, [pc, #84]
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	movs	r5, #225
	movs	r1, #0
	lsls	r5, r5, #7
	str	r1, [sp, #44]
	add	r6, sp, #68
	add	r5, fp
	movs	r7, #31
.L_080dd010:
	ldr	r0, [sp, #44]
	movs	r1, #6
	bl	sub_080022fc
	ldr	r3, [pc, #56]
	add	r3, fp
	ldr	r2, [r3, #0]
	ldr	r3, [r2, #20]
	b.n	.L_080dd058
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x00007741
	.4byte 0x02013800
	.4byte 0x02010000
	.4byte 0x080ede48
	.4byte 0x00007824
	.4byte 0x03001b04
	.4byte 0x080c90e5
	.4byte 0x03001ad0
	.4byte 0x000077d8
	.4byte 0x04000020
	.4byte 0x000000b4
	.2byte 0x7828
	.2byte 0x0000
.L_080dd058:
	cmp	r0, r3
	bge.n	.L_080dd08c
	lsls	r3, r0, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	adds	r1, r6, #0
	bl	sub_080e396c
	bl	sub_08004458
	ands	r0, r7
	adds	r0, #40
	negs	r0, r0
	str	r0, [r5, #4]
	ldr	r1, [r6, #0]
	lsrs	r3, r1, #31
	adds	r1, r1, r3
	movs	r3, #80
	subs	r3, r3, r0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r1, r1, #1
	asrs	r3, r3, #1
	adds	r1, r1, r3
	str	r1, [r5, #0]
	b.n	.L_080dd0a4
.L_080dd08c:
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #80
	str	r3, [r5, #0]
	bl	sub_08004458
	ands	r0, r7
	adds	r0, #40
	negs	r0, r0
	str	r0, [r5, #4]
.L_080dd0a4:
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
	ldr	r2, [sp, #44]
	adds	r2, #1
	adds	r5, #28
	str	r2, [sp, #44]
	cmp	r2, #32
	bne.n	.L_080dd010
	movs	r3, #0
	str	r3, [sp, #40]
.L_080dd0ba:
	movs	r0, #0
	movs	r7, #225
	lsls	r7, r7, #7
	str	r0, [sp, #44]
	add	r7, fp
.L_080dd0c4:
	ldr	r1, [sp, #44]
	ldr	r2, [sp, #40]
	lsls	r3, r1, #1
	cmp	r2, r3
	bge.n	.L_080dd0d4
	cmp	r2, #40
	bgt.n	.L_080dd0d4
	b.n	.L_080dd220
.L_080dd0d4:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	blt.n	.L_080dd168
	cmp	r3, #23
	bgt.n	.L_080dd162
	adds	r6, r3, #0
	cmp	r3, #0
	bge.n	.L_080dd0e6
	adds	r6, r3, #3
.L_080dd0e6:
	ldr	r3, [sp, #44]
	asrs	r6, r6, #2
	ldr	r2, [pc, #400]
	movs	r4, #1
	ands	r4, r3
	lsls	r3, r6, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #392]
	ldrb	r5, [r3, r6]
	ldr	r2, [r7, #0]
	lsrs	r3, r5, #1
	ldr	r0, [pc, #388]
	subs	r2, r2, r3
	ldr	r3, [pc, #388]
	adds	r1, r1, r0
	ldrb	r0, [r3, r6]
	ldr	r3, [r7, #4]
	str	r5, [sp, #0]
	adds	r3, r3, r0
	ldr	r0, [pc, #380]
	ldrb	r0, [r0, r6]
	str	r0, [sp, #4]
	ldr	r0, [sp, #16]
	lsls	r4, r4, #2
	subs	r3, #40
	ldr	r4, [r4, r0]
	subs	r2, #8
	ldr	r0, [sp, #52]
	bl	sub_080072f4
	ldr	r3, [r7, #24]
	cmp	r3, #11
	bgt.n	.L_080dd162
	movs	r1, #16
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	str	r1, [sp, #0]
	movs	r1, #21
	str	r1, [sp, #4]
	ldr	r1, [pc, #344]
	ldr	r4, [sp, #60]
	adds	r2, #4
	subs	r3, #40
	ldr	r0, [sp, #52]
	add	r1, fp
	bl	sub_080072f4
	movs	r1, #29
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	str	r1, [sp, #0]
	movs	r1, #35
	str	r1, [sp, #4]
	ldr	r1, [pc, #320]
	subs	r3, #19
	subs	r2, #16
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	add	r1, fp
	bl	sub_080072f4
	ldr	r3, [r7, #24]
.L_080dd162:
	adds	r3, #1
	str	r3, [r7, #24]
	b.n	.L_080dd220
.L_080dd168:
	ldr	r1, [r7, #4]
	movs	r5, #24
	cmp	r1, #56
	ble.n	.L_080dd176
	subs	r3, r5, r1
	adds	r5, r3, #0
	adds	r5, #56
.L_080dd176:
	adds	r3, r1, #0
	movs	r1, #16
	ldr	r2, [r7, #0]
	str	r1, [sp, #0]
	ldr	r1, [pc, #272]
	movs	r6, #21
	adds	r2, #4
	subs	r3, #40
	ldr	r4, [sp, #60]
	add	r1, fp
	str	r6, [sp, #4]
	ldr	r0, [sp, #52]
	bl	sub_080072f4
	movs	r1, #29
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	str	r1, [sp, #0]
	movs	r1, #35
	str	r1, [sp, #4]
	ldr	r1, [pc, #244]
	subs	r2, #16
	subs	r3, #19
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	add	r1, fp
	bl	sub_080072f4
	cmp	r5, #0
	ble.n	.L_080dd1ca
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	ldr	r1, [pc, #224]
	subs	r2, #20
	adds	r3, #16
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	add	r1, fp
	bl	sub_080072f4
.L_080dd1ca:
	ldr	r3, [r7, #0]
	subs	r3, #6
	str	r3, [r7, #0]
	ldr	r3, [r7, #4]
	adds	r3, #12
	str	r3, [r7, #4]
	cmp	r3, #79
	ble.n	.L_080dd220
	ldr	r2, [pc, #192]
	movs	r3, #0
	add	r2, fp
	str	r3, [r7, #24]
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r0, #134
	bl	sub_080f9010
	movs	r1, #6
	ldr	r0, [sp, #44]
	bl	sub_080022fc
	ldr	r6, [pc, #168]
	add	r6, fp
	ldr	r2, [r6, #0]
	ldr	r3, [r2, #20]
	adds	r4, r0, #0
	cmp	r4, r3
	bge.n	.L_080dd220
	lsls	r5, r4, #1
	adds	r5, #36
	movs	r3, #8
	ldrsh	r0, [r2, r5]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	adds	r3, r4, #0
	bl	sub_080d6888
	ldr	r3, [r6, #0]
	movs	r1, #1
	ldrsh	r0, [r3, r5]
	bl	sub_080b5088
.L_080dd220:
	ldr	r3, [sp, #44]
	adds	r3, #1
	adds	r7, #28
	str	r3, [sp, #44]
	cmp	r3, #24
	beq.n	.L_080dd22e
	b.n	.L_080dd0c4
.L_080dd22e:
	movs	r0, #4
	movs	r1, #8
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #104]
	movs	r3, #1
	add	r2, fp
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_080030f8
	ldr	r0, [sp, #40]
	adds	r0, #1
	str	r0, [sp, #40]
	cmp	r0, #88
	beq.n	.L_080dd254
	b.n	.L_080dd0ba
.L_080dd254:
	ldr	r0, [pc, #80]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #120
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x080edeb2
	.4byte 0x080ede9f
	.4byte 0x02010000
	.4byte 0x080edeab
	.4byte 0x080edea5
	.4byte 0x000016ac
	.4byte 0x000017fc
	.4byte 0x00001bf3
	.4byte 0x000077a8
	.4byte 0x00007828
	.4byte 0x00007824
	.4byte 0x080cd261

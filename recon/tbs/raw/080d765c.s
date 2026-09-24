.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_08002304, 0x08002304
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_080072f4, 0x080072f4
	.set sub_080072f8, 0x080072f8
	.set sub_08007300, 0x08007300
	.set sub_08009008, 0x08009008
	.set sub_08009020, 0x08009020
	.set sub_08009038, 0x08009038
	.set sub_080b5040, 0x080b5040
	.set sub_080b50e8, 0x080b50e8
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
	.set sub_080e3908, 0x080e3908
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global BattleFx_InitializeMode10
	.global Func_080d765c
	.thumb_func
BattleFx_InitializeMode10:
Func_080d765c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #112]
	ldr	r1, [r5, #0]
	sub	sp, #332
	str	r1, [sp, #48]
	subs	r3, r5, #4
	ldr	r3, [r3, #0]
	str	r3, [sp, #44]
	ldr	r4, [sp, #44]
	ldr	r7, [pc, #100]
	ldr	r2, [r5, #4]
	adds	r4, r4, r7
	movs	r3, #1
	str	r2, [sp, #28]
	str	r3, [sp, #24]
	str	r0, [r4, #0]
	movs	r0, #0
	mov	r8, r4
	bl	sub_080cd594
	bl	sub_080c9048
	ldr	r2, [pc, #56]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r0, [sp, #44]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r0, r0, r1
	movs	r3, #0
	movs	r1, #144
	str	r3, [r0, #0]
	mov	sl, r0
	lsls	r1, r1, #3
	ldr	r0, [pc, #48]
	bl	sub_080041d8
	ldr	r2, [pc, #44]
	ldr	r3, [pc, #24]
	movs	r0, #1
	strh	r3, [r2, #0]
	movs	r1, #0
	bl	sub_080cd104
	ldr	r2, [pc, #36]
	ldr	r3, [pc, #12]
	strh	r3, [r2, #0]
	b.n	.L_080d76f0
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00002137
	.4byte 0x0000f0f0
	.4byte 0x03001ef0
	.4byte 0x00007828
	.4byte 0x080cd261
	.4byte 0x04000048
	.2byte 0x0040
	.2byte 0x0400
.L_080d76f0:
	ldr	r1, [sp, #44]
	ldr	r0, [pc, #168]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	ldr	r0, [pc, #160]
	ldr	r1, [sp, #28]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	movs	r6, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r6, [sp, #0]
	bl	sub_080ed408
	ldr	r2, [r5, #24]
	ldr	r3, [sp, #24]
	str	r2, [sp, #36]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	bl	sub_080ed408
	ldr	r5, [r5, #28]
	ldr	r2, [pc, #116]
	str	r5, [sp, #32]
	movs	r3, #240
	mov	r4, r8
	str	r3, [r2, #16]
	ldr	r0, [r4, #0]
	bl	sub_080d6750
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [pc, #96]
	movs	r0, #1
	movs	r2, #0
	bl	sub_080b5040
	movs	r1, #186
	lsls	r1, r1, #1
	movs	r0, #9
	movs	r2, #1
	bl	sub_080dbb24
	ldr	r3, [pc, #48]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	subs	r2, #2
	mov	r5, sl
	strh	r3, [r2, #0]
	str	r6, [r5, #0]
	ldr	r7, [sp, #44]
	ldr	r0, [pc, #48]
	movs	r3, #50
	adds	r2, r7, r0
	movs	r1, #0
	str	r3, [r2, #0]
	ldr	r5, [pc, #40]
	mov	r8, r1
	b.n	.L_080d77b4
	movs	r0, r0
	.4byte 0x00007741
	.4byte 0x00000080
	.4byte 0x00001010
	.4byte 0x00003f44
	.4byte 0x000000b9
	.4byte 0x000000ba
	.4byte 0x03001ce0
	.4byte 0x0000003b
	.4byte 0x00007784
	.2byte 0x0000
	.2byte 0x0201
.L_080d77b4:
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	str	r3, [r5, #0]
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r3, #120
	str	r3, [r5, #4]
	movs	r2, #1
	movs	r3, #1
	negs	r3, r3
	add	r8, r2
	str	r3, [r5, #24]
	mov	r3, r8
	adds	r5, #28
	cmp	r3, #64
	bne.n	.L_080d77b4
	movs	r4, #0
	ldr	r1, [pc, #132]
	mov	r8, r4
	add	r2, sp, #76
.L_080d77e6:
	ldrb	r3, [r1, #0]
	movs	r5, #1
	str	r3, [r2, #0]
	add	r8, r5
	ldrb	r3, [r1, #1]
	mov	r7, r8
	str	r3, [r2, #4]
	adds	r1, #2
	adds	r2, #8
	cmp	r7, #32
	bne.n	.L_080d77e6
	movs	r0, #141
	bl	sub_080f9010
	movs	r0, #0
	str	r0, [sp, #40]
	ldr	r3, [pc, #100]
	mov	r2, sp
	adds	r2, #52
	movs	r1, #16
	ldr	r3, [r3, #0]
	str	r2, [sp, #16]
	mov	sl, r1
.L_080d7814:
	ldr	r3, [sp, #40]
	cmp	r3, #15
	bhi.n	.L_080d787c
	ldr	r7, [pc, #84]
	cmp	r3, #1
	bne.n	.L_080d7848
	ldr	r5, [pc, #80]
	adds	r6, r5, #0
	movs	r3, #63
	adds	r6, #128
.L_080d7828:
	str	r3, [sp, #12]
	bl	sub_08004458
	ldr	r3, [sp, #12]
	ands	r0, r3
	strb	r0, [r5, #0]
	adds	r5, #1
	cmp	r5, r6
	bne.n	.L_080d7828
	ldr	r3, [pc, #40]
	movs	r1, #144
	strh	r3, [r7, #0]
	ldr	r0, [pc, #52]
	lsls	r1, r1, #3
	bl	sub_080041d8
.L_080d7848:
	ldr	r4, [sp, #24]
	ldrh	r3, [r7, #0]
	adds	r3, r3, r4
	strh	r3, [r7, #0]
	ldr	r5, [sp, #40]
	adds	r4, #3
	str	r4, [sp, #24]
	cmp	r5, #15
	bne.n	.L_080d787c
	ldr	r0, [pc, #28]
	bl	sub_08004278
	b.n	.L_080d787c
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x080ee974
	.4byte 0x03001b04
	.4byte 0x02010000
	.4byte 0x02010002
	.2byte 0x66cd
	.2byte 0x080d
.L_080d787c:
	ldr	r7, [sp, #40]
	cmp	r7, #103
	ble.n	.L_080d7888
	movs	r0, #0
	mov	sl, r0
	b.n	.L_080d789e
.L_080d7888:
	ldr	r1, [sp, #40]
	cmp	r1, #63
	ble.n	.L_080d7894
	movs	r2, #6
	mov	sl, r2
	b.n	.L_080d789e
.L_080d7894:
	ldr	r3, [sp, #40]
	cmp	r3, #31
	ble.n	.L_080d789e
	movs	r4, #10
	mov	sl, r4
.L_080d789e:
	ldr	r5, [sp, #40]
	cmp	r5, #167
	bgt.n	.L_080d78c4
	bl	sub_08004458
	movs	r5, #3
	ands	r0, r5
	subs	r0, #1
	mov	r9, r0
	bl	sub_08004458
	ldr	r3, [pc, #796]
	ands	r0, r5
	subs	r7, r0, #1
	mov	r1, r9
	adds	r0, #31
	strh	r1, [r3, #4]
	strh	r0, [r3, #6]
	b.n	.L_080d78d2
.L_080d78c4:
	movs	r2, #0
	mov	r9, r2
	ldr	r2, [pc, #776]
	movs	r7, #0
	movs	r3, #32
	strh	r7, [r2, #4]
	strh	r3, [r2, #6]
.L_080d78d2:
	ldr	r2, [sp, #40]
	subs	r2, #176
	cmp	r2, #3
	bhi.n	.L_080d78ee
	ldr	r3, [pc, #764]
	ldrsb	r3, [r3, r2]
	negs	r4, r3
	ldr	r2, [pc, #752]
	mov	r9, r4
	adds	r7, r3, #0
	mov	r5, r9
	adds	r3, #32
	strh	r5, [r2, #4]
	strh	r3, [r2, #6]
.L_080d78ee:
	movs	r0, #0
	mov	r1, sl
	mov	r8, r0
	cmp	r1, #0
	beq.n	.L_080d793a
	ldr	r2, [pc, #736]
	ldr	r6, [pc, #740]
	mov	fp, r2
.L_080d78fe:
	mov	r0, r8
	movs	r1, #3
	bl	sub_080022fc
	mov	r4, fp
	lsls	r3, r0, #1
	ldrh	r1, [r4, r3]
	ldr	r5, [sp, #44]
	ldr	r4, [pc, #724]
	ldrb	r2, [r6, #0]
	adds	r1, r5, r1
	mov	r3, r9
	ldrb	r5, [r4, r0]
	ldr	r4, [pc, #716]
	subs	r2, r2, r3
	ldrb	r3, [r6, #1]
	ldrb	r0, [r4, r0]
	subs	r3, r3, r5
	str	r5, [sp, #4]
	movs	r5, #1
	str	r0, [sp, #0]
	subs	r3, r3, r7
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #36]
	add	r8, r5
	bl	sub_080072f4
	adds	r6, #2
	cmp	r8, sl
	bne.n	.L_080d78fe
.L_080d793a:
	ldr	r3, [pc, #688]
	ldr	r0, [sp, #40]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #52]
	str	r4, [sp, #56]
	cmp	r0, #174
	bne.n	.L_080d796c
	ldr	r2, [sp, #44]
	ldr	r3, [pc, #672]
	movs	r1, #0
	movs	r4, #13
	mov	r8, r1
	negs	r4, r4
	adds	r0, r2, r3
.L_080d7958:
	ldmia	r0!, {r1}
	ldrb	r2, [r1, #9]
	adds	r3, r4, #0
	movs	r5, #1
	ands	r3, r2
	add	r8, r5
	strb	r3, [r1, #9]
	mov	r1, r8
	cmp	r1, #9
	bne.n	.L_080d7958
.L_080d796c:
	ldr	r2, [sp, #40]
	cmp	r2, #208
	ble.n	.L_080d7990
	ldr	r4, [sp, #44]
	ldr	r5, [pc, #636]
	adds	r3, r4, r5
	ldr	r0, [r3, #0]
	ldr	r1, [pc, #636]
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_080d7984
	adds	r3, #3
.L_080d7984:
	asrs	r3, r3, #2
	movs	r2, #3
	ands	r3, r2
	ldrb	r1, [r1, r3]
	bl	sub_08009020
.L_080d7990:
	ldr	r0, [sp, #16]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [sp, #52]
	add	r2, sp, #60
	str	r3, [r0, #4]
	movs	r3, #0
	str	r3, [r2, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r2, #4]
	mov	sl, r2
	lsls	r3, r7, #16
	movs	r2, #152
	negs	r3, r3
	mov	r5, r9
	lsls	r2, r2, #15
	movs	r1, #0
	adds	r4, r3, r2
	lsls	r3, r5, #16
	negs	r3, r3
	str	r1, [sp, #20]
	mov	fp, r1
	mov	r9, r3
.L_080d79c0:
	mov	r0, fp
	ldr	r1, [sp, #44]
	movs	r5, #144
	lsls	r3, r0, #2
	ldr	r2, [pc, #548]
	lsls	r5, r5, #16
	adds	r3, r3, r1
	movs	r7, #0
	mov	r8, r4
	add	r5, r9
	adds	r6, r3, r2
.L_080d79d6:
	mov	r3, sl
	mov	r0, r8
	str	r5, [r3, #0]
	str	r0, [r3, #8]
	mov	r1, sl
	ldmia	r6!, {r0}
	ldr	r2, [sp, #16]
	movs	r3, #0
	str	r4, [sp, #8]
	bl	sub_08009008
	movs	r1, #128
	lsls	r1, r1, #14
	adds	r7, #1
	adds	r5, r5, r1
	ldr	r4, [sp, #8]
	cmp	r7, #3
	bne.n	.L_080d79d6
	ldr	r3, [sp, #20]
	movs	r2, #3
	adds	r3, #1
	add	fp, r2
	adds	r4, r4, r1
	str	r3, [sp, #20]
	cmp	r3, #3
	bne.n	.L_080d79c0
	ldr	r1, [sp, #40]
	subs	r1, #160
	cmp	r1, #157
	bhi.n	.L_080d7a58
	ldr	r3, [sp, #40]
	ldr	r5, [sp, #40]
	subs	r3, #208
	movs	r2, #80
	movs	r4, #8
	cmp	r5, #175
	bgt.n	.L_080d7a2c
	movs	r3, #96
	subs	r2, r3, r1
	lsls	r3, r1, #2
	adds	r4, r3, #0
	subs	r4, #56
	b.n	.L_080d7a40
.L_080d7a2c:
	ldr	r7, [sp, #40]
	cmp	r7, #208
	ble.n	.L_080d7a40
	cmp	r3, #0
	bge.n	.L_080d7a3a
	adds	r3, r7, #0
	subs	r3, #205
.L_080d7a3a:
	asrs	r3, r3, #2
	adds	r4, r3, #0
	adds	r4, #8
.L_080d7a40:
	movs	r3, #24
	str	r3, [sp, #0]
	movs	r3, #48
	str	r3, [sp, #4]
	ldr	r5, [pc, #432]
	ldr	r3, [sp, #44]
	ldr	r0, [sp, #48]
	adds	r1, r3, r5
	ldr	r7, [sp, #32]
	adds	r3, r4, #0
	bl	sub_08007300
.L_080d7a58:
	ldr	r0, [sp, #40]
	cmp	r0, #32
	bne.n	.L_080d7a64
	movs	r0, #134
	bl	sub_080f9010
.L_080d7a64:
	ldr	r1, [sp, #40]
	cmp	r1, #64
	bne.n	.L_080d7a70
	movs	r0, #134
	bl	sub_080f9010
.L_080d7a70:
	ldr	r2, [sp, #40]
	cmp	r2, #104
	bne.n	.L_080d7a7c
	movs	r0, #134
	bl	sub_080f9010
.L_080d7a7c:
	ldr	r3, [sp, #40]
	cmp	r3, #176
	bne.n	.L_080d7a88
	movs	r0, #134
	bl	sub_080f9010
.L_080d7a88:
	ldr	r4, [sp, #40]
	cmp	r4, #226
	bne.n	.L_080d7a94
	movs	r0, #145
	bl	sub_080f9010
.L_080d7a94:
	bl	sub_080049ac
	ldr	r5, [sp, #40]
	cmp	r5, #32
	bne.n	.L_080d7af2
	movs	r7, #0
	ldr	r5, [pc, #348]
	mov	r8, r7
	movs	r6, #127
	movs	r7, #31
.L_080d7aa8:
	bl	sub_08004458
	ands	r0, r7
	adds	r0, #68
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	sub_08004458
	ands	r0, r7
	adds	r0, #8
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #63
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08004458
	negs	r0, r0
	ands	r0, r6
	subs	r0, #64
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	movs	r0, #1
	add	r8, r0
	adds	r3, #32
	mov	r1, r8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r1, #32
	bne.n	.L_080d7aa8
.L_080d7af2:
	ldr	r2, [sp, #40]
	cmp	r2, #64
	bne.n	.L_080d7b50
	movs	r3, #0
	ldr	r5, [pc, #264]
	mov	r8, r3
	movs	r6, #31
.L_080d7b00:
	bl	sub_08004458
	movs	r1, #48
	bl	sub_08002304
	adds	r0, #60
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	sub_08004458
	ands	r0, r6
	adds	r0, #52
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	subs	r3, #63
	lsls	r3, r3, #12
	str	r3, [r5, #12]
	bl	sub_08004458
	negs	r0, r0
	ands	r0, r6
	subs	r0, #32
	lsls	r0, r0, #13
	str	r0, [r5, #16]
	bl	sub_08004458
	movs	r3, #15
	movs	r4, #1
	ands	r3, r0
	add	r8, r4
	adds	r3, #32
	mov	r7, r8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #32
	bne.n	.L_080d7b00
.L_080d7b50:
	ldr	r0, [sp, #40]
	cmp	r0, #104
	bne.n	.L_080d7bac
	movs	r1, #0
	ldr	r5, [pc, #164]
	mov	r8, r1
	movs	r6, #31
.L_080d7b5e:
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #52
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	bl	sub_08004458
	ands	r0, r6
	adds	r0, #72
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	subs	r3, #63
	lsls	r3, r3, #11
	str	r3, [r5, #12]
	bl	sub_08004458
	negs	r0, r0
	ands	r0, r6
	subs	r0, #32
	lsls	r0, r0, #13
	str	r0, [r5, #16]
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	movs	r2, #1
	adds	r3, #32
	add	r8, r2
	str	r3, [r5, #24]
	mov	r3, r8
	adds	r5, #28
	cmp	r3, #32
	bne.n	.L_080d7b5e
.L_080d7bac:
	ldr	r3, [sp, #40]
	subs	r3, #32
	cmp	r3, #175
	bhi.n	.L_080d7c58
	movs	r4, #0
	ldr	r5, [pc, #72]
	mov	r8, r4
.L_080d7bba:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_080d7c4c
	ldr	r7, [sp, #40]
	cmp	r7, #191
	ble.n	.L_080d7c08
	mov	r0, r8
	movs	r1, #7
	bl	sub_080022fc
	adds	r4, r0, #4
	b.n	.L_080d7c0e
	movs	r0, r0
	.4byte 0x03001ad0
	.4byte 0x080ee994
	.4byte 0x080ee998
	.4byte 0x080ee974
	.4byte 0x080ee9a1
	.4byte 0x080ee99e
	.4byte 0x080eda80
	.4byte 0x000077d8
	.4byte 0x000077e0
	.4byte 0x080ee9a4
	.4byte 0x00000c46
	.4byte 0x02010000
	.2byte 0x0380
	.2byte 0x0201
.L_080d7c08:
	movs	r4, #3
	mov	r0, r8
	ands	r4, r0
.L_080d7c0e:
	ldr	r2, [pc, #592]
	ldr	r0, [pc, #592]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldrb	r0, [r0, r4]
	ldr	r2, [sp, #44]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	ldr	r0, [pc, #576]
	ldrb	r0, [r0, r4]
	ldr	r4, [sp, #32]
	str	r0, [sp, #4]
	ldr	r0, [sp, #48]
	bl	sub_080072f4
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #12]
	adds	r2, r2, r3
	str	r2, [r5, #0]
	ldr	r3, [r5, #4]
	ldr	r2, [r5, #16]
	movs	r7, #128
	lsls	r7, r7, #6
	adds	r3, r3, r2
	adds	r2, r2, r7
	str	r3, [r5, #4]
	str	r2, [r5, #16]
.L_080d7c4c:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #64
	bne.n	.L_080d7bba
.L_080d7c58:
	ldr	r2, [sp, #40]
	cmp	r2, #223
	bgt.n	.L_080d7c60
	b.n	.L_080d7dc8
.L_080d7c60:
	cmp	r2, #224
	bne.n	.L_080d7ca4
	movs	r3, #0
	ldr	r5, [pc, #516]
	mov	r8, r3
	movs	r6, #127
.L_080d7c6c:
	movs	r3, #144
	lsls	r3, r3, #15
	str	r3, [r5, #0]
	movs	r3, #224
	lsls	r3, r3, #14
	str	r3, [r5, #4]
	bl	sub_08004458
	ands	r0, r6
	negs	r0, r0
	subs	r0, #64
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r6
	adds	r0, #16
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	sub_08004458
	movs	r4, #1
	add	r8, r4
	mov	r7, r8
	str	r0, [r5, #24]
	adds	r5, #28
	cmp	r7, #128
	bne.n	.L_080d7c6c
.L_080d7ca4:
	movs	r0, #0
	ldr	r5, [pc, #452]
	mov	r8, r0
.L_080d7caa:
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_080d7cb2
	adds	r3, #3
.L_080d7cb2:
	asrs	r3, r3, #2
	ldr	r1, [sp, #40]
	adds	r3, #224
	cmp	r1, r3
	blt.n	.L_080d7d28
	mov	r0, r8
	movs	r1, #3
	bl	sub_080022fc
	movs	r3, #1
	mov	r2, r8
	ands	r3, r2
	adds	r4, r0, #0
	cmp	r3, #0
	bne.n	.L_080d7cf6
	ldr	r2, [pc, #412]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #44]
	adds	r1, r3, r1
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	ldr	r0, [pc, #404]
	ldrb	r0, [r0, r4]
	movs	r7, #2
	ldrsh	r2, [r5, r7]
	str	r0, [sp, #0]
	ldr	r0, [pc, #396]
	ldrb	r0, [r0, r4]
	ldr	r4, [sp, #36]
	str	r0, [sp, #4]
	ldr	r0, [sp, #48]
	bl	sub_080072f4
.L_080d7cf6:
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #12]
	ldr	r1, [r5, #4]
	adds	r2, r2, r3
	ldr	r3, [r5, #16]
	movs	r7, #16
	str	r2, [r5, #0]
	adds	r1, r1, r3
	asrs	r2, r2, #16
	negs	r7, r7
	str	r1, [r5, #4]
	cmp	r2, r7
	blt.n	.L_080d7d16
	asrs	r3, r1, #16
	cmp	r3, #120
	ble.n	.L_080d7d22
.L_080d7d16:
	movs	r3, #144
	lsls	r3, r3, #15
	str	r3, [r5, #0]
	movs	r3, #224
	lsls	r3, r3, #14
	str	r3, [r5, #4]
.L_080d7d22:
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_080d7d28:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #128
	bne.n	.L_080d7caa
	ldr	r2, [sp, #40]
	cmp	r2, #228
	bne.n	.L_080d7d5c
	movs	r3, #0
	ldr	r2, [pc, #316]
	ldr	r1, [pc, #300]
	mov	r8, r3
	movs	r0, #0
.L_080d7d44:
	ldr	r3, [r1, #0]
	str	r3, [r2, #0]
	movs	r4, #1
	ldr	r3, [r1, #4]
	add	r8, r4
	mov	r5, r8
	str	r3, [r2, #4]
	str	r0, [r2, #24]
	adds	r1, #28
	adds	r2, #28
	cmp	r5, #128
	bne.n	.L_080d7d44
.L_080d7d5c:
	movs	r7, #0
	ldr	r5, [pc, #284]
	ldr	r6, [pc, #264]
	mov	r8, r7
.L_080d7d64:
	mov	r3, r8
	ldr	r0, [sp, #40]
	adds	r3, #228
	cmp	r0, r3
	blt.n	.L_080d7dba
	ldr	r0, [r5, #24]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	movs	r1, #9
	asrs	r0, r0, #1
	bl	sub_080022fc
	ldr	r3, [pc, #256]
	ldrb	r4, [r3, r0]
	ldr	r3, [pc, #256]
	lsls	r0, r0, #1
	ldrh	r1, [r3, r0]
	ldr	r2, [sp, #28]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	subs	r2, r2, r0
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #36]
	bl	sub_080072f4
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #18
	bne.n	.L_080d7dba
	ldr	r3, [r6, #0]
	str	r3, [r5, #0]
	ldr	r3, [r6, #4]
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #24]
.L_080d7dba:
	movs	r7, #1
	add	r8, r7
	mov	r0, r8
	adds	r5, #28
	adds	r6, #28
	cmp	r0, #128
	bne.n	.L_080d7d64
.L_080d7dc8:
	ldr	r1, [sp, #44]
	ldr	r3, [pc, #188]
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r4, [sp, #40]
	movs	r5, #144
	adds	r4, #1
	lsls	r5, r5, #1
	str	r4, [sp, #40]
	cmp	r4, r5
	beq.n	.L_080d7dfe
	ldr	r3, [pc, #164]
	ldr	r3, [r3, #0]
	movs	r2, #3
	movs	r7, #16
	ands	r3, r2
	mov	sl, r7
	cmp	r3, #0
	bne.n	.L_080d7df8
	b.n	.L_080d7814
.L_080d7df8:
	cmp	r4, #16
	bgt.n	.L_080d7dfe
	b.n	.L_080d7814
.L_080d7dfe:
	ldr	r1, [sp, #44]
	ldr	r2, [pc, #140]
	movs	r0, #0
	mov	r8, r0
	adds	r5, r1, r2
.L_080d7e08:
	ldmia	r5!, {r0}
	bl	sub_08009038
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	cmp	r4, #9
	bne.n	.L_080d7e08
	bl	sub_080d67dc
	ldr	r3, [pc, #116]
	ldr	r2, [pc, #60]
	ldr	r0, [pc, #116]
	strh	r2, [r3, #0]
	bl	sub_080f9010
	movs	r1, #225
	ldr	r0, [sp, #44]
	movs	r5, #0
	lsls	r1, r1, #7
	mov	r8, r5
	movs	r7, #127
	movs	r6, #63
	adds	r5, r0, r1
.L_080d7e38:
	bl	sub_08004458
	ands	r0, r7
	ldr	r3, [pc, #92]
	adds	r0, #64
	lsls	r0, r0, #16
	str	r3, [r5, #8]
	str	r0, [r5, #0]
	bl	sub_08004458
	ands	r0, r7
	negs	r0, r0
	subs	r0, #64
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08004458
	b.n	.L_080d7ea0
	.4byte 0x00001010
	.4byte 0x080ee9be
	.4byte 0x080ee9a8
	.4byte 0x080ee9b3
	.4byte 0x02010000
	.4byte 0x080ee9da
	.4byte 0x080ee9d4
	.4byte 0x080ee9d7
	.4byte 0x02010e00
	.4byte 0x080ede96
	.4byte 0x080ede84
	.4byte 0x00007824
	.4byte 0x03001b04
	.4byte 0x000077d8
	.4byte 0x04000052
	.4byte 0x00000121
	.2byte 0x0000
	.2byte 0xfff0
.L_080d7ea0:
	.2byte 0x4030
	negs	r0, r0
	subs	r0, #127
	lsls	r0, r0, #12
	str	r0, [r5, #12]
	bl	sub_08004458
	movs	r2, #1
	ands	r0, r6
	movs	r3, #0
	adds	r0, #127
	add	r8, r2
	str	r3, [r5, #24]
	lsls	r0, r0, #12
	mov	r3, r8
	str	r0, [r5, #16]
	adds	r5, #28
	cmp	r3, #64
	bne.n	.L_080d7e38
	movs	r4, #0
	ldr	r5, [pc, #848]
	mov	r8, r4
.L_080d7ecc:
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08004458
	mov	r7, r8
	lsrs	r3, r7, #31
	add	r3, r8
	movs	r2, #63
	ands	r2, r0
	asrs	r3, r3, #1
	adds	r2, r2, r3
	negs	r3, r7
	str	r2, [r5, #4]
	movs	r0, #1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	add	r8, r0
	asrs	r3, r3, #1
	mov	r1, r8
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r1, #64
	bne.n	.L_080d7ecc
	movs	r2, #0
	ldr	r3, [pc, #796]
	str	r2, [sp, #40]
	mov	r9, r3
.L_080d7f08:
	ldr	r5, [sp, #40]
	movs	r4, #0
	mov	sl, r4
	cmp	r5, #96
	bne.n	.L_080d7f18
	movs	r0, #134
	bl	sub_080b50e8
.L_080d7f18:
	ldr	r7, [sp, #40]
	cmp	r7, #16
	bne.n	.L_080d7f28
	ldr	r0, [sp, #44]
	ldr	r1, [pc, #768]
	movs	r3, #32
	adds	r2, r0, r1
	str	r3, [r2, #0]
.L_080d7f28:
	ldr	r2, [sp, #40]
	cmp	r2, #16
	ble.n	.L_080d7f42
	adds	r3, r2, #0
	subs	r3, #16
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	mov	sl, r3
	cmp	r3, #16
	ble.n	.L_080d7f42
	movs	r3, #16
	mov	sl, r3
.L_080d7f42:
	ldr	r3, [sp, #40]
	subs	r3, #9
	cmp	r3, #62
	bhi.n	.L_080d7f5a
	ldr	r4, [sp, #40]
	movs	r3, #3
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L_080d7f5a
	movs	r0, #132
	bl	sub_080f9010
.L_080d7f5a:
	ldr	r5, [sp, #40]
	cmp	r5, #72
	bne.n	.L_080d7f66
	movs	r0, #145
	bl	sub_080f9010
.L_080d7f66:
	ldr	r7, [sp, #40]
	cmp	r7, #64
	ble.n	.L_080d7f8e
	movs	r3, #64
	subs	r3, r3, r7
	lsls	r2, r3, #3
	subs	r2, r2, r3
	movs	r3, #40
	str	r3, [sp, #0]
	movs	r3, #80
	str	r3, [sp, #4]
	ldr	r4, [pc, #680]
	ldr	r3, [sp, #44]
	adds	r2, #88
	adds	r1, r3, r4
	ldr	r0, [sp, #48]
	mov	r3, r9
	ldr	r5, [sp, #36]
	bl	sub_080072f8
.L_080d7f8e:
	ldr	r7, [sp, #40]
	cmp	r7, #71
	bgt.n	.L_080d7fd8
	movs	r0, #0
	mov	r1, sl
	mov	r8, r0
	cmp	r1, #0
	beq.n	.L_080d7fd8
	ldr	r7, [pc, #652]
	ldr	r6, [pc, #652]
.L_080d7fa2:
	mov	r0, r8
	movs	r1, #3
	bl	sub_080022fc
	ldr	r4, [pc, #648]
	lsls	r3, r0, #1
	ldrb	r5, [r4, r0]
	ldrh	r1, [r7, r3]
	ldr	r2, [sp, #44]
	ldr	r4, [pc, #640]
	ldrb	r3, [r6, #1]
	ldrb	r0, [r4, r0]
	adds	r1, r2, r1
	ldrb	r2, [r6, #0]
	subs	r3, r3, r5
	str	r5, [sp, #4]
	movs	r5, #1
	str	r0, [sp, #0]
	subs	r2, #56
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #36]
	add	r8, r5
	bl	sub_080072f4
	adds	r6, #2
	cmp	r8, sl
	bne.n	.L_080d7fa2
.L_080d7fd8:
	ldr	r7, [sp, #40]
	cmp	r7, #72
	bne.n	.L_080d803e
	ldr	r1, [sp, #44]
	movs	r2, #225
	movs	r0, #0
	lsls	r2, r2, #7
	ldr	r6, [pc, #584]
	mov	r8, r0
	adds	r5, r1, r2
.L_080d7fec:
	mov	r3, r8
	movs	r2, #15
	ands	r2, r3
	lsls	r2, r2, #1
	ldrb	r3, [r6, r2]
	subs	r3, #56
	adds	r2, #1
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldrb	r3, [r6, r2]
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	subs	r3, #63
	lsls	r3, r3, #13
	str	r3, [r5, #12]
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	negs	r3, r3
	movs	r4, #1
	subs	r3, #16
	add	r8, r4
	lsls	r3, r3, #14
	mov	r7, r8
	str	r3, [r5, #16]
	adds	r5, #28
	cmp	r7, #64
	bne.n	.L_080d7fec
	ldr	r0, [sp, #40]
	cmp	r0, #72
	bne.n	.L_080d803e
	ldr	r1, [sp, #44]
	ldr	r3, [pc, #492]
	adds	r2, r1, r3
	movs	r3, #4
	str	r3, [r2, #0]
.L_080d803e:
	ldr	r4, [sp, #40]
	cmp	r4, #71
	ble.n	.L_080d80e0
	movs	r5, #32
	mov	sl, r5
	cmp	r4, #72
	beq.n	.L_080d8050
	movs	r7, #64
	mov	sl, r7
.L_080d8050:
	movs	r0, #0
	mov	r1, sl
	mov	r8, r0
	cmp	r1, #0
	beq.n	.L_080d80da
	ldr	r2, [sp, #44]
	movs	r3, #225
	lsls	r3, r3, #7
	adds	r6, r2, r3
.L_080d8062:
	movs	r4, #6
	ldrsh	r7, [r6, r4]
	cmp	r7, #135
	bgt.n	.L_080d80d0
	movs	r1, #3
	mov	r0, r8
	bl	sub_080022fc
	ldr	r2, [pc, #456]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r5, [sp, #44]
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	ldr	r4, [pc, #448]
	ldr	r3, [pc, #448]
	adds	r1, r5, r1
	ldrb	r5, [r3, r0]
	ldrb	r0, [r4, r0]
	subs	r3, r7, r5
	str	r0, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #36]
	bl	sub_080072f4
	movs	r2, #128
	adds	r0, r6, #0
	movs	r1, #64
	lsls	r2, r2, #9
	bl	sub_080e3908
	movs	r5, #6
	ldrsh	r3, [r6, r5]
	cmp	r3, #120
	ble.n	.L_080d80d0
	movs	r7, #128
	ldr	r3, [r6, #16]
	lsls	r7, r7, #12
	cmp	r3, r7
	ble.n	.L_080d80d0
	negs	r3, r3
	cmp	r3, #0
	bge.n	.L_080d80bc
	adds	r3, #3
.L_080d80bc:
	asrs	r3, r3, #2
	str	r3, [r6, #16]
	movs	r3, #240
	lsls	r3, r3, #15
	str	r3, [r6, #4]
	ldr	r0, [sp, #44]
	ldr	r1, [pc, #344]
	movs	r3, #1
	adds	r2, r0, r1
	str	r3, [r2, #0]
.L_080d80d0:
	movs	r2, #1
	add	r8, r2
	adds	r6, #28
	cmp	r8, sl
	bne.n	.L_080d8062
.L_080d80da:
	ldr	r3, [sp, #40]
	cmp	r3, #71
	bgt.n	.L_080d8144
.L_080d80e0:
	ldr	r7, [sp, #44]
	movs	r0, #225
	movs	r4, #0
	lsls	r0, r0, #7
	mov	r8, r4
	adds	r5, r7, r0
.L_080d80ec:
	movs	r1, #2
	ldrsh	r2, [r5, r1]
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	movs	r1, #24
	str	r1, [sp, #0]
	movs	r1, #48
	subs	r2, #12
	subs	r3, #24
	str	r1, [sp, #4]
	ldr	r0, [sp, #48]
	ldr	r1, [sp, #44]
	ldr	r7, [sp, #36]
	bl	sub_08007300
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	asrs	r3, r3, #16
	cmp	r3, #120
	ble.n	.L_080d8138
	ldr	r0, [sp, #40]
	cmp	r0, #47
	bgt.n	.L_080d8138
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	adds	r3, #64
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r3, [pc, #272]
	str	r3, [r5, #4]
.L_080d8138:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L_080d80ec
.L_080d8144:
	ldr	r2, [pc, #260]
	ldr	r4, [sp, #44]
	movs	r3, #0
	mov	r8, r3
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_080d8186
	movs	r6, #36
	movs	r5, #32
.L_080d8158:
	ldr	r7, [sp, #40]
	cmp	r7, r5
	bne.n	.L_080d8172
	ldr	r0, [sp, #44]
	ldr	r3, [r0, r2]
	ldrsh	r0, [r3, r6]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #9
	movs	r2, #5
	subs	r3, #1
	bl	sub_080d6888
.L_080d8172:
	movs	r2, #1
	add	r8, r2
	ldr	r4, [sp, #44]
	ldr	r2, [pc, #208]
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	adds	r6, #2
	adds	r5, #8
	cmp	r8, r3
	bne.n	.L_080d8158
.L_080d8186:
	ldr	r5, [sp, #40]
	cmp	r5, #72
	ble.n	.L_080d8208
	adds	r6, r5, #0
	movs	r7, #0
	ldr	r5, [pc, #136]
	mov	r8, r7
	subs	r6, #54
.L_080d8196:
	ldr	r3, [r5, #24]
	cmp	r3, #17
	bhi.n	.L_080d81ce
	lsrs	r0, r3, #31
	adds	r0, r3, r0
	movs	r1, #9
	asrs	r0, r0, #1
	bl	sub_080022fc
	ldr	r3, [pc, #164]
	ldrb	r4, [r3, r0]
	ldr	r3, [pc, #164]
	lsls	r0, r0, #1
	ldrh	r1, [r3, r0]
	ldr	r0, [sp, #28]
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	adds	r1, r0, r1
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	subs	r2, r2, r0
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #36]
	bl	sub_080072f4
	ldr	r3, [r5, #24]
.L_080d81ce:
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #18
	bne.n	.L_080d81fc
	ldr	r7, [sp, #40]
	cmp	r7, #127
	bgt.n	.L_080d81fc
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08004458
	lsrs	r3, r6, #31
	movs	r2, #63
	adds	r3, r6, r3
	asrs	r3, r3, #1
	ands	r2, r0
	adds	r2, r2, r3
	movs	r3, #0
	str	r2, [r5, #4]
	str	r3, [r5, #24]
.L_080d81fc:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #64
	bne.n	.L_080d8196
.L_080d8208:
	ldr	r3, [sp, #40]
	subs	r3, #72
	cmp	r3, #7
	bhi.n	.L_080d8258
	movs	r0, #8
	movs	r1, #8
	bl	sub_080e155c
	b.n	.L_080d8260
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0xfffffc20
	.4byte 0x000077a8
	.4byte 0x000014f9
	.4byte 0x080ee9e0
	.4byte 0x080ee974
	.4byte 0x080ee9e9
	.4byte 0x080ee9e6
	.4byte 0x080ee9f2
	.4byte 0x080ee9ec
	.4byte 0x080ee9ef
	.4byte 0xfff00000
	.4byte 0x00007828
	.4byte 0x080ede96
	.2byte 0xde84
	.2byte 0x080e
.L_080d8258:
	movs	r0, #2
	movs	r1, #2
	bl	sub_080e155c
.L_080d8260:
	ldr	r3, [sp, #44]
	ldr	r4, [pc, #68]
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r7, [sp, #40]
	movs	r5, #14
	adds	r7, #1
	add	r9, r5
	str	r7, [sp, #40]
	cmp	r7, #146
	beq.n	.L_080d8280
	b.n	.L_080d7f08
.L_080d8280:
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r0, [pc, #28]
	bl	sub_08004278
	bl	sub_080cdbc0
	add	sp, #332
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00007824
	.4byte 0x080cd261

@ Retired from C: the registered source did not reproduce these bytes
@ under the family's flags. Disassembled from the audited extent at
@ 0x080bbb0c (6332 bytes); the draft C stays in games/gs1/recon/en/main/.
.syntax unified
	.thumb
	.global Func_080bbb0c
	.thumb_func
Func_080bbb0c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #100	@ 0x64
	str	r0, [sp, #80]	@ 0x50
	movs	r0, #0
	str	r0, [sp, #60]	@ 0x3c
	ldr	r3, [pc, #796]	@ (0x80bbe40)
	movs	r5, #166	@ 0xa6
	ldr	r3, [r3, #0]
	lsls	r5, r5, #1
	str	r0, [sp, #52]	@ 0x34
	str	r0, [sp, #44]	@ 0x2c
	str	r0, [sp, #40]	@ 0x28
	str	r0, [sp, #28]
	str	r0, [sp, #20]
	adds	r0, r5, #0
	str	r3, [sp, #56]	@ 0x38
	adds	r6, r1, #0
	bl	Func_08004938
	str	r0, [sp, #16]
	ldr	r1, [sp, #80]	@ 0x50
	ldrb	r1, [r1, #0]
	ldr	r2, [sp, #80]	@ 0x50
	str	r1, [sp, #68]	@ 0x44
	ldr	r4, [sp, #80]	@ 0x50
	adds	r2, #2
	ldrb	r3, [r2, r6]
	ldr	r4, [r4, #76]	@ 0x4c
	mov	sl, r3
	str	r4, [sp, #64]	@ 0x40
	adds	r3, r6, #0
	adds	r3, #28
	ldrsb	r3, [r2, r3]
	ldr	r0, [sp, #80]	@ 0x50
	ldr	r1, [sp, #80]	@ 0x50
	ldr	r0, [r0, #80]	@ 0x50
	str	r3, [sp, #48]	@ 0x30
	adds	r3, r6, #0
	adds	r3, #44	@ 0x2c
	ldrsb	r3, [r1, r3]
	mov	r9, r0
	adds	r0, r4, #0
	str	r3, [sp, #32]
	mov	r8, r2
	bl	Func_08077080
	str	r0, [sp, #76]	@ 0x4c
	ldr	r0, [sp, #68]	@ 0x44
	bl	Func_08077008
	str	r0, [sp, #72]	@ 0x48
	mov	r0, sl
	bl	Func_08077008
	adds	r7, r0, #0
	ldr	r3, [pc, #700]	@ (0x80bbe44)
	adds	r2, r5, #0
	ldr	r0, [sp, #16]
	adds	r1, r7, #0
	bl	Func_080072f0
	ldr	r2, [sp, #76]	@ 0x4c
	ldrb	r3, [r2, #8]
	cmp	r3, #255	@ 0xff
	beq.n	.L_080bbbb0
	adds	r3, r6, #0
	ldr	r4, [sp, #80]	@ 0x50
	adds	r3, #16
	ldrsb	r3, [r4, r3]
	mov	fp, r3
	cmp	r3, #0
	bge.n	.L_080bbbb4
	mov	r5, fp
	negs	r5, r5
	mov	fp, r5
	b.n	.L_080bbbb4
.L_080bbbb0:
	movs	r0, #0
	mov	fp, r0
.L_080bbbb4:
	mov	r1, r9
	cmp	r1, #4
	beq.n	.L_080bbc12
	lsls	r3, r1, #2
	adds	r4, r7, #0
	adds	r3, r3, r7
	adds	r4, #36	@ 0x24
	movs	r2, #38	@ 0x26
	ldrsh	r0, [r3, r2]
	movs	r5, #2
	ldrsh	r3, [r4, r5]
	movs	r1, #0
	cmp	r0, r3
	blt.n	.L_080bbbe2
	adds	r2, r4, #0
.L_080bbbd2:
	adds	r1, #1
	adds	r2, #4
	cmp	r1, #3
	bgt.n	.L_080bbbe2
	movs	r5, #2
	ldrsh	r3, [r2, r5]
	cmp	r0, r3
	bge.n	.L_080bbbd2
.L_080bbbe2:
	cmp	r1, #4
	bne.n	.L_080bbbec
	movs	r1, #1
	negs	r1, r1
	str	r1, [sp, #20]
.L_080bbbec:
	movs	r2, #2
	ldrsh	r3, [r4, r2]
	movs	r1, #0
	cmp	r0, r3
	bgt.n	.L_080bbc0a
	adds	r2, r7, #0
	adds	r2, #36	@ 0x24
.L_080bbbfa:
	adds	r1, #1
	adds	r2, #4
	cmp	r1, #3
	bgt.n	.L_080bbc0a
	movs	r4, #2
	ldrsh	r3, [r2, r4]
	cmp	r0, r3
	ble.n	.L_080bbbfa
.L_080bbc0a:
	cmp	r1, #4
	bne.n	.L_080bbc12
	movs	r5, #1
	str	r5, [sp, #20]
.L_080bbc12:
	ldr	r0, [sp, #80]	@ 0x50
	ldr	r2, [r0, #80]	@ 0x50
	cmp	r2, #3
	bhi.n	.L_080bbc32
	adds	r0, #72	@ 0x48
	str	r0, [sp, #4]
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	cmp	r3, #2
	beq.n	.L_080bbc38
	lsls	r3, r2, #2
	ldr	r4, [sp, #72]	@ 0x48
	adds	r3, #72	@ 0x48
	ldrsh	r4, [r4, r3]
	str	r4, [sp, #12]
	b.n	.L_080bbc3c
.L_080bbc32:
	ldr	r0, [sp, #80]	@ 0x50
	adds	r0, #72	@ 0x48
	str	r0, [sp, #4]
.L_080bbc38:
	movs	r1, #100	@ 0x64
	str	r1, [sp, #12]
.L_080bbc3c:
	ldr	r5, [sp, #4]
	movs	r4, #0
	ldrsh	r3, [r5, r4]
	cmp	r3, #5
	bne.n	.L_080bbc78
	cmp	r2, #3
	bhi.n	.L_080bbc78
	ldr	r0, [sp, #20]
	cmp	r0, #0
	ble.n	.L_080bbc78
	lsls	r3, r2, #2
	adds	r3, #72	@ 0x48
	adds	r3, r7, r3
	movs	r1, #2
	ldrsh	r5, [r3, r1]
	ldr	r2, [sp, #12]
	ldr	r3, [pc, #488]	@ (0x80bbe48)
	subs	r5, r2, r5
	adds	r5, #30
	muls	r5, r3
	bl	Func_080771a0
	ldr	r3, [pc, #480]	@ (0x80bbe4c)
	ands	r0, r3
	cmp	r5, r0
	ble.n	.L_080bbc78
	movs	r0, #13
	movs	r1, #5
	bl	Func_080bbabc
.L_080bbc78:
	ldr	r4, [sp, #76]	@ 0x4c
	ldrb	r3, [r4, #1]
	movs	r5, #15
	ands	r5, r3
	str	r5, [sp, #24]
	adds	r3, r6, #0
	adds	r3, #56	@ 0x38
	mov	r6, r8
	movs	r1, #1
	ldrsb	r0, [r6, r3]
	negs	r1, r1
	cmp	r0, r1
	bne.n	.L_080bbca6
	ldr	r2, [pc, #444]	@ (0x80bbe50)
	ldrb	r3, [r4, #3]
	mov	r4, fp
	ldrb	r2, [r2, r4]
	ldr	r0, [sp, #68]	@ 0x44
	str	r2, [sp, #0]
	mov	r1, sl
	mov	r2, r9
	bl	Func_08077178
.L_080bbca6:
	str	r0, [sp, #36]	@ 0x24
	ldr	r5, [sp, #76]	@ 0x4c
	ldrb	r3, [r5, #3]
	movs	r6, #128	@ 0x80
	adds	r3, #206	@ 0xce
	lsls	r3, r3, #24
	lsls	r6, r6, #17
	cmp	r3, r6
	bls.n	.L_080bbcba
	b.n	.L_080bbdda
.L_080bbcba:
	ldr	r0, [sp, #72]	@ 0x48
	movs	r1, #148	@ 0x94
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldrb	r5, [r3, #0]
	bl	Func_080b7514
	ldr	r2, [sp, #76]	@ 0x4c
	ldrb	r3, [r2, #3]
	mov	r8, r0
	cmp	r3, #51	@ 0x33
	bne.n	.L_080bbcdc
	ldr	r3, [sp, #56]	@ 0x38
	ldr	r0, [r3, #0]
	bl	Func_080c1fa8
	adds	r5, r0, #0
.L_080bbcdc:
	ldr	r4, [sp, #36]	@ 0x24
	cmp	r4, #0
	beq.n	.L_080bbdc0
	adds	r0, r5, #0
	bl	Func_080b6cdc
	cmp	r0, #0
	beq.n	.L_080bbdc0
	mov	r6, r8
	cmp	r6, #0
	blt.n	.L_080bbdc0
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_080c1df4
	movs	r3, #128	@ 0x80
	adds	r6, r0, #0
	lsls	r3, r3, #8
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_080bbd0c
	adds	r0, r5, #0
	bl	Func_080c1f50
.L_080bbd0c:
	ldr	r2, [pc, #324]	@ (0x80bbe54)
	adds	r1, r5, #0
	ands	r2, r6
	mov	r0, r8
	bl	Func_08077140
	ldr	r1, [sp, #56]	@ 0x38
	movs	r2, #100	@ 0x64
	adds	r1, #2
	ldrsh	r3, [r1, r2]
	movs	r5, #0
	mov	lr, r5
	mov	ip, r1
	movs	r0, #100	@ 0x64
	movs	r4, #0
	cmp	r3, #254	@ 0xfe
	bne.n	.L_080bbd40
	mov	r3, r8
	strh	r3, [r1, r2]
	b.n	.L_080bbd5e
.L_080bbd34:
	mov	r3, lr
	mov	r4, r8
	adds	r3, #102	@ 0x66
	strh	r4, [r1, r0]
	strh	r2, [r1, r3]
	b.n	.L_080bbd5e
.L_080bbd40:
	mov	r6, ip
	ldrsh	r2, [r0, r6]
	cmp	r2, #255	@ 0xff
	beq.n	.L_080bbd34
	adds	r5, #1
	adds	r0, #2
	adds	r4, #2
	cmp	r5, #5
	bgt.n	.L_080bbd5e
	ldrsh	r3, [r0, r1]
	mov	lr, r4
	cmp	r3, #254	@ 0xfe
	bne.n	.L_080bbd40
	mov	r2, r8
	strh	r2, [r0, r1]
.L_080bbd5e:
	bl	Func_080b7548
	mov	r0, r8
	bl	Func_080b7dd0
	ldr	r2, [r0, #12]
	cmp	r2, #0
	bge.n	.L_080bbd72
	ldr	r3, [pc, #220]	@ (0x80bbe4c)
	adds	r2, r2, r3
.L_080bbd72:
	ldr	r3, [r0, #16]
	asrs	r2, r2, #16
	cmp	r3, #0
	bge.n	.L_080bbd7e
	ldr	r4, [pc, #208]	@ (0x80bbe4c)
	adds	r3, r3, r4
.L_080bbd7e:
	asrs	r3, r3, #16
	mov	r1, r8
	bl	Func_080b6f44
	bl	Func_080b6c90
	add	r5, sp, #84	@ 0x54
	adds	r0, r5, #0
	bl	Func_080b6ae0
	cmp	r0, #0
	ble.n	.L_080bbda8
	adds	r6, r5, #0
	adds	r5, r0, #0
.L_080bbd9a:
	ldrh	r0, [r6, #0]
	subs	r5, #1
	adds	r6, #2
	bl	Func_080b8000
	cmp	r5, #0
	bne.n	.L_080bbd9a
.L_080bbda8:
	movs	r0, #0
	mov	r1, r8
	bl	Func_080bbabc
	ldr	r6, [pc, #164]	@ (0x80bbe58)
	ldr	r5, [sp, #64]	@ 0x40
	cmp	r5, r6
	beq.n	.L_080bbdbc
	ldr	r1, [pc, #160]	@ (0x80bbe5c)
	b.n	.L_080bbdca
.L_080bbdbc:
	ldr	r1, [pc, #160]	@ (0x80bbe60)
	b.n	.L_080bbdca
.L_080bbdc0:
	ldr	r0, [sp, #64]	@ 0x40
	ldr	r1, [pc, #148]	@ (0x80bbe58)
	cmp	r0, r1
	bne.n	.L_080bbdd2
	ldr	r1, [pc, #152]	@ (0x80bbe64)
.L_080bbdca:
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L_080bbdda
.L_080bbdd2:
	ldr	r1, [pc, #148]	@ (0x80bbe68)
	movs	r0, #4
	bl	Func_080bbabc
.L_080bbdda:
	ldr	r2, [sp, #36]	@ 0x24
	cmp	r2, #0
	beq.n	.L_080bbe9a
	ldr	r3, [sp, #76]	@ 0x4c
	ldrb	r2, [r3, #3]
	adds	r3, r2, #0
	cmp	r3, #53	@ 0x35
	bne.n	.L_080bbe1e
	movs	r4, #0
	str	r4, [sp, #36]	@ 0x24
	movs	r3, #187	@ 0xbb
	ldr	r5, [sp, #56]	@ 0x38
	lsls	r3, r3, #2
	ldrsh	r3, [r5, r3]
	movs	r2, #0
	cmp	r3, sl
	bne.n	.L_080bbe02
	movs	r0, #1
	str	r0, [sp, #36]	@ 0x24
	b.n	.L_080bbe9a
.L_080bbe02:
	adds	r2, #1
	cmp	r2, #19
	bhi.n	.L_080bbe9a
	movs	r1, #187	@ 0xbb
	lsls	r3, r2, #4
	lsls	r1, r1, #2
	ldr	r4, [sp, #56]	@ 0x38
	adds	r3, r3, r1
	ldrsh	r3, [r4, r3]
	cmp	r3, sl
	bne.n	.L_080bbe02
	movs	r6, #1
	str	r6, [sp, #36]	@ 0x24
	b.n	.L_080bbe9a
.L_080bbe1e:
	adds	r3, r2, #0
	cmp	r3, #35	@ 0x23
	bne.n	.L_080bbe2a
	movs	r0, #1
	str	r0, [sp, #52]	@ 0x34
	b.n	.L_080bbe9a
.L_080bbe2a:
	cmp	r3, #34	@ 0x22
	bne.n	.L_080bbe34
	movs	r1, #1
	str	r1, [sp, #40]	@ 0x28
	b.n	.L_080bbe9a
.L_080bbe34:
	cmp	r3, #27
	bne.n	.L_080bbe6c
	movs	r2, #1
	str	r2, [sp, #28]
	b.n	.L_080bbe9a
	movs	r0, r0
	.4byte 0x03001e74
	.4byte 0x03001388
	.4byte 0x0000028f
	.4byte 0x0000ffff
	.4byte 0x080c2ab8
	.4byte 0x00007fff
	.4byte 0x000001f7
	.4byte 0x000008f5
	.4byte 0x000008f3
	.4byte 0x000008f4
	.2byte 0x08f6
	.2byte 0x0000
.L_080bbe6c:
	cmp	r3, #55	@ 0x37
	bne.n	.L_080bbe84
	ldr	r5, [sp, #72]	@ 0x48
	movs	r4, #56	@ 0x38
	ldrsh	r3, [r5, r4]
	cmp	r3, #0
	beq.n	.L_080bbe9a
	movs	r0, #12
	ldr	r1, [sp, #68]	@ 0x44
	bl	Func_080bbabc
	b.n	.L_080bbe9a
.L_080bbe84:
	cmp	r3, #32
	bne.n	.L_080bbe9a
	movs	r6, #58	@ 0x3a
	ldrsh	r3, [r7, r6]
	cmp	r3, #0
	beq.n	.L_080bbe96
	movs	r0, #10
	str	r0, [sp, #24]
	b.n	.L_080bbe9a
.L_080bbe96:
	movs	r1, #0
	str	r1, [sp, #36]	@ 0x24
.L_080bbe9a:
	ldr	r2, [sp, #28]
	cmp	r2, #0
	beq.n	.L_080bbea2
	b.n	.L_080bc666
.L_080bbea2:
	movs	r4, #56	@ 0x38
	ldrsh	r3, [r7, r4]
	cmp	r3, #0
	bne.n	.L_080bbeb8
	ldr	r5, [sp, #76]	@ 0x4c
	ldrb	r0, [r5, #3]
	bl	Func_080772b8
	cmp	r0, #0
	bne.n	.L_080bbeb8
	b.n	.L_080bc666
.L_080bbeb8:
	ldr	r3, [sp, #24]
	adds	r3, #1
	cmp	r3, #12
	bls.n	.L_080bbec2
	b.n	.L_080bc666
.L_080bbec2:
	ldr	r2, [pc, #792]	@ (0x80bc1dc)
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080bc20c
	.4byte 0x080bc666
	.4byte 0x080bc150
	.4byte 0x080bc51e
	.4byte 0x080bbf00
	.4byte 0x080bbf00
	.4byte 0x080bc2ba
	.4byte 0x080bc2ba
	.4byte 0x080bc666
	.4byte 0x080bc2ba
	.4byte 0x080bc666
	.4byte 0x080bc084
	.2byte 0xc49a
	.2byte 0x080b
	ldrh	r1, [r7, #62]	@ 0x3e
	ldr	r2, [sp, #52]	@ 0x34
	movs	r0, #56	@ 0x38
	ldrsh	r6, [r7, r0]
	mov	fp, r1
	cmp	r2, #0
	beq.n	.L_080bbf12
	lsrs	r1, r1, #1
	mov	fp, r1
.L_080bbf12:
	movs	r3, #1
	mov	r8, r3
.L_080bbf16:
	mov	r4, r9
	cmp	r4, #4
	beq.n	.L_080bbf2c
	lsls	r3, r4, #2
	adds	r3, #72	@ 0x48
	adds	r3, r7, r3
	movs	r5, #2
	ldrsh	r3, [r3, r5]
	ldr	r0, [sp, #12]
	subs	r3, r0, r3
	str	r3, [sp, #60]	@ 0x3c
.L_080bbf2c:
	mov	r1, r8
	cmp	r1, #0
	bne.n	.L_080bbf36
	movs	r2, #0
	str	r2, [sp, #60]	@ 0x3c
.L_080bbf36:
	ldr	r3, [sp, #76]	@ 0x4c
	ldr	r4, [sp, #24]
	ldrh	r5, [r3, #10]
	cmp	r4, #4
	bne.n	.L_080bbf58
	ldr	r1, [sp, #72]	@ 0x48
	movs	r2, #0
	ldrh	r0, [r1, #60]	@ 0x3c
	ldr	r3, [sp, #60]	@ 0x3c
	mov	r1, fp
	bl	Func_08077180
	movs	r1, #10
	muls	r0, r5
	bl	Func_080022ec
	b.n	.L_080bbf66
.L_080bbf58:
	ldr	r2, [sp, #72]	@ 0x48
	mov	r1, fp
	ldrh	r0, [r2, #60]	@ 0x3c
	ldr	r3, [sp, #60]	@ 0x3c
	adds	r2, r5, #0
	bl	Func_08077180
.L_080bbf66:
	adds	r5, r0, #0
	ldr	r3, [sp, #48]	@ 0x30
	ldr	r4, [sp, #32]
	muls	r5, r3
	cmp	r4, #0
	beq.n	.L_080bbfbc
	cmp	r4, #1
	bne.n	.L_080bbf84
	lsls	r3, r5, #2
	adds	r0, r3, r5
	cmp	r0, #0
	bge.n	.L_080bbf80
	adds	r0, #3
.L_080bbf80:
	asrs	r5, r0, #2
	b.n	.L_080bbf8e
.L_080bbf84:
	lsls	r3, r5, #1
	adds	r3, r3, r5
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r5, r3, #1
.L_080bbf8e:
	ldrb	r0, [r7, #15]
	movs	r1, #5
	bl	Func_080022f4
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	adds	r0, r5, r0
	adds	r5, r0, #6
	mov	r0, r8
	cmp	r0, #0
	bne.n	.L_080bbfbc
	movs	r1, #0
	movs	r0, #6
	bl	Func_080bbabc
	mov	r2, sl
	ldr	r1, [pc, #560]	@ (0x80bc1e0)
	cmp	r2, #7
	bhi.n	.L_080bbfb6
	adds	r1, #1
.L_080bbfb6:
	movs	r0, #5
	bl	Func_080bbabc
.L_080bbfbc:
	bl	Func_080771a0
	movs	r3, #3
	ldr	r4, [pc, #544]	@ (0x80bc1e4)
	ands	r3, r0
	adds	r5, r5, r3
	adds	r3, r7, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080bbfea
	cmp	r3, #1
	bne.n	.L_080bbfe0
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
	b.n	.L_080bbfea
.L_080bbfe0:
	adds	r0, r5, #0
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r0, #0
.L_080bbfea:
	cmp	r5, #0
	bgt.n	.L_080bbff0
	movs	r5, #1
.L_080bbff0:
	ldr	r0, [sp, #40]	@ 0x28
	cmp	r0, #0
	beq.n	.L_080bc004
	subs	r3, r6, #1
	cmp	r5, r3
	bge.n	.L_080bc004
	adds	r5, r3, #0
	cmp	r5, #0
	bgt.n	.L_080bc004
	movs	r5, #1
.L_080bc004:
	movs	r0, #183	@ 0xb7
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L_080bc020
	ldr	r2, [sp, #4]
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	cmp	r3, #5
	bne.n	.L_080bc020
	cmp	r6, r5
	bgt.n	.L_080bc020
	subs	r5, r6, #1
.L_080bc020:
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	cmp	r4, #1
	bgt.n	.L_080bc02c
	b.n	.L_080bbf16
.L_080bc02c:
	mov	r1, sl
	movs	r0, #8
	bl	Func_080bbabc
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	subs	r6, r6, r5
	adds	r1, r5, #0
	movs	r0, #1
	mov	r5, sl
	bl	Func_080bbabc
	cmp	r5, #7
	bhi.n	.L_080bc054
	ldr	r3, [pc, #408]	@ (0x80bc1e8)
	ldr	r0, [sp, #20]
	adds	r1, r0, r3
	b.n	.L_080bc05a
.L_080bc054:
	ldr	r3, [pc, #404]	@ (0x80bc1ec)
	ldr	r2, [sp, #20]
	adds	r1, r2, r3
.L_080bc05a:
	movs	r0, #4
	bl	Func_080bbabc
	cmp	r6, #0
	ble.n	.L_080bc066
	b.n	.L_080bc634
.L_080bc066:
	mov	r1, sl
	movs	r0, #9
	bl	Func_080bbabc
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
	mov	r3, sl
	movs	r6, #0
	cmp	r3, #7
	bls.n	.L_080bc080
	b.n	.L_080bc62a
.L_080bc080:
	ldr	r1, [pc, #364]	@ (0x80bc1f0)
	b.n	.L_080bc62c
	ldr	r5, [sp, #76]	@ 0x4c
	ldrh	r3, [r5, #10]
	cmp	r3, #0
	bne.n	.L_080bc08e
	b.n	.L_080bc666
.L_080bc08e:
	mov	r1, r9
	movs	r0, #58	@ 0x3a
	ldrsh	r6, [r7, r0]
	cmp	r1, #4
	beq.n	.L_080bc0a8
	lsls	r3, r1, #2
	adds	r3, #72	@ 0x48
	adds	r3, r7, r3
	movs	r2, #2
	ldrsh	r3, [r3, r2]
	ldr	r4, [sp, #12]
	subs	r3, r4, r3
	str	r3, [sp, #60]	@ 0x3c
.L_080bc0a8:
	ldr	r0, [sp, #76]	@ 0x4c
	ldrh	r5, [r0, #10]
	movs	r2, #128	@ 0x80
	ldr	r1, [sp, #60]	@ 0x3c
	lsls	r2, r2, #1
	adds	r0, r5, #0
	bl	Func_08077188
	ldr	r2, [pc, #312]	@ (0x80bc1f4)
	mov	r1, fp
	lsls	r3, r1, #2
	ldr	r3, [r2, r3]
	adds	r5, r0, #0
	adds	r0, r3, #0
	muls	r0, r5
	movs	r1, #100	@ 0x64
	bl	Func_080022ec
	ldr	r4, [pc, #276]	@ (0x80bc1e4)
	adds	r3, r7, r4
	ldr	r2, [sp, #48]	@ 0x30
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r5, r0, #0
	muls	r5, r2
	cmp	r3, #0
	beq.n	.L_080bc0f6
	cmp	r3, #1
	bne.n	.L_080bc0ec
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
	b.n	.L_080bc0f6
.L_080bc0ec:
	adds	r0, r5, #0
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r0, #0
.L_080bc0f6:
	ldr	r0, [sp, #76]	@ 0x4c
	ldrb	r3, [r0, #3]
	cmp	r3, #32
	bne.n	.L_080bc104
	cmp	r5, r6
	ble.n	.L_080bc104
	adds	r5, r6, #0
.L_080bc104:
	mov	r1, sl
	movs	r0, #8
	bl	Func_080bbabc
	adds	r1, r5, #0
	movs	r0, #1
	bl	Func_080bbabc
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	mov	r1, sl
	cmp	r1, #7
	bhi.n	.L_080bc126
	ldr	r1, [pc, #212]	@ (0x80bc1f8)
	b.n	.L_080bc128
.L_080bc126:
	ldr	r1, [pc, #212]	@ (0x80bc1fc)
.L_080bc128:
	movs	r0, #4
	subs	r6, r6, r5
	bl	Func_080bbabc
	cmp	r6, #0
	bgt.n	.L_080bc136
	movs	r6, #0
.L_080bc136:
	movs	r0, #11
	mov	r1, sl
	bl	Func_080bbabc
	movs	r2, #58	@ 0x3a
	ldrsh	r3, [r7, r2]
	subs	r3, r3, r6
	str	r3, [sp, #44]	@ 0x2c
	mov	r0, sl
	strh	r6, [r7, #58]	@ 0x3a
	bl	Func_08077128
	b.n	.L_080bc666
	ldr	r4, [sp, #76]	@ 0x4c
	ldrh	r3, [r4, #10]
	cmp	r3, #0
	bne.n	.L_080bc15a
	b.n	.L_080bc666
.L_080bc15a:
	mov	r0, r9
	movs	r5, #56	@ 0x38
	ldrsh	r6, [r7, r5]
	ldr	r1, [sp, #12]
	adds	r5, r3, #0
	cmp	r0, #4
	bne.n	.L_080bc16a
	movs	r1, #100	@ 0x64
.L_080bc16a:
	movs	r2, #128	@ 0x80
	lsls	r2, r2, #1
	adds	r0, r5, #0
	bl	Func_08077190
	ldr	r2, [pc, #136]	@ (0x80bc200)
	mov	r1, fp
	lsls	r3, r1, #2
	ldr	r3, [r2, r3]
	adds	r5, r0, #0
	adds	r0, r3, #0
	muls	r0, r5
	movs	r1, #100	@ 0x64
	bl	Func_080022ec
	ldr	r2, [sp, #48]	@ 0x30
	adds	r5, r0, #0
	muls	r5, r2
	bl	Func_080771a0
	movs	r3, #3
	ands	r3, r0
	adds	r5, r5, r3
	movs	r4, #52	@ 0x34
	ldrsh	r3, [r7, r4]
	adds	r6, r6, r5
	cmp	r6, r3
	ble.n	.L_080bc1aa
	adds	r6, r3, #0
	movs	r5, #56	@ 0x38
	ldrsh	r3, [r7, r5]
	subs	r5, r6, r3
.L_080bc1aa:
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
	movs	r0, #52	@ 0x34
	ldrsh	r3, [r7, r0]
	cmp	r6, r3
	bne.n	.L_080bc1c4
	ldr	r1, [pc, #72]	@ (0x80bc204)
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L_080bc1d4
.L_080bc1c4:
	adds	r1, r5, #0
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #56]	@ (0x80bc208)
	movs	r0, #4
	bl	Func_080bbabc
.L_080bc1d4:
	movs	r1, #56	@ 0x38
	ldrsh	r3, [r7, r1]
	b.n	.L_080bc640
	movs	r0, r0
	.4byte 0x080bbecc
	.4byte 0x00000822
	.4byte 0x0000012b
	.4byte 0x00000834
	.4byte 0x00000831
	.4byte 0x00000825
	.4byte 0x080c2ac0
	.4byte 0x0000082a
	.4byte 0x00000829
	.4byte 0x080c2ad8
	.4byte 0x00000820
	.2byte 0x081d
	.2byte 0x0000
	ldr	r2, [sp, #76]	@ 0x4c
	ldrh	r3, [r2, #10]
	cmp	r3, #0
	bne.n	.L_080bc216
	b.n	.L_080bc666
.L_080bc216:
	mov	r4, r9
	movs	r3, #58	@ 0x3a
	ldrsh	r6, [r7, r3]
	cmp	r4, #4
	beq.n	.L_080bc230
	lsls	r3, r4, #2
	adds	r3, #72	@ 0x48
	adds	r3, r7, r3
	movs	r5, #2
	ldrsh	r3, [r3, r5]
	ldr	r0, [sp, #12]
	subs	r3, r0, r3
	str	r3, [sp, #60]	@ 0x3c
.L_080bc230:
	ldr	r1, [sp, #76]	@ 0x4c
	ldrh	r5, [r1, #10]
	movs	r2, #128	@ 0x80
	ldr	r1, [sp, #60]	@ 0x3c
	lsls	r2, r2, #1
	adds	r0, r5, #0
	bl	Func_08077188
	ldr	r2, [pc, #844]	@ (0x80bc590)
	mov	r4, fp
	lsls	r3, r4, #2
	ldr	r3, [r2, r3]
	adds	r5, r0, #0
	adds	r0, r3, #0
	muls	r0, r5
	movs	r1, #100	@ 0x64
	bl	Func_080022ec
	ldr	r1, [pc, #828]	@ (0x80bc594)
	adds	r3, r7, r1
	adds	r5, r0, #0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r0, [sp, #48]	@ 0x30
	muls	r5, r0
	cmp	r3, #0
	beq.n	.L_080bc27e
	cmp	r3, #1
	bne.n	.L_080bc274
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
	b.n	.L_080bc27e
.L_080bc274:
	adds	r0, r5, #0
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r0, #0
.L_080bc27e:
	mov	r1, sl
	movs	r0, #8
	bl	Func_080bbabc
	adds	r1, r5, #0
	movs	r0, #1
	bl	Func_080bbabc
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
	mov	r2, sl
	cmp	r2, #7
	bhi.n	.L_080bc2a0
	ldr	r1, [pc, #760]	@ (0x80bc598)
	b.n	.L_080bc2a2
.L_080bc2a0:
	ldr	r1, [pc, #760]	@ (0x80bc59c)
.L_080bc2a2:
	movs	r0, #4
	subs	r6, r6, r5
	bl	Func_080bbabc
	cmp	r6, #0
	bgt.n	.L_080bc2b0
	movs	r6, #0
.L_080bc2b0:
	movs	r0, #11
	mov	r1, sl
	bl	Func_080bbabc
	b.n	.L_080bc514
	ldr	r4, [sp, #76]	@ 0x4c
	ldrh	r3, [r4, #10]
	cmp	r3, #0
	bne.n	.L_080bc2c4
	b.n	.L_080bc666
.L_080bc2c4:
	movs	r0, #1
	movs	r5, #56	@ 0x38
	ldrsh	r6, [r7, r5]
	mov	r8, r0
.L_080bc2cc:
	mov	r1, r9
	cmp	r1, #4
	beq.n	.L_080bc2e2
	lsls	r3, r1, #2
	adds	r3, #72	@ 0x48
	adds	r3, r7, r3
	movs	r2, #2
	ldrsh	r3, [r3, r2]
	ldr	r4, [sp, #12]
	subs	r3, r4, r3
	str	r3, [sp, #60]	@ 0x3c
.L_080bc2e2:
	mov	r5, r8
	cmp	r5, #0
	bne.n	.L_080bc2ec
	movs	r0, #0
	str	r0, [sp, #60]	@ 0x3c
.L_080bc2ec:
	ldr	r4, [sp, #4]
	ldr	r1, [sp, #76]	@ 0x4c
	movs	r2, #0
	ldrsh	r3, [r4, r2]
	ldrh	r5, [r1, #10]
	cmp	r3, #6
	bne.n	.L_080bc38c
	ldr	r0, [sp, #64]	@ 0x40
	ldr	r1, [pc, #672]	@ (0x80bc5a0)
	adds	r3, r0, r1
	cmp	r3, #21
	bhi.n	.L_080bc37a
	ldr	r2, [pc, #668]	@ (0x80bc5a4)
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080bc364
	.4byte 0x080bc36a
	.4byte 0x080bc370
	.4byte 0x080bc376
	.4byte 0x080bc37a
	.4byte 0x080bc37a
	.4byte 0x080bc364
	.4byte 0x080bc36a
	.4byte 0x080bc370
	.4byte 0x080bc376
	.4byte 0x080bc37a
	.4byte 0x080bc37a
	.4byte 0x080bc364
	.4byte 0x080bc36a
	.4byte 0x080bc370
	.4byte 0x080bc376
	.4byte 0x080bc37a
	.4byte 0x080bc37a
	.4byte 0x080bc364
	.4byte 0x080bc36a
	.4byte 0x080bc370
	.2byte 0xc376
	.2byte 0x080b
	movs	r2, #3
	str	r2, [sp, #8]
	b.n	.L_080bc37a
	movs	r3, #6
	str	r3, [sp, #8]
	b.n	.L_080bc37a
	movs	r4, #9
	str	r4, [sp, #8]
	b.n	.L_080bc37a
	movs	r0, #12
	str	r0, [sp, #8]
.L_080bc37a:
	movs	r1, #52	@ 0x34
	ldrsh	r3, [r7, r1]
	ldr	r2, [sp, #8]
	movs	r1, #100	@ 0x64
	adds	r0, r2, #0
	muls	r0, r3
	bl	Func_080022ec
	adds	r5, r5, r0
.L_080bc38c:
	movs	r2, #128	@ 0x80
	adds	r0, r5, #0
	lsls	r2, r2, #1
	ldr	r1, [sp, #60]	@ 0x3c
	bl	Func_08077188
	ldr	r3, [sp, #48]	@ 0x30
	adds	r5, r0, #0
	muls	r5, r3
	ldr	r3, [sp, #24]
	cmp	r3, #6
	beq.n	.L_080bc3c4
	cmp	r3, #6
	bgt.n	.L_080bc3ae
	cmp	r3, #5
	beq.n	.L_080bc3b4
	b.n	.L_080bc3d8
.L_080bc3ae:
	cmp	r3, #8
	beq.n	.L_080bc3bc
	b.n	.L_080bc3d8
.L_080bc3b4:
	ldr	r2, [pc, #496]	@ (0x80bc5a8)
	mov	r4, fp
	lsls	r3, r4, #2
	b.n	.L_080bc3ca
.L_080bc3bc:
	ldr	r2, [pc, #492]	@ (0x80bc5ac)
	mov	r0, fp
	lsls	r3, r0, #2
	b.n	.L_080bc3ca
.L_080bc3c4:
	ldr	r2, [pc, #488]	@ (0x80bc5b0)
	mov	r1, fp
	lsls	r3, r1, #2
.L_080bc3ca:
	ldr	r3, [r2, r3]
	movs	r1, #100	@ 0x64
	adds	r0, r3, #0
	muls	r0, r5
	bl	Func_080022ec
	adds	r5, r0, #0
.L_080bc3d8:
	bl	Func_080771a0
	movs	r3, #3
	ldr	r2, [pc, #436]	@ (0x80bc594)
	ands	r3, r0
	adds	r5, r5, r3
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080bc406
	cmp	r3, #1
	bne.n	.L_080bc3fc
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
	b.n	.L_080bc406
.L_080bc3fc:
	adds	r0, r5, #0
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r0, #0
.L_080bc406:
	movs	r0, #183	@ 0xb7
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L_080bc422
	ldr	r0, [sp, #4]
	movs	r4, #0
	ldrsh	r3, [r0, r4]
	cmp	r3, #6
	bne.n	.L_080bc422
	cmp	r6, r5
	ble.n	.L_080bc422
	adds	r5, r6, #0
.L_080bc422:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #1
	bgt.n	.L_080bc42e
	b.n	.L_080bc2cc
.L_080bc42e:
	mov	r1, sl
	movs	r0, #8
	bl	Func_080bbabc
	adds	r1, r5, #0
	movs	r0, #1
	bl	Func_080bbabc
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
	mov	r3, sl
	cmp	r3, #7
	bhi.n	.L_080bc454
	ldr	r3, [pc, #356]	@ (0x80bc5b4)
	ldr	r4, [sp, #20]
	adds	r1, r4, r3
	b.n	.L_080bc45a
.L_080bc454:
	ldr	r3, [pc, #352]	@ (0x80bc5b8)
	ldr	r0, [sp, #20]
	adds	r1, r0, r3
.L_080bc45a:
	movs	r0, #4
	subs	r6, r6, r5
	bl	Func_080bbabc
	cmp	r6, #0
	bgt.n	.L_080bc48c
	mov	r1, sl
	movs	r0, #9
	bl	Func_080bbabc
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	mov	r1, sl
	movs	r6, #0
	cmp	r1, #7
	bhi.n	.L_080bc482
	ldr	r1, [pc, #316]	@ (0x80bc5bc)
	b.n	.L_080bc484
.L_080bc482:
	ldr	r1, [pc, #316]	@ (0x80bc5c0)
.L_080bc484:
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L_080bc494
.L_080bc48c:
	movs	r0, #11
	mov	r1, sl
	bl	Func_080bbabc
.L_080bc494:
	movs	r2, #56	@ 0x38
	ldrsh	r3, [r7, r2]
	b.n	.L_080bc640
	ldr	r4, [sp, #76]	@ 0x4c
	ldrh	r3, [r4, #10]
	cmp	r3, #0
	bne.n	.L_080bc4a4
	b.n	.L_080bc666
.L_080bc4a4:
	mov	r0, r9
	movs	r5, #58	@ 0x3a
	ldrsh	r6, [r7, r5]
	ldr	r1, [sp, #12]
	adds	r5, r3, #0
	cmp	r0, #4
	bne.n	.L_080bc4b4
	movs	r1, #100	@ 0x64
.L_080bc4b4:
	movs	r2, #128	@ 0x80
	lsls	r2, r2, #1
	adds	r0, r5, #0
	bl	Func_08077190
	ldr	r2, [pc, #260]	@ (0x80bc5c4)
	mov	r1, fp
	lsls	r3, r1, #2
	ldr	r3, [r2, r3]
	adds	r5, r0, #0
	adds	r0, r3, #0
	muls	r0, r5
	movs	r1, #100	@ 0x64
	bl	Func_080022ec
	ldr	r2, [sp, #48]	@ 0x30
	adds	r5, r0, #0
	muls	r5, r2
	movs	r4, #54	@ 0x36
	ldrsh	r3, [r7, r4]
	adds	r6, r6, r5
	cmp	r6, r3
	ble.n	.L_080bc4ea
	adds	r6, r3, #0
	movs	r5, #58	@ 0x3a
	ldrsh	r3, [r7, r5]
	subs	r5, r6, r3
.L_080bc4ea:
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
	movs	r0, #54	@ 0x36
	ldrsh	r3, [r7, r0]
	cmp	r6, r3
	bne.n	.L_080bc504
	ldr	r1, [pc, #204]	@ (0x80bc5c8)
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L_080bc514
.L_080bc504:
	adds	r1, r5, #0
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #188]	@ (0x80bc5cc)
	movs	r0, #4
	bl	Func_080bbabc
.L_080bc514:
	strh	r6, [r7, #58]	@ 0x3a
	mov	r0, sl
	bl	Func_08077128
	b.n	.L_080bc666
	ldr	r1, [sp, #36]	@ 0x24
	cmp	r1, #0
	bne.n	.L_080bc526
	b.n	.L_080bc64e
.L_080bc526:
	ldr	r2, [sp, #76]	@ 0x4c
	ldrh	r3, [r2, #10]
	cmp	r3, #0
	bne.n	.L_080bc530
	b.n	.L_080bc666
.L_080bc530:
	mov	r4, r9
	movs	r3, #56	@ 0x38
	ldrsh	r6, [r7, r3]
	cmp	r4, #4
	beq.n	.L_080bc54a
	lsls	r3, r4, #2
	adds	r3, #72	@ 0x48
	adds	r3, r7, r3
	movs	r5, #2
	ldrsh	r3, [r3, r5]
	ldr	r0, [sp, #12]
	subs	r3, r0, r3
	str	r3, [sp, #60]	@ 0x3c
.L_080bc54a:
	ldr	r1, [sp, #76]	@ 0x4c
	ldrh	r5, [r1, #10]
	movs	r2, #128	@ 0x80
	ldr	r1, [sp, #60]	@ 0x3c
	lsls	r2, r2, #1
	adds	r0, r5, #0
	bl	Func_08077188
	ldr	r2, [sp, #48]	@ 0x30
	adds	r5, r0, #0
	muls	r5, r2
	mov	r4, fp
	ldr	r2, [pc, #108]	@ (0x80bc5d0)
	lsls	r3, r4, #2
	ldr	r3, [r2, r3]
	movs	r1, #100	@ 0x64
	adds	r0, r3, #0
	muls	r0, r5
	bl	Func_080022ec
	adds	r5, r0, #0
	ldr	r0, [pc, #28]	@ (0x80bc594)
	adds	r3, r7, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080bc5de
	cmp	r3, #1
	bne.n	.L_080bc5d4
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
	b.n	.L_080bc5de
	movs	r0, r0
	.4byte 0x080c2af0
	.4byte 0x0000012b
	.4byte 0x00000827
	.4byte 0x00000826
	.4byte 0xfffffe84
	.4byte 0x080bc30c
	.4byte 0x080c2b08
	.4byte 0x080c2b20
	.4byte 0x080c2b38
	.4byte 0x00000834
	.4byte 0x00000831
	.4byte 0x00000825
	.4byte 0x00000824
	.4byte 0x080c2b50
	.4byte 0x00000821
	.4byte 0x0000081e
	.2byte 0x2b68
	.2byte 0x080c
.L_080bc5d4:
	adds	r0, r5, #0
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r0, #0
.L_080bc5de:
	mov	r1, sl
	movs	r0, #8
	bl	Func_080bbabc
	adds	r1, r5, #0
	movs	r0, #1
	bl	Func_080bbabc
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	mov	r1, sl
	cmp	r1, #7
	bhi.n	.L_080bc600
	ldr	r1, [pc, #916]	@ (0x80bc994)
	b.n	.L_080bc602
.L_080bc600:
	ldr	r1, [pc, #916]	@ (0x80bc998)
.L_080bc602:
	movs	r0, #4
	subs	r6, r6, r5
	bl	Func_080bbabc
	cmp	r6, #0
	bgt.n	.L_080bc634
	mov	r1, sl
	movs	r0, #9
	bl	Func_080bbabc
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
	mov	r2, sl
	movs	r6, #0
	cmp	r2, #7
	bhi.n	.L_080bc62a
	ldr	r1, [pc, #884]	@ (0x80bc99c)
	b.n	.L_080bc62c
.L_080bc62a:
	ldr	r1, [pc, #884]	@ (0x80bc9a0)
.L_080bc62c:
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L_080bc63c
.L_080bc634:
	movs	r0, #11
	mov	r1, sl
	bl	Func_080bbabc
.L_080bc63c:
	movs	r4, #56	@ 0x38
	ldrsh	r3, [r7, r4]
.L_080bc640:
	subs	r3, r3, r6
	str	r3, [sp, #44]	@ 0x2c
	mov	r0, sl
	strh	r6, [r7, #56]	@ 0x38
	bl	Func_08077128
	b.n	.L_080bc666
.L_080bc64e:
	mov	r1, sl
	movs	r0, #11
	bl	Func_080bbabc
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	ldr	r1, [pc, #836]	@ (0x80bc9a4)
	movs	r0, #4
	bl	Func_080bbabc
.L_080bc666:
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
	ldr	r5, [sp, #76]	@ 0x4c
	ldrb	r0, [r5, #3]
	bl	Func_080772b8
	cmp	r0, #0
	bne.n	.L_080bc690
	movs	r6, #56	@ 0x38
	ldrsh	r3, [r7, r6]
	cmp	r3, #0
	bne.n	.L_080bc690
	ldrb	r0, [r5, #3]
	bl	Func_080bbae8
	cmp	r0, #0
	bne.n	.L_080bc690
	bl	.L_080bd2c0
.L_080bc690:
	ldr	r0, [sp, #36]	@ 0x24
	cmp	r0, #0
	bne.n	.L_080bc69a
	bl	.L_080bd2c0
.L_080bc69a:
	ldr	r1, [sp, #76]	@ 0x4c
	ldrb	r3, [r1, #3]
	subs	r3, #3
	cmp	r3, #66	@ 0x42
	bls.n	.L_080bc6a8
	bl	.L_080bd2c0
.L_080bc6a8:
	ldr	r2, [pc, #764]	@ (0x80bc9a8)
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080bcd38
	.4byte 0x080bc884
	.4byte 0x080bccd6
	.4byte 0x080bcb68
	.4byte 0x080bcb22
	.4byte 0x080bcad8
	.4byte 0x080bca8e
	.4byte 0x080bcc8c
	.4byte 0x080bcc42
	.4byte 0x080bcbfc
	.4byte 0x080bcbb2
	.4byte 0x080bce84
	.4byte 0x080bce30
	.4byte 0x080bcdde
	.4byte 0x080bcd8c
	.4byte 0x080bced8
	.4byte 0x080bcef4
	.4byte 0x080bcf10
	.4byte 0x080bcf20
	.4byte 0x080bcf32
	.4byte 0x080bcf42
	.4byte 0x080bcf50
	.4byte 0x080bcf5c
	.4byte 0x080bd1fa
	.4byte 0x080bcfa4
	.4byte 0x080bd20a
	.4byte 0x080bcfe0
	.4byte 0x080bcff4
	.4byte 0x080bd006
	.4byte 0x080bd068
	.4byte 0x080bd180
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2b6
	.4byte 0x080bd282
	.4byte 0x080bd29c
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd2c0
	.4byte 0x080bd274
	.4byte 0x080bd24e
	.4byte 0x080bd2c0
	.4byte 0x080bccf4
	.4byte 0x080bcd16
	.4byte 0x080bca5c
	.4byte 0x080bca2a
	.4byte 0x080bd006
	.4byte 0x080bc926
	.4byte 0x080bc926
	.4byte 0x080bc9dc
	.4byte 0x080bc7bc
	.4byte 0x080bd2c0
	.4byte 0x080bd23e
	.4byte 0x080bcf7e
	.4byte 0x080bd2c0
	.2byte 0xd128
	.2byte 0x080b
	movs	r3, #156	@ 0x9c
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080bc7dc
	movs	r3, #0
	ldr	r1, [pc, #480]	@ (0x80bc9ac)
	strb	r3, [r2, #0]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
.L_080bc7dc:
	ldr	r4, [pc, #464]	@ (0x80bc9b0)
	adds	r2, r7, r4
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080bc802
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #0
	movs	r0, #7
	bl	Func_080bbabc
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	ldr	r1, [pc, #440]	@ (0x80bc9b4)
	movs	r0, #4
	bl	Func_080bbabc
.L_080bc802:
	movs	r5, #158	@ 0x9e
	lsls	r5, r5, #1
	adds	r3, r7, r5
	movs	r6, #0
	ldr	r0, [pc, #428]	@ (0x80bc9b8)
	strb	r6, [r3, #0]
	adds	r2, r7, r0
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080bc828
	ldr	r1, [pc, #420]	@ (0x80bc9bc)
	strb	r6, [r2, #0]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
.L_080bc828:
	ldr	r1, [pc, #404]	@ (0x80bc9c0)
	adds	r2, r7, r1
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080bc844
	ldr	r1, [pc, #400]	@ (0x80bc9c4)
	strb	r6, [r2, #0]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
.L_080bc844:
	movs	r3, #160	@ 0xa0
	lsls	r3, r3, #1
	adds	r2, r7, r3
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080bc862
	ldr	r1, [pc, #372]	@ (0x80bc9c8)
	strb	r6, [r2, #0]
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #0
	mov	r1, sl
	bl	Func_080bbabc
.L_080bc862:
	ldr	r4, [pc, #360]	@ (0x80bc9cc)
	adds	r5, r7, r4
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	beq.n	.L_080bc878
	ldr	r1, [pc, #352]	@ (0x80bc9d0)
	movs	r0, #4
	bl	Func_080bbabc
	strb	r6, [r5, #0]
.L_080bc878:
	movs	r0, #7
	movs	r1, #0
	bl	Func_080bbabc
	bl	.L_080bd2c0
	movs	r5, #156	@ 0x9c
	lsls	r5, r5, #1
	adds	r2, r7, r5
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080bc8ac
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #0
	movs	r0, #7
	bl	Func_080bbabc
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	ldr	r1, [pc, #260]	@ (0x80bc9ac)
	movs	r0, #4
	bl	Func_080bbabc
.L_080bc8ac:
	ldr	r6, [pc, #256]	@ (0x80bc9b0)
	adds	r2, r7, r6
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080bc8d2
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #0
	movs	r0, #7
	bl	Func_080bbabc
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	ldr	r1, [pc, #232]	@ (0x80bc9b4)
	movs	r0, #4
	bl	Func_080bbabc
.L_080bc8d2:
	movs	r0, #158	@ 0x9e
	lsls	r0, r0, #1
	adds	r3, r7, r0
	movs	r5, #0
	ldr	r1, [pc, #220]	@ (0x80bc9b8)
	strb	r5, [r3, #0]
	adds	r2, r7, r1
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080bc900
	strb	r5, [r2, #0]
	movs	r1, #0
	movs	r0, #7
	bl	Func_080bbabc
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	ldr	r1, [pc, #192]	@ (0x80bc9bc)
	movs	r0, #4
	bl	Func_080bbabc
.L_080bc900:
	ldr	r3, [pc, #188]	@ (0x80bc9c0)
	adds	r2, r7, r3
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_080bc90e
	bl	.L_080bd2c0
.L_080bc90e:
	strb	r5, [r2, #0]
	movs	r1, #0
	movs	r0, #7
	bl	Func_080bbabc
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	ldr	r1, [pc, #160]	@ (0x80bc9c4)
	bl	.L_080bd236
	ldr	r1, [sp, #76]	@ 0x4c
	ldrh	r4, [r7, #56]	@ 0x38
	ldrb	r3, [r1, #3]
	movs	r6, #56	@ 0x38
	ldrsh	r5, [r7, r6]
	mov	r8, r4
	ldrh	r6, [r7, #52]	@ 0x34
	movs	r0, #52	@ 0x34
	ldrsh	r2, [r7, r0]
	cmp	r3, #61	@ 0x3d
	bne.n	.L_080bc944
	lsls	r0, r2, #4
	subs	r0, r0, r2
	lsls	r0, r0, #2
	b.n	.L_080bc94a
.L_080bc944:
	lsls	r0, r2, #4
	subs	r0, r0, r2
	lsls	r0, r0, #1
.L_080bc94a:
	movs	r1, #100	@ 0x64
	bl	Func_080022ec
	adds	r5, r5, r0
	lsls	r3, r6, #16
	asrs	r2, r3, #16
	cmp	r5, r2
	ble.n	.L_080bc95c
	adds	r5, r2, #0
.L_080bc95c:
	mov	r4, r8
	lsls	r3, r4, #16
	asrs	r3, r3, #16
	subs	r1, r5, r3
	cmp	r1, #0
	bne.n	.L_080bc972
	ldr	r6, [sp, #24]
	cmp	r6, #1
	beq.n	.L_080bc972
	bl	.L_080bd2c0
.L_080bc972:
	cmp	r5, r2
	bne.n	.L_080bc980
	ldr	r1, [pc, #92]	@ (0x80bc9d4)
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L_080bc98e
.L_080bc980:
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #80]	@ (0x80bc9d8)
	movs	r0, #4
	bl	Func_080bbabc
.L_080bc98e:
	strh	r5, [r7, #56]	@ 0x38
	b.n	.L_080bcfd8
	movs	r0, r0
	.4byte 0x00000827
	.4byte 0x00000826
	.4byte 0x00000825
	.4byte 0x00000824
	.4byte 0x00000854
	.4byte 0x080bc6b0
	.4byte 0x0000088b
	.4byte 0x0000013b
	.4byte 0x0000088d
	.4byte 0x0000013d
	.4byte 0x0000088c
	.4byte 0x00000141
	.4byte 0x00000894
	.4byte 0x0000088f
	.4byte 0x00000131
	.4byte 0x00000884
	.4byte 0x00000820
	.2byte 0x081d
	.2byte 0x0000
	movs	r1, #54	@ 0x36
	ldrsh	r6, [r7, r1]
	movs	r0, #58	@ 0x3a
	ldrsh	r5, [r7, r0]
	lsls	r0, r6, #3
	subs	r0, r0, r6
	movs	r1, #100	@ 0x64
	bl	Func_080022ec
	mov	r8, r5
	adds	r5, r5, r0
	cmp	r5, r6
	ble.n	.L_080bc9f8
	adds	r5, r6, #0
.L_080bc9f8:
	mov	r2, r8
	subs	r1, r5, r2
	cmp	r1, #0
	bne.n	.L_080bca0a
	ldr	r3, [sp, #24]
	cmp	r3, #11
	beq.n	.L_080bca0a
	bl	.L_080bd2c0
.L_080bca0a:
	cmp	r5, r6
	bne.n	.L_080bca18
	ldr	r1, [pc, #836]	@ (0x80bcd54)
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L_080bca26
.L_080bca18:
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #824]	@ (0x80bcd58)
	movs	r0, #4
	bl	Func_080bbabc
.L_080bca26:
	strh	r5, [r7, #58]	@ 0x3a
	b.n	.L_080bcfd8
	ldr	r4, [pc, #816]	@ (0x80bcd5c)
	movs	r5, #163	@ 0xa3
	adds	r2, r7, r4
	movs	r3, #8
	lsls	r5, r5, #1
	strb	r3, [r2, #0]
	adds	r2, r7, r5
	movs	r3, #5
	strb	r3, [r2, #0]
	mov	r0, sl
	bl	Func_08077010
	adds	r3, r7, #0
	adds	r3, #64	@ 0x40
	ldrh	r1, [r3, #0]
	ldr	r3, [sp, #16]
	adds	r3, #64	@ 0x40
	ldrh	r3, [r3, #0]
	movs	r0, #1
	subs	r1, r1, r3
	bl	Func_080bbabc
	ldr	r1, [pc, #776]	@ (0x80bcd60)
	bl	.L_080bd236
	ldr	r6, [pc, #764]	@ (0x80bcd5c)
	movs	r0, #163	@ 0xa3
	adds	r2, r7, r6
	movs	r3, #252	@ 0xfc
	lsls	r0, r0, #1
	strb	r3, [r2, #0]
	adds	r2, r7, r0
	movs	r3, #5
	strb	r3, [r2, #0]
	mov	r0, sl
	bl	Func_08077010
	ldr	r3, [sp, #16]
	adds	r3, #64	@ 0x40
	ldrh	r1, [r3, #0]
	adds	r3, r7, #0
	adds	r3, #64	@ 0x40
	ldrh	r3, [r3, #0]
	movs	r0, #1
	subs	r1, r1, r3
	bl	Func_080bbabc
	ldr	r1, [pc, #728]	@ (0x80bcd64)
	bl	.L_080bd236
	ldr	r1, [pc, #728]	@ (0x80bcd68)
	adds	r2, r7, r1
	ldrb	r3, [r2, #0]
	subs	r3, #1
	strb	r3, [r2, #0]
	movs	r4, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r4, r4
	cmp	r3, r4
	bge.n	.L_080bcaa8
	movs	r3, #252	@ 0xfc
	strb	r3, [r2, #0]
.L_080bcaa8:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L_080bcab4
	movs	r3, #4
	strb	r3, [r2, #0]
.L_080bcab4:
	mov	r0, sl
	bl	Func_08077010
	ldr	r5, [sp, #16]
	ldrh	r3, [r7, #60]	@ 0x3c
	ldrh	r1, [r5, #60]	@ 0x3c
	movs	r0, #1
	subs	r1, r1, r3
	movs	r6, #153	@ 0x99
	bl	Func_080bbabc
	lsls	r6, r6, #1
	ldr	r1, [pc, #668]	@ (0x80bcd6c)
	movs	r0, #4
	bl	Func_080bbabc
	adds	r2, r7, r6
	b.n	.L_080bcfee
	ldr	r0, [pc, #652]	@ (0x80bcd68)
	adds	r2, r7, r0
	ldrb	r3, [r2, #0]
	subs	r3, #2
	strb	r3, [r2, #0]
	movs	r1, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r1, r1
	cmp	r3, r1
	bge.n	.L_080bcaf2
	movs	r3, #252	@ 0xfc
	strb	r3, [r2, #0]
.L_080bcaf2:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L_080bcafe
	movs	r3, #4
	strb	r3, [r2, #0]
.L_080bcafe:
	mov	r0, sl
	bl	Func_08077010
	ldr	r2, [sp, #16]
	ldrh	r3, [r7, #60]	@ 0x3c
	ldrh	r1, [r2, #60]	@ 0x3c
	movs	r0, #1
	subs	r1, r1, r3
	bl	Func_080bbabc
	ldr	r1, [pc, #600]	@ (0x80bcd6c)
	movs	r0, #4
	bl	Func_080bbabc
	movs	r3, #153	@ 0x99
	lsls	r3, r3, #1
	adds	r2, r7, r3
	b.n	.L_080bcfee
	ldr	r4, [pc, #580]	@ (0x80bcd68)
	adds	r2, r7, r4
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	movs	r5, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r5, r5
	cmp	r3, r5
	bge.n	.L_080bcb3c
	movs	r3, #252	@ 0xfc
	strb	r3, [r2, #0]
.L_080bcb3c:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L_080bcb48
	movs	r3, #4
	strb	r3, [r2, #0]
.L_080bcb48:
	mov	r0, sl
	bl	Func_08077010
	ldr	r6, [sp, #16]
	ldrh	r1, [r7, #60]	@ 0x3c
	ldrh	r3, [r6, #60]	@ 0x3c
	movs	r0, #1
	subs	r1, r1, r3
	bl	Func_080bbabc
	ldr	r1, [pc, #528]	@ (0x80bcd70)
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #153	@ 0x99
	b.n	.L_080bcfea
	ldr	r1, [pc, #508]	@ (0x80bcd68)
	adds	r2, r7, r1
	ldrb	r3, [r2, #0]
	adds	r3, #2
	strb	r3, [r2, #0]
	movs	r4, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r4, r4
	cmp	r3, r4
	bge.n	.L_080bcb82
	movs	r3, #252	@ 0xfc
	strb	r3, [r2, #0]
.L_080bcb82:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L_080bcb8e
	movs	r3, #4
	strb	r3, [r2, #0]
.L_080bcb8e:
	mov	r0, sl
	bl	Func_08077010
	ldr	r5, [sp, #16]
	ldrh	r1, [r7, #60]	@ 0x3c
	ldrh	r3, [r5, #60]	@ 0x3c
	movs	r0, #1
	subs	r1, r1, r3
	movs	r6, #153	@ 0x99
	bl	Func_080bbabc
	lsls	r6, r6, #1
	ldr	r1, [pc, #456]	@ (0x80bcd70)
	movs	r0, #4
	bl	Func_080bbabc
	adds	r2, r7, r6
	b.n	.L_080bcfee
	ldr	r0, [pc, #448]	@ (0x80bcd74)
	adds	r2, r7, r0
	ldrb	r3, [r2, #0]
	subs	r3, #1
	strb	r3, [r2, #0]
	movs	r1, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r1, r1
	cmp	r3, r1
	bge.n	.L_080bcbcc
	movs	r3, #252	@ 0xfc
	strb	r3, [r2, #0]
.L_080bcbcc:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L_080bcbd8
	movs	r3, #4
	strb	r3, [r2, #0]
.L_080bcbd8:
	mov	r0, sl
	bl	Func_08077010
	ldr	r2, [sp, #16]
	ldrh	r3, [r7, #62]	@ 0x3e
	ldrh	r1, [r2, #62]	@ 0x3e
	movs	r0, #1
	subs	r1, r1, r3
	bl	Func_080bbabc
	ldr	r1, [pc, #392]	@ (0x80bcd78)
	movs	r0, #4
	bl	Func_080bbabc
	movs	r3, #154	@ 0x9a
	lsls	r3, r3, #1
	adds	r2, r7, r3
	b.n	.L_080bcfee
	ldr	r4, [pc, #372]	@ (0x80bcd74)
	adds	r2, r7, r4
	ldrb	r3, [r2, #0]
	subs	r3, #2
	strb	r3, [r2, #0]
	movs	r5, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r5, r5
	cmp	r3, r5
	bge.n	.L_080bcc16
	movs	r3, #252	@ 0xfc
	strb	r3, [r2, #0]
.L_080bcc16:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L_080bcc22
	movs	r3, #4
	strb	r3, [r2, #0]
.L_080bcc22:
	mov	r0, sl
	bl	Func_08077010
	ldr	r6, [sp, #16]
	ldrh	r3, [r7, #62]	@ 0x3e
	ldrh	r1, [r6, #62]	@ 0x3e
	movs	r0, #1
	subs	r1, r1, r3
	bl	Func_080bbabc
	ldr	r1, [pc, #320]	@ (0x80bcd78)
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #154	@ 0x9a
	b.n	.L_080bcfea
	ldr	r1, [pc, #304]	@ (0x80bcd74)
	adds	r2, r7, r1
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	movs	r4, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r4, r4
	cmp	r3, r4
	bge.n	.L_080bcc5c
	movs	r3, #252	@ 0xfc
	strb	r3, [r2, #0]
.L_080bcc5c:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L_080bcc68
	movs	r3, #4
	strb	r3, [r2, #0]
.L_080bcc68:
	mov	r0, sl
	bl	Func_08077010
	ldr	r5, [sp, #16]
	ldrh	r1, [r7, #62]	@ 0x3e
	ldrh	r3, [r5, #62]	@ 0x3e
	movs	r0, #1
	subs	r1, r1, r3
	movs	r6, #154	@ 0x9a
	bl	Func_080bbabc
	lsls	r6, r6, #1
	ldr	r1, [pc, #248]	@ (0x80bcd7c)
	movs	r0, #4
	bl	Func_080bbabc
	adds	r2, r7, r6
	b.n	.L_080bcfee
	ldr	r0, [pc, #228]	@ (0x80bcd74)
	adds	r2, r7, r0
	ldrb	r3, [r2, #0]
	adds	r3, #2
	strb	r3, [r2, #0]
	movs	r1, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r1, r1
	cmp	r3, r1
	bge.n	.L_080bcca6
	movs	r3, #252	@ 0xfc
	strb	r3, [r2, #0]
.L_080bcca6:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #4
	ble.n	.L_080bccb2
	movs	r3, #4
	strb	r3, [r2, #0]
.L_080bccb2:
	mov	r0, sl
	bl	Func_08077010
	ldr	r2, [sp, #16]
	ldrh	r1, [r7, #62]	@ 0x3e
	ldrh	r3, [r2, #62]	@ 0x3e
	movs	r0, #1
	subs	r1, r1, r3
	bl	Func_080bbabc
	ldr	r1, [pc, #180]	@ (0x80bcd7c)
	movs	r0, #4
	bl	Func_080bbabc
	movs	r3, #154	@ 0x9a
	lsls	r3, r3, #1
	adds	r2, r7, r3
	b.n	.L_080bcfee
	movs	r4, #56	@ 0x38
	ldrsh	r3, [r7, r4]
	cmp	r3, #0
	beq.n	.L_080bcce0
	b.n	.L_080bd2c0
.L_080bcce0:
	ldr	r1, [pc, #156]	@ (0x80bcd80)
	movs	r0, #4
	bl	Func_080bbabc
	ldrh	r3, [r7, #52]	@ 0x34
	mov	r0, sl
	strh	r3, [r7, #56]	@ 0x38
	bl	Func_08077128
	b.n	.L_080bd2c0
	movs	r5, #56	@ 0x38
	ldrsh	r3, [r7, r5]
	cmp	r3, #0
	beq.n	.L_080bccfe
	b.n	.L_080bd2c0
.L_080bccfe:
	ldr	r1, [pc, #128]	@ (0x80bcd80)
	movs	r0, #4
	bl	Func_080bbabc
	ldrh	r3, [r7, #52]	@ 0x34
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	lsrs	r3, r3, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	strh	r2, [r7, #56]	@ 0x38
	b.n	.L_080bcfd8
	movs	r6, #56	@ 0x38
	ldrsh	r3, [r7, r6]
	cmp	r3, #0
	beq.n	.L_080bcd20
	b.n	.L_080bd2c0
.L_080bcd20:
	ldr	r1, [pc, #92]	@ (0x80bcd80)
	movs	r0, #4
	bl	Func_080bbabc
	movs	r1, #52	@ 0x34
	ldrsh	r0, [r7, r1]
	movs	r1, #10
	lsls	r0, r0, #3
	bl	Func_080022ec
	strh	r0, [r7, #56]	@ 0x38
	b.n	.L_080bcfd8
	ldr	r2, [pc, #72]	@ (0x80bcd84)
	adds	r5, r7, r2
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	beq.n	.L_080bcd4c
	ldr	r1, [pc, #64]	@ (0x80bcd88)
	movs	r0, #4
	bl	Func_080bbabc
.L_080bcd4c:
	movs	r3, #0
	strb	r3, [r5, #0]
	b.n	.L_080bd2c0
	movs	r0, r0
	.4byte 0x00000821
	.4byte 0x0000081e
	.4byte 0x00000147
	.4byte 0x00000877
	.4byte 0x00000878
	.4byte 0x00000133
	.4byte 0x00000860
	.4byte 0x00000861
	.4byte 0x00000135
	.4byte 0x00000862
	.4byte 0x00000863
	.4byte 0x00000864
	.4byte 0x00000131
	.2byte 0x0884
	.2byte 0x0000
	ldr	r3, [pc, #816]	@ (0x80bd0c0)
	adds	r2, r7, r3
	ldrb	r3, [r2, #0]
	subs	r3, #1
	strb	r3, [r2, #0]
	movs	r4, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r4, r4
	cmp	r3, r4
	bge.n	.L_080bcda6
	movs	r3, #252	@ 0xfc
	strb	r3, [r2, #0]
.L_080bcda6:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	ldrb	r1, [r2, #0]
	cmp	r3, #4
	ble.n	.L_080bcdb6
	movs	r3, #4
	strb	r3, [r2, #0]
	movs	r1, #4
.L_080bcdb6:
	ldr	r5, [sp, #16]
	ldr	r6, [pc, #772]	@ (0x80bd0c0)
	adds	r3, r5, r6
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	lsls	r3, r1, #24
	asrs	r3, r3, #24
	subs	r2, r2, r3
	lsls	r1, r2, #2
	adds	r1, r1, r2
	lsls	r1, r1, #2
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #752]	@ (0x80bd0c4)
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #155	@ 0x9b
	b.n	.L_080bcfea
	ldr	r1, [pc, #736]	@ (0x80bd0c0)
	adds	r2, r7, r1
	ldrb	r3, [r2, #0]
	subs	r3, #2
	strb	r3, [r2, #0]
	movs	r4, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r4, r4
	cmp	r3, r4
	bge.n	.L_080bcdf8
	movs	r3, #252	@ 0xfc
	strb	r3, [r2, #0]
.L_080bcdf8:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	ldrb	r1, [r2, #0]
	cmp	r3, #4
	ble.n	.L_080bce08
	movs	r3, #4
	strb	r3, [r2, #0]
	movs	r1, #4
.L_080bce08:
	ldr	r5, [sp, #16]
	ldr	r6, [pc, #692]	@ (0x80bd0c0)
	adds	r3, r5, r6
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	lsls	r3, r1, #24
	asrs	r3, r3, #24
	subs	r2, r2, r3
	lsls	r1, r2, #2
	adds	r1, r1, r2
	lsls	r1, r1, #2
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #668]	@ (0x80bd0c4)
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #155	@ 0x9b
	b.n	.L_080bcfea
	ldr	r1, [pc, #652]	@ (0x80bd0c0)
	adds	r2, r7, r1
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	movs	r4, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r4, r4
	cmp	r3, r4
	bge.n	.L_080bce4a
	movs	r3, #252	@ 0xfc
	strb	r3, [r2, #0]
.L_080bce4a:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	ldrb	r1, [r2, #0]
	cmp	r3, #4
	ble.n	.L_080bce5a
	movs	r3, #4
	strb	r3, [r2, #0]
	movs	r1, #4
.L_080bce5a:
	ldr	r5, [sp, #16]
	ldr	r6, [pc, #608]	@ (0x80bd0c0)
	adds	r2, r5, r6
	ldrb	r2, [r2, #0]
	lsls	r2, r2, #24
	asrs	r2, r2, #24
	lsls	r3, r1, #24
	asrs	r3, r3, #24
	subs	r3, r3, r2
	lsls	r1, r3, #2
	adds	r1, r1, r3
	lsls	r1, r1, #2
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #588]	@ (0x80bd0c8)
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #155	@ 0x9b
	b.n	.L_080bcfea
	ldr	r1, [pc, #568]	@ (0x80bd0c0)
	adds	r2, r7, r1
	ldrb	r3, [r2, #0]
	adds	r3, #2
	strb	r3, [r2, #0]
	movs	r4, #4
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	negs	r4, r4
	cmp	r3, r4
	bge.n	.L_080bce9e
	movs	r3, #252	@ 0xfc
	strb	r3, [r2, #0]
.L_080bce9e:
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	ldrb	r1, [r2, #0]
	cmp	r3, #4
	ble.n	.L_080bceae
	movs	r3, #4
	strb	r3, [r2, #0]
	movs	r1, #4
.L_080bceae:
	ldr	r5, [sp, #16]
	ldr	r6, [pc, #524]	@ (0x80bd0c0)
	adds	r2, r5, r6
	ldrb	r2, [r2, #0]
	lsls	r2, r2, #24
	asrs	r2, r2, #24
	lsls	r3, r1, #24
	asrs	r3, r3, #24
	subs	r3, r3, r2
	lsls	r1, r3, #2
	adds	r1, r1, r3
	lsls	r1, r1, #2
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #504]	@ (0x80bd0c8)
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #155	@ 0x9b
	b.n	.L_080bcfea
	ldr	r1, [pc, #496]	@ (0x80bd0cc)
	adds	r5, r7, r1
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	beq.n	.L_080bcee6
	b.n	.L_080bd2c0
.L_080bcee6:
	ldr	r1, [pc, #488]	@ (0x80bd0d0)
	movs	r0, #4
	bl	Func_080bbabc
	movs	r3, #1
	strb	r3, [r5, #0]
	b.n	.L_080bd2c0
	ldr	r2, [pc, #468]	@ (0x80bd0cc)
	adds	r5, r7, r2
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	cmp	r3, #1
	ble.n	.L_080bcf02
	b.n	.L_080bd2c0
.L_080bcf02:
	ldr	r1, [pc, #464]	@ (0x80bd0d4)
	movs	r0, #4
	bl	Func_080bbabc
	movs	r3, #2
	strb	r3, [r5, #0]
	b.n	.L_080bd2c0
	ldr	r1, [pc, #452]	@ (0x80bd0d8)
	movs	r0, #4
	bl	Func_080bbabc
	movs	r3, #156	@ 0x9c
	lsls	r3, r3, #1
	adds	r2, r7, r3
	b.n	.L_080bcfee
	ldr	r1, [pc, #440]	@ (0x80bd0dc)
	movs	r0, #4
	bl	Func_080bbabc
	ldr	r4, [pc, #436]	@ (0x80bd0e0)
	movs	r3, #7
	adds	r2, r7, r4
	strb	r3, [r2, #0]
	b.n	.L_080bd2c0
	movs	r5, #157	@ 0x9d
	ldr	r1, [pc, #428]	@ (0x80bd0e4)
	movs	r0, #4
	lsls	r5, r5, #1
	bl	Func_080bbabc
	adds	r2, r7, r5
	b.n	.L_080bcfee
	ldr	r6, [pc, #420]	@ (0x80bd0e8)
	ldr	r1, [pc, #420]	@ (0x80bd0ec)
	movs	r0, #4
	bl	Func_080bbabc
	adds	r2, r7, r6
	b.n	.L_080bcfee
	ldr	r1, [pc, #412]	@ (0x80bd0f0)
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #158	@ 0x9e
	b.n	.L_080bcfea
	mov	r1, sl
	cmp	r1, #7
	bhi.n	.L_080bcf6c
	ldr	r1, [pc, #400]	@ (0x80bd0f4)
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L_080bcf74
.L_080bcf6c:
	ldr	r1, [pc, #392]	@ (0x80bd0f8)
	movs	r0, #4
	bl	Func_080bbabc
.L_080bcf74:
	ldr	r2, [pc, #388]	@ (0x80bd0fc)
	adds	r1, r7, r2
	ldrb	r2, [r1, #0]
	movs	r3, #7
	b.n	.L_080bcf9e
	mov	r3, sl
	cmp	r3, #7
	bhi.n	.L_080bcf8e
	ldr	r1, [pc, #364]	@ (0x80bd0f4)
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L_080bcf96
.L_080bcf8e:
	ldr	r1, [pc, #360]	@ (0x80bd0f8)
	movs	r0, #4
	bl	Func_080bbabc
.L_080bcf96:
	ldr	r4, [pc, #356]	@ (0x80bd0fc)
	adds	r1, r7, r4
	ldrb	r2, [r1, #0]
	movs	r3, #16
.L_080bcf9e:
	orrs	r3, r2
	strb	r3, [r1, #0]
	b.n	.L_080bd2c0
	movs	r5, #149	@ 0x95
	movs	r0, #9
	mov	r1, sl
	lsls	r5, r5, #1
	bl	Func_080bbabc
	adds	r3, r7, r5
	ldrb	r3, [r3, #0]
	cmp	r3, #2
	bne.n	.L_080bcfbc
	ldr	r1, [pc, #324]	@ (0x80bd100)
	b.n	.L_080bcfc4
.L_080bcfbc:
	ldr	r6, [sp, #64]	@ 0x40
	cmp	r6, #219	@ 0xdb
	bne.n	.L_080bcfcc
	ldr	r1, [pc, #320]	@ (0x80bd104)
.L_080bcfc4:
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L_080bcfd4
.L_080bcfcc:
	ldr	r1, [pc, #312]	@ (0x80bd108)
	movs	r0, #4
	bl	Func_080bbabc
.L_080bcfd4:
	movs	r3, #0
	strh	r3, [r7, #56]	@ 0x38
.L_080bcfd8:
	mov	r0, sl
	bl	Func_08077128
	b.n	.L_080bd2c0
	ldr	r1, [pc, #296]	@ (0x80bd10c)
	movs	r0, #4
	bl	Func_080bbabc
	movs	r0, #159	@ 0x9f
.L_080bcfea:
	lsls	r0, r0, #1
	adds	r2, r7, r0
.L_080bcfee:
	movs	r3, #7
	strb	r3, [r2, #0]
	b.n	.L_080bd2c0
	ldr	r1, [pc, #280]	@ (0x80bd110)
	movs	r0, #4
	bl	Func_080bbabc
	ldr	r1, [pc, #276]	@ (0x80bd114)
	movs	r3, #7
	adds	r2, r7, r1
	strb	r3, [r2, #0]
	b.n	.L_080bd2c0
	ldr	r3, [sp, #72]	@ 0x48
	ldr	r4, [sp, #76]	@ 0x4c
	movs	r2, #56	@ 0x38
	ldrsh	r6, [r3, r2]
	ldrb	r3, [r4, #3]
	adds	r2, r6, #0
	ldr	r5, [sp, #44]	@ 0x2c
	cmp	r3, #60	@ 0x3c
	bne.n	.L_080bd01e
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
.L_080bd01e:
	ldr	r1, [sp, #72]	@ 0x48
	movs	r0, #52	@ 0x34
	ldrsh	r3, [r1, r0]
	adds	r6, r6, r5
	cmp	r6, r3
	ble.n	.L_080bd02e
	adds	r6, r3, #0
	subs	r5, r6, r2
.L_080bd02e:
	movs	r1, #0
	movs	r0, #7
	bl	Func_080bbabc
	movs	r0, #0
	ldr	r1, [sp, #68]	@ 0x44
	bl	Func_080bbabc
	ldr	r4, [sp, #72]	@ 0x48
	movs	r2, #52	@ 0x34
	ldrsh	r3, [r4, r2]
	cmp	r6, r3
	bne.n	.L_080bd052
	ldr	r1, [pc, #204]	@ (0x80bd118)
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L_080bd062
.L_080bd052:
	adds	r1, r5, #0
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #192]	@ (0x80bd11c)
	movs	r0, #4
	bl	Func_080bbabc
.L_080bd062:
	ldr	r5, [sp, #72]	@ 0x48
	strh	r6, [r5, #56]	@ 0x38
	b.n	.L_080bd0b8
	ldr	r0, [sp, #72]	@ 0x48
	movs	r6, #58	@ 0x3a
	ldrsh	r5, [r0, r6]
	ldr	r6, [sp, #44]	@ 0x2c
	movs	r1, #54	@ 0x36
	ldrsh	r3, [r0, r1]
	adds	r2, r5, #0
	adds	r5, r5, r6
	cmp	r5, r3
	ble.n	.L_080bd080
	adds	r5, r3, #0
	subs	r6, r5, r2
.L_080bd080:
	movs	r1, #0
	movs	r0, #7
	bl	Func_080bbabc
	movs	r0, #0
	ldr	r1, [sp, #68]	@ 0x44
	bl	Func_080bbabc
	ldr	r4, [sp, #72]	@ 0x48
	movs	r2, #54	@ 0x36
	ldrsh	r3, [r4, r2]
	cmp	r5, r3
	bne.n	.L_080bd0a4
	ldr	r1, [pc, #132]	@ (0x80bd120)
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L_080bd0b4
.L_080bd0a4:
	adds	r1, r6, #0
	movs	r0, #1
	bl	Func_080bbabc
	ldr	r1, [pc, #116]	@ (0x80bd124)
	movs	r0, #4
	bl	Func_080bbabc
.L_080bd0b4:
	ldr	r6, [sp, #72]	@ 0x48
	strh	r5, [r6, #58]	@ 0x3a
.L_080bd0b8:
	ldr	r0, [sp, #68]	@ 0x44
	bl	Func_08077128
	b.n	.L_080bd2c0
	.4byte 0x00000137
	.4byte 0x00000865
	.4byte 0x00000866
	.4byte 0x00000131
	.4byte 0x00000867
	.4byte 0x00000874
	.4byte 0x00000868
	.4byte 0x00000869
	.4byte 0x00000139
	.4byte 0x0000086a
	.4byte 0x0000013b
	.4byte 0x0000086b
	.4byte 0x0000086c
	.4byte 0x0000086d
	.4byte 0x00000876
	.4byte 0x0000013d
	.4byte 0x0000084f
	.4byte 0x00000850
	.4byte 0x0000084c
	.4byte 0x0000086f
	.4byte 0x00000870
	.4byte 0x0000013f
	.4byte 0x00000820
	.4byte 0x0000081d
	.4byte 0x00000821
	.2byte 0x081e
	.2byte 0x0000
	ldr	r0, [sp, #44]	@ 0x2c
	movs	r1, #10
	bl	Func_080022ec
	adds	r5, r0, #0
	movs	r0, #58	@ 0x3a
	ldrsh	r3, [r7, r0]
	cmp	r3, r5
	bge.n	.L_080bd13c
	adds	r5, r3, #0
.L_080bd13c:
	ldr	r3, [sp, #72]	@ 0x48
	ldr	r6, [sp, #72]	@ 0x48
	movs	r2, #58	@ 0x3a
	ldrsh	r1, [r3, r2]
	movs	r4, #54	@ 0x36
	ldrsh	r2, [r6, r4]
	adds	r3, r1, r5
	cmp	r3, r2
	ble.n	.L_080bd150
	subs	r5, r2, r1
.L_080bd150:
	cmp	r5, #0
	bne.n	.L_080bd156
	b.n	.L_080bd2c0
.L_080bd156:
	movs	r0, #1
	adds	r1, r5, #0
	bl	Func_080bbabc
	mov	r0, sl
	cmp	r0, #7
	bhi.n	.L_080bd16e
	ldr	r1, [pc, #520]	@ (0x80bd370)
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L_080bd176
.L_080bd16e:
	ldr	r1, [pc, #516]	@ (0x80bd374)
	movs	r0, #4
	bl	Func_080bbabc
.L_080bd176:
	ldr	r0, [sp, #68]	@ 0x44
	adds	r1, r5, #0
	bl	Func_08077120
	b.n	.L_080bd2c0
	ldr	r1, [pc, #500]	@ (0x80bd378)
	adds	r2, r7, r1
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	ble.n	.L_080bd198
	movs	r4, #153	@ 0x99
	movs	r3, #0
	lsls	r4, r4, #1
	strb	r3, [r2, #0]
	adds	r2, r7, r4
	strb	r3, [r2, #0]
.L_080bd198:
	ldr	r5, [pc, #480]	@ (0x80bd37c)
	adds	r1, r7, r5
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	ble.n	.L_080bd1b0
	movs	r6, #154	@ 0x9a
	lsls	r6, r6, #1
	movs	r2, #0
	adds	r3, r7, r6
	strb	r2, [r1, #0]
	strb	r2, [r3, #0]
.L_080bd1b0:
	ldr	r0, [pc, #460]	@ (0x80bd380)
	adds	r2, r7, r0
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	ble.n	.L_080bd1c8
	movs	r1, #155	@ 0x9b
	movs	r3, #0
	lsls	r1, r1, #1
	strb	r3, [r2, #0]
	adds	r2, r7, r1
	strb	r3, [r2, #0]
.L_080bd1c8:
	ldr	r3, [pc, #440]	@ (0x80bd384)
	adds	r2, r7, r3
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	ble.n	.L_080bd1d8
	movs	r3, #0
	strb	r3, [r2, #0]
.L_080bd1d8:
	movs	r4, #150	@ 0x96
	lsls	r4, r4, #1
	ldr	r5, [pc, #424]	@ (0x80bd388)
	movs	r2, #0
	adds	r3, r7, r4
	movs	r6, #151	@ 0x97
	strb	r2, [r3, #0]
	ldr	r0, [pc, #420]	@ (0x80bd38c)
	adds	r3, r7, r5
	lsls	r6, r6, #1
	strb	r2, [r3, #0]
	adds	r3, r7, r6
	strb	r2, [r3, #0]
	adds	r3, r7, r0
	strb	r2, [r3, #0]
	ldr	r1, [pc, #408]	@ (0x80bd390)
	b.n	.L_080bd236
	ldr	r1, [pc, #408]	@ (0x80bd394)
	movs	r0, #4
	bl	Func_080bbabc
	movs	r1, #160	@ 0xa0
	lsls	r1, r1, #1
	adds	r2, r7, r1
	b.n	.L_080bd296
	ldr	r2, [pc, #396]	@ (0x80bd398)
	adds	r5, r7, r2
	ldrb	r3, [r5, #0]
	adds	r2, r3, #0
	cmp	r2, #0
	bne.n	.L_080bd224
	ldr	r1, [pc, #388]	@ (0x80bd39c)
	movs	r0, #4
	bl	Func_080bbabc
	movs	r3, #7
	strb	r3, [r5, #0]
	b.n	.L_080bd2c0
.L_080bd224:
	cmp	r2, #1
	bls.n	.L_080bd2c0
	adds	r3, #255	@ 0xff
	strb	r3, [r5, #0]
	movs	r0, #1
	ldrb	r1, [r5, #0]
	bl	Func_080bbabc
	ldr	r1, [pc, #360]	@ (0x80bd3a0)
.L_080bd236:
	movs	r0, #4
	bl	Func_080bbabc
	b.n	.L_080bd2c0
	ldr	r1, [pc, #356]	@ (0x80bd3a4)
	movs	r0, #4
	bl	Func_080bbabc
	movs	r3, #162	@ 0xa2
	lsls	r3, r3, #1
	adds	r2, r7, r3
	b.n	.L_080bd2b0
	ldr	r1, [pc, #344]	@ (0x80bd3a8)
	movs	r0, #4
	bl	Func_080bbabc
	movs	r4, #164	@ 0xa4
	lsls	r4, r4, #1
	adds	r2, r7, r4
	movs	r3, #1
	mov	r5, sl
	strb	r3, [r2, #0]
	cmp	r5, #7
	bhi.n	.L_080bd2c0
	ldr	r3, [sp, #56]	@ 0x38
	adds	r3, #67	@ 0x43
	ldrb	r2, [r3, #0]
	movs	r1, #2
	orrs	r2, r1
	strb	r2, [r3, #0]
	b.n	.L_080bd2c0
	ldr	r6, [pc, #308]	@ (0x80bd3ac)
	ldr	r1, [pc, #312]	@ (0x80bd3b0)
	movs	r0, #4
	bl	Func_080bbabc
	adds	r2, r7, r6
	b.n	.L_080bd296
	ldr	r1, [pc, #304]	@ (0x80bd3b4)
	movs	r0, #4
	bl	Func_080bbabc
	ldr	r0, [pc, #300]	@ (0x80bd3b8)
	adds	r2, r7, r0
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	bgt.n	.L_080bd2c0
.L_080bd296:
	movs	r3, #1
	strb	r3, [r2, #0]
	b.n	.L_080bd2c0
	ldr	r1, [pc, #284]	@ (0x80bd3bc)
	movs	r0, #4
	bl	Func_080bbabc
	ldr	r1, [pc, #272]	@ (0x80bd3b8)
	adds	r2, r7, r1
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #1
	bgt.n	.L_080bd2c0
.L_080bd2b0:
	movs	r3, #2
	strb	r3, [r2, #0]
	b.n	.L_080bd2c0
	movs	r1, #1
	negs	r1, r1
	movs	r0, #4
	bl	Func_080bbabc
.L_080bd2c0:
	movs	r0, #7
	movs	r1, #0
	bl	Func_080bbabc
	movs	r2, #56	@ 0x38
	ldrsh	r3, [r7, r2]
	cmp	r3, #0
	beq.n	.L_080bd304
	movs	r3, #158	@ 0x9e
	lsls	r3, r3, #1
	adds	r5, r7, r3
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080bd304
	cmp	r3, #6
	bhi.n	.L_080bd304
	ldr	r4, [sp, #44]	@ 0x2c
	cmp	r4, #0
	ble.n	.L_080bd304
	bl	Func_080771a0
	movs	r3, #3
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L_080bd304
	strb	r0, [r5, #0]
	mov	r1, sl
	movs	r0, #0
	bl	Func_080bbabc
	ldr	r1, [pc, #192]	@ (0x80bd3c0)
	movs	r0, #4
	bl	Func_080bbabc
.L_080bd304:
	ldr	r0, [sp, #16]
	bl	Func_08002df0
	mov	r0, sl
	bl	Func_08077010
	ldr	r3, [pc, #176]	@ (0x80bd3c4)
	ldr	r3, [r3, #0]
	adds	r3, #65	@ 0x41
	ldrb	r0, [r3, #0]
	bl	Func_08015130
	movs	r5, #56	@ 0x38
	ldrsh	r3, [r7, r5]
	cmp	r3, #0
	beq.n	.L_080bd32c
	movs	r0, #11
	mov	r1, sl
	bl	Func_080bbabc
.L_080bd32c:
	ldr	r6, [sp, #72]	@ 0x48
	movs	r0, #160	@ 0xa0
	lsls	r0, r0, #1
	adds	r3, r6, r0
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080bd35c
	bl	Func_080771a0
	movs	r3, #3
	ands	r0, r3
	cmp	r0, #0
	bne.n	.L_080bd35c
	ldr	r1, [sp, #44]	@ 0x2c
	cmp	r1, #0
	ble.n	.L_080bd35c
	asrs	r0, r1, #2
	cmp	r0, #0
	bne.n	.L_080bd354
	movs	r0, #1
.L_080bd354:
	ldr	r2, [sp, #80]	@ 0x50
	ldr	r3, [r2, #96]	@ 0x60
	adds	r3, r3, r0
	str	r3, [r2, #96]	@ 0x60
.L_080bd35c:
	add	sp, #100	@ 0x64
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x0000085f
	.4byte 0x0000085e
	.4byte 0x00000133
	.4byte 0x00000135
	.4byte 0x00000137
	.4byte 0x00000147
	.4byte 0x0000012d
	.4byte 0x0000012f
	.4byte 0x00000896
	.4byte 0x00000872
	.4byte 0x00000141
	.4byte 0x00000873
	.4byte 0x00000875
	.4byte 0x0000087d
	.4byte 0x0000087e
	.4byte 0x00000145
	.4byte 0x0000087f
	.4byte 0x00000881
	.4byte 0x0000012b
	.4byte 0x00000882
	.4byte 0x00000883
	.4byte 0x03001e74

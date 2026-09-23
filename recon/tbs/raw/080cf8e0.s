.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_08005340, 0x08005340
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080072f8, 0x080072f8
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e155c, 0x080e155c
	.set sub_080e38b8, 0x080e38b8
	.set sub_080e3944, 0x080e3944
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Func_080cf8e0
	.thumb_func
Func_080cf8e0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #80
	ldr	r2, [pc, #100]
	str	r0, [sp, #56]
	adds	r3, r2, #0
	ldmia	r3!, {r0}
	ldr	r3, [r3, #0]
	str	r3, [sp, #52]
	subs	r2, #108
	ldr	r3, [pc, #92]
	mov	fp, r0
	ldr	r2, [r2, #0]
	mov	r8, r1
	ldr	r1, [sp, #56]
	add	r3, fp
	str	r2, [sp, #36]
	movs	r0, #1
	str	r1, [r3, #0]
	bl	sub_080cd594
	ldr	r3, [pc, #60]
	ldr	r2, [pc, #72]
	ldr	r7, [pc, #72]
	strh	r3, [r2, #0]
	adds	r0, r7, #0
	bl	sub_08002f40
	adds	r6, r0, #0
	movs	r0, #160
	movs	r2, #128
	adds	r1, r6, #0
	ldr	r5, [pc, #60]
	adds	r6, #128
	lsls	r0, r0, #19
	bl	sub_080072f8
	mov	r1, fp
	adds	r0, r6, #0
	bl	sub_08005340
	ldr	r0, [pc, #44]
	bl	sub_08002f40
	adds	r6, r0, #0
	movs	r0, #160
	adds	r1, r6, #0
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f8
	b.n	.L_080cf970
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000052
	.4byte 0x000000bf
	.4byte 0x03001388
	.2byte 0x009e
	.2byte 0x0000
.L_080cf970:
	movs	r1, #250
	adds	r6, #128
	lsls	r1, r1, #6
	add	r1, fp
	adds	r0, r6, #0
	bl	sub_08005340
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_080cf988
	ldr	r0, [pc, #712]
	b.n	.L_080cf9b8
.L_080cf988:
	mov	r3, r8
	cmp	r3, #1
	bne.n	.L_080cf992
	ldr	r0, [pc, #708]
	b.n	.L_080cf9b8
.L_080cf992:
	mov	r4, r8
	cmp	r4, #2
	bne.n	.L_080cf99c
	ldr	r0, [pc, #700]
	b.n	.L_080cf9b8
.L_080cf99c:
	mov	r0, r8
	cmp	r0, #3
	beq.n	.L_080cf9b6
	mov	r1, r8
	cmp	r1, #4
	bne.n	.L_080cf9ac
	adds	r0, r7, #0
	b.n	.L_080cf9b8
.L_080cf9ac:
	mov	r2, r8
	cmp	r2, #6
	bne.n	.L_080cf9b6
	ldr	r0, [pc, #680]
	b.n	.L_080cf9b8
.L_080cf9b6:
	ldr	r0, [pc, #680]
.L_080cf9b8:
	bl	sub_08002f40
	adds	r6, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #672]
	adds	r1, r6, #0
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	movs	r3, #0
	mov	sl, r3
	movs	r1, #1
	movs	r2, #128
	ldr	r3, [pc, #656]
	negs	r1, r1
	lsls	r2, r2, #3
.L_080cf9da:
	movs	r4, #1
	add	sl, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	sl, r2
	bne.n	.L_080cf9da
	ldr	r5, [pc, #644]
	add	r5, fp
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5098
	ldr	r6, [r0, #0]
	movs	r0, #0
	str	r0, [sp, #44]
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	movs	r4, #128
	lsls	r4, r4, #11
	cmp	r3, #0
	beq.n	.L_080cfa70
	ldr	r5, [pc, #612]
	mov	r9, r0
	add	r5, fp
	movs	r7, #0
.L_080cfa0c:
	ldr	r1, [sp, #44]
	ldr	r2, [r5, #0]
	lsls	r3, r1, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	str	r4, [sp, #8]
	bl	sub_080b5098
	ldr	r3, [pc, #592]
	movs	r2, #0
	ldr	r0, [r0, #0]
	ldr	r4, [sp, #8]
	mov	sl, r2
	adds	r1, r7, r3
.L_080cfa28:
	ldr	r3, [r6, #8]
	str	r4, [r1, #4]
	str	r3, [r1, #0]
	ldr	r3, [r6, #16]
	str	r3, [r1, #8]
	ldr	r2, [r6, #8]
	ldr	r3, [r0, #8]
	subs	r3, r3, r2
	movs	r2, #128
	asrs	r3, r3, #4
	lsls	r2, r2, #11
	str	r3, [r1, #12]
	str	r2, [r1, #16]
	ldr	r2, [r6, #16]
	ldr	r3, [r0, #16]
	subs	r3, r3, r2
	asrs	r3, r3, #4
	movs	r2, #1
	str	r3, [r1, #20]
	add	sl, r2
	mov	r3, r9
	str	r3, [r1, #24]
	mov	r3, sl
	adds	r1, #28
	cmp	r3, #16
	bne.n	.L_080cfa28
	ldr	r1, [sp, #44]
	adds	r1, #1
	str	r1, [sp, #44]
	ldr	r3, [r5, #0]
	movs	r0, #224
	ldr	r3, [r3, #20]
	lsls	r0, r0, #1
	adds	r7, r7, r0
	cmp	r1, r3
	bne.n	.L_080cfa0c
.L_080cfa70:
	movs	r2, #0
	mov	sl, r2
	movs	r1, #1
	movs	r2, #128
	ldr	r3, [pc, #504]
	negs	r1, r1
	lsls	r2, r2, #1
.L_080cfa7e:
	movs	r4, #1
	add	sl, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	sl, r2
	bne.n	.L_080cfa7e
	ldr	r0, [sp, #56]
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_080cfae4
	movs	r1, #7
	movs	r3, #3
	movs	r6, #2
	movs	r0, #46
	movs	r2, #7
	str	r6, [sp, #0]
	bl	sub_080ed408
	ldr	r5, [pc, #468]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	mov	r1, r8
	lsls	r1, r1, #1
	str	r3, [sp, #60]
	str	r1, [sp, #28]
	ldr	r3, [pc, #456]
	ldrsb	r3, [r3, r1]
	cmp	r3, #0
	bne.n	.L_080cfac6
	movs	r3, #3
	movs	r2, #7
	movs	r0, #47
	movs	r1, #7
	str	r3, [sp, #0]
	b.n	.L_080cfb34
.L_080cfac6:
	movs	r3, #7
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	str	r6, [sp, #0]
	bl	sub_080ed408
	adds	r3, r5, #0
	adds	r3, #188
	ldr	r3, [r3, #0]
	mov	r4, sp
	adds	r4, #60
	str	r4, [sp, #32]
	str	r3, [r4, #4]
	b.n	.L_080cfb46
.L_080cfae4:
	movs	r3, #7
	movs	r6, #2
	movs	r0, #46
	movs	r1, #7
	movs	r2, #7
	str	r6, [sp, #0]
	bl	sub_080ed408
	ldr	r5, [pc, #384]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	mov	r0, r8
	lsls	r0, r0, #1
	str	r3, [sp, #60]
	str	r0, [sp, #28]
	ldr	r3, [pc, #372]
	ldrsb	r3, [r3, r0]
	cmp	r3, #0
	bne.n	.L_080cfb2a
	movs	r3, #3
	movs	r1, #7
	movs	r0, #47
	movs	r2, #7
	str	r3, [sp, #0]
	bl	sub_080ed408
	adds	r3, r5, #0
	adds	r3, #188
	ldr	r3, [r3, #0]
	mov	r1, sp
	adds	r1, #60
	str	r1, [sp, #32]
	str	r3, [r1, #4]
	b.n	.L_080cfb46
.L_080cfb2a:
	movs	r2, #7
	movs	r3, #3
	movs	r0, #47
	movs	r1, #7
	str	r6, [sp, #0]
.L_080cfb34:
	bl	sub_080ed408
	adds	r3, r5, #0
	adds	r3, #188
	ldr	r3, [r3, #0]
	mov	r2, sp
	adds	r2, #60
	str	r2, [sp, #32]
	str	r3, [r2, #4]
.L_080cfb46:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #300]
	movs	r3, #75
	add	r2, fp
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #296]
	lsls	r1, r1, #3
	bl	sub_080041d8
	ldr	r3, [pc, #280]
	ldr	r4, [sp, #28]
	ldrsb	r3, [r3, r4]
	cmp	r3, #0
	bne.n	.L_080cfb7a
	ldr	r3, [pc, #252]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #3
	adds	r3, #72
	b.n	.L_080cfb86
.L_080cfb7a:
	ldr	r3, [pc, #240]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #3
	adds	r3, #56
.L_080cfb86:
	str	r3, [sp, #40]
	movs	r0, #103
	bl	sub_080f9010
	ldr	r1, [sp, #40]
	movs	r0, #0
	str	r0, [sp, #48]
	cmp	r1, #0
	bne.n	.L_080cfb9a
	b.n	.L_080cfea6
.L_080cfb9a:
	ldr	r2, [sp, #36]
	adds	r2, #12
	str	r2, [sp, #20]
.L_080cfba0:
	bl	sub_080049ac
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #20]
	bl	sub_080051d8
	movs	r3, #0
	str	r3, [sp, #44]
	ldr	r3, [pc, #184]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L_080cfbbe
	b.n	.L_080cfd88
.L_080cfbbe:
	ldr	r4, [pc, #176]
	movs	r0, #0
	str	r4, [sp, #16]
	str	r0, [sp, #12]
.L_080cfbc6:
	ldr	r1, [sp, #48]
	ldr	r2, [sp, #12]
	cmp	r1, r2
	bge.n	.L_080cfbd0
	b.n	.L_080cfd64
.L_080cfbd0:
	adds	r3, r2, #0
	adds	r3, #17
	ldr	r7, [sp, #16]
	cmp	r1, r3
	bne.n	.L_080cfbfc
	ldr	r3, [pc, #144]
	ldr	r4, [sp, #44]
	add	r3, fp
	ldr	r2, [r3, #0]
	lsls	r3, r4, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	adds	r3, r4, #0
	bl	sub_080d6888
	movs	r0, #133
	bl	sub_080b50e8
.L_080cfbfc:
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #24]
	cmp	r3, #0
	bge.n	.L_080cfc06
	b.n	.L_080cfd64
.L_080cfc06:
	ldr	r3, [sp, #48]
	ldr	r4, [sp, #12]
	movs	r1, #3
	subs	r0, r3, r4
	bl	sub_080022ec
	adds	r5, r0, #0
	cmp	r5, #9
	ble.n	.L_080cfc1a
	movs	r5, #9
.L_080cfc1a:
	add	r6, sp, #68
	ldr	r0, [sp, #16]
	adds	r1, r6, #0
	bl	sub_080e3944
	ldr	r3, [r6, #0]
	asrs	r2, r3, #1
	str	r2, [r6, #0]
	cmp	r5, #4
	ble.n	.L_080cfc88
	lsls	r1, r5, #1
	ldr	r3, [r6, #4]
	movs	r0, #32
	adds	r1, r1, r5
	lsls	r1, r1, #8
	str	r0, [sp, #0]
	movs	r0, #24
	str	r0, [sp, #4]
	add	r1, fp
	subs	r2, #16
	subs	r3, #12
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	bl	sub_080072f4
	b.n	.L_080cfca6
	movs	r0, r0
	.4byte 0x0000009f
	.4byte 0x00000059
	.4byte 0x000000a0
	.4byte 0x0000008d
	.4byte 0x00000077
	.4byte 0x03001388
	.4byte 0x02010018
	.4byte 0x00007828
	.4byte 0x02010000
	.4byte 0x02011c18
	.4byte 0x03001e50
	.4byte 0x080ee0b6
	.4byte 0x00007784
	.2byte 0xd261
	.2byte 0x080c
.L_080cfc88:
	lsls	r1, r5, #1
	ldr	r3, [r6, #4]
	movs	r0, #24
	adds	r1, r1, r5
	lsls	r1, r1, #8
	str	r0, [sp, #0]
	movs	r0, #32
	str	r0, [sp, #4]
	add	r1, fp
	subs	r2, #12
	subs	r3, #16
	ldr	r4, [sp, #60]
	ldr	r0, [sp, #52]
	bl	sub_080072f4
.L_080cfca6:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	bne.n	.L_080cfcb6
	adds	r0, r7, #0
	movs	r1, #63
	ldr	r2, [pc, #540]
	bl	sub_080e38b8
.L_080cfcb6:
	ldr	r3, [r7, #4]
	cmp	r3, #0
	bge.n	.L_080cfd64
	movs	r3, #0
	str	r3, [r7, #4]
	movs	r3, #1
	str	r3, [r7, #24]
	ldr	r0, [pc, #524]
	ldr	r1, [sp, #28]
	ldrsb	r3, [r0, r1]
	movs	r2, #4
	mov	r9, r2
	cmp	r3, #0
	beq.n	.L_080cfcd6
	movs	r3, #16
	mov	r9, r3
.L_080cfcd6:
	movs	r4, #0
	mov	r0, r9
	mov	sl, r4
	cmp	r0, #0
	beq.n	.L_080cfd64
	ldr	r1, [sp, #44]
	lsls	r1, r1, #2
	movs	r2, #63
	str	r1, [sp, #24]
	mov	r8, r2
.L_080cfcea:
	ldr	r3, [sp, #24]
	lsls	r2, r3, #3
	add	r2, sl
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r4, [pc, #480]
	lsls	r3, r3, #2
	adds	r5, r3, r4
	ldr	r3, [r7, #0]
	str	r3, [r5, #0]
	ldr	r3, [r7, #4]
	str	r3, [r5, #4]
	ldr	r3, [r7, #8]
	str	r3, [r5, #8]
	ldr	r0, [pc, #460]
	ldr	r1, [sp, #28]
	ldrsb	r6, [r0, r1]
	cmp	r6, #0
	bne.n	.L_080cfd2e
	bl	sub_08004458
	mov	r2, r8
	ands	r0, r2
	subs	r0, #32
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	str	r6, [r5, #16]
	bl	sub_08004458
	mov	r3, r8
	ands	r0, r3
	subs	r0, #32
	lsls	r0, r0, #11
	b.n	.L_080cfd56
.L_080cfd2e:
	bl	sub_08004458
	mov	r4, r8
	ands	r0, r4
	subs	r0, #32
	lsls	r0, r0, #13
	str	r0, [r5, #12]
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r3, #32
	lsls	r3, r3, #12
	str	r3, [r5, #16]
	bl	sub_08004458
	mov	r1, r8
	ands	r0, r1
	subs	r0, #32
	lsls	r0, r0, #13
.L_080cfd56:
	str	r0, [r5, #20]
	movs	r2, #1
	movs	r3, #0
	add	sl, r2
	str	r3, [r5, #24]
	cmp	sl, r9
	bne.n	.L_080cfcea
.L_080cfd64:
	ldr	r3, [sp, #16]
	movs	r4, #224
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #44]
	lsls	r4, r4, #1
	adds	r3, r3, r4
	str	r3, [sp, #16]
	adds	r0, #8
	adds	r1, #1
	ldr	r3, [pc, #356]
	str	r0, [sp, #12]
	str	r1, [sp, #44]
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	cmp	r1, r3
	beq.n	.L_080cfd88
	b.n	.L_080cfbc6
.L_080cfd88:
	ldr	r3, [pc, #332]
	ldr	r4, [pc, #328]
	movs	r2, #0
	mov	sl, r2
	mov	r8, r3
	mov	r9, r4
.L_080cfd94:
	mov	r0, r8
	ldr	r3, [r0, #24]
	cmp	r3, #44
	bhi.n	.L_080cfe6e
	ldr	r3, [r0, #4]
	cmp	r3, #0
	blt.n	.L_080cfe6e
	add	r6, sp, #68
	adds	r1, r6, #0
	bl	sub_080e3944
	ldr	r3, [r6, #0]
	asrs	r2, r3, #1
	str	r2, [r6, #0]
	ldr	r4, [sp, #28]
	mov	r1, r9
	ldrsb	r3, [r1, r4]
	cmp	r3, #0
	bne.n	.L_080cfdee
	mov	r0, r8
	ldr	r3, [r0, #24]
	cmp	r3, #0
	bge.n	.L_080cfdc4
	adds	r3, #7
.L_080cfdc4:
	asrs	r3, r3, #3
	lsls	r1, r3, #3
	adds	r1, r1, r3
	lsls	r1, r1, #7
	movs	r3, #250
	lsls	r3, r3, #6
	movs	r0, #24
	add	r1, fp
	adds	r1, r1, r3
	ldr	r3, [r6, #4]
	str	r0, [sp, #0]
	movs	r0, #48
	str	r0, [sp, #4]
	ldr	r0, [sp, #32]
	subs	r2, #12
	ldr	r4, [r0, #4]
	subs	r3, #24
	ldr	r0, [sp, #52]
	bl	sub_080072f4
	b.n	.L_080cfe44
.L_080cfdee:
	mov	r1, r8
	ldr	r0, [r1, #24]
	movs	r1, #5
	bl	sub_080022ec
	movs	r1, #1
	mov	r3, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080cfe04
	adds	r0, #9
.L_080cfe04:
	ldr	r3, [sp, #56]
	mov	r4, r8
	ldr	r2, [r3, #4]
	ldr	r3, [r4, #12]
	cmp	r3, #0
	ble.n	.L_080cfe12
	eors	r2, r1
.L_080cfe12:
	lsls	r7, r2, #2
	ldr	r2, [pc, #200]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #200]
	movs	r2, #240
	ldrb	r5, [r3, r0]
	lsls	r2, r2, #5
	add	r1, fp
	adds	r1, r1, r2
	ldr	r2, [r6, #0]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #184]
	ldrb	r4, [r3, r0]
	ldr	r3, [r6, #4]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	ldr	r0, [sp, #32]
	str	r5, [sp, #0]
	str	r4, [sp, #4]
	ldr	r4, [r7, r0]
	ldr	r0, [sp, #52]
	bl	sub_080072f4
.L_080cfe44:
	ldr	r2, [sp, #28]
	mov	r1, r9
	ldrsb	r3, [r1, r2]
	cmp	r3, #0
	bne.n	.L_080cfe5c
	movs	r2, #128
	mov	r0, r8
	movs	r1, #62
	lsls	r2, r2, #4
	bl	sub_080e38b8
	b.n	.L_080cfe66
.L_080cfe5c:
	mov	r0, r8
	movs	r1, #62
	ldr	r2, [pc, #108]
	bl	sub_080e38b8
.L_080cfe66:
	mov	r4, r8
	ldr	r3, [r4, #24]
	adds	r3, #1
	str	r3, [r4, #24]
.L_080cfe6e:
	movs	r1, #1
	movs	r2, #128
	movs	r0, #28
	add	sl, r1
	lsls	r2, r2, #1
	add	r8, r0
	cmp	sl, r2
	bne.n	.L_080cfd94
	movs	r0, #2
	movs	r1, #2
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #96]
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [sp, #48]
	ldr	r4, [sp, #40]
	adds	r3, #1
	str	r3, [sp, #48]
	cmp	r3, r4
	beq.n	.L_080cfea6
	b.n	.L_080cfba0
.L_080cfea6:
	ldr	r0, [pc, #72]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #80
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0xffff8000
	.4byte 0x080ee0b6
	.4byte 0x02011c00
	.4byte 0x00007828
	.4byte 0x080ee0e8
	.4byte 0x080ee0c4
	.4byte 0x080ee0d6
	.4byte 0x00007824
	.4byte 0x080cd261

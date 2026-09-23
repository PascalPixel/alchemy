.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_08002304, 0x08002304
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_08009020, 0x08009020
	.set sub_08009038, 0x08009038
	.set sub_080b5088, 0x080b5088
	.set sub_080b5098, 0x080b5098
	.set sub_080b50d8, 0x080b50d8
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6750, 0x080d6750
	.set sub_080d6888, 0x080d6888
	.set sub_080dbb24, 0x080dbb24
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e3908, 0x080e3908
	.set sub_080e3944, 0x080e3944
	.set sub_080e6d3c, 0x080e6d3c
	.set sub_080e6eac, 0x080e6eac
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Func_080d6970
	.thumb_func
Func_080d6970:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #200]
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #100
	str	r3, [sp, #64]
	ldr	r3, [pc, #192]
	mov	r9, r1
	add	r3, r9
	str	r0, [r3, #0]
	ldr	r1, [pc, #188]
	movs	r0, #8
	movs	r2, #1
	bl	sub_080dbb24
	movs	r2, #0
	mov	sl, r2
	movs	r1, #128
	ldr	r2, [pc, #176]
	movs	r0, #127
	lsls	r1, r1, #3
.L_080d69a6:
	mov	r3, sl
	ands	r3, r0
	strb	r3, [r2, #0]
	movs	r3, #1
	add	sl, r3
	adds	r2, #1
	cmp	sl, r1
	bne.n	.L_080d69a6
	movs	r4, #0
	movs	r0, #127
	mov	sl, r4
	mov	r8, r0
	mov	fp, r4
.L_080d69c0:
	movs	r7, #0
	mov	r6, fp
.L_080d69c4:
	bl	sub_08004458
	mov	r1, r8
	adds	r5, r0, #0
	ands	r5, r1
	bl	sub_08004458
	mov	r2, r8
	ldr	r3, [pc, #124]
	ands	r0, r2
	adds	r0, r6, r0
	adds	r5, r6, r5
	adds	r0, r0, r3
	adds	r5, r5, r3
	ldrb	r2, [r0, #0]
	ldrb	r3, [r5, #0]
	adds	r7, #1
	strb	r3, [r0, #0]
	strb	r2, [r5, #0]
	cmp	r7, #128
	bne.n	.L_080d69c4
	movs	r0, #1
	add	sl, r0
	movs	r4, #128
	mov	r1, sl
	add	fp, r4
	cmp	r1, #8
	bne.n	.L_080d69c0
	movs	r0, #0
	bl	sub_080cd594
	ldr	r2, [pc, #84]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, #48
	strh	r3, [r2, #0]
	ldr	r0, [pc, #76]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r6, #1
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r6, [sp, #0]
	bl	sub_080ed408
	ldr	r5, [pc, #52]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r1, #7
	str	r3, [sp, #68]
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	str	r6, [sp, #0]
	b.n	.L_080d6a64
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x00000177
	.4byte 0x02010000
	.4byte 0x04000020
	.4byte 0x000000b2
	.2byte 0x1e50
	.2byte 0x0300
.L_080d6a64:
	bl	sub_080ed408
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r2, sp
	adds	r2, #68
	str	r2, [sp, #36]
	str	r3, [r2, #4]
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #752]
	add	r3, r9
	str	r6, [r3, #0]
	add	r2, r9
	movs	r3, #0
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #744]
	lsls	r1, r1, #3
	bl	sub_080041d8
	ldr	r3, [pc, #740]
	ldr	r2, [pc, #740]
	movs	r4, #90
	movs	r5, #225
	str	r3, [r2, #0]
	negs	r4, r4
	movs	r3, #0
	lsls	r5, r5, #7
	mov	sl, r3
	movs	r7, #7
	mov	r8, r4
	movs	r6, #0
	add	r5, r9
.L_080d6aa8:
	mov	r0, sl
	cmp	r0, #4
	bgt.n	.L_080d6aba
	str	r6, [r5, #0]
	bl	sub_08004458
	ands	r0, r7
	adds	r0, #104
	b.n	.L_080d6ac6
.L_080d6aba:
	mov	r1, r8
	str	r1, [r5, #0]
	bl	sub_08004458
	ands	r0, r7
	adds	r0, #108
.L_080d6ac6:
	str	r0, [r5, #4]
	bl	sub_08004458
	ands	r0, r7
	adds	r0, #4
	str	r0, [r5, #16]
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	adds	r3, #16
	str	r3, [r5, #24]
	movs	r3, #1
	add	sl, r3
	movs	r2, #20
	mov	r4, sl
	add	r8, r2
	adds	r6, #20
	adds	r5, #28
	cmp	r4, #16
	bne.n	.L_080d6aa8
	ldr	r5, [pc, #648]
	movs	r0, #0
	mov	sl, r0
	add	r5, r9
.L_080d6af8:
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	str	r3, [r5, #0]
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r3, #64
	str	r3, [r5, #4]
	bl	sub_08004458
	movs	r3, #7
	ands	r3, r0
	movs	r1, #1
	negs	r3, r3
	add	sl, r1
	subs	r3, #8
	mov	r2, sl
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #16
	bne.n	.L_080d6af8
	movs	r5, #232
	movs	r3, #0
	lsls	r5, r5, #7
	mov	sl, r3
	movs	r6, #0
	add	r5, r9
.L_080d6b36:
	movs	r3, #128
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	movs	r3, #128
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	bl	sub_08004458
	movs	r3, #255
	ands	r3, r0
	adds	r3, #200
	movs	r4, #1
	negs	r3, r3
	add	sl, r4
	lsls	r3, r3, #9
	mov	r0, sl
	str	r3, [r5, #12]
	str	r6, [r5, #16]
	str	r6, [r5, #24]
	adds	r5, #28
	cmp	r0, #16
	bne.n	.L_080d6b36
	ldr	r5, [pc, #540]
	mov	r1, r9
	ldr	r0, [r1, r5]
	bl	sub_080d6750
	ldr	r2, [pc, #532]
	movs	r3, #0
	str	r2, [sp, #56]
	str	r3, [sp, #52]
	str	r3, [sp, #60]
.L_080d6b76:
	ldr	r3, [pc, #528]
	ldr	r5, [r3, #0]
	ldr	r3, [pc, #528]
	ldr	r3, [r3, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d6ba4
	ldr	r4, [sp, #60]
	cmp	r4, #190
	ble.n	.L_080d6ba4
	ldr	r0, [pc, #512]
	cmp	r4, r0
	bgt.n	.L_080d6ba4
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r3, [pc, #508]
	ldr	r0, [sp, #64]
	bl	sub_080072f0
	movs	r1, #143
	lsls	r1, r1, #1
	str	r1, [sp, #60]
.L_080d6ba4:
	ldr	r2, [sp, #60]
	cmp	r2, #224
	bne.n	.L_080d6bb4
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #0
	str	r3, [r2, #0]
.L_080d6bb4:
	bl	sub_080049ac
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	sub_080051d8
	ldr	r3, [sp, #60]
	cmp	r3, #31
	bne.n	.L_080d6c04
	ldr	r2, [pc, #460]
	movs	r3, #8
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #157
	bl	sub_080f9010
	ldr	r1, [pc, #424]
	mov	r0, r9
	ldr	r3, [r0, r1]
	ldr	r3, [r3, #20]
	movs	r4, #0
	mov	sl, r4
	cmp	r3, #0
	beq.n	.L_080d6c04
	ldr	r5, [pc, #408]
	movs	r6, #36
	add	r5, r9
.L_080d6bec:
	ldr	r3, [r5, #0]
	movs	r1, #6
	ldrsh	r0, [r3, r6]
	bl	sub_080b5088
	movs	r3, #1
	add	sl, r3
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	adds	r6, #2
	cmp	sl, r3
	bne.n	.L_080d6bec
.L_080d6c04:
	ldr	r4, [sp, #60]
	cmp	r4, #72
	bne.n	.L_080d6c10
	movs	r0, #136
	bl	sub_080f9010
.L_080d6c10:
	ldr	r0, [sp, #60]
	cmp	r0, #140
	bne.n	.L_080d6c1c
	movs	r0, #156
	bl	sub_080f9010
.L_080d6c1c:
	ldr	r1, [sp, #52]
	movs	r2, #128
	ldr	r3, [sp, #56]
	lsls	r2, r2, #7
	adds	r1, r1, r2
	movs	r4, #128
	adds	r3, r3, r1
	lsls	r4, r4, #15
	str	r1, [sp, #52]
	str	r3, [sp, #56]
	cmp	r3, r4
	ble.n	.L_080d6c36
	str	r4, [sp, #56]
.L_080d6c36:
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #16
	ldr	r2, [sp, #56]
	bl	sub_080e6d3c
	ldr	r0, [sp, #60]
	subs	r0, #48
	cmp	r0, #48
	bhi.n	.L_080d6c76
	movs	r1, #24
	bl	sub_080022ec
	movs	r1, #3
	bl	sub_080022fc
	ldr	r3, [pc, #324]
	ldr	r6, [pc, #324]
	adds	r5, r0, #0
	add	r3, r9
	lsls	r5, r5, #1
	ldrb	r1, [r6, r5]
	ldr	r0, [r3, #0]
	bl	sub_08009020
	ldr	r3, [pc, #312]
	adds	r5, #1
	add	r3, r9
	ldr	r0, [r3, #0]
	ldrb	r1, [r6, r5]
	bl	sub_08009020
.L_080d6c76:
	ldr	r3, [sp, #60]
	subs	r3, #72
	cmp	r3, #55
	bhi.n	.L_080d6cee
	movs	r6, #232
	movs	r0, #0
	lsls	r6, r6, #7
	mov	sl, r0
	add	r6, r9
.L_080d6c88:
	mov	r3, sl
	ldr	r1, [sp, #60]
	adds	r3, #72
	cmp	r1, r3
	blt.n	.L_080d6ce2
	ldr	r5, [r6, #4]
	ldr	r2, [pc, #272]
	cmp	r5, r2
	bgt.n	.L_080d6ce2
	adds	r0, r1, #0
	add	r0, sl
	cmp	r0, #0
	bge.n	.L_080d6ca4
	adds	r0, #3
.L_080d6ca4:
	movs	r1, #5
	asrs	r0, r0, #2
	bl	sub_080022fc
	ldr	r4, [pc, #252]
	lsls	r1, r0, #1
	ldrh	r1, [r4, r1]
	ldr	r4, [pc, #252]
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	asrs	r3, r5, #16
	ldrb	r5, [r4, r0]
	lsrs	r4, r5, #1
	subs	r2, r2, r4
	ldr	r4, [pc, #240]
	ldrb	r4, [r4, r0]
	lsrs	r0, r4, #1
	add	r1, r9
	subs	r3, r3, r0
	str	r4, [sp, #4]
	str	r5, [sp, #0]
	ldr	r4, [sp, #68]
	ldr	r0, [sp, #64]
	bl	sub_080072f4
	movs	r2, #128
	adds	r0, r6, #0
	movs	r1, #64
	lsls	r2, r2, #5
	bl	sub_080e3908
.L_080d6ce2:
	movs	r4, #1
	add	sl, r4
	mov	r0, sl
	adds	r6, #28
	cmp	r0, #16
	bne.n	.L_080d6c88
.L_080d6cee:
	ldr	r1, [sp, #60]
	cmp	r1, #128
	bne.n	.L_080d6d52
	ldr	r5, [pc, #132]
	movs	r2, #0
	mov	sl, r2
	movs	r6, #255
	add	r5, r9
.L_080d6cfe:
	bl	sub_08004458
	movs	r1, #96
	bl	sub_08002304
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	sub_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #88
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r6
	negs	r0, r0
	lsls	r0, r0, #11
	str	r0, [r5, #16]
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	negs	r3, r3
	subs	r3, #16
	str	r3, [r5, #24]
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	adds	r5, #28
	cmp	r4, #48
	bne.n	.L_080d6cfe
	ldr	r2, [pc, #40]
	movs	r3, #0
	str	r3, [r2, #0]
.L_080d6d52:
	ldr	r0, [sp, #60]
	subs	r0, #128
	str	r0, [sp, #48]
	cmp	r0, #96
	bls.n	.L_080d6d5e
	b.n	.L_080d6eee
.L_080d6d5e:
	str	r0, [sp, #44]
	cmp	r0, #80
	ble.n	.L_080d6db8
	movs	r1, #80
	str	r1, [sp, #44]
	b.n	.L_080d6dc0
	movs	r0, r0
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0xffffe000
	.4byte 0x04000028
	.4byte 0x00007240
	.4byte 0x00007828
	.4byte 0xffc00000
	.4byte 0x03001e80
	.4byte 0x03001b04
	.4byte 0x0000011d
	.4byte 0x03000164
	.4byte 0x000077a8
	.4byte 0x000077e4
	.4byte 0x080ee910
	.4byte 0x000077e8
	.4byte 0x0067ffff
	.4byte 0x080ee916
	.4byte 0x080ee920
	.2byte 0xe925
	.2byte 0x080e
.L_080d6db8:
	ldr	r2, [pc, #556]
	movs	r3, #2
	add	r2, r9
	str	r3, [r2, #0]
.L_080d6dc0:
	movs	r7, #225
	movs	r2, #0
	lsls	r7, r7, #7
	mov	sl, r2
	add	r7, r9
.L_080d6dca:
	ldr	r3, [r7, #24]
	ldr	r4, [sp, #44]
	cmp	r4, r3
	bgt.n	.L_080d6dd4
	b.n	.L_080d6ee0
.L_080d6dd4:
	mov	r0, sl
	mov	fp, r9
	cmp	r0, #5
	ble.n	.L_080d6de4
	movs	r1, #216
	lsls	r1, r1, #3
	add	r1, r9
	mov	fp, r1
.L_080d6de4:
	ldr	r4, [sp, #44]
	subs	r2, r4, r3
	ldr	r3, [r7, #16]
	adds	r0, r3, #0
	muls	r0, r2
	str	r0, [sp, #40]
	adds	r6, r0, #0
	cmp	r0, #184
	ble.n	.L_080d6dfc
.L_080d6df6:
	subs	r6, #64
	cmp	r6, #184
	bgt.n	.L_080d6df6
.L_080d6dfc:
	cmp	r6, #119
	bgt.n	.L_080d6e24
	movs	r1, #1
	mov	r0, sl
	ands	r0, r1
	movs	r1, #24
	ldr	r3, [r7, #4]
	ldr	r2, [r7, #0]
	str	r1, [sp, #0]
	movs	r1, #8
	str	r1, [sp, #4]
	ldr	r1, [sp, #36]
	lsls	r0, r0, #2
	subs	r3, r3, r6
	ldr	r4, [r0, r1]
	subs	r3, #8
	ldr	r0, [sp, #64]
	mov	r1, fp
	bl	sub_080072f4
.L_080d6e24:
	mov	r3, sl
	movs	r4, #1
	ands	r3, r4
	lsls	r3, r3, #2
	movs	r2, #0
	str	r3, [sp, #32]
	mov	r8, r2
.L_080d6e32:
	ldr	r1, [r7, #4]
	mov	r0, r8
	lsls	r3, r0, #6
	subs	r2, r1, r6
	adds	r5, r2, r3
	movs	r3, #64
	negs	r3, r3
	movs	r2, #0
	movs	r0, #64
	cmp	r5, r3
	blt.n	.L_080d6e80
	cmp	r5, #0
	bge.n	.L_080d6e5a
	negs	r2, r5
	lsls	r3, r2, #1
	adds	r0, r5, #0
	adds	r3, r3, r2
	lsls	r2, r3, #3
	adds	r0, #64
	movs	r5, #0
.L_080d6e5a:
	adds	r3, r5, r0
	cmp	r3, r1
	ble.n	.L_080d6e64
	subs	r3, r3, r1
	subs	r0, r0, r3
.L_080d6e64:
	mov	r4, fp
	movs	r3, #24
	adds	r1, r4, r2
	ldr	r2, [r7, #0]
	str	r3, [sp, #0]
	str	r0, [sp, #4]
	ldr	r3, [sp, #36]
	ldr	r0, [sp, #32]
	adds	r1, #192
	ldr	r4, [r0, r3]
	ldr	r0, [sp, #64]
	adds	r3, r5, #0
	bl	sub_080072f4
.L_080d6e80:
	movs	r4, #1
	add	r8, r4
	mov	r0, r8
	cmp	r0, #3
	bne.n	.L_080d6e32
	mov	r6, sl
	ands	r6, r4
	cmp	r6, #0
	beq.n	.L_080d6ee0
	ldr	r1, [sp, #40]
	ldr	r5, [r7, #4]
	movs	r2, #127
	subs	r3, r5, r1
	ands	r3, r2
	subs	r3, #16
	movs	r1, #3
	mov	r0, sl
	mov	r8, r3
	bl	sub_080022fc
	ldr	r3, [pc, #320]
	adds	r1, r0, #0
	ldrb	r4, [r3, r1]
	mov	r2, r8
	adds	r3, r2, r4
	mov	ip, r4
	cmp	r3, r5
	ble.n	.L_080d6ebc
	subs	r3, r3, r5
	subs	r4, r4, r3
.L_080d6ebc:
	cmp	r4, #0
	ble.n	.L_080d6ee0
	ldr	r2, [pc, #300]
	lsls	r3, r1, #1
	ldrh	r1, [r2, r3]
	mov	r3, ip
	ldr	r2, [r7, #0]
	str	r3, [sp, #0]
	str	r4, [sp, #4]
	ldr	r3, [sp, #36]
	lsls	r0, r6, #2
	ldr	r4, [r0, r3]
	add	r1, r9
	adds	r2, #8
	ldr	r0, [sp, #64]
	mov	r3, r8
	bl	sub_080072f4
.L_080d6ee0:
	movs	r4, #1
	add	sl, r4
	mov	r0, sl
	adds	r7, #28
	cmp	r0, #10
	beq.n	.L_080d6eee
	b.n	.L_080d6dca
.L_080d6eee:
	ldr	r1, [sp, #48]
	cmp	r1, #95
	bhi.n	.L_080d6f9e
	movs	r5, #232
	movs	r2, #0
	lsls	r5, r5, #7
	mov	sl, r2
	movs	r6, #255
	add	r5, r9
.L_080d6f00:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_080d6f8e
	movs	r1, #5
	mov	r0, sl
	bl	sub_080022fc
	ldr	r2, [pc, #228]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	ldr	r4, [pc, #216]
	ldrb	r4, [r4, r0]
	str	r4, [sp, #0]
	ldr	r4, [pc, #216]
	ldrb	r0, [r4, r0]
	add	r1, r9
	str	r0, [sp, #4]
	ldr	r4, [sp, #68]
	ldr	r0, [sp, #64]
	bl	sub_080072f4
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #16]
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r1, #240
	adds	r2, r2, r3
	lsls	r1, r1, #15
	adds	r3, r3, r0
	str	r2, [r5, #4]
	str	r3, [r5, #16]
	cmp	r2, r1
	bls.n	.L_080d6f8c
	ldr	r2, [sp, #60]
	cmp	r2, #159
	bgt.n	.L_080d6f8c
	bl	sub_08004458
	movs	r1, #96
	bl	sub_08002304
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	sub_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #88
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #128
	lsls	r0, r0, #11
	str	r0, [r5, #12]
	bl	sub_08004458
	ands	r0, r6
	negs	r0, r0
	lsls	r0, r0, #11
	str	r0, [r5, #16]
.L_080d6f8c:
	ldr	r3, [r5, #24]
.L_080d6f8e:
	adds	r3, #1
	str	r3, [r5, #24]
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	adds	r5, #28
	cmp	r4, #32
	bne.n	.L_080d6f00
.L_080d6f9e:
	ldr	r3, [sp, #60]
	subs	r3, #224
	cmp	r3, #23
	bhi.n	.L_080d7034
	ldr	r0, [sp, #60]
	movs	r3, #3
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_080d7034
	ldr	r2, [pc, #48]
	movs	r4, #160
	movs	r1, #0
	lsls	r4, r4, #19
	mov	sl, r1
	mov	r8, r2
.L_080d6fbc:
	ldrh	r3, [r4, #0]
	movs	r7, #31
	ands	r7, r3
	lsls	r3, r3, #16
	mov	r0, r8
	lsrs	r6, r3, #21
	lsrs	r5, r3, #26
	ands	r6, r0
	ands	r5, r0
	adds	r0, r7, r6
	adds	r0, r0, r5
	movs	r1, #3
	str	r4, [sp, #8]
	bl	sub_080022ec
	ldr	r4, [sp, #8]
	cmp	r7, r0
	ble.n	.L_080d7000
	subs	r7, #1
	b.n	.L_080d7000
	.4byte 0x0000001f
	.4byte 0x000077a8
	.4byte 0x080ee930
	.4byte 0x080ee92a
	.4byte 0x080ee934
	.4byte 0x080ee93e
	.2byte 0xe943
	.2byte 0x080e
.L_080d7000:
	cmp	r7, r0
	bge.n	.L_080d7006
	adds	r7, #1
.L_080d7006:
	cmp	r6, r0
	ble.n	.L_080d700c
	subs	r6, #1
.L_080d700c:
	cmp	r6, r0
	bge.n	.L_080d7012
	adds	r6, #1
.L_080d7012:
	cmp	r5, r0
	ble.n	.L_080d7018
	subs	r5, #1
.L_080d7018:
	cmp	r5, r0
	bge.n	.L_080d701e
	adds	r5, #1
.L_080d701e:
	lsls	r2, r6, #5
	lsls	r3, r5, #10
	movs	r1, #1
	orrs	r3, r2
	add	sl, r1
	orrs	r3, r7
	mov	r2, sl
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r2, #64
	bne.n	.L_080d6fbc
.L_080d7034:
	ldr	r3, [sp, #48]
	cmp	r3, #172
	bhi.n	.L_080d710e
	ldr	r2, [pc, #500]
	mov	r0, r9
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	movs	r4, #0
	mov	sl, r4
	cmp	r3, #0
	beq.n	.L_080d710e
	add	r1, sp, #76
	mov	r8, r1
	add	r6, sp, #88
	mov	fp, r4
.L_080d7052:
	mov	r4, sl
	mov	r3, r9
	ldr	r2, [r3, r2]
	lsls	r3, r4, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	sub_080b5098
	ldr	r2, [r0, #0]
	ldr	r3, [r2, #8]
	str	r3, [r6, #0]
	ldr	r3, [r2, #12]
	str	r3, [r6, #4]
	ldr	r3, [r2, #16]
	adds	r0, r6, #0
	str	r3, [r6, #8]
	mov	r1, r8
	bl	sub_080e3944
	mov	r2, fp
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	ldr	r4, [pc, #432]
	add	r3, r9
	movs	r7, #0
	adds	r5, r3, r4
.L_080d7088:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	bne.n	.L_080d70b0
	bl	sub_08004458
	movs	r1, #15
	ldr	r3, [sp, #76]
	ands	r0, r1
	adds	r3, r3, r0
	subs	r3, #8
	str	r3, [r5, #0]
	bl	sub_08004458
	movs	r2, #15
	ldr	r3, [sp, #80]
	ands	r0, r2
	adds	r3, r3, r0
	subs	r3, #40
	str	r3, [r5, #4]
	ldr	r0, [r5, #24]
.L_080d70b0:
	cmp	r0, #4
	bhi.n	.L_080d70d8
	ldr	r2, [pc, #384]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #384]
	ldrb	r0, [r3, r0]
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	lsrs	r4, r0, #1
	subs	r2, r2, r4
	subs	r3, r3, r4
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	add	r1, r9
	ldr	r4, [sp, #68]
	ldr	r0, [sp, #64]
	bl	sub_080072f4
	ldr	r0, [r5, #24]
.L_080d70d8:
	adds	r3, r0, #1
	str	r3, [r5, #24]
	ldr	r4, [sp, #60]
	cmp	r4, #199
	bgt.n	.L_080d70f2
	cmp	r3, #5
	bne.n	.L_080d70f2
	bl	sub_08004458
	movs	r3, #7
	ands	r3, r0
	negs	r3, r3
	str	r3, [r5, #24]
.L_080d70f2:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #6
	bne.n	.L_080d7088
	ldr	r2, [pc, #308]
	mov	r4, r9
	ldr	r3, [r4, r2]
	movs	r1, #1
	ldr	r3, [r3, #20]
	movs	r0, #6
	add	sl, r1
	add	fp, r0
	cmp	sl, r3
	bne.n	.L_080d7052
.L_080d710e:
	ldr	r0, [sp, #60]
	cmp	r0, #232
	ble.n	.L_080d71dc
	ldr	r1, [pc, #296]
	lsls	r3, r0, #1
	adds	r6, r3, r1
	movs	r2, #0
	movs	r3, #0
	mov	sl, r2
	mov	ip, r3
	adds	r4, r6, #0
.L_080d7124:
	movs	r7, #0
.L_080d7126:
	cmp	r4, #127
	bhi.n	.L_080d7168
	movs	r5, #7
	adds	r0, r4, #0
	ands	r0, r5
	lsls	r3, r0, #5
	add	r3, sl
	ldr	r1, [pc, #268]
	lsls	r3, r3, #2
	adds	r3, r3, r7
	adds	r3, r3, r1
	ldrb	r1, [r3, #0]
	adds	r3, r4, #0
	cmp	r4, #0
	bge.n	.L_080d7146
	adds	r3, r4, #7
.L_080d7146:
	asrs	r3, r3, #3
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_080d7150
	adds	r2, r1, #7
.L_080d7150:
	asrs	r2, r2, #3
	lsls	r3, r3, #4
	adds	r3, r3, r2
	lsls	r3, r3, #3
	adds	r3, r3, r0
	ldr	r2, [sp, #64]
	ands	r1, r5
	lsls	r3, r3, #3
	adds	r3, r3, r1
	adds	r3, r2, r3
	mov	r0, ip
	strb	r0, [r3, #0]
.L_080d7168:
	adds	r7, #1
	cmp	r7, #4
	bne.n	.L_080d7126
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	adds	r4, #1
	cmp	r2, #32
	bne.n	.L_080d7124
	movs	r4, #0
	movs	r3, #0
	mov	lr, r4
	mov	sl, r3
	adds	r4, r6, #1
.L_080d7184:
	movs	r7, #0
	mov	ip, r4
.L_080d7188:
	cmp	r4, #127
	bhi.n	.L_080d71ca
	movs	r5, #7
	mov	r0, ip
	ands	r0, r5
	lsls	r3, r0, #5
	add	r3, sl
	ldr	r1, [pc, #172]
	lsls	r3, r3, #2
	adds	r3, r3, r7
	adds	r3, r3, r1
	ldrb	r1, [r3, #0]
	mov	r3, ip
	cmp	r3, #0
	bge.n	.L_080d71a8
	adds	r3, #7
.L_080d71a8:
	asrs	r3, r3, #3
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_080d71b2
	adds	r2, r1, #7
.L_080d71b2:
	asrs	r2, r2, #3
	lsls	r3, r3, #4
	adds	r3, r3, r2
	lsls	r3, r3, #3
	adds	r3, r3, r0
	ldr	r2, [sp, #64]
	ands	r1, r5
	lsls	r3, r3, #3
	adds	r3, r3, r1
	adds	r3, r2, r3
	mov	r0, lr
	strb	r0, [r3, #0]
.L_080d71ca:
	adds	r7, #1
	cmp	r7, #4
	bne.n	.L_080d7188
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	adds	r4, #1
	cmp	r2, #32
	bne.n	.L_080d7184
.L_080d71dc:
	ldr	r3, [sp, #60]
	subs	r3, #161
	cmp	r3, #62
	bhi.n	.L_080d726c
	ldr	r2, [pc, #72]
	movs	r3, #0
	mov	r4, r9
	mov	sl, r3
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_080d726c
.L_080d71f4:
	mov	r0, sl
	lsls	r3, r0, #3
	ldr	r1, [sp, #60]
	adds	r3, #160
	cmp	r1, r3
	ble.n	.L_080d725c
	mov	r3, r9
	ldr	r2, [r3, r2]
	lsls	r3, r0, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	sub_080b5098
	adds	r6, r0, #0
	ldr	r2, [r6, #0]
	movs	r0, #128
	ldr	r3, [r2, #12]
	lsls	r0, r0, #12
	movs	r1, #128
	adds	r3, r3, r0
	lsls	r1, r1, #16
	str	r3, [r2, #12]
	cmp	r3, r1
	ble.n	.L_080d7226
	str	r1, [r2, #12]
.L_080d7226:
	movs	r3, #0
	movs	r5, #0
	str	r3, [r2, #72]
	b.n	.L_080d7250
	movs	r0, r0
	.4byte 0x00007828
	.4byte 0x00007240
	.4byte 0x080ee948
	.4byte 0x080ee952
	.4byte 0xfffffe10
	.2byte 0x0000
	.2byte 0x0201
.L_080d7248:
	movs	r1, #5
	bl	sub_08009020
	adds	r5, #1
.L_080d7250:
	ldr	r0, [r6, #0]
	adds	r1, r5, #0
	bl	sub_080b50d8
	cmp	r0, #0
	bne.n	.L_080d7248
.L_080d725c:
	ldr	r2, [pc, #296]
	mov	r4, r9
	ldr	r3, [r4, r2]
	movs	r1, #1
	ldr	r3, [r3, #20]
	add	sl, r1
	cmp	sl, r3
	bne.n	.L_080d71f4
.L_080d726c:
	ldr	r2, [pc, #280]
	mov	r1, r9
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	movs	r0, #0
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L_080d72e0
	movs	r5, #143
	movs	r7, #8
	movs	r6, #36
	lsls	r5, r5, #1
.L_080d7284:
	ldr	r3, [sp, #60]
	cmp	r3, r5
	bne.n	.L_080d72a0
	mov	r4, r9
	ldr	r3, [r4, r2]
	ldrsh	r0, [r3, r6]
	bl	sub_080b5098
	movs	r3, #192
	ldr	r2, [r0, #0]
	lsls	r3, r3, #15
	str	r3, [r2, #12]
	ldr	r3, [pc, #236]
	str	r3, [r2, #72]
.L_080d72a0:
	adds	r3, r5, #0
	ldr	r2, [sp, #60]
	adds	r3, #16
	cmp	r2, r3
	bne.n	.L_080d72cc
	ldr	r3, [pc, #220]
	add	r3, r9
	ldr	r3, [r3, #0]
	movs	r2, #1
	ldrsh	r0, [r3, r6]
	movs	r1, #7
	mov	r3, sl
	negs	r2, r2
	str	r7, [sp, #0]
	bl	sub_080d6888
	movs	r0, #134
	bl	sub_080f9010
	ldr	r3, [pc, #200]
	add	r3, r9
	str	r7, [r3, #0]
.L_080d72cc:
	ldr	r2, [pc, #184]
	mov	r1, r9
	ldr	r3, [r1, r2]
	movs	r0, #1
	ldr	r3, [r3, #20]
	add	sl, r0
	adds	r6, #2
	adds	r5, #5
	cmp	sl, r3
	bne.n	.L_080d7284
.L_080d72e0:
	movs	r3, #151
	ldr	r2, [sp, #60]
	lsls	r3, r3, #1
	cmp	r2, r3
	beq.n	.L_080d72ec
	b.n	.L_080d7430
.L_080d72ec:
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r0, [pc, #152]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	movs	r1, #180
	lsls	r1, r1, #5
	ldr	r0, [pc, #140]
	add	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r6, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r6, [sp, #0]
	bl	sub_080ed408
	ldr	r5, [pc, #116]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r1, #7
	str	r3, [sp, #68]
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	str	r6, [sp, #0]
	bl	sub_080ed408
	adds	r5, #188
	ldr	r4, [sp, #36]
	ldr	r3, [r5, #0]
	ldr	r2, [pc, #92]
	str	r3, [r4, #4]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	subs	r2, #48
	strh	r3, [r2, #0]
	adds	r2, #8
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #72]
	add	r3, r9
	str	r6, [r3, #0]
	add	r2, r9
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #60]
	bl	sub_080041d8
	ldr	r2, [pc, #20]
	mov	r1, r9
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	movs	r0, #0
	mov	sl, r0
	b.n	.L_080d73ac
	movs	r0, r0
	.4byte 0x00003f46
	.4byte 0x00000080
	.4byte 0x00007828
	.4byte 0x0000ab85
	.4byte 0x000077a8
	.4byte 0x00000098
	.4byte 0x000000c0
	.4byte 0x03001e50
	.4byte 0x04000050
	.4byte 0x00007784
	.2byte 0xd261
	.2byte 0x080c
.L_080d73ac:
	cmp	r3, #0
	beq.n	.L_080d7430
.L_080d73b0:
	mov	r4, sl
	mov	r3, r9
	ldr	r2, [r3, r2]
	lsls	r3, r4, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	sub_080b5098
	mov	r3, sl
	lsls	r2, r3, #2
	add	r2, sl
	lsls	r3, r2, #3
	subs	r3, r3, r2
	ldr	r0, [r0, #0]
	lsls	r3, r3, #3
	movs	r4, #225
	add	r3, r9
	lsls	r4, r4, #7
	mov	r8, r0
	movs	r7, #0
	adds	r6, r3, r4
.L_080d73da:
	mov	r0, r8
	ldr	r3, [r0, #8]
	str	r3, [r6, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r6, #4]
	ldr	r3, [r0, #16]
	str	r3, [r6, #8]
	ldr	r3, [pc, #584]
	adds	r5, r7, #0
	muls	r5, r3
	adds	r0, r5, #0
	bl	sub_08002322
	lsls	r0, r0, #2
	str	r0, [r6, #12]
	bl	sub_08004458
	ldr	r3, [pc, #568]
	movs	r1, #128
	ands	r3, r0
	lsls	r1, r1, #9
	adds	r3, r3, r1
	str	r3, [r6, #16]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r7, #1
	lsls	r0, r0, #2
	movs	r3, #0
	str	r0, [r6, #20]
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r7, #10
	bne.n	.L_080d73da
	movs	r2, #1
	add	sl, r2
	ldr	r2, [pc, #532]
	mov	r4, r9
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	cmp	sl, r3
	bne.n	.L_080d73b0
.L_080d7430:
	ldr	r0, [sp, #60]
	ldr	r1, [pc, #524]
	cmp	r0, r1
	bgt.n	.L_080d743a
	b.n	.L_080d7594
.L_080d743a:
	movs	r2, #0
	mov	sl, r2
	ldr	r2, [pc, #508]
	mov	r4, r9
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L_080d744c
	b.n	.L_080d7594
.L_080d744c:
	ldr	r1, [sp, #60]
	ldr	r3, [pc, #500]
	mov	r0, sp
	adds	r0, #76
	adds	r3, r1, r3
	movs	r4, #151
	mov	r1, sp
	str	r0, [sp, #24]
	lsls	r4, r4, #1
	movs	r0, #0
	adds	r1, #88
	str	r3, [sp, #20]
	str	r4, [sp, #16]
	str	r0, [sp, #12]
	str	r1, [sp, #28]
.L_080d746a:
	ldr	r3, [sp, #60]
	ldr	r4, [sp, #16]
	cmp	r3, r4
	blt.n	.L_080d74f8
	ldr	r0, [sp, #12]
	movs	r1, #157
	lsls	r1, r1, #1
	ldr	r4, [sp, #60]
	adds	r3, r0, r1
	cmp	r4, r3
	bge.n	.L_080d74f8
	ldr	r0, [sp, #20]
	mov	r1, r9
	mov	r4, sl
	ldr	r2, [r1, r2]
	lsls	r3, r4, #1
	adds	r3, #36
	lsrs	r6, r0, #31
	adds	r6, r0, r6
	ldrsh	r0, [r2, r3]
	bl	sub_080b5098
	ldr	r2, [r0, #0]
	ldr	r4, [sp, #28]
	ldr	r3, [r2, #8]
	str	r3, [r4, #0]
	movs	r3, #0
	str	r3, [r4, #4]
	ldr	r3, [r2, #16]
	str	r3, [r4, #8]
	ldr	r0, [sp, #24]
	mov	r8, r0
	mov	r1, r8
	ldr	r0, [sp, #28]
	bl	sub_080e3944
	asrs	r6, r6, #1
	mov	r1, r8
	ldr	r2, [r1, #0]
	lsls	r5, r6, #4
	subs	r5, r5, r6
	ldr	r3, [r1, #4]
	asrs	r2, r2, #1
	lsls	r5, r5, #5
	str	r2, [r1, #0]
	add	r5, r9
	movs	r4, #20
	movs	r0, #24
	subs	r2, #20
	subs	r3, #24
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #68]
	adds	r1, r5, #0
	ldr	r0, [sp, #64]
	bl	sub_080072f4
	mov	r1, r8
	movs	r4, #20
	movs	r0, #24
	ldr	r3, [r1, #4]
	ldr	r2, [r1, #0]
	str	r0, [sp, #4]
	ldr	r1, [sp, #36]
	str	r4, [sp, #0]
	subs	r3, #24
	ldr	r4, [r1, #4]
	ldr	r0, [sp, #64]
	adds	r1, r5, #0
	bl	sub_080072f4
.L_080d74f8:
	ldr	r3, [sp, #16]
	ldr	r2, [sp, #60]
	adds	r3, #6
	cmp	r2, r3
	blt.n	.L_080d7570
	ldr	r2, [sp, #12]
	add	r3, sp, #76
	add	r2, sl
	mov	r8, r3
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #3
	movs	r4, #225
	add	r3, r9
	lsls	r4, r4, #7
	movs	r0, #12
	movs	r7, #0
	mov	r6, r8
	adds	r5, r3, r4
	mov	fp, r0
.L_080d7520:
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_080e3944
	ldr	r3, [r6, #0]
	asrs	r2, r3, #1
	str	r2, [r6, #0]
	ldr	r3, [r5, #24]
	cmp	r3, #26
	bhi.n	.L_080d7556
	ldr	r3, [pc, #272]
	mov	r4, fp
	ldrh	r1, [r3, r4]
	ldr	r3, [pc, #272]
	ldrh	r4, [r3, r4]
	mov	r3, r8
	ldr	r3, [r3, #4]
	lsrs	r0, r4, #1
	subs	r2, r2, r0
	subs	r3, r3, r0
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	add	r1, r9
	ldr	r4, [sp, #68]
	ldr	r0, [sp, #64]
	bl	sub_080072f4
.L_080d7556:
	movs	r2, #128
	adds	r0, r5, #0
	movs	r1, #60
	lsls	r2, r2, #5
	bl	sub_080e3908
	ldr	r3, [r5, #24]
	adds	r7, #1
	adds	r3, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #5
	bne.n	.L_080d7520
.L_080d7570:
	ldr	r4, [sp, #20]
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #12]
	subs	r4, #4
	movs	r2, #1
	adds	r0, #4
	adds	r1, #4
	str	r4, [sp, #20]
	str	r0, [sp, #16]
	str	r1, [sp, #12]
	add	sl, r2
	ldr	r2, [pc, #180]
	mov	r4, r9
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	cmp	sl, r3
	beq.n	.L_080d7594
	b.n	.L_080d746a
.L_080d7594:
	ldr	r0, [sp, #60]
	cmp	r0, #127
	bgt.n	.L_080d75a4
	movs	r0, #4
	movs	r1, #16
	bl	sub_080e155c
	b.n	.L_080d75be
.L_080d75a4:
	ldr	r1, [sp, #60]
	ldr	r2, [pc, #152]
	cmp	r1, r2
	bgt.n	.L_080d75b6
	movs	r0, #2
	movs	r1, #2
	bl	sub_080e155c
	b.n	.L_080d75be
.L_080d75b6:
	movs	r0, #4
	movs	r1, #8
	bl	sub_080e155c
.L_080d75be:
	bl	sub_080cd52c
	ldr	r2, [pc, #140]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [sp, #60]
	movs	r4, #183
	adds	r3, #1
	lsls	r4, r4, #1
	str	r3, [sp, #60]
	cmp	r3, r4
	beq.n	.L_080d75e2
	bl	.L_080d6b76
.L_080d75e2:
	ldr	r0, [pc, #112]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	movs	r0, #134
	bl	sub_080b50e8
	movs	r1, #128
	movs	r0, #2
	lsls	r1, r1, #16
	ldr	r2, [sp, #56]
	ldr	r5, [pc, #84]
	bl	sub_080e6eac
	movs	r0, #0
	mov	sl, r0
	add	r5, r9
.L_080d760e:
	ldmia	r5!, {r0}
	bl	sub_08009038
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #8
	bne.n	.L_080d760e
	bl	sub_080cdbc0
	add	sp, #100
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00003334
	.4byte 0x00007fff
	.4byte 0x00007828
	.4byte 0x0000012d
	.4byte 0xfffffed2
	.4byte 0x080ee958
	.4byte 0x080ee966
	.4byte 0x00007824
	.4byte 0x080cd261
	.4byte 0x000077d8

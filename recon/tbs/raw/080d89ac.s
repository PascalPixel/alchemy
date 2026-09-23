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
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_08004bd4, 0x08004bd4
	.set sub_08004c1c, 0x08004c1c
	.set sub_08004c6c, 0x08004c6c
	.set sub_08004cb4, 0x08004cb4
	.set sub_080051d8, 0x080051d8
	.set sub_08005340, 0x08005340
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080cef64, 0x080cef64
	.set sub_080d6888, 0x080d6888
	.set sub_080de2f8, 0x080de2f8
	.set sub_080e0524, 0x080e0524
	.set sub_080e3944, 0x080e3944
	.set sub_080e396c, 0x080e396c
	.set sub_080f9010, 0x080f9010
	.global BattleEffectA
	.global Func_080d89ac
	.thumb_func
BattleEffectA:
Func_080d89ac:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #176]
	adds	r3, r2, #0
	adds	r6, r0, #0
	ldmia	r3!, {r0}
	sub	sp, #128
	str	r0, [sp, #72]
	ldr	r3, [r3, #0]
	str	r3, [sp, #68]
	ldr	r2, [r2, #8]
	str	r2, [sp, #56]
	ldr	r2, [pc, #160]
	mov	sl, r1
	adds	r5, r0, r2
	movs	r1, #0
	str	r1, [sp, #48]
	movs	r0, #0
	str	r6, [r5, #0]
	bl	sub_080cd594
	ldr	r5, [r5, #0]
	ldr	r3, [r5, #28]
	cmp	r3, #1
	bne.n	.L_080d8a32
	movs	r3, #6
	mov	r0, sl
	eors	r3, r0
	negs	r2, r3
	orrs	r2, r3
	lsrs	r1, r2, #31
	movs	r3, #2
	subs	r1, r3, r1
	cmp	r0, #6
	beq.n	.L_080d8a00
	cmp	r0, #0
	bne.n	.L_080d8a14
.L_080d8a00:
	add	r3, sp, #88
	ldr	r2, [r5, #4]
	str	r3, [sp, #0]
	add	r3, sp, #84
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r3, #0
	bl	sub_080de2f8
	b.n	.L_080d8a26
.L_080d8a14:
	add	r3, sp, #88
	ldr	r2, [r5, #4]
	str	r3, [sp, #0]
	add	r3, sp, #84
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r3, #1
	bl	sub_080de2f8
.L_080d8a26:
	ldr	r1, [sp, #72]
	ldr	r2, [pc, #68]
	adds	r3, r1, r2
	ldr	r2, [r3, #0]
	movs	r3, #0
	str	r3, [r2, #24]
.L_080d8a32:
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_080d8a78
	ldr	r0, [sp, #72]
	ldr	r1, [pc, #52]
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	add	r5, sp, #116
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	adds	r1, r5, #0
	bl	sub_080e396c
	ldr	r2, [r5, #0]
	movs	r3, #64
	subs	r3, r3, r2
	ldr	r2, [pc, #32]
	str	r3, [sp, #48]
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	ldr	r3, [pc, #12]
	subs	r2, #8
	strh	r3, [r2, #0]
	movs	r3, #0
	str	r3, [sp, #44]
	b.n	.L_080d8a7c
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x03001eec
	.4byte 0x00007828
	.2byte 0x0028
	.2byte 0x0400
.L_080d8a78:
	movs	r0, #1
	str	r0, [sp, #44]
.L_080d8a7c:
	ldr	r1, [sp, #56]
	ldr	r0, [pc, #836]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [pc, #828]
	ldr	r1, [sp, #72]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	mov	r1, sl
	cmp	r1, #1
	bls.n	.L_080d8aa6
	cmp	r1, #3
	beq.n	.L_080d8aa6
	cmp	r1, #4
	beq.n	.L_080d8aa6
	cmp	r1, #5
	bne.n	.L_080d8abc
.L_080d8aa6:
	ldr	r2, [sp, #72]
	ldr	r0, [pc, #800]
	adds	r3, r2, r0
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #24]
	cmp	r3, #0
	bne.n	.L_080d8ab8
	ldr	r0, [pc, #792]
	b.n	.L_080d8ac8
.L_080d8ab8:
	ldr	r0, [pc, #792]
	b.n	.L_080d8ac8
.L_080d8abc:
	mov	r1, sl
	cmp	r1, #6
	bne.n	.L_080d8ac6
	ldr	r0, [pc, #788]
	b.n	.L_080d8ac8
.L_080d8ac6:
	ldr	r0, [pc, #788]
.L_080d8ac8:
	bl	sub_08002f40
	adds	r2, r0, #0
	movs	r0, #160
	adds	r1, r2, #0
	ldr	r3, [pc, #780]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	ldr	r2, [sp, #44]
	cmp	r2, #0
	bne.n	.L_080d8af0
	mov	r3, sl
	cmp	r3, #6
	bne.n	.L_080d8aec
	ldr	r0, [pc, #748]
	b.n	.L_080d8afc
.L_080d8aec:
	ldr	r0, [pc, #756]
	b.n	.L_080d8afc
.L_080d8af0:
	mov	r0, sl
	cmp	r0, #6
	bne.n	.L_080d8afa
	ldr	r0, [pc, #752]
	b.n	.L_080d8afc
.L_080d8afa:
	ldr	r0, [pc, #752]
.L_080d8afc:
	bl	sub_08002f40
	adds	r2, r0, #0
	ldr	r3, [sp, #72]
	movs	r0, #128
	adds	r2, #128
	lsls	r0, r0, #5
	adds	r1, r3, r0
	adds	r0, r2, #0
	bl	sub_08005340
	ldr	r2, [pc, #696]
	ldr	r1, [sp, #72]
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	ldr	r0, [r3, #4]
	mov	r3, sp
	adds	r3, #76
	adds	r1, r3, #0
	str	r3, [sp, #40]
	bl	sub_080cef64
	mov	r0, sl
	cmp	r0, #0
	beq.n	.L_080d8b32
	cmp	r0, #6
	bne.n	.L_080d8b82
.L_080d8b32:
	movs	r1, #0
	ldr	r5, [pc, #696]
	mov	r8, r1
	movs	r6, #255
.L_080d8b3a:
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	adds	r3, #64
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	bl	sub_08004458
	movs	r3, #0
	ands	r0, r6
	str	r3, [r5, #24]
	subs	r0, #127
	movs	r2, #1
	movs	r3, #128
	lsls	r0, r0, #15
	add	r8, r2
	lsls	r3, r3, #2
	str	r0, [r5, #8]
	adds	r5, #28
	cmp	r8, r3
	bne.n	.L_080d8b3a
	ldr	r0, [sp, #72]
	ldr	r1, [pc, #596]
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #3
	adds	r3, #88
	b.n	.L_080d8cda
.L_080d8b82:
	mov	r2, sl
	cmp	r2, #1
	bne.n	.L_080d8bd6
	movs	r3, #0
	ldr	r5, [pc, #612]
	mov	r8, r3
	movs	r6, #255
.L_080d8b90:
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #4]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #8]
	movs	r1, #128
	movs	r0, #1
	movs	r3, #0
	add	r8, r0
	lsls	r1, r1, #2
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r8, r1
	bne.n	.L_080d8b90
	ldr	r2, [sp, #72]
	ldr	r0, [pc, #512]
	adds	r3, r2, r0
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #3
	adds	r3, #88
	b.n	.L_080d8cda
.L_080d8bd6:
	mov	r1, sl
	cmp	r1, #2
	bne.n	.L_080d8c42
	movs	r2, #0
	ldr	r7, [pc, #528]
	mov	r8, r2
.L_080d8be2:
	bl	sub_08004458
	ldr	r3, [pc, #524]
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08004458
	movs	r5, #63
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002322
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	str	r3, [r7, #0]
	ldr	r3, [pc, #500]
	adds	r0, r6, #0
	str	r3, [r7, #4]
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	str	r3, [r7, #8]
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r3, #32
	lsls	r3, r3, #13
	str	r3, [r7, #16]
	movs	r3, #0
	str	r3, [r7, #24]
	movs	r0, #128
	movs	r3, #1
	add	r8, r3
	lsls	r0, r0, #2
	adds	r7, #28
	cmp	r8, r0
	bne.n	.L_080d8be2
	ldr	r1, [sp, #72]
	ldr	r2, [pc, #404]
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #3
	adds	r3, #88
	b.n	.L_080d8cda
.L_080d8c42:
	mov	r3, sl
	cmp	r3, #3
	bne.n	.L_080d8c8e
	movs	r0, #0
	ldr	r5, [pc, #420]
	mov	r8, r0
	movs	r6, #255
.L_080d8c50:
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #14
	str	r0, [r5, #4]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	movs	r1, #1
	movs	r2, #128
	lsls	r0, r0, #15
	movs	r3, #0
	add	r8, r1
	lsls	r2, r2, #2
	str	r0, [r5, #8]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r8, r2
	bne.n	.L_080d8c50
	ldr	r0, [sp, #72]
	ldr	r1, [pc, #320]
	adds	r3, r0, r1
	b.n	.L_080d8cd2
.L_080d8c8e:
	movs	r2, #0
	ldr	r5, [pc, #348]
	mov	r8, r2
	movs	r6, #255
.L_080d8c96:
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #4]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	movs	r3, #0
	str	r0, [r5, #8]
	str	r3, [r5, #24]
	movs	r0, #128
	movs	r3, #1
	add	r8, r3
	lsls	r0, r0, #2
	adds	r5, #28
	cmp	r8, r0
	bne.n	.L_080d8c96
	ldr	r1, [sp, #72]
	ldr	r2, [pc, #252]
	adds	r3, r1, r2
.L_080d8cd2:
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #3
	adds	r3, #72
.L_080d8cda:
	str	r3, [sp, #64]
	movs	r3, #64
	ldr	r0, [sp, #72]
	ldr	r1, [pc, #232]
	str	r3, [sp, #52]
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #24]
	cmp	r3, #0
	bne.n	.L_080d8cf4
	movs	r2, #32
	str	r2, [sp, #52]
	b.n	.L_080d8cfc
.L_080d8cf4:
	cmp	r3, #2
	bne.n	.L_080d8cfc
	movs	r3, #128
	str	r3, [sp, #52]
.L_080d8cfc:
	ldr	r0, [sp, #72]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r0, r1
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r3, [pc, #240]
	movs	r1, #144
	adds	r2, r0, r3
	movs	r3, #75
	lsls	r1, r1, #3
	str	r3, [r2, #0]
	ldr	r0, [pc, #232]
	bl	sub_080041d8
	ldr	r1, [sp, #64]
	movs	r0, #0
	mov	fp, r0
	cmp	r1, #0
	bne.n	.L_080d8d26
	b.n	.L_080d913a
.L_080d8d26:
	str	r0, [sp, #12]
.L_080d8d28:
	ldr	r3, [pc, #216]
	ldr	r3, [r3, #0]
	mov	r2, fp
	str	r3, [sp, #36]
	cmp	r2, #40
	bne.n	.L_080d8d3a
	movs	r0, #0
	bl	sub_080b50e8
.L_080d8d3a:
	ldr	r0, [sp, #72]
	ldr	r1, [pc, #140]
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #28]
	cmp	r3, #1
	beq.n	.L_080d8d4a
	b.n	.L_080d8e80
.L_080d8d4a:
	ldr	r2, [sp, #44]
	cmp	r2, #0
	bne.n	.L_080d8e08
	ldr	r0, [sp, #12]
	bl	sub_08002322
	lsls	r3, r0, #2
	adds	r3, r3, r0
	ldr	r2, [sp, #88]
	lsls	r3, r3, #2
	ldr	r0, [sp, #48]
	asrs	r3, r3, #16
	adds	r3, r3, r2
	adds	r3, r3, r0
	subs	r3, #20
	ldr	r0, [sp, #12]
	mov	r8, r3
	bl	sub_0800231c
	lsls	r0, r0, #2
	ldr	r3, [sp, #84]
	asrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r5, r0, #0
	mov	r1, fp
	subs	r5, #24
	cmp	r1, #32
	ble.n	.L_080d8d8a
	lsls	r3, r1, #1
	subs	r3, r5, r3
	adds	r5, r3, #0
	adds	r5, #64
.L_080d8d8a:
	ldr	r2, [sp, #72]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r7, r2, r3
	movs	r6, #40
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	ldr	r4, [sp, #76]
	ldr	r0, [sp, #68]
	adds	r1, r7, #0
	mov	r2, r8
	adds	r3, r5, #0
	bl	sub_080072f4
	mov	r0, fp
	cmp	r0, #3
	bgt.n	.L_080d8e80
	ldr	r1, [sp, #40]
	str	r6, [sp, #0]
	str	r6, [sp, #4]
	ldr	r0, [sp, #68]
	ldr	r4, [r1, #4]
	mov	r2, r8
	adds	r1, r7, #0
	adds	r3, r5, #0
	bl	sub_080072f4
	b.n	.L_080d8e80
	movs	r0, r0
	.4byte 0x00000073
	.4byte 0x000000ba
	.4byte 0x00007828
	.4byte 0x000000b3
	.4byte 0x000000b9
	.4byte 0x0000008d
	.4byte 0x000000c0
	.4byte 0x03001388
	.4byte 0x00000091
	.4byte 0x0000008e
	.4byte 0x00000092
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0xffce0000
	.4byte 0x00007784
	.4byte 0x080cd261
	.2byte 0x1e80
	.2byte 0x0300
.L_080d8e08:
	ldr	r0, [sp, #12]
	bl	sub_08002322
	ldr	r3, [sp, #88]
	lsls	r2, r0, #2
	adds	r2, r2, r0
	lsrs	r1, r3, #31
	adds	r3, r3, r1
	lsls	r2, r2, #1
	asrs	r3, r3, #1
	asrs	r2, r2, #16
	adds	r2, r2, r3
	subs	r2, #10
	ldr	r0, [sp, #12]
	mov	r9, r2
	bl	sub_0800231c
	lsls	r0, r0, #2
	ldr	r3, [sp, #84]
	asrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r5, r0, #0
	mov	r2, fp
	subs	r5, #24
	cmp	r2, #32
	ble.n	.L_080d8e44
	lsls	r3, r2, #1
	subs	r3, r5, r3
	adds	r5, r3, #0
	adds	r5, #64
.L_080d8e44:
	ldr	r3, [sp, #72]
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r6, r3, r0
	movs	r1, #20
	movs	r7, #40
	str	r1, [sp, #0]
	mov	r2, r9
	mov	r8, r1
	str	r7, [sp, #4]
	ldr	r4, [sp, #76]
	ldr	r0, [sp, #68]
	adds	r1, r6, #0
	adds	r3, r5, #0
	bl	sub_080072f4
	mov	r2, fp
	cmp	r2, #3
	bgt.n	.L_080d8e80
	mov	r3, r8
	str	r3, [sp, #0]
	ldr	r0, [sp, #40]
	str	r7, [sp, #4]
	adds	r1, r6, #0
	ldr	r4, [r0, #4]
	mov	r2, r9
	ldr	r0, [sp, #68]
	adds	r3, r5, #0
	bl	sub_080072f4
.L_080d8e80:
	movs	r1, #0
	str	r1, [sp, #60]
	ldr	r2, [pc, #732]
	ldr	r0, [sp, #72]
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L_080d8e92
	b.n	.L_080d9110
.L_080d8e92:
	ldr	r1, [sp, #36]
	mov	r3, sp
	adds	r1, #12
	adds	r3, #92
	mov	r0, fp
	str	r1, [sp, #32]
	str	r3, [sp, #28]
	lsls	r0, r0, #9
	movs	r1, #36
	movs	r3, #0
	str	r0, [sp, #20]
	str	r1, [sp, #16]
	str	r3, [sp, #8]
.L_080d8eac:
	ldr	r0, [sp, #72]
	adds	r6, r0, r2
	ldr	r1, [sp, #16]
	ldr	r3, [r6, #0]
	ldrsh	r0, [r3, r1]
	bl	sub_080b5098
	ldr	r3, [sp, #60]
	lsls	r3, r3, #3
	ldr	r5, [r0, #0]
	mov	r9, r3
	bl	sub_080049ac
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #32]
	bl	sub_080051d8
	ldr	r3, [r5, #8]
	ldr	r0, [sp, #28]
	str	r3, [r0, #0]
	movs	r3, #160
	lsls	r3, r3, #14
	str	r3, [r0, #4]
	ldr	r3, [r5, #16]
	str	r3, [r0, #8]
	ldr	r0, [sp, #28]
	bl	sub_08004cb4
	mov	r3, r9
	adds	r3, #20
	cmp	fp, r3
	bne.n	.L_080d8ef2
	movs	r0, #126
	bl	sub_080f9010
.L_080d8ef2:
	mov	r3, r9
	adds	r3, #36
	cmp	fp, r3
	bne.n	.L_080d8f10
	ldr	r3, [r6, #0]
	ldr	r1, [sp, #16]
	ldrsh	r0, [r3, r1]
	movs	r3, #28
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	ldr	r3, [sp, #60]
	bl	sub_080d6888
.L_080d8f10:
	cmp	fp, r9
	bgt.n	.L_080d8f16
	b.n	.L_080d90e4
.L_080d8f16:
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L_080d8f58
	cmp	r3, #6
	beq.n	.L_080d8f58
	mov	r0, sl
	cmp	r0, #1
	bne.n	.L_080d8f38
	mov	r1, fp
	lsls	r5, r1, #9
	adds	r0, r5, #0
	bl	sub_08004bd4
	adds	r0, r5, #0
	bl	sub_08004c6c
	b.n	.L_080d8f6c
.L_080d8f38:
	mov	r2, sl
	cmp	r2, #2
	bne.n	.L_080d8f52
	ldr	r3, [sp, #60]
	lsls	r0, r3, #2
	adds	r0, r0, r3
	lsls	r0, r0, #3
	mov	r1, fp
	subs	r0, r1, r0
	lsls	r0, r0, #9
	bl	sub_08004c1c
	b.n	.L_080d8f6c
.L_080d8f52:
	mov	r2, sl
	cmp	r2, #3
	bne.n	.L_080d8f60
.L_080d8f58:
	ldr	r0, [sp, #20]
	bl	sub_08004c1c
	b.n	.L_080d8f6c
.L_080d8f60:
	ldr	r0, [sp, #20]
	bl	sub_08004c1c
	ldr	r0, [sp, #20]
	bl	sub_08004bd4
.L_080d8f6c:
	ldr	r0, [sp, #52]
	movs	r3, #0
	mov	r8, r3
	cmp	r0, #0
	bne.n	.L_080d8f78
	b.n	.L_080d90e4
.L_080d8f78:
	mov	r1, sl
	subs	r1, #3
	ldr	r2, [sp, #8]
	ldr	r3, [pc, #488]
	str	r1, [sp, #24]
	adds	r6, r2, r3
.L_080d8f84:
	ldr	r0, [sp, #24]
	cmp	r0, #2
	bhi.n	.L_080d8f9a
	mov	r1, r8
	lsrs	r3, r1, #31
	add	r3, r8
	asrs	r3, r3, #1
	add	r3, r9
	adds	r2, r3, #0
	adds	r2, #32
	b.n	.L_080d8f9e
.L_080d8f9a:
	movs	r2, #128
	lsls	r2, r2, #9
.L_080d8f9e:
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_080d8fa6
	adds	r3, #3
.L_080d8fa6:
	asrs	r3, r3, #2
	add	r3, r9
	cmp	fp, r3
	bgt.n	.L_080d8fb0
	b.n	.L_080d90d6
.L_080d8fb0:
	cmp	fp, r2
	blt.n	.L_080d8fb6
	b.n	.L_080d90d6
.L_080d8fb6:
	ldr	r3, [r6, #0]
	asrs	r3, r3, #8
	adds	r0, r3, #0
	muls	r0, r3
	ldr	r3, [r6, #4]
	asrs	r3, r3, #8
	adds	r2, r3, #0
	muls	r2, r3
	ldr	r3, [r6, #8]
	asrs	r3, r3, #8
	adds	r1, r3, #0
	muls	r1, r3
	adds	r0, r0, r2
	adds	r3, r1, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #404]
	bl	sub_080072f0
	asrs	r7, r0, #9
	cmp	r7, #0
	beq.n	.L_080d90d6
	add	r5, sp, #104
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080e3944
	mov	r2, sl
	cmp	r2, #0
	bne.n	.L_080d8ff8
	ldr	r3, [r5, #0]
	ldr	r0, [sp, #48]
	adds	r3, r3, r0
	b.n	.L_080d8ffc
.L_080d8ff8:
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
.L_080d8ffc:
	str	r3, [r5, #0]
	ldr	r3, [r5, #4]
	adds	r3, #16
	str	r3, [r5, #4]
	ldr	r1, [pc, #360]
	ldr	r3, [r5, #8]
	cmp	r3, r1
	bgt.n	.L_080d9012
	movs	r3, #157
	lsls	r3, r3, #1
	str	r3, [r5, #8]
.L_080d9012:
	ldr	r2, [pc, #352]
	cmp	r3, r2
	ble.n	.L_080d901c
	str	r2, [r5, #8]
	adds	r3, r2, #0
.L_080d901c:
	ldr	r0, [pc, #344]
	adds	r2, r3, r0
	cmp	r2, #0
	bge.n	.L_080d9028
	adds	r2, r3, #0
	subs	r2, #187
.L_080d9028:
	asrs	r2, r2, #7
	movs	r3, #3
	mov	r1, sl
	subs	r4, r3, r2
	cmp	r1, #0
	bne.n	.L_080d9044
	mov	r2, r8
	lsls	r0, r2, #2
	add	r0, fp
	movs	r1, #9
	bl	sub_080022fc
	adds	r4, r0, #0
	b.n	.L_080d9052
.L_080d9044:
	mov	r3, sl
	cmp	r3, #3
	beq.n	.L_080d9052
	cmp	r3, #4
	beq.n	.L_080d9052
	cmp	r3, #5
	bne.n	.L_080d907a
.L_080d9052:
	ldr	r2, [pc, #296]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r0, [sp, #72]
	ldr	r3, [pc, #292]
	adds	r1, r0, r1
	ldrb	r0, [r3, r4]
	ldr	r2, [r5, #0]
	lsrs	r4, r0, #1
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r0, [sp, #40]
	subs	r2, r2, r4
	subs	r3, r3, r4
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #68]
	bl	sub_080072f4
	b.n	.L_080d90a2
.L_080d907a:
	lsls	r0, r4, #1
	ldr	r2, [pc, #260]
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #56]
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	adds	r1, r2, r1
	ldr	r2, [r5, #0]
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #4]
	str	r4, [sp, #0]
	ldr	r0, [sp, #40]
	subs	r3, r3, r4
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #68]
	bl	sub_080072f4
.L_080d90a2:
	mov	r1, sl
	cmp	r1, #2
	bls.n	.L_080d90ac
	cmp	r1, #6
	bne.n	.L_080d90d6
.L_080d90ac:
	ldr	r5, [r6, #0]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	sub_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #0]
	ldr	r5, [r6, #4]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	sub_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #4]
	ldr	r5, [r6, #8]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	sub_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #8]
.L_080d90d6:
	movs	r2, #1
	ldr	r3, [sp, #52]
	add	r8, r2
	adds	r6, #28
	cmp	r8, r3
	beq.n	.L_080d90e4
	b.n	.L_080d8f84
.L_080d90e4:
	ldr	r0, [sp, #20]
	ldr	r1, [pc, #160]
	adds	r0, r0, r1
	str	r0, [sp, #20]
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #8]
	ldr	r1, [sp, #60]
	movs	r0, #224
	lsls	r0, r0, #3
	adds	r3, r3, r0
	adds	r2, #2
	adds	r1, #1
	str	r2, [sp, #16]
	str	r3, [sp, #8]
	str	r1, [sp, #60]
	ldr	r2, [pc, #96]
	ldr	r0, [sp, #72]
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	cmp	r1, r3
	beq.n	.L_080d9110
	b.n	.L_080d8eac
.L_080d9110:
	bl	sub_080cd52c
	ldr	r3, [pc, #116]
	ldr	r1, [sp, #72]
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r1, #128
	ldr	r0, [sp, #12]
	lsls	r1, r1, #4
	movs	r2, #1
	ldr	r3, [sp, #64]
	adds	r0, r0, r1
	add	fp, r2
	str	r0, [sp, #12]
	cmp	fp, r3
	beq.n	.L_080d913a
	b.n	.L_080d8d28
.L_080d913a:
	ldr	r0, [pc, #84]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #128
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00007828
	.4byte 0x02010000
	.4byte 0x030001d8
	.4byte 0x00000139
	.4byte 0x0000027a
	.4byte 0xfffffec6
	.4byte 0x080ede84
	.4byte 0x080ede96
	.4byte 0x080ede48
	.4byte 0xfffff000
	.4byte 0x00007824
	.4byte 0x080cd261

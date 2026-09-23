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
	.set sub_080049ac, 0x080049ac
	.set sub_08004c1c, 0x08004c1c
	.set sub_08004c6c, 0x08004c6c
	.set sub_08004cf0, 0x08004cf0
	.set sub_080051d8, 0x080051d8
	.set sub_08005340, 0x08005340
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080cef64, 0x080cef64
	.set sub_080d40ec, 0x080d40ec
	.set sub_080de2f8, 0x080de2f8
	.set sub_080e3944, 0x080e3944
	.set sub_080e396c, 0x080e396c
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Func_080d05fc
	.thumb_func
Func_080d05fc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #136]
	adds	r3, r6, #0
	ldmia	r3!, {r1}
	sub	sp, #132
	str	r1, [sp, #64]
	ldr	r3, [r3, #0]
	str	r3, [sp, #60]
	adds	r3, r6, #0
	subs	r3, #108
	ldr	r3, [r3, #0]
	ldr	r2, [r6, #8]
	str	r3, [sp, #48]
	ldr	r3, [pc, #116]
	adds	r5, r0, #0
	adds	r7, r1, r3
	str	r5, [r7, #0]
	movs	r0, #1
	mov	r8, r2
	bl	sub_080cd594
	ldr	r2, [r7, #0]
	ldr	r3, [r2, #28]
	cmp	r3, #1
	bne.n	.L_080d064e
	add	r3, sp, #80
	ldr	r2, [r2, #4]
	str	r3, [sp, #0]
	add	r3, sp, #76
	str	r3, [sp, #4]
	adds	r0, r5, #0
	movs	r1, #3
	movs	r3, #0
	bl	sub_080de2f8
.L_080d064e:
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #60]
	ldr	r0, [pc, #76]
	strh	r3, [r2, #0]
	bl	sub_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #68]
	movs	r2, #128
	adds	r1, r5, #0
	lsls	r0, r0, #19
	bl	sub_080072f0
	adds	r5, #128
	ldr	r1, [sp, #64]
	adds	r0, r5, #0
	bl	sub_08005340
	ldr	r0, [pc, #48]
	bl	sub_08002f40
	mov	r1, r8
	bl	sub_08005340
	ldr	r0, [pc, #40]
	bl	sub_08002f40
	movs	r3, #128
	ldr	r2, [sp, #64]
	lsls	r3, r3, #5
	b.n	.L_080d06b0
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x00000079
	.4byte 0x03001388
	.4byte 0x00000073
	.2byte 0x0076
	.2byte 0x0000
.L_080d06b0:
	adds	r1, r2, r3
	bl	sub_08005340
	ldr	r0, [pc, #952]
	bl	sub_08002f40
	movs	r3, #128
	ldr	r2, [sp, #64]
	adds	r5, r0, #0
	lsls	r3, r3, #6
	adds	r5, #128
	adds	r1, r2, r3
	adds	r0, r5, #0
	bl	sub_08005340
	movs	r3, #239
	ldr	r1, [sp, #64]
	lsls	r3, r3, #7
	adds	r2, r1, r3
	movs	r3, #3
	str	r3, [r2, #0]
	ldr	r3, [pc, #920]
	adds	r2, r1, r3
	ldr	r3, [pc, #920]
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #916]
	bl	sub_080041d8
	ldr	r3, [r7, #0]
	mov	r2, sp
	adds	r2, #120
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	adds	r1, r2, #0
	str	r2, [sp, #40]
	bl	sub_080e396c
	ldr	r3, [sp, #40]
	ldr	r2, [r3, #0]
	movs	r3, #64
	subs	r3, r3, r2
	ldr	r2, [pc, #888]
	str	r3, [sp, #44]
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	movs	r0, #142
	bl	sub_080f9010
	movs	r1, #0
	str	r1, [sp, #56]
	ldr	r3, [r7, #0]
	ldr	r2, [r3, #20]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	movs	r2, #72
	lsls	r3, r3, #2
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_080d072c
	b.n	.L_080d0ab2
.L_080d072c:
	mov	r3, sp
	adds	r3, #68
	str	r3, [sp, #20]
.L_080d0732:
	ldr	r1, [sp, #56]
	cmp	r1, #64
	bne.n	.L_080d073e
	movs	r0, #0
	bl	sub_080b50e8
.L_080d073e:
	ldr	r2, [pc, #836]
	movs	r3, #0
	ldr	r0, [sp, #56]
	ldr	r1, [pc, #832]
	bl	sub_080d40ec
	ldr	r3, [pc, #832]
	ldr	r2, [sp, #64]
	adds	r7, r2, r3
	ldr	r3, [r7, #0]
	ldr	r3, [r3, #28]
	cmp	r3, #1
	bne.n	.L_080d07e2
	ldr	r1, [sp, #56]
	lsls	r5, r1, #11
	adds	r0, r5, #0
	bl	sub_08002322
	lsls	r3, r0, #2
	adds	r3, r3, r0
	ldr	r2, [sp, #80]
	lsls	r3, r3, #2
	asrs	r3, r3, #16
	adds	r3, r3, r2
	ldr	r2, [sp, #44]
	adds	r3, r3, r2
	subs	r3, #20
	adds	r0, r5, #0
	mov	r8, r3
	bl	sub_0800231c
	ldr	r3, [sp, #76]
	lsls	r0, r0, #2
	asrs	r0, r0, #16
	adds	r0, r0, r3
	ldr	r3, [r7, #0]
	adds	r6, r0, #0
	ldr	r1, [sp, #20]
	ldr	r0, [r3, #4]
	bl	sub_080cef64
	ldr	r3, [sp, #56]
	subs	r6, #24
	cmp	r3, #32
	ble.n	.L_080d07a0
	lsls	r3, r3, #1
	subs	r3, r6, r3
	adds	r6, r3, #0
	adds	r6, #64
.L_080d07a0:
	ldr	r1, [sp, #64]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r7, r1, r2
	movs	r5, #40
	adds	r3, r6, #0
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	ldr	r4, [sp, #68]
	ldr	r0, [sp, #60]
	adds	r1, r7, #0
	mov	r2, r8
	bl	sub_080072f4
	ldr	r3, [sp, #56]
	cmp	r3, #3
	bgt.n	.L_080d07d6
	ldr	r1, [sp, #20]
	str	r5, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #60]
	ldr	r4, [r1, #4]
	mov	r2, r8
	adds	r1, r7, #0
	adds	r3, r6, #0
	bl	sub_080072f4
.L_080d07d6:
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
.L_080d07e2:
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [pc, #668]
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r1, #7
	str	r3, [sp, #68]
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r2, [pc, #648]
	ldr	r1, [sp, #20]
	ldr	r3, [r2, #0]
	str	r3, [r1, #4]
	ldr	r2, [sp, #56]
	cmp	r2, #16
	ble.n	.L_080d082c
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080d082c
	ldr	r3, [sp, #64]
	ldr	r1, [pc, #592]
	adds	r2, r3, r1
	ldr	r3, [r2, #0]
	ldr	r1, [pc, #624]
	adds	r3, r3, r1
	str	r3, [r2, #0]
.L_080d082c:
	ldr	r1, [sp, #56]
	lsls	r3, r1, #1
	adds	r3, r3, r1
	movs	r2, #0
	lsls	r3, r3, #9
	str	r2, [sp, #52]
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	mov	fp, r1
.L_080d083e:
	ldr	r2, [sp, #64]
	ldr	r1, [pc, #584]
	adds	r3, r2, r1
	ldr	r1, [sp, #52]
	ldr	r2, [r3, #0]
	lsls	r3, r1, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	sub_080b5098
	mov	r2, fp
	ldr	r6, [r0, #0]
	cmp	r2, #95
	bls.n	.L_080d085c
	b.n	.L_080d0a18
.L_080d085c:
	bl	sub_080049ac
	ldr	r0, [sp, #48]
	adds	r1, r0, #0
	adds	r1, #12
	bl	sub_080051d8
	ldr	r3, [r6, #8]
	add	r5, sp, #108
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	add	r3, sp, #96
	mov	r9, r3
	mov	r1, r9
	adds	r0, r5, #0
	bl	sub_080e3944
	ldr	r1, [sp, #40]
	ldr	r2, [sp, #44]
	ldr	r3, [r1, #0]
	mov	r1, r9
	adds	r3, r3, r2
	str	r3, [r1, #0]
	ldr	r3, [r1, #4]
	mov	r2, fp
	subs	r3, #24
	str	r3, [r1, #4]
	cmp	r2, #67
	ble.n	.L_080d089e
	b.n	.L_080d09dc
.L_080d089e:
	movs	r1, #0
	str	r1, [sp, #16]
	ldr	r1, [sp, #12]
	lsls	r2, r1, #3
	subs	r2, r2, r1
	ldr	r1, [sp, #64]
	lsls	r2, r2, #2
	adds	r2, r2, r1
	movs	r1, #225
	lsls	r1, r1, #7
	adds	r5, r2, r1
	movs	r3, #168
	ldr	r2, [sp, #8]
	lsls	r3, r3, #10
	subs	r2, r3, r2
	mov	r1, fp
	movs	r3, #64
	subs	r3, r3, r1
	lsls	r3, r3, #9
	movs	r7, #0
	add	r6, sp, #84
	mov	r8, r2
	mov	sl, r3
.L_080d08cc:
	bl	sub_080049ac
	mov	r2, fp
	cmp	r2, #63
	bgt.n	.L_080d08f0
	mov	r3, r8
	str	r3, [r6, #0]
	str	r3, [r6, #4]
	str	r3, [r6, #8]
	adds	r0, r6, #0
	bl	sub_08004cf0
	mov	r0, sl
	bl	sub_08004c6c
	mov	r0, sl
	bl	sub_08004c1c
.L_080d08f0:
	ldr	r0, [sp, #16]
	bl	sub_08004c6c
	add	r2, sp, #108
	adds	r1, r2, #0
	ldr	r0, [pc, #416]
	bl	sub_080e3944
	mov	r1, r9
	ldr	r3, [r1, #0]
	ldr	r2, [sp, #108]
	adds	r2, r2, r3
	str	r2, [r5, #12]
	ldr	r3, [sp, #112]
	ldr	r2, [r1, #4]
	adds	r3, r3, r2
	adds	r3, #16
	str	r3, [r5, #16]
	ldr	r2, [sp, #16]
	ldr	r3, [pc, #364]
	adds	r7, #1
	adds	r2, r2, r3
	str	r2, [sp, #16]
	adds	r5, #28
	cmp	r7, #3
	bne.n	.L_080d08cc
	ldr	r1, [sp, #12]
	str	r1, [sp, #32]
	movs	r7, #0
.L_080d092a:
	ldr	r3, [sp, #32]
	adds	r2, r7, r3
	lsls	r3, r2, #3
	ldr	r1, [sp, #64]
	subs	r3, r3, r2
	lsls	r3, r3, #2
	movs	r2, #225
	lsls	r2, r2, #7
	adds	r3, r1, r3
	adds	r7, #1
	adds	r3, r3, r2
	movs	r1, #3
	adds	r0, r7, #0
	str	r3, [sp, #36]
	str	r7, [sp, #24]
	bl	sub_080022fc
	ldr	r3, [sp, #32]
	adds	r0, r0, r3
	lsls	r3, r0, #3
	subs	r3, r3, r0
	ldr	r1, [sp, #64]
	lsls	r3, r3, #2
	movs	r2, #225
	lsls	r2, r2, #7
	adds	r3, r1, r3
	adds	r3, r3, r2
	mov	r2, fp
	str	r3, [sp, #28]
	cmp	r2, #0
	bge.n	.L_080d096a
	adds	r2, #15
.L_080d096a:
	asrs	r2, r2, #4
	movs	r3, #5
	subs	r3, r3, r2
	mov	r8, r3
	mov	r1, r8
	movs	r3, #0
	mov	sl, r3
	lsls	r7, r1, #1
.L_080d097a:
	ldr	r2, [sp, #28]
	ldr	r1, [sp, #36]
	ldr	r3, [r2, #12]
	ldr	r6, [r1, #12]
	subs	r3, r3, r6
	mov	r0, sl
	muls	r0, r3
	movs	r1, #24
	bl	sub_080022ec
	ldr	r2, [sp, #28]
	ldr	r1, [sp, #36]
	ldr	r3, [r2, #16]
	ldr	r5, [r1, #16]
	subs	r3, r3, r5
	adds	r6, r6, r0
	movs	r1, #24
	mov	r0, sl
	muls	r0, r3
	bl	sub_080022ec
	ldr	r2, [pc, #248]
	subs	r3, r7, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #64]
	adds	r5, r5, r0
	adds	r1, r3, r1
	movs	r2, #128
	mov	r3, r8
	lsls	r2, r2, #5
	subs	r6, r6, r3
	subs	r5, r5, r3
	adds	r1, r1, r2
	str	r7, [sp, #0]
	adds	r2, r6, #0
	str	r7, [sp, #4]
	ldr	r4, [sp, #68]
	ldr	r0, [sp, #60]
	adds	r3, r5, #0
	bl	sub_080072f4
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #24
	bne.n	.L_080d097a
	ldr	r7, [sp, #24]
	cmp	r7, #3
	bne.n	.L_080d092a
.L_080d09dc:
	mov	r3, fp
	cmp	r3, #63
	ble.n	.L_080d0a18
	mov	r1, r9
	ldr	r2, [r1, #0]
	ldr	r3, [r1, #4]
	movs	r5, #48
	movs	r1, #24
	subs	r2, #24
	subs	r3, #24
	str	r1, [sp, #0]
	ldr	r4, [sp, #68]
	ldr	r1, [sp, #64]
	str	r5, [sp, #4]
	ldr	r0, [sp, #60]
	bl	sub_080072f4
	mov	r3, r9
	movs	r1, #24
	ldr	r2, [r3, #0]
	ldr	r3, [r3, #4]
	str	r1, [sp, #0]
	str	r5, [sp, #4]
	ldr	r1, [sp, #20]
	subs	r3, #24
	ldr	r4, [r1, #4]
	ldr	r0, [sp, #60]
	ldr	r1, [sp, #64]
	bl	sub_080072f4
.L_080d0a18:
	ldr	r3, [sp, #8]
	ldr	r1, [pc, #136]
	ldr	r2, [sp, #12]
	adds	r3, r3, r1
	adds	r2, #32
	str	r3, [sp, #8]
	ldr	r3, [sp, #52]
	str	r2, [sp, #12]
	movs	r2, #8
	negs	r2, r2
	adds	r3, #1
	add	fp, r2
	str	r3, [sp, #52]
	cmp	r3, #1
	beq.n	.L_080d0a38
	b.n	.L_080d083e
.L_080d0a38:
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r2, [pc, #96]
	ldr	r1, [sp, #64]
	adds	r3, r1, r2
	ldr	r1, [sp, #52]
	movs	r0, #1
	str	r1, [r3, #0]
	bl	sub_080030f8
	ldr	r2, [sp, #56]
	adds	r2, #1
	str	r2, [sp, #56]
	ldr	r1, [sp, #64]
	ldr	r2, [pc, #44]
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #20]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	ldr	r1, [sp, #56]
	adds	r3, #72
	b.n	.L_080d0aac
	.4byte 0x0000008f
	.4byte 0x00007784
	.4byte 0x04040404
	.4byte 0x080cd261
	.4byte 0x04000028
	.4byte 0x00005555
	.4byte 0x0000aaab
	.4byte 0x00007828
	.4byte 0x03001e50
	.4byte 0x03001f0c
	.4byte 0x01010101
	.4byte 0x080ee128
	.4byte 0x080ede5c
	.4byte 0xffffd000
	.2byte 0x7824
	.2byte 0x0000
.L_080d0aac:
	cmp	r1, r3
	beq.n	.L_080d0ab2
	b.n	.L_080d0732
.L_080d0ab2:
	ldr	r0, [pc, #28]
	bl	sub_08004278
	bl	sub_080cdbc0
	add	sp, #132
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080cd261

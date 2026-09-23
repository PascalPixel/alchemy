.syntax unified
	.thumb
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_08014274, 0x08014274
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_08014878, 0x08014878
	.set sub_0801489c, 0x0801489c
	.set sub_08014cc0, 0x08014cc0
	.set sub_0801587c, 0x0801587c
	.set sub_080cad84, 0x080cad84
	.set sub_080cda84, 0x080cda84
	.set sub_080cdf5c, 0x080cdf5c
	.set sub_080ce458, 0x080ce458
	.set sub_080ceafc, 0x080ceafc
	.set sub_080d170c, 0x080d170c
	.set sub_080d17ac, 0x080d17ac
	.set sub_080d2d84, 0x080d2d84
	.set sub_080d3118, 0x080d3118
	.set sub_080db9a8, 0x080db9a8
	.set sub_080dbd5c, 0x080dbd5c
	.set sub_080dbdc8, 0x080dbdc8
	.set sub_080dbdd4, 0x080dbdd4
	.set sub_080dbe80, 0x080dbe80
	.set sub_080dc294, 0x080dc294
	.set sub_080dc37c, 0x080dc37c
	.set sub_080dc384, 0x080dc384
	.set sub_080dc390, 0x080dc390
	.set sub_080eaf98, 0x080eaf98
	.set sub_080eb01c, 0x080eb01c
	.set sub_080eb298, 0x080eb298
	.set sub_080eb2f0, 0x080eb2f0
	.set sub_081c0010, 0x081c0010
	.global Func_080e781c
	.thumb_func
Func_080e781c:
	push	{lr}
	bl	sub_080cdf5c
	bl	sub_080cad84
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #181
	lsls	r1, r1, #1
	adds	r2, r3, r1
	movs	r3, #252
	lsls	r3, r3, #8
	adds	r3, #137
	strh	r3, [r2, #0]
	pop	{pc}
	push	{lr}
	bl	.L_080e7844
	pop	{pc}
.L_080e7844:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #132
	lsls	r1, r1, #6
	adds	r1, #96
	movs	r0, #92
	sub	sp, #100
	bl	sub_08014cc0
	movs	r5, #192
	str	r0, [sp, #72]
	lsls	r5, r5, #18
	adds	r3, r5, #0
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r1, #1
	str	r3, [sp, #68]
	negs	r1, r1
	ldr	r0, [r5, #60]
	movs	r2, #0
	str	r0, [sp, #64]
	movs	r6, #20
	ldr	r7, [r3, #16]
	str	r1, [sp, #24]
	str	r1, [sp, #32]
	str	r2, [sp, #20]
	bl	sub_080dc294
	mov	r3, sp
	adds	r3, #76
	str	r3, [sp, #16]
	ldr	r3, [r7, #8]
	ldr	r4, [sp, #16]
	movs	r0, #128
	str	r3, [r4, #0]
	ldr	r3, [r7, #12]
	lsls	r0, r0, #14
	str	r3, [r4, #4]
	ldr	r3, [r7, #16]
	str	r3, [r4, #8]
	ldr	r2, [sp, #68]
	ldrh	r1, [r2, #0]
	ldr	r2, [sp, #16]
	bl	sub_0801489c
	movs	r3, #145
	lsls	r3, r3, #1
	movs	r4, #0
	mov	r8, r3
.L_080e78b0:
	ldr	r3, [r5, #108]
	ldr	r1, [r3, r6]
	ldr	r3, [r1, #0]
	ldr	r2, [r1, #80]
	cmp	r3, #0
	beq.n	.L_080e78e4
	ldr	r3, [r2, #40]
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, r8
	bne.n	.L_080e78e4
	ldrb	r3, [r2, #24]
	cmp	r3, #1
	bne.n	.L_080e78e4
	ldrh	r3, [r1, #32]
	adds	r2, r1, #0
	adds	r2, #8
	subs	r3, #2
	ldr	r0, [sp, #16]
	movs	r1, #4
	str	r4, [sp, #4]
	bl	sub_080dbe80
	ldr	r4, [sp, #4]
	cmp	r0, #0
	bge.n	.L_080e794e
.L_080e78e4:
	adds	r4, #1
	adds	r6, #4
	cmp	r4, #79
	ble.n	.L_080e78b0
	ldr	r2, [sp, #68]
	movs	r1, #30
	ldrsh	r0, [r2, r1]
	adds	r1, r7, #0
	ldr	r2, [sp, #16]
	bl	sub_080cda84
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_080e790a
	movs	r4, #0
	str	r4, [sp, #28]
	str	r4, [sp, #20]
	b.n	.L_080e796a
.L_080e790a:
	ldr	r1, [sp, #16]
	adds	r5, r7, #0
	adds	r5, #34
	ldr	r0, [r1, #0]
	ldrb	r2, [r5, #0]
	ldr	r1, [r1, #8]
	bl	sub_080dbd5c
	cmp	r0, #0
	beq.n	.L_080e7966
	ldr	r2, [sp, #16]
	ldr	r1, [r2, #8]
	ldr	r0, [r2, #0]
	ldrb	r2, [r5, #0]
	bl	sub_080dbdc8
	ldr	r4, [sp, #68]
	adds	r2, r0, #0
	movs	r0, #160
	lsls	r0, r0, #23
	movs	r3, #30
	ldrsh	r1, [r4, r3]
	adds	r0, #5
	bl	sub_080ce458
	str	r0, [sp, #20]
	ldr	r1, [sp, #20]
	movs	r0, #2
	str	r0, [sp, #28]
	cmp	r1, #0
	bne.n	.L_080e796a
	movs	r2, #1
	str	r2, [sp, #28]
	b.n	.L_080e796a
.L_080e794e:
	movs	r3, #2
	adds	r0, r4, #0
	str	r3, [sp, #28]
	str	r4, [sp, #24]
	bl	sub_080d2d84
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	b.n	.L_080e796a
.L_080e7966:
	movs	r4, #0
	str	r4, [sp, #28]
.L_080e796a:
	ldr	r0, [pc, #520]
	bl	sub_08013300
	movs	r2, #152
	ldr	r1, [sp, #64]
	lsls	r2, r2, #5
	adds	r2, #72
	adds	r3, r1, r2
	ldrh	r5, [r3, #0]
	ldr	r3, [pc, #504]
	ldr	r1, [sp, #72]
	lsls	r5, r5, #5
	adds	r5, r5, r3
	bl	sub_0801587c
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [sp, #72]
	adds	r1, r5, #0
	ldr	r2, [pc, #488]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r4, sp
	adds	r4, #88
	str	r4, [sp, #12]
	ldr	r0, [sp, #16]
	ldr	r3, [r0, #0]
	str	r3, [r4, #0]
	ldr	r3, [r0, #4]
	str	r3, [r4, #4]
	ldr	r3, [r0, #8]
	str	r3, [r4, #8]
	ldr	r0, [sp, #12]
	bl	sub_080dc390
	movs	r3, #128
	ldr	r1, [sp, #72]
	ldr	r4, [sp, #12]
	lsls	r3, r3, #5
	adds	r3, #80
	adds	r2, r1, r3
	ldr	r3, [r4, #0]
	movs	r0, #128
	str	r3, [r2, #0]
	lsls	r0, r0, #5
	ldr	r3, [r4, #8]
	adds	r0, #84
	adds	r2, r1, r0
	str	r3, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #5
	movs	r4, #128
	adds	r2, #88
	lsls	r4, r4, #5
	adds	r4, #92
	adds	r3, r1, r2
	movs	r2, #0
	str	r2, [r3, #0]
	adds	r3, r1, r4
	str	r2, [r3, #0]
	ldr	r0, [pc, #408]
	bl	sub_08013300
	ldr	r1, [sp, #72]
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r1, #128
	lsls	r1, r1, #4
	ldr	r2, [sp, #72]
	str	r0, [sp, #48]
	bl	sub_080142d4
	str	r0, [sp, #44]
	ldr	r0, [sp, #72]
	movs	r1, #211
	movs	r2, #131
	movs	r3, #13
	lsls	r1, r1, #5
	lsls	r2, r2, #5
	negs	r3, r3
	adds	r6, r0, r1
	adds	r5, r0, r2
	adds	r7, r3, #0
	movs	r4, #63
.L_080e7a18:
	ldr	r0, [sp, #44]
	movs	r3, #128
	str	r0, [sp, #0]
	movs	r1, #8
	adds	r0, r5, #0
	movs	r2, #15
	lsls	r3, r3, #23
	str	r4, [sp, #4]
	bl	sub_080eaf98
	movs	r3, #250
	strh	r3, [r5, #30]
	ldrb	r3, [r5, #5]
	ldrb	r1, [r5, #9]
	movs	r2, #32
	ldr	r4, [sp, #4]
	orrs	r3, r2
	strb	r3, [r5, #5]
	ands	r1, r7
	movs	r3, #15
	ands	r1, r3
	subs	r4, #1
	subs	r3, #16
	strb	r1, [r5, #9]
	str	r3, [r6, #24]
	adds	r5, #40
	adds	r6, #28
	cmp	r4, #0
	bge.n	.L_080e7a18
	movs	r0, #130
	bl	sub_081c0010
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #128
	ldr	r2, [sp, #72]
	lsls	r3, r3, #5
	adds	r3, #88
	movs	r1, #0
	adds	r3, r2, r3
	str	r1, [sp, #40]
	str	r1, [sp, #36]
	str	r1, [sp, #60]
	str	r1, [sp, #56]
	str	r1, [sp, #52]
	str	r3, [sp, #8]
.L_080e7a76:
	ldr	r4, [sp, #36]
	cmp	r4, #0
	beq.n	.L_080e7b1c
	movs	r1, #204
	ldr	r2, [sp, #16]
	lsls	r1, r1, #6
	movs	r0, #63
	adds	r1, #51
	mov	r9, r0
	mov	sl, r1
	mov	r8, r2
.L_080e7a8c:
	ldr	r2, [sp, #40]
	mov	r3, r9
	ands	r2, r3
	ldr	r0, [sp, #72]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #3
	movs	r1, #131
	adds	r3, r0, r3
	lsls	r1, r1, #5
	adds	r7, r3, r1
	mov	r3, sl
	str	r3, [r7, #20]
	str	r3, [r7, #24]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r0, r3
	movs	r0, #211
	mov	r1, r8
	lsls	r0, r0, #5
	adds	r6, r3, r0
	ldr	r3, [r1, #0]
	str	r4, [sp, #4]
	str	r3, [r6, #0]
	ldr	r3, [r1, #4]
	str	r3, [r6, #4]
	ldr	r3, [r1, #8]
	str	r3, [r6, #8]
	bl	sub_08014878
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r2, #144
	lsls	r2, r2, #12
	lsls	r5, r5, #2
	adds	r5, r5, r2
	bl	sub_08014878
	adds	r2, r6, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	bl	sub_08014878
	lsls	r0, r0, #5
	lsrs	r0, r0, #16
	str	r0, [r6, #12]
	bl	sub_08014878
	movs	r3, #128
	lsls	r0, r0, #9
	lsls	r3, r3, #1
	lsrs	r0, r0, #16
	adds	r0, r0, r3
	str	r0, [r6, #20]
	bl	sub_08014878
	movs	r3, #0
	add	r0, sl
	str	r0, [r6, #16]
	str	r3, [r6, #24]
	ldr	r0, [sp, #40]
	ldr	r4, [sp, #4]
	adds	r0, #1
	mov	r1, r9
	ands	r0, r1
	subs	r4, #1
	str	r0, [sp, #40]
	cmp	r4, #0
	bne.n	.L_080e7a8c
.L_080e7b1c:
	ldr	r2, [sp, #72]
	movs	r3, #211
	movs	r4, #131
	lsls	r4, r4, #5
	lsls	r3, r3, #5
	ldr	r5, [sp, #12]
	adds	r6, r2, r3
	adds	r2, r2, r4
	mov	r8, r2
	movs	r4, #63
.L_080e7b30:
	ldr	r2, [r6, #24]
	cmp	r2, #0
	blt.n	.L_080e7bda
	ldr	r0, [sp, #44]
	movs	r3, #7
	asrs	r2, r2, #2
	ands	r2, r3
	ldr	r3, [pc, #44]
	lsls	r2, r2, #3
	mov	r7, r8
	adds	r2, r0, r2
	ands	r2, r3
	ldr	r1, [pc, #36]
	ldrh	r3, [r7, #8]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r7, #8]
	movs	r1, #200
	ldr	r3, [r7, #20]
	lsls	r1, r1, #5
	adds	r1, #153
	movs	r2, #128
	adds	r3, r3, r1
	lsls	r2, r2, #9
	str	r3, [r7, #20]
	cmp	r3, r2
	ble.n	.L_080e7b84
	movs	r3, #128
	lsls	r3, r3, #9
	b.n	.L_080e7b84
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x000001d9
	.4byte 0x06011000
	.4byte 0x84000400
	.2byte 0x01ec
	.2byte 0x0000
.L_080e7b84:
	str	r3, [r7, #20]
	str	r3, [r7, #24]
	adds	r0, r5, #0
	ldr	r3, [r6, #0]
	str	r4, [sp, #4]
	str	r3, [r5, #0]
	ldr	r3, [r6, #4]
	str	r3, [r5, #4]
	ldr	r3, [r6, #8]
	str	r3, [r5, #8]
	bl	sub_080dc390
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #20]
	adds	r0, r3, #0
	muls	r0, r2
	bl	sub_08002096
	ldr	r3, [r5, #0]
	lsls	r0, r0, #3
	adds	r3, r3, r0
	str	r3, [r7, #12]
	adds	r0, r7, #0
	ldr	r3, [r5, #8]
	str	r3, [r7, #16]
	bl	sub_080eb01c
	ldr	r3, [r6, #12]
	ldr	r2, [r6, #16]
	adds	r3, #1
	str	r3, [r6, #12]
	ldr	r3, [r6, #4]
	ldr	r4, [sp, #4]
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r3, [r6, #24]
	adds	r3, #1
	str	r3, [r6, #24]
	cmp	r3, #32
	bne.n	.L_080e7bda
	movs	r3, #1
	negs	r3, r3
	str	r3, [r6, #24]
.L_080e7bda:
	movs	r3, #40
	subs	r4, #1
	add	r8, r3
	adds	r6, #28
	cmp	r4, #0
	bge.n	.L_080e7b30
	ldr	r0, [sp, #52]
	movs	r4, #0
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r7, r3, #1
.L_080e7bf0:
	ldr	r1, [sp, #32]
	movs	r2, #1
	negs	r2, r2
	movs	r6, #0
	cmp	r1, r2
	beq.n	.L_080e7c06
	ldr	r3, [sp, #32]
	cmp	r3, #0
	bge.n	.L_080e7c04
	adds	r3, #3
.L_080e7c04:
	asrs	r6, r3, #2
.L_080e7c06:
	adds	r0, r7, r4
	lsls	r0, r0, #11
	str	r4, [sp, #4]
	bl	sub_08002096
	lsls	r3, r0, #3
	ldr	r0, [sp, #52]
	asrs	r3, r3, #16
	adds	r5, r3, #0
	adds	r5, #23
	ldr	r4, [sp, #4]
	cmp	r0, #0
	bge.n	.L_080e7c22
	adds	r0, #3
.L_080e7c22:
	asrs	r0, r0, #2
	adds	r0, r0, r4
	movs	r3, #144
	lsls	r3, r3, #7
	lsls	r0, r0, #11
	adds	r0, r0, r3
	str	r4, [sp, #4]
	bl	sub_08002096
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	asrs	r3, r3, #16
	adds	r1, r6, #0
	adds	r0, r3, #0
	adds	r1, #28
	adds	r0, #12
	ldr	r4, [sp, #4]
	cmp	r1, #31
	ble.n	.L_080e7c4c
	movs	r1, #31
.L_080e7c4c:
	adds	r5, r5, r6
	cmp	r5, #31
	ble.n	.L_080e7c54
	movs	r5, #31
.L_080e7c54:
	adds	r0, r0, r6
	cmp	r0, #31
	ble.n	.L_080e7c5c
	movs	r0, #31
.L_080e7c5c:
	lsls	r3, r5, #5
	lsls	r2, r0, #10
	orrs	r2, r3
	ldr	r0, [pc, #804]
	movs	r3, #255
	subs	r3, r3, r4
	lsls	r3, r3, #1
	orrs	r2, r1
	adds	r3, r3, r0
	strh	r2, [r3, #0]
	adds	r4, #1
	cmp	r4, #31
	ble.n	.L_080e7bf0
	ldr	r1, [sp, #32]
	movs	r2, #1
	negs	r2, r2
	cmp	r1, r2
	beq.n	.L_080e7c88
	cmp	r1, #126
	bgt.n	.L_080e7c88
	adds	r1, #1
	str	r1, [sp, #32]
.L_080e7c88:
	ldr	r4, [sp, #72]
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #80
	adds	r3, r4, r0
	ldr	r3, [r3, #0]
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r1, #84
	mov	sl, r3
	movs	r2, #128
	adds	r3, r4, r1
	ldr	r3, [r3, #0]
	lsls	r2, r2, #5
	adds	r2, #88
	adds	r4, r4, r2
	ldr	r6, [r4, #0]
	mov	r8, r3
	mov	r9, r4
	ldr	r3, [sp, #72]
	movs	r4, #128
	ldr	r0, [sp, #64]
	lsls	r4, r4, #5
	movs	r1, #152
	adds	r4, #92
	lsls	r1, r1, #5
	adds	r3, r3, r4
	adds	r1, #72
	ldr	r5, [r3, #0]
	mov	fp, r3
	adds	r3, r0, r1
	ldrh	r3, [r3, #0]
	ldr	r2, [sp, #72]
	subs	r4, #92
	adds	r3, #128
	adds	r7, r2, r4
	str	r3, [sp, #0]
	movs	r3, #192
	adds	r0, r7, #0
	movs	r1, #31
	movs	r2, #31
	lsls	r3, r3, #24
	bl	sub_080eaf98
	ldrb	r3, [r7, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r7, #9]
	strb	r3, [r7, #5]
	movs	r3, #15
	ands	r3, r2
	strb	r3, [r7, #9]
	ldr	r1, [sp, #68]
	ldr	r0, [r1, #16]
	ldrh	r1, [r1, #0]
	bl	sub_080db9a8
	movs	r3, #3
	ands	r0, r3
	movs	r4, #13
	ldrb	r3, [r7, #9]
	negs	r4, r4
	adds	r2, r4, #0
	ands	r3, r2
	lsls	r0, r0, #2
	orrs	r3, r0
	strb	r3, [r7, #9]
	mov	r0, sl
	movs	r3, #1
	mov	r1, r8
	str	r0, [r7, #12]
	strh	r3, [r7, #30]
	str	r1, [r7, #16]
	str	r6, [r7, #20]
	str	r5, [r7, #24]
	adds	r0, r7, #0
	bl	sub_080eb01c
	ldr	r2, [sp, #28]
	cmp	r2, #0
	bne.n	.L_080e7dac
	ldr	r3, [sp, #60]
	cmp	r3, #1
	beq.n	.L_080e7d7a
	cmp	r3, #1
	bgt.n	.L_080e7d3a
	cmp	r3, #0
	beq.n	.L_080e7d42
	b.n	.L_080e7dac
.L_080e7d3a:
	ldr	r4, [sp, #60]
	cmp	r4, #2
	beq.n	.L_080e7d8c
	b.n	.L_080e7dac
.L_080e7d42:
	ldr	r0, [sp, #56]
	cmp	r0, #10
	bne.n	.L_080e7d52
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #30
	bl	sub_081c0010
.L_080e7d52:
	mov	r1, r9
	ldr	r3, [r1, #0]
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r4, fp
	str	r3, [r1, #0]
	str	r3, [r4, #0]
	movs	r0, #252
	ldr	r3, [r1, #0]
	lsls	r0, r0, #6
	adds	r0, #255
	cmp	r3, r0
	ble.n	.L_080e7dac
	movs	r2, #1
	movs	r1, #1
	negs	r2, r2
	str	r1, [sp, #60]
	str	r2, [sp, #56]
	b.n	.L_080e7dac
.L_080e7d7a:
	ldr	r3, [sp, #56]
	cmp	r3, #30
	bne.n	.L_080e7dac
	movs	r0, #1
	movs	r4, #2
	negs	r0, r0
	str	r4, [sp, #60]
	str	r0, [sp, #56]
	b.n	.L_080e7dac
.L_080e7d8c:
	movs	r1, #0
	str	r1, [sp, #36]
	mov	r2, r9
	ldr	r3, [r2, #0]
	ldr	r4, [pc, #500]
	mov	r0, fp
	adds	r3, r3, r4
	str	r3, [r2, #0]
	str	r3, [r0, #0]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bgt.n	.L_080e7dac
	movs	r1, #186
	lsls	r1, r1, #2
	adds	r1, #255
	str	r1, [sp, #60]
.L_080e7dac:
	ldr	r2, [sp, #28]
	cmp	r2, #1
	bne.n	.L_080e7e5e
	ldr	r3, [sp, #60]
	cmp	r3, #1
	beq.n	.L_080e7e0a
	cmp	r3, #1
	bgt.n	.L_080e7dc2
	cmp	r3, #0
	beq.n	.L_080e7dce
	b.n	.L_080e7e5e
.L_080e7dc2:
	ldr	r4, [sp, #60]
	cmp	r4, #2
	beq.n	.L_080e7e1e
	cmp	r4, #3
	beq.n	.L_080e7e34
	b.n	.L_080e7e5e
.L_080e7dce:
	ldr	r0, [sp, #56]
	cmp	r0, #30
	bne.n	.L_080e7dda
	movs	r0, #140
	bl	sub_081c0010
.L_080e7dda:
	ldr	r1, [sp, #8]
	movs	r2, #192
	ldr	r3, [r1, #0]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r1, #0]
	ldr	r4, [sp, #72]
	movs	r0, #128
	lsls	r0, r0, #5
	adds	r0, #92
	adds	r2, r4, r0
	str	r3, [r2, #0]
	ldr	r3, [r1, #0]
	movs	r1, #191
	lsls	r1, r1, #8
	adds	r1, #255
	cmp	r3, r1
	ble.n	.L_080e7e5e
	movs	r3, #1
	movs	r2, #1
	negs	r3, r3
	str	r2, [sp, #60]
	str	r3, [sp, #56]
	b.n	.L_080e7e5e
.L_080e7e0a:
	ldr	r0, [sp, #56]
	movs	r4, #2
	str	r4, [sp, #36]
	cmp	r0, #40
	bne.n	.L_080e7e5e
	movs	r1, #1
	negs	r1, r1
	str	r4, [sp, #60]
	str	r1, [sp, #56]
	b.n	.L_080e7e5e
.L_080e7e1e:
	ldr	r3, [sp, #56]
	movs	r2, #1
	str	r2, [sp, #36]
	cmp	r3, #40
	bne.n	.L_080e7e5e
	movs	r0, #1
	movs	r4, #3
	negs	r0, r0
	str	r4, [sp, #60]
	str	r0, [sp, #56]
	b.n	.L_080e7e5e
.L_080e7e34:
	movs	r1, #0
	str	r1, [sp, #36]
	ldr	r2, [sp, #8]
	ldr	r4, [pc, #340]
	ldr	r3, [r2, #0]
	movs	r1, #128
	adds	r3, r3, r4
	str	r3, [r2, #0]
	ldr	r0, [sp, #72]
	lsls	r1, r1, #5
	adds	r1, #92
	adds	r2, r0, r1
	str	r3, [r2, #0]
	ldr	r2, [sp, #8]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bgt.n	.L_080e7e5e
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	str	r3, [sp, #60]
.L_080e7e5e:
	ldr	r4, [sp, #28]
	cmp	r4, #2
	bne.n	.L_080e7efc
	ldr	r0, [sp, #60]
	cmp	r0, #1
	beq.n	.L_080e7eba
	cmp	r0, #1
	bgt.n	.L_080e7e74
	cmp	r0, #0
	beq.n	.L_080e7e7c
	b.n	.L_080e7efc
.L_080e7e74:
	ldr	r1, [sp, #60]
	cmp	r1, #2
	beq.n	.L_080e7eea
	b.n	.L_080e7efc
.L_080e7e7c:
	ldr	r2, [sp, #56]
	cmp	r2, #40
	bne.n	.L_080e7e88
	movs	r0, #140
	bl	sub_081c0010
.L_080e7e88:
	ldr	r4, [sp, #8]
	movs	r0, #192
	ldr	r3, [r4, #0]
	lsls	r0, r0, #2
	adds	r3, r3, r0
	str	r3, [r4, #0]
	ldr	r1, [sp, #72]
	movs	r4, #128
	lsls	r4, r4, #5
	adds	r4, #92
	adds	r2, r1, r4
	str	r3, [r2, #0]
	ldr	r0, [sp, #8]
	movs	r1, #191
	ldr	r3, [r0, #0]
	lsls	r1, r1, #8
	adds	r1, #255
	cmp	r3, r1
	ble.n	.L_080e7efc
	movs	r3, #1
	movs	r2, #1
	negs	r3, r3
	str	r2, [sp, #60]
	str	r3, [sp, #56]
	b.n	.L_080e7efc
.L_080e7eba:
	ldr	r4, [sp, #56]
	cmp	r4, #40
	bne.n	.L_080e7ed6
	movs	r0, #0
	str	r0, [sp, #32]
	movs	r0, #254
	lsls	r0, r0, #7
	adds	r0, #255
	movs	r1, #0
	bl	sub_080d170c
	movs	r0, #100
	bl	sub_080d17ac
.L_080e7ed6:
	ldr	r2, [sp, #56]
	movs	r1, #2
	str	r1, [sp, #36]
	cmp	r2, #100
	bne.n	.L_080e7efc
	movs	r3, #1
	negs	r3, r3
	str	r1, [sp, #60]
	str	r3, [sp, #56]
	b.n	.L_080e7efc
.L_080e7eea:
	ldr	r0, [sp, #56]
	movs	r4, #1
	str	r4, [sp, #36]
	cmp	r0, #80
	bne.n	.L_080e7efc
	movs	r1, #186
	lsls	r1, r1, #2
	adds	r1, #255
	str	r1, [sp, #60]
.L_080e7efc:
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #56]
	ldr	r3, [sp, #52]
	movs	r0, #186
	ldr	r4, [sp, #60]
	lsls	r0, r0, #2
	adds	r2, #1
	adds	r3, #1
	adds	r0, #255
	str	r2, [sp, #56]
	str	r3, [sp, #52]
	cmp	r4, r0
	beq.n	.L_080e7f1c
	b.n	.L_080e7a76
.L_080e7f1c:
	ldr	r1, [sp, #24]
	movs	r2, #1
	negs	r2, r2
	cmp	r1, r2
	beq.n	.L_080e7f46
	adds	r0, r1, #0
	bl	sub_080d2d84
	ldr	r3, [r0, #8]
	ldr	r1, [r0, #16]
	adds	r0, #34
	ldrb	r2, [r0, #0]
	adds	r0, r3, #0
	movs	r3, #0
	bl	sub_080dbdd4
	movs	r1, #0
	ldr	r0, [sp, #24]
	movs	r2, #0
	bl	sub_080d3118
.L_080e7f46:
	ldr	r3, [sp, #20]
	cmp	r3, #0
	beq.n	.L_080e7f5c
	ldr	r0, [sp, #68]
	movs	r4, #24
	ldrsh	r1, [r0, r4]
	movs	r3, #26
	ldrsh	r2, [r0, r3]
	ldr	r0, [sp, #20]
	bl	sub_080ceafc
.L_080e7f5c:
	ldr	r0, [sp, #48]
	bl	sub_08014274
	ldr	r4, [sp, #28]
	cmp	r4, #2
	bne.n	.L_080e7f70
	movs	r0, #100
	bl	sub_080dc37c
	b.n	.L_080e7f74
.L_080e7f70:
	bl	sub_080dc384
.L_080e7f74:
	movs	r0, #92
	bl	sub_0801314c
	add	sp, #100
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x05000200
	.4byte 0xfffffd00
	.2byte 0xfc00
	.2byte 0xffff
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	mov	r8, r2
	mov	r0, r8
	mov	r9, r1
	mov	fp, r3
	bl	sub_08002090
	ldr	r3, [pc, #56]
	mov	r1, r9
	mov	sl, r3
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x1c05
	mov	r0, r8
	bl	sub_08002096
	mov	r1, r9
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x263f
	asrs	r0, r0, #16
	asrs	r5, r5, #16
	subs	r5, r6, r5
	subs	r6, r6, r0
	adds	r1, r6, #0
	adds	r0, r5, #0
	mov	r2, fp
	bl	sub_080eb2f0
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r3}
	mov	fp, r3
	pop	{r5, r6, pc}
	.2byte 0x021c
	.2byte 0x0300
.L_080e7fec:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #124]
	adds	r7, r0, #0
	adds	r5, r1, #0
	mov	r8, r2
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_080e8002
	adds	r3, r7, #7
.L_080e8002:
	asrs	r7, r3, #3
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_080e800c
	adds	r3, r5, #7
.L_080e800c:
	asrs	r5, r3, #3
	ldr	r3, [pc, #100]
	movs	r1, #128
	lsls	r1, r1, #3
	movs	r2, #0
	ldr	r0, [pc, #96]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2600
	adds	r1, r5, #0
	cmp	r1, #0
	blt.n	.L_080e805a
	cmp	r1, #19
	bgt.n	.L_080e806a
.L_080e8028:
	adds	r0, r7, #0
	movs	r4, #0
	cmp	r0, #0
	blt.n	.L_080e804a
	cmp	r0, #29
	bgt.n	.L_080e805a
.L_080e8034:
	ldr	r5, [pc, #64]
	lsls	r3, r6, #5
	adds	r3, r3, r4
	lsls	r2, r6, #4
	lsls	r3, r3, #1
	adds	r2, r2, r4
	adds	r3, r3, r5
	lsls	r2, r2, #1
	mov	r5, r8
	ldrh	r2, [r2, r5]
	strh	r2, [r3, #0]
.L_080e804a:
	adds	r4, #1
	adds	r0, #1
	cmp	r4, #15
	bgt.n	.L_080e805a
	cmp	r0, #0
	blt.n	.L_080e804a
	cmp	r0, #29
	ble.n	.L_080e8034
.L_080e805a:
	adds	r6, #1
	adds	r1, #1
	cmp	r6, #15
	bgt.n	.L_080e806a
	cmp	r1, #0
	blt.n	.L_080e805a
	cmp	r1, #19
	ble.n	.L_080e8028
.L_080e806a:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x080f39c4
	.4byte 0x03000260
	.2byte 0x2000
	.2byte 0x0600
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	ldr	r3, [r3, #108]
	movs	r2, #133
	mov	fp, r3
	ldr	r3, [pc, #152]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	sl, r0
	ldr	r0, [r3, #0]
	sub	sp, #16
	bl	sub_080d2d84
	ldr	r3, [r0, #8]
	add	r7, sp, #4
	str	r3, [r7, #0]
	mov	r9, r0
	ldr	r3, [r0, #12]
	movs	r0, #128
	lsls	r0, r0, #13
	adds	r3, r3, r0
	str	r3, [r7, #4]
	mov	r2, r9
	ldr	r3, [r2, #16]
	str	r3, [r7, #8]
	movs	r3, #164
	lsls	r3, r3, #6
	adds	r3, #133
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080e80ea
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #174
	add	r3, fp
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	movs	r0, #128
	lsls	r1, r1, #11
	lsls	r0, r0, #10
	negs	r1, r1
	adds	r2, r7, #0
	bl	sub_0801489c
.L_080e80ea:
	adds	r0, r7, #0
	bl	sub_080dc390
	movs	r3, #2
	ldrsh	r0, [r7, r3]
	ldr	r3, [pc, #52]
	ldr	r4, [pc, #60]
	subs	r2, r3, r0
	strh	r2, [r4, #0]
	movs	r2, #10
	ldrsh	r1, [r7, r2]
	subs	r0, #64
	subs	r3, r3, r1
	strh	r3, [r4, #2]
	subs	r1, #64
	bl	.L_080e7fec
	movs	r3, #164
	lsls	r3, r3, #6
	adds	r3, #134
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080e8198
	movs	r6, #164
	lsls	r6, r6, #6
	movs	r3, #0
	adds	r6, #136
	mov	r8, r3
	add	r6, sl
	b.n	.L_080e8138
	.4byte 0x00000040
	.4byte 0x02000240
	.2byte 0x1120
	.2byte 0x0300
.L_080e8138:
	ldrh	r0, [r6, #0]
	lsrs	r0, r0, #1
	add	r0, r8
	lsls	r0, r0, #11
	bl	sub_08002096
	lsls	r5, r0, #3
	ldrh	r0, [r6, #0]
	movs	r2, #144
	lsrs	r0, r0, #2
	add	r0, r8
	lsls	r2, r2, #7
	lsls	r0, r0, #11
	adds	r0, r0, r2
	bl	sub_08002096
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	asrs	r5, r5, #16
	asrs	r3, r3, #16
	adds	r5, #23
	adds	r3, #12
	lsls	r3, r3, #10
	lsls	r5, r5, #5
	orrs	r3, r5
	movs	r0, #28
	orrs	r3, r0
	ldr	r0, [pc, #220]
	mov	r2, r8
	lsls	r1, r2, #1
	adds	r2, r1, r0
	strh	r3, [r2, #0]
	ldr	r2, [pc, #216]
	adds	r1, r1, r2
	strh	r3, [r1, #0]
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	cmp	r0, #15
	ble.n	.L_080e8138
	movs	r2, #164
	lsls	r2, r2, #6
	adds	r2, #136
	add	r2, sl
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
.L_080e8198:
	movs	r3, #164
	lsls	r3, r3, #6
	adds	r3, #132
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080e8276
	movs	r2, #164
	movs	r6, #152
	movs	r4, #132
	lsls	r2, r2, #6
	lsls	r6, r6, #6
	lsls	r4, r4, #6
	adds	r2, #130
	movs	r3, #15
	add	r6, sl
	add	r4, sl
	mov	r8, r3
	add	sl, r2
.L_080e81c2:
	ldr	r5, [r6, #24]
	cmp	r5, #0
	bne.n	.L_080e8200
	str	r4, [sp, #0]
	bl	sub_08014878
	ldr	r4, [sp, #0]
	adds	r2, r7, #0
	strh	r0, [r4, #28]
	str	r5, [r7, #8]
	str	r5, [r7, #4]
	str	r5, [r7, #0]
	movs	r0, #128
	ldrh	r1, [r4, #28]
	lsls	r0, r0, #6
	adds	r1, r1, r0
	movs	r0, #216
	lsls	r0, r0, #14
	bl	sub_0801489c
	ldr	r3, [r7, #0]
	movs	r2, #128
	str	r3, [r6, #0]
	ldr	r3, [r7, #4]
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r3, [r7, #8]
	ldr	r5, [r6, #24]
	str	r3, [r6, #8]
	ldr	r4, [sp, #0]
.L_080e8200:
	cmp	r5, #0
	blt.n	.L_080e825a
	asrs	r2, r5, #2
	movs	r3, #3
	ands	r2, r3
	mov	r3, sl
	ldrh	r1, [r3, #0]
	ldr	r3, [pc, #56]
	lsls	r2, r2, #2
	adds	r1, r1, r2
	ands	r1, r3
	ldr	r2, [pc, #52]
	ldrh	r3, [r4, #8]
	mov	r0, r9
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r4, #8]
	adds	r1, r7, #0
	ldr	r3, [r0, #8]
	ldr	r2, [r6, #0]
	str	r4, [sp, #0]
	adds	r3, r3, r2
	str	r3, [r7, #0]
	ldr	r3, [r0, #12]
	ldr	r2, [r6, #4]
	adds	r3, r3, r2
	str	r3, [r7, #4]
	ldr	r3, [r0, #16]
	ldr	r2, [r6, #8]
	adds	r0, r4, #0
	adds	r3, r3, r2
	str	r3, [r7, #8]
	bl	sub_080eb298
	ldr	r5, [r6, #24]
	b.n	.L_080e8258
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x050001e0
	.2byte 0x03e0
	.2byte 0x0500
.L_080e8258:
	ldr	r4, [sp, #0]
.L_080e825a:
	adds	r3, r5, #1
	str	r3, [r6, #24]
	cmp	r3, #16
	bne.n	.L_080e8266
	movs	r3, #0
	str	r3, [r6, #24]
.L_080e8266:
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
	mov	r3, r8
	adds	r4, #40
	adds	r6, #28
	cmp	r3, #0
	bge.n	.L_080e81c2
.L_080e8276:
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #173
	add	r3, fp
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080e829e
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r2, #174
	add	r2, fp
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	ldrh	r1, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080e82ae
	subs	r3, r1, #1
	strh	r3, [r2, #0]
.L_080e829e:
	movs	r3, #192
	lsls	r3, r3, #4
	adds	r3, #174
	add	r3, fp
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_080e82bc
.L_080e82ae:
	movs	r2, #179
	movs	r3, #128
	lsls	r2, r2, #1
	lsls	r3, r3, #6
	add	r2, fp
	adds	r3, #155
	strh	r3, [r2, #0]
.L_080e82bc:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000

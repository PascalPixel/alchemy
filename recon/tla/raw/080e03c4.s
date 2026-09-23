.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0800206c, 0x0800206c
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_08014274, 0x08014274
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_0801489c, 0x0801489c
	.set sub_08014cc0, 0x08014cc0
	.set sub_0801587c, 0x0801587c
	.set sub_08016ce4, 0x08016ce4
	.set sub_08020030, 0x08020030
	.set sub_08020090, 0x08020090
	.set sub_080200c0, 0x080200c0
	.set sub_080200c8, 0x080200c8
	.set sub_080200e8, 0x080200e8
	.set sub_08020210, 0x08020210
	.set sub_08020218, 0x08020218
	.set sub_08020278, 0x08020278
	.set sub_08020280, 0x08020280
	.set sub_08038040, 0x08038040
	.set sub_08038120, 0x08038120
	.set sub_080c8978, 0x080c8978
	.set sub_080c89a6, 0x080c89a6
	.set sub_080cb82c, 0x080cb82c
	.set sub_080cb8a4, 0x080cb8a4
	.set sub_080d22a8, 0x080d22a8
	.set sub_080d3838, 0x080d3838
	.set sub_080d440c, 0x080d440c
	.set sub_080d92a4, 0x080d92a4
	.set sub_080d92d4, 0x080d92d4
	.set sub_080d9a74, 0x080d9a74
	.set sub_080db9c0, 0x080db9c0
	.set sub_080db9cc, 0x080db9cc
	.set sub_080dbe80, 0x080dbe80
	.set sub_080dbed0, 0x080dbed0
	.set sub_080dc044, 0x080dc044
	.set sub_080dc10c, 0x080dc10c
	.set sub_080dc294, 0x080dc294
	.set sub_080dc384, 0x080dc384
	.set sub_080dc390, 0x080dc390
	.set sub_080eaf98, 0x080eaf98
	.set sub_080eb01c, 0x080eb01c
	.set sub_080eb298, 0x080eb298
	.set sub_080eb4a0, 0x080eb4a0
	.set sub_080ebe70, 0x080ebe70
	.set sub_080ebea8, 0x080ebea8
	.set sub_080ebeb4, 0x080ebeb4
	.set sub_080ebec8, 0x080ebec8
	.set sub_080ebf68, 0x080ebf68
	.set sub_081c0010, 0x081c0010
	.global Func_080e03c4
	.thumb_func
Func_080e03c4:
	push	{lr}
	bl	.L_080e03cc
	pop	{pc}
.L_080e03cc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r5, [r3, #0]
	sub	sp, #40
	ldr	r1, [r5, #20]
	ldr	r7, [r5, #16]
	str	r1, [sp, #0]
	ldr	r3, [r7, #8]
	add	r2, sp, #16
	str	r3, [r2, #0]
	ldr	r3, [r7, #12]
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r2, #4]
	ldr	r3, [r7, #16]
	mov	fp, r2
	str	r3, [r2, #8]
	adds	r3, r5, #0
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080e042c
	ldr	r3, [r7, #8]
	add	r2, sp, #4
	str	r3, [r2, #0]
	ldr	r3, [r7, #12]
	movs	r0, #128
	lsls	r0, r0, #14
	adds	r3, r3, r0
	str	r3, [r2, #4]
	ldr	r3, [r7, #16]
	mov	r9, r2
	str	r3, [r2, #8]
	ldrh	r1, [r5, #0]
	bl	sub_0801489c
	b.n	.L_080e0444
.L_080e042c:
	add	r3, sp, #4
	mov	r9, r3
	ldr	r3, [r5, #4]
	mov	r1, r9
	str	r3, [r1, #0]
	movs	r2, #128
	ldr	r3, [r5, #8]
	lsls	r2, r2, #14
	adds	r3, r3, r2
	str	r3, [r1, #4]
	ldr	r3, [r5, #12]
	str	r3, [r1, #8]
.L_080e0444:
	ldr	r1, [r5, #4]
	add	r0, sp, #28
	str	r1, [r0, #0]
	movs	r3, #128
	ldr	r2, [r5, #8]
	lsls	r3, r3, #14
	adds	r2, r2, r3
	str	r2, [r0, #4]
	ldr	r3, [r5, #12]
	str	r3, [r0, #8]
	movs	r0, #139
	lsls	r0, r0, #1
	bl	sub_080dc10c
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_080e0468
	b.n	.L_080e05fe
.L_080e0468:
	bl	sub_080dc294
	movs	r0, #138
	bl	sub_081c0010
	ldrh	r3, [r7, #6]
	ldr	r2, [pc, #40]
	strh	r3, [r6, #6]
	movs	r3, #166
	lsls	r3, r3, #9
	adds	r3, #204
	str	r3, [r6, #48]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #5
	bl	sub_08020090
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08020278
	movs	r7, #0
	mov	sl, fp
	mov	r8, r9
	b.n	.L_080e04a4
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_080e04a4:
	mov	r2, sl
	mov	r1, r8
	ldr	r5, [r2, #0]
	ldr	r3, [r1, #0]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	bl	sub_08002054
	adds	r5, r5, r0
	str	r5, [r6, #8]
	mov	r2, sl
	mov	r1, r8
	ldr	r5, [r2, #4]
	ldr	r3, [r1, #4]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	bl	sub_08002054
	adds	r5, r5, r0
	str	r5, [r6, #12]
	mov	r2, sl
	mov	r1, r8
	ldr	r5, [r2, #8]
	ldr	r3, [r1, #8]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	bl	sub_08002054
	movs	r3, #192
	lsls	r3, r3, #8
	adds	r5, r5, r0
	movs	r1, #10
	adds	r0, r7, #0
	muls	r0, r3
	str	r5, [r6, #16]
	bl	sub_08002054
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r0, r0, r3
	str	r0, [r6, #24]
	str	r0, [r6, #28]
	adds	r7, #1
	movs	r0, #1
	bl	sub_08013560
	cmp	r7, #11
	blt.n	.L_080e04a4
	movs	r0, #10
	bl	sub_08013560
	adds	r0, r6, #0
	movs	r1, #6
	bl	sub_08020090
	movs	r0, #15
	bl	sub_08013560
	movs	r5, #9
.L_080e0526:
	ldr	r3, [r6, #12]
	ldr	r1, [pc, #224]
	movs	r0, #1
	adds	r3, r3, r1
	str	r3, [r6, #12]
	subs	r5, #1
	bl	sub_08013560
	cmp	r5, #0
	bge.n	.L_080e0526
	adds	r0, r6, #0
	movs	r1, #5
	bl	sub_08020090
	movs	r0, #132
	bl	sub_081c0010
	ldr	r2, [sp, #0]
	cmp	r2, #0
	beq.n	.L_080e055a
	ldr	r3, [pc, #192]
	ldr	r2, [r2, #12]
	ldr	r0, [sp, #0]
	adds	r1, r3, #0
	bl	sub_080200e8
.L_080e055a:
	movs	r0, #20
	bl	sub_08013560
	movs	r5, #12
.L_080e0562:
	ldr	r3, [r6, #12]
	movs	r1, #192
	lsls	r1, r1, #9
	adds	r3, r3, r1
	str	r3, [r6, #12]
	movs	r0, #1
	subs	r5, #1
	bl	sub_08013560
	cmp	r5, #0
	bge.n	.L_080e0562
	movs	r0, #10
	bl	sub_08013560
	movs	r0, #114
	bl	sub_081c0010
	movs	r7, #0
	mov	sl, r9
	mov	r8, fp
.L_080e058a:
	mov	r2, r8
	mov	r1, sl
	ldr	r3, [r2, #0]
	ldr	r5, [r1, #0]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	bl	sub_08002054
	adds	r5, r5, r0
	str	r5, [r6, #8]
	mov	r2, r8
	mov	r1, sl
	ldr	r3, [r2, #4]
	ldr	r5, [r1, #4]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	bl	sub_08002054
	adds	r5, r5, r0
	str	r5, [r6, #12]
	mov	r2, r8
	mov	r1, sl
	ldr	r3, [r2, #8]
	ldr	r5, [r1, #8]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r7, #0
	muls	r0, r3
	bl	sub_08002054
	ldr	r3, [pc, #68]
	adds	r5, r5, r0
	movs	r1, #10
	adds	r0, r7, #0
	muls	r0, r3
	str	r5, [r6, #16]
	bl	sub_08002054
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r0, r0, r2
	str	r0, [r6, #24]
	str	r0, [r6, #28]
	adds	r7, #1
	movs	r0, #1
	bl	sub_08013560
	cmp	r7, #11
	blt.n	.L_080e058a
	adds	r0, r6, #0
	bl	sub_080200c8
	bl	sub_080dc384
.L_080e05fe:
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xfffe0000
	.4byte 0xfff70000
	.2byte 0x4000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r2, r3, #0
	adds	r2, #224
	ldr	r2, [r2, #0]
	sub	sp, #24
	ldr	r0, [r3, #92]
	str	r2, [sp, #8]
	mov	r8, r0
	ldr	r3, [r3, #108]
	movs	r2, #132
	str	r3, [sp, #4]
	movs	r3, #132
	lsls	r3, r3, #6
	add	r3, r8
	ldr	r3, [r3, #0]
	lsls	r2, r2, #6
	adds	r2, #16
	mov	sl, r3
	add	r2, r8
	movs	r3, #0
	strh	r3, [r2, #0]
	mov	r9, r3
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #8
	add	r3, r8
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r9, r3
	bge.n	.L_080e070c
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #10
	add	r2, r8
	mov	fp, r2
.L_080e066e:
	mov	r1, fp
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	mov	r0, sl
	lsls	r6, r3, #3
	subs	r6, r6, r3
	lsls	r6, r6, #2
	ldr	r3, [r0, #8]
	movs	r2, #208
	lsls	r2, r2, #5
	add	r6, r8
	adds	r7, r6, r2
	str	r3, [r7, #0]
	ldr	r3, [r0, #12]
	str	r3, [r7, #4]
	ldr	r3, [r0, #16]
	str	r3, [r7, #8]
	bl	sub_08014878
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r1, #128
	lsls	r1, r1, #11
	lsls	r5, r5, #2
	adds	r5, r5, r1
	bl	sub_08014878
	adds	r2, r7, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	movs	r2, #0
	str	r2, [r7, #12]
	bl	sub_08014878
	movs	r3, #128
	lsls	r0, r0, #1
	lsls	r3, r3, #10
	subs	r3, r3, r0
	str	r3, [r7, #16]
	movs	r3, #0
	str	r3, [r7, #20]
	bl	sub_08014878
	adds	r5, r0, #0
	movs	r0, #192
	lsls	r0, r0, #9
	adds	r5, r5, r0
	bl	sub_08014878
	movs	r2, #208
	lsls	r2, r2, #5
	adds	r2, #12
	adds	r6, r6, r2
	adds	r1, r0, #0
	adds	r2, r6, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	movs	r3, #0
	str	r3, [r7, #24]
	mov	r0, fp
	ldrh	r3, [r0, #0]
	movs	r2, #63
	adds	r3, #1
	ands	r3, r2
	mov	r1, fp
	strh	r3, [r1, #0]
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #8
	add	r3, r8
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	movs	r2, #1
	add	r9, r2
	cmp	r9, r3
	blt.n	.L_080e066e
.L_080e070c:
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #12
	add	r2, r8
	movs	r7, #208
	movs	r5, #128
	str	r2, [sp, #0]
	ldr	r6, [pc, #48]
	movs	r1, #0
	lsls	r7, r7, #5
	lsls	r5, r5, #5
	movs	r3, #3
	mov	r9, r1
	add	r7, r8
	add	r5, r8
	mov	fp, r3
.L_080e072c:
	ldr	r1, [r7, #24]
	cmp	r1, #0
	blt.n	.L_080e0798
	movs	r3, #1
	mov	r2, r9
	ands	r3, r2
	adds	r0, r5, #0
	cmp	r3, #0
	beq.n	.L_080e0750
	mov	r3, fp
	ands	r1, r3
	lsls	r3, r1, #1
	ldr	r1, [sp, #0]
	ldrh	r2, [r1, #0]
	b.n	.L_080e0760
	movs	r0, r0
	.2byte 0xfc00
	.2byte 0xffff
.L_080e0750:
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #14
	add	r2, r8
	mov	r3, fp
	ldrh	r2, [r2, #0]
	ands	r1, r3
	lsls	r3, r1, #1
.L_080e0760:
	adds	r2, r2, r3
	ldr	r3, [pc, #44]
	ands	r2, r3
	ldrh	r3, [r5, #8]
	ands	r3, r6
	orrs	r3, r2
	strh	r3, [r5, #8]
	adds	r1, r7, #0
	bl	sub_080eb298
	adds	r0, r7, #0
	movs	r1, #63
	ldr	r2, [pc, #24]
	bl	sub_080c8978
	ldr	r3, [r7, #24]
	adds	r3, #1
	str	r3, [r7, #24]
	cmp	r3, #32
	bne.n	.L_080e0798
	movs	r3, #1
	negs	r3, r3
	str	r3, [r7, #24]
	b.n	.L_080e0798
	.4byte 0x000003ff
	.2byte 0xec00
	.2byte 0xffff
.L_080e0798:
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	adds	r5, #40
	adds	r7, #28
	cmp	r1, #63
	ble.n	.L_080e072c
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #4
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #4
	bls.n	.L_080e07b8
	b.n	.L_080e0954
.L_080e07b8:
	ldr	r2, [pc, #436]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080e07d4
	.4byte 0x080e082e
	.4byte 0x080e0850
	.4byte 0x080e08da
	.2byte 0x0924
	.2byte 0x080e
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	ldr	r1, [sp, #8]
	lsls	r3, r5, #1
	ldr	r2, [r1, #4]
	adds	r3, r3, r5
	adds	r2, r2, r3
	lsls	r3, r0, #1
	adds	r3, r3, r0
	add	r6, sp, #12
	subs	r2, r2, r3
	str	r2, [r6, #0]
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	ldr	r1, [sp, #8]
	lsls	r3, r5, #1
	ldr	r2, [r1, #8]
	adds	r3, r3, r5
	adds	r2, r2, r3
	lsls	r3, r0, #1
	adds	r3, r3, r0
	subs	r2, r2, r3
	str	r2, [r6, #4]
	mov	r0, sl
	ldr	r3, [r1, #12]
	ldr	r1, [r6, #0]
	str	r3, [r6, #8]
	bl	sub_080200e8
	movs	r1, #132
	lsls	r1, r1, #6
	adds	r1, #6
	add	r1, r8
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	cmp	r3, #20
	beq.n	.L_080e082c
	b.n	.L_080e0954
.L_080e082c:
	b.n	.L_080e090c
	ldr	r3, [sp, #8]
	mov	r0, sl
	ldr	r1, [r3, #4]
	ldr	r2, [r3, #8]
	ldr	r3, [r3, #12]
	bl	sub_080200e8
	movs	r1, #132
	lsls	r1, r1, #6
	adds	r1, #6
	add	r1, r8
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r3, #6
	beq.n	.L_080e084e
	b.n	.L_080e0954
.L_080e084e:
	b.n	.L_080e090c
	movs	r6, #132
	lsls	r6, r6, #6
	adds	r6, #6
	add	r6, r8
	movs	r1, #0
	ldrsh	r3, [r6, r1]
	cmp	r3, #0
	bne.n	.L_080e086c
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #16
	add	r2, r8
	movs	r3, #1
	strh	r3, [r2, #0]
.L_080e086c:
	ldr	r2, [sp, #4]
	movs	r0, #208
	lsls	r0, r0, #4
	adds	r0, #49
	add	r5, sp, #12
	adds	r3, r2, r0
	movs	r0, #0
	ldrsb	r0, [r3, r0]
	adds	r1, r5, #0
	bl	sub_080d92a4
	ldr	r3, [r5, #0]
	mov	r1, sl
	str	r3, [r1, #8]
	ldr	r2, [pc, #232]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r1, #12]
	ldr	r3, [r5, #8]
	str	r3, [r1, #16]
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #18
	add	r3, r8
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	beq.n	.L_080e08ba
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #4
	add	r3, r8
	ldrh	r2, [r3, #0]
	adds	r2, #1
	strh	r2, [r3, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r6, #0]
.L_080e08ba:
	movs	r1, #0
	ldrsh	r3, [r6, r1]
	cmp	r3, #90
	bne.n	.L_080e0954
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #4
	add	r3, r8
	ldrh	r2, [r3, #0]
	adds	r2, #1
	strh	r2, [r3, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r6, #0]
	b.n	.L_080e0954
	mov	r0, sl
	movs	r1, #3
	bl	sub_08020090
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #8
	movs	r3, #3
	add	r2, r8
	movs	r1, #132
	strh	r3, [r2, #0]
	lsls	r1, r1, #6
	adds	r1, #6
	add	r1, r8
	movs	r3, #0
	ldrsh	r2, [r1, r3]
	mov	r0, sl
	ldr	r3, [r0, #12]
	lsls	r2, r2, #17
	adds	r3, r3, r2
	str	r3, [r0, #12]
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	cmp	r3, #10
	bne.n	.L_080e0954
.L_080e090c:
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #4
	add	r3, r8
	ldrh	r2, [r3, #0]
	adds	r2, #1
	strh	r2, [r3, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r1, #0]
	b.n	.L_080e0954
	movs	r2, #0
	mov	r3, sl
	str	r2, [r3, #24]
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #8
	add	r3, r8
	strh	r2, [r3, #0]
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #6
	add	r3, r8
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #30
	bne.n	.L_080e0954
	movs	r2, #132
	lsls	r2, r2, #6
	movs	r3, #186
	adds	r2, #4
	lsls	r3, r3, #2
	add	r2, r8
	adds	r3, #255
	strh	r3, [r2, #0]
.L_080e0954:
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #6
	add	r2, r8
	ldrh	r3, [r2, #0]
	add	sp, #24
	adds	r3, #1
	strh	r3, [r2, #0]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x080e07c0
	.2byte 0x0000
	.2byte 0xfffc
	.2byte 0xb5e0
	movs	r3, #192
	sub	sp, #8
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	add	r1, sp, #4
	adds	r3, #224
	mov	r2, sp
	adds	r6, r0, #0
	ldr	r7, [r3, #0]
	bl	sub_080d9a74
	cmp	r0, #0
	beq.n	.L_080e099e
	adds	r2, r7, #0
	adds	r2, #32
	movs	r3, #1
	strb	r3, [r2, #0]
	b.n	.L_080e09b8
.L_080e099e:
	movs	r1, #208
	lsls	r1, r1, #4
	adds	r1, #50
	adds	r3, r5, r1
	strb	r6, [r3, #0]
	ldr	r2, [sp, #4]
	adds	r1, #1
	adds	r3, r5, r1
	strb	r2, [r3, #0]
	ldr	r2, [sp, #0]
	subs	r1, #2
	adds	r3, r5, r1
	strb	r2, [r3, #0]
.L_080e09b8:
	bl	.L_080e09c0
	add	sp, #8
	pop	{r5, r6, r7, pc}
.L_080e09c0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #132
	lsls	r1, r1, #6
	adds	r1, #20
	movs	r0, #92
	sub	sp, #16
	bl	sub_08014cc0
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	ldr	r2, [r2, #108]
	adds	r3, #224
	ldr	r3, [r3, #0]
	str	r2, [sp, #12]
	mov	r9, r3
	ldr	r5, [r3, #16]
	ldr	r3, [r3, #20]
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L_080e0a08
	mov	r0, r9
	ldr	r3, [r0, #8]
	movs	r2, #128
	lsls	r2, r2, #11
	adds	r3, r3, r2
	str	r3, [r0, #8]
	ldr	r3, [r0, #12]
	adds	r3, r3, r2
	str	r3, [r0, #12]
.L_080e0a08:
	bl	sub_080dc294
	movs	r0, #108
	adds	r0, #255
	movs	r1, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_080dc10c
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_080e0a22
	b.n	.L_080e0c60
.L_080e0a22:
	ldrh	r3, [r5, #6]
	ldr	r2, [pc, #56]
	mov	r1, r8
	strh	r3, [r1, #6]
	mov	r3, r8
	adds	r3, #85
	strb	r2, [r3, #0]
	adds	r3, r5, #0
	adds	r3, #35
	ldrb	r2, [r3, #0]
	movs	r3, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080e0a48
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #32
	orrs	r3, r2
	strb	r3, [r1, #0]
.L_080e0a48:
	movs	r1, #0
	mov	r0, r8
	bl	sub_08020090
	movs	r0, #138
	bl	sub_081c0010
	ldr	r0, [pc, #12]
	bl	sub_08013300
	b.n	.L_080e0a68
	movs	r0, r0
	.4byte 0x00000000
	.2byte 0x01ea
	.2byte 0x0000
.L_080e0a68:
	mov	r1, sl
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r1, r5, #0
	mov	r2, sl
	str	r0, [sp, #8]
	bl	sub_080142d4
	movs	r3, #132
	lsls	r3, r3, #6
	mov	fp, r0
	adds	r3, #12
	add	r3, sl
	mov	r2, fp
	strh	r2, [r3, #0]
	ldr	r0, [pc, #488]
	bl	sub_08013300
	mov	r1, sl
	bl	sub_0801587c
	bl	sub_080143ac
	adds	r1, r5, #0
	mov	r2, sl
	str	r0, [sp, #4]
	bl	sub_080142d4
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #14
	add	r3, sl
	movs	r6, #208
	movs	r5, #128
	strh	r0, [r3, #0]
	lsls	r6, r6, #5
	lsls	r5, r5, #5
	add	r6, sl
	add	r5, sl
	movs	r7, #63
.L_080e0ac0:
	mov	r3, fp
	str	r3, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #4
	movs	r2, #4
	movs	r3, #0
	bl	sub_080eaf98
	ldrb	r1, [r5, #9]
	movs	r4, #13
	negs	r4, r4
	movs	r3, #250
	strh	r3, [r5, #30]
	adds	r3, r4, #0
	ands	r1, r3
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	strb	r3, [r5, #5]
	movs	r3, #15
	ands	r1, r3
	strb	r1, [r5, #9]
	mov	r0, r8
	ldr	r3, [r0, #8]
	subs	r7, #1
	str	r3, [r6, #0]
	adds	r5, #40
	ldr	r3, [r0, #12]
	str	r3, [r6, #4]
	ldr	r3, [r0, #16]
	str	r3, [r6, #8]
	bl	sub_08014878
	adds	r1, r0, #0
	movs	r0, #192
	adds	r2, r6, #0
	lsls	r0, r0, #12
	bl	sub_0801489c
	movs	r3, #1
	negs	r3, r3
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r7, #0
	bge.n	.L_080e0ac0
	mov	r2, r9
	ldr	r1, [r2, #4]
	ldr	r2, [r2, #8]
	movs	r3, #128
	mov	r4, r9
	lsls	r3, r3, #14
	adds	r2, r2, r3
	mov	r0, r8
	ldr	r3, [r4, #12]
	bl	sub_080dbed0
	mov	r0, r8
	movs	r1, #1
	bl	sub_08020090
	movs	r0, #10
	bl	sub_08013560
	movs	r7, #15
.L_080e0b40:
	mov	r0, r8
	ldr	r3, [r0, #12]
	ldr	r1, [pc, #308]
	subs	r7, #1
	adds	r3, r3, r1
	str	r3, [r0, #12]
	movs	r0, #1
	bl	sub_08013560
	cmp	r7, #0
	bge.n	.L_080e0b40
	movs	r0, #132
	bl	sub_081c0010
	mov	r3, r9
	adds	r3, #32
	movs	r6, #0
	ldrsb	r6, [r3, r6]
	cmp	r6, #0
	beq.n	.L_080e0b84
	movs	r7, #15
.L_080e0b6a:
	mov	r2, r8
	ldr	r0, [r2, #8]
	ldr	r1, [r2, #12]
	subs	r7, #1
	ldr	r2, [r2, #16]
	bl	sub_080dc044
	cmp	r7, #0
	bge.n	.L_080e0b6a
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_080e0c4e
.L_080e0b84:
	movs	r0, #6
	bl	sub_08013560
	movs	r3, #132
	lsls	r3, r3, #6
	add	r3, sl
	mov	r4, r8
	str	r4, [r3, #0]
	movs	r5, #132
	movs	r3, #132
	lsls	r5, r5, #6
	lsls	r3, r3, #6
	adds	r5, #4
	adds	r3, #6
	add	r5, sl
	add	r3, sl
	strh	r6, [r5, #0]
	strh	r6, [r3, #0]
	movs	r3, #132
	lsls	r3, r3, #6
	movs	r2, #132
	adds	r3, #10
	lsls	r2, r2, #6
	add	r3, sl
	adds	r2, #8
	strh	r6, [r3, #0]
	add	r2, sl
	movs	r3, #1
	strh	r3, [r2, #0]
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #18
	add	r3, sl
	movs	r1, #144
	strh	r6, [r3, #0]
	ldr	r0, [pc, #180]
	lsls	r1, r1, #3
	bl	sub_080145a8
	movs	r2, #186
	movs	r0, #0
	ldrsh	r3, [r5, r0]
	lsls	r2, r2, #2
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_080e0c48
	mov	r9, r2
.L_080e0be2:
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #16
	add	r3, sl
	movs	r1, #0
	ldrsh	r5, [r3, r1]
	cmp	r5, #1
	bne.n	.L_080e0c32
	movs	r0, #128
	lsls	r0, r0, #2
	bl	sub_081c0010
	movs	r4, #208
	ldr	r2, [sp, #12]
	lsls	r4, r4, #4
	movs	r1, #208
	adds	r4, #49
	lsls	r1, r1, #4
	adds	r3, r2, r4
	adds	r1, #50
	movs	r0, #0
	ldrsb	r0, [r3, r0]
	adds	r4, #2
	adds	r3, r2, r1
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	adds	r3, r2, r4
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	bl	sub_080d92d4
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #18
	add	r3, sl
	movs	r0, #140
	strh	r5, [r3, #0]
	adds	r0, #255
	bl	sub_081c0010
.L_080e0c32:
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #4
	add	r3, sl
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, r9
	bne.n	.L_080e0be2
.L_080e0c48:
	ldr	r0, [pc, #52]
	bl	sub_08014644
.L_080e0c4e:
	ldr	r0, [sp, #8]
	bl	sub_08014274
	ldr	r0, [sp, #4]
	bl	sub_08014274
	mov	r0, r8
	bl	sub_080200c8
.L_080e0c60:
	bl	sub_080dc384
	movs	r0, #92
	bl	sub_0801314c
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x000001eb
	.4byte 0xfffe0000
	.2byte 0x0619
	.2byte 0x080e
	push	{lr}
	adds	r1, r0, #0
	adds	r1, #100
	movs	r2, #0
	ldrsh	r3, [r1, r2]
	movs	r4, #128
	lsls	r2, r3, #2
	adds	r2, r2, r3
	ldrh	r3, [r0, #6]
	lsls	r2, r2, #4
	adds	r3, r3, r2
	lsls	r4, r4, #5
	adds	r3, r3, r4
	strh	r3, [r0, #6]
	cmp	r2, r4
	bcs.n	.L_080e0caa
	ldrh	r3, [r1, #0]
	adds	r3, #1
	strh	r3, [r1, #0]
.L_080e0caa:
	pop	{pc}
	push	{lr}
	ldr	r1, [pc, #56]
	ldr	r3, [r0, #28]
	ldr	r2, [r0, #24]
	adds	r3, r3, r1
	str	r3, [r0, #28]
	ldrh	r3, [r0, #6]
	adds	r2, r2, r1
	movs	r1, #128
	lsls	r1, r1, #6
	adds	r3, r3, r1
	strh	r3, [r0, #6]
	ldr	r3, [r0, #12]
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r3, r3, r1
	str	r3, [r0, #12]
	movs	r3, #192
	lsls	r3, r3, #6
	str	r2, [r0, #24]
	cmp	r2, r3
	bge.n	.L_080e0ce0
	ldr	r3, [pc, #8]
	adds	r2, r0, #0
	adds	r2, #84
	strb	r3, [r2, #0]
.L_080e0ce0:
	pop	{pc}
	movs	r0, r0
	.4byte 0x00000000
	.2byte 0xfd80
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	adds	r7, r0, #0
	ldr	r2, [r3, #16]
	movs	r3, #64
	adds	r3, r3, r7
	mov	sl, r3
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	sub	sp, #12
	mov	r8, r3
	cmp	r3, #0
	bne.n	.L_080e0d94
	ldr	r3, [r2, #8]
	mov	r6, sp
	str	r3, [r6, #0]
	adds	r0, r6, #0
	ldr	r3, [r2, #12]
	str	r3, [r6, #4]
	ldr	r3, [r2, #16]
	str	r3, [r6, #8]
	bl	sub_080dc390
	ldr	r3, [r6, #0]
	movs	r2, #128
	str	r3, [r7, #20]
	str	r3, [r7, #4]
	lsls	r2, r2, #12
	ldr	r3, [r6, #8]
	adds	r3, r3, r2
	str	r3, [r7, #24]
	str	r3, [r7, #8]
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	lsls	r5, r5, #13
	lsls	r0, r0, #13
	lsrs	r0, r0, #16
	lsrs	r5, r5, #16
	movs	r3, #192
	lsls	r3, r3, #8
	subs	r5, r5, r0
	adds	r5, r5, r3
	movs	r0, #240
	adds	r2, r6, #0
	lsls	r0, r0, #15
	adds	r1, r5, #0
	bl	sub_0801489c
	ldr	r3, [r6, #0]
	mov	r2, r8
	str	r3, [r7, #12]
	ldr	r3, [r6, #8]
	str	r3, [r7, #16]
	movs	r3, #160
	lsls	r3, r3, #11
	str	r3, [r7, #36]
	str	r3, [r7, #32]
	adds	r3, r7, #0
	adds	r3, #66
	strb	r2, [r3, #0]
	mov	r2, sl
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	ldr	r3, [pc, #76]
	movs	r2, #2
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080e0dc4
	movs	r0, #246
	bl	sub_081c0010
	b.n	.L_080e0dc4
.L_080e0d94:
	mov	r3, r8
	cmp	r3, #1
	bne.n	.L_080e0dae
	adds	r0, r7, #0
	bl	sub_080ebe70
	cmp	r0, #0
	bne.n	.L_080e0dc4
	mov	r2, sl
	ldrb	r3, [r2, #0]
	subs	r3, #1
	strb	r3, [r2, #0]
	b.n	.L_080e0dc4
.L_080e0dae:
	mov	r3, r8
	cmp	r3, #2
	bne.n	.L_080e0dc4
	adds	r0, r7, #0
	bl	sub_080ebe70
	cmp	r0, #0
	bne.n	.L_080e0dc4
	adds	r0, r7, #0
	bl	sub_080ebf68
.L_080e0dc4:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r6, [r3, #0]
	sub	sp, #12
	ldr	r7, [r6, #16]
	bl	sub_080d22a8
	movs	r2, #1
	negs	r2, r2
	movs	r3, #0
	adds	r0, r2, #0
	adds	r1, r2, #0
	bl	sub_080d440c
	bl	sub_080dc294
	movs	r0, #10
	bl	sub_08013560
	movs	r1, #128
	movs	r2, #0
	movs	r3, #24
	ldrsh	r0, [r6, r3]
	lsls	r1, r1, #7
	bl	sub_080d3838
	movs	r0, #30
	bl	sub_08013560
	ldr	r3, [pc, #120]
	movs	r0, #131
	str	r3, [r7, #108]
	bl	sub_081c0010
	movs	r1, #28
	adds	r0, r7, #0
	bl	sub_08020090
	movs	r0, #40
	bl	sub_08013560
	movs	r0, #220
	bl	sub_081c0010
	adds	r0, r7, #0
	movs	r1, #0
	bl	sub_08020278
	movs	r1, #3
	adds	r0, r7, #0
	bl	sub_08020090
	ldr	r3, [pc, #80]
	adds	r2, r7, #0
	adds	r2, #100
	str	r3, [r7, #108]
	movs	r3, #0
	strh	r3, [r2, #0]
	movs	r0, #70
	bl	sub_08013560
	adds	r0, r7, #0
	movs	r1, #0
	bl	sub_08020218
	ldr	r5, [pc, #44]
	adds	r3, r7, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	ldr	r3, [pc, #48]
	mov	r5, sp
	str	r3, [r7, #108]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r7, #56]
	ldr	r3, [r7, #8]
	adds	r0, r5, #0
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	adds	r6, #80
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	mov	sl, r6
	str	r3, [r5, #8]
	bl	sub_080dc390
	movs	r3, #0
	mov	r8, r3
	b.n	.L_080e0ea0
	.4byte 0x00000000
	.4byte 0x080db91d
	.4byte 0x080e0c85
	.2byte 0x0cad
	.2byte 0x080e
.L_080e0ea0:
	movs	r1, #168
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #8]
	adds	r0, r6, #0
	lsls	r1, r1, #2
	bl	sub_080ebec8
	adds	r0, r6, #0
	ldr	r1, [pc, #132]
	bl	sub_080ebeb4
	adds	r0, r6, #0
	movs	r1, #7
	bl	sub_080ebea8
	bl	sub_08014878
	lsls	r1, r0, #3
	subs	r1, r1, r0
	lsrs	r1, r1, #16
	ldr	r0, [r6, #0]
	bl	sub_08020280
	bl	sub_08014878
	ldr	r3, [pc, #104]
	lsrs	r0, r0, #1
	adds	r0, r0, r3
	str	r0, [r6, #44]
	str	r0, [r6, #40]
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #1
	add	r8, r3
	mov	r3, r8
	adds	r6, #72
	cmp	r3, #23
	bls.n	.L_080e0ea0
	movs	r0, #70
	bl	sub_08013560
	movs	r3, #0
	mov	r2, sl
	mov	r8, r3
	movs	r1, #2
	adds	r2, #64
.L_080e0efe:
	movs	r3, #5
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L_080e0f08
	strb	r1, [r2, #0]
.L_080e0f08:
	movs	r3, #1
	add	r8, r3
	mov	r3, r8
	adds	r2, #72
	cmp	r3, #23
	bls.n	.L_080e0efe
	movs	r0, #40
	bl	sub_08013560
	bl	sub_080dc384
	movs	r3, #0
	str	r3, [r7, #24]
	ldr	r3, [r7, #20]
	movs	r0, #10
	str	r3, [r7, #12]
	bl	sub_08013560
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x080e0ced
	.2byte 0x3333
	.2byte 0x0001
	push	{r5, r6, lr}
	ldr	r3, [pc, #96]
	ldr	r1, [r0, #20]
	movs	r5, #253
	lsls	r5, r5, #1
	movs	r2, #160
	lsls	r2, r2, #12
	adds	r3, r3, r5
	adds	r4, r1, r2
	movs	r5, #0
	ldrsh	r2, [r3, r5]
	ldr	r3, [pc, #80]
	ldr	r6, [r0, #104]
	cmp	r2, r3
	bne.n	.L_080e0f64
	movs	r2, #128
	lsls	r2, r2, #11
	adds	r4, r1, r2
.L_080e0f64:
	ldr	r5, [r0, #12]
	cmp	r5, r4
	bgt.n	.L_080e0f70
	bl	sub_080200c8
	b.n	.L_080e0fa2
.L_080e0f70:
	ldr	r3, [r0, #24]
	movs	r4, #192
	lsls	r4, r4, #4
	movs	r1, #128
	adds	r2, r3, r4
	lsls	r1, r1, #9
	cmp	r2, r1
	ble.n	.L_080e0f82
	adds	r2, r1, #0
.L_080e0f82:
	str	r2, [r0, #24]
	str	r2, [r0, #28]
	ldr	r4, [pc, #36]
	ldr	r3, [r6, #8]
	str	r3, [r0, #8]
	adds	r3, r5, r4
	str	r3, [r0, #12]
	subs	r3, r1, r2
	lsls	r2, r3, #2
	adds	r2, r2, r3
	ldr	r3, [r6, #16]
	movs	r5, #144
	adds	r3, r3, r2
	lsls	r5, r5, #12
	adds	r3, r3, r5
	str	r3, [r0, #16]
.L_080e0fa2:
	pop	{r5, r6, pc}
	.4byte 0x02000240
	.4byte 0x00000001
	.2byte 0x0000
	.2byte 0xfffe
	.2byte 0xb560
	ldr	r3, [pc, #100]
	ldr	r1, [r0, #20]
	movs	r5, #253
	lsls	r5, r5, #1
	movs	r2, #160
	lsls	r2, r2, #12
	adds	r3, r3, r5
	adds	r4, r1, r2
	movs	r5, #0
	ldrsh	r2, [r3, r5]
	ldr	r3, [pc, #84]
	ldr	r6, [r0, #104]
	cmp	r2, r3
	bne.n	.L_080e0fd4
	movs	r2, #128
	lsls	r2, r2, #11
	adds	r4, r1, r2
.L_080e0fd4:
	ldr	r5, [r0, #12]
	cmp	r5, r4
	bgt.n	.L_080e0fe0
	bl	sub_080200c8
	b.n	.L_080e1014
.L_080e0fe0:
	ldr	r3, [r0, #24]
	movs	r4, #192
	lsls	r4, r4, #4
	movs	r1, #128
	adds	r2, r3, r4
	lsls	r1, r1, #9
	cmp	r2, r1
	ble.n	.L_080e0ff2
	adds	r2, r1, #0
.L_080e0ff2:
	negs	r3, r2
	str	r2, [r0, #24]
	str	r3, [r0, #28]
	ldr	r4, [pc, #36]
	ldr	r3, [r6, #8]
	str	r3, [r0, #8]
	adds	r3, r5, r4
	str	r3, [r0, #12]
	subs	r3, r1, r2
	lsls	r2, r3, #2
	adds	r2, r2, r3
	ldr	r3, [r6, #16]
	movs	r5, #128
	subs	r3, r3, r2
	lsls	r5, r5, #13
	adds	r3, r3, r5
	str	r3, [r0, #16]
.L_080e1014:
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x00000001
	.2byte 0x0000
	.2byte 0xfffe
.L_080e1024:
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	sub	sp, #12
	ldr	r1, [r3, #16]
	movs	r2, #4
	str	r1, [sp, #0]
	mov	sl, r3
	add	r2, sp
	movs	r3, #63
	adds	r6, r0, #0
	movs	r7, #0
	mov	r9, r2
	mov	fp, r3
.L_080e1050:
	ldr	r2, [r6, #12]
	movs	r3, #128
	lsls	r3, r3, #15
	movs	r0, #16
	ldr	r1, [r6, #8]
	adds	r2, r2, r3
	adds	r0, #255
	ldr	r3, [r6, #16]
	bl	sub_080200c0
	lsls	r3, r7, #2
	mov	r1, r9
	str	r0, [r3, r1]
	cmp	r0, #0
	beq.n	.L_080e1108
	ldr	r3, [r6, #20]
	movs	r2, #0
	str	r3, [r0, #20]
	adds	r3, r0, #0
	adds	r3, #85
	ldr	r5, [r0, #80]
	strb	r2, [r3, #0]
	adds	r3, #15
	strh	r2, [r3, #0]
	ldr	r1, [pc, #20]
	movs	r3, #204
	lsls	r3, r3, #7
	adds	r3, #102
	mov	r8, r1
	str	r6, [r0, #104]
	str	r3, [r0, #28]
	str	r3, [r0, #24]
	cmp	r5, #0
	beq.n	.L_080e1108
	b.n	.L_080e109c
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_080e109c:
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_08020030
	mov	r2, r8
	strb	r2, [r5, #26]
	ldrb	r0, [r5, #16]
	bl	sub_08014274
	movs	r3, #226
	lsls	r3, r3, #3
	add	r3, sl
	ldrh	r3, [r3, #0]
	movs	r2, #1
	strb	r3, [r5, #16]
	ldrb	r3, [r5, #17]
	orrs	r3, r2
	strb	r3, [r5, #17]
	ldrb	r3, [r5, #16]
	ldr	r2, [pc, #64]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r1, [r3, #2]
	ldr	r2, [pc, #52]
	ldrh	r3, [r5, #8]
	lsls	r1, r1, #17
	lsrs	r1, r1, #22
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r5, #8]
	movs	r1, #33
	ldrb	r3, [r5, #5]
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	mov	r2, fp
	ands	r3, r2
	movs	r2, #64
	orrs	r3, r2
	ldrb	r2, [r5, #7]
	strb	r3, [r5, #5]
	mov	r3, fp
	ands	r3, r2
	movs	r2, #128
	orrs	r3, r2
	strb	r3, [r5, #7]
	ldr	r3, [r5, #40]
	mov	r1, r8
	strb	r1, [r3, #22]
	b.n	.L_080e1108
	.4byte 0xfffffc00
	.2byte 0x36e0
	.2byte 0x0200
.L_080e1108:
	adds	r7, #1
	cmp	r7, #1
	ble.n	.L_080e1050
	ldr	r2, [sp, #4]
	ldr	r3, [pc, #56]
	ldr	r0, [r2, #80]
	str	r3, [r2, #108]
	ldrb	r1, [r0, #9]
	movs	r2, #13
	negs	r2, r2
	adds	r3, r2, #0
	ands	r3, r1
	strb	r3, [r0, #9]
	mov	r3, r9
	ldr	r1, [r3, #4]
	ldr	r3, [pc, #40]
	ldr	r0, [r1, #80]
	str	r3, [r1, #108]
	ldr	r1, [sp, #0]
	add	sp, #12
	ldr	r3, [r1, #80]
	ldrb	r1, [r3, #9]
	movs	r3, #12
	ands	r3, r1
	ldrb	r1, [r0, #9]
	ands	r2, r1
	orrs	r2, r3
	strb	r2, [r0, #9]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x080e0fb1
	.2byte 0x0f41
	.2byte 0x080e
	push	{r5, r6, lr}
	adds	r5, r0, #0
	adds	r3, r5, #0
	adds	r3, #100
	movs	r1, #0
	ldrsh	r6, [r3, r1]
	adds	r1, r5, #0
	adds	r1, #102
	ldrh	r3, [r1, #0]
	adds	r2, r3, #1
	lsls	r3, r3, #16
	asrs	r0, r3, #16
	ldr	r3, [pc, #68]
	strh	r2, [r1, #0]
	movs	r2, #253
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #60]
	cmp	r2, r3
	bne.n	.L_080e1192
	movs	r1, #7
	bl	sub_08002064
	cmp	r0, #0
	bne.n	.L_080e11a2
	adds	r0, r5, #0
	bl	.L_080e1024
	b.n	.L_080e11a2
.L_080e1192:
	movs	r1, #5
	bl	sub_08002064
	cmp	r0, #0
	bne.n	.L_080e11a2
	adds	r0, r5, #0
	bl	.L_080e1024
.L_080e11a2:
	cmp	r6, #1
	bne.n	.L_080e11b0
	ldrh	r3, [r5, #6]
	movs	r2, #192
	lsls	r2, r2, #4
	adds	r3, r3, r2
	strh	r3, [r5, #6]
.L_080e11b0:
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x02000240
	.2byte 0x0001
	.2byte 0x0000
	push	{r5, lr}
	ldr	r5, [pc, #72]
	movs	r3, #153
	lsls	r3, r3, #2
	adds	r2, r5, r3
	movs	r3, #0
	str	r3, [r2, #0]
	bl	sub_080cb82c
	movs	r2, #154
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080e11f0
	movs	r0, #150
	movs	r1, #4
	bl	sub_08038120
	ldr	r0, [pc, #36]
	movs	r1, #1
	bl	sub_08038040
	b.n	.L_080e1200
.L_080e11f0:
	movs	r0, #236
	movs	r1, #2
	bl	sub_08038120
	ldr	r0, [pc, #20]
	movs	r1, #1
	bl	sub_08038040
.L_080e1200:
	bl	sub_080cb8a4
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x00000dbf
	.2byte 0x0dc1
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	sub	sp, #8
	str	r3, [sp, #4]
	movs	r6, #0
	ldr	r1, [r3, #16]
	ldr	r7, [r1, #80]
	ldrh	r3, [r1, #6]
	ldr	r2, [r7, #40]
	str	r3, [sp, #0]
	mov	r8, r1
	ldrb	r1, [r7, #26]
	mov	sl, r2
	mov	fp, r1
	bl	sub_080143ac
	ldr	r2, [sp, #4]
	movs	r1, #226
	lsls	r1, r1, #3
	adds	r3, r2, r1
	strh	r0, [r3, #0]
	movs	r1, #128
	lsls	r0, r0, #16
	lsls	r1, r1, #1
	ldr	r2, [pc, #276]
	asrs	r0, r0, #16
	bl	sub_080142d4
	ldr	r5, [pc, #272]
	movs	r3, #153
	lsls	r3, r3, #2
	adds	r2, r5, r3
	movs	r3, #150
	lsls	r3, r3, #20
	movs	r0, #70
	str	r3, [r2, #0]
	adds	r0, #255
	bl	sub_08016ce4
	movs	r1, #154
	lsls	r1, r1, #2
	adds	r3, r5, r1
	strb	r0, [r3, #0]
	movs	r1, #0
	mov	r0, r8
	bl	sub_08020278
	ldr	r3, [pc, #236]
	mov	r2, r8
	str	r3, [r2, #108]
	mov	r5, r8
	mov	r3, r8
	adds	r3, #102
	adds	r5, #100
	strh	r6, [r5, #0]
	movs	r0, #140
	strh	r6, [r3, #0]
	bl	sub_081c0010
	movs	r0, #15
	bl	sub_08013560
	movs	r3, #1
	strh	r3, [r5, #0]
	movs	r0, #10
	bl	sub_08013560
	movs	r3, #7
	movs	r6, #1
	movs	r5, #19
	mov	r9, r3
.L_080e12b4:
	mov	r1, r9
	mov	r2, sl
	strb	r1, [r2, #5]
	movs	r0, #2
	strb	r6, [r7, #25]
	bl	sub_08013560
	movs	r3, #0
	mov	r1, sl
	strb	r3, [r1, #5]
	strb	r6, [r7, #25]
	strb	r6, [r7, #26]
	movs	r0, #3
	subs	r5, #1
	bl	sub_08013560
	cmp	r5, #0
	bge.n	.L_080e12b4
	movs	r3, #0
	mov	r2, r8
	str	r3, [r2, #108]
	mov	r3, sp
	ldrh	r3, [r3, #0]
	mov	r1, r8
	strh	r3, [r1, #6]
	movs	r5, #0
	movs	r1, #1
	movs	r3, #2
	mov	r6, fp
	mov	r8, r1
	orrs	r6, r3
	mov	r9, r5
.L_080e12f4:
	adds	r0, r5, #0
	movs	r1, #5
	bl	sub_08002064
	cmp	r0, #0
	bne.n	.L_080e1308
	mov	r2, r8
	strb	r2, [r7, #25]
	strb	r6, [r7, #26]
	b.n	.L_080e131a
.L_080e1308:
	cmp	r0, #2
	bne.n	.L_080e131a
	mov	r3, r8
	strb	r3, [r7, #25]
	mov	r1, fp
	mov	r2, r9
	mov	r3, sl
	strb	r1, [r7, #26]
	strb	r2, [r3, #5]
.L_080e131a:
	cmp	r5, #15
	bne.n	.L_080e1324
	movs	r0, #174
	bl	sub_081c0010
.L_080e1324:
	movs	r0, #1
	adds	r5, #1
	bl	sub_08013560
	cmp	r5, #69
	ble.n	.L_080e12f4
	movs	r3, #1
	mov	r1, fp
	movs	r2, #0
	strb	r3, [r7, #25]
	mov	r3, sl
	strb	r1, [r7, #26]
	strb	r2, [r3, #5]
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #4]
	movs	r2, #226
	lsls	r2, r2, #3
	adds	r3, r1, r2
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	bl	sub_08014274
	ldr	r0, [pc, #32]
	movs	r1, #1
	bl	sub_08038040
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x080ed90c
	.4byte 0x02000240
	.4byte 0x080e1155
	.2byte 0x0dbe
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #248
	ldr	r7, [r3, #0]
	movs	r5, #0
	adds	r6, r7, #0
	adds	r6, #160
	b.n	.L_080e139a
.L_080e138e:
	movs	r0, #1
	adds	r5, #1
	bl	sub_08013560
	cmp	r5, #89
	bgt.n	.L_080e13ae
.L_080e139a:
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #31
	ble.n	.L_080e138e
	adds	r3, r7, #0
	adds	r3, #162
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #31
	ble.n	.L_080e138e
.L_080e13ae:
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	ldr	r3, [pc, #48]
	movs	r2, #2
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080e13c8
	movs	r1, #7
	bl	sub_08020278
	b.n	.L_080e13d0
.L_080e13c8:
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_08020278
.L_080e13d0:
	ldr	r3, [pc, #16]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080e13e2
	adds	r0, r5, #0
	bl	.L_080e1448
.L_080e13e2:
	pop	{r5, pc}
	.2byte 0x122c
	.2byte 0x0300
	push	{r5, r6, lr}
	ldr	r6, [pc, #48]
	adds	r5, r0, #0
	ldr	r0, [r6, #0]
	movs	r3, #1
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_080e140a
	movs	r1, #6
	lsrs	r0, r0, #1
	bl	sub_0800206c
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_08020278
	ldr	r0, [r6, #0]
.L_080e140a:
	movs	r3, #15
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_080e1418
	adds	r0, r5, #0
	bl	.L_080e1448
.L_080e1418:
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x122c
	.2byte 0x0300
	push	{lr}
	cmp	r1, #0
	bne.n	.L_080e1430
	str	r1, [r0, #108]
	movs	r1, #0
	bl	sub_08020278
	b.n	.L_080e143c
.L_080e1430:
	cmp	r1, #1
	bne.n	.L_080e1438
	ldr	r3, [pc, #8]
	b.n	.L_080e143a
.L_080e1438:
	ldr	r3, [pc, #8]
.L_080e143a:
	str	r3, [r0, #108]
.L_080e143c:
	pop	{pc}
	movs	r0, r0
	.4byte 0x080e13b1
	.2byte 0x13e9
	.2byte 0x080e
.L_080e1448:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #248
	ldr	r6, [r3, #0]
	ldr	r2, [pc, #164]
	adds	r3, r6, #0
	adds	r3, #164
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	sub	sp, #4
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r3, [r3, #2]
	movs	r2, #166
	lsrs	r3, r3, #5
	adds	r2, r2, r6
	mov	fp, r3
	movs	r1, #0
	ldrsh	r3, [r2, r1]
	mov	r9, r0
	mov	r8, r3
	lsls	r3, r3, #2
	add	r3, r8
	lsls	r3, r3, #4
	mov	sl, r2
	adds	r5, r3, r6
	movs	r7, #1
.L_080e148c:
	mov	r2, fp
	str	r2, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #16
	movs	r2, #16
	ldr	r3, [pc, #112]
	bl	sub_080eaf98
	ldrb	r3, [r5, #5]
	movs	r1, #33
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	ldrb	r2, [r5, #9]
	strb	r3, [r5, #5]
	movs	r3, #15
	ands	r3, r2
	strb	r3, [r5, #9]
	mov	r0, r9
	bl	sub_080db9c0
	movs	r3, #3
	ands	r0, r3
	movs	r1, #13
	ldrb	r3, [r5, #9]
	negs	r1, r1
	adds	r2, r1, #0
	lsls	r0, r0, #2
	ands	r3, r2
	orrs	r3, r0
	subs	r7, #1
	strb	r3, [r5, #9]
	adds	r5, #40
	cmp	r7, #0
	bge.n	.L_080e148c
	mov	r1, r8
	lsls	r3, r1, #1
	movs	r2, #0
	adds	r3, #160
	strh	r2, [r6, r3]
	adds	r3, r6, #0
	mov	r2, r9
	adds	r3, #168
	str	r2, [r3, #0]
	mov	r1, sl
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #20]
	add	sp, #4
	eors	r3, r2
	mov	r2, sl
	strh	r3, [r2, #0]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x020036e0
	.2byte 0x4000
	.2byte 0x8000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #248
	ldr	r3, [r3, #0]
	sub	sp, #28
	str	r3, [sp, #12]
	adds	r3, #168
	ldr	r1, [r3, #0]
	movs	r2, #0
	ldr	r6, [sp, #12]
	str	r1, [sp, #8]
	str	r2, [sp, #4]
	mov	sl, r3
	add	r7, sp, #16
	adds	r6, #160
.L_080e1538:
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	lsls	r0, r0, #10
	bl	sub_08002096
	movs	r1, #0
	ldrsh	r3, [r6, r1]
	mov	r8, r0
	cmp	r3, #0
	blt.n	.L_080e15dc
	cmp	r3, #31
	bgt.n	.L_080e15dc
	ldr	r2, [sp, #8]
	adds	r0, r7, #0
	ldr	r3, [r2, #8]
	str	r3, [r7, #0]
	ldr	r1, [sp, #8]
	movs	r3, #0
	ldrsh	r2, [r6, r3]
	ldr	r3, [r1, #12]
	lsls	r2, r2, #16
	adds	r3, r3, r2
	str	r3, [r7, #4]
	ldr	r3, [r1, #16]
	str	r3, [r7, #8]
	bl	sub_080dc390
	ldr	r3, [r7, #8]
	ldr	r1, [sp, #4]
	mov	r9, r3
	lsls	r3, r1, #2
	ldr	r2, [r7, #0]
	adds	r3, r3, r1
	ldr	r1, [sp, #12]
	lsls	r3, r3, #4
	mov	fp, r2
	adds	r5, r3, r1
	movs	r2, #0
.L_080e1584:
	mov	r3, fp
	str	r3, [r5, #12]
	mov	r1, r9
	mov	r3, r8
	str	r1, [r5, #16]
	str	r3, [r5, #20]
	str	r3, [r5, #24]
	cmp	r2, #0
	bne.n	.L_080e15a6
	mov	r1, sl
	ldr	r0, [r1, #0]
	str	r2, [sp, #0]
	bl	sub_080db9cc
	subs	r0, #1
	strh	r0, [r5, #30]
	b.n	.L_080e15c2
.L_080e15a6:
	mov	r3, sl
	ldr	r0, [r3, #0]
	str	r2, [sp, #0]
	bl	sub_080db9cc
	ldr	r3, [r5, #16]
	ldr	r1, [pc, #68]
	adds	r0, #1
	adds	r3, r3, r1
	str	r3, [r5, #16]
	ldr	r3, [r5, #24]
	strh	r0, [r5, #30]
	negs	r3, r3
	str	r3, [r5, #24]
.L_080e15c2:
	ldr	r2, [sp, #0]
	adds	r0, r5, #0
	str	r2, [sp, #0]
	bl	sub_080eb01c
	ldr	r2, [sp, #0]
	adds	r5, #40
	adds	r2, #1
	cmp	r2, #1
	ble.n	.L_080e1584
	ldrh	r3, [r6, #0]
	adds	r3, #1
	strh	r3, [r6, #0]
.L_080e15dc:
	ldr	r2, [sp, #4]
	adds	r6, #2
	adds	r2, #1
	str	r2, [sp, #4]
	cmp	r2, #1
	ble.n	.L_080e1538
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb560
	movs	r1, #172
	movs	r0, #248
	bl	sub_08014cc0
	adds	r6, r0, #0
	bl	sub_080143ac
	adds	r3, r6, #0
	adds	r3, #164
	strh	r0, [r3, #0]
	movs	r1, #128
	lsls	r0, r0, #16
	lsls	r1, r1, #1
	ldr	r2, [pc, #44]
	asrs	r0, r0, #16
	bl	sub_080142d4
	adds	r3, r6, #0
	movs	r2, #186
	movs	r5, #0
	adds	r3, #166
	lsls	r2, r2, #2
	strh	r5, [r3, #0]
	adds	r2, #255
	subs	r3, #6
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r1, #144
	adds	r3, #6
	str	r5, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #12]
	bl	sub_080145a8
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x080ed80c
	.2byte 0x150d
	.2byte 0x080e
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #248
	ldr	r3, [r3, #0]
	adds	r3, #164
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	sub_08014274
	ldr	r0, [pc, #12]
	bl	sub_08014644
	movs	r0, #248
	bl	sub_0801314c
	pop	{pc}
	movs	r0, r0
	.2byte 0x150d
	.2byte 0x080e
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #192
	lsls	r1, r1, #18
	adds	r3, r1, #0
	adds	r3, #224
	ldr	r3, [r3, #0]
	sub	sp, #72
	mov	fp, r3
	adds	r3, r1, #0
	adds	r3, #240
	ldr	r3, [r3, #0]
	ldr	r0, [r1, #92]
	str	r3, [sp, #8]
	mov	r8, r0
	ldr	r1, [r1, #108]
	mov	r9, r1
	add	r1, sp, #60
	mov	sl, r1
.L_080e16a8:
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #36
	add	r3, r8
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #5
	bls.n	.L_080e16ba
	b.n	.L_080e1b1a
.L_080e16ba:
	ldr	r2, [pc, #804]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080e170e
	.4byte 0x080e1770
	.4byte 0x080e1838
	.4byte 0x080e1966
	.4byte 0x080e19e4
	.2byte 0x1af8
	.2byte 0x080e
.L_080e16dc:
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #50
	add	r3, r8
	strb	r6, [r3, #0]
	movs	r3, #134
	lsls	r3, r3, #3
	add	r3, r8
	movs	r2, #128
	strh	r4, [r3, #0]
	lsls	r2, r2, #3
	adds	r2, #36
	add	r2, r8
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #3
	movs	r3, #255
	adds	r2, #38
	lsls	r3, r3, #8
	add	r2, r8
	adds	r3, #255
	strh	r3, [r2, #0]
	b.n	.L_080e16a8
	movs	r0, #128
	lsls	r0, r0, #3
	adds	r0, #38
	add	r0, r8
	movs	r3, #0
	ldrsh	r1, [r0, r3]
	cmp	r1, #0
	bne.n	.L_080e1752
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #46
	add	r3, r8
	strb	r1, [r3, #0]
	movs	r3, #204
	lsls	r3, r3, #2
	adds	r3, #255
	movs	r2, #1
	add	r3, r8
	strb	r2, [r3, #0]
	movs	r3, #205
	lsls	r3, r3, #2
	adds	r3, #255
	add	r3, r8
	strb	r1, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #44
	add	r3, r8
	strb	r2, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #3
	movs	r3, #186
	adds	r2, #42
	b.n	.L_080e1b12
.L_080e1752:
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #44
	add	r3, r8
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	cmp	r2, #0
	beq.n	.L_080e1764
	b.n	.L_080e1b2c
.L_080e1764:
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #42
	add	r3, r8
	strh	r2, [r3, #0]
	b.n	.L_080e1aa6
	movs	r6, #128
	lsls	r6, r6, #3
	adds	r6, #38
	add	r6, r8
	movs	r4, #0
	ldrsh	r1, [r6, r4]
	ldrh	r2, [r6, #0]
	cmp	r1, #0
	bne.n	.L_080e17d0
	movs	r5, #232
	lsls	r5, r5, #2
	movs	r3, #128
	mov	r0, sl
	lsls	r3, r3, #9
	add	r5, r8
	str	r3, [r5, #24]
	str	r3, [r5, #20]
	str	r1, [r0, #8]
	str	r1, [r0, #4]
	str	r1, [r0, #0]
	mov	r2, fp
	ldrh	r1, [r2, #2]
	movs	r0, #152
	movs	r3, #128
	lsls	r3, r3, #8
	lsls	r0, r0, #6
	adds	r1, r1, r3
	mov	r2, sl
	adds	r0, #102
	bl	sub_0801489c
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r7, #225
	lsls	r7, r7, #2
	add	r7, r8
	str	r3, [r7, #12]
	movs	r2, #205
	ldr	r3, [r4, #4]
	lsls	r2, r2, #2
	str	r3, [r7, #16]
	adds	r2, #255
	ldr	r3, [r4, #8]
	add	r2, r8
	str	r3, [r7, #20]
	movs	r3, #1
	strb	r3, [r2, #0]
	ldrh	r2, [r6, #0]
.L_080e17d0:
	movs	r7, #160
	lsls	r3, r2, #16
	lsls	r7, r7, #13
	cmp	r3, r7
	bne.n	.L_080e17e8
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #46
	add	r2, r8
	movs	r3, #1
	strb	r3, [r2, #0]
	ldrh	r2, [r6, #0]
.L_080e17e8:
	movs	r0, #224
	lsls	r3, r2, #16
	lsls	r0, r0, #14
	cmp	r3, r0
	beq.n	.L_080e17f4
	b.n	.L_080e1b1a
.L_080e17f4:
	movs	r3, #0
	mov	r1, sl
	str	r3, [r1, #8]
	str	r3, [r1, #4]
	str	r3, [r1, #0]
	mov	r2, fp
	movs	r0, #160
	ldrh	r1, [r2, #2]
	lsls	r0, r0, #11
	mov	r2, sl
	bl	sub_0801489c
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r7, #225
	lsls	r7, r7, #2
	add	r7, r8
	str	r3, [r7, #12]
	movs	r2, #128
	ldr	r3, [r4, #4]
	lsls	r2, r2, #3
	str	r3, [r7, #16]
	adds	r2, #36
	ldr	r3, [r4, #8]
	add	r2, r8
	str	r3, [r7, #20]
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r6, #0]
	b.n	.L_080e1b1a
	movs	r5, #128
	lsls	r5, r5, #3
	adds	r5, #38
	add	r5, r8
	movs	r6, #0
	ldrsh	r3, [r5, r6]
	movs	r7, #225
	lsls	r7, r7, #2
	add	r7, r8
	cmp	r3, #90
	bne.n	.L_080e1866
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #36
	add	r3, r8
	ldrh	r2, [r3, #0]
	adds	r2, #1
	strh	r2, [r3, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r5, #0]
	b.n	.L_080e1b1a
.L_080e1866:
	movs	r2, #128
	movs	r3, #131
	lsls	r2, r2, #3
	lsls	r3, r3, #3
	adds	r2, #12
	add	r3, r8
	add	r2, r8
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #0]
	subs	r1, r3, r2
	cmp	r1, #0
	bge.n	.L_080e1880
	subs	r1, r2, r3
.L_080e1880:
	ldr	r4, [r7, #0]
	subs	r3, r4, r2
	cmp	r3, #0
	bge.n	.L_080e188a
	subs	r3, r2, r4
.L_080e188a:
	cmp	r1, r3
	blt.n	.L_080e18b6
	movs	r2, #128
	movs	r3, #132
	lsls	r2, r2, #3
	lsls	r3, r3, #3
	adds	r2, #20
	add	r3, r8
	add	r2, r8
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #0]
	subs	r1, r3, r2
	cmp	r1, #0
	bge.n	.L_080e18a8
	subs	r1, r2, r3
.L_080e18a8:
	ldr	r0, [r7, #8]
	subs	r3, r0, r2
	cmp	r3, #0
	bge.n	.L_080e18b2
	subs	r3, r2, r0
.L_080e18b2:
	cmp	r1, r3
	bge.n	.L_080e18ce
.L_080e18b6:
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #36
	add	r3, r8
	ldrh	r2, [r3, #0]
	adds	r2, #1
	strh	r2, [r3, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r5, #0]
	b.n	.L_080e16a8
.L_080e18ce:
	mov	r0, sl
	str	r4, [r0, #0]
	ldr	r3, [r7, #4]
	mov	r1, fp
	str	r3, [r0, #4]
	ldr	r3, [r7, #8]
	str	r3, [r0, #8]
	ldr	r0, [r1, #16]
	mov	r1, sl
	bl	sub_08020210
	cmp	r0, #0
	ble.n	.L_080e1918
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #50
	add	r2, r8
	movs	r3, #1
	strb	r3, [r2, #0]
	movs	r2, #134
	movs	r3, #255
	lsls	r2, r2, #3
	lsls	r3, r3, #8
	add	r2, r8
	adds	r3, #255
	strh	r3, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #36
	add	r2, r8
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r3, #1
	negs	r3, r3
	strh	r3, [r5, #0]
	b.n	.L_080e16a8
.L_080e1918:
	mov	r5, r9
	movs	r4, #0
	adds	r5, #20
.L_080e191e:
	ldr	r1, [r5, #0]
	cmp	r1, #0
	beq.n	.L_080e195c
	ldr	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_080e195c
	adds	r3, r1, #0
	adds	r3, #89
	ldrb	r2, [r3, #0]
	movs	r6, #1
	adds	r3, r6, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080e195c
	mov	r2, fp
	ldr	r3, [r2, #16]
	cmp	r1, r3
	beq.n	.L_080e195c
	ldrh	r3, [r1, #32]
	adds	r2, r1, #0
	adds	r2, #8
	subs	r3, #2
	adds	r0, r7, #0
	movs	r1, #2
	str	r4, [sp, #4]
	bl	sub_080dbe80
	ldr	r4, [sp, #4]
	cmp	r0, #0
	blt.n	.L_080e195c
	b.n	.L_080e16dc
.L_080e195c:
	adds	r4, #1
	adds	r5, #4
	cmp	r4, #79
	ble.n	.L_080e191e
	b.n	.L_080e1b1a
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #50
	add	r3, r8
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	cmp	r2, #0
	beq.n	.L_080e1982
	movs	r2, #205
	lsls	r2, r2, #2
	adds	r2, #255
	add	r2, r8
	movs	r3, #0
	b.n	.L_080e1996
.L_080e1982:
	movs	r3, #204
	lsls	r3, r3, #2
	adds	r3, #255
	add	r3, r8
	strb	r2, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #45
	add	r2, r8
	movs	r3, #1
.L_080e1996:
	strb	r3, [r2, #0]
	movs	r3, #0
	mov	r4, sl
	str	r3, [r4, #8]
	str	r3, [r4, #4]
	str	r3, [r4, #0]
	movs	r0, #128
	mov	r6, fp
	lsls	r0, r0, #10
	mov	r2, sl
	ldrh	r1, [r6, #2]
	bl	sub_0801489c
	mov	r0, sl
	ldr	r3, [r0, #0]
	movs	r7, #225
	lsls	r7, r7, #2
	add	r7, r8
	str	r3, [r7, #12]
	movs	r2, #128
	ldr	r3, [r0, #4]
	lsls	r2, r2, #3
	str	r3, [r7, #16]
	adds	r2, #36
	ldr	r3, [r0, #8]
	add	r2, r8
	str	r3, [r7, #20]
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #3
	movs	r3, #255
	adds	r2, #38
	lsls	r3, r3, #8
	b.n	.L_080e1b14
	movs	r0, r0
	.2byte 0x16c4
	.2byte 0x080e
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #50
	add	r3, r8
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	movs	r5, #232
	lsls	r5, r5, #2
	add	r5, r8
	cmp	r2, #0
	beq.n	.L_080e1abe
	movs	r0, #128
	lsls	r0, r0, #3
	adds	r0, #38
	add	r0, r8
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	cmp	r3, #31
	bgt.n	.L_080e1a60
	ldr	r3, [pc, #224]
	add	r2, sp, #28
	mov	ip, r2
	ldmia	r3!, {r4, r6, r7}
	stmia	r2!, {r4, r6, r7}
	ldmia	r3!, {r1, r4, r6}
	stmia	r2!, {r1, r4, r6}
	ldmia	r3!, {r1, r7}
	stmia	r2!, {r1, r7}
	ldr	r3, [pc, #208]
	add	r4, sp, #12
	adds	r2, r4, #0
	ldmia	r3!, {r1, r6, r7}
	stmia	r2!, {r1, r6, r7}
	ldr	r3, [r3, #0]
	mov	r6, ip
	str	r3, [r2, #0]
	movs	r3, #7
	ldrh	r2, [r0, #0]
	ands	r3, r2
	lsls	r2, r2, #16
	lsls	r3, r3, #2
	asrs	r2, r2, #16
	ldr	r1, [r6, r3]
	cmp	r2, #0
	bge.n	.L_080e1a40
	adds	r2, #7
.L_080e1a40:
	asrs	r3, r2, #3
	lsls	r3, r3, #2
	ldr	r3, [r4, r3]
	adds	r2, r3, #0
	muls	r2, r1
	cmp	r2, #0
	bge.n	.L_080e1a56
	movs	r7, #255
	lsls	r7, r7, #8
	adds	r7, #255
	adds	r2, r2, r7
.L_080e1a56:
	mov	r0, fp
	ldrh	r3, [r0, #2]
	asrs	r2, r2, #16
	adds	r3, r3, r2
	strh	r3, [r5, #28]
.L_080e1a60:
	movs	r0, #128
	lsls	r0, r0, #3
	adds	r0, #38
	add	r0, r8
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	ldrh	r2, [r0, #0]
	cmp	r3, #32
	bne.n	.L_080e1a80
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #45
	add	r2, r8
	movs	r3, #1
	strb	r3, [r2, #0]
	ldrh	r2, [r0, #0]
.L_080e1a80:
	lsls	r3, r2, #16
	movs	r2, #200
	lsls	r2, r2, #14
	cmp	r3, r2
	bne.n	.L_080e1b1a
	movs	r1, #205
	lsls	r1, r1, #2
	adds	r1, #255
	movs	r2, #0
	movs	r3, #1
	add	r1, r8
	str	r2, [r5, #24]
	str	r2, [r5, #20]
	strb	r3, [r1, #0]
	movs	r3, #204
	lsls	r3, r3, #2
	adds	r3, #255
	add	r3, r8
	strb	r2, [r3, #0]
.L_080e1aa6:
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #36
	add	r2, r8
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r0, #0]
	b.n	.L_080e1b1a
.L_080e1abe:
	ldr	r3, [r5, #20]
	ldr	r4, [pc, #48]
	adds	r3, r3, r4
	str	r3, [r5, #20]
	str	r3, [r5, #24]
	cmp	r3, #0
	bge.n	.L_080e1b1a
	str	r2, [r5, #24]
	str	r2, [r5, #20]
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #36
	add	r2, r8
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #3
	movs	r3, #255
	adds	r2, #38
	lsls	r3, r3, #8
	b.n	.L_080e1b14
	movs	r0, r0
	.4byte 0x080f0f34
	.4byte 0x080f0f54
	.2byte 0xd000
	.2byte 0xffff
	.2byte 0x2380
	lsls	r3, r3, #3
	adds	r3, #45
	add	r3, r8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080e1b1a
	movs	r2, #128
	lsls	r2, r2, #3
	movs	r3, #186
	adds	r2, #36
.L_080e1b12:
	lsls	r3, r3, #2
.L_080e1b14:
	add	r2, r8
	adds	r3, #255
	strh	r3, [r2, #0]
.L_080e1b1a:
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #44
	add	r3, r8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080e1b6a
.L_080e1b2c:
	movs	r5, #249
	lsls	r5, r5, #2
	add	r5, r8
	ldr	r2, [r5, #20]
	ldrh	r3, [r5, #28]
	movs	r6, #128
	movs	r7, #128
	lsls	r6, r6, #3
	lsls	r7, r7, #6
	movs	r0, #128
	adds	r2, r2, r6
	adds	r3, r3, r7
	lsls	r0, r0, #9
	str	r2, [r5, #20]
	str	r2, [r5, #24]
	strh	r3, [r5, #28]
	cmp	r2, r0
	blt.n	.L_080e1b6a
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #24]
	str	r3, [r5, #20]
	mov	r1, fp
	ldrh	r3, [r1, #2]
	movs	r2, #128
	strh	r3, [r5, #28]
	lsls	r2, r2, #3
	ldr	r3, [pc, #52]
	adds	r2, #44
	add	r2, r8
	strb	r3, [r2, #0]
.L_080e1b6a:
	movs	r0, #128
	lsls	r0, r0, #3
	adds	r0, #45
	add	r0, r8
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	cmp	r3, #0
	beq.n	.L_080e1bb8
	movs	r5, #249
	lsls	r5, r5, #2
	add	r5, r8
	ldr	r2, [r5, #20]
	ldr	r3, [pc, #24]
	movs	r4, #128
	adds	r2, r2, r3
	ldrh	r3, [r5, #28]
	movs	r6, #204
	lsls	r4, r4, #6
	lsls	r6, r6, #7
	adds	r3, r3, r4
	adds	r6, #101
	str	r2, [r5, #20]
	b.n	.L_080e1ba0
	.4byte 0x00000000
	.2byte 0xfc00
	.2byte 0xffff
.L_080e1ba0:
	str	r2, [r5, #24]
	movs	r1, #0
	strh	r3, [r5, #28]
	cmp	r2, r6
	bgt.n	.L_080e1bb8
	ldr	r3, [pc, #8]
	str	r1, [r5, #24]
	str	r1, [r5, #20]
	strb	r3, [r0, #0]
	b.n	.L_080e1bb8
	.2byte 0x0000
	.2byte 0x0000
.L_080e1bb8:
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #42
	add	r2, r8
	movs	r0, #186
	movs	r7, #0
	ldrsh	r3, [r2, r7]
	lsls	r0, r0, #2
	adds	r0, #255
	ldrh	r1, [r2, #0]
	cmp	r3, r0
	bne.n	.L_080e1bd2
	b.n	.L_080e1d7c
.L_080e1bd2:
	adds	r3, r1, #1
	strh	r3, [r2, #0]
	lsls	r3, r3, #16
	movs	r5, #249
	asrs	r3, r3, #16
	lsls	r5, r5, #2
	subs	r3, #10
	add	r5, r8
	cmp	r3, #62
	bls.n	.L_080e1be8
	b.n	.L_080e1d7c
.L_080e1be8:
	ldr	r2, [pc, #304]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080e1cec
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1cfa
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d08
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d50
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d20
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d2e
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d3c
	.4byte 0x080e1d7c
	.4byte 0x080e1d7c
	.4byte 0x080e1d50
	.2byte 0x1d6c
	.2byte 0x080e
	movs	r3, #133
	lsls	r3, r3, #3
	add	r3, r8
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #32]
	adds	r2, #32
	b.n	.L_080e1d5e
	movs	r3, #133
	lsls	r3, r3, #3
	add	r3, r8
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #20]
	adds	r2, #64
	b.n	.L_080e1d5e
	movs	r3, #133
	lsls	r3, r3, #3
	add	r3, r8
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #4]
	adds	r2, #32
	b.n	.L_080e1d5e
	movs	r0, r0
	.4byte 0x000003ff
	.2byte 0x1bf0
	.2byte 0x080e
	movs	r3, #133
	lsls	r3, r3, #3
	add	r3, r8
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #32]
	adds	r2, #96
	b.n	.L_080e1d5e
	movs	r3, #133
	lsls	r3, r3, #3
	add	r3, r8
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #20]
	adds	r2, #128
	b.n	.L_080e1d5e
	movs	r3, #133
	lsls	r3, r3, #3
	add	r3, r8
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #4]
	adds	r2, #160
	b.n	.L_080e1d5e
	movs	r0, r0
	.2byte 0x03ff
	.2byte 0x0000
	movs	r3, #133
	lsls	r3, r3, #3
	add	r3, r8
	ldrh	r3, [r3, #0]
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r2, #255
.L_080e1d5e:
	ands	r2, r3
	ldrh	r1, [r5, #8]
	ldr	r3, [pc, #436]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #8]
	b.n	.L_080e1d7c
	movs	r2, #128
	lsls	r2, r2, #3
	movs	r3, #186
	adds	r2, #42
	lsls	r3, r3, #2
	add	r2, r8
	adds	r3, #255
	strh	r3, [r2, #0]
.L_080e1d7c:
	movs	r7, #242
	lsls	r7, r7, #2
	add	r7, r8
	ldr	r3, [r7, #0]
	mov	r1, sl
	str	r3, [r1, #0]
	ldr	r3, [r7, #4]
	mov	r0, sl
	str	r3, [r1, #4]
	ldr	r3, [r7, #8]
	movs	r5, #249
	str	r3, [r1, #8]
	bl	sub_080dc390
	mov	r2, sl
	ldr	r3, [r2, #0]
	lsls	r5, r5, #2
	add	r5, r8
	str	r3, [r5, #12]
	movs	r7, #225
	ldr	r3, [r2, #8]
	lsls	r7, r7, #2
	str	r3, [r5, #16]
	adds	r0, r5, #0
	add	r7, r8
	bl	sub_080eb01c
	ldr	r3, [r7, #0]
	mov	r4, sl
	str	r3, [r4, #0]
	ldr	r3, [r7, #4]
	mov	r0, sl
	str	r3, [r4, #4]
	ldr	r3, [r7, #8]
	mov	r6, sl
	str	r3, [r4, #8]
	bl	sub_080dc390
	movs	r5, #232
	ldr	r3, [r6, #0]
	lsls	r5, r5, #2
	add	r5, r8
	str	r3, [r5, #12]
	adds	r0, r5, #0
	ldr	r3, [r6, #8]
	str	r3, [r5, #16]
	bl	sub_080eb01c
	movs	r3, #205
	lsls	r3, r3, #2
	adds	r3, #255
	add	r3, r8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080e1df8
	adds	r0, r7, #0
	movs	r1, #64
	movs	r2, #0
	bl	sub_080c8978
.L_080e1df8:
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r3, #46
	add	r3, r8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080e1eb8
	mov	r7, r8
	movs	r0, #9
	adds	r7, #4
	mov	r9, r0
.L_080e1e12:
	ldr	r0, [r7, #24]
	cmp	r0, #0
	bne.n	.L_080e1e62
	bl	sub_08014878
	movs	r6, #255
	movs	r1, #128
	lsls	r1, r1, #1
	ands	r6, r0
	adds	r6, r6, r1
	bl	sub_08014878
	movs	r5, #248
	lsls	r5, r5, #5
	movs	r3, #240
	lsls	r3, r3, #14
	adds	r5, #255
	movs	r2, #224
	lsls	r2, r2, #7
	ands	r5, r0
	str	r3, [r7, #0]
	movs	r3, #224
	adds	r5, r5, r2
	lsls	r3, r3, #13
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	ldr	r0, [r7, #24]
.L_080e1e62:
	cmp	r0, #0
	ble.n	.L_080e1e8c
	movs	r3, #2
	ldrsh	r1, [r7, r3]
	movs	r3, #1
	ands	r0, r3
	ldr	r3, [pc, #172]
	lsls	r0, r0, #6
	movs	r4, #6
	ldrsh	r2, [r7, r4]
	adds	r0, r0, r3
	movs	r3, #8
	str	r3, [sp, #0]
	bl	sub_080eb4a0
	adds	r0, r7, #0
	movs	r1, #60
	movs	r2, #0
	bl	sub_080c89a6
	ldr	r0, [r7, #24]
.L_080e1e8c:
	adds	r0, #1
	movs	r3, #204
	str	r0, [r7, #24]
	lsls	r3, r3, #2
	adds	r3, #255
	add	r3, r8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #1
	bne.n	.L_080e1eaa
	cmp	r0, #10
	bne.n	.L_080e1eaa
	movs	r3, #0
	str	r3, [r7, #24]
.L_080e1eaa:
	movs	r6, #1
	negs	r6, r6
	add	r9, r6
	mov	r0, r9
	adds	r7, #28
	cmp	r0, #0
	bge.n	.L_080e1e12
.L_080e1eb8:
	movs	r7, #225
	lsls	r7, r7, #2
	add	r7, r8
	ldr	r3, [r7, #0]
	mov	r1, sl
	str	r3, [r1, #0]
	ldr	r3, [r7, #4]
	mov	r2, fp
	str	r3, [r1, #4]
	ldr	r3, [r7, #8]
	ldr	r0, [pc, #80]
	str	r3, [r1, #8]
	mov	r6, sl
	ldrh	r1, [r2, #2]
	mov	r2, sl
	bl	sub_0801489c
	mov	r0, sl
	bl	sub_080dc390
	ldr	r2, [sp, #8]
	movs	r4, #2
	ldrsh	r3, [r6, r4]
	adds	r2, #168
	str	r3, [r2, #0]
	ldr	r2, [sp, #8]
	movs	r7, #10
	ldrsh	r3, [r6, r7]
	adds	r2, #172
	str	r3, [r2, #0]
	ldr	r2, [sp, #8]
	movs	r3, #5
	adds	r2, #193
	strb	r3, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #3
	adds	r2, #38
	add	r2, r8
	ldrh	r3, [r2, #0]
	add	sp, #72
	adds	r3, #1
	strh	r3, [r2, #0]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xfffffc00
	.4byte 0x080eda0c
	.4byte 0xffdc0000

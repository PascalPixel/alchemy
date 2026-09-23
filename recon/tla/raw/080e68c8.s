.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
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
	.set sub_08016ca4, 0x08016ca4
	.set sub_08020090, 0x08020090
	.set sub_080200c0, 0x080200c0
	.set sub_080200c8, 0x080200c8
	.set sub_08020218, 0x08020218
	.set sub_08020278, 0x08020278
	.set sub_08020330, 0x08020330
	.set sub_080ad2f0, 0x080ad2f0
	.set sub_080c8978, 0x080c8978
	.set sub_080cad84, 0x080cad84
	.set sub_080cda84, 0x080cda84
	.set sub_080cdf5c, 0x080cdf5c
	.set sub_080ce31c, 0x080ce31c
	.set sub_080ce458, 0x080ce458
	.set sub_080ceafc, 0x080ceafc
	.set sub_080d2d84, 0x080d2d84
	.set sub_080d3838, 0x080d3838
	.set sub_080d3b28, 0x080d3b28
	.set sub_080db9c0, 0x080db9c0
	.set sub_080db9cc, 0x080db9cc
	.set sub_080dba5c, 0x080dba5c
	.set sub_080dbc04, 0x080dbc04
	.set sub_080dbdc8, 0x080dbdc8
	.set sub_080dbe80, 0x080dbe80
	.set sub_080dbed0, 0x080dbed0
	.set sub_080dbf94, 0x080dbf94
	.set sub_080dc044, 0x080dc044
	.set sub_080dc10c, 0x080dc10c
	.set sub_080dc294, 0x080dc294
	.set sub_080dc384, 0x080dc384
	.set sub_080dc390, 0x080dc390
	.set sub_080dc954, 0x080dc954
	.set sub_080eaf98, 0x080eaf98
	.set sub_080eb01c, 0x080eb01c
	.set sub_080eb298, 0x080eb298
	.set sub_081c0010, 0x081c0010
	.global Func_080e68c8
	.thumb_func
Func_080e68c8:
	push	{lr}
	bl	.L_080e68d0
	pop	{pc}
.L_080e68d0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #200
	lsls	r1, r1, #5
	adds	r1, #60
	movs	r0, #92
	sub	sp, #44
	bl	sub_08014cc0
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #224
	ldr	r3, [r3, #0]
	mov	r9, r0
	ldr	r0, [r3, #16]
	ldr	r5, [r2, #108]
	movs	r1, #0
	movs	r2, #96
	str	r0, [sp, #28]
	str	r1, [sp, #24]
	str	r2, [sp, #20]
	str	r2, [sp, #12]
	movs	r4, #197
	lsls	r4, r4, #1
	mov	sl, r3
	adds	r3, r5, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	beq.n	.L_080e694a
	ldr	r3, [r0, #8]
	ldr	r1, [pc, #704]
	movs	r2, #128
	lsls	r2, r2, #12
	ands	r3, r1
	mov	r0, sl
	adds	r3, r3, r2
	str	r3, [r0, #4]
	ldr	r4, [sp, #28]
	mov	r7, sl
	ldr	r3, [r4, #12]
	adds	r7, #4
	str	r3, [r0, #8]
	ldr	r3, [r4, #16]
	ands	r3, r1
	adds	r3, r3, r2
	str	r3, [r0, #12]
	mov	r2, sl
	movs	r0, #128
	ldrh	r1, [r2, #0]
	lsls	r0, r0, #13
	adds	r2, r7, #0
	bl	sub_0801489c
.L_080e694a:
	bl	sub_080dc294
	movs	r0, #98
	movs	r3, #0
	adds	r0, #255
	movs	r1, #0
	movs	r2, #0
	bl	sub_080dc10c
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #48
	add	r3, r9
	mov	fp, r0
	str	r0, [r3, #0]
	cmp	r0, #0
	bne.n	.L_080e696e
	b.n	.L_080e70ce
.L_080e696e:
	add	r4, sp, #24
	ldrb	r4, [r4, #0]
	movs	r0, #208
	mov	r3, fp
	lsls	r0, r0, #4
	adds	r3, #85
	adds	r0, #76
	strb	r4, [r3, #0]
	adds	r3, r5, r0
	ldrh	r2, [r3, #0]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080e69b8
	mov	r3, fp
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #1
	bne.n	.L_080e69c0
	mov	r2, fp
	ldr	r1, [r2, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r1, #9]
	mov	r1, fp
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	b.n	.L_080e69c0
.L_080e69b8:
	mov	r2, fp
	adds	r2, #35
	movs	r3, #1
	strb	r3, [r2, #0]
.L_080e69c0:
	ldr	r4, [sp, #28]
	mov	r0, fp
	ldrh	r3, [r4, #6]
	movs	r1, #0
	strh	r3, [r0, #6]
	bl	sub_08020090
	mov	r1, sl
	adds	r1, #65
	str	r1, [sp, #4]
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	beq.n	.L_080e69ee
	mov	r0, fp
	movs	r1, #6
	bl	sub_08020278
	ldr	r2, [sp, #4]
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	bne.n	.L_080e6a02
.L_080e69ee:
	mov	r3, sl
	movs	r0, #177
	ldr	r1, [r3, #4]
	ldr	r2, [r3, #8]
	lsls	r0, r0, #1
	ldr	r3, [r3, #12]
	b.n	.L_080e6a10
.L_080e69fc:
	movs	r4, #1
	str	r4, [sp, #8]
	b.n	.L_080e6b1a
.L_080e6a02:
	mov	r2, sl
	mov	r4, sl
	movs	r0, #100
	ldr	r1, [r2, #4]
	adds	r0, #255
	ldr	r2, [r2, #8]
	ldr	r3, [r4, #12]
.L_080e6a10:
	bl	sub_080dc10c
	str	r0, [sp, #24]
	movs	r3, #200
	lsls	r3, r3, #5
	ldr	r0, [sp, #24]
	adds	r3, #52
	add	r3, r9
	str	r0, [r3, #0]
	cmp	r0, #0
	bne.n	.L_080e6a28
	b.n	.L_080e70ce
.L_080e6a28:
	ldr	r3, [r0, #16]
	movs	r1, #192
	lsls	r1, r1, #10
	adds	r3, r3, r1
	str	r3, [r0, #16]
	adds	r3, r0, #0
	movs	r2, #0
	adds	r3, #85
	adds	r1, r0, #0
	strb	r2, [r3, #0]
	adds	r1, #35
	movs	r3, #1
	movs	r4, #208
	strb	r3, [r1, #0]
	lsls	r4, r4, #4
	str	r2, [r0, #24]
	adds	r4, #76
	adds	r2, r5, r4
	ldrh	r2, [r2, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080e6a58
	bl	sub_080dba5c
.L_080e6a58:
	movs	r0, #138
	bl	sub_081c0010
	mov	r0, sl
	ldr	r2, [r0, #8]
	movs	r3, #128
	lsls	r3, r3, #14
	adds	r2, r2, r3
	ldr	r1, [r0, #4]
	ldr	r3, [r0, #12]
	mov	r0, fp
	bl	sub_080dbed0
	movs	r0, #10
	bl	sub_08013560
	mov	r0, fp
	movs	r1, #1
	bl	sub_08020090
	movs	r0, #15
	bl	sub_08013560
	movs	r4, #9
	mov	r8, r4
.L_080e6a8a:
	mov	r0, fp
	ldr	r3, [r0, #12]
	ldr	r1, [pc, #340]
	adds	r3, r3, r1
	str	r3, [r0, #12]
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_080e6a8a
	mov	r0, fp
	movs	r1, #2
	bl	sub_08020090
	movs	r0, #132
	bl	sub_081c0010
	movs	r4, #0
	str	r4, [sp, #8]
	mov	r7, sl
	adds	r6, r5, #0
	mov	r8, r4
	adds	r7, #4
	adds	r6, #20
.L_080e6ac2:
	ldmia	r6!, {r5}
	cmp	r5, #0
	beq.n	.L_080e6b10
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080e6b10
	adds	r0, r5, #0
	bl	sub_08020330
	movs	r1, #203
	lsls	r1, r1, #1
	cmp	r0, r1
	beq.n	.L_080e6aea
	adds	r3, r5, #0
	adds	r3, #89
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080e6b10
.L_080e6aea:
	mov	r2, sl
	ldr	r3, [r2, #16]
	cmp	r5, r3
	beq.n	.L_080e6b10
	ldr	r3, [r5, #80]
	ldrb	r3, [r3, #27]
	cmp	r3, #0
	beq.n	.L_080e6b10
	ldrh	r3, [r5, #32]
	adds	r2, r5, #0
	adds	r2, #8
	subs	r3, #2
	adds	r0, r7, #0
	movs	r1, #4
	bl	sub_080dbe80
	cmp	r0, #0
	blt.n	.L_080e6b10
	b.n	.L_080e69fc
.L_080e6b10:
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	cmp	r4, #79
	ble.n	.L_080e6ac2
.L_080e6b1a:
	ldr	r5, [sp, #28]
	mov	r1, sl
	adds	r5, #34
	ldr	r0, [r1, #4]
	ldrb	r2, [r5, #0]
	ldr	r1, [r1, #12]
	bl	sub_080dbdc8
	cmp	r0, #255
	bne.n	.L_080e6b32
	movs	r2, #4
	str	r2, [sp, #8]
.L_080e6b32:
	mov	r4, sl
	ldr	r1, [sp, #28]
	movs	r3, #30
	ldrsh	r0, [r4, r3]
	adds	r2, r7, #0
	bl	sub_080cda84
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	bne.n	.L_080e6b4c
	movs	r2, #2
	str	r2, [sp, #8]
.L_080e6b4c:
	mov	r3, sl
	ldr	r0, [r7, #0]
	ldr	r1, [r3, #12]
	ldrb	r2, [r5, #0]
	bl	sub_080dbc04
	cmp	r0, #0
	bne.n	.L_080e6b60
	movs	r4, #3
	str	r4, [sp, #8]
.L_080e6b60:
	ldr	r0, [sp, #8]
	cmp	r0, #0
	beq.n	.L_080e6b96
	movs	r0, #2
	bl	sub_08013560
	movs	r1, #15
	mov	r8, r1
.L_080e6b70:
	mov	r2, fp
	ldr	r0, [r2, #8]
	ldr	r1, [r2, #12]
	ldr	r2, [r2, #16]
	bl	sub_080dc044
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r4, r8
	cmp	r4, #0
	bge.n	.L_080e6b70
	movs	r0, #114
	bl	sub_081c0010
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_080e70ce
.L_080e6b96:
	mov	r3, sl
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080e6be8
	bl	sub_08014878
	movs	r1, #128
	lsls	r1, r1, #5
	cmp	r0, r1
	bcs.n	.L_080e6bbc
	movs	r2, #200
	lsls	r2, r2, #5
	adds	r2, #56
	add	r2, r9
	movs	r3, #1
	b.n	.L_080e6bf2
.L_080e6bbc:
	ldr	r3, [sp, #4]
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	cmp	r2, #0
	bne.n	.L_080e6bd2
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #56
	add	r3, r9
	strh	r2, [r3, #0]
	b.n	.L_080e6bf4
.L_080e6bd2:
	movs	r2, #200
	lsls	r2, r2, #5
	adds	r2, #56
	add	r2, r9
	movs	r3, #3
	b.n	.L_080e6bf2
	movs	r0, r0
	.4byte 0xfff00000
	.2byte 0x8000
	.2byte 0xfffd
.L_080e6be8:
	.2byte 0x22c8
	lsls	r2, r2, #5
	adds	r2, #56
	add	r2, r9
	movs	r3, #2
.L_080e6bf2:
	strh	r3, [r2, #0]
.L_080e6bf4:
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #56
	add	r3, r9
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #0
	bne.n	.L_080e6ce0
	ldr	r0, [pc, #908]
	bl	sub_08013300
	mov	r1, r9
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r5, #128
	lsls	r5, r5, #4
	adds	r1, r5, #0
	mov	r2, r9
	str	r0, [sp, #20]
	bl	sub_080142d4
	str	r0, [sp, #16]
	movs	r3, #200
	add	r0, sp, #16
	ldrh	r0, [r0, #0]
	lsls	r3, r3, #5
	adds	r3, #44
	movs	r7, #144
	add	r3, r9
	movs	r1, #0
	lsls	r7, r7, #5
	mov	r2, r9
	strh	r0, [r3, #0]
	mov	r8, r1
	add	r7, r9
	adds	r0, r2, r5
.L_080e6c40:
	ldr	r3, [sp, #16]
	movs	r1, #8
	str	r3, [sp, #0]
	movs	r3, #128
	movs	r2, #8
	lsls	r3, r3, #23
	adds	r6, r0, #0
	bl	sub_080eaf98
	ldrb	r3, [r6, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r3, r2
	strb	r3, [r6, #9]
	ldr	r0, [sp, #24]
	bl	sub_080db9c0
	movs	r3, #3
	ands	r0, r3
	movs	r4, #13
	ldrb	r3, [r6, #9]
	negs	r4, r4
	adds	r2, r4, #0
	ands	r3, r2
	lsls	r0, r0, #2
	orrs	r3, r0
	strb	r3, [r6, #9]
	ldr	r0, [sp, #24]
	bl	sub_080db9cc
	movs	r3, #204
	lsls	r3, r3, #6
	adds	r3, #51
	adds	r0, #1
	strh	r0, [r6, #30]
	str	r3, [r6, #20]
	str	r3, [r6, #24]
	mov	r0, sl
	ldr	r3, [r0, #4]
	str	r3, [r7, #0]
	ldr	r3, [r0, #8]
	str	r3, [r7, #4]
	ldr	r3, [r0, #12]
	str	r3, [r7, #8]
	bl	sub_08014878
	adds	r1, r0, #0
	movs	r0, #128
	adds	r2, r7, #0
	lsls	r0, r0, #10
	bl	sub_0801489c
	movs	r1, #0
	str	r1, [r7, #12]
	str	r1, [r7, #16]
	str	r1, [r7, #20]
	bl	sub_08014878
	adds	r2, r7, #0
	adds	r1, r0, #0
	adds	r2, #12
	ldr	r0, [pc, #724]
	bl	sub_0801489c
	mov	r2, r8
	negs	r3, r2
	lsls	r3, r3, #1
	str	r3, [r7, #24]
	movs	r3, #1
	add	r8, r3
	adds	r0, r6, #0
	mov	r4, r8
	adds	r0, #40
	adds	r7, #28
	cmp	r4, #63
	ble.n	.L_080e6c40
	b.n	.L_080e6ebe
.L_080e6ce0:
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #56
	add	r3, r9
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #2
	bne.n	.L_080e6dd6
	ldr	r1, [sp, #4]
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	cmp	r3, #0
	bne.n	.L_080e6d02
	ldr	r0, [pc, #664]
	bl	sub_08013300
	b.n	.L_080e6d08
.L_080e6d02:
	ldr	r0, [pc, #664]
	bl	sub_08013300
.L_080e6d08:
	mov	r1, r9
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r5, #128
	lsls	r5, r5, #4
	mov	r2, r9
	adds	r1, r5, #0
	str	r0, [sp, #20]
	bl	sub_080142d4
	str	r0, [sp, #16]
	movs	r3, #200
	add	r2, sp, #16
	lsls	r3, r3, #5
	ldrh	r2, [r2, #0]
	adds	r3, #44
	add	r3, r9
	movs	r7, #144
	strh	r2, [r3, #0]
	lsls	r7, r7, #5
	movs	r3, #0
	mov	r4, r9
	mov	r8, r3
	add	r7, r9
	adds	r0, r4, r5
.L_080e6d3e:
	adds	r6, r0, #0
	ldr	r0, [sp, #16]
	movs	r3, #128
	str	r0, [sp, #0]
	movs	r1, #8
	adds	r0, r6, #0
	movs	r2, #8
	lsls	r3, r3, #23
	bl	sub_080eaf98
	ldrb	r3, [r6, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r3, r2
	strb	r3, [r6, #9]
	ldr	r0, [sp, #24]
	bl	sub_080db9c0
	movs	r3, #3
	ands	r0, r3
	movs	r1, #13
	ldrb	r3, [r6, #9]
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	lsls	r0, r0, #2
	orrs	r3, r0
	strb	r3, [r6, #9]
	ldr	r0, [sp, #24]
	bl	sub_080db9cc
	adds	r0, #1
	strh	r0, [r6, #30]
	mov	r2, sl
	ldr	r3, [r2, #4]
	str	r3, [r7, #0]
	ldr	r3, [r2, #8]
	str	r3, [r7, #4]
	ldr	r3, [r2, #12]
	str	r3, [r7, #8]
	bl	sub_08014878
	adds	r1, r0, #0
	movs	r0, #128
	adds	r2, r7, #0
	lsls	r0, r0, #10
	bl	sub_0801489c
	movs	r3, #0
	str	r3, [r7, #12]
	str	r3, [r7, #16]
	str	r3, [r7, #20]
	bl	sub_08014878
	adds	r2, r7, #0
	adds	r1, r0, #0
	movs	r0, #128
	adds	r2, #12
	lsls	r0, r0, #9
	bl	sub_0801489c
	movs	r1, #1
	mov	r4, r8
	add	r8, r1
	negs	r3, r4
	adds	r0, r6, #0
	mov	r2, r8
	str	r3, [r7, #24]
	adds	r0, #40
	adds	r7, #28
	cmp	r2, #63
	ble.n	.L_080e6d3e
	b.n	.L_080e6ebe
.L_080e6dd6:
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #56
	add	r3, r9
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #3
	bne.n	.L_080e6ebe
	ldr	r0, [pc, #440]
	bl	sub_08013300
	mov	r1, r9
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r1, #128
	mov	r2, r9
	str	r0, [sp, #20]
	bl	sub_080142d4
	str	r0, [sp, #16]
	movs	r3, #200
	add	r0, sp, #16
	lsls	r3, r3, #5
	ldrh	r0, [r0, #0]
	adds	r3, #44
	add	r3, r9
	movs	r7, #144
	movs	r6, #128
	strh	r0, [r3, #0]
	movs	r1, #0
	lsls	r7, r7, #5
	lsls	r6, r6, #4
	mov	r8, r1
	add	r7, r9
	add	r6, r9
.L_080e6e20:
	ldr	r4, [sp, #16]
	mov	r2, r8
	movs	r3, #1
	ands	r3, r2
	lsls	r3, r3, #1
	adds	r3, r4, r3
	str	r3, [sp, #0]
	movs	r1, #4
	adds	r0, r6, #0
	movs	r2, #4
	movs	r3, #0
	bl	sub_080eaf98
	ldrb	r3, [r6, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r6, #9]
	strb	r3, [r6, #5]
	movs	r3, #15
	ands	r3, r2
	strb	r3, [r6, #9]
	mov	r0, fp
	bl	sub_080db9c0
	movs	r3, #3
	ands	r0, r3
	movs	r1, #13
	ldrb	r3, [r6, #9]
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	lsls	r0, r0, #2
	orrs	r3, r0
	strb	r3, [r6, #9]
	mov	r0, fp
	bl	sub_080db9cc
	subs	r0, #1
	strh	r0, [r6, #30]
	mov	r2, sl
	ldr	r3, [r2, #4]
	movs	r4, #160
	str	r3, [r7, #0]
	lsls	r4, r4, #14
	ldr	r3, [r2, #8]
	adds	r3, r3, r4
	str	r3, [r7, #4]
	ldr	r3, [r2, #12]
	str	r3, [r7, #8]
	bl	sub_08014878
	adds	r5, r0, #0
	movs	r0, #192
	lsls	r0, r0, #10
	lsls	r5, r5, #2
	adds	r5, r5, r0
	bl	sub_08014878
	adds	r2, r7, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	mov	r1, r8
	negs	r2, r1
	lsls	r3, r2, #1
	adds	r3, r3, r2
	cmp	r3, #0
	bge.n	.L_080e6eac
	adds	r3, #3
.L_080e6eac:
	movs	r2, #1
	asrs	r3, r3, #2
	add	r8, r2
	str	r3, [r7, #24]
	mov	r3, r8
	adds	r6, #40
	adds	r7, #28
	cmp	r3, #63
	ble.n	.L_080e6e20
.L_080e6ebe:
	movs	r7, #200
	lsls	r7, r7, #5
	adds	r7, #56
	add	r7, r9
	movs	r4, #0
	ldrsh	r3, [r7, r4]
	ldrh	r2, [r7, #0]
	cmp	r3, #0
	bne.n	.L_080e6f6a
	ldr	r0, [pc, #208]
	bl	sub_08013300
	mov	r1, r9
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r1, #128
	lsls	r1, r1, #3
	movs	r2, #0
	str	r0, [sp, #12]
	bl	sub_080142d4
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #46
	movs	r6, #200
	add	r3, r9
	lsls	r6, r6, #5
	add	r6, r9
	strh	r0, [r3, #0]
	movs	r3, #128
	str	r0, [sp, #0]
	lsls	r3, r3, #24
	movs	r2, #0
	adds	r0, r6, #0
	movs	r1, #16
	bl	sub_080eaf98
	mov	r1, sl
	ldr	r0, [r1, #16]
	bl	sub_080db9cc
	mov	r2, sl
	strh	r0, [r6, #30]
	ldr	r0, [r2, #16]
	bl	sub_080db9c0
	ldrb	r2, [r6, #9]
	movs	r3, #3
	ands	r0, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r6, #5]
	movs	r1, #32
	orrs	r2, r1
	lsls	r0, r0, #2
	strb	r2, [r6, #5]
	orrs	r3, r0
	movs	r2, #15
	ands	r3, r2
	strb	r3, [r6, #9]
	mov	r4, fp
	ldr	r3, [r4, #8]
	add	r5, sp, #32
	str	r3, [r5, #0]
	movs	r0, #128
	ldr	r3, [r4, #12]
	lsls	r0, r0, #14
	adds	r3, r3, r0
	str	r3, [r5, #4]
	adds	r0, r5, #0
	ldr	r3, [r4, #16]
	str	r3, [r5, #8]
	bl	sub_080dc390
	ldr	r3, [r5, #0]
	str	r3, [r6, #12]
	ldr	r3, [r5, #8]
	str	r3, [r6, #16]
	ldrh	r2, [r7, #0]
	movs	r1, #0
	ldrsh	r3, [r7, r1]
	cmp	r3, #0
	beq.n	.L_080e6f74
.L_080e6f6a:
	lsls	r3, r2, #16
	movs	r2, #192
	lsls	r2, r2, #10
	cmp	r3, r2
	bne.n	.L_080e6fa8
.L_080e6f74:
	movs	r0, #6
	bl	sub_08013560
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #52
	add	r3, r9
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r2, #24]
	movs	r0, #4
	bl	sub_08013560
	b.n	.L_080e6fae
	movs	r0, r0
	.4byte 0x000001e1
	.4byte 0x00013333
	.4byte 0x000001e2
	.4byte 0x000001f3
	.2byte 0x01e9
	.2byte 0x0000
.L_080e6fa8:
	movs	r0, #10
	bl	sub_08013560
.L_080e6fae:
	movs	r5, #200
	movs	r3, #200
	lsls	r5, r5, #5
	lsls	r3, r3, #5
	adds	r5, #40
	adds	r3, #42
	movs	r2, #0
	add	r3, r9
	add	r5, r9
	movs	r1, #144
	strh	r2, [r5, #0]
	lsls	r1, r1, #3
	strh	r2, [r3, #0]
	ldr	r0, [pc, #296]
	bl	sub_080145a8
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #56
	add	r3, r9
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	cmp	r3, #1
	bne.n	.L_080e7012
	movs	r2, #186
	movs	r0, #0
	ldrsh	r3, [r5, r0]
	lsls	r2, r2, #2
	adds	r2, #255
	movs	r6, #0
	cmp	r3, r2
	beq.n	.L_080e709c
	movs	r5, #200
	lsls	r5, r5, #5
	adds	r5, #40
	mov	r8, r2
	add	r5, r9
.L_080e6ff8:
	cmp	r6, #24
	bne.n	.L_080e7000
	bl	.L_080e716c
.L_080e7000:
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	adds	r6, #1
	cmp	r3, r8
	bne.n	.L_080e6ff8
	b.n	.L_080e709c
.L_080e7012:
	mov	r3, sl
	movs	r2, #30
	ldrsh	r0, [r3, r2]
	bl	sub_080ce31c
	adds	r2, r0, #0
	movs	r0, #160
	mov	r3, sl
	lsls	r0, r0, #23
	movs	r4, #30
	ldrsh	r1, [r3, r4]
	adds	r0, #5
	bl	sub_080ce458
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #40
	add	r3, r9
	movs	r2, #186
	movs	r4, #0
	ldrsh	r3, [r3, r4]
	lsls	r2, r2, #2
	adds	r2, #255
	adds	r7, r0, #0
	movs	r6, #0
	cmp	r3, r2
	beq.n	.L_080e708e
	mov	r8, r2
.L_080e704a:
	movs	r0, #1
	movs	r5, #0
	bl	sub_08013560
	cmp	r6, #30
	bne.n	.L_080e7058
	movs	r5, #1
.L_080e7058:
	cmp	r6, #50
	bne.n	.L_080e705e
	movs	r5, #2
.L_080e705e:
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r3, #196
	add	r3, sl
	strh	r5, [r3, #0]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r3, #198
	add	r3, sl
	strh	r6, [r3, #0]
	adds	r2, r6, #0
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_080ceafc
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #40
	add	r3, r9
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	adds	r6, #1
	cmp	r3, r8
	bne.n	.L_080e704a
.L_080e708e:
	movs	r1, #186
	lsls	r1, r1, #2
	adds	r1, #255
	adds	r0, r7, #0
	adds	r2, r6, #0
	bl	sub_080ceafc
.L_080e709c:
	ldr	r0, [pc, #84]
	bl	sub_08014644
	movs	r0, #114
	bl	sub_081c0010
	mov	r2, fp
	mov	r4, fp
	ldr	r1, [r2, #8]
	mov	r0, fp
	ldr	r2, [r2, #12]
	ldr	r3, [r4, #16]
	bl	sub_080dbf94
	ldr	r0, [sp, #20]
	cmp	r0, #96
	beq.n	.L_080e70c2
	bl	sub_08014274
.L_080e70c2:
	ldr	r1, [sp, #12]
	cmp	r1, #96
	beq.n	.L_080e70ce
	adds	r0, r1, #0
	bl	sub_08014274
.L_080e70ce:
	mov	r0, fp
	bl	sub_080200c8
	ldr	r0, [sp, #24]
	bl	sub_080200c8
	bl	sub_080dc384
	movs	r0, #92
	bl	sub_0801314c
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x6401
	.2byte 0x080e
.L_080e70f8:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #108]
	movs	r1, #200
	ldr	r3, [r3, #92]
	lsls	r1, r1, #5
	adds	r1, #48
	adds	r3, r3, r1
	adds	r6, r0, #0
	ldr	r5, [r3, #0]
	bl	sub_080d2d84
	adds	r1, r0, #0
	cmp	r5, #0
	beq.n	.L_080e7124
	ldr	r3, [r5, #8]
	str	r3, [r1, #8]
	ldr	r3, [r5, #12]
	str	r3, [r1, #12]
	ldr	r3, [r5, #16]
	str	r3, [r1, #16]
.L_080e7124:
	movs	r2, #197
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080e7142
	adds	r0, r1, #0
	adds	r0, #85
	movs	r2, #0
	movs	r3, #2
	strb	r3, [r0, #0]
	str	r2, [r1, #20]
	b.n	.L_080e714a
.L_080e7142:
	adds	r2, r1, #0
	adds	r2, #85
	movs	r3, #3
	strb	r3, [r2, #0]
.L_080e714a:
	movs	r3, #184
	lsls	r3, r3, #5
	adds	r3, #10
	str	r3, [r1, #72]
	movs	r0, #50
	bl	sub_08013560
	ldr	r3, [pc, #12]
	movs	r1, #176
	lsls	r1, r1, #1
	adds	r2, r7, r1
	orrs	r6, r3
	strh	r6, [r2, #0]
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x1000
	.2byte 0x0000
.L_080e716c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r3, [r2, #92]
	movs	r1, #200
	lsls	r1, r1, #5
	adds	r1, #48
	ldr	r2, [r2, #108]
	adds	r3, r3, r1
	ldr	r7, [r3, #0]
	ldr	r3, [pc, #168]
	mov	r8, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_08016ca4
	adds	r5, r0, #0
	movs	r0, #79
	bl	sub_080d2d84
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_080e71c0
	movs	r0, #234
	adds	r0, #255
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	bl	sub_080200c0
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080e7228
	movs	r3, #79
	lsls	r3, r3, #2
	adds	r3, #20
	mov	r1, r8
	str	r6, [r1, r3]
.L_080e71c0:
	bl	sub_08014878
	movs	r2, #192
	lsls	r2, r2, #8
	cmp	r0, r2
	bcs.n	.L_080e71f0
	ldr	r5, [pc, #100]
	bl	sub_08014878
	lsls	r0, r0, #4
	lsrs	r0, r0, #16
	lsls	r0, r0, #1
	ldrh	r5, [r5, r0]
	adds	r0, r5, #0
	bl	sub_080ad2f0
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_080e7204
	movs	r5, #128
	lsls	r5, r5, #8
	adds	r5, #1
	b.n	.L_080e7204
.L_080e71f0:
	bl	sub_08014878
	ldrb	r3, [r5, #15]
	muls	r3, r0
	lsls	r3, r3, #1
	lsrs	r3, r3, #16
	adds	r5, r3, #1
	movs	r3, #128
	lsls	r3, r3, #8
	orrs	r5, r3
.L_080e7204:
	ldr	r3, [r7, #8]
	movs	r0, #79
	str	r3, [r6, #8]
	ldr	r3, [r7, #12]
	adds	r1, r5, #0
	str	r3, [r6, #12]
	ldr	r3, [r7, #16]
	str	r3, [r6, #16]
	bl	sub_080d3b28
	movs	r3, #208
	lsls	r3, r3, #4
	adds	r3, #52
	add	r3, r8
	strh	r5, [r3, #0]
	movs	r0, #79
	bl	.L_080e70f8
.L_080e7228:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02000240
	.2byte 0x0fc0
	.2byte 0x080f
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #240
	lsls	r1, r1, #5
	adds	r1, #144
	movs	r0, #92
	sub	sp, #32
	bl	sub_08014cc0
	str	r0, [sp, #16]
	bl	sub_080cdf5c
	bl	sub_080cad84
	mov	r8, r0
	ldr	r0, [pc, #620]
	bl	sub_08013300
	ldr	r1, [sp, #16]
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r5, #128
	lsls	r5, r5, #3
	adds	r1, r5, #0
	ldr	r2, [sp, #16]
	str	r0, [sp, #8]
	bl	sub_080142d4
	ldr	r2, [sp, #16]
	movs	r3, #239
	movs	r1, #0
	lsls	r3, r3, #4
	adds	r6, r2, r5
	mov	r9, r0
	mov	sl, r1
	movs	r7, #15
	adds	r5, r2, r3
.L_080e7290:
	mov	r3, sl
	ands	r3, r7
	lsls	r3, r3, #1
	add	r3, r9
	str	r3, [sp, #0]
	adds	r0, r5, #0
	movs	r1, #4
	movs	r2, #4
	movs	r3, #0
	bl	sub_080eaf98
	ldrb	r3, [r5, #5]
	movs	r2, #32
	orrs	r3, r2
	ldrb	r2, [r5, #9]
	movs	r1, #13
	strb	r3, [r5, #5]
	negs	r1, r1
	adds	r3, r7, #0
	ands	r3, r2
	adds	r2, r1, #0
	ands	r3, r2
	strb	r3, [r5, #9]
	movs	r2, #1
	movs	r3, #240
	strh	r3, [r5, #30]
	add	sl, r2
	subs	r3, #241
	str	r3, [r6, #24]
	mov	r3, sl
	adds	r5, #40
	adds	r6, #28
	cmp	r3, #99
	ble.n	.L_080e7290
	movs	r0, #200
	bl	sub_081c0010
	movs	r0, #20
	bl	sub_08013560
	movs	r3, #0
	mov	r1, r8
	str	r3, [r1, #24]
	str	r3, [r1, #28]
	mov	r0, r8
	movs	r1, #0
	bl	sub_08020218
	movs	r3, #0
	movs	r2, #2
	str	r2, [sp, #4]
	str	r3, [sp, #12]
	mov	fp, r3
	mov	r9, r3
.L_080e72fc:
	ldr	r1, [sp, #4]
	cmp	r1, #0
	beq.n	.L_080e736c
	movs	r7, #160
	lsls	r7, r7, #15
	mov	sl, r1
.L_080e7308:
	bl	sub_08014878
	mov	r2, fp
	ldr	r1, [sp, #16]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	movs	r2, #128
	adds	r3, r1, r3
	lsls	r2, r2, #3
	adds	r6, r3, r2
	movs	r3, #0
	str	r3, [r6, #24]
	mov	r1, r8
	ldr	r3, [r1, #8]
	adds	r5, r0, #0
	str	r3, [r6, #0]
	bl	sub_08014878
	mov	r1, r8
	ldr	r2, [r1, #12]
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #2
	adds	r2, r2, r3
	str	r2, [r6, #4]
	ldr	r3, [r1, #16]
	str	r3, [r6, #8]
	bl	sub_08014878
	adds	r1, r5, #0
	lsls	r0, r0, #3
	adds	r2, r6, #0
	bl	sub_0801489c
	movs	r2, #1
	add	fp, r2
	mov	r0, fp
	movs	r1, #100
	str	r5, [r6, #12]
	str	r7, [r6, #20]
	bl	sub_08002064
	movs	r3, #1
	negs	r3, r3
	add	sl, r3
	mov	r1, sl
	mov	fp, r0
	cmp	r1, #0
	bne.n	.L_080e7308
.L_080e736c:
	ldr	r2, [sp, #16]
	movs	r3, #128
	movs	r1, #239
	lsls	r3, r3, #3
	lsls	r1, r1, #4
	adds	r6, r2, r3
	adds	r7, r2, r1
	movs	r2, #99
	add	r5, sp, #20
	mov	sl, r2
.L_080e7380:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	blt.n	.L_080e73d6
	cmp	r3, #19
	bhi.n	.L_080e73c8
	ldr	r3, [r6, #0]
	ldr	r1, [r6, #12]
	ldr	r0, [r6, #20]
	str	r3, [r5, #0]
	adds	r2, r5, #0
	ldr	r3, [r6, #4]
	str	r3, [r5, #4]
	ldr	r3, [r6, #8]
	str	r3, [r5, #8]
	bl	sub_0801489c
	adds	r0, r5, #0
	bl	sub_080dc390
	ldr	r3, [r5, #0]
	adds	r0, r7, #0
	str	r3, [r7, #12]
	ldr	r3, [r5, #8]
	str	r3, [r7, #16]
	bl	sub_080eb01c
	ldr	r3, [r6, #12]
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r3, r3, r1
	str	r3, [r6, #12]
	ldr	r2, [pc, #276]
	ldr	r3, [r6, #20]
	adds	r3, r3, r2
	str	r3, [r6, #20]
	ldr	r3, [r6, #24]
.L_080e73c8:
	adds	r3, #1
	str	r3, [r6, #24]
	cmp	r3, #20
	bne.n	.L_080e73d6
	movs	r3, #1
	negs	r3, r3
	str	r3, [r6, #24]
.L_080e73d6:
	movs	r3, #1
	negs	r3, r3
	add	sl, r3
	mov	r1, sl
	adds	r7, #40
	adds	r6, #28
	cmp	r1, #0
	bge.n	.L_080e7380
	ldr	r2, [sp, #12]
	cmp	r2, #1
	beq.n	.L_080e741e
	cmp	r2, #1
	bgt.n	.L_080e73f6
	cmp	r2, #0
	beq.n	.L_080e73fe
	b.n	.L_080e7498
.L_080e73f6:
	ldr	r3, [sp, #12]
	cmp	r3, #2
	beq.n	.L_080e745c
	b.n	.L_080e7498
.L_080e73fe:
	mov	r1, r9
	cmp	r1, #20
	bne.n	.L_080e7408
	movs	r2, #3
	str	r2, [sp, #4]
.L_080e7408:
	mov	r3, r9
	cmp	r3, #30
	bne.n	.L_080e7498
	movs	r3, #1
	movs	r1, #5
	movs	r2, #1
	negs	r3, r3
	str	r1, [sp, #4]
	str	r2, [sp, #12]
	mov	r9, r3
	b.n	.L_080e7498
.L_080e741e:
	mov	r1, r8
	ldrh	r3, [r1, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	mov	r2, r9
	strh	r3, [r1, #6]
	lsls	r0, r2, #16
	movs	r1, #30
	bl	sub_08002054
	mov	r3, r8
	str	r0, [r3, #24]
	movs	r3, #128
	lsls	r3, r3, #9
	cmp	r0, r3
	ble.n	.L_080e7444
	mov	r1, r8
	str	r3, [r1, #24]
.L_080e7444:
	mov	r2, r8
	ldr	r3, [r2, #24]
	str	r3, [r2, #28]
	mov	r3, r9
	cmp	r3, #40
	bne.n	.L_080e7498
	movs	r2, #1
	movs	r1, #2
	negs	r2, r2
	str	r1, [sp, #12]
	mov	r9, r2
	b.n	.L_080e7498
.L_080e745c:
	mov	r3, r9
	cmp	r3, #0
	bne.n	.L_080e7476
	movs	r3, #128
	lsls	r3, r3, #7
	mov	r1, r8
	strh	r3, [r1, #6]
	mov	r0, r8
	movs	r1, #1
	bl	sub_08020218
	movs	r2, #3
	str	r2, [sp, #4]
.L_080e7476:
	mov	r3, r9
	cmp	r3, #10
	bne.n	.L_080e7480
	movs	r1, #1
	str	r1, [sp, #4]
.L_080e7480:
	mov	r2, r9
	cmp	r2, #20
	bne.n	.L_080e748a
	movs	r3, #0
	str	r3, [sp, #4]
.L_080e748a:
	mov	r1, r9
	cmp	r1, #40
	bne.n	.L_080e7498
	movs	r2, #186
	lsls	r2, r2, #2
	adds	r2, #255
	str	r2, [sp, #12]
.L_080e7498:
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #186
	ldr	r1, [sp, #12]
	lsls	r2, r2, #2
	movs	r3, #1
	adds	r2, #255
	add	r9, r3
	cmp	r1, r2
	beq.n	.L_080e74b0
	b.n	.L_080e72fc
.L_080e74b0:
	ldr	r0, [sp, #8]
	bl	sub_08014274
	movs	r0, #92
	bl	sub_0801314c
	bl	sub_080dc954
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x000001ef
	.2byte 0x0000
	.2byte 0xfffc
.L_080e74d8:
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #240
	lsls	r1, r1, #5
	adds	r1, #144
	movs	r0, #92
	sub	sp, #24
	bl	sub_08014cc0
	str	r0, [sp, #20]
	bl	sub_080cdf5c
	mov	r9, r0
	bl	sub_080cdf5c
	bl	sub_080cad84
	adds	r7, r0, #0
	movs	r0, #20
	bl	sub_08013560
	bl	sub_080dc294
	ldr	r0, [pc, #192]
	bl	sub_08013300
	ldr	r1, [sp, #20]
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r5, #128
	lsls	r5, r5, #3
	adds	r1, r5, #0
	ldr	r2, [sp, #20]
	str	r0, [sp, #12]
	bl	sub_080142d4
	ldr	r2, [sp, #20]
	movs	r3, #239
	movs	r1, #0
	lsls	r3, r3, #4
	mov	r8, r0
	mov	sl, r1
	adds	r5, r2, r5
	movs	r4, #15
	adds	r6, r2, r3
.L_080e7540:
	mov	r3, sl
	ands	r3, r4
	lsls	r3, r3, #1
	add	r3, r8
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #4
	movs	r2, #4
	movs	r3, #0
	str	r4, [sp, #4]
	bl	sub_080eaf98
	ldrb	r3, [r6, #5]
	movs	r2, #32
	ldr	r4, [sp, #4]
	orrs	r3, r2
	ldrb	r2, [r6, #9]
	movs	r1, #13
	strb	r3, [r6, #5]
	negs	r1, r1
	adds	r3, r4, #0
	ands	r3, r2
	adds	r2, r1, #0
	ands	r3, r2
	strb	r3, [r6, #9]
	movs	r2, #1
	movs	r3, #240
	strh	r3, [r6, #30]
	add	sl, r2
	subs	r3, #241
	str	r3, [r5, #24]
	mov	r3, sl
	adds	r6, #40
	adds	r5, #28
	cmp	r3, #99
	ble.n	.L_080e7540
	movs	r0, #10
	bl	sub_08013560
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	mov	r0, r9
	bl	sub_080d3838
	movs	r0, #20
	bl	sub_08013560
	movs	r1, #28
	adds	r0, r7, #0
	bl	sub_08020090
	ldr	r3, [pc, #44]
	movs	r0, #131
	str	r3, [r7, #108]
	bl	sub_081c0010
	movs	r0, #30
	bl	sub_08013560
	movs	r0, #220
	bl	sub_081c0010
	ldr	r6, [pc, #16]
	movs	r1, #3
	adds	r0, r7, #0
	bl	sub_08020090
	movs	r1, #49
	movs	r5, #0
	mov	r9, r1
	b.n	.L_080e75dc
	.4byte 0x00001000
	.4byte 0x000001ef
	.2byte 0xb91d
	.2byte 0x080d
.L_080e75dc:
	ldrh	r3, [r7, #6]
	movs	r0, #1
	adds	r3, r3, r5
	adds	r3, r3, r6
	strh	r3, [r7, #6]
	bl	sub_08013560
	movs	r2, #1
	negs	r2, r2
	add	r9, r2
	mov	r3, r9
	adds	r5, #60
	cmp	r3, #0
	bge.n	.L_080e75dc
	movs	r2, #0
	movs	r1, #2
	str	r1, [sp, #8]
	str	r2, [sp, #16]
	mov	fp, r2
	mov	r9, r2
.L_080e7604:
	ldr	r3, [sp, #8]
	cmp	r3, #0
	beq.n	.L_080e76bc
	mov	sl, r3
.L_080e760c:
	bl	sub_08014878
	mov	r1, fp
	ldr	r2, [sp, #20]
	lsls	r3, r1, #3
	subs	r3, r3, r1
	lsls	r3, r3, #2
	adds	r6, r2, r3
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r5, r6, r3
	movs	r1, #0
	str	r1, [r5, #24]
	ldr	r3, [r7, #8]
	mov	r8, r0
	str	r3, [r5, #0]
	bl	sub_08014878
	ldr	r2, [r7, #12]
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #2
	adds	r2, r2, r3
	str	r2, [r5, #4]
	ldr	r3, [r7, #16]
	str	r3, [r5, #8]
	bl	sub_08014878
	adds	r2, r5, #0
	lsls	r0, r0, #3
	mov	r1, r8
	bl	sub_0801489c
	movs	r2, #1
	mov	r3, fp
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_080e767e
	ldr	r3, [pc, #416]
	movs	r1, #0
	str	r1, [r5, #12]
	str	r3, [r5, #16]
	str	r1, [r5, #20]
	bl	sub_08014878
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #3
	lsls	r2, r2, #11
	lsls	r0, r0, #2
	adds	r3, #12
	adds	r0, r0, r2
	mov	r1, r8
	adds	r2, r6, r3
	bl	sub_0801489c
	b.n	.L_080e76a2
.L_080e767e:
	movs	r3, #160
	lsls	r3, r3, #11
	str	r2, [r5, #12]
	str	r3, [r5, #16]
	str	r2, [r5, #20]
	bl	sub_08014878
	movs	r3, #128
	movs	r1, #128
	lsls	r3, r3, #3
	lsls	r1, r1, #11
	lsls	r0, r0, #2
	adds	r3, #12
	adds	r0, r0, r1
	adds	r2, r6, r3
	mov	r1, r8
	bl	sub_0801489c
.L_080e76a2:
	movs	r1, #1
	add	fp, r1
	mov	r0, fp
	movs	r1, #100
	bl	sub_08002064
	movs	r2, #1
	negs	r2, r2
	add	sl, r2
	mov	r3, sl
	mov	fp, r0
	cmp	r3, #0
	bne.n	.L_080e760c
.L_080e76bc:
	movs	r1, #0
	ldr	r2, [sp, #20]
	mov	sl, r1
	movs	r3, #128
	movs	r1, #239
	lsls	r3, r3, #3
	lsls	r1, r1, #4
	adds	r5, r2, r3
	adds	r6, r2, r1
.L_080e76ce:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_080e770e
	cmp	r3, #19
	bhi.n	.L_080e7700
	adds	r0, r6, #0
	adds	r1, r5, #0
	bl	sub_080eb298
	movs	r3, #1
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080e76f4
	movs	r2, #128
	adds	r0, r5, #0
	movs	r1, #63
	lsls	r2, r2, #8
	b.n	.L_080e76fa
.L_080e76f4:
	adds	r0, r5, #0
	movs	r1, #63
	ldr	r2, [pc, #260]
.L_080e76fa:
	bl	sub_080c8978
	ldr	r3, [r5, #24]
.L_080e7700:
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #20
	bne.n	.L_080e770e
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L_080e770e:
	movs	r3, #1
	add	sl, r3
	mov	r1, sl
	adds	r6, #40
	adds	r5, #28
	cmp	r1, #99
	ble.n	.L_080e76ce
	ldr	r2, [sp, #16]
	cmp	r2, #1
	beq.n	.L_080e7774
	cmp	r2, #1
	bgt.n	.L_080e772c
	cmp	r2, #0
	beq.n	.L_080e7734
	b.n	.L_080e77c8
.L_080e772c:
	ldr	r3, [sp, #16]
	cmp	r3, #2
	beq.n	.L_080e77b6
	b.n	.L_080e77c8
.L_080e7734:
	mov	r1, r9
	cmp	r1, #10
	bne.n	.L_080e7740
	movs	r0, #198
	bl	sub_081c0010
.L_080e7740:
	ldrh	r3, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	strh	r3, [r7, #6]
	mov	r3, r9
	cmp	r3, #40
	bne.n	.L_080e7754
	movs	r1, #3
	str	r1, [sp, #8]
.L_080e7754:
	mov	r2, r9
	cmp	r2, #50
	bne.n	.L_080e775e
	movs	r3, #4
	str	r3, [sp, #8]
.L_080e775e:
	mov	r1, r9
	cmp	r1, #60
	bne.n	.L_080e77c8
	movs	r1, #1
	movs	r2, #5
	movs	r3, #1
	negs	r1, r1
	str	r2, [sp, #8]
	str	r3, [sp, #16]
	mov	r9, r1
	b.n	.L_080e77c8
.L_080e7774:
	ldrh	r3, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r3, r3, r2
	strh	r3, [r7, #6]
	mov	r3, r9
	lsls	r0, r3, #16
	movs	r1, #30
	bl	sub_08002054
	movs	r3, #128
	lsls	r3, r3, #9
	subs	r3, r3, r0
	movs	r5, #0
	str	r3, [r7, #24]
	cmp	r3, #0
	bge.n	.L_080e7798
	str	r5, [r7, #24]
.L_080e7798:
	ldr	r3, [r7, #24]
	movs	r1, #0
	str	r3, [r7, #28]
	adds	r0, r7, #0
	bl	sub_08020218
	mov	r1, r9
	cmp	r1, #30
	bne.n	.L_080e77c8
	movs	r3, #1
	movs	r2, #2
	negs	r3, r3
	str	r2, [sp, #16]
	mov	r9, r3
	b.n	.L_080e77c8
.L_080e77b6:
	movs	r1, #0
	mov	r2, r9
	str	r1, [sp, #8]
	cmp	r2, #20
	bne.n	.L_080e77c8
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	str	r3, [sp, #16]
.L_080e77c8:
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #186
	ldr	r2, [sp, #16]
	lsls	r3, r3, #2
	movs	r1, #1
	adds	r3, #255
	add	r9, r1
	cmp	r2, r3
	beq.n	.L_080e77e0
	b.n	.L_080e7604
.L_080e77e0:
	ldr	r0, [sp, #12]
	bl	sub_08014274
	movs	r0, #92
	bl	sub_0801314c
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xfffc0000
	.2byte 0x6000
	.2byte 0xffff
	.2byte 0xb500
	bl	.L_080e74d8
	bl	sub_080dc384
	movs	r0, #10
	bl	sub_08013560
	pop	{pc}
	.2byte 0x0000

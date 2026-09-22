.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_08014274, 0x08014274
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_0801489c, 0x0801489c
	.set sub_080148e8, 0x080148e8
	.set sub_0801587c, 0x0801587c
	.set sub_08020090, 0x08020090
	.set sub_080200c0, 0x080200c0
	.set sub_080201f0, 0x080201f0
	.set sub_08020218, 0x08020218
	.set sub_080cdf5c, 0x080cdf5c
	.set sub_080d2240, 0x080d2240
	.set sub_080d2d84, 0x080d2d84
	.set sub_080d3300, 0x080d3300
	.set sub_080d3600, 0x080d3600
	.set sub_080d489c, 0x080d489c
	.set sub_080dbb78, 0x080dbb78
	.set sub_080dc390, 0x080dc390
	.set sub_080eaf98, 0x080eaf98
	.set sub_080eb01c, 0x080eb01c
	.set sub_081c0010, 0x081c0010
	.global Overlay_080e25e8
Overlay_080e25e8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	str	r3, [sp, #12]
	ldr	r3, [sp, #64]
	str	r1, [sp, #20]
	str	r2, [sp, #16]
	str	r0, [sp, #24]
	lsls	r3, r3, #16
	movs	r6, #192
	asrs	r3, r3, #16
	lsls	r6, r6, #18
	ldr	r7, [sp, #68]
	mov	fp, r3
	ldr	r5, [r6, #108]
	bl	sub_080cdf5c
	bl	sub_080d2d84
	ldr	r3, [r0, #20]
	movs	r2, #204
	asrs	r3, r3, #20
	str	r3, [sp, #8]
	lsls	r2, r2, #4
	adds	r1, r5, r2
	lsls	r3, r7, #3
	ldr	r2, [r6, #32]
	subs	r3, r3, r7
	lsls	r3, r3, #3
	adds	r2, r2, r3
	movs	r3, #132
	lsls	r3, r3, #1
	adds	r3, r3, r2
	adds	r5, r1, #0
	mov	r9, r3
	movs	r0, #0
	ldrsh	r3, [r5, r0]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	beq.n	.L_080e2704
.L_080e2644:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	ldr	r4, [sp, #60]
	mov	r2, fp
	subs	r1, r3, r4
	movs	r0, #2
	ldrsh	r3, [r5, r0]
	ldr	r4, [sp, #8]
	subs	r3, r3, r2
	mov	r0, r9
	subs	r2, r3, r4
	ldr	r3, [r0, #8]
	asrs	r3, r3, #20
	adds	r1, r1, r3
	ldr	r3, [r0, #12]
	asrs	r3, r3, #20
	adds	r2, r2, r3
	cmp	r1, #0
	blt.n	.L_080e26f6
	ldr	r3, [sp, #16]
	cmp	r1, r3
	bge.n	.L_080e26f6
	cmp	r2, #0
	blt.n	.L_080e26f6
	ldr	r4, [sp, #12]
	cmp	r2, r4
	bge.n	.L_080e26f6
	ldr	r3, [sp, #20]
	ldr	r0, [sp, #24]
	ldr	r4, [sp, #60]
	adds	r3, r3, r2
	adds	r6, r0, r1
	mov	r8, r3
	mov	r0, fp
	adds	r4, r4, r1
	movs	r3, #1
	adds	r7, r0, r2
	mov	r1, r8
	movs	r2, #1
	adds	r0, r6, #0
	mov	sl, r4
	str	r4, [sp, #0]
	str	r7, [sp, #4]
	bl	sub_080201f0
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	movs	r3, #2
	ldrsh	r1, [r5, r3]
	movs	r2, #2
	lsls	r1, r1, #20
	lsls	r0, r0, #20
	bl	sub_080dbb78
	ldr	r1, [pc, #96]
	movs	r4, #200
	lsls	r4, r4, #5
	movs	r3, #0
	adds	r4, #76
	strb	r3, [r0, #2]
	adds	r0, r1, r4
	movs	r2, #0
	ldrsb	r2, [r0, r2]
	cmp	r2, #63
	bgt.n	.L_080e26f6
	movs	r4, #184
	lsls	r4, r4, #5
	lsls	r2, r2, #3
	adds	r4, #76
	adds	r3, r2, r4
	strb	r6, [r1, r3]
	mov	r4, r8
	adds	r3, r3, r1
	strb	r4, [r3, #1]
	mov	r4, sl
	strb	r4, [r3, #2]
	strb	r7, [r3, #3]
	movs	r3, #184
	lsls	r3, r3, #5
	adds	r3, #80
	adds	r2, r2, r3
	ldrh	r3, [r5, #0]
	strb	r3, [r1, r2]
	adds	r2, r2, r1
	ldrh	r3, [r5, #2]
	strb	r3, [r2, #1]
	ldrb	r3, [r0, #0]
	adds	r3, #1
	strb	r3, [r0, #0]
.L_080e26f6:
	adds	r5, #4
	movs	r4, #0
	ldrsh	r3, [r5, r4]
	movs	r0, #1
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_080e2644
.L_080e2704:
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x1000
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	ldr	r1, [pc, #84]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #76
	adds	r2, r1, r3
	movs	r3, #0
	ldrsb	r3, [r2, r3]
	movs	r6, #0
	sub	sp, #8
	cmp	r6, r3
	bge.n	.L_080e276c
	movs	r3, #184
	lsls	r3, r3, #5
	adds	r3, #76
	adds	r7, r2, #0
	adds	r5, r1, r3
.L_080e273a:
	ldrb	r3, [r5, #2]
	ldrb	r0, [r5, #0]
	ldrb	r1, [r5, #1]
	str	r3, [sp, #0]
	movs	r2, #1
	ldrb	r3, [r5, #3]
	adds	r6, #1
	str	r3, [sp, #4]
	movs	r3, #1
	bl	sub_080201f0
	ldrb	r0, [r5, #4]
	ldrb	r1, [r5, #5]
	lsls	r0, r0, #20
	lsls	r1, r1, #20
	movs	r2, #2
	bl	sub_080dbb78
	movs	r3, #0
	strb	r3, [r0, #2]
	movs	r3, #0
	ldrsb	r3, [r7, r3]
	adds	r5, #8
	cmp	r6, r3
	blt.n	.L_080e273a
.L_080e276c:
	add	sp, #8
	pop	{r5, r6, r7, pc}
	.2byte 0x1000
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #44
	str	r1, [sp, #36]
	ldr	r1, [sp, #76]
	lsls	r3, r3, #16
	lsls	r1, r1, #16
	asrs	r3, r3, #16
	asrs	r1, r1, #16
	str	r3, [sp, #32]
	str	r1, [sp, #28]
	str	r0, [sp, #40]
	movs	r5, #192
	lsls	r2, r2, #16
	lsls	r5, r5, #18
	asrs	r6, r2, #16
	ldr	r7, [r5, #108]
	bl	sub_080cdf5c
	bl	sub_080d2d84
	ldr	r3, [r5, #32]
	movs	r2, #132
	lsls	r2, r2, #1
	adds	r1, r3, r2
	movs	r3, #204
	lsls	r3, r3, #4
	adds	r4, r7, r3
	ldr	r3, [r1, #8]
	asrs	r5, r3, #20
	str	r5, [sp, #20]
	ldr	r2, [r1, #12]
	asrs	r5, r2, #20
	str	r5, [sp, #16]
	cmp	r6, #2
	bgt.n	.L_080e27d2
	lsls	r3, r6, #3
	subs	r3, r3, r6
	lsls	r3, r3, #3
	adds	r1, r1, r3
	ldr	r3, [r1, #8]
	ldr	r2, [r1, #12]
.L_080e27d2:
	ldr	r1, [sp, #40]
	asrs	r3, r3, #20
	subs	r3, r1, r3
	str	r3, [sp, #12]
	ldr	r3, [sp, #36]
	asrs	r2, r2, #20
	subs	r2, r3, r2
	str	r2, [sp, #8]
	movs	r5, #204
	ldr	r3, [r0, #20]
	lsls	r5, r5, #4
	asrs	r3, r3, #20
	negs	r3, r3
	str	r3, [sp, #24]
	adds	r3, r7, r5
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	beq.n	.L_080e28c0
	adds	r5, r4, #0
.L_080e27fe:
	movs	r3, #2
	ldrsh	r6, [r5, r3]
	ldr	r4, [sp, #24]
	ldr	r2, [sp, #20]
	movs	r1, #0
	ldrsh	r7, [r5, r1]
	ldr	r0, [sp, #16]
	adds	r3, r6, r4
	subs	r1, r7, r2
	subs	r2, r3, r0
	ldr	r3, [sp, #12]
	cmp	r1, r3
	blt.n	.L_080e28b2
	ldr	r4, [sp, #32]
	adds	r3, r3, r4
	cmp	r1, r3
	bge.n	.L_080e28b2
	ldr	r0, [sp, #8]
	cmp	r2, r0
	blt.n	.L_080e28b2
	ldr	r4, [sp, #28]
	adds	r3, r0, r4
	cmp	r2, r3
	bge.n	.L_080e28b2
	ldr	r0, [sp, #12]
	ldr	r4, [sp, #80]
	subs	r3, r1, r0
	ldr	r1, [sp, #8]
	ldr	r0, [sp, #84]
	subs	r2, r2, r1
	ldr	r1, [sp, #40]
	adds	r4, r4, r3
	adds	r1, r1, r3
	ldr	r3, [sp, #36]
	adds	r0, r0, r2
	mov	r8, r4
	adds	r3, r3, r2
	mov	r9, r0
	str	r1, [sp, #0]
	str	r3, [sp, #4]
	mov	r0, r8
	movs	r2, #1
	mov	fp, r1
	mov	sl, r3
	mov	r1, r9
	movs	r3, #1
	bl	sub_080201f0
	lsls	r1, r6, #20
	movs	r2, #2
	lsls	r0, r7, #20
	bl	sub_080dbb78
	ldr	r1, [pc, #100]
	movs	r4, #200
	lsls	r4, r4, #5
	movs	r3, #0
	adds	r4, #76
	strb	r3, [r0, #2]
	adds	r0, r1, r4
	movs	r2, #0
	ldrsb	r2, [r0, r2]
	cmp	r2, #63
	bgt.n	.L_080e28b2
	movs	r4, #184
	lsls	r4, r4, #5
	lsls	r2, r2, #3
	adds	r4, #76
	adds	r3, r2, r4
	mov	r4, r8
	strb	r4, [r1, r3]
	adds	r3, r3, r1
	mov	r4, r9
	strb	r4, [r3, #1]
	mov	r4, fp
	strb	r4, [r3, #2]
	mov	r4, sl
	strb	r4, [r3, #3]
	movs	r3, #184
	lsls	r3, r3, #5
	adds	r3, #80
	adds	r2, r2, r3
	ldrh	r3, [r5, #0]
	strb	r3, [r1, r2]
	adds	r2, r2, r1
	ldrh	r3, [r5, #2]
	strb	r3, [r2, #1]
	ldrb	r3, [r0, #0]
	adds	r3, #1
	strb	r3, [r0, #0]
.L_080e28b2:
	adds	r5, #4
	movs	r4, #0
	ldrsh	r3, [r5, r4]
	movs	r0, #1
	negs	r0, r0
	cmp	r3, r0
	bne.n	.L_080e27fe
.L_080e28c0:
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x1000
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r8, r2
	adds	r6, r1, #0
	sub	sp, #24
	adds	r5, r0, #0
	bl	sub_080cdf5c
	mov	r9, r0
	bl	sub_080d2d84
	mov	sl, r0
	mov	r0, r8
	lsls	r0, r0, #16
	mov	r8, r0
	lsls	r6, r6, #16
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #0
	mov	r3, r8
	bl	sub_080200c0
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_080e2912
	b.n	.L_080e2a5a
.L_080e2912:
	movs	r1, #0
	bl	sub_08020218
	movs	r3, #178
	lsls	r3, r3, #7
	adds	r3, #153
	str	r3, [r7, #72]
	movs	r3, #160
	lsls	r3, r3, #11
	str	r3, [r7, #40]
	movs	r3, #204
	lsls	r3, r3, #7
	adds	r3, #102
	str	r3, [r7, #24]
	str	r3, [r7, #28]
	ldr	r5, [pc, #0]
	b.n	.L_080e2938
	.2byte 0x1000
	.2byte 0x0000
.L_080e2938:
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [r7, #24]
	movs	r3, #182
	lsls	r3, r3, #2
	adds	r3, #255
	adds	r2, r2, r3
	ldrh	r3, [r7, #6]
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r3, r3, r5
	adds	r0, #255
	str	r2, [r7, #24]
	str	r2, [r7, #28]
	strh	r3, [r7, #6]
	cmp	r2, r0
	ble.n	.L_080e2938
	movs	r5, #128
	lsls	r5, r5, #9
	mov	r0, sl
	str	r5, [r7, #24]
	str	r5, [r7, #28]
	movs	r1, #1
	bl	sub_08020090
	mov	r1, sl
	adds	r0, r7, #0
	bl	sub_080d3600
	mov	r3, sl
	ldrh	r2, [r3, #6]
	adds	r3, #100
	strh	r2, [r3, #0]
	movs	r0, #10
	bl	sub_080d2240
	mov	r0, r9
	movs	r1, #3
	bl	sub_080d3300
	movs	r1, #129
	lsls	r1, r1, #1
	mov	r0, r9
	bl	sub_080d489c
	movs	r0, #32
	bl	sub_080d2240
	str	r5, [r7, #72]
	movs	r5, #192
	lsls	r5, r5, #11
	movs	r0, #152
	str	r5, [r7, #40]
	bl	sub_081c0010
	movs	r0, #15
	bl	sub_080d2240
	movs	r0, #152
	str	r5, [r7, #40]
	bl	sub_081c0010
	movs	r0, #23
	bl	sub_080d2240
	movs	r0, #146
	bl	sub_081c0010
	ldr	r3, [r7, #8]
	add	r1, sp, #12
	str	r3, [r1, #0]
	ldr	r3, [r7, #12]
	mov	r0, sl
	str	r3, [r1, #4]
	ldr	r3, [r7, #16]
	mov	r2, sp
	str	r3, [r1, #8]
	mov	r9, r1
	ldr	r3, [r0, #8]
	mov	sl, r2
	str	r3, [r2, #0]
	ldr	r3, [r0, #12]
	str	r3, [r2, #4]
	ldr	r3, [r0, #16]
	movs	r0, #0
	str	r3, [r2, #8]
	movs	r3, #20
	mov	fp, r3
	mov	r8, r0
.L_080e29ec:
	mov	r2, sl
	mov	r0, r9
	ldr	r3, [r2, #0]
	ldr	r5, [r0, #0]
	movs	r1, #20
	subs	r3, r3, r5
	mov	r0, r8
	muls	r0, r3
	bl	sub_08002054
	mov	r2, r8
	adds	r5, r5, r0
	movs	r1, #20
	lsls	r0, r2, #15
	str	r5, [r7, #8]
	bl	sub_08002054
	bl	sub_08002096
	mov	r2, r9
	adds	r6, r0, #0
	mov	r0, sl
	ldr	r5, [r2, #4]
	ldr	r3, [r0, #4]
	movs	r1, #20
	subs	r3, r3, r5
	mov	r0, r8
	muls	r0, r3
	bl	sub_08002054
	lsls	r3, r6, #2
	adds	r3, r3, r6
	adds	r5, r5, r0
	lsls	r3, r3, #3
	adds	r5, r5, r3
	str	r5, [r7, #12]
	mov	r2, r9
	mov	r0, sl
	ldr	r5, [r2, #8]
	ldr	r3, [r0, #8]
	movs	r1, #20
	subs	r3, r3, r5
	mov	r0, r8
	muls	r0, r3
	bl	sub_08002054
	adds	r5, r5, r0
	str	r5, [r7, #16]
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #1
	add	r8, r3
	cmp	r8, fp
	blt.n	.L_080e29ec
.L_080e2a5a:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r0
	adds	r6, r1, #0
	mov	r9, r2
	mov	sl, r3
	cmp	r0, #31
	ble.n	.L_080e2a82
	movs	r1, #31
	mov	r8, r1
.L_080e2a82:
	movs	r7, #0
.L_080e2a84:
	adds	r0, r6, #0
	bl	sub_08002090
	mov	r5, r8
	muls	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	mov	r3, r8
	muls	r3, r0
	asrs	r3, r3, #16
	asrs	r5, r5, #16
	adds	r3, #32
	adds	r5, #32
	lsls	r3, r3, #6
	adds	r3, r3, r5
	lsrs	r2, r7, #31
	mov	r1, r9
	lsls	r3, r3, #1
	ldrh	r3, [r3, r1]
	adds	r2, r7, r2
	asrs	r2, r2, #1
	adds	r2, #225
	mov	r1, sl
	strb	r2, [r1, r3]
	movs	r3, #184
	lsls	r3, r3, #1
	adds	r7, #1
	adds	r6, r6, r3
	cmp	r7, #29
	ble.n	.L_080e2a84
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
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
	ldr	r1, [r2, #0]
	ldr	r2, [r3, #108]
	ldr	r1, [r1, #16]
	movs	r0, #204
	lsls	r0, r0, #4
	adds	r2, r2, r0
	mov	r9, r1
	ldr	r5, [r3, #92]
	mov	r0, r9
	movs	r3, #0
	ldrsh	r1, [r2, r3]
	ldr	r3, [r0, #8]
	lsls	r1, r1, #20
	subs	r1, r1, r3
	movs	r3, #2
	ldrsh	r0, [r2, r3]
	mov	r2, r9
	ldr	r3, [r2, #16]
	movs	r4, #128
	lsls	r0, r0, #20
	lsls	r4, r4, #12
	subs	r0, r0, r3
	adds	r1, r1, r4
	adds	r0, r0, r4
	sub	sp, #8
	bl	sub_080148e8
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	movs	r3, #216
	str	r0, [sp, #4]
	lsls	r3, r3, #5
	movs	r0, #216
	adds	r3, #6
	lsls	r0, r0, #5
	adds	r2, r5, r3
	adds	r0, #8
	movs	r3, #0
	strh	r3, [r2, #0]
	adds	r2, r5, r0
	movs	r3, #1
	strh	r3, [r2, #0]
	ldr	r0, [pc, #248]
	bl	sub_08013300
	adds	r1, r5, #0
	bl	sub_0801587c
	bl	sub_080143ac
	movs	r2, #192
	lsls	r2, r2, #1
	mov	r8, r2
	mov	r1, r8
	adds	r2, r5, #0
	adds	r6, r0, #0
	bl	sub_080142d4
	movs	r2, #216
	mov	fp, r0
	movs	r0, #216
	lsls	r0, r0, #5
	lsls	r2, r2, #5
	adds	r3, r5, r0
	adds	r2, #2
	strh	r6, [r3, #0]
	mov	r0, fp
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	movs	r3, #132
	movs	r2, #0
	lsls	r3, r3, #5
	mov	r0, r8
	mov	sl, r2
	adds	r6, r5, r3
	adds	r7, r5, r0
.L_080e2b7a:
	mov	r2, fp
	str	r2, [sp, #0]
	adds	r0, r7, #0
	movs	r1, #4
	movs	r2, #4
	movs	r3, #0
	bl	sub_080eaf98
	ldrb	r1, [r7, #9]
	movs	r0, #13
	movs	r3, #250
	negs	r0, r0
	strh	r3, [r7, #30]
	adds	r3, r0, #0
	ands	r1, r3
	ldrb	r3, [r7, #5]
	movs	r2, #32
	orrs	r3, r2
	strb	r3, [r7, #5]
	movs	r3, #15
	ands	r1, r3
	strb	r1, [r7, #9]
	mov	r2, r9
	ldr	r3, [r2, #8]
	str	r3, [r6, #0]
	ldr	r3, [r2, #12]
	str	r3, [r6, #4]
	ldr	r3, [r2, #16]
	str	r3, [r6, #8]
	bl	sub_08014878
	movs	r3, #128
	adds	r5, r0, #0
	lsls	r3, r3, #12
	lsls	r5, r5, #3
	adds	r5, r5, r3
	bl	sub_08014878
	adds	r2, r6, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	ldr	r0, [sp, #4]
	str	r0, [r6, #12]
	bl	sub_08014878
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r0, r0, r2
	str	r0, [r6, #16]
	bl	sub_08014878
	movs	r3, #1
	lsrs	r0, r0, #6
	mov	r2, sl
	add	r0, r8
	ands	r3, r2
	str	r0, [r6, #20]
	cmp	r3, #0
	beq.n	.L_080e2bf8
	negs	r3, r0
	str	r3, [r6, #20]
.L_080e2bf8:
	mov	r0, sl
	negs	r3, r0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	movs	r2, #1
	asrs	r3, r3, #1
	add	sl, r2
	str	r3, [r6, #24]
	mov	r3, sl
	adds	r7, #40
	adds	r6, #28
	cmp	r3, #95
	ble.n	.L_080e2b7a
	movs	r0, #220
	bl	sub_081c0010
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #20]
	bl	sub_080145a8
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x000001e0
	.2byte 0x2c39
	.2byte 0x080e
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #92]
	movs	r0, #216
	lsls	r0, r0, #5
	mov	r8, r3
	movs	r5, #132
	adds	r0, #2
	movs	r7, #192
	sub	sp, #12
	lsls	r5, r5, #5
	add	r0, r8
	lsls	r7, r7, #1
	movs	r3, #95
	add	r5, r8
	mov	r9, r0
	mov	r6, sp
	add	r7, r8
	mov	sl, r3
.L_080e2c68:
	ldr	r0, [r5, #24]
	cmp	r0, #63
	bhi.n	.L_080e2ce2
	movs	r1, #6
	asrs	r0, r0, #3
	bl	sub_08002064
	mov	r3, r9
	ldrh	r1, [r3, #0]
	ldr	r3, [pc, #56]
	lsls	r0, r0, #1
	adds	r1, r1, r0
	ands	r1, r3
	ldr	r2, [pc, #52]
	ldrh	r3, [r7, #8]
	movs	r0, #192
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r7, #8]
	adds	r2, r6, #0
	ldr	r3, [r5, #0]
	lsls	r0, r0, #14
	str	r3, [r6, #0]
	ldr	r3, [r5, #4]
	str	r3, [r6, #4]
	ldr	r3, [r5, #8]
	str	r3, [r6, #8]
	ldr	r1, [r5, #12]
	bl	sub_0801489c
	adds	r0, r6, #0
	bl	sub_080dc390
	ldr	r3, [r6, #0]
	adds	r0, r7, #0
	str	r3, [r7, #12]
	ldr	r3, [r6, #8]
	b.n	.L_080e2cbc
	.4byte 0x000003ff
	.2byte 0xfc00
	.2byte 0xffff
.L_080e2cbc:
	str	r3, [r7, #16]
	bl	sub_080eb01c
	ldr	r3, [r5, #12]
	ldr	r1, [r5, #20]
	ldr	r2, [r5, #16]
	adds	r3, r3, r1
	str	r3, [r5, #12]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	adds	r3, r1, #0
	adds	r3, #8
	cmp	r1, #0
	bge.n	.L_080e2cde
	adds	r3, r1, #0
	subs	r3, #8
.L_080e2cde:
	str	r3, [r5, #20]
	ldr	r0, [r5, #24]
.L_080e2ce2:
	adds	r3, r0, #1
	movs	r0, #1
	negs	r0, r0
	add	sl, r0
	str	r3, [r5, #24]
	mov	r3, sl
	adds	r7, #40
	adds	r5, #28
	cmp	r3, #0
	bge.n	.L_080e2c68
	movs	r0, #216
	lsls	r0, r0, #5
	adds	r0, #4
	add	r0, r8
	movs	r4, #0
	movs	r1, #216
	strh	r4, [r0, #0]
	lsls	r1, r1, #5
	adds	r1, #6
	add	r1, r8
	movs	r5, #0
	ldrsh	r3, [r1, r5]
	ldrh	r2, [r1, #0]
	cmp	r3, #30
	bne.n	.L_080e2d1a
	movs	r3, #1
	strh	r3, [r0, #0]
	ldrh	r2, [r1, #0]
.L_080e2d1a:
	movs	r0, #224
	lsls	r3, r2, #16
	lsls	r0, r0, #15
	cmp	r3, r0
	bne.n	.L_080e2d30
	movs	r3, #216
	lsls	r3, r3, #5
	adds	r3, #8
	add	r3, r8
	strh	r4, [r3, #0]
	ldrh	r2, [r1, #0]
.L_080e2d30:
	adds	r3, r2, #1
	strh	r3, [r1, #0]
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #92]
	ldr	r0, [pc, #20]
	bl	sub_08014644
	movs	r3, #216
	lsls	r3, r3, #5
	adds	r5, r5, r3
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl	sub_08014274
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0x2c39
	.2byte 0x080e
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r2, r3, #0
	ldr	r6, [r3, #92]
	adds	r2, #224
	movs	r1, #216
	ldr	r2, [r2, #0]
	lsls	r1, r1, #5
	adds	r1, #18
	adds	r1, r6, r1
	sub	sp, #20
	movs	r3, #216
	ldr	r2, [r2, #16]
	lsls	r3, r3, #5
	str	r1, [sp, #4]
	adds	r3, #16
	adds	r3, r3, r6
	mov	sl, r2
	mov	r9, r3
	movs	r2, #0
	ldrsh	r7, [r1, r2]
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_080e2daa
	cmp	r3, #1
	beq.n	.L_080e2e82
	b.n	.L_080e2e8c
.L_080e2daa:
	lsls	r2, r7, #8
	mov	fp, r2
	mov	r0, fp
	bl	sub_08002096
	movs	r3, #216
	lsls	r3, r3, #5
	adds	r3, #24
	adds	r3, r6, r3
	lsls	r0, r0, #5
	movs	r1, #216
	str	r3, [sp, #0]
	lsls	r1, r1, #5
	str	r0, [r3, #0]
	adds	r1, #20
	adds	r1, r1, r6
	ldr	r3, [r1, #0]
	movs	r2, #160
	lsls	r2, r2, #3
	adds	r3, r3, r2
	str	r3, [r1, #0]
	mov	r8, r1
	movs	r3, #216
	movs	r1, #216
	lsls	r3, r3, #5
	lsls	r1, r1, #5
	adds	r3, #28
	adds	r1, #40
	adds	r2, r6, r3
	adds	r3, r6, r1
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #0]
	add	r5, sp, #8
	subs	r3, r3, r2
	muls	r3, r7
	cmp	r3, #0
	bge.n	.L_080e2df6
	adds	r3, #127
.L_080e2df6:
	asrs	r3, r3, #7
	adds	r3, r2, r3
	mov	r0, fp
	str	r3, [r5, #0]
	bl	sub_08002096
	movs	r1, #216
	movs	r3, #217
	lsls	r1, r1, #5
	lsls	r3, r3, #5
	adds	r1, #44
	adds	r2, r6, r3
	adds	r3, r6, r1
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #0]
	subs	r3, r3, r2
	muls	r3, r7
	cmp	r3, #0
	bge.n	.L_080e2e1e
	adds	r3, #127
.L_080e2e1e:
	asrs	r3, r3, #7
	adds	r3, r2, r3
	lsls	r2, r0, #6
	adds	r3, r3, r2
	str	r3, [r5, #4]
	movs	r1, #216
	movs	r3, #216
	lsls	r3, r3, #5
	lsls	r1, r1, #5
	adds	r3, #36
	adds	r1, #48
	adds	r2, r6, r3
	adds	r3, r6, r1
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #0]
	subs	r3, r3, r2
	muls	r3, r7
	cmp	r3, #0
	bge.n	.L_080e2e46
	adds	r3, #127
.L_080e2e46:
	asrs	r3, r3, #7
	adds	r3, r2, r3
	str	r3, [r5, #8]
	ldr	r2, [sp, #0]
	mov	r3, r8
	ldr	r1, [r3, #0]
	ldr	r0, [r2, #0]
	adds	r2, r5, #0
	bl	sub_0801489c
	ldr	r3, [r5, #0]
	mov	r1, sl
	str	r3, [r1, #8]
	ldr	r3, [r5, #4]
	str	r3, [r1, #12]
	ldr	r3, [r5, #8]
	str	r3, [r1, #16]
	cmp	r7, #127
	ble.n	.L_080e2e8c
	mov	r2, r9
	ldrh	r3, [r2, #0]
	mov	r1, r9
	adds	r3, #1
	strh	r3, [r1, #0]
	ldr	r2, [sp, #4]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	strh	r3, [r2, #0]
	b.n	.L_080e2e8c
.L_080e2e82:
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	mov	r1, r9
	strh	r3, [r1, #0]
.L_080e2e8c:
	movs	r3, #216
	lsls	r3, r3, #5
	adds	r3, #18
	adds	r2, r6, r3
	ldrh	r3, [r2, #0]
	movs	r1, #216
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r1, r1, #5
	adds	r1, #12
	adds	r3, r6, r1
	mov	r1, sl
	ldr	r2, [r3, #0]
	ldr	r3, [r1, #8]
	add	sp, #20
	str	r3, [r2, #8]
	ldr	r3, [r1, #12]
	str	r3, [r2, #12]
	ldr	r3, [r1, #16]
	str	r3, [r2, #16]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r3, r2, #0
	adds	r3, #224
	ldr	r6, [r2, #92]
	ldr	r1, [r3, #0]
	movs	r0, #216
	movs	r3, #216
	lsls	r0, r0, #5
	lsls	r3, r3, #5
	adds	r0, #18
	adds	r3, #16
	adds	r0, r0, r6
	adds	r3, r3, r6
	mov	sl, r0
	movs	r2, #0
	ldrsh	r7, [r0, r2]
	mov	r8, r3
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	sub	sp, #16
	ldr	r4, [r1, #16]
	cmp	r3, #1
	beq.n	.L_080e2fd6
	cmp	r3, #1
	bgt.n	.L_080e2f02
	cmp	r3, #0
	beq.n	.L_080e2f0a
	b.n	.L_080e302e
.L_080e2f02:
	cmp	r3, #2
	bne.n	.L_080e2f08
	b.n	.L_080e3014
.L_080e2f08:
	b.n	.L_080e302e
.L_080e2f0a:
	lsls	r0, r7, #8
	str	r4, [sp, #0]
	bl	sub_08002096
	movs	r2, #216
	lsls	r2, r2, #5
	adds	r2, #24
	adds	r2, r2, r6
	lsls	r0, r0, #5
	movs	r3, #216
	str	r0, [r2, #0]
	lsls	r3, r3, #5
	adds	r3, #20
	adds	r1, r6, r3
	ldr	r3, [r1, #0]
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r3, r3, r0
	str	r3, [r1, #0]
	movs	r0, #216
	movs	r3, #216
	lsls	r3, r3, #5
	lsls	r0, r0, #5
	adds	r3, #28
	adds	r0, #40
	mov	ip, r2
	adds	r2, r6, r3
	adds	r3, r6, r0
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #0]
	add	r5, sp, #4
	subs	r3, r3, r2
	muls	r3, r7
	ldr	r4, [sp, #0]
	cmp	r3, #0
	bge.n	.L_080e2f54
	adds	r3, #127
.L_080e2f54:
	asrs	r3, r3, #7
	adds	r3, r2, r3
	movs	r0, #216
	str	r3, [r5, #0]
	lsls	r0, r0, #5
	movs	r3, #217
	lsls	r3, r3, #5
	adds	r0, #44
	adds	r2, r6, r3
	adds	r3, r6, r0
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #0]
	subs	r3, r3, r2
	muls	r3, r7
	cmp	r3, #0
	bge.n	.L_080e2f76
	adds	r3, #127
.L_080e2f76:
	asrs	r3, r3, #7
	adds	r3, r2, r3
	str	r3, [r5, #4]
	movs	r0, #216
	movs	r3, #216
	lsls	r3, r3, #5
	lsls	r0, r0, #5
	adds	r3, #36
	adds	r0, #48
	adds	r2, r6, r3
	adds	r3, r6, r0
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #0]
	subs	r3, r3, r2
	muls	r3, r7
	cmp	r3, #0
	bge.n	.L_080e2f9a
	adds	r3, #127
.L_080e2f9a:
	asrs	r3, r3, #7
	adds	r3, r2, r3
	str	r3, [r5, #8]
	mov	r2, ip
	ldr	r0, [r2, #0]
	ldr	r1, [r1, #0]
	adds	r2, r5, #0
	str	r4, [sp, #0]
	bl	sub_0801489c
	ldr	r3, [r5, #0]
	ldr	r4, [sp, #0]
	str	r3, [r4, #8]
	ldr	r3, [r5, #4]
	str	r3, [r4, #12]
	ldr	r3, [r5, #8]
	str	r3, [r4, #16]
	cmp	r7, #127
	ble.n	.L_080e302e
	mov	r0, r8
	ldrh	r3, [r0, #0]
	mov	r2, r8
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	mov	r0, sl
	strh	r3, [r0, #0]
	b.n	.L_080e302e
.L_080e2fd6:
	ldr	r3, [r4, #12]
	ldr	r2, [pc, #128]
	adds	r3, r3, r2
	str	r3, [r4, #12]
	adds	r3, r1, #0
	adds	r3, #52
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #1
	bne.n	.L_080e2ffc
	cmp	r7, #40
	bne.n	.L_080e302e
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	mov	r0, r8
	strh	r3, [r0, #0]
	b.n	.L_080e302e
.L_080e2ffc:
	cmp	r7, #40
	bne.n	.L_080e302e
	mov	r2, r8
	ldrh	r3, [r2, #0]
	mov	r0, r8
	adds	r3, #1
	strh	r3, [r0, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	mov	r2, sl
	b.n	.L_080e302c
.L_080e3014:
	ldr	r3, [r4, #12]
	movs	r0, #243
	lsls	r0, r0, #10
	adds	r0, #204
	adds	r3, r3, r0
	str	r3, [r4, #12]
	cmp	r7, #40
	bne.n	.L_080e302e
	movs	r3, #186
	lsls	r3, r3, #2
	adds	r3, #255
	mov	r2, r8
.L_080e302c:
	strh	r3, [r2, #0]
.L_080e302e:
	movs	r3, #216
	lsls	r3, r3, #5
	adds	r3, #18
	adds	r2, r6, r3
	ldrh	r3, [r2, #0]
	movs	r0, #216
	adds	r3, #1
	strh	r3, [r2, #0]
	lsls	r0, r0, #5
	adds	r0, #12
	adds	r3, r6, r0
	ldr	r2, [r3, #0]
	ldr	r3, [r4, #8]
	add	sp, #16
	str	r3, [r2, #8]
	ldr	r3, [r4, #12]
	str	r3, [r2, #12]
	ldr	r3, [r4, #16]
	str	r3, [r2, #16]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0xffff999a

.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08014878, 0x08014878
	.set sub_0801489c, 0x0801489c
	.set sub_08020090, 0x08020090
	.set sub_080200a8, 0x080200a8
	.set sub_080200c8, 0x080200c8
	.set sub_08020148, 0x08020148
	.set sub_08020150, 0x08020150
	.set sub_08020218, 0x08020218
	.set sub_08020278, 0x08020278
	.set sub_08020330, 0x08020330
	.set sub_080daecc, 0x080daecc
	.set sub_080db974, 0x080db974
	.set sub_080dc10c, 0x080dc10c
	.set sub_080dc294, 0x080dc294
	.set sub_080dc384, 0x080dc384
	.set sub_080dd668, 0x080dd668
	.set sub_081c0010, 0x081c0010
	.global Overlay_080dda30
Overlay_080dda30:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r7, [r3, #0]
	sub	sp, #12
	ldr	r5, [r7, #20]
	movs	r6, #0
	cmp	r5, #0
	beq.n	.L_080ddb30
	bl	.L_080ddbd8
	adds	r0, r5, #0
	bl	sub_08020330
	movs	r3, #186
	lsls	r3, r3, #1
	cmp	r0, r3
	bne.n	.L_080dda5c
	movs	r6, #1
.L_080dda5c:
	adds	r0, r5, #0
	bl	sub_08020330
	movs	r3, #197
	lsls	r3, r3, #1
	cmp	r0, r3
	bne.n	.L_080dda6c
	movs	r6, #2
.L_080dda6c:
	cmp	r6, #0
	bne.n	.L_080ddaa6
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_08020090
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_08020218
	adds	r3, r5, #0
	adds	r3, #89
	strb	r6, [r3, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	movs	r0, #10
	bl	sub_08013560
	movs	r0, #126
	bl	sub_081c0010
	movs	r0, #40
	bl	sub_08013560
	b.n	.L_080ddb2c
.L_080ddaa6:
	cmp	r6, #1
	bne.n	.L_080ddb20
	movs	r3, #4
	mov	r6, sp
	mov	r8, r3
.L_080ddab0:
	ldr	r3, [r7, #4]
	str	r3, [r6, #0]
	ldr	r3, [r7, #12]
	str	r3, [r6, #8]
	bl	sub_08014878
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r3, #128
	lsls	r3, r3, #11
	lsls	r5, r5, #1
	adds	r5, r5, r3
	bl	sub_08014878
	adds	r2, r6, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	ldr	r2, [r7, #8]
	movs	r0, #26
	str	r2, [r6, #4]
	ldr	r1, [r6, #0]
	ldr	r3, [r6, #8]
	adds	r0, #255
	bl	sub_080dc10c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080ddafa
	ldr	r1, [pc, #72]
	bl	sub_080200a8
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #2
	strb	r3, [r2, #0]
.L_080ddafa:
	bl	sub_08014878
	lsls	r0, r0, #1
	lsrs	r0, r0, #16
	adds	r0, #2
	bl	sub_08013560
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_080ddab0
	movs	r3, #26
	ldrsh	r0, [r7, r3]
	movs	r1, #1
	bl	sub_080daecc
	b.n	.L_080ddb2c
.L_080ddb20:
	movs	r0, #10
	bl	sub_08013560
	movs	r0, #126
	bl	sub_081c0010
.L_080ddb2c:
	bl	sub_080dc384
.L_080ddb30:
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0ee8
	.2byte 0x080f
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	sub	sp, #12
	ldr	r7, [r3, #0]
	bl	.L_080ddbd8
	movs	r0, #134
	bl	sub_081c0010
	movs	r3, #4
	mov	r6, sp
	mov	r8, r3
.L_080ddb5c:
	ldr	r3, [r7, #4]
	str	r3, [r6, #0]
	ldr	r3, [r7, #12]
	str	r3, [r6, #8]
	bl	sub_08014878
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r3, #128
	lsls	r3, r3, #11
	lsls	r5, r5, #1
	adds	r5, r5, r3
	bl	sub_08014878
	adds	r2, r6, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	ldr	r2, [r7, #8]
	movs	r0, #26
	ldr	r1, [r6, #0]
	ldr	r3, [r6, #8]
	adds	r0, #255
	str	r2, [r6, #4]
	bl	sub_080dc10c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080ddba6
	ldr	r1, [pc, #56]
	bl	sub_080200a8
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #2
	strb	r3, [r2, #0]
.L_080ddba6:
	bl	sub_08014878
	lsls	r0, r0, #1
	lsrs	r0, r0, #16
	adds	r0, #2
	bl	sub_08013560
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_080ddb5c
	movs	r0, #30
	bl	sub_08013560
	bl	sub_080dc384
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0ee8
	.2byte 0x080f
.L_080ddbd8:
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
	sub	sp, #16
	ldr	r0, [r3, #16]
	mov	r9, r3
	mov	r8, r0
	bl	sub_080dc294
	movs	r2, #4
	movs	r3, #23
	add	r2, sp
	str	r3, [sp, #0]
	mov	sl, r2
	mov	fp, sl
.L_080ddc06:
	mov	r0, r9
	ldrh	r3, [r0, #0]
	movs	r2, #128
	lsls	r2, r2, #7
	cmp	r3, r2
	bne.n	.L_080ddc22
	mov	r0, r8
	ldr	r3, [r0, #8]
	mov	r2, fp
	str	r3, [r2, #0]
	ldr	r3, [r0, #12]
	movs	r0, #160
	lsls	r0, r0, #12
	b.n	.L_080ddc38
.L_080ddc22:
	movs	r2, #192
	lsls	r2, r2, #8
	cmp	r3, r2
	bne.n	.L_080ddc46
	mov	r0, r8
	ldr	r3, [r0, #8]
	mov	r2, fp
	str	r3, [r2, #0]
	ldr	r3, [r0, #12]
	movs	r0, #192
	lsls	r0, r0, #13
.L_080ddc38:
	adds	r3, r3, r0
	str	r3, [r2, #4]
	mov	r2, r8
	ldr	r3, [r2, #16]
	mov	r0, fp
	str	r3, [r0, #8]
	b.n	.L_080ddc6a
.L_080ddc46:
	mov	r2, r8
	ldr	r3, [r2, #8]
	mov	r0, fp
	str	r3, [r0, #0]
	movs	r0, #160
	ldr	r3, [r2, #12]
	lsls	r0, r0, #12
	adds	r3, r3, r0
	mov	r2, fp
	str	r3, [r2, #4]
	mov	r2, r8
	ldr	r3, [r2, #16]
	mov	r2, fp
	str	r3, [r2, #8]
	mov	r3, r9
	ldrh	r1, [r3, #0]
	bl	sub_0801489c
.L_080ddc6a:
	mov	r0, fp
	ldr	r1, [r0, #0]
	ldr	r2, [r0, #4]
	ldr	r3, [r0, #8]
	movs	r0, #168
	lsls	r0, r0, #2
	bl	sub_080dc10c
	adds	r7, r0, #0
	ldr	r6, [r7, #80]
	movs	r0, #33
	adds	r4, r6, #0
	adds	r4, #28
	ldrb	r3, [r6, #5]
	ldrb	r1, [r4, #5]
	negs	r0, r0
	movs	r2, #32
	ands	r2, r3
	adds	r3, r0, #0
	ands	r1, r3
	orrs	r1, r2
	strb	r1, [r4, #5]
	ldrb	r0, [r4, #7]
	ldrb	r2, [r6, #7]
	movs	r5, #63
	lsrs	r2, r2, #6
	adds	r3, r5, #0
	lsls	r2, r2, #6
	ands	r3, r0
	orrs	r3, r2
	strb	r3, [r4, #7]
	ands	r1, r5
	ldrb	r3, [r6, #5]
	ldr	r2, [pc, #44]
	lsrs	r3, r3, #6
	lsls	r3, r3, #6
	orrs	r1, r3
	strb	r1, [r4, #5]
	ldrh	r3, [r4, #8]
	ldrh	r1, [r6, #8]
	ands	r3, r2
	lsls	r1, r1, #22
	lsrs	r1, r1, #22
	orrs	r3, r1
	strh	r3, [r4, #8]
	movs	r3, #15
	ldrb	r2, [r6, #9]
	ldrb	r1, [r4, #9]
	lsrs	r2, r2, #4
	lsls	r2, r2, #4
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r4, #9]
	cmp	r7, #0
	beq.n	.L_080ddd70
	b.n	.L_080ddce0
	movs	r0, r0
	.2byte 0xfc00
	.2byte 0xffff
.L_080ddce0:
	movs	r3, #179
	lsls	r3, r3, #8
	adds	r3, #51
	str	r3, [r7, #28]
	str	r3, [r7, #24]
	movs	r3, #192
	lsls	r3, r3, #9
	adds	r2, r7, #0
	adds	r2, #85
	str	r3, [r7, #52]
	str	r3, [r7, #48]
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r0, r7, #0
	movs	r1, #11
	bl	sub_08020278
	adds	r0, r7, #0
	movs	r1, #7
	bl	sub_08020090
	adds	r0, r7, #0
	ldr	r1, [pc, #140]
	bl	sub_080200a8
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08020218
	mov	r2, r9
	ldr	r3, [r2, #4]
	mov	r0, sl
	str	r3, [r0, #0]
	ldr	r3, [r2, #8]
	str	r3, [r0, #4]
	ldr	r3, [r2, #12]
	str	r3, [r0, #8]
	ldrh	r3, [r2, #0]
	movs	r2, #192
	lsls	r2, r2, #8
	cmp	r3, r2
	bne.n	.L_080ddd42
	mov	r3, r9
	movs	r0, #224
	ldrh	r1, [r3, #0]
	lsls	r0, r0, #12
	mov	r2, sl
	bl	sub_0801489c
.L_080ddd42:
	bl	sub_08014878
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r0, #128
	lsls	r0, r0, #11
	lsls	r5, r5, #1
	adds	r5, r5, r0
	bl	sub_08014878
	mov	r2, sl
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	mov	r2, sl
	mov	r0, sl
	ldr	r1, [r2, #0]
	ldr	r3, [r0, #8]
	ldr	r2, [r2, #4]
	adds	r0, r7, #0
	bl	sub_08020148
.L_080ddd70:
	movs	r0, #131
	bl	sub_081c0010
	movs	r0, #2
	bl	sub_08013560
	ldr	r2, [sp, #0]
	subs	r2, #1
	str	r2, [sp, #0]
	cmp	r2, #0
	blt.n	.L_080ddd88
	b.n	.L_080ddc06
.L_080ddd88:
	movs	r0, #8
	bl	sub_08013560
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0e58
	.2byte 0x080f
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r6, [r3, #0]
	sub	sp, #20
	ldr	r7, [r6, #20]
	ldr	r5, [r6, #16]
	cmp	r7, #0
	beq.n	.L_080dde92
	bl	sub_080dc294
	adds	r0, r5, #0
	str	r7, [r5, #104]
	ldr	r1, [pc, #216]
	bl	sub_080200a8
	ldr	r0, [r6, #4]
	add	r5, sp, #8
	str	r0, [r5, #0]
	movs	r2, #128
	ldr	r1, [r6, #8]
	lsls	r2, r2, #13
	adds	r1, r1, r2
	str	r1, [r5, #4]
	movs	r3, #128
	ldr	r2, [r6, #12]
	lsls	r3, r3, #14
	adds	r0, r0, r3
	movs	r3, #128
	str	r2, [r5, #8]
	lsls	r3, r3, #8
	bl	.L_080ddfd4
	ldr	r2, [pc, #180]
	str	r0, [sp, #0]
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	adds	r0, r0, r2
	movs	r3, #0
	ldr	r2, [r5, #8]
	bl	.L_080ddfd4
	str	r0, [sp, #4]
	movs	r0, #15
	mov	r8, sp
	bl	sub_08013560
	mov	r6, r8
	movs	r5, #1
.L_080dde08:
	ldmia	r6!, {r0}
	cmp	r0, #0
	beq.n	.L_080dde18
	movs	r1, #224
	ldrh	r2, [r0, #6]
	lsls	r1, r1, #12
	bl	sub_080db974
.L_080dde18:
	subs	r5, #1
	cmp	r5, #0
	bge.n	.L_080dde08
	ldr	r0, [sp, #0]
	bl	sub_08020150
	ldr	r3, [pc, #124]
	movs	r0, #130
	str	r3, [r7, #108]
	bl	sub_081c0010
	adds	r2, r7, #0
	ldr	r0, [sp, #0]
	adds	r2, #85
	movs	r3, #4
	strb	r3, [r2, #0]
	ldr	r5, [r7, #12]
	cmp	r0, #0
	beq.n	.L_080dde82
	mov	r2, r8
	ldr	r3, [r2, #4]
	cmp	r3, #0
	beq.n	.L_080dde82
	movs	r2, #128
	lsls	r2, r2, #14
	adds	r3, r5, r2
	cmp	r5, r3
	bgt.n	.L_080dde82
	b.n	.L_080dde54
.L_080dde52:
	ldr	r0, [sp, #0]
.L_080dde54:
	ldr	r3, [r0, #12]
	movs	r1, #128
	lsls	r1, r1, #7
	adds	r3, r3, r1
	str	r3, [r0, #12]
	mov	r3, r8
	ldr	r2, [r3, #4]
	movs	r0, #1
	ldr	r3, [r2, #12]
	adds	r3, r3, r1
	str	r3, [r2, #12]
	ldr	r3, [r7, #12]
	adds	r3, r3, r1
	str	r3, [r7, #12]
	bl	sub_08013560
	movs	r3, #128
	lsls	r3, r3, #14
	adds	r2, r5, r3
	ldr	r3, [r7, #12]
	cmp	r3, r2
	ble.n	.L_080dde52
	ldr	r0, [sp, #0]
.L_080dde82:
	bl	sub_080dd668
	mov	r2, r8
	ldr	r0, [r2, #4]
	bl	sub_080dd668
	bl	sub_080dc384
.L_080dde92:
	add	sp, #20
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x080f0e60
	.4byte 0xffe00000
	.2byte 0xb91d
	.2byte 0x080d
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
	sub	sp, #20
	mov	sl, r3
	bl	sub_080dc294
	mov	r3, sl
	ldr	r0, [r3, #4]
	add	r5, sp, #8
	str	r0, [r5, #0]
	ldr	r1, [r3, #8]
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r1, r1, r3
	str	r1, [r5, #4]
	mov	r3, sl
	ldr	r2, [r3, #12]
	movs	r3, #128
	lsls	r3, r3, #14
	adds	r0, r0, r3
	movs	r3, #128
	str	r2, [r5, #8]
	lsls	r3, r3, #8
	bl	.L_080ddfd4
	ldr	r3, [pc, #220]
	str	r0, [sp, #0]
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	adds	r0, r0, r3
	ldr	r2, [r5, #8]
	movs	r3, #0
	bl	.L_080ddfd4
	str	r0, [sp, #4]
	movs	r0, #15
	mov	fp, sp
	bl	sub_08013560
	movs	r0, #1
	mov	r7, fp
	mov	r8, r0
.L_080ddf0e:
	ldmia	r7!, {r6}
	cmp	r6, #0
	beq.n	.L_080ddf20
	movs	r1, #192
	ldrh	r2, [r6, #6]
	adds	r0, r6, #0
	lsls	r1, r1, #13
	bl	sub_080db974
.L_080ddf20:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r0, r8
	cmp	r0, #0
	bge.n	.L_080ddf0e
	ldr	r0, [sp, #0]
	bl	sub_08020150
	movs	r0, #134
	bl	sub_081c0010
	movs	r0, #128
	movs	r3, #23
	lsls	r0, r0, #10
	adds	r7, r5, #0
	mov	r8, r3
	mov	r9, r0
.L_080ddf44:
	mov	r3, sl
	ldr	r1, [r3, #4]
	movs	r0, #128
	str	r1, [r7, #0]
	lsls	r0, r0, #13
	ldr	r2, [r3, #8]
	adds	r2, r2, r0
	str	r2, [r7, #4]
	movs	r0, #209
	ldr	r3, [r3, #12]
	lsls	r0, r0, #1
	str	r3, [r7, #8]
	adds	r0, #255
	bl	sub_080dc10c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080ddfa0
	ldr	r1, [pc, #100]
	bl	sub_080200a8
	bl	sub_08014878
	mov	r3, r9
	adds	r2, r6, #0
	adds	r2, #85
	str	r3, [r6, #52]
	add	r0, r9
	movs	r3, #0
	str	r0, [r6, #48]
	strb	r3, [r2, #0]
	bl	sub_08014878
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r0, #128
	lsls	r0, r0, #12
	lsls	r5, r5, #3
	adds	r5, r5, r0
	bl	sub_08014878
	adds	r1, r5, #0
	adds	r2, r0, #0
	adds	r0, r6, #0
	bl	sub_080db974
.L_080ddfa0:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r0, r8
	cmp	r0, #0
	bge.n	.L_080ddf44
	ldr	r0, [sp, #0]
	bl	sub_080200c8
	mov	r3, fp
	ldr	r0, [r3, #4]
	bl	sub_080200c8
	bl	sub_080dc384
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffe00000
	.2byte 0x0e78
	.2byte 0x080f
.L_080ddfd4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r0, #138
	adds	r6, r1, #0
	mov	r8, r2
	adds	r7, r3, #0
	bl	sub_081c0010
	movs	r0, #139
	adds	r1, r5, #0
	lsls	r0, r0, #1
	adds	r2, r6, #0
	mov	r3, r8
	bl	sub_080dc10c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080de056
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r5, #48]
	str	r3, [r5, #52]
	ldr	r1, [r5, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r1, #9]
	movs	r1, #3
	bl	sub_08020090
	ldr	r3, [r5, #24]
	movs	r2, #128
	lsls	r2, r2, #9
	cmp	r3, r2
	bge.n	.L_080de054
	ldr	r6, [pc, #36]
.L_080de02a:
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r3, r2
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	ldrh	r3, [r5, #6]
	movs	r0, #1
	adds	r3, r3, r6
	strh	r3, [r5, #6]
	bl	sub_08013560
	movs	r2, #255
	ldr	r3, [r5, #24]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	ble.n	.L_080de02a
	b.n	.L_080de054
	movs	r0, r0
	.2byte 0x2000
	.2byte 0x0000
.L_080de054:
	strh	r7, [r5, #6]
.L_080de056:
	adds	r0, r5, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}

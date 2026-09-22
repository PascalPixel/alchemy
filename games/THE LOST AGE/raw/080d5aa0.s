.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_0801489c, 0x0801489c
	.set sub_08016cfc, 0x08016cfc
	.set sub_08020000, 0x08020000
	.set sub_08020058, 0x08020058
	.set sub_08020090, 0x08020090
	.set sub_08020148, 0x08020148
	.set sub_08020150, 0x08020150
	.set sub_080201d0, 0x080201d0
	.set sub_08020210, 0x08020210
	.set sub_080cad84, 0x080cad84
	.set sub_080d0520, 0x080d0520
	.set sub_081c0010, 0x081c0010
	.global Overlay_080d5aa0
Overlay_080d5aa0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r2, [pc, #300]
	movs	r3, #133
	mov	sl, r2
	lsls	r3, r3, #2
	add	r3, sl
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl	sub_080cad84
	adds	r5, r0, #0
	ldr	r3, [r5, #80]
	movs	r2, #194
	mov	r8, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	strh	r2, [r3, #0]
	ldr	r3, [pc, #264]
	ldr	r1, [pc, #268]
	ldr	r3, [r3, #0]
	movs	r2, #15
	lsrs	r3, r3, #4
	ands	r3, r2
	lsls	r3, r3, #1
	ldrh	r7, [r1, r3]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	movs	r0, #0
	cmp	r7, r3
	beq.n	.L_080d5bc8
	ldr	r2, [r5, #8]
	ldr	r3, [r5, #16]
	ldr	r0, [pc, #240]
	movs	r1, #128
	lsls	r1, r1, #12
	ands	r2, r0
	ands	r3, r0
	mov	r6, sp
	adds	r2, r2, r1
	adds	r3, r3, r1
	str	r3, [r5, #16]
	str	r2, [r5, #8]
	str	r2, [r6, #0]
	movs	r0, #128
	ldr	r3, [r5, #12]
	lsls	r0, r0, #13
	str	r3, [r6, #4]
	adds	r1, r7, #0
	ldr	r3, [r5, #16]
	adds	r2, r6, #0
	str	r3, [r6, #8]
	bl	sub_0801489c
	ldr	r2, [pc, #204]
	ldr	r3, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #20]
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #8]
	bl	sub_080201d0
	cmp	r0, #0
	bne.n	.L_080d5b40
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_08020210
	cmp	r0, #0
	beq.n	.L_080d5b44
.L_080d5b40:
	movs	r0, #0
	b.n	.L_080d5bc8
.L_080d5b44:
	strh	r7, [r5, #6]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r5, #0
	adds	r3, #100
	ldrh	r3, [r3, #0]
	adds	r2, r5, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	adds	r3, r5, #0
	adds	r3, #102
	ldrh	r3, [r3, #0]
	mov	r2, r8
	strb	r3, [r2, #26]
	mov	r3, r8
	strh	r0, [r3, #18]
	movs	r0, #152
	bl	sub_081c0010
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #52]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r5, #48]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #40]
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_08020090
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	adds	r0, r5, #0
	bl	sub_08020148
	adds	r0, r5, #0
	bl	sub_08020150
	adds	r3, r5, #0
	adds	r3, #98
	ldrb	r2, [r3, #0]
	movs	r1, #3
	mov	r3, r8
	ands	r2, r1
	ldrb	r1, [r3, #9]
	movs	r3, #13
	negs	r3, r3
	lsls	r2, r2, #2
	ands	r3, r1
	orrs	r3, r2
	mov	r2, r8
	strb	r3, [r2, #9]
	movs	r2, #128
	lsls	r2, r2, #2
	ldr	r3, [pc, #20]
	adds	r2, #18
	add	r2, sl
	strb	r3, [r2, #0]
	movs	r0, #1
.L_080d5bc8:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x03001150
	.4byte 0x080f0890
	.4byte 0xfff00000
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb560
	adds	r5, r1, #0
	bl	sub_080cad84
	adds	r4, r0, #0
	cmp	r4, #0
	bne.n	.L_080d5c00
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080d5c6a
.L_080d5c00:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	ldr	r1, [pc, #100]
	adds	r3, #228
	ldr	r0, [r3, #0]
	ldr	r2, [r3, #4]
	ldr	r3, [r4, #16]
	ands	r2, r1
	ands	r0, r1
	ldr	r1, [r4, #8]
	subs	r3, r3, r2
	ldr	r2, [r4, #12]
	subs	r1, r1, r0
	subs	r6, r3, r2
	adds	r2, r5, #0
	adds	r5, #4
	cmp	r1, #0
	bge.n	.L_080d5c2e
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r1, r1, r3
.L_080d5c2e:
	asrs	r3, r1, #16
	str	r3, [r2, #0]
	adds	r3, r6, #0
	cmp	r3, #0
	bge.n	.L_080d5c40
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r3, r3, r2
.L_080d5c40:
	asrs	r3, r3, #16
	str	r3, [r5, #0]
	adds	r3, r4, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #1
	bne.n	.L_080d5c68
	ldr	r3, [r4, #80]
	ldr	r3, [r3, #40]
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	bl	sub_08020000
	ldr	r3, [r5, #0]
	movs	r2, #8
	ldrsb	r2, [r0, r2]
	subs	r3, r3, r2
	str	r3, [r5, #0]
.L_080d5c68:
	movs	r0, #0
.L_080d5c6a:
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #108]
	movs	r1, #128
	ldr	r3, [pc, #228]
	lsls	r1, r1, #2
	adds	r1, #14
	adds	r3, r3, r1
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	sub	sp, #8
	bl	sub_081c0010
	movs	r0, #149
	lsls	r0, r0, #1
	bl	sub_081c0010
	movs	r0, #147
	bl	sub_081c0010
	movs	r1, #197
	lsls	r1, r1, #1
	adds	r3, r6, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080d5d04
	ldr	r3, [pc, #184]
	movs	r2, #254
	lsls	r2, r2, #7
	adds	r2, #255
	strh	r2, [r3, #0]
	movs	r0, #128
	lsls	r0, r0, #3
	movs	r1, #16
	adds	r0, #1
	bl	sub_080d0520
	movs	r3, #217
	lsls	r3, r3, #1
	adds	r2, r6, r3
	movs	r3, #0
	strh	r3, [r2, #0]
	movs	r0, #16
	bl	sub_08013560
	movs	r7, #240
	movs	r1, #240
	movs	r5, #0
	lsls	r1, r1, #2
	lsls	r7, r7, #7
	movs	r6, #30
.L_080d5ce0:
	adds	r3, r7, #0
	ldr	r2, [pc, #132]
	orrs	r3, r1
	orrs	r3, r6
	strh	r3, [r2, #0]
	movs	r0, #1
	str	r1, [sp, #4]
	bl	sub_08013560
	ldr	r2, [pc, #120]
	ldr	r1, [sp, #4]
	adds	r5, #1
	subs	r1, #64
	adds	r7, r7, r2
	subs	r6, #2
	cmp	r5, #15
	ble.n	.L_080d5ce0
	b.n	.L_080d5d5c
.L_080d5d04:
	movs	r3, #254
	lsls	r3, r3, #7
	movs	r5, #160
	adds	r3, #255
	lsls	r5, r5, #19
	strh	r3, [r5, #0]
	movs	r0, #132
	lsls	r0, r0, #1
	adds	r0, #255
	movs	r1, #16
	bl	sub_080d0520
	movs	r3, #217
	lsls	r3, r3, #1
	adds	r2, r6, r3
	movs	r3, #0
	strh	r3, [r2, #0]
	movs	r0, #16
	bl	sub_08013560
	movs	r7, #240
	movs	r2, #240
	mov	r8, r5
	lsls	r2, r2, #2
	lsls	r7, r7, #7
	movs	r6, #30
	movs	r5, #15
.L_080d5d3a:
	adds	r3, r7, #0
	orrs	r3, r2
	orrs	r3, r6
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r0, #1
	str	r2, [sp, #0]
	bl	sub_08013560
	ldr	r3, [pc, #28]
	ldr	r2, [sp, #0]
	subs	r5, #1
	subs	r2, #64
	adds	r7, r7, r3
	subs	r6, #2
	cmp	r5, #0
	bge.n	.L_080d5d3a
.L_080d5d5c:
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x050001e6
	.2byte 0xf800
	.2byte 0xffff
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	adds	r6, r1, #0
	bl	sub_080cad84
	movs	r3, #1
	adds	r5, r0, #0
	eors	r3, r6
	ldr	r0, [r5, #80]
	negs	r2, r3
	orrs	r2, r3
	movs	r1, #12
	lsrs	r2, r2, #31
	adds	r1, #255
	movs	r6, #26
	subs	r6, r6, r2
	mov	r8, r0
	bl	sub_08020058
	movs	r1, #0
	mov	r2, r8
	strb	r1, [r2, #26]
	movs	r3, #15
	strb	r3, [r0, #5]
	ldr	r2, [pc, #56]
	ldr	r3, [r5, #8]
	movs	r0, #128
	ands	r3, r2
	lsls	r0, r0, #12
	adds	r3, r3, r0
	str	r3, [r5, #8]
	ldr	r3, [r5, #16]
	str	r1, [r5, #36]
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r5, #16]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r1, [r5, #44]
	adds	r0, r5, #0
	str	r3, [r5, #56]
	str	r3, [r5, #64]
	adds	r1, r6, #0
	bl	sub_08020090
	movs	r0, #18
	bl	sub_08013560
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000
	.2byte 0xfff0
.L_080d5de0:
	.2byte 0xb560
	mov	r6, r8
	push	{r6}
	ldr	r3, [pc, #96]
	mov	r8, r0
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl	sub_080cad84
	adds	r5, r0, #0
	ldr	r6, [r5, #80]
	movs	r1, #12
	adds	r1, #255
	adds	r0, r6, #0
	bl	sub_08020058
	movs	r1, #0
	strb	r1, [r6, #26]
	movs	r3, #15
	strb	r3, [r0, #5]
	ldr	r2, [pc, #60]
	ldr	r3, [r5, #8]
	movs	r0, #128
	ands	r3, r2
	lsls	r0, r0, #12
	adds	r3, r3, r0
	str	r3, [r5, #8]
	ldr	r3, [r5, #16]
	str	r1, [r5, #36]
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r5, #16]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r1, [r5, #44]
	adds	r0, r5, #0
	str	r3, [r5, #56]
	str	r3, [r5, #64]
	mov	r1, r8
	bl	sub_08020090
	movs	r0, #18
	bl	sub_08013560
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff0
	.2byte 0xb500
	movs	r0, #26
	bl	.L_080d5de0
	movs	r0, #144
	lsls	r0, r0, #1
	bl	sub_08016cfc
	pop	{pc}

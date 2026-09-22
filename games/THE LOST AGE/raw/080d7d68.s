.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_0800205c, 0x0800205c
	.set sub_08013560, 0x08013560
	.set sub_08014878, 0x08014878
	.set sub_0801489c, 0x0801489c
	.set sub_08020090, 0x08020090
	.set sub_080200a8, 0x080200a8
	.set sub_080200c8, 0x080200c8
	.set sub_08020228, 0x08020228
	.set sub_08020278, 0x08020278
	.set sub_08020280, 0x08020280
	.set sub_080d2d84, 0x080d2d84
	.set sub_080d32c8, 0x080d32c8
	.set sub_080d332c, 0x080d332c
	.set sub_080d3838, 0x080d3838
	.set sub_080d7a78, 0x080d7a78
	.set sub_080d7ab4, 0x080d7ab4
	.set sub_080db974, 0x080db974
	.set sub_080dc10c, 0x080dc10c
	.set sub_080dc390, 0x080dc390
	.set sub_080ebe70, 0x080ebe70
	.set sub_080ebea8, 0x080ebea8
	.set sub_080ebeb4, 0x080ebeb4
	.set sub_080ebec8, 0x080ebec8
	.set sub_080ebf68, 0x080ebf68
	.set sub_08108058, 0x08108058
	.set sub_08108060, 0x08108060
	.set sub_081c0010, 0x081c0010
	.global Overlay_080d7d68
Overlay_080d7d68:
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
	bcs.n	.L_080d7d8e
	ldrh	r3, [r1, #0]
	adds	r3, #1
	strh	r3, [r1, #0]
.L_080d7d8e:
	pop	{pc}
	push	{lr}
	ldr	r1, [pc, #36]
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
	movs	r3, #192
	lsls	r3, r3, #6
	str	r2, [r0, #24]
	cmp	r2, r3
	bge.n	.L_080d7db6
	bl	sub_080200c8
.L_080d7db6:
	pop	{pc}
	.2byte 0xfe40
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #432]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	adds	r7, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl	sub_080d2d84
	movs	r2, #64
	adds	r2, r2, r7
	movs	r6, #0
	ldrsb	r6, [r2, r6]
	mov	r9, r0
	mov	sl, r2
	cmp	r6, #0
	bne.n	.L_080d7e76
	ldr	r2, [r7, #20]
	ldr	r3, [r7, #24]
	str	r2, [r7, #4]
	str	r3, [r7, #8]
	mov	r8, sp
	str	r2, [sp, #0]
	str	r3, [sp, #8]
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	lsls	r1, r5, #1
	lsls	r3, r0, #1
	adds	r3, r3, r0
	adds	r1, r1, r5
	lsls	r1, r1, #11
	lsls	r3, r3, #11
	lsrs	r3, r3, #16
	lsrs	r1, r1, #16
	subs	r1, r1, r3
	movs	r3, #192
	lsls	r3, r3, #8
	movs	r0, #240
	adds	r1, r1, r3
	lsls	r0, r0, #15
	mov	r2, r8
	bl	sub_0801489c
	mov	r1, r8
	ldr	r3, [r1, #0]
	mov	r2, sl
	str	r3, [r7, #12]
	ldr	r3, [r1, #8]
	mov	r1, r9
	str	r3, [r7, #16]
	movs	r3, #160
	lsls	r3, r3, #11
	str	r3, [r7, #36]
	str	r3, [r7, #32]
	adds	r3, r7, #0
	adds	r3, #66
	strb	r6, [r3, #0]
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	ldr	r3, [r1, #80]
	ldr	r0, [r7, #0]
	ldrb	r3, [r3, #9]
	ldrb	r1, [r0, #9]
	movs	r2, #12
	ands	r2, r3
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r0, #9]
	adds	r3, r7, #0
	adds	r3, #71
	strb	r6, [r3, #0]
	strh	r6, [r7, #56]
	ldr	r3, [pc, #280]
	movs	r2, #1
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d7f6a
	movs	r0, #134
	bl	sub_081c0010
	b.n	.L_080d7f6a
.L_080d7e76:
	cmp	r6, #1
	bne.n	.L_080d7e98
	movs	r2, #56
	ldrsh	r3, [r7, r2]
	cmp	r3, #3
	bne.n	.L_080d7f42
	ldr	r1, [r7, #0]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	adds	r2, r7, #0
	strb	r3, [r1, #9]
	adds	r2, #71
	movs	r3, #4
	strb	r3, [r2, #0]
	b.n	.L_080d7f42
.L_080d7e98:
	cmp	r6, #2
	bne.n	.L_080d7ed6
	adds	r0, r7, #0
	bl	sub_080ebe70
	cmp	r0, #0
	bne.n	.L_080d7f6a
	ldr	r3, [r7, #4]
	ldr	r1, [r7, #0]
	str	r3, [r7, #20]
	ldr	r3, [r7, #8]
	str	r3, [r7, #24]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	adds	r2, r7, #0
	adds	r2, #71
	strb	r3, [r1, #9]
	movs	r3, #4
	strb	r3, [r2, #0]
	adds	r3, r7, #0
	adds	r3, #68
	strb	r0, [r3, #0]
	mov	r2, sl
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	movs	r3, #40
	strh	r3, [r7, #58]
	b.n	.L_080d7f6a
.L_080d7ed6:
	cmp	r6, #3
	bne.n	.L_080d7f3e
	movs	r1, #1
	mov	r8, r1
	adds	r3, r7, #0
	adds	r3, #68
	mov	r2, r8
	strb	r2, [r3, #0]
	ldr	r3, [r7, #20]
	mov	r1, r9
	str	r3, [r7, #4]
	ldr	r3, [r7, #24]
	mov	r5, sp
	str	r3, [r7, #8]
	movs	r2, #160
	ldr	r3, [r1, #8]
	lsls	r2, r2, #13
	str	r3, [r5, #0]
	adds	r0, r5, #0
	ldr	r3, [r1, #12]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r1, #16]
	str	r3, [r5, #8]
	bl	sub_080dc390
	bl	sub_08014878
	adds	r1, r0, #0
	movs	r0, #128
	adds	r2, r5, #0
	lsls	r0, r0, #11
	bl	sub_0801489c
	ldr	r3, [r5, #0]
	mov	r1, sl
	str	r3, [r7, #12]
	mov	r2, r8
	ldr	r3, [r5, #8]
	str	r3, [r7, #16]
	ldrb	r3, [r1, #0]
	adds	r3, #1
	strb	r3, [r1, #0]
	ldr	r3, [pc, #76]
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d7f6a
	movs	r0, #145
	bl	sub_081c0010
	b.n	.L_080d7f6a
.L_080d7f3e:
	cmp	r6, #4
	bne.n	.L_080d7f56
.L_080d7f42:
	adds	r0, r7, #0
	bl	sub_080ebe70
	cmp	r0, #0
	bne.n	.L_080d7f6a
	mov	r1, sl
	ldrb	r3, [r1, #0]
	subs	r3, #1
	strb	r3, [r1, #0]
	b.n	.L_080d7f6a
.L_080d7f56:
	cmp	r6, #5
	bne.n	.L_080d7f6a
	adds	r0, r7, #0
	bl	sub_080ebe70
	cmp	r0, #0
	bne.n	.L_080d7f6a
	adds	r0, r7, #0
	bl	sub_080ebf68
.L_080d7f6a:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02000240
	.2byte 0x1100
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #12
	adds	r6, r0, #0
	bl	sub_080d2d84
	ldr	r3, [pc, #412]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	bl	sub_080d2d84
	mov	r8, r0
	cmp	r5, #0
	bne.n	.L_080d7fa8
	b.n	.L_080d8126
.L_080d7fa8:
	bl	sub_080d7a78
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #380]
	mov	sl, r3
	bl	sub_08108058
	movs	r0, #30
	bl	sub_08013560
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	adds	r0, r6, #0
	bl	sub_080d3838
	movs	r0, #20
	bl	sub_08013560
	movs	r0, #173
	bl	sub_081c0010
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_080d332c
	movs	r0, #174
	bl	sub_081c0010
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_080d332c
	movs	r0, #175
	bl	sub_081c0010
	movs	r1, #1
	adds	r0, r6, #0
	bl	sub_080d332c
	movs	r0, #20
	bl	sub_08013560
	movs	r0, #140
	bl	sub_081c0010
	ldr	r3, [pc, #300]
	adds	r2, r5, #0
	str	r3, [r5, #108]
	adds	r2, #100
	movs	r3, #0
	strh	r3, [r2, #0]
	movs	r0, #80
	bl	sub_08013560
	ldr	r3, [pc, #284]
	adds	r0, r5, #0
	str	r3, [r5, #108]
	movs	r1, #3
	bl	sub_08020090
	ldr	r3, [r5, #8]
	mov	r7, sp
	str	r3, [r7, #0]
	adds	r0, r7, #0
	ldr	r3, [r5, #12]
	movs	r6, #23
	str	r3, [r7, #4]
	ldr	r3, [r5, #16]
	mov	r5, sl
	str	r3, [r7, #8]
	bl	sub_080dc390
	adds	r5, #80
.L_080d8042:
	movs	r1, #168
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #8]
	adds	r0, r5, #0
	lsls	r1, r1, #2
	bl	sub_080ebec8
	adds	r0, r5, #0
	ldr	r1, [pc, #236]
	bl	sub_080ebeb4
	adds	r0, r5, #0
	movs	r1, #7
	bl	sub_080ebea8
	ldr	r0, [r5, #0]
	movs	r1, #10
	bl	sub_08020280
	bl	sub_08014878
	movs	r1, #3
	bl	sub_0800205c
	movs	r3, #128
	lsls	r3, r3, #9
	adds	r0, r0, r3
	str	r0, [r5, #44]
	str	r0, [r5, #40]
	subs	r6, #1
	movs	r0, #1
	bl	sub_08013560
	adds	r5, #72
	cmp	r6, #0
	bge.n	.L_080d8042
	movs	r0, #60
	bl	sub_08013560
	ldr	r5, [pc, #156]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r5, r5, r2
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	ldr	r0, [r5, #0]
	bl	sub_080d3838
	movs	r0, #20
	bl	sub_08013560
	ldr	r0, [r5, #0]
	bl	sub_080d2d84
	movs	r1, #28
	bl	sub_08020090
	movs	r0, #20
	bl	sub_08013560
	mov	r2, sl
	movs	r1, #2
	adds	r2, #144
	movs	r6, #23
.L_080d80c4:
	movs	r3, #5
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L_080d80ce
	strb	r1, [r2, #0]
.L_080d80ce:
	subs	r6, #1
	adds	r2, #72
	cmp	r6, #0
	bge.n	.L_080d80c4
	movs	r0, #60
	bl	sub_08013560
	ldr	r3, [pc, #100]
	mov	r2, r8
	str	r3, [r2, #108]
	movs	r0, #100
	bl	sub_08013560
	mov	r2, sl
	movs	r1, #5
	adds	r2, #144
	movs	r6, #23
.L_080d80f0:
	movs	r3, #5
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L_080d80fa
	strb	r1, [r2, #0]
.L_080d80fa:
	subs	r6, #1
	adds	r2, #72
	cmp	r6, #0
	bge.n	.L_080d80f0
	movs	r0, #10
	bl	sub_08013560
	movs	r5, #0
	mov	r3, r8
	str	r5, [r3, #108]
	movs	r3, #128
	lsls	r3, r3, #9
	mov	r2, r8
	str	r3, [r2, #24]
	str	r3, [r2, #28]
	movs	r0, #30
	bl	sub_08013560
	bl	sub_08108060
	bl	sub_080d7ab4
.L_080d8126:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x00201090
	.4byte 0x080d7d69
	.4byte 0x080d7d91
	.4byte 0x080d7dbd
	.2byte 0x7d49
	.2byte 0x080d
	push	{lr}
	ldr	r1, [pc, #36]
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
	movs	r3, #192
	lsls	r3, r3, #6
	str	r2, [r0, #24]
	cmp	r2, r3
	bge.n	.L_080d816e
	bl	sub_080200c8
.L_080d816e:
	pop	{pc}
	.2byte 0xfc00
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #108]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	adds	r7, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl	sub_080d2d84
	adds	r3, r7, #0
	adds	r3, #100
	ldrh	r1, [r3, #0]
	adds	r6, r0, #0
	subs	r1, #1
	strh	r1, [r3, #0]
	mov	r5, sp
	ldr	r3, [r6, #8]
	lsls	r1, r1, #16
	str	r3, [r5, #0]
	asrs	r1, r1, #16
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	movs	r3, #204
	lsls	r3, r3, #7
	adds	r3, #102
	adds	r0, r1, #0
	muls	r0, r3
	adds	r3, r7, #0
	adds	r3, #102
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	lsls	r1, r1, #11
	adds	r1, r1, r3
	adds	r2, r5, #0
	bl	sub_0801489c
	ldr	r3, [r5, #0]
	ldr	r2, [r7, #12]
	str	r3, [r7, #8]
	movs	r1, #160
	ldr	r3, [r5, #8]
	lsls	r1, r1, #13
	str	r3, [r7, #16]
	ldr	r3, [pc, #24]
	adds	r2, r2, r3
	str	r2, [r7, #12]
	ldr	r3, [r6, #12]
	adds	r3, r3, r1
	cmp	r2, r3
	bge.n	.L_080d81e0
	adds	r0, r7, #0
	bl	sub_080200c8
.L_080d81e0:
	add	sp, #12
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #224]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r7, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl	sub_080d2d84
	movs	r3, #64
	adds	r3, r3, r7
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	mov	sl, r3
	mov	r8, r2
	cmp	r2, #0
	bne.n	.L_080d829e
	ldr	r3, [r0, #8]
	mov	r6, sp
	str	r3, [r6, #0]
	ldr	r3, [r0, #12]
	str	r3, [r6, #4]
	ldr	r3, [r0, #16]
	str	r3, [r6, #8]
	bl	sub_08014878
	lsls	r5, r0, #2
	adds	r5, r5, r0
	movs	r3, #160
	lsls	r3, r3, #12
	lsls	r5, r5, #1
	adds	r5, r5, r3
	bl	sub_08014878
	adds	r2, r6, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	adds	r0, r6, #0
	bl	sub_080dc390
	ldr	r2, [r6, #0]
	movs	r0, #240
	str	r2, [r7, #20]
	movs	r1, #192
	ldr	r3, [r6, #8]
	lsls	r0, r0, #15
	str	r3, [r7, #24]
	str	r2, [r7, #4]
	str	r3, [r7, #8]
	lsls	r1, r1, #8
	str	r2, [r6, #0]
	adds	r2, r6, #0
	str	r3, [r6, #8]
	bl	sub_0801489c
	ldr	r3, [r6, #0]
	mov	r2, r8
	str	r3, [r7, #12]
	ldr	r3, [r6, #8]
	str	r3, [r7, #16]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #36]
	movs	r3, #160
	lsls	r3, r3, #11
	str	r3, [r7, #32]
	adds	r3, r7, #0
	adds	r3, #66
	strb	r2, [r3, #0]
	mov	r2, sl
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	ldr	r3, [pc, #80]
	movs	r2, #1
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d82ce
	movs	r0, #144
	bl	sub_081c0010
	b.n	.L_080d82ce
.L_080d829e:
	mov	r3, r8
	cmp	r3, #1
	bne.n	.L_080d82b8
	adds	r0, r7, #0
	bl	sub_080ebe70
	cmp	r0, #0
	bne.n	.L_080d82ce
	mov	r2, sl
	ldrb	r3, [r2, #0]
	subs	r3, #1
	strb	r3, [r2, #0]
	b.n	.L_080d82ce
.L_080d82b8:
	mov	r3, r8
	cmp	r3, #2
	bne.n	.L_080d82ce
	adds	r0, r7, #0
	bl	sub_080ebe70
	cmp	r0, #0
	bne.n	.L_080d82ce
	adds	r0, r7, #0
	bl	sub_080ebf68
.L_080d82ce:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0x1100
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #380]
	adds	r5, r0, #0
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r7, r3, r0
	ldr	r0, [r7, #0]
	sub	sp, #12
	bl	sub_080d2d84
	mov	sl, r0
	adds	r0, r5, #0
	bl	sub_080d2d84
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_080d830c
	b.n	.L_080d85a2
.L_080d830c:
	bl	sub_080d7a78
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #336]
	mov	r9, r3
	bl	sub_08108058
	movs	r0, #30
	bl	sub_08013560
	adds	r2, r6, #0
	movs	r3, #0
	adds	r2, #91
	strb	r3, [r2, #0]
	movs	r0, #152
	bl	sub_081c0010
	adds	r0, r5, #0
	movs	r1, #4
	movs	r2, #15
	bl	sub_080d32c8
	movs	r0, #152
	bl	sub_081c0010
	movs	r1, #4
	movs	r2, #15
	adds	r0, r5, #0
	bl	sub_080d32c8
	movs	r0, #30
	bl	sub_08013560
	ldr	r3, [pc, #280]
	movs	r0, #153
	str	r3, [r6, #108]
	bl	sub_081c0010
	adds	r0, r5, #0
	movs	r1, #8
	movs	r2, #22
	bl	sub_080d32c8
	movs	r0, #140
	bl	sub_081c0010
	movs	r1, #166
	lsls	r1, r1, #9
	movs	r5, #128
	adds	r1, #204
	lsls	r5, r5, #9
	adds	r2, r5, #0
	adds	r0, r1, #0
	bl	sub_08020228
	ldr	r3, [pc, #240]
	adds	r0, r6, #0
	str	r3, [r6, #108]
	movs	r1, #3
	bl	sub_08020090
	movs	r0, #90
	bl	sub_08013560
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	ldr	r0, [r7, #0]
	bl	sub_080d3838
	movs	r0, #20
	bl	sub_08013560
	ldr	r0, [r7, #0]
	bl	sub_080d2d84
	movs	r1, #28
	bl	sub_08020090
	movs	r0, #30
	bl	sub_08013560
	ldr	r1, [pc, #192]
	adds	r2, r5, #0
	adds	r0, r1, #0
	bl	sub_08020228
	ldr	r3, [r6, #8]
	mov	r8, sp
	str	r3, [sp, #0]
	mov	r0, r8
	ldr	r3, [r6, #12]
	mov	r5, r9
	str	r3, [sp, #4]
	adds	r5, #80
	ldr	r3, [r6, #16]
	movs	r7, #23
	str	r3, [sp, #8]
	bl	sub_080dc390
	mov	r6, r8
.L_080d83dc:
	movs	r1, #168
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #8]
	adds	r0, r5, #0
	lsls	r1, r1, #2
	bl	sub_080ebec8
	adds	r0, r5, #0
	ldr	r1, [pc, #140]
	bl	sub_080ebeb4
	adds	r0, r5, #0
	movs	r1, #7
	bl	sub_080ebea8
	ldr	r0, [r5, #0]
	movs	r1, #11
	bl	sub_08020280
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r5, #40]
	bl	sub_08014878
	movs	r2, #192
	lsls	r2, r2, #9
	adds	r0, r0, r2
	str	r0, [r5, #44]
	subs	r7, #1
	movs	r0, #1
	bl	sub_08013560
	adds	r5, #72
	cmp	r7, #0
	bge.n	.L_080d83dc
	movs	r0, #140
	bl	sub_08013560
	mov	r2, r9
	movs	r1, #2
	adds	r2, #144
	movs	r7, #23
.L_080d8430:
	movs	r3, #5
	ldrsb	r3, [r2, r3]
	cmp	r3, #0
	beq.n	.L_080d843a
	strb	r1, [r2, #0]
.L_080d843a:
	subs	r7, #1
	adds	r2, #72
	cmp	r7, #0
	bge.n	.L_080d8430
	movs	r0, #20
	bl	sub_08013560
	movs	r0, #1
	movs	r1, #1
	movs	r2, #1
	bl	sub_08020228
	movs	r0, #30
	bl	sub_08013560
	ldr	r3, [pc, #8]
	movs	r7, #0
	mov	r5, r8
	mov	r9, r3
	b.n	.L_080d8480
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x00201204
	.4byte 0x080d7af9
	.4byte 0x080d8149
	.4byte 0x00019999
	.2byte 0x81ed
	.2byte 0x080d
.L_080d8480:
	mov	r0, sl
	ldr	r1, [r0, #8]
	movs	r3, #240
	str	r1, [r5, #0]
	lsls	r3, r3, #15
	ldr	r2, [r0, #12]
	adds	r2, r2, r3
	str	r2, [r5, #4]
	ldr	r3, [r0, #16]
	movs	r0, #168
	str	r3, [r5, #8]
	lsls	r0, r0, #2
	bl	sub_080dc10c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080d84e8
	bl	sub_08014878
	movs	r1, #3
	bl	sub_0800205c
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r0, r0, r2
	adds	r2, r6, #0
	adds	r2, #100
	movs	r3, #100
	str	r0, [r6, #28]
	str	r0, [r6, #24]
	movs	r1, #24
	strh	r3, [r2, #0]
	lsls	r0, r7, #16
	bl	sub_08002054
	adds	r3, r6, #0
	adds	r3, #102
	strh	r0, [r3, #0]
	ldr	r3, [pc, #224]
	mov	r0, r9
	str	r3, [r6, #108]
	adds	r3, r6, #0
	adds	r3, #85
	strb	r0, [r3, #0]
	movs	r1, #7
	adds	r0, r6, #0
	bl	sub_08020090
	adds	r0, r6, #0
	movs	r1, #11
	bl	sub_08020278
.L_080d84e8:
	adds	r7, #1
	cmp	r7, #23
	ble.n	.L_080d8480
	movs	r0, #100
	bl	sub_08013560
	movs	r0, #149
	lsls	r0, r0, #1
	bl	sub_081c0010
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #151
	bl	sub_081c0010
	mov	r2, sl
	ldr	r3, [r2, #8]
	mov	r0, r8
	str	r3, [r0, #0]
	movs	r7, #0
	ldr	r3, [r2, #12]
	movs	r2, #144
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r0, #4]
	mov	r0, sl
	ldr	r3, [r0, #16]
	mov	r2, r8
	mov	r5, r8
	str	r3, [r2, #8]
	b.n	.L_080d857c
.L_080d8528:
	movs	r3, #153
	lsls	r3, r3, #8
	adds	r3, #153
	adds	r2, r6, #0
	adds	r2, #85
	str	r3, [r6, #28]
	str	r3, [r6, #24]
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #11
	str	r3, [r6, #40]
	ldr	r3, [r6, #12]
	adds	r7, #1
	str	r3, [r6, #20]
	bl	sub_08014878
	movs	r3, #179
	lsls	r3, r3, #9
	adds	r3, #102
	adds	r0, r0, r3
	str	r0, [r6, #48]
	bl	sub_08014878
	movs	r1, #128
	adds	r2, r0, #0
	lsls	r1, r1, #14
	adds	r0, r6, #0
	bl	sub_080db974
	adds	r0, r6, #0
	movs	r1, #11
	bl	sub_08020278
	adds	r2, r6, #0
	adds	r2, #94
	movs	r3, #8
	strh	r3, [r2, #0]
	adds	r0, r6, #0
	ldr	r1, [pc, #60]
	bl	sub_080200a8
.L_080d857c:
	cmp	r7, #7
	bgt.n	.L_080d8594
	movs	r0, #168
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	lsls	r0, r0, #2
	bl	sub_080dc10c
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_080d8528
.L_080d8594:
	movs	r0, #15
	bl	sub_08013560
	bl	sub_08108060
	bl	sub_080d7ab4
.L_080d85a2:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x080d8175
	.2byte 0x0e54
	.2byte 0x080f
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #316]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	adds	r7, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl	sub_080d2d84
	movs	r2, #64
	adds	r2, r2, r7
	movs	r1, #0
	ldrsb	r1, [r2, r1]
	adds	r5, r0, #0
	mov	r8, r2
	ldrb	r3, [r2, #0]
	mov	sl, r1
	cmp	r1, #0
	bne.n	.L_080d866c
	ldr	r3, [r5, #8]
	mov	r6, sp
	str	r3, [r6, #0]
	bl	sub_08014878
	ldr	r3, [r5, #12]
	lsls	r2, r0, #2
	adds	r2, r2, r0
	adds	r3, r3, r2
	movs	r2, #240
	lsls	r2, r2, #12
	adds	r3, r3, r2
	str	r3, [r6, #4]
	adds	r0, r6, #0
	ldr	r3, [r5, #16]
	str	r3, [r6, #8]
	bl	sub_080dc390
	bl	sub_08014878
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r3, #128
	lsls	r3, r3, #10
	lsls	r5, r5, #1
	adds	r5, r5, r3
	bl	sub_08014878
	adds	r2, r6, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	ldr	r2, [r6, #0]
	ldr	r1, [pc, #216]
	str	r2, [r7, #12]
	movs	r5, #192
	ldr	r3, [r6, #8]
	lsls	r5, r5, #10
	str	r3, [r7, #16]
	adds	r3, r3, r1
	str	r2, [r7, #4]
	str	r3, [r7, #8]
	str	r5, [r7, #36]
	bl	sub_08014878
	lsls	r3, r0, #1
	adds	r3, r3, r0
	adds	r3, r3, r5
	str	r3, [r7, #32]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #40]
	str	r3, [r7, #44]
	adds	r3, r7, #0
	adds	r3, #66
	mov	r2, sl
	strb	r2, [r3, #0]
	adds	r2, r7, #0
	movs	r3, #1
	adds	r2, #65
	strb	r3, [r2, #0]
	mov	r1, r8
	ldrb	r3, [r1, #0]
	adds	r3, #1
	strb	r3, [r1, #0]
	b.n	.L_080d86f6
.L_080d866c:
	subs	r3, #1
	movs	r2, #128
	lsls	r3, r3, #24
	lsls	r2, r2, #17
	cmp	r3, r2
	bhi.n	.L_080d86ea
	adds	r0, r7, #0
	bl	sub_080ebe70
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_080d86f6
	ldr	r3, [r7, #4]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r7, #8]
	str	r3, [r5, #8]
	bl	sub_08014878
	adds	r1, r0, #0
	movs	r0, #192
	adds	r2, r5, #0
	lsls	r0, r0, #12
	bl	sub_0801489c
	ldr	r3, [r5, #0]
	str	r3, [r7, #12]
	ldr	r3, [r5, #8]
	str	r3, [r7, #16]
	adds	r3, r7, #0
	adds	r3, #65
	strb	r6, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #28]
	str	r6, [r7, #36]
	bl	sub_08014878
	ldr	r3, [pc, #76]
	adds	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #8
	str	r0, [r7, #32]
	str	r3, [r7, #40]
	str	r3, [r7, #44]
	movs	r0, #143
	bl	sub_081c0010
	mov	r1, r8
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	ldrb	r2, [r1, #0]
	cmp	r3, #1
	bne.n	.L_080d86de
	subs	r3, r2, #1
	strb	r3, [r1, #0]
	b.n	.L_080d86e4
.L_080d86de:
	adds	r3, r2, #1
	mov	r2, r8
	strb	r3, [r2, #0]
.L_080d86e4:
	movs	r3, #6
	strh	r3, [r7, #58]
	b.n	.L_080d86f6
.L_080d86ea:
	mov	r3, sl
	cmp	r3, #3
	bne.n	.L_080d86f6
	adds	r0, r7, #0
	bl	sub_080ebf68
.L_080d86f6:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0xff9c0000
	.4byte 0x00023333

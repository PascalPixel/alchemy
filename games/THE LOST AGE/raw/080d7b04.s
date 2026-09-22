.syntax unified
	.thumb
	.set sub_0800205c, 0x0800205c
	.set sub_08013560, 0x08013560
	.set sub_08014878, 0x08014878
	.set sub_0801489c, 0x0801489c
	.set sub_08020090, 0x08020090
	.set sub_080200c8, 0x080200c8
	.set sub_08020280, 0x08020280
	.set sub_080d2d84, 0x080d2d84
	.set sub_080d32c8, 0x080d32c8
	.set sub_080d332c, 0x080d332c
	.set sub_080d3838, 0x080d3838
	.set sub_080d7a78, 0x080d7a78
	.set sub_080d7ab4, 0x080d7ab4
	.set sub_080dc390, 0x080dc390
	.set sub_080ebea8, 0x080ebea8
	.set sub_080ebeb4, 0x080ebeb4
	.set sub_080ebec8, 0x080ebec8
	.set sub_080ebf68, 0x080ebf68
	.set sub_08108058, 0x08108058
	.set sub_08108060, 0x08108060
	.set sub_081c0010, 0x081c0010
	.global Overlay_080d7b04
Overlay_080d7b04:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #248]
	movs	r1, #133
	adds	r5, r0, #0
	lsls	r1, r1, #2
	adds	r3, r3, r1
	adds	r7, r5, #0
	ldr	r0, [r3, #0]
	adds	r7, #64
	sub	sp, #12
	bl	sub_080d2d84
	movs	r2, #0
	ldrsb	r2, [r7, r2]
	cmp	r2, #0
	bne.n	.L_080d7b3c
	ldrh	r3, [r5, #60]
	adds	r3, #1
	strh	r3, [r5, #60]
	ldrh	r3, [r5, #62]
	adds	r3, #1
	strh	r3, [r5, #62]
	movs	r1, #56
	ldrsh	r3, [r5, r1]
	cmp	r3, #60
	bne.n	.L_080d7bd8
	strh	r2, [r5, #56]
	b.n	.L_080d7bc4
.L_080d7b3c:
	cmp	r2, #1
	bne.n	.L_080d7b50
	ldrh	r3, [r5, #62]
	adds	r3, #1
	strh	r3, [r5, #62]
	movs	r2, #56
	ldrsh	r3, [r5, r2]
	cmp	r3, #40
	bne.n	.L_080d7bd8
	b.n	.L_080d7bc0
.L_080d7b50:
	cmp	r2, #2
	bne.n	.L_080d7ba8
	ldrh	r3, [r5, #62]
	mov	r6, sp
	adds	r3, #1
	strh	r3, [r5, #62]
	movs	r1, #160
	ldr	r3, [r0, #8]
	lsls	r1, r1, #13
	str	r3, [r6, #0]
	ldr	r3, [r0, #12]
	adds	r3, r3, r1
	str	r3, [r6, #4]
	ldr	r3, [r0, #16]
	adds	r0, r6, #0
	str	r3, [r6, #8]
	bl	sub_080dc390
	ldr	r3, [r6, #0]
	ldr	r2, [r5, #20]
	subs	r3, r3, r2
	cmp	r3, #0
	bge.n	.L_080d7b80
	adds	r3, #7
.L_080d7b80:
	asrs	r3, r3, #3
	adds	r3, r2, r3
	str	r3, [r5, #20]
	ldr	r2, [r5, #24]
	ldr	r3, [r6, #8]
	subs	r3, r3, r2
	cmp	r3, #0
	bge.n	.L_080d7b92
	adds	r3, #7
.L_080d7b92:
	asrs	r3, r3, #3
	adds	r3, r2, r3
	str	r3, [r5, #24]
	movs	r2, #56
	ldrsh	r3, [r5, r2]
	cmp	r3, #40
	bne.n	.L_080d7bda
	movs	r3, #0
	strh	r3, [r5, #56]
	ldrb	r3, [r7, #0]
	b.n	.L_080d7bc8
.L_080d7ba8:
	cmp	r2, #3
	bne.n	.L_080d7bce
	ldrh	r3, [r5, #60]
	subs	r3, #1
	strh	r3, [r5, #60]
	ldrh	r3, [r5, #62]
	adds	r3, #1
	strh	r3, [r5, #62]
	movs	r1, #56
	ldrsh	r3, [r5, r1]
	cmp	r3, #60
	bne.n	.L_080d7bd8
.L_080d7bc0:
	movs	r3, #0
	strh	r3, [r5, #56]
.L_080d7bc4:
	ldrb	r3, [r7, #0]
	mov	r6, sp
.L_080d7bc8:
	adds	r3, #1
	strb	r3, [r7, #0]
	b.n	.L_080d7bda
.L_080d7bce:
	cmp	r2, #4
	bne.n	.L_080d7bd8
	adds	r0, r5, #0
	bl	sub_080ebf68
.L_080d7bd8:
	mov	r6, sp
.L_080d7bda:
	ldr	r3, [r5, #20]
	str	r3, [r6, #0]
	ldr	r3, [r5, #24]
	str	r3, [r6, #8]
	movs	r2, #60
	ldrsh	r0, [r5, r2]
	movs	r3, #62
	ldrsh	r1, [r5, r3]
	lsls	r0, r0, #16
	lsls	r1, r1, #11
	adds	r2, r6, #0
	bl	sub_0801489c
	ldr	r3, [r6, #0]
	add	sp, #12
	str	r3, [r5, #4]
	ldr	r3, [r6, #8]
	str	r3, [r5, #8]
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	sub	sp, #12
	adds	r6, r0, #0
	bl	sub_080d2d84
	adds	r7, r0, #0
	cmp	r7, #0
	bne.n	.L_080d7c16
	b.n	.L_080d7d32
.L_080d7c16:
	bl	sub_080d7a78
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r0, [pc, #276]
	ldr	r5, [r3, #0]
	bl	sub_08108058
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
	ldr	r3, [pc, #220]
	movs	r0, #40
	str	r3, [r7, #108]
	bl	sub_08013560
	movs	r0, #153
	bl	sub_081c0010
	movs	r1, #12
	movs	r2, #22
	adds	r0, r6, #0
	bl	sub_080d32c8
	ldr	r3, [r7, #8]
	mov	r6, sp
	str	r3, [r6, #0]
	ldr	r3, [r7, #12]
	adds	r0, r6, #0
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	adds	r5, #80
	str	r3, [r6, #8]
	bl	sub_080dc390
	adds	r0, r7, #0
	bl	sub_080200c8
	movs	r0, #164
	bl	sub_081c0010
	movs	r7, #23
.L_080d7c9c:
	movs	r1, #168
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #8]
	adds	r0, r5, #0
	lsls	r1, r1, #2
	bl	sub_080ebec8
	adds	r0, r5, #0
	ldr	r1, [pc, #144]
	bl	sub_080ebeb4
	adds	r0, r5, #0
	movs	r1, #7
	bl	sub_080ebea8
	bl	sub_08014878
	lsls	r1, r0, #3
	subs	r1, r1, r0
	lsrs	r1, r1, #16
	ldr	r0, [r5, #0]
	bl	sub_08020280
	bl	sub_08014878
	movs	r1, #3
	bl	sub_0800205c
	movs	r3, #128
	lsls	r3, r3, #9
	adds	r0, r0, r3
	str	r0, [r5, #44]
	str	r0, [r5, #40]
	subs	r7, #1
	movs	r0, #1
	bl	sub_08013560
	adds	r5, #72
	cmp	r7, #0
	bge.n	.L_080d7c9c
	movs	r0, #60
	bl	sub_08013560
	ldr	r5, [pc, #80]
	movs	r3, #133
	lsls	r3, r3, #2
	adds	r5, r5, r3
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
	movs	r0, #40
	bl	sub_08013560
	movs	r0, #164
	bl	sub_081c0010
	movs	r0, #100
	bl	sub_08013560
	bl	sub_08108060
	bl	sub_080d7ab4
.L_080d7d32:
	add	sp, #12
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0020118c
	.4byte 0x080d7af9
	.4byte 0x080d7b05
	.4byte 0x02000240

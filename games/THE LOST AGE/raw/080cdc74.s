.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_0801489c, 0x0801489c
	.set sub_08020090, 0x08020090
	.set sub_080200e8, 0x080200e8
	.set sub_08020148, 0x08020148
	.set sub_08020150, 0x08020150
	.set sub_08020210, 0x08020210
	.set sub_080d22a8, 0x080d22a8
	.set sub_080d2350, 0x080d2350
	.set sub_080d2c98, 0x080d2c98
	.set sub_080d2d84, 0x080d2d84
	.set sub_080d3be8, 0x080d3be8
	.set sub_080d407c, 0x080d407c
	.set sub_080eaf28, 0x080eaf28
	.set sub_081c0010, 0x081c0010
	.global Overlay_080cdc74
Overlay_080cdc74:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #252]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	sub	sp, #24
	bl	sub_080d2d84
	ldrh	r3, [r0, #6]
	movs	r2, #12
	lsrs	r5, r3, #12
	adds	r3, r5, #2
	ands	r3, r2
	lsls	r5, r3, #12
	add	r3, sp, #12
	mov	r8, r3
	ldr	r3, [r0, #8]
	mov	r2, r8
	str	r3, [r2, #0]
	movs	r2, #128
	ldr	r3, [r0, #12]
	lsls	r2, r2, #11
	adds	r3, r3, r2
	ldr	r2, [pc, #208]
	adds	r1, r5, #0
	ands	r3, r2
	mov	r2, r8
	str	r3, [r2, #4]
	ldr	r3, [r0, #16]
	movs	r0, #128
	str	r3, [r2, #8]
	lsls	r0, r0, #13
	bl	sub_0801489c
	mov	r0, r8
	movs	r1, #1
	bl	sub_080eaf28
	adds	r7, r0, #0
	movs	r0, #0
	cmp	r7, #0
	beq.n	.L_080cdd70
	adds	r3, r7, #0
	adds	r3, #98
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080cdd70
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #106
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r0, [r3, r2]
	cmp	r0, #1
	beq.n	.L_080cdcf0
	bl	sub_080d2d84
	cmp	r7, r0
	beq.n	.L_080cdd6a
.L_080cdcf0:
	ldr	r3, [r7, #8]
	mov	r2, r8
	str	r3, [r2, #0]
	ldr	r3, [r7, #12]
	movs	r0, #128
	str	r3, [r2, #4]
	ldr	r3, [r7, #16]
	lsls	r0, r0, #13
	adds	r1, r5, #0
	str	r3, [r2, #8]
	bl	sub_0801489c
	mov	r0, r8
	movs	r1, #1
	bl	sub_080eaf28
	cmp	r0, #0
	beq.n	.L_080cdd24
	adds	r3, r0, #0
	adds	r3, #89
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L_080cdd70
.L_080cdd24:
	ldr	r3, [r7, #8]
	mov	r0, sp
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	movs	r1, #1
	str	r3, [r0, #8]
	bl	sub_080eaf28
	cmp	r0, #0
	beq.n	.L_080cdd52
	adds	r3, r0, #0
	adds	r3, #89
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L_080cdd70
.L_080cdd52:
	adds	r6, r7, #0
	adds	r6, #34
	movs	r3, #2
	ldrb	r5, [r6, #0]
	adds	r0, r7, #0
	strb	r3, [r6, #0]
	mov	r1, r8
	bl	sub_08020210
	strb	r5, [r6, #0]
	cmp	r0, #0
	ble.n	.L_080cdd6e
.L_080cdd6a:
	movs	r0, #0
	b.n	.L_080cdd70
.L_080cdd6e:
	movs	r0, #1
.L_080cdd70:
	add	sp, #24
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #276]
	movs	r0, #133
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl	sub_080d2d84
	adds	r5, r0, #0
	ldrh	r3, [r5, #6]
	movs	r2, #12
	lsrs	r6, r3, #12
	adds	r3, r6, #2
	ands	r3, r2
	lsls	r6, r3, #12
	ldr	r3, [r5, #8]
	mov	r7, sp
	str	r3, [r7, #0]
	movs	r2, #128
	ldr	r3, [r5, #12]
	lsls	r2, r2, #11
	adds	r3, r3, r2
	ldr	r2, [pc, #236]
	adds	r1, r6, #0
	ands	r3, r2
	str	r3, [r7, #4]
	adds	r2, r7, #0
	ldr	r3, [r5, #16]
	str	r3, [r7, #8]
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r8, r3
	mov	r0, r8
	bl	sub_0801489c
	movs	r1, #1
	adds	r0, r7, #0
	bl	sub_080eaf28
	ldr	r3, [r0, #8]
	mov	sl, r0
	str	r3, [r7, #0]
	adds	r1, r6, #0
	ldr	r3, [r0, #12]
	mov	r6, sl
	str	r3, [r7, #4]
	adds	r2, r7, #0
	ldr	r3, [r0, #16]
	adds	r6, #34
	mov	r0, r8
	str	r3, [r7, #8]
	bl	sub_0801489c
	ldrb	r0, [r6, #0]
	movs	r3, #2
	mov	r8, r0
	strb	r3, [r6, #0]
	mov	r0, sl
	adds	r1, r7, #0
	bl	sub_08020210
	mov	r2, r8
	strb	r2, [r6, #0]
	cmp	r0, #0
	bgt.n	.L_080cde94
	movs	r1, #8
	adds	r0, r5, #0
	bl	sub_08020090
	movs	r0, #15
	bl	sub_08013560
	movs	r0, #185
	bl	sub_081c0010
	movs	r3, #204
	lsls	r3, r3, #6
	adds	r3, #51
	mov	r0, sl
	str	r3, [r0, #48]
	str	r3, [r0, #52]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	mov	r9, r3
	ldr	r3, [r7, #8]
	bl	sub_08020148
	movs	r0, #2
	bl	sub_08013560
	ldr	r2, [r5, #52]
	mov	r3, r9
	str	r3, [r5, #52]
	ldr	r6, [r5, #48]
	str	r3, [r5, #48]
	ldr	r1, [r7, #0]
	ldr	r3, [r7, #8]
	mov	r8, r2
	adds	r0, r5, #0
	ldr	r2, [r7, #4]
	bl	sub_08020148
	mov	r0, sl
	bl	sub_08020150
	movs	r0, #1
	bl	sub_08013560
	bl	sub_080d2c98
	mov	r0, sl
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	bl	sub_080200e8
	movs	r0, #10
	ldrsh	r1, [r5, r0]
	movs	r0, #18
	ldrsh	r3, [r5, r0]
	ldr	r2, [r5, #12]
	lsls	r1, r1, #16
	lsls	r3, r3, #16
	adds	r0, r5, #0
	bl	sub_080200e8
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08020090
	mov	r2, r8
	str	r6, [r5, #48]
	str	r2, [r5, #52]
.L_080cde94:
	add	sp, #12
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.2byte 0x0000
	.2byte 0xfff8
	.2byte 0xb560
	adds	r5, r1, #0
	adds	r6, r0, #0
	bl	sub_080d22a8
	adds	r0, r5, #0
	bl	sub_080d3be8
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_080d407c
	bl	sub_080d2350
	pop	{r5, r6, pc}

.syntax unified
	.thumb
	.set sub_0801489c, 0x0801489c
	.set sub_08020090, 0x08020090
	.set sub_08020148, 0x08020148
	.set sub_08020150, 0x08020150
	.set sub_080201d0, 0x080201d0
	.set sub_08020210, 0x08020210
	.set sub_080cad84, 0x080cad84
	.set sub_081c0010, 0x081c0010
	.global Overlay_080d57e4
Overlay_080d57e4:
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
	adds	r6, r0, #0
	ldr	r3, [r6, #80]
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
	ldrh	r5, [r1, r3]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	movs	r0, #0
	cmp	r5, r3
	beq.n	.L_080d590e
	ldr	r2, [r6, #8]
	ldr	r3, [r6, #16]
	ldr	r0, [pc, #240]
	movs	r1, #128
	lsls	r1, r1, #12
	ands	r2, r0
	ands	r3, r0
	mov	r7, sp
	adds	r2, r2, r1
	adds	r3, r3, r1
	str	r3, [r6, #16]
	str	r2, [r6, #8]
	str	r2, [r7, #0]
	movs	r0, #128
	ldr	r3, [r6, #12]
	lsls	r0, r0, #13
	str	r3, [r7, #4]
	adds	r1, r5, #0
	ldr	r3, [r6, #16]
	adds	r2, r7, #0
	str	r3, [r7, #8]
	bl	sub_0801489c
	ldr	r2, [pc, #204]
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #20]
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #8]
	bl	sub_080201d0
	cmp	r0, #0
	bne.n	.L_080d5884
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	sub_08020210
	cmp	r0, #0
	beq.n	.L_080d5888
.L_080d5884:
	movs	r0, #0
	b.n	.L_080d590e
.L_080d5888:
	strh	r5, [r6, #6]
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r3, r6, #0
	adds	r3, #100
	ldrh	r3, [r3, #0]
	adds	r2, r6, #0
	movs	r5, #128
	adds	r2, #85
	lsls	r5, r5, #9
	strb	r3, [r2, #0]
	str	r5, [r6, #24]
	adds	r3, r6, #0
	adds	r3, #102
	ldrh	r3, [r3, #0]
	mov	r2, r8
	strb	r3, [r2, #26]
	mov	r3, r8
	strh	r0, [r3, #18]
	movs	r0, #152
	bl	sub_081c0010
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r6, #48]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r6, #40]
	adds	r0, r6, #0
	movs	r1, #2
	str	r5, [r6, #52]
	bl	sub_08020090
	ldr	r1, [r7, #0]
	ldr	r2, [r7, #4]
	ldr	r3, [r7, #8]
	adds	r0, r6, #0
	bl	sub_08020148
	adds	r0, r6, #0
	bl	sub_08020150
	adds	r3, r6, #0
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
	ldr	r3, [pc, #16]
	adds	r2, #18
	add	r2, sl
	strb	r3, [r2, #0]
	movs	r0, #1
.L_080d590e:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x00000000
	.4byte 0x02000240
	.4byte 0x03001150
	.4byte 0x080f0890
	.4byte 0xfff00000
	.2byte 0x0000
	.2byte 0xfff8
.L_080d5930:
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	movs	r2, #0
	str	r2, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #160
	ldr	r3, [r3, #0]
	mov	sl, r0
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_080d59ca
	adds	r7, r3, #0
	ldr	r2, [pc, #128]
	movs	r3, #128
	lsls	r3, r3, #21
	str	r3, [sp, #0]
	movs	r3, #127
	adds	r7, #236
	mov	fp, r2
	mov	r9, r3
.L_080d5966:
	movs	r3, #16
	ldrsb	r3, [r7, r3]
	cmp	r3, #1
	bne.n	.L_080d59ba
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldr	r3, [r7, #4]
	ldr	r5, [r2, #4]
	subs	r1, r1, r3
	ldr	r3, [r7, #8]
	ldr	r6, [r2, #8]
	subs	r5, r5, r3
	ldr	r3, [r7, #12]
	asrs	r1, r1, #8
	adds	r0, r1, #0
	subs	r6, r6, r3
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x122d
	adds	r1, r5, #0
	mov	r8, r0
	adds	r0, r5, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x1236
	adds	r5, r0, #0
	adds	r1, r6, #0
	adds	r0, r6, #0
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x44a8
	mov	r2, r8
	adds	r3, r2, r0
	movs	r2, #128
	lsls	r2, r2, #3
	cmp	r3, r2
	bgt.n	.L_080d59ba
	ldr	r2, [sp, #0]
	cmp	r2, r3
	ble.n	.L_080d59ba
	str	r3, [sp, #0]
	str	r7, [sp, #4]
.L_080d59ba:
	movs	r3, #1
	negs	r3, r3
	add	r9, r3
	mov	r2, r9
	adds	r7, #32
	cmp	r2, #0
	bge.n	.L_080d5966
	ldr	r0, [sp, #4]
.L_080d59ca:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #176]
	movs	r3, #133
	mov	r8, r2
	lsls	r3, r3, #2
	add	r3, r8
	ldr	r0, [r3, #0]
	bl	sub_080cad84
	adds	r5, r0, #0
	adds	r0, #8
	ldr	r7, [r5, #80]
	bl	.L_080d5930
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #194
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r6, r0, #0
	movs	r2, #0
	strh	r2, [r3, #0]
	movs	r0, #0
	cmp	r6, #0
	beq.n	.L_080d5a98
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
	str	r6, [r5, #104]
	bl	sub_08020090
	ldr	r1, [r6, #4]
	ldr	r2, [r6, #8]
	ldr	r3, [r6, #12]
	adds	r0, r5, #0
	bl	sub_08020148
	adds	r0, r5, #0
	bl	sub_08020150
	adds	r3, r5, #0
	adds	r3, #85
	ldrb	r2, [r3, #0]
	adds	r3, #15
	strh	r2, [r3, #0]
	ldrb	r1, [r7, #9]
	ldrb	r3, [r7, #26]
	adds	r2, r5, #0
	adds	r2, #102
	strh	r3, [r2, #0]
	lsls	r3, r1, #28
	lsrs	r3, r3, #30
	subs	r2, #4
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	ldr	r3, [pc, #28]
	movs	r2, #4
	strb	r3, [r7, #26]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #18
	strb	r3, [r7, #9]
	add	r2, r8
	movs	r3, #4
	strb	r3, [r2, #0]
	movs	r0, #1
	b.n	.L_080d5a98
	.4byte 0x00000000
	.2byte 0x0240
	.2byte 0x0200
.L_080d5a98:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}

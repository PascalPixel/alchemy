.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014d00, 0x08014d00
	.global Overlay_080d45d8
Overlay_080d45d8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	movs	r1, #213
	lsls	r3, r3, #18
	lsls	r1, r1, #4
	movs	r0, #108
	ldr	r7, [r3, #32]
	bl	sub_08014d00
	movs	r1, #230
	lsls	r1, r1, #1
	adds	r0, r0, r1
	ldr	r3, [r0, #0]
	adds	r3, #91
	ldrb	r3, [r3, #0]
	mov	sl, r3
	cmp	r3, #0
	bne.n	.L_080d468e
	movs	r2, #144
	lsls	r2, r2, #4
	adds	r2, #108
	adds	r2, r2, r7
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_080d468e
	movs	r1, #144
	movs	r2, #144
	lsls	r1, r1, #4
	lsls	r2, r2, #4
	adds	r1, #100
	adds	r2, #104
	adds	r6, r7, r1
	adds	r3, r7, r2
	ldr	r2, [r3, #0]
	ldr	r3, [r6, #0]
	subs	r2, r2, r3
	movs	r3, #144
	lsls	r3, r3, #4
	adds	r3, #110
	adds	r5, r7, r3
	ldrh	r3, [r5, #0]
	adds	r3, #1
	strh	r3, [r5, #0]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	adds	r0, r3, #0
	muls	r0, r2
	mov	r3, r8
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	bl	sub_08002054
	ldr	r1, [r6, #0]
	adds	r1, r1, r0
	movs	r0, #144
	lsls	r0, r0, #4
	adds	r0, #92
	adds	r3, r7, r0
	ldr	r0, [r3, #0]
	ldr	r3, [pc, #60]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2196
	lsls	r1, r1, #4
	adds	r3, r7, r1
	str	r0, [r3, #0]
	movs	r0, #142
	lsls	r0, r0, #1
	adds	r3, r7, r0
	ldrh	r3, [r3, #0]
	ldr	r2, [pc, #44]
	adds	r3, #1
	str	r3, [r2, #0]
	movs	r1, #0
	ldrsh	r2, [r5, r1]
	mov	r1, r8
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r2, r3
	bne.n	.L_080d468e
	mov	r2, sl
	mov	r3, r8
	strh	r2, [r3, #0]
	ldr	r0, [pc, #20]
	bl	sub_08014644
.L_080d468e:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0300021c
	.4byte 0x03001144
	.2byte 0x45d9
	.2byte 0x080d
	push	{r5, r6, r7, lr}
	adds	r7, r1, #0
	movs	r3, #192
	movs	r1, #213
	lsls	r3, r3, #18
	lsls	r1, r1, #4
	adds	r6, r0, #0
	movs	r0, #108
	ldr	r5, [r3, #32]
	bl	sub_08014d00
	movs	r1, #197
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080d470a
	movs	r1, #128
	ldr	r3, [pc, #60]
	lsls	r1, r1, #9
	adds	r0, r6, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2290
	movs	r3, #144
	lsls	r2, r2, #4
	lsls	r3, r3, #4
	adds	r2, #100
	adds	r3, #104
	adds	r1, r5, r2
	adds	r2, r5, r3
	ldr	r3, [r2, #0]
	str	r3, [r1, #0]
	movs	r1, #144
	lsls	r1, r1, #4
	adds	r1, #108
	adds	r3, r5, r1
	adds	r1, #2
	str	r0, [r2, #0]
	strh	r7, [r3, #0]
	adds	r3, r5, r1
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #3
	strh	r2, [r3, #0]
	ldr	r0, [pc, #12]
	adds	r1, #148
	bl	sub_080145a8
.L_080d470a:
	pop	{r5, r6, r7, pc}
	.4byte 0x03000230
	.2byte 0x45d9
	.2byte 0x080d
	push	{r5, r6, lr}
	movs	r3, #192
	movs	r1, #213
	lsls	r3, r3, #18
	lsls	r1, r1, #4
	movs	r0, #108
	ldr	r6, [r3, #32]
	bl	sub_08014d00
	movs	r2, #197
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080d4768
	movs	r2, #144
	lsls	r2, r2, #4
	adds	r2, #108
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_080d4768
	movs	r3, #144
	lsls	r3, r3, #4
	adds	r3, #108
	adds	r6, r6, r3
.L_080d4750:
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #44
	adds	r5, #1
	adds	r2, #255
	cmp	r5, r2
	bgt.n	.L_080d4768
	movs	r2, #0
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	bne.n	.L_080d4750
.L_080d4768:
	pop	{r5, r6, pc}

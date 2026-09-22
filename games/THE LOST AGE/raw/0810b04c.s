.syntax unified
	.thumb
	.set sub_08014878, 0x08014878
	.set sub_0801489c, 0x0801489c
	.set sub_08038060, 0x08038060
	.set sub_08038078, 0x08038078
	.set sub_08038120, 0x08038120
	.set sub_080c8590, 0x080c8590
	.set sub_080c8598, 0x080c8598
	.set sub_080c85b8, 0x080c85b8
	.set sub_0810a864, 0x0810a864
	.set sub_0810a898, 0x0810a898
	.set sub_0810a960, 0x0810a960
	.global Overlay_0810b04c
Overlay_0810b04c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r2, #129
	lsls	r2, r2, #3
	adds	r2, #255
	adds	r3, r3, r2
	movs	r5, #0
	ldrsb	r5, [r3, r5]
	adds	r7, r1, #0
	adds	r6, r0, #0
	adds	r1, r5, #0
	adds	r0, r7, #0
	bl	sub_0810a864
	mov	r8, r0
	cmp	r6, #0
	beq.n	.L_0810b0ac
	adds	r0, r6, #0
	bl	sub_08038060
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_0810a898
	cmp	r0, #0
	beq.n	.L_0810b08e
	ldr	r5, [pc, #40]
	b.n	.L_0810b090
.L_0810b08e:
	ldr	r5, [pc, #40]
.L_0810b090:
	adds	r0, r5, #0
	bl	sub_0810a960
	movs	r1, #5
	adds	r5, r0, #0
	mov	r0, r8
	bl	sub_08038120
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08038078
.L_0810b0ac:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x000012dd
	.2byte 0x12de
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	movs	r2, #64
	adds	r2, r2, r6
	movs	r7, #0
	ldrsb	r7, [r2, r7]
	sub	sp, #12
	mov	r8, r2
	cmp	r7, #0
	bne.n	.L_0810b136
	ldr	r3, [r6, #20]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r6, #24]
	str	r3, [r5, #8]
	bl	sub_08014878
	adds	r1, r0, #0
	movs	r0, #160
	lsls	r0, r0, #14
	adds	r2, r5, #0
	bl	sub_0801489c
	ldr	r1, [r5, #0]
	ldr	r2, [r5, #8]
	adds	r0, r6, #0
	bl	sub_080c8598
	ldr	r3, [r6, #20]
	str	r3, [r5, #0]
	ldr	r3, [r6, #24]
	str	r3, [r5, #8]
	bl	sub_08014878
	adds	r1, r0, #0
	movs	r0, #128
	adds	r2, r5, #0
	lsls	r0, r0, #11
	bl	sub_0801489c
	ldr	r3, [r5, #0]
	mov	r2, r8
	str	r3, [r6, #12]
	ldr	r3, [r5, #8]
	str	r3, [r6, #16]
	movs	r3, #128
	lsls	r3, r3, #10
	str	r3, [r6, #32]
	movs	r3, #204
	lsls	r3, r3, #7
	adds	r3, #102
	str	r3, [r6, #36]
	adds	r3, r6, #0
	adds	r3, #66
	strb	r7, [r3, #0]
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	b.n	.L_0810b15e
.L_0810b136:
	cmp	r7, #1
	bne.n	.L_0810b14a
	adds	r0, r6, #0
	bl	sub_080c8590
	cmp	r0, #0
	bne.n	.L_0810b15e
	mov	r3, r8
	strb	r0, [r3, #0]
	b.n	.L_0810b15e
.L_0810b14a:
	cmp	r7, #2
	bne.n	.L_0810b15e
	adds	r0, r6, #0
	bl	sub_080c8590
	cmp	r0, #0
	bne.n	.L_0810b15e
	adds	r0, r6, #0
	bl	sub_080c85b8
.L_0810b15e:
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}

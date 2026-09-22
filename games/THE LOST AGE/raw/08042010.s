.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08014dac, 0x08014dac
	.set sub_0803aae4, 0x0803aae4
	.set sub_0803b094, 0x0803b094
	.set sub_080416cc, 0x080416cc
	.global Overlay_08042010
Overlay_08042010:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r6, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #60]
	mov	sl, r1
	movs	r1, #152
	mov	r9, r2
	lsls	r1, r1, #5
	movs	r2, #0
	mov	r8, r2
	adds	r1, #66
	adds	r5, r7, r1
	mov	r3, r8
	movs	r1, #1
	strh	r3, [r5, #0]
	bl	sub_0803b094
	ldrh	r3, [r5, #0]
	movs	r4, #244
	lsls	r4, r4, #4
	lsls	r3, r3, #1
	adds	r3, r3, r4
	mov	r0, r8
	strh	r0, [r7, r3]
	ldr	r2, [pc, #56]
	ldrh	r3, [r5, #0]
	lsrs	r6, r6, #3
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r5, #0]
	mov	r2, sl
	movs	r1, #14
	ldrsh	r3, [r2, r1]
	movs	r4, #12
	ldrsh	r2, [r2, r4]
	adds	r3, r3, r6
	mov	r0, r9
	lsrs	r1, r0, #3
	adds	r3, #1
	adds	r2, r2, r1
	lsls	r3, r3, #5
	adds	r3, r3, r2
	movs	r2, #160
	adds	r1, r3, #1
	lsls	r2, r2, #2
	cmp	r1, r2
	bcs.n	.L_0804209e
	ldr	r3, [pc, #12]
	movs	r4, #244
	lsls	r1, r1, #1
	lsls	r4, r4, #4
	b.n	.L_0804208c
	movs	r0, r0
	.4byte 0x000001ff
	.2byte 0x2000
	.2byte 0x0600
.L_0804208c:
	adds	r2, r1, r3
	adds	r0, r7, r4
	adds	r1, r7, r1
	movs	r3, #7
	mov	r4, r9
	adds	r1, #8
	ands	r3, r4
	bl	sub_080416cc
.L_0804209e:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	movs	r0, #128
	lsls	r0, r0, #2
	mov	r8, r2
	mov	sl, r3
	adds	r7, r1, #0
	bl	sub_08014dac
	ldrb	r3, [r5, #0]
	adds	r6, r0, #0
	adds	r2, r6, #0
	cmp	r3, #0
	beq.n	.L_080420d8
.L_080420ca:
	ldrb	r3, [r5, #0]
	adds	r5, #1
	strh	r3, [r2, #0]
	adds	r2, #2
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_080420ca
.L_080420d8:
	ldr	r3, [pc, #20]
	adds	r0, r6, #0
	strh	r3, [r2, #0]
	adds	r1, r7, #0
	mov	r2, r8
	mov	r3, sl
	bl	sub_0803aae4
	adds	r0, r6, #0
	bl	sub_08013164
	b.n	.L_080420f4
	.2byte 0x0000
	.2byte 0x0000
.L_080420f4:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	movs	r0, #128
	lsls	r0, r0, #2
	mov	sl, r2
	mov	r8, r3
	adds	r7, r1, #0
	bl	sub_08014dac
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r6, r0, #0
	ldr	r0, [r3, #60]
	ldrb	r3, [r5, #0]
	adds	r2, r6, #0
	cmp	r3, #0
	beq.n	.L_08042132
.L_08042124:
	ldrb	r3, [r5, #0]
	adds	r5, #1
	strh	r3, [r2, #0]
	adds	r2, #2
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_08042124
.L_08042132:
	ldr	r3, [pc, #56]
	mov	r4, r8
	strh	r3, [r2, #0]
	lsrs	r2, r4, #3
	movs	r1, #14
	ldrsh	r3, [r7, r1]
	mov	r4, sl
	adds	r3, r3, r2
	movs	r1, #12
	ldrsh	r2, [r7, r1]
	adds	r3, #1
	lsrs	r1, r4, #3
	adds	r2, r2, r1
	lsls	r3, r3, #5
	adds	r3, r3, r2
	movs	r2, #160
	adds	r1, r3, #1
	lsls	r2, r2, #2
	cmp	r1, r2
	bcs.n	.L_0804217e
	ldr	r3, [pc, #20]
	lsls	r1, r1, #1
	adds	r2, r1, r3
	adds	r1, r0, r1
	movs	r3, #7
	adds	r1, #8
	ands	r3, r4
	adds	r0, r6, #0
	b.n	.L_08042174
	.4byte 0x00000000
	.2byte 0x2000
	.2byte 0x0600
.L_08042174:
	bl	sub_080416cc
	adds	r0, r6, #0
	bl	sub_08013164
.L_0804217e:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}

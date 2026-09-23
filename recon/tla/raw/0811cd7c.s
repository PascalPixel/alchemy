.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08013164, 0x08013164
	.set sub_08014878, 0x08014878
	.set sub_08014dac, 0x08014dac
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_08015128, 0x08015128
	.set sub_08015768, 0x08015768
	.set sub_08016ca4, 0x08016ca4
	.set sub_080381c0, 0x080381c0
	.set sub_080ad008, 0x080ad008
	.set sub_0811a188, 0x0811a188
	.set sub_0811a31c, 0x0811a31c
	.set sub_08126548, 0x08126548
	.global Func_0811cd7c
	.thumb_func
Func_0811cd7c:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #48]
	adds	r3, #176
	ldr	r3, [r3, #0]
	movs	r6, #0
	mov	r8, r3
	movs	r3, #160
	lsls	r3, r3, #11
	str	r3, [r5, #16]
	movs	r3, #128
	mov	r2, r8
	lsls	r3, r3, #7
	str	r6, [r5, #12]
	str	r6, [r5, #20]
	str	r3, [r2, #0]
	strh	r3, [r5, #54]
	movs	r3, #244
	lsls	r3, r3, #8
	strh	r3, [r5, #52]
	ldr	r3, [pc, #136]
	str	r6, [r5, #28]
	str	r3, [r5, #32]
	str	r6, [r5, #24]
	sub	sp, #16
	mov	sl, r0
	bl	sub_08014de4
	adds	r0, r5, #0
	adds	r0, #12
	bl	sub_08015128
	movs	r3, #54
	ldrsh	r0, [r5, r3]
	bl	sub_08015068
	movs	r2, #52
	ldrsh	r0, [r5, r2]
	bl	sub_08015024
	add	r0, sp, #4
	str	r6, [r0, #0]
	str	r6, [r0, #4]
	adds	r1, r5, #0
	ldr	r3, [r5, #32]
	movs	r5, #1
	str	r3, [r0, #8]
	ldr	r3, [pc, #88]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x21c0
	ldr	r3, [pc, #84]
	lsls	r1, r1, #8
	ldr	r0, [pc, #84]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4a14
	adds	r1, r0, #0
	movs	r0, #0
	bl	sub_08015768
	ldr	r2, [pc, #76]
	mov	r3, sl
	adds	r3, #120
	str	r3, [r2, #16]
	movs	r1, #118
	mov	r2, sl
	mov	r3, r8
	subs	r1, r1, r2
	movs	r2, #128
	str	r5, [r3, #16]
	lsls	r2, r2, #10
	movs	r0, #240
	movs	r3, #128
	str	r2, [sp, #0]
	lsls	r0, r0, #15
	lsls	r1, r1, #16
	lsls	r3, r3, #4
	movs	r2, #0
	bl	sub_08126548
	mov	r3, r8
	str	r5, [r3, #20]
	str	r6, [r3, #16]
	add	sp, #16
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x02ee0000
	.4byte 0x03000354
	.4byte 0x03000230
	.4byte 0x03c90000
	.4byte 0x07920000
	.2byte 0x11e0
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	mov	r5, sp
	movs	r0, #3
	adds	r1, r5, #0
	bl	sub_0811a31c
	cmp	r0, #0
	ble.n	.L_0811ce8c
	movs	r6, #0
	adds	r7, r5, #0
	mov	r8, r6
	adds	r5, r0, #0
.L_0811ce6e:
	ldrh	r0, [r6, r7]
	bl	sub_08016ca4
	movs	r2, #44
	adds	r2, #255
	adds	r3, r0, r2
	mov	r2, r8
	ldrh	r0, [r6, r7]
	strb	r2, [r3, #0]
	subs	r5, #1
	bl	sub_080ad008
	adds	r6, #2
	cmp	r5, #0
	bne.n	.L_0811ce6e
.L_0811ce8c:
	add	sp, #28
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #0
	sub	sp, #36
	str	r2, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #36]
	adds	r3, r2, #0
	adds	r3, #69
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_0811cebe
	movs	r3, #1
	str	r3, [sp, #4]
	b.n	.L_0811cf7a
.L_0811cebe:
	adds	r2, #70
	str	r2, [sp, #0]
	movs	r0, #1
	ldrb	r2, [r2, #0]
	movs	r6, #0
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, r3, r2
	movs	r2, #152
	lsls	r2, r2, #5
	lsls	r3, r3, #4
	adds	r2, #136
	adds	r2, r2, r3
	add	r3, sp, #8
	mov	sl, r3
	mov	r1, sl
	mov	r9, r2
	bl	sub_0811a188
	ldr	r2, [sp, #4]
	mov	r8, r0
	cmp	r2, r8
	bge.n	.L_0811cf08
	mov	fp, sl
	movs	r7, #0
	mov	r5, r8
.L_0811cef4:
	mov	r3, fp
	ldrsh	r0, [r7, r3]
	bl	sub_08016ca4
	ldrb	r3, [r0, #15]
	subs	r5, #1
	adds	r6, r6, r3
	adds	r7, #2
	cmp	r5, #0
	bne.n	.L_0811cef4
.L_0811cf08:
	lsls	r0, r6, #5
	subs	r0, r0, r6
	lsls	r0, r0, #2
	adds	r0, r0, r6
	mov	r1, r8
	lsls	r0, r0, #2
	bl	sub_08002054
	mov	r1, sl
	add	r9, r0
	movs	r0, #2
	bl	sub_0811a188
	movs	r6, #0
	mov	r8, r0
	cmp	r6, r8
	bge.n	.L_0811cf42
	movs	r7, #0
	mov	r5, r8
.L_0811cf2e:
	mov	r3, sl
	ldrsh	r0, [r7, r3]
	bl	sub_08016ca4
	ldrb	r3, [r0, #15]
	subs	r5, #1
	adds	r6, r6, r3
	adds	r7, #2
	cmp	r5, #0
	bne.n	.L_0811cf2e
.L_0811cf42:
	lsls	r0, r6, #5
	subs	r0, r0, r6
	lsls	r0, r0, #2
	adds	r0, r0, r6
	lsls	r0, r0, #2
	mov	r1, r8
	bl	sub_08002054
	mov	r3, r9
	subs	r3, r3, r0
	mov	r9, r3
	cmp	r3, #0
	ble.n	.L_0811cf72
	bl	sub_08014878
	movs	r3, #156
	lsls	r3, r3, #6
	adds	r3, #16
	muls	r3, r0
	lsrs	r3, r3, #16
	cmp	r3, r9
	bcs.n	.L_0811cf72
	movs	r2, #1
	str	r2, [sp, #4]
.L_0811cf72:
	ldr	r2, [sp, #0]
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
.L_0811cf7a:
	ldr	r3, [pc, #36]
	movs	r2, #166
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #2
	bne.n	.L_0811cf8e
	movs	r3, #0
	str	r3, [sp, #4]
.L_0811cf8e:
	ldr	r0, [sp, #4]
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	movs	r0, #17
	bl	sub_08014dac
	adds	r6, r0, #0
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #0
	bl	sub_080381c0
	movs	r5, #1
	negs	r5, r5
	cmp	r0, #0
	blt.n	.L_0811cfc4
	adds	r5, r0, #0
.L_0811cfc4:
	adds	r0, r6, #0
	bl	sub_08013164
	adds	r0, r5, #0
	pop	{r5, r6, pc}
	.2byte 0x0000

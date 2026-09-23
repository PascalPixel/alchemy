.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08014de4, 0x08014de4
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_08015128, 0x08015128
	.set sub_080156e8, 0x080156e8
	.set sub_08015768, 0x08015768
	.set sub_08015778, 0x08015778
	.set sub_0811be3c, 0x0811be3c
	.set sub_08126548, 0x08126548
	.global Func_08126700
	.thumb_func
Func_08126700:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #48]
	mov	fp, r2
	mov	r8, r3
	movs	r2, #12
	add	r2, r8
	adds	r5, r0, #0
	mov	r9, r1
	lsls	r0, r4, #16
	movs	r1, #100
	sub	sp, #40
	mov	sl, r2
	bl	sub_08002054
	mov	r3, sl
	mov	r2, r9
	str	r2, [r3, #4]
	mov	r2, fp
	str	r2, [r3, #8]
	ldr	r2, [pc, #192]
	str	r5, [r3, #0]
	movs	r6, #255
	add	r3, sp, #4
	movs	r5, #0
	lsls	r6, r6, #17
	movs	r1, #192
	str	r5, [r3, #0]
	str	r5, [r3, #4]
	str	r5, [r3, #8]
	adds	r7, r0, #0
	mov	fp, r2
	adds	r0, r6, #0
	lsls	r1, r1, #8
	mov	r9, r3
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x0072
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl	sub_08015768
	bl	sub_08014de4
	mov	r0, sl
	bl	sub_08015128
	mov	r2, r8
	movs	r3, #54
	ldrsh	r0, [r2, r3]
	bl	sub_08015068
	mov	r2, r8
	movs	r3, #52
	ldrsh	r0, [r2, r3]
	bl	sub_08015024
	add	r0, sp, #28
	str	r5, [r0, #0]
	str	r5, [r0, #4]
	mov	r2, r8
	ldr	r3, [r2, #32]
	mov	r1, r8
	str	r3, [r0, #8]
	ldr	r3, [pc, #108]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b1a
	movs	r5, #120
	str	r5, [r3, #12]
	str	r5, [r3, #16]
	bl	sub_08014de4
	mov	r0, r8
	mov	r1, sl
	bl	sub_080156e8
	add	r6, sp, #16
	adds	r1, r6, #0
	mov	r0, r9
	bl	sub_08015778
	ldr	r3, [r6, #4]
	ldr	r2, [r6, #0]
	movs	r1, #240
	subs	r2, r5, r2
	subs	r5, r5, r3
	lsls	r5, r5, #8
	adds	r3, r5, #0
	lsls	r1, r1, #15
	lsls	r5, r7, #8
	lsls	r2, r2, #8
	adds	r0, r1, #0
	subs	r5, r5, r7
	str	r7, [sp, #0]
	lsls	r6, r5, #1
	bl	sub_08126548
	movs	r1, #192
	adds	r0, r6, #0
	lsls	r1, r1, #8
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x00ad
	adds	r1, r0, #0
	adds	r2, r5, #0
	adds	r0, r6, #0
	bl	sub_08015768
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03000230
	.4byte 0x03000354
	.2byte 0x11e0
	.2byte 0x0300
.L_08126804:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r2
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r2, [r2, #48]
	lsls	r3, r3, #16
	mov	r8, r2
	movs	r2, #12
	add	r2, r8
	adds	r5, r0, #0
	mov	r9, r1
	adds	r0, r3, #0
	movs	r1, #100
	sub	sp, #40
	mov	sl, r2
	bl	sub_08002054
	mov	r3, sl
	mov	r2, r9
	str	r2, [r3, #4]
	mov	r2, fp
	str	r2, [r3, #8]
	ldr	r2, [pc, #184]
	str	r5, [r3, #0]
	movs	r6, #255
	add	r3, sp, #4
	movs	r5, #0
	lsls	r6, r6, #17
	movs	r1, #192
	str	r5, [r3, #0]
	str	r5, [r3, #4]
	str	r5, [r3, #8]
	adds	r7, r0, #0
	mov	fp, r2
	adds	r0, r6, #0
	lsls	r1, r1, #8
	mov	r9, r3
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x0072
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl	sub_08015768
	bl	sub_08014de4
	mov	r0, sl
	bl	sub_08015128
	mov	r2, r8
	movs	r3, #54
	ldrsh	r0, [r2, r3]
	bl	sub_08015068
	mov	r2, r8
	movs	r3, #52
	ldrsh	r0, [r2, r3]
	bl	sub_08015024
	add	r0, sp, #28
	mov	r1, r8
	str	r5, [r0, #0]
	str	r5, [r0, #4]
	str	r6, [r0, #8]
	ldr	r3, [pc, #104]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b1a
	movs	r5, #120
	str	r5, [r3, #12]
	str	r5, [r3, #16]
	bl	sub_08014de4
	mov	r0, r8
	mov	r1, sl
	bl	sub_080156e8
	add	r6, sp, #16
	adds	r1, r6, #0
	mov	r0, r9
	bl	sub_08015778
	ldr	r3, [r6, #4]
	ldr	r2, [r6, #0]
	movs	r1, #240
	subs	r2, r5, r2
	subs	r5, r5, r3
	lsls	r5, r5, #8
	adds	r3, r5, #0
	lsls	r1, r1, #15
	lsls	r5, r7, #8
	lsls	r2, r2, #8
	adds	r0, r1, #0
	subs	r5, r5, r7
	str	r7, [sp, #0]
	lsls	r6, r5, #1
	bl	sub_08126548
	movs	r1, #192
	adds	r0, r6, #0
	lsls	r1, r1, #8
	mov	lr, fp
	.2byte 0xf800
	.2byte 0x00ad
	adds	r1, r0, #0
	adds	r2, r5, #0
	adds	r0, r6, #0
	bl	sub_08015768
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03000230
	.4byte 0x03000354
	.2byte 0x11e0
	.2byte 0x0300
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	adds	r6, r1, #0
	mov	r8, r2
	bl	sub_0811be3c
	ldr	r5, [r0, #0]
	adds	r0, r6, #0
	bl	sub_0811be3c
	ldr	r3, [r0, #0]
	ldr	r1, [r5, #8]
	ldr	r0, [r3, #8]
	ldr	r4, [r5, #16]
	ldr	r2, [r3, #16]
	adds	r0, r0, r1
	adds	r2, r2, r4
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	asrs	r0, r0, #1
	asrs	r2, r2, #1
	movs	r1, #0
	mov	r3, r8
	bl	.L_08126804
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.2byte 0x0000

.syntax unified
	.thumb
	.set sub_08015778, 0x08015778
	.set sub_08016ca4, 0x08016ca4
	.set sub_0811bd10, 0x0811bd10
	.set sub_0811bdb0, 0x0811bdb0
	.set sub_0811be3c, 0x0811be3c
	.set sub_081280fc, 0x081280fc
	.set sub_08128194, 0x08128194
	.global Func_0811c2b4
	.thumb_func
Func_0811c2b4:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	sub	sp, #12
	mov	r8, r1
	mov	sl, r0
	bl	sub_0811be3c
	ldr	r5, [r0, #0]
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_0811bdb0
	adds	r5, #8
	adds	r6, r0, #0
	bl	sub_0811bd10
	mov	r1, r8
	adds	r0, r5, #0
	bl	sub_08015778
	ldr	r1, [r6, #12]
	ldr	r6, [pc, #44]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c05
	mov	r0, sl
	bl	.L_0811c37c
	adds	r1, r0, #0
	asrs	r1, r1, #16
	adds	r0, r5, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x4642
	ldr	r3, [r2, #4]
	add	sp, #12
	subs	r3, r3, r0
	str	r3, [r2, #4]
	movs	r0, #0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x021c
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r1, #0
	mov	r8, r0
	bl	sub_0811be3c
	ldr	r5, [r0, #0]
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_0811bdb0
	adds	r5, #8
	adds	r6, r0, #0
	bl	sub_0811bd10
	adds	r0, r5, #0
	adds	r1, r7, #0
	bl	sub_08015778
	ldr	r1, [r6, #12]
	ldr	r6, [pc, #56]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c05
	mov	r0, r8
	bl	sub_08016ca4
	movs	r2, #165
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldrh	r0, [r3, #0]
	bl	sub_081280fc
	cmp	r0, #0
	beq.n	.L_0811c362
	adds	r0, r5, #0
	movs	r1, #24
	b.n	.L_0811c366
.L_0811c362:
	adds	r0, r5, #0
	movs	r1, #48
.L_0811c366:
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x687b
	subs	r3, r3, r0
	str	r3, [r7, #4]
	movs	r0, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x021c
	.2byte 0x0300
.L_0811c37c:
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_08016ca4
	movs	r2, #165
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldrh	r0, [r3, #0]
	bl	sub_08128194
	lsls	r0, r0, #24
	lsrs	r3, r0, #8
	cmp	r3, #0
	bne.n	.L_0811c3b6
	adds	r0, r5, #0
	bl	sub_08016ca4
	movs	r2, #165
	lsls	r2, r2, #1
	adds	r3, r0, r2
	ldrh	r0, [r3, #0]
	bl	sub_081280fc
	movs	r3, #192
	lsls	r3, r3, #13
	cmp	r0, #0
	bne.n	.L_0811c3b6
	movs	r3, #192
	lsls	r3, r3, #14
.L_0811c3b6:
	adds	r0, r3, #0
	pop	{r5, pc}
	.align 2, 0

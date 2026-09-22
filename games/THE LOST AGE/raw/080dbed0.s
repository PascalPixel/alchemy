.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08013560, 0x08013560
	.set sub_08020290, 0x08020290
	.global Overlay_080dbed0
Overlay_080dbed0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r1, [sp, #0]
	mov	r9, r3
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r1, #192
	ldr	r5, [r3, #16]
	ldrh	r3, [r3, #0]
	lsls	r1, r1, #8
	adds	r6, r0, #0
	mov	fp, r2
	movs	r7, #0
	cmp	r3, r1
	beq.n	.L_080dbf04
	movs	r1, #0
	bl	sub_08020290
.L_080dbf04:
	ldr	r3, [r5, #8]
	add	r2, sp, #4
	str	r3, [r2, #0]
	movs	r1, #128
	ldr	r3, [r5, #12]
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r2, #4]
	mov	r8, r2
	ldr	r3, [r5, #16]
	str	r3, [r2, #8]
	movs	r2, #0
	mov	sl, r2
.L_080dbf1e:
	mov	r3, r8
	ldr	r5, [r3, #0]
	ldr	r1, [sp, #0]
	subs	r3, r1, r5
	adds	r0, r7, #0
	muls	r0, r3
	movs	r1, #10
	bl	sub_08002054
	adds	r5, r5, r0
	str	r5, [r6, #8]
	mov	r2, r8
	ldr	r5, [r2, #4]
	mov	r1, fp
	subs	r3, r1, r5
	adds	r0, r7, #0
	muls	r0, r3
	movs	r1, #10
	bl	sub_08002054
	adds	r5, r5, r0
	str	r5, [r6, #12]
	mov	r2, r8
	ldr	r5, [r2, #8]
	mov	r1, r9
	subs	r3, r1, r5
	adds	r0, r7, #0
	muls	r0, r3
	movs	r1, #10
	bl	sub_08002054
	movs	r1, #10
	adds	r5, r5, r0
	mov	r0, sl
	str	r5, [r6, #16]
	bl	sub_08002054
	movs	r2, #128
	lsls	r2, r2, #7
	adds	r0, r0, r2
	str	r0, [r6, #24]
	str	r0, [r6, #28]
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #192
	lsls	r3, r3, #8
	adds	r7, #1
	add	sl, r3
	cmp	r7, #10
	ble.n	.L_080dbf1e
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}

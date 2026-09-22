.syntax unified
	.thumb
	.set sub_0803aae4, 0x0803aae4
	.set sub_0803b094, 0x0803b094
	.global Overlay_08041fa4
Overlay_08041fa4:
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	mov	fp, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #60]
	mov	r9, r2
	movs	r2, #152
	movs	r3, #0
	lsls	r2, r2, #5
	mov	r8, r3
	adds	r2, #66
	adds	r6, r5, r2
	mov	r2, r8
	mov	sl, r1
	strh	r2, [r6, #0]
	movs	r1, #1
	bl	sub_0803b094
	ldrh	r3, [r6, #0]
	movs	r1, #244
	lsls	r1, r1, #4
	lsls	r3, r3, #1
	adds	r3, r3, r1
	mov	r2, r8
	strh	r2, [r5, r3]
	ldr	r2, [pc, #24]
	ldrh	r3, [r6, #0]
	adds	r5, r5, r1
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r6, #0]
	adds	r0, r5, #0
	mov	r1, sl
	mov	r2, r9
	mov	r3, fp
	bl	sub_0803aae4
	b.n	.L_08042000
	.2byte 0x01ff
	.2byte 0x0000
.L_08042000:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r3}
	mov	fp, r3
	pop	{r5, r6, pc}

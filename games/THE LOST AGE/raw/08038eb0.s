.syntax unified
	.thumb
	.global Overlay_08038eb0
Overlay_08038eb0:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	movs	r2, #227
	lsls	r2, r2, #4
	adds	r1, r3, r2
	ldr	r0, [r1, #0]
	cmp	r0, #0
	beq.n	.L_08038eda
	ldr	r2, [r0, #0]
	cmp	r2, #0
	bne.n	.L_08038ed4
	movs	r4, #224
	lsls	r4, r4, #4
	adds	r4, #52
	adds	r3, r3, r4
	str	r1, [r3, #0]
.L_08038ed4:
	movs	r3, #0
	str	r2, [r1, #0]
	str	r3, [r0, #0]
.L_08038eda:
	pop	{pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #60]
	movs	r1, #230
	lsls	r1, r1, #3
	adds	r3, r2, r1
	cmp	r0, r3
	bcc.n	.L_08038f06
	movs	r1, #227
	lsls	r1, r1, #4
	adds	r3, r2, r1
	cmp	r0, r3
	bcs.n	.L_08038f06
	adds	r1, #4
	adds	r3, r2, r1
	ldr	r2, [r3, #0]
	str	r0, [r3, #0]
	movs	r3, #0
	str	r0, [r2, #0]
	str	r3, [r0, #0]
.L_08038f06:
	pop	{pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #60]
	movs	r1, #230
	lsls	r1, r1, #3
	adds	r2, r0, r1
	movs	r1, #227
	lsls	r1, r1, #4
	adds	r3, r0, r1
	str	r2, [r3, #0]
	movs	r3, #62
.L_08038f20:
	adds	r1, r2, #0
	adds	r1, #28
	subs	r3, #1
	str	r1, [r2, #0]
	adds	r2, r1, #0
	cmp	r3, #0
	bge.n	.L_08038f20
	movs	r2, #224
	lsls	r2, r2, #4
	movs	r3, #0
	adds	r2, #52
	str	r3, [r1, #0]
	adds	r3, r0, r2
	str	r1, [r3, #0]
	pop	{pc}

.syntax unified
	.thumb
	.set sub_080380c8, 0x080380c8
	.global Func_08104b58
	.thumb_func
Func_08104b58:
	push	{r5, r6, lr}
	adds	r5, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	sub	sp, #4
	adds	r4, r0, #0
	adds	r6, r1, #0
	ldr	r3, [r3, #0]
	cmp	r5, #0
	bne.n	.L_08104b76
	movs	r1, #128
	lsls	r1, r1, #3
	adds	r1, #238
	b.n	.L_08104b7a
.L_08104b76:
	movs	r1, #158
	lsls	r1, r1, #3
.L_08104b7a:
	adds	r3, r3, r1
	ldrh	r0, [r3, #0]
	movs	r1, #128
	str	r2, [sp, #0]
	lsls	r1, r1, #23
	adds	r2, r4, #0
	adds	r3, r6, #0
	bl	sub_080380c8
	cmp	r0, #0
	bne.n	.L_08104b96
	movs	r0, #1
	negs	r0, r0
	b.n	.L_08104ba2
.L_08104b96:
	movs	r3, #0
	strb	r3, [r0, #4]
	strh	r3, [r0, #12]
	movs	r3, #1
	strb	r3, [r0, #5]
	movs	r0, #1
.L_08104ba2:
	add	sp, #4
	pop	{r5, r6, pc}
	.2byte 0x0000

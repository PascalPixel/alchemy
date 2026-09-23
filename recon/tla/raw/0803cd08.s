.syntax unified
	.thumb
	.global Func_0803cd08
	.thumb_func
Func_0803cd08:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #60]
	movs	r4, #152
	lsls	r4, r4, #5
	adds	r4, #108
	ldrh	r3, [r4, r2]
	adds	r5, r0, #0
	movs	r0, #152
	adds	r7, r1, #0
	lsls	r0, r0, #5
	movs	r1, #0
	movs	r6, #0
	mov	ip, r1
	adds	r0, #76
	cmp	r3, r5
	bne.n	.L_0803cd38
	ldr	r6, [r0, r2]
	cmp	r7, #0
	beq.n	.L_0803cd56
	str	r1, [r0, r2]
	strh	r1, [r4, r2]
	b.n	.L_0803cd56
.L_0803cd38:
	adds	r1, #1
	adds	r0, #4
	adds	r4, #2
	cmp	r1, #7
	bhi.n	.L_0803cd56
	ldrh	r3, [r4, r2]
	cmp	r3, r5
	bne.n	.L_0803cd38
	ldr	r6, [r0, r2]
	cmp	r7, #0
	beq.n	.L_0803cd56
	mov	r3, ip
	str	r3, [r0, r2]
	mov	r3, ip
	strh	r3, [r4, r2]
.L_0803cd56:
	adds	r0, r6, #0
	pop	{r5, r6, r7, pc}
	.align 2, 0

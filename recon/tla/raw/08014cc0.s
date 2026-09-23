.syntax unified
	.thumb
	.global Func_08014cc0
	.thumb_func
Func_08014cc0:
	push	{r5, lr}
	movs	r4, #192
	adds	r5, r0, #0
	lsls	r4, r4, #18
	ldr	r0, [r4, r5]
	cmp	r0, #0
	bne.n	.L_08014cf8
	adds	r3, r1, #3
	lsrs	r3, r3, #2
	ldr	r0, [r4, #4]
	lsls	r1, r3, #2
	ldr	r3, [pc, #36]
	adds	r2, r0, r1
	cmp	r2, r3
	bls.n	.L_08014cf4
	ldr	r0, [r4, #0]
	movs	r3, #129
	adds	r1, r0, r1
	lsls	r3, r3, #18
	cmp	r1, r3
	bcc.n	.L_08014cee
	movs	r0, #0
	b.n	.L_08014cf8
.L_08014cee:
	str	r1, [r4, #0]
	str	r0, [r4, r5]
	b.n	.L_08014cf8
.L_08014cf4:
	str	r2, [r4, #4]
	str	r0, [r4, r5]
.L_08014cf8:
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0x6fbf
	.2byte 0x0300
	push	{r5, lr}
	movs	r4, #192
	adds	r5, r0, #0
	lsls	r4, r4, #18
	ldr	r0, [r4, r5]
	cmp	r0, #0
	bne.n	.L_08014d38
	adds	r3, r1, #3
	ldr	r0, [r4, #0]
	lsrs	r3, r3, #2
	lsls	r1, r3, #2
	movs	r3, #129
	adds	r2, r0, r1
	lsls	r3, r3, #18
	cmp	r2, r3
	bcc.n	.L_08014d34
	ldr	r0, [r4, #4]
	ldr	r3, [pc, #24]
	adds	r1, r0, r1
	cmp	r1, r3
	bls.n	.L_08014d2e
	movs	r0, #0
	b.n	.L_08014d38
.L_08014d2e:
	str	r1, [r4, #4]
	str	r0, [r4, r5]
	b.n	.L_08014d38
.L_08014d34:
	str	r2, [r4, #0]
	str	r0, [r4, r5]
.L_08014d38:
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x03006fbf

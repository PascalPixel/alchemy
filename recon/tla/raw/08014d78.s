.syntax unified
	.thumb
	.global Func_08014d78
	.thumb_func
Func_08014d78:
	push	{lr}
	movs	r1, #192
	lsls	r1, r1, #18
	adds	r3, r0, #3
	ldr	r2, [r1, #4]
	lsrs	r3, r3, #2
	ldr	r4, [pc, #32]
	lsls	r0, r3, #2
	adds	r3, r2, r0
	cmp	r3, r4
	bls.n	.L_08014da2
	ldr	r2, [r1, #0]
	movs	r3, #129
	adds	r0, r2, r0
	lsls	r3, r3, #18
	cmp	r0, r3
	bcc.n	.L_08014d9e
	movs	r0, #0
	b.n	.L_08014da6
.L_08014d9e:
	str	r0, [r1, #0]
	b.n	.L_08014da4
.L_08014da2:
	str	r3, [r1, #4]
.L_08014da4:
	adds	r0, r2, #0
.L_08014da6:
	pop	{pc}
	.2byte 0x6fbf
	.2byte 0x0300
	push	{lr}
	movs	r1, #192
	lsls	r1, r1, #18
	adds	r3, r0, #3
	ldr	r2, [r1, #0]
	lsrs	r3, r3, #2
	lsls	r0, r3, #2
	movs	r4, #129
	adds	r3, r2, r0
	lsls	r4, r4, #18
	cmp	r3, r4
	bcc.n	.L_08014dd6
	ldr	r2, [r1, #4]
	ldr	r3, [pc, #20]
	adds	r0, r2, r0
	cmp	r0, r3
	bls.n	.L_08014dd2
	movs	r0, #0
	b.n	.L_08014dda
.L_08014dd2:
	str	r0, [r1, #4]
	b.n	.L_08014dd8
.L_08014dd6:
	str	r3, [r1, #0]
.L_08014dd8:
	adds	r0, r2, #0
.L_08014dda:
	pop	{pc}
	.4byte 0x03006fbf

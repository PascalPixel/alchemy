.syntax unified
	.thumb
	.global Func_08024c70
	.thumb_func
Func_08024c70:
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r1, r5, #0
	adds	r1, #96
	ldrb	r3, [r1, #0]
	cmp	r3, #59
	bls.n	.L_08024c94
	movs	r3, #128
	lsls	r3, r3, #24
	movs	r2, #0
	strb	r2, [r1, #0]
	str	r3, [r5, #56]
	str	r3, [r5, #60]
	str	r3, [r5, #64]
	adds	r3, r5, #0
	adds	r3, #86
	strb	r2, [r3, #0]
	b.n	.L_08024c9e
.L_08024c94:
	adds	r0, r5, #0
	bl	Func_08023f64
	cmp	r0, #0
	beq.n	.L_08024ca8
.L_08024c9e:
	ldrh	r3, [r5, #4]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r5, #4]
	b.n	.L_08024caa
.L_08024ca8:
	movs	r0, #0
.L_08024caa:
	pop	{r5, pc}

.syntax unified
	.thumb
	.set sub_08020138, 0x08020138
	.set sub_080cad84, 0x080cad84
	.global ObjectMotion_ResetTargetsAndVelocity
	.global Func_080d2de4
	.thumb_func
ObjectMotion_ResetTargetsAndVelocity:
Func_080d2de4:
	push	{r5, r6, lr}
	bl	sub_080cad84
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080d2e18
	adds	r1, r6, #0
	adds	r1, #90
	ldrb	r2, [r1, #0]
	movs	r3, #1
	orrs	r3, r2
	strb	r3, [r1, #0]
	bl	sub_08020138
	movs	r3, #128
	lsls	r3, r3, #24
	str	r3, [r6, #56]
	str	r3, [r6, #60]
	str	r3, [r6, #64]
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #86
	str	r5, [r6, #36]
	str	r5, [r6, #40]
	str	r5, [r6, #44]
	strb	r5, [r3, #0]
.L_080d2e18:
	pop	{r5, r6, pc}

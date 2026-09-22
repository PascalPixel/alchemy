.syntax unified
	.thumb
	.global Overlay_08016dd0
Overlay_08016dd0:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	ldr	r4, [pc, #28]
	lsls	r3, r6, #20
	movs	r0, #4
	ands	r0, r6
	lsrs	r6, r3, #23
	movs	r5, #15
	ldrb	r3, [r4, r6]
	adds	r2, r5, #0
	lsls	r2, r0
	ands	r1, r5
	bics	r3, r2
	lsls	r1, r0
	orrs	r3, r1
	strb	r3, [r4, r6]
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x0040
	.2byte 0x0200
	svc	11
	bx	lr
	movs	r0, #0
	svc	25
	bx	lr

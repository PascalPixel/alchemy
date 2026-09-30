.syntax unified
	.thumb
	push	{r5, r6, lr}
	adds	r5, r0, #0
	movs	r2, #4
	ldrsh	r3, [r5, r2]
	ldr	r2, [r5, #0]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r6, [r3, #4]
	adds	r0, r6, #0
	bl	GameFlag_Test
	adds	r3, r5, #0
	adds	r3, #87
	strb	r0, [r3, #0]
	adds	r0, r6, #0
	bl	GameFlag_ClearBit
	ldrh	r3, [r5, #4]
	movs	r0, #1
	adds	r3, #2
	strh	r3, [r5, #4]
	pop	{r5, r6, pc}
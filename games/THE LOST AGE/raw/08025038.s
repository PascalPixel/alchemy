.syntax unified
	.thumb
	.set sub_0802471c, 0x0802471c
	.set sub_08024738, 0x08024738
	.set sub_0802d45c, 0x0802d45c
	.global Overlay_08025038
Overlay_08025038:
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r2, [r5, #12]
	movs	r3, #0
	movs	r1, #0
	bl	sub_0802471c
	adds	r3, r5, #0
	adds	r3, #85
	ldrb	r2, [r3, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0802506e
	adds	r3, r5, #0
	adds	r3, #34
	ldrb	r0, [r3, #0]
	ldr	r2, [r5, #16]
	ldr	r1, [r5, #8]
	bl	sub_0802d45c
	ldr	r3, [r5, #12]
	ldr	r2, [r5, #20]
	str	r0, [r5, #20]
	subs	r3, r3, r2
	adds	r3, r3, r0
	str	r3, [r5, #12]
.L_0802506e:
	ldrh	r3, [r5, #4]
	movs	r0, #1
	adds	r3, #1
	strh	r3, [r5, #4]
	pop	{r5, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r3, #4
	ldrsh	r2, [r5, r3]
	ldr	r3, [r5, #0]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r3, #4
	ldmia	r3!, {r1}
	ldmia	r3!, {r2}
	ldr	r3, [r3, #0]
	bl	sub_08024738
	ldrh	r3, [r5, #4]
	movs	r0, #1
	adds	r3, #4
	strh	r3, [r5, #4]
	pop	{r5, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r3, #4
	ldrsh	r2, [r5, r3]
	ldr	r3, [r5, #0]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r3, #4
	ldmia	r3!, {r1}
	ldr	r2, [r5, #12]
	ldr	r3, [r3, #0]
	bl	sub_08024738
	ldrh	r3, [r5, #4]
	movs	r0, #1
	adds	r3, #3
	strh	r3, [r5, #4]
	pop	{r5, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r3, #4
	ldrsh	r2, [r5, r3]
	ldr	r3, [r5, #0]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r3, #4
	ldmia	r3!, {r2}
	ldr	r1, [r5, #8]
	ldmia	r3!, {r0}
	adds	r1, r1, r2
	ldr	r4, [r3, #0]
	ldr	r2, [r5, #12]
	ldr	r3, [r5, #16]
	adds	r2, r2, r0
	adds	r3, r3, r4
	adds	r0, r5, #0
	bl	sub_08024738
	ldrh	r3, [r5, #4]
	movs	r0, #1
	adds	r3, #4
	strh	r3, [r5, #4]
	pop	{r5, pc}

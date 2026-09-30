.syntax unified
	.thumb
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r3, #4
	ldrsh	r2, [r5, r3]
	ldr	r3, [r5, #0]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	adds	r3, #4
	ldmia	r3!, {r4}
	ldr	r1, [r3, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	cmp	r4, r3
	beq.n	.L_08024d4a
	adds	r0, r5, #0
	adds	r0, #93
	ldrb	r2, [r0, #0]
	lsls	r3, r4, #16
	adds	r2, #1
	strb	r2, [r0, #0]
	lsls	r2, r2, #24
	lsrs	r2, r2, #24
	asrs	r3, r3, #16
	cmp	r2, r3
	bge.n	.L_08024d52
	adds	r0, r5, #0
.L_08024d4a:
	bl	Script_FindLabel
	strh	r0, [r5, #4]
	b.n	.L_08024d5c
.L_08024d52:
	movs	r3, #0
	strb	r3, [r0, #0]
	ldrh	r3, [r5, #4]
	adds	r3, #3
	strh	r3, [r5, #4]
.L_08024d5c:
	movs	r0, #1
	pop	{r5, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r2, #4
	ldrsh	r3, [r5, r2]
	ldr	r2, [r5, #0]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r1, [r3, #4]
	bl	Script_FindLabel
	strh	r0, [r5, #4]
	movs	r0, #1
	pop	{r5, pc}
	.2byte 0x0000

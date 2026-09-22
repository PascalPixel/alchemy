.syntax unified
	.thumb
	.global Overlay_08022b80
Overlay_08022b80:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	ldrb	r4, [r5, #27]
	movs	r0, #0
	cmp	r0, r4
	bge.n	.L_08022ba8
	lsls	r6, r1, #4
	adds	r1, r5, #0
	adds	r1, #40
.L_08022b92:
	ldmia	r1!, {r2}
	cmp	r2, #0
	beq.n	.L_08022ba2
	ldr	r3, [r2, #12]
	cmp	r3, #0
	beq.n	.L_08022ba2
	strh	r6, [r2, #2]
	ldrb	r4, [r5, #27]
.L_08022ba2:
	adds	r0, #1
	cmp	r0, r4
	blt.n	.L_08022b92
.L_08022ba8:
	movs	r0, #0
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	adds	r5, r0, #0
	ldrb	r4, [r5, #27]
	movs	r0, #0
	adds	r6, r1, #0
	cmp	r0, r4
	bge.n	.L_08022bd4
	adds	r1, r5, #0
	adds	r1, #40
.L_08022bbe:
	ldmia	r1!, {r2}
	cmp	r2, #0
	beq.n	.L_08022bce
	ldr	r3, [r2, #12]
	cmp	r3, #0
	beq.n	.L_08022bce
	strb	r6, [r2, #21]
	ldrb	r4, [r5, #27]
.L_08022bce:
	adds	r0, #1
	cmp	r0, r4
	blt.n	.L_08022bbe
.L_08022bd4:
	pop	{r5, r6, pc}

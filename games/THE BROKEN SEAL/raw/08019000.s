.syntax unified
	.thumb
	.global Overlay_08019000
Overlay_08019000:
	push	{r5, r6, r7, lr}
	adds	r4, r3, #0
	ldr	r3, [pc, #128]
	ldr	r6, [r3, #0]
	ldrh	r3, [r0, #10]
	adds	r7, r1, #0
	adds	r4, #1
	adds	r1, r2, #0
	subs	r3, #1
	ldr	r2, [sp, #16]
	adds	r1, #1
	cmp	r4, r3
	bhi.n	.L_08019082
	ldrh	r3, [r0, #8]
	subs	r3, #1
	cmp	r1, r3
	bhi.n	.L_08019082
	movs	r5, #240
	lsls	r5, r5, #8
	cmp	r2, #3
	beq.n	.L_08019042
	cmp	r2, #3
	bhi.n	.L_08019038
	movs	r5, #224
	lsls	r5, r5, #8
	cmp	r2, #2
	beq.n	.L_08019042
	b.n	.L_08019040
.L_08019038:
	movs	r5, #128
	lsls	r5, r5, #5
	cmp	r2, #4
	beq.n	.L_08019042
.L_08019040:
	movs	r5, #0
.L_08019042:
	cmp	r2, #1
	beq.n	.L_08019082
	cmp	r2, #1
	bcc.n	.L_0801906a
	cmp	r2, #4
	bhi.n	.L_0801906a
	ldrh	r2, [r0, #14]
	ldrh	r3, [r0, #12]
	adds	r2, r2, r4
	adds	r3, r3, r1
	lsls	r2, r2, #5
	adds	r0, r2, r3
	movs	r3, #160
	lsls	r3, r3, #2
	cmp	r0, r3
	bcs.n	.L_08019082
	lsls	r3, r0, #1
	orrs	r5, r7
	strh	r5, [r6, r3]
	b.n	.L_08019082
.L_0801906a:
	ldrh	r2, [r0, #14]
	ldrh	r3, [r0, #12]
	adds	r2, r2, r4
	adds	r3, r3, r1
	lsls	r2, r2, #5
	adds	r0, r2, r3
	movs	r3, #160
	lsls	r3, r3, #2
	cmp	r0, r3
	bcs.n	.L_08019082
	lsls	r3, r0, #1
	strh	r7, [r6, r3]
.L_08019082:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
